#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/trampoline.h>

#include "../../nvme_glue_helper.h"
#include "../nvme_callee.h"
#include <asm/cacheflush.h>
#include <linux/atomic.h>
#include <linux/priv_mempool.h>
#include <linux/hdreg.h>
#include <linux/sort.h>
#include <linux/aer.h>
#include <lcd_domains/microkernel.h>

#include <lcd_config/post_hook.h>

//#define HANDLE_IRQ_LOCALLY
#define CHRDEV_NAME	"nvme_lcd"

#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(file_hash, CPTR_HASH_BITS);
static DEFINE_HASHTABLE(dev_table, CPTR_HASH_BITS);
static DEFINE_HASHTABLE(bdev_table, CPTR_HASH_BITS);
static DEFINE_HASHTABLE(pdev_hash, CPTR_HASH_BITS);

priv_pool_t *iocb_pool;
static unsigned long pool_pfn_start, pool_pfn_end;
void *pool_base = NULL;
size_t pool_size = 0;

/* hacks for unregistering */
int null_major;
struct gendisk *disk_g;
struct request_queue *rq_g;
struct blk_mq_tag_set *set_g;

struct pci_dev *g_pdev = NULL;
#define DRVNAME	"nvme_lcd"

struct blk_mq_ops_container *g_blk_mq_ops_container;

/* hack for init_request */
int hw_depth;
static struct glue_cspace *c_cspace;

#define IOCB_POOL_SIZE		(256UL << 20)
#define IOCB_POOL_PAGES		(IOCB_POOL_SIZE >> PAGE_SHIFT)
#define IOCB_POOL_ORDER		ilog2(IOCB_POOL_PAGES)
#define QUEUE_RQ_BUF_SIZE	4096

/* Create a shadow copy of the user data into LCDs */
//#define CONFIG_COPY_USER_DATA
/*
 * Abhi says that the max limit on io_vec is 256 entries (with each one
 * spanning a whole page, amounting to 1M  data per bio)
 */
#define MAX_BVECS		256
struct bio_vec_queue {
	struct bio_vec bvec_array[MAX_BVECS];
};

DEFINE_PER_CPU(struct bio_vec_queue, bio_vec_queues);

#define PCI_CLASS_STORAGE_EXPRESS	0x010802
static const struct pci_device_id nvme_id_table[] = {
	{ PCI_DEVICE_CLASS(PCI_CLASS_STORAGE_EXPRESS, 0xffffff) },
	{ 0 },
};


void iocb_data_pool_init(void)
{
	pool_base = vzalloc(IOCB_POOL_SIZE);
	pool_size = IOCB_POOL_SIZE;
	iocb_pool = priv_pool_init(BLK_USER_BUF_POOL, (void*) pool_base,
			pool_size, QUEUE_RQ_BUF_SIZE); }

void iocb_data_pool_free(void)
{
	if (iocb_pool) {
		vfree(iocb_pool->pool);
		priv_pool_destroy(iocb_pool);
	}
}

bool is_in_range(void *mem) {
	if ((mem < pool_base) || (mem > (pool_base + pool_size))) {
		return false;
	}
	return true;
}

int glue_nvme_init(void)
{
	int ret;
	ret = glue_cap_init();
	if (ret) {
		LIBLCD_ERR("cap init");
		goto fail1;
	}
	ret = glue_cap_create(&c_cspace);
	if (ret) {
		LIBLCD_ERR("cap create");
		goto fail2;
	}

	iocb_data_pool_init();
	return 0;
fail2:
	glue_cap_exit();
fail1:
	return ret;

}

void glue_nvme_exit(void)
{
	glue_cap_destroy(c_cspace);
	glue_cap_exit();
	iocb_data_pool_free();
}

int glue_insert_file(struct file_ptr_container *file_c)
{
	BUG_ON(!file_c->file);

	file_c->my_ref = __cptr((unsigned long)file_c->file);

	hash_add(file_hash, &file_c->hentry, (unsigned long) file_c->file);

	return 0;
}

int glue_lookup_file(struct cptr c, struct file_ptr_container **file_cout) {
	struct file_ptr_container *file_c;

	hash_for_each_possible(file_hash, file_c,
			hentry, (unsigned long) cptr_val(c)) {
		if (file_c->file == (struct file*) c.cptr) {
			*file_cout = file_c;
			goto exit;
		}
	}
exit:
	return 0;
}

void glue_remove_file(struct file_ptr_container *file_c)
{
	hash_del(&file_c->hentry);
}

int glue_insert_device(struct device_container *dev_c)
{
	BUG_ON(!dev_c->dev);

	dev_c->my_ref = __cptr((unsigned long)dev_c->dev);

	hash_add(dev_table, &dev_c->hentry, (unsigned long) dev_c->dev);

	return 0;
}

int glue_lookup_device(struct cptr c, struct
		device_container **dev_cout) {
	struct device_container *dev_c;

	hash_for_each_possible(dev_table, dev_c,
			hentry, (unsigned long) cptr_val(c)) {
		if (dev_c->dev == (struct device*) c.cptr)
			*dev_cout = dev_c;
	}
	return 0;
}

void glue_remove_device(struct device_container *dev_c)
{
	hash_del(&dev_c->hentry);
}

int inline glue_insert_pdev_hash(struct pci_dev_container *dev_container)
{
	BUG_ON(!dev_container->pdev);

	dev_container->my_ref = __cptr((unsigned long)dev_container->pdev);

	hash_add(pdev_hash, &dev_container->hentry,
				(unsigned long) dev_container->pdev);

	return 0;
}

int inline glue_lookup_pdev_hash(struct cptr c, struct pci_dev_container
		**dev_container_out)
{
        struct pci_dev_container *dev_container;

        hash_for_each_possible(pdev_hash, dev_container,
				hentry, (unsigned long) cptr_val(c)) {
		if (!dev_container) {
			WARN_ON(!dev_container);
			continue;
		}
		if (dev_container->pdev == (struct pci_dev*) c.cptr) {
	                *dev_container_out = dev_container;
			break;
		}
        }
        return 0;
}

void inline glue_remove_pdev_hash(struct pci_dev_container *dev_container)
{
	hash_del(&dev_container->hentry);
}

int glue_insert_bdevice(struct block_device_container *bdev_c)
{
	BUG_ON(!bdev_c->bdev);

	bdev_c->my_ref = __cptr((unsigned long)bdev_c->bdev);

	hash_add(bdev_table, &bdev_c->hentry, (unsigned long) bdev_c->bdev);

	return 0;
}

int glue_lookup_bdevice(struct cptr c, struct
		block_device_container **bdev_cout) {
	struct block_device_container *bdev_c;

	hash_for_each_possible(bdev_table, bdev_c,
			hentry, (unsigned long) cptr_val(c)) {
		if (bdev_c->bdev == (struct block_device*) c.cptr)
			*bdev_cout = bdev_c;
	}
	return 0;
}

void glue_remove_bdevice(struct block_device_container *bdev_c)
{
	hash_del(&bdev_c->hentry);
}

int blk_mq_init_queue_callee(struct fipc_message *request)
{
	struct request_queue *rq;
	int ret = 0;
	struct blk_mq_tag_set_container *set_container;
        struct request_queue_container *rq_container;

	ret = glue_cap_lookup_blk_mq_tag_set_type(c_cspace,
			__cptr(fipc_get_reg0(request)), &set_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	rq = blk_mq_init_queue(&set_container->tag_set);

	if (!rq) {
		LIBLCD_ERR("blk layer returned bad address!");
		goto fail_blk;
	}

	/* Hack for remove */
	rq_g = rq;

	rq_container = container_of(rq, struct request_queue_container,
			request_queue);

	ret = glue_cap_insert_request_queue_type(c_cspace, rq_container,
					&rq_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	printk("%s, request_queue %p | cptr: %lx\n", __func__, rq,
						rq_container->my_ref.cptr);
	rq_container->other_ref.cptr = fipc_get_reg1(request);

	fipc_set_reg0(request, rq_container->my_ref.cptr);

	return ret;

fail_blk:
fail_lookup:
fail_insert:
	return ret;
}

int blk_get_queue_callee(struct fipc_message *_request)
{
	struct request_queue_container *rq_container;
	int func_ret;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &rq_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	func_ret = blk_get_queue(&rq_container->request_queue);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int blk_mq_end_request_callee(struct fipc_message *_request)
{
	struct request *rq;
	int ret = 0;
	struct request_queue_container *q_container;
	struct request_queue *q;
	int tag;
	int error;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	tag = fipc_get_reg1(_request);
	error = fipc_get_reg2(_request);

	rq = blk_mq_get_rq_from_tag(q, tag);

	printk("%s, rq: %p\n", __func__, rq);
	blk_mq_end_request(rq, error);

fail_lookup:
	return ret;
}

int blk_mq_update_nr_hw_queues_callee(struct fipc_message *request)
{
	struct blk_mq_tag_set_container *set_container;
	int ret;
	int nr_hw_queues;

	ret = glue_cap_lookup_blk_mq_tag_set_type(c_cspace,
			__cptr(fipc_get_reg0(request)), &set_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	nr_hw_queues = fipc_get_reg1(request);
	blk_mq_update_nr_hw_queues(&set_container->tag_set, nr_hw_queues);

fail_lookup:
	return ret;
}

int blk_mq_free_tag_set_callee(struct fipc_message *request)
{
	struct blk_mq_tag_set_container *set_container;
	struct blk_mq_ops_container *ops_container;
	int ret;

	ret = glue_cap_lookup_blk_mq_tag_set_type(c_cspace,
			__cptr(fipc_get_reg0(request)), &set_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	ret = glue_cap_lookup_blk_mq_ops_type(c_cspace,
			__cptr(fipc_get_reg1(request)), &ops_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	blk_mq_free_tag_set(&set_container->tag_set);

	glue_cap_remove(c_cspace, set_container->my_ref);
	glue_cap_remove(c_cspace, ops_container->my_ref);
	kfree(ops_container);
	kfree(set_container);

	return ret;

fail_lookup:
	return ret;
}

int blk_mq_start_request_callee(struct fipc_message *_request)
{

	struct request *rq;
	int ret = 0;
	struct request_queue_container *q_container;
	struct request_queue *q;
	int tag;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	tag = fipc_get_reg1(_request);

	rq = blk_mq_get_rq_from_tag(q, tag);

	assert(rq != NULL);
	printk("%s, rq: %p\n", __func__, rq);
	if (rq)
		blk_mq_start_request(rq);

fail_lookup:
	return ret;
}

int blk_put_queue_callee(struct fipc_message *request)
{
	struct request_queue_container *rq_container;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(request)), &rq_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	blk_put_queue(&rq_container->request_queue);

	glue_cap_remove(c_cspace, rq_container->my_ref);

fail_lookup:
	return ret;
}

int blk_mq_start_stopped_hw_queues_callee(struct fipc_message *request)
{
	struct request_queue_container *rq_container;
	int ret;
	bool async;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(request)), &rq_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	async = fipc_get_reg1(request);

	blk_mq_start_stopped_hw_queues(&rq_container->request_queue, async);

fail_lookup:
	return ret;
}

int blk_mq_complete_request_callee(struct fipc_message *_request)
{
	struct request *rq;
	int ret = 0;
	struct request_queue_container *q_container;
	struct request_queue *q;
	int tag;
	int error;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	tag = fipc_get_reg1(_request);
	error = fipc_get_reg0(_request);

	rq = blk_mq_get_rq_from_tag(q, tag);

	printk("%s, rq: %p\n", __func__, rq);
	blk_mq_complete_request(rq, error);

fail_lookup:
	return ret;
}

int blk_mq_map_queue_callee(struct fipc_message *request)
{
#if 0
	struct request_queue *rq;
	int ctx_index;
	int ret = 0;
	struct blk_mq_hw_ctx *func_ret;
#ifndef CONFIG_LVD
	unsigned 	long mem_order;
	unsigned 	long driver_data_offset;
	cptr_t driver_data_cptr;
	gva_t driver_data_gva;
#endif
	/* TODO: sync wrapper */
	rq = kzalloc(sizeof( *rq ), GFP_KERNEL);
	if (!rq) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ctx_index = fipc_get_reg2(request);
	func_ret = kzalloc(sizeof( *func_ret ), GFP_KERNEL);
	if (!func_ret) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = blk_mq_map_queue(rq, ctx_index);

	fipc_set_reg3(request, (u64) func_ret->driver_data);

fail_alloc:
	return ret;
#endif
	return 0;
}

int blk_queue_logical_block_size_callee(struct fipc_message *request)
{
	unsigned short size;
	struct request_queue_container *q_container;
	struct request_queue *q;
	int ret;

	size = fipc_get_reg0(request);
	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg1(request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	blk_queue_logical_block_size(q, size);
	fipc_set_reg0(request, q->limits.logical_block_size);
	fipc_set_reg1(request, q->limits.physical_block_size);
	fipc_set_reg2(request, q->limits.io_min);

fail_lookup:
	return ret;
}

int blk_mq_tagset_busy_iter_callee(struct fipc_message *_request)
{
	LCD_MSG("%s, called", __func__);
	return 0;
}

int blk_queue_physical_block_size_callee(struct fipc_message *request)
{
	unsigned int size;
	struct request_queue_container *rq_container;
	int ret;

	size = fipc_get_reg0(request);
	ret = glue_cap_lookup_request_queue_type(c_cspace,
				__cptr(fipc_get_reg1(request)),
				&rq_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	blk_queue_physical_block_size(&rq_container->request_queue, size);
fail_lookup:
	return ret;
}


int alloc_disk_node_callee(struct fipc_message *request)
{
	int minors;
	int node_id;
	int ret = 0;
	struct gendisk *disk;
	struct gendisk_container *disk_container;

	minors = fipc_get_reg0(request);
	node_id = fipc_get_reg1(request);

	disk = alloc_disk_node(minors, node_id);

	if (!disk) {
		LIBLCD_ERR("call to alloc_disk_node failed");
		goto fail_alloc;
	}

	printk("%s disk: %p \n", __func__, disk);

	disk_container = container_of(disk, struct gendisk_container, gendisk);

	/* Hack for remove */
	disk_g = disk;

	ret = glue_cap_insert_gendisk_type(c_cspace, disk_container,
				&disk_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	disk_container->other_ref.cptr = fipc_get_reg2(request);
	fipc_set_reg0(request, disk_container->my_ref.cptr);

fail_insert:
fail_alloc:
	return ret;
}


int put_disk_callee(struct fipc_message *request)
{
	struct gendisk *disk;
	struct gendisk_container *disk_container;
	struct block_device_operations_container *blo_container;
	int ret;

        ret = glue_cap_lookup_gendisk_type(c_cspace, __cptr(fipc_get_reg0(request)),
                                        &disk_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	ret = glue_cap_lookup_blk_dev_ops_type(c_cspace, __cptr(fipc_get_reg1(request)),
                                        &blo_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	disk = &disk_container->gendisk;
 	/* disk_container may be deleted after the call to put_disk,
	 * so remove from cspace here */
	glue_cap_remove(c_cspace, disk_container->my_ref);

	put_disk(disk);

	glue_cap_remove(c_cspace, blo_container->my_ref);
	kfree(blo_container);

fail_lookup:
	return ret;
}

int del_gendisk_callee(struct fipc_message *request)
{
	struct gendisk *disk;
	struct gendisk_container *disk_container;
	int ret;

        ret = glue_cap_lookup_gendisk_type(c_cspace, __cptr(fipc_get_reg0(request)),
                                        &disk_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	disk = &disk_container->gendisk;

	printk("Calling del_gendisk on cpu: %d, disk %p \n",
			raw_smp_processor_id(), disk);

	del_gendisk(disk);

fail_lookup:
	return ret;
}

int register_blkdev_callee(struct fipc_message *request)
{
	int ret;
	int major;
	unsigned int devno;

	devno = fipc_get_reg0(request);

	ret = register_blkdev(devno, "nvme");

	printk("register_blkdev returns %d\n", ret);

	if (ret < 0) {
		LIBLCD_ERR("register_blkdev failed! ret = %d", ret);
		goto fail;
	}

	null_major = major = ret;
	fipc_set_reg0(request, major);
fail:
        return 0;
}

int unregister_blkdev_callee(struct fipc_message *request)
{
	unsigned int devno;
	int ret = 0;

	devno = fipc_get_reg0(request);
	unregister_blkdev(devno, "nvme");
	return ret;
}

int blk_cleanup_queue_callee(struct fipc_message *request)
{
	struct request_queue_container *rq_container;
	int ret = 0;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(request)),
			&rq_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	blk_cleanup_queue(&rq_container->request_queue);

fail_lookup:
	return ret;
}

int _queue_rq_fn(struct blk_mq_hw_ctx *ctx, const struct blk_mq_queue_data *bd,
		struct trampoline_hidden_args *hidden_args)
{
        int ret;
	struct fipc_message r;
        struct fipc_message *request = &r;
	union rq_pack pack;
	struct blk_mq_ops_container *ops_container =
		hidden_args->struct_container;
        struct blk_mq_hw_ctx_container *ctx_container;
	void *lcd_buf[MAX_RQ_BUFS] = {0};
	struct ext_registers *this_reg_page = get_register_page(smp_processor_id());

	INIT_FIPC_MSG(request);
        /*XXX Beware!! hwctx can be unique per hw context of the driver, if multiple
         * exists, then we need one cspace insert function per hwctx. Should be handled
         * in the init_hctx routine */

        ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container,
					blk_mq_hw_ctx);

	pack.qnum = ctx->queue_num;
	pack.cmd_typ = bd->rq->cmd_type;
	pack.tag = bd->rq->tag;
	pack.rq_bytes = blk_rq_bytes(bd->rq);

        async_msg_set_fn_type(request, QUEUE_RQ_FN);

        fipc_set_reg0(request, pack.reg);
        fipc_set_reg1(request, ctx_container->other_ref.cptr);
        fipc_set_reg2(request, ops_container->other_ref.cptr);

	if (blk_rq_bytes(bd->rq)) {
		printk("%s rq has %d bytes\n", __func__, blk_rq_bytes(bd->rq));
		//dump_stack();
	}

	if (blk_rq_bytes(bd->rq)) {
		struct req_iterator iter;
		struct bio_vec bvec;
		int i = 0;

		rq_for_each_segment(bvec, bd->rq, iter) {
			void *buf = page_address(bvec.bv_page);
			lcd_buf[i] = priv_alloc(BLK_USER_BUF_POOL);
			printk("%s, pool_base: %p alloc from priv[%d]: %p offset: 0x%lx\n",
					__func__, pool_base,
					smp_processor_id(),
					lcd_buf[i],
					(unsigned long)(lcd_buf[i] - pool_base));
			if (lcd_buf[i] && is_in_range(lcd_buf[i]))
				memcpy(lcd_buf[i], buf + bvec.bv_offset, bvec.bv_len);
			else
				LIBLCD_ERR("%s Allocated buffer not in range", __func__);
			//print_hex_dump(KERN_INFO, "i/o:", DUMP_PREFIX_OFFSET, 32, 1,
			//		lcd_buf, bvec.bv_len, true);
			//memset(lcd_buf, 0x0, bvec.bv_len);
			this_reg_page->regs[i] = (unsigned long)(lcd_buf[i] - pool_base);
			i++;
		}
	}

	printk("%s, rq: %p\n", __func__, bd->rq);
	vmfunc_klcd_wrapper(request, 1);

	if (blk_rq_bytes(bd->rq)) {
		struct req_iterator iter;
		struct bio_vec bvec;
		int i = 0;

		rq_for_each_segment(bvec, bd->rq, iter) {
			void *buf = page_address(bvec.bv_page);
			if (lcd_buf[i]) {
				memcpy(buf + bvec.bv_offset, lcd_buf[i], bvec.bv_len);
				priv_free(lcd_buf[i], BLK_USER_BUF_POOL);
			}
		}
	}

        ret = fipc_get_reg0(request);

        return ret;
}

int _init_request_fn(void *data, struct request *rq, unsigned int hctx_idx,
			unsigned int rq_idx,
			unsigned int numa_node,
			struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	int func_ret = 0;
	struct blk_mq_ops_container *ops_container =
		hidden_args->struct_container;

	async_msg_set_fn_type(request, INIT_REQUEST_FN);
	fipc_set_reg0(request, rq->tag);
	fipc_set_reg1(request, hctx_idx);
	fipc_set_reg2(request, rq_idx);
	fipc_set_reg3(request, numa_node);

	/* ops container is passed to call ops.init_hctx in the LCD glue */
	fipc_set_reg4(request, ops_container->other_ref.cptr);

	vmfunc_klcd_wrapper(request, 1);

	func_ret = fipc_get_reg0(request);

	return 0;
}

LCD_TRAMPOLINE_DATA(init_request_fn_trampoline);
int LCD_TRAMPOLINE_LINKAGE(init_request_fn_trampoline)
init_request_fn_trampoline(void *data, struct request *rq, unsigned int hctx_idx,
			unsigned int rq_idx, unsigned int numa_node)
{
	int ( *volatile init_request_fn_fp )(void *, struct request*, unsigned
			int, unsigned int, unsigned int,
			struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, init_request_fn_trampoline);
	init_request_fn_fp = _init_request_fn;
	return init_request_fn_fp(data, rq, hctx_idx, rq_idx, numa_node, hidden_args);
}


LCD_TRAMPOLINE_DATA(queue_rq_fn_trampoline);
int LCD_TRAMPOLINE_LINKAGE(queue_rq_fn_trampoline)
queue_rq_fn_trampoline(struct blk_mq_hw_ctx *ctx, const struct blk_mq_queue_data *bd)
{
	int ( *volatile queue_rq_fn_fp )(struct blk_mq_hw_ctx *, const struct blk_mq_queue_data *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, queue_rq_fn_trampoline);
	queue_rq_fn_fp = _queue_rq_fn;
	return queue_rq_fn_fp(ctx, bd, hidden_args);

}

struct blk_mq_hw_ctx *_map_queue_fn(struct request_queue *rq, int m, struct trampoline_hidden_args *hidden_args)
{
	/* In kernel v4.9, this function is no longer registered
	 * in the ops field. Because the kernel itself handles
	 * the map. All the kernel does is to call blk_mq_map_queue.
	 * So instead of going to the LCD, I am going to call
	 * blk_mq_map_queue here! */
	/* TODO: Call to LCD and call this function from there */
	return blk_mq_map_queue(rq, m);
}

LCD_TRAMPOLINE_DATA(map_queue_fn_trampoline);
struct blk_mq_hw_ctx *LCD_TRAMPOLINE_LINKAGE(map_queue_fn_trampoline) map_queue_fn_trampoline(struct request_queue *req_queue, int m)

{
	struct blk_mq_hw_ctx* ( *volatile map_queue_fn_fp )(struct
			request_queue *, int , struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, map_queue_fn_trampoline);
	map_queue_fn_fp = _map_queue_fn;
	return map_queue_fn_fp(req_queue, m, hidden_args);
}

int _init_hctx_fn(struct blk_mq_hw_ctx *ctx, void *data, unsigned int index, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;
	int func_ret = 0;
	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container =
		hidden_args->struct_container;

	ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container,
			blk_mq_hw_ctx);

	ops_container = (struct blk_mq_ops_container
			*)hidden_args->struct_container;

	ret = glue_cap_insert_blk_mq_hw_ctx_type(c_cspace, ctx_container,
			&ctx_container->my_ref);
	if (ret) {
		LIBLCD_ERR("klcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(request, INIT_HCTX_FN);
	fipc_set_reg0(request, ops_container->other_ref.cptr);
	fipc_set_reg1(request, ctx_container->my_ref.cptr);
	fipc_set_reg2(request, index);

	vmfunc_klcd_wrapper(request, 1);

	ctx_container->other_ref.cptr = fipc_get_reg0(request);
	func_ret = fipc_get_reg1(request);

	printk("init_hctx hctx %p | cptr %lu\n", ctx, ctx_container->other_ref.cptr);

fail_insert:
	return func_ret;
}

LCD_TRAMPOLINE_DATA(init_hctx_fn_trampoline);
int LCD_TRAMPOLINE_LINKAGE(init_hctx_fn_trampoline) init_hctx_fn_trampoline(struct blk_mq_hw_ctx *ctx, void *data, unsigned int index)

{
	int ( *volatile init_hctx_fn_fp )(struct blk_mq_hw_ctx *, void *, unsigned int, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, init_hctx_fn_trampoline);
	init_hctx_fn_fp = _init_hctx_fn;
	return init_hctx_fn_fp(ctx, data, index, hidden_args);

}

int _exit_hctx_fn(struct blk_mq_hw_ctx *ctx, unsigned int index, struct
		trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container = hidden_args->struct_container;

	ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container,
			blk_mq_hw_ctx);
	ops_container = (struct blk_mq_ops_container
			*)hidden_args->struct_container;

	async_msg_set_fn_type(request, EXIT_HCTX_FN);
	fipc_set_reg0(request, ops_container->other_ref.cptr);
	fipc_set_reg1(request, ctx_container->other_ref.cptr);
	fipc_set_reg2(request, index);

	vmfunc_klcd_wrapper(request, 1);

	printk("exit_hctx hctx %p | cptr %lu\n", ctx, ctx_container->other_ref.cptr);

	return 0;
}

LCD_TRAMPOLINE_DATA(exit_hctx_fn_trampoline);
int LCD_TRAMPOLINE_LINKAGE(exit_hctx_fn_trampoline) exit_hctx_fn_trampoline(struct blk_mq_hw_ctx *ctx, unsigned int index)

{
	int ( *volatile exit_hctx_fn_fp )(struct blk_mq_hw_ctx *, unsigned int,
					struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, exit_hctx_fn_trampoline);
	exit_hctx_fn_fp = _exit_hctx_fn;
	return exit_hctx_fn_fp(ctx, index, hidden_args);
}

void _complete_fn(struct request *rq,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct blk_mq_ops_container *ops_container = hidden_args->struct_container;

	ops_container = (struct blk_mq_ops_container
			*)hidden_args->struct_container;

	async_msg_set_fn_type(_request, COMPLETE_FN);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, rq->tag);
	fipc_set_reg2(_request, get_queue_num(rq));
	vmfunc_klcd_wrapper(_request, 1);

	return;
}

LCD_TRAMPOLINE_DATA(complete_fn_trampoline);
void LCD_TRAMPOLINE_LINKAGE(complete_fn_trampoline) complete_fn_trampoline(struct request *request)

{
	void ( *volatile complete_fn_fp )(struct request *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, complete_fn_trampoline);
	complete_fn_fp = _complete_fn;
	return complete_fn_fp(request, hidden_args);

}

void _timeout_fn(struct request *request, bool reserved,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	async_msg_set_fn_type(_request, TIMEOUT_FN);
	fipc_set_reg1(_request, reserved);

	vmfunc_klcd_wrapper(_request, 1);

	return;
}

LCD_TRAMPOLINE_DATA(timeout_fn_trampoline);
void LCD_TRAMPOLINE_LINKAGE(timeout_fn_trampoline) timeout_fn_trampoline(struct request
			*request, bool reserved)

{
	void ( *volatile timeout_fn_fp )(struct request *, bool,
				struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, timeout_fn_trampoline);
	timeout_fn_fp = _timeout_fn;
	return timeout_fn_fp(request, reserved, hidden_args);
}

void _poll_fn(struct blk_mq_hw_ctx *ctx, unsigned int tag,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container =
		hidden_args->struct_container;

	ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container,
			blk_mq_hw_ctx);

	async_msg_set_fn_type(_request, POLL_FN);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, ctx_container->other_ref.cptr);
	fipc_set_reg2(_request, tag);

	vmfunc_klcd_wrapper(_request, 1);

	return;
}

LCD_TRAMPOLINE_DATA(poll_fn_trampoline);
void LCD_TRAMPOLINE_LINKAGE(poll_fn_trampoline) poll_fn_trampoline(struct
		blk_mq_hw_ctx *ctx, unsigned int tag)

{
	void ( *volatile poll_fn_fp )(struct blk_mq_hw_ctx *, unsigned int,
				struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, poll_fn_trampoline);
	poll_fn_fp = _poll_fn;
	return poll_fn_fp(ctx, tag, hidden_args);
}

void setup_blk_mq_ops(struct blk_mq_ops_container *ops_container)
{
	struct trampoline_hidden_args *queue_rq_hidden_args;
	struct trampoline_hidden_args *complete_hidden_args;
	struct trampoline_hidden_args *map_queue_hidden_args;
	struct trampoline_hidden_args *init_hctx_hidden_args;
	struct trampoline_hidden_args *exit_hctx_hidden_args;
	struct trampoline_hidden_args *init_request_hidden_args;
	struct trampoline_hidden_args *timeout_hidden_args;
	struct trampoline_hidden_args *poll_hidden_args;
	int err;

	/* Setup function pointers and trampolines */
	queue_rq_hidden_args = kzalloc(sizeof(*queue_rq_hidden_args), GFP_KERNEL);
	if (!queue_rq_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc1;
	}
	queue_rq_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(queue_rq_fn_trampoline);
	if (!queue_rq_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	queue_rq_hidden_args->t_handle->hidden_args = queue_rq_hidden_args;
	queue_rq_hidden_args->struct_container = ops_container;
	ops_container->blk_mq_ops.queue_rq =
		LCD_HANDLE_TO_TRAMPOLINE(queue_rq_hidden_args->t_handle);

	err = set_memory_x(((unsigned long)queue_rq_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(queue_rq_fn_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (err) {
                LIBLCD_ERR("set mem nx");
                goto fail_x1;
        }

	/* complete */
	complete_hidden_args = kzalloc(sizeof( *complete_hidden_args ), GFP_KERNEL);
	if (!complete_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc2;
	}
	complete_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(complete_fn_trampoline);
	if (!complete_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup2;
	}
	complete_hidden_args->t_handle->hidden_args = complete_hidden_args;
	complete_hidden_args->struct_container = ops_container;
	ops_container->blk_mq_ops.complete =
		LCD_HANDLE_TO_TRAMPOLINE(complete_hidden_args->t_handle);

        err = set_memory_x(((unsigned long)complete_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(complete_fn_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (err) {
                LIBLCD_ERR("set mem nx");
                goto fail_x2;
        }

	/* map_queue */
	map_queue_hidden_args = kzalloc(sizeof( *map_queue_hidden_args ), GFP_KERNEL);
	if (!map_queue_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc3;
	}
	map_queue_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(map_queue_fn_trampoline);
	if (!map_queue_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup3;
	}
	map_queue_hidden_args->t_handle->hidden_args = map_queue_hidden_args;
	map_queue_hidden_args->struct_container = ops_container;
	ops_container->blk_mq_ops.map_queue =
		LCD_HANDLE_TO_TRAMPOLINE(map_queue_hidden_args->t_handle);
	printk("map_queue in setup %p \n",ops_container->blk_mq_ops.map_queue);

	err = set_memory_x(((unsigned long)map_queue_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(map_queue_fn_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
	if (err) {
		LIBLCD_ERR("set mem nx");
		goto fail_x3;
	}

	/* init_hctx */
	init_hctx_hidden_args = kzalloc(sizeof( *init_hctx_hidden_args ), GFP_KERNEL);
	if (!init_hctx_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc4;
	}
	init_hctx_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(init_hctx_fn_trampoline);
	if (!init_hctx_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup4;
	}
	init_hctx_hidden_args->t_handle->hidden_args = init_hctx_hidden_args;
	init_hctx_hidden_args->struct_container = ops_container;
	ops_container->blk_mq_ops.init_hctx =
		LCD_HANDLE_TO_TRAMPOLINE(init_hctx_hidden_args->t_handle);

	printk("init_hctx in setup %p \n",ops_container->blk_mq_ops.init_hctx);
	err = set_memory_x(((unsigned long)init_hctx_hidden_args->t_handle) & PAGE_MASK,
			ALIGN(LCD_TRAMPOLINE_SIZE(init_hctx_fn_trampoline),
				PAGE_SIZE) >> PAGE_SHIFT);
	if (err) {
		LIBLCD_ERR("set mem nx");
		goto fail_x4;
	}

	/* exit_hctx */
	exit_hctx_hidden_args = kzalloc(sizeof( *exit_hctx_hidden_args ), GFP_KERNEL);
	if (!exit_hctx_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc5;
	}
	exit_hctx_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(exit_hctx_fn_trampoline);
	if (!exit_hctx_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup5;
	}
	exit_hctx_hidden_args->t_handle->hidden_args = exit_hctx_hidden_args;
	exit_hctx_hidden_args->struct_container = ops_container;
	ops_container->blk_mq_ops.exit_hctx =
		LCD_HANDLE_TO_TRAMPOLINE(exit_hctx_hidden_args->t_handle);

	printk("exit_hctx in setup %p \n",ops_container->blk_mq_ops.exit_hctx);
	err = set_memory_x(((unsigned long)exit_hctx_hidden_args->t_handle) & PAGE_MASK,
			ALIGN(LCD_TRAMPOLINE_SIZE(exit_hctx_fn_trampoline),
				PAGE_SIZE) >> PAGE_SHIFT);
	if (err) {
		LIBLCD_ERR("set mem nx");
		goto fail_x5;
	}

	/* init_request */
	init_request_hidden_args = kzalloc(sizeof( *init_request_hidden_args ),
			GFP_KERNEL);
	if (!init_request_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc6;
	}
	init_request_hidden_args->t_handle =
		LCD_DUP_TRAMPOLINE(init_request_fn_trampoline);
	if (!init_request_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup6;
	}
	init_request_hidden_args->t_handle->hidden_args = init_request_hidden_args;
	init_request_hidden_args->struct_container = ops_container;
	ops_container->blk_mq_ops.init_request =
		LCD_HANDLE_TO_TRAMPOLINE(init_request_hidden_args->t_handle);

	printk("init_request in setup %p \n",ops_container->blk_mq_ops.init_request);
	err = set_memory_x(((unsigned long)init_request_hidden_args->t_handle) & PAGE_MASK,
			ALIGN(LCD_TRAMPOLINE_SIZE(init_request_fn_trampoline),
				PAGE_SIZE) >> PAGE_SHIFT);
	if (err) {
		LIBLCD_ERR("set mem nx");
		goto fail_x6;
	}

	/* timeout */
	timeout_hidden_args = kzalloc(sizeof( *timeout_hidden_args ), GFP_KERNEL);
	if (!timeout_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc7;
	}
	timeout_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(timeout_fn_trampoline);
	if (!timeout_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup7;
	}
	timeout_hidden_args->t_handle->hidden_args = timeout_hidden_args;
	timeout_hidden_args->struct_container = ops_container;
	ops_container->blk_mq_ops.timeout =
		LCD_HANDLE_TO_TRAMPOLINE(timeout_hidden_args->t_handle);

	printk("timeout in setup %p \n",ops_container->blk_mq_ops.timeout);
	err = set_memory_x(((unsigned long)timeout_hidden_args->t_handle) & PAGE_MASK,
			ALIGN(LCD_TRAMPOLINE_SIZE(timeout_fn_trampoline),
				PAGE_SIZE) >> PAGE_SHIFT);
	if (err) {
		LIBLCD_ERR("set mem nx");
		goto fail_x7;
	}

	/* poll */
	poll_hidden_args = kzalloc(sizeof( *poll_hidden_args ), GFP_KERNEL);
	if (!poll_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc8;
	}
	poll_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(poll_fn_trampoline);
	if (!poll_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup8;
	}
	poll_hidden_args->t_handle->hidden_args = poll_hidden_args;
	poll_hidden_args->struct_container = ops_container;
	ops_container->blk_mq_ops.poll =
		LCD_HANDLE_TO_TRAMPOLINE(poll_hidden_args->t_handle);

	printk("poll in setup %p \n",ops_container->blk_mq_ops.poll);
	err = set_memory_x(((unsigned long)poll_hidden_args->t_handle) & PAGE_MASK,
			ALIGN(LCD_TRAMPOLINE_SIZE(poll_fn_trampoline),
				PAGE_SIZE) >> PAGE_SHIFT);
	if (err) {
		LIBLCD_ERR("set mem nx");
		goto fail_x8;
	}

	return;

fail_x8:
	kfree(poll_hidden_args->t_handle);
fail_dup8:
	kfree(poll_hidden_args);
fail_alloc8:
fail_x7:
	kfree(timeout_hidden_args->t_handle);
fail_dup7:
	kfree(timeout_hidden_args);
fail_alloc7:
fail_x6:
	kfree(init_request_hidden_args->t_handle);
fail_dup6:
	kfree(init_request_hidden_args);
fail_alloc6:
fail_x5:
	kfree(exit_hctx_hidden_args->t_handle);
fail_dup5:
	kfree(exit_hctx_hidden_args);
fail_alloc5:
fail_x4:
	kfree(init_hctx_hidden_args->t_handle);
fail_dup4:
	kfree(init_hctx_hidden_args);
fail_alloc4:
fail_x3:
	kfree(map_queue_hidden_args->t_handle);
fail_dup3:
	kfree(map_queue_hidden_args);
fail_alloc3:
fail_x2:
	kfree(complete_hidden_args->t_handle);
fail_dup2:
	kfree(complete_hidden_args);
fail_alloc2:
fail_x1:
	kfree(queue_rq_hidden_args->t_handle);
fail_dup1:
	kfree(queue_rq_hidden_args);
fail_alloc1:
	return;
}

int blk_mq_alloc_tag_set_callee(struct fipc_message *request)
{
	struct blk_mq_tag_set_container *set_container;
	struct blk_mq_ops_container *ops_container;

	int func_ret = 0;
	int err;

	/* Allocate tag_set_container struct here */
	set_container = kzalloc(sizeof(struct blk_mq_tag_set_container), GFP_KERNEL);
	if (!set_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc1;
	}

	err = glue_cap_insert_blk_mq_tag_set_type(c_cspace, set_container,
			&set_container->my_ref);
	if (err) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert1;
	}

	/* Store the caller's reference! */
	set_container->other_ref.cptr = fipc_get_reg0(request);
	printk("set_other_ref %ld \n",set_container->other_ref.cptr);

	/* Allocate ops_container */
	g_blk_mq_ops_container = ops_container = kzalloc(sizeof(struct
				blk_mq_ops_container), GFP_KERNEL);

	if (!g_blk_mq_ops_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc2;
	}

	err = glue_cap_insert_blk_mq_ops_type(c_cspace, ops_container,
			&ops_container->my_ref);
	if (err) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert2;
	}

	ops_container->other_ref.cptr = fipc_get_reg1(request);
	printk("ops_other_ref %ld \n",ops_container->other_ref.cptr);

	setup_blk_mq_ops(ops_container);

	/*
	 * This is required because, the blk_mq_tag_set which is passed to blk
	 * layer is this one
	 */
	set_container->tag_set.ops = &ops_container->blk_mq_ops;

	/* Get the rest of the members from LCD */
	set_container->tag_set.nr_hw_queues = fipc_get_reg2(request);
	set_container->tag_set.queue_depth = fipc_get_reg3(request);
	hw_depth = set_container->tag_set.queue_depth;
	set_container->tag_set.timeout = fipc_get_reg3(request) & 0xFFFF;
	set_container->tag_set.numa_node = fipc_get_reg3(request) >> 16;
	set_container->tag_set.cmd_size = fipc_get_reg5(request);
	set_container->tag_set.flags = fipc_get_reg6(request);

	/* call the real function */
	func_ret = blk_mq_alloc_tag_set((&set_container->tag_set));
	LIBLCD_MSG("block_alloc_tag set returns %d",func_ret);

	/* Hack for remove */
	set_g = &set_container->tag_set;
	fipc_set_reg0(request, set_container->my_ref.cptr);
	fipc_set_reg1(request, ops_container->my_ref.cptr);
	fipc_set_reg2(request, func_ret);

	return func_ret;

fail_insert2:
	kfree(ops_container);
fail_alloc2:
	glue_cap_remove(c_cspace, set_container->my_ref);
fail_insert1:
	kfree(set_container);
fail_alloc1:
	return func_ret;
}

int bd_open(struct block_device *device, fmode_t mode,
			struct trampoline_hidden_args *hidden_args)
{
	int ret = 0;
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct block_device_container *bdev_container;
	struct gendisk_container *gdisk_container;
	struct block_device_operations_container *bdops_container =
		hidden_args->struct_container;

	bdev_container = kzalloc(sizeof(struct block_device_container),
			GFP_KERNEL);

	if (!bdev_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	bdev_container->bdev = device;

	glue_insert_bdevice(bdev_container);

	gdisk_container = container_of(device->bd_disk, struct gendisk_container,
				gendisk);

	async_msg_set_fn_type(request, BD_OPEN_FN);
	fipc_set_reg0(request, bdops_container->other_ref.cptr);
	fipc_set_reg1(request, bdev_container->my_ref.cptr);
	fipc_set_reg2(request, mode);
	fipc_set_reg3(request, gdisk_container->other_ref.cptr);

	printk("%s, %s:%d on cpu:%d\n", __func__, current->comm, current->pid,
				smp_processor_id());
	vmfunc_klcd_wrapper(request, 1);

	ret = fipc_get_reg0(request);

	bdev_container->other_ref.cptr = fipc_get_reg1(request);

fail_alloc:
	return ret;
}

void bd_release(struct gendisk *disk, fmode_t mode,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct gendisk_container *disk_container;
	struct block_device_operations_container *bdops_container =
		hidden_args->struct_container;

	disk_container = container_of(disk, struct gendisk_container, gendisk);

	printk("%s, %s:%d on cpu:%d\n", __func__, current->comm, current->pid,
				smp_processor_id());

	async_msg_set_fn_type(request, BD_RELEASE_FN);
	fipc_set_reg0(request, bdops_container->other_ref.cptr);
	fipc_set_reg1(request, disk_container->other_ref.cptr);
	fipc_set_reg2(request, mode);

	vmfunc_klcd_wrapper(request, 1);

	return;
}

int blk_mq_stop_hw_queues_callee(struct fipc_message *_request)
{
	struct request_queue_container *rq_container;
	struct request_queue *rq;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&rq_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	rq = &rq_container->request_queue;

	blk_mq_stop_hw_queues(rq);

fail_lookup:
	return ret;
}

int irq_set_affinity_hint_callee(struct fipc_message *_request)
{
	int irq;
	struct cpumask m;
	int func_ret;
	bool mask_valid;

	irq = fipc_get_reg0(_request);
	mask_valid = fipc_get_reg1(_request);
	if (mask_valid)
		m.bits[0] = fipc_get_reg2(_request);

	func_ret = irq_set_affinity_hint(irq, mask_valid ? &m : NULL);

	fipc_set_reg0(_request, func_ret);
	return 0;
}

int get_device_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	struct device *func_ret;
	struct device_container *device_container = NULL;

	LIBLCD_MSG("%s, called", __func__);

	glue_lookup_device(__cptr(fipc_get_reg0(_request)), &device_container);

	dev = device_container->dev;

	func_ret = get_device(dev);

	return 0;
}


int put_device_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	struct device_container *device_container = NULL;

	LIBLCD_MSG("%s, called", __func__);

	glue_lookup_device(__cptr(fipc_get_reg0(_request)), &device_container);

	dev = device_container->dev;

	put_device(dev);

	return 0;
}

LCD_TRAMPOLINE_DATA(bd_open_trampoline);
int LCD_TRAMPOLINE_LINKAGE(bd_open_trampoline) bd_open_trampoline(struct block_device *device, fmode_t mode)

{
	int ( *volatile open_fp )(struct block_device *, fmode_t , struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, bd_open_trampoline);
	open_fp = bd_open;
	return open_fp(device, mode, hidden_args);
}

LCD_TRAMPOLINE_DATA(bd_release_trampoline);
void LCD_TRAMPOLINE_LINKAGE(bd_release_trampoline) bd_release_trampoline(struct gendisk *disk, fmode_t mode)

{
	void ( *volatile rel_fp )(struct gendisk *, fmode_t , struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, bd_release_trampoline);
	rel_fp = bd_release;
	return rel_fp(disk, mode, hidden_args);
}

long bd_ioctl(struct block_device *bdev, fmode_t mode, unsigned int cmd,
		unsigned long arg, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	long func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct block_device_container *bdev_container = NULL;
	struct block_device_operations_container *bdops_container =
		hidden_args->struct_container;

	glue_lookup_bdevice(__cptr((unsigned long) bdev), &bdev_container);

	async_msg_set_fn_type(_request, BD_IOCTL_FN);
	fipc_set_reg0(_request, bdops_container->other_ref.cptr);
	fipc_set_reg1(_request, bdev_container->other_ref.cptr);
	fipc_set_reg2(_request, mode);
	fipc_set_reg3(_request, cmd);
	fipc_set_reg4(_request, arg);

	ret = vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

LCD_TRAMPOLINE_DATA(bd_ioctl_trampoline);
long LCD_TRAMPOLINE_LINKAGE(bd_ioctl_trampoline)
bd_ioctl_trampoline(struct block_device *bdev, fmode_t mode, unsigned int cmd,
		unsigned long arg)
{
	long ( *volatile bd_ioctl_fn_fp )(struct block_device*, fmode_t, unsigned int,
			unsigned long,
			struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, bd_ioctl_trampoline);
	bd_ioctl_fn_fp = bd_ioctl;
	return bd_ioctl_fn_fp(bdev, mode, cmd, arg, hidden_args);
}

int bd_getgeo(struct block_device *device, struct hd_geometry *geo,
			struct trampoline_hidden_args *hidden_args)
{
	int ret = 0;
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct block_device_container *bdev_container = NULL;
	struct gendisk_container *gdisk_container;
	struct block_device_operations_container *bdops_container =
		hidden_args->struct_container;

	glue_lookup_bdevice(__cptr((unsigned long) device), &bdev_container);

	gdisk_container = container_of(device->bd_disk, struct gendisk_container,
				gendisk);

	async_msg_set_fn_type(request, BD_GETGEO_FN);
	fipc_set_reg0(request, bdops_container->other_ref.cptr);
	fipc_set_reg1(request, bdev_container->other_ref.cptr);
	fipc_set_reg2(request, gdisk_container->other_ref.cptr);

	printk("%s, %s:%d on cpu:%d\n", __func__, current->comm, current->pid,
				smp_processor_id());
	vmfunc_klcd_wrapper(request, 1);

	ret = fipc_get_reg0(request);

	geo->heads = fipc_get_reg1(request);
	geo->sectors = fipc_get_reg2(request);
	geo->cylinders = fipc_get_reg3(request);

	return ret;
}

LCD_TRAMPOLINE_DATA(bd_getgeo_trampoline);
int LCD_TRAMPOLINE_LINKAGE(bd_getgeo_trampoline) bd_getgeo_trampoline(struct block_device *device, struct hd_geometry *geo)

{
	int ( *volatile getgeo_fp )(struct block_device *, struct hd_geometry*,
			struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, bd_getgeo_trampoline);
	getgeo_fp = bd_getgeo;
	return getgeo_fp(device, geo, hidden_args);
}

void bd_revalidate_disk(struct gendisk *disk,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct gendisk_container *disk_container;
	struct block_device_operations_container *bdops_container =
		hidden_args->struct_container;

	disk_container = container_of(disk, struct gendisk_container, gendisk);

	printk("%s, %s:%d on cpu:%d\n", __func__, current->comm, current->pid,
				smp_processor_id());

	async_msg_set_fn_type(request, BD_REVALIDATE_DISK_FN);
	fipc_set_reg0(request, bdops_container->other_ref.cptr);
	fipc_set_reg1(request, disk_container->other_ref.cptr);

	vmfunc_klcd_wrapper(request, 1);

	return;
}

LCD_TRAMPOLINE_DATA(bd_revalidate_disk_trampoline);
void LCD_TRAMPOLINE_LINKAGE(bd_revalidate_disk_trampoline) bd_revalidate_disk_trampoline(struct gendisk *disk)

{
	void ( *volatile revalidate_disk_fp )(struct gendisk *, struct
			trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, bd_revalidate_disk_trampoline);
	revalidate_disk_fp = bd_revalidate_disk;
	return revalidate_disk_fp(disk, hidden_args);
}

int device_add_disk_callee(struct fipc_message *request)
{
	struct gendisk_container *disk_container;
	struct block_device_operations_container *blo_container;
	struct request_queue_container *rq_container;
	struct device_container *device_container = NULL;
	struct gendisk *disk;
	struct trampoline_hidden_args *open_hargs;
	struct trampoline_hidden_args *rel_hargs;
	struct trampoline_hidden_args *ioctl_hargs;
	struct trampoline_hidden_args *getgeo_hargs;
	struct trampoline_hidden_args *revalidate_disk_hargs;
	struct device *parent;
	int ret;

	glue_lookup_device(__cptr(fipc_get_reg0(request)), &device_container);

	parent = device_container->dev;

	ret = glue_cap_lookup_gendisk_type(c_cspace, __cptr(fipc_get_reg1(request)),
					&disk_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup1;
	}

	ret = glue_cap_lookup_request_queue_type(c_cspace, __cptr(fipc_get_reg3(request)),
			&rq_container);
	if(ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup2;
	}

	blo_container = kzalloc(sizeof(*blo_container), GFP_KERNEL);
	if(!blo_container) {
		LIBLCD_ERR("alloc failed");
		goto fail_alloc1;
	}

	blo_container->other_ref.cptr = fipc_get_reg2(request);

	ret = glue_cap_insert_blk_dev_ops_type(c_cspace, blo_container, &blo_container->my_ref);
	if(ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert1;
	}


	/* setup the fops */
	blo_container->block_device_operations.owner = THIS_MODULE;

        open_hargs = kzalloc(sizeof(*open_hargs), GFP_KERNEL);
        if (!open_hargs) {
                LIBLCD_ERR("kzalloc hidden args");
                goto fail_alloc2;
        }

        open_hargs->t_handle = LCD_DUP_TRAMPOLINE(bd_open_trampoline);
        if (!open_hargs->t_handle) {
                LIBLCD_ERR("duplicate trampoline");
                goto fail_dup1;
        }

        open_hargs->t_handle->hidden_args = open_hargs;
        open_hargs->struct_container = blo_container;
        blo_container->block_device_operations.open = LCD_HANDLE_TO_TRAMPOLINE(open_hargs->t_handle);

        ret = set_memory_x(((unsigned long)open_hargs->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(bd_open_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem nx");
                goto fail_x1;
        }

	rel_hargs = kzalloc(sizeof(*rel_hargs), GFP_KERNEL);
        if (!rel_hargs) {
                LIBLCD_ERR("kzalloc hidden args");
                goto fail_alloc3;
        }

        rel_hargs->t_handle = LCD_DUP_TRAMPOLINE(bd_release_trampoline);
        if (!rel_hargs->t_handle) {
                LIBLCD_ERR("duplicate trampoline");
                goto fail_dup2;
        }

	rel_hargs->t_handle->hidden_args = rel_hargs;
        rel_hargs->struct_container = blo_container;

        blo_container->block_device_operations.release = LCD_HANDLE_TO_TRAMPOLINE(rel_hargs->t_handle);

        ret = set_memory_x(((unsigned long)rel_hargs->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(bd_release_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem nx");
                goto fail_x2;
        }

	/* ioctl */
	ioctl_hargs = kzalloc(sizeof(*ioctl_hargs), GFP_KERNEL);
        if (!ioctl_hargs) {
                LIBLCD_ERR("kzalloc hidden args");
                goto fail_alloc4;
        }

        ioctl_hargs->t_handle = LCD_DUP_TRAMPOLINE(bd_ioctl_trampoline);
        if (!ioctl_hargs->t_handle) {
                LIBLCD_ERR("duplicate trampoline");
                goto fail_dup3;
        }

	ioctl_hargs->t_handle->hidden_args = ioctl_hargs;
        ioctl_hargs->struct_container = blo_container;

        blo_container->block_device_operations.ioctl = LCD_HANDLE_TO_TRAMPOLINE(ioctl_hargs->t_handle);

        ret = set_memory_x(((unsigned long)ioctl_hargs->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(bd_ioctl_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem nx");
                goto fail_x3;
        }

	getgeo_hargs = kzalloc(sizeof(*getgeo_hargs), GFP_KERNEL);
        if (!getgeo_hargs) {
                LIBLCD_ERR("kzalloc hidden args");
                goto fail_alloc5;
        }

        getgeo_hargs->t_handle = LCD_DUP_TRAMPOLINE(bd_getgeo_trampoline);
        if (!getgeo_hargs->t_handle) {
                LIBLCD_ERR("duplicate trampoline");
                goto fail_dup4;
        }

        getgeo_hargs->t_handle->hidden_args = getgeo_hargs;
        getgeo_hargs->struct_container = blo_container;
        blo_container->block_device_operations.getgeo = LCD_HANDLE_TO_TRAMPOLINE(getgeo_hargs->t_handle);

        ret = set_memory_x(((unsigned long)getgeo_hargs->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(bd_getgeo_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem nx");
                goto fail_x4;
        }

	revalidate_disk_hargs = kzalloc(sizeof(*revalidate_disk_hargs), GFP_KERNEL);
        if (!revalidate_disk_hargs) {
                LIBLCD_ERR("kzalloc hidden args");
                goto fail_alloc6;
        }

        revalidate_disk_hargs->t_handle = LCD_DUP_TRAMPOLINE(bd_revalidate_disk_trampoline);
        if (!revalidate_disk_hargs->t_handle) {
                LIBLCD_ERR("duplicate trampoline");
                goto fail_dup5;
        }

        revalidate_disk_hargs->t_handle->hidden_args = revalidate_disk_hargs;
        revalidate_disk_hargs->struct_container = blo_container;
        blo_container->block_device_operations.revalidate_disk = LCD_HANDLE_TO_TRAMPOLINE(revalidate_disk_hargs->t_handle);

        ret = set_memory_x(((unsigned long)revalidate_disk_hargs->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(bd_revalidate_disk_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem nx");
                goto fail_x5;
        }

	disk = &disk_container->gendisk;
	printk("address of disk before calling add_diks %p \n",disk);

	disk->flags = fipc_get_reg4(request);
	disk->queue = &rq_container->request_queue;
	set_capacity(disk, fipc_get_reg5(request));

	memcpy(disk->disk_name, (void*)&request->regs[6], sizeof(unsigned long));

	disk->fops = &blo_container->block_device_operations;

	printk("Adding disk on cpu: %d, disk %p disk_name: %s\n",
			raw_smp_processor_id(), disk, disk->disk_name);

	device_add_disk(parent, disk);

#if 0
	{
		struct kobject *kobj = &disk_to_dev(disk)->kobj;
		printk("%s, disk_to_dev(disk) %p, kobj %p, kobj->sd %p\n", __func__,
				disk_to_dev(disk), kobj, kobj->sd);
	}
#endif
	fipc_set_reg0(request, blo_container->my_ref.cptr);

	return ret;

fail_x5:
	kfree(revalidate_disk_hargs->t_handle);
fail_dup5:
	kfree(revalidate_disk_hargs);
fail_alloc6:
fail_x4:
	kfree(getgeo_hargs->t_handle);
fail_dup4:
	kfree(getgeo_hargs);
fail_alloc5:
fail_x3:
	kfree(ioctl_hargs->t_handle);
fail_dup3:
	kfree(ioctl_hargs);
fail_alloc4:
fail_x2:
	kfree(rel_hargs->t_handle);
fail_dup2:
	kfree(rel_hargs);
fail_alloc3:
fail_x1:
	kfree(open_hargs->t_handle);
fail_dup1:
	kfree(open_hargs);
fail_alloc2:
	glue_cap_remove(c_cspace, blo_container->my_ref);
fail_insert1:
	kfree(blo_container);
fail_alloc1:
fail_lookup2:
fail_lookup1:
	return ret;
}

int sync_setup_memory(void *data, size_t sz, unsigned long *order, cptr_t *data_cptr, unsigned long *data_offset)
{
        int ret;
        struct page *p;
        unsigned long data_len;
        unsigned long mem_len;
        /*
         * Determine page that contains (start of) data
         */
        p = virt_to_head_page(data);
        if (!p) {
                LIBLCD_ERR("failed to translate to page");
                ret = -EINVAL;
                goto fail1;
        }
        data_len = sz;
        mem_len = ALIGN(data + data_len - page_address(p), PAGE_SIZE);
        *order = ilog2(roundup_pow_of_two(mem_len >> PAGE_SHIFT));
        /*
         * Volunteer memory
         */
        *data_offset = data - page_address(p);
        ret = lcd_volunteer_pages(p, *order, data_cptr);
        if (ret) {
                LIBLCD_ERR("failed to volunteer memory");
                goto fail2;
        }
        /*
         * Done
         */
        return 0;
fail2:
fail1:
        return ret;
}

LCD_TRAMPOLINE_DATA(probe_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(probe_trampoline)
probe_trampoline(struct pci_dev *dev,
		const struct pci_device_id *id)
{
	int ( *volatile probe_fp )(struct pci_dev *,
		const struct pci_device_id *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			probe_trampoline);
	probe_fp = probe;
	return probe_fp(dev, id, hidden_args);
}

LCD_TRAMPOLINE_DATA(remove_trampoline);
void  LCD_TRAMPOLINE_LINKAGE(remove_trampoline)
remove_trampoline(struct pci_dev *dev)
{
	void ( *volatile remove_fp )(struct pci_dev *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			remove_trampoline);
	remove_fp = remove;
	return remove_fp(dev, hidden_args);
}

int pci_disable_msi_callee(struct fipc_message *_request)
{
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;
	int ret = 0;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	pci_disable_msi(pdev);

	LIBLCD_MSG("%s returned");
	return ret;
}

int pci_disable_msix_callee(struct fipc_message *_request)
{
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;
	int ret = 0;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	pci_disable_msix(pdev);

	fipc_set_reg0(_request, pdev->msix_enabled);

	LIBLCD_MSG("%s returned");
	return ret;
}

int pci_enable_msix_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long p_offset;
	cptr_t p_cptr, lcd_cptr;
	gva_t p_gva;
	int nvec;
	struct msix_entry *entries;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg4(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	nvec = fipc_get_reg0(_request);

	sync_ret = lcd_cptr_alloc(&p_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}

	mem_order = fipc_get_reg1(_request);
	p_offset = fipc_get_reg2(_request);
	lcd_cptr = __cptr(fipc_get_reg3(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_cptr,
			p_cptr);

	sync_ret = lcd_map_virt(p_cptr, mem_order, &p_gva);

	if (sync_ret) {
		LIBLCD_ERR("failed to map void *p");
		lcd_exit(-1);
	}

	entries = (struct msix_entry*)(void*)(gva_val(p_gva) + p_offset);

	LIBLCD_MSG("%s, dev->msix_enabled %d | nvec %d\n",
			__func__, pdev->msix_enabled, nvec);

	func_ret = pci_enable_msix(pdev, entries, nvec);

	LIBLCD_MSG("%s, returned %d", __func__, func_ret);

	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, pdev->msix_enabled);

	return ret;
}

int device_release_driver_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct pci_dev_container *dev_container = NULL;
	struct device *dev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	dev = &dev_container->pdev->dev;
	device_release_driver(dev);

fail_lookup:
	return ret;
}

int pci_enable_msi_range_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	int minvec, maxvec;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	minvec = fipc_get_reg1(_request);
	maxvec = fipc_get_reg2(_request);

	func_ret = pci_enable_msi_range(pdev, minvec, maxvec);

	LIBLCD_MSG("%s, returned %d", __func__, func_ret);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_enable_msix_range_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long p_offset;
	cptr_t p_cptr, lcd_cptr;
	gva_t p_gva;
	int minvec, maxvec;
	struct msix_entry *entries;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	LCD_MSG("%s Looking up with pdev_other.cptr %lx", __func__,
			__cptr(fipc_get_reg5(_request)));

	glue_lookup_pdev_hash(__cptr(fipc_get_reg5(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	minvec = fipc_get_reg0(_request);
	maxvec = fipc_get_reg1(_request);

	sync_ret = lcd_cptr_alloc(&p_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}

	mem_order = fipc_get_reg2(_request);
	p_offset = fipc_get_reg3(_request);
	lcd_cptr = __cptr(fipc_get_reg4(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_cptr,
			p_cptr);

	sync_ret = lcd_map_virt(p_cptr, mem_order, &p_gva);

	if (sync_ret) {
		LIBLCD_ERR("failed to map void *p");
		lcd_exit(-1);
	}

	entries = (struct msix_entry*)(void*)(gva_val(p_gva) + p_offset);

	LIBLCD_MSG("%s, dev->msix_enabled %d | minvec %d | maxvec %d",
			__func__, pdev->msix_enabled, minvec, maxvec);

	func_ret = pci_enable_msix_range(pdev, entries, minvec, maxvec);

	LIBLCD_MSG("%s, returned %d", __func__, func_ret);

	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, pdev->msix_enabled);

fail_lookup:
	return ret;
}


int __pci_register_driver_callee(struct fipc_message *_request)
{
	struct pci_driver_container *drv_container;
	struct trampoline_hidden_args *drv_probe_hidden_args;
	struct trampoline_hidden_args *drv_remove_hidden_args;

	int func_ret = 0;
	int ret = 0;

	drv_container = kzalloc(sizeof( struct pci_driver_container   ),
		GFP_KERNEL);
	if (!drv_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_pci_driver_type(c_cspace,
		drv_container,
		&drv_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	drv_container->other_ref.cptr = fipc_get_reg0(_request);

	drv_probe_hidden_args = kzalloc(sizeof( *drv_probe_hidden_args ),
		GFP_KERNEL);
	if (!drv_probe_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc1;
	}
	drv_probe_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(probe_trampoline);
	if (!drv_probe_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	drv_probe_hidden_args->t_handle->hidden_args = drv_probe_hidden_args;
	drv_probe_hidden_args->struct_container = drv_container;
	drv_container->pci_driver.probe = LCD_HANDLE_TO_TRAMPOLINE(drv_probe_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_probe_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(probe_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	drv_remove_hidden_args = kzalloc(sizeof( *drv_remove_hidden_args ),
		GFP_KERNEL);
	if (!drv_remove_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc2;
	}
	drv_remove_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(remove_trampoline);
	if (!drv_remove_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup2;
	}
	drv_remove_hidden_args->t_handle->hidden_args = drv_remove_hidden_args;
	drv_remove_hidden_args->struct_container = drv_container;
	drv_container->pci_driver.remove = LCD_HANDLE_TO_TRAMPOLINE(drv_remove_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_remove_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(remove_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	drv_container->pci_driver.name = DRVNAME;
	drv_container->pci_driver.id_table = nvme_id_table;

	/* XXX: We should rather call __pci_register_driver
	 * (at least according to the RPC semantics).
	 * However, kobject subsys is not happy with us on mangling
	 * the module name. If we call pci_register_driver instead,
	 * module pointer is taken from THIS_MODULE and kobject is
	 * happy. So, do _not_ do such crap! kobject is unhappy
	owner_container->module.state = MODULE_STATE_LIVE;
	strcpy(owner_container->module.name, "ixgbe_lcd");
	atomic_inc(&owner_container->module.refcnt);
	*/

	func_ret = pci_register_driver(&drv_container->pci_driver);

	LIBLCD_MSG("%s returned %d", __func__, func_ret);

	fipc_set_reg0(_request, drv_container->my_ref.cptr);
	fipc_set_reg2(_request, func_ret);

	return ret;
fail_alloc:
fail_alloc1:
fail_alloc2:
fail_dup1:
fail_dup2:
fail_insert:
	return ret;
}

int sync_probe_callee(struct fipc_message *_request)
{
	int ret;
	cptr_t res0_cptr;
	unsigned int res0_len;
	unsigned int pool_ord;
	cptr_t pool_cptr;
	cptr_t lcd_pool_cptr;
	cptr_t lcd_res0_cptr;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	LIBLCD_MSG("%s, called", __func__);


	glue_lookup_pdev_hash(__cptr(fipc_get_reg2(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	/*
	 * nvme driver just needs res[0]
	 */

	/* get LCD's pool cptr */
	lcd_res0_cptr = __cptr(fipc_get_reg0(_request));
	lcd_pool_cptr = __cptr(fipc_get_reg1(_request));

	res0_len = pci_resource_len(pdev, 0);

	LIBLCD_MSG("res0 start: %lx, len: %d",
			pci_resource_start(pdev, 0),
			get_order(res0_len));

	ret = lcd_volunteer_dev_mem(__gpa(pci_resource_start(pdev, 0)),
			get_order(res0_len),
			&res0_cptr);
	if (ret) {
		LIBLCD_ERR("volunteer devmem");
		goto fail_vol;
	}

	ret = lcd_volunteer_vmalloc_mem(__gva((unsigned long)iocb_pool->pool),
			IOCB_POOL_PAGES, &pool_cptr);
	pool_ord = IOCB_POOL_ORDER;

	if (ret) {
		LIBLCD_ERR("volunteer shared region");
		goto fail_vol;
	}

	pool_pfn_start = (unsigned long)iocb_pool->pool >> PAGE_SHIFT;
	pool_pfn_end = pool_pfn_start + IOCB_POOL_PAGES;

	copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, pool_cptr, lcd_pool_cptr);
	copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, res0_cptr, lcd_res0_cptr);

	fipc_set_reg0(_request, res0_len);
	fipc_set_reg1(_request, pool_ord);

fail_vol:
	return 0;
}

int probe(struct pci_dev *dev,
		const struct pci_device_id *id,
		struct trampoline_hidden_args *hidden_args)
{
	struct pci_dev_container *dev_container = NULL;
	struct device_container *device_container;
	int ret = 0;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_driver_container *pdrv_container =
		hidden_args->struct_container;

	INIT_IPC_MSG(&r);
	/* assign pdev to a global instance */
	g_pdev = dev;

	LIBLCD_MSG("%s, irq # %d | msix_enabled %d", __func__, dev->irq,
				dev->msix_enabled);

	dev_container = kzalloc(sizeof( struct pci_dev_container   ),
		GFP_KERNEL);
	if (!dev_container) {
		LIBLCD_ERR("kzalloc dev_container");
		goto fail_alloc;
	}

	dev_container->pdev = dev;

	device_container = kzalloc(sizeof( struct device_container   ),
		GFP_KERNEL);
	if (!device_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	device_container->dev = &dev_container->pdev->dev;

	glue_insert_device(device_container);
	/*
	 * Kernel does not give us pci_dev with a container wrapped in.
	 * So, let's have a pointer!
	 */
	glue_insert_pdev_hash(dev_container);

	async_msg_set_fn_type(_request, PROBE);

	fipc_set_reg0(_request, pdrv_container->other_ref.cptr);
	fipc_set_reg1(_request, dev_container->my_ref.cptr);
	//fipc_set_reg2(_request, *dev->dev.dma_mask);
	fipc_set_reg2(_request, device_container->my_ref.cptr);
	fipc_set_reg3(_request, ((unsigned long) id->vendor << 32) |
					id->device);
	fipc_set_reg4(_request, ((unsigned long) id->subvendor << 32) |
					id->subdevice);
	fipc_set_reg5(_request, ((unsigned long) id->class << 32) |
					id->class_mask);
	fipc_set_reg6(_request, ((unsigned long) id->driver_data << 32) |
	/* needed for pci_is_enabled() check */
					atomic_read(&dev->enable_cnt));

	vmfunc_klcd_wrapper(_request, 1);

	printk("%s, send request done\n", __func__);

	func_ret = fipc_get_reg0(_request);
	dev_container->other_ref.cptr = fipc_get_reg1(_request);

	return func_ret;

fail_alloc:
	return ret;
}

#ifdef CONFIG_LVD
int poll(struct blk_mq_hw_ctx *hctx, unsigned int tag)
#else
int poll(struct blk_mq_hw_ctx *hctx, unsigned int tag,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct blk_mq_hw_ctx_container *hctx_c;

	INIT_IPC_MSG(&r);

	hctx_c = container_of(hctx, struct blk_mq_hw_ctx_container, blk_mq_hw_ctx);

	async_msg_set_fn_type(_request, POLL);
	fipc_set_reg0(_request, tag);
	fipc_set_reg1(_request, hctx_c->other_ref.cptr);
	//fipc_set_reg2(_request, hctx_c->state);

	/*if (0)
		printk("%s, poll - budget %d | napi_c->other_ref %lx\n", __func__,
			budget, napi_c->other_ref.cptr);*/

#ifdef CONFIG_LCD_TRACE_BUFFER
	//add_trace_entry(EVENT_SOFTIRQ_POLL, async_msg_get_fn_type(_request));
#endif
	vmfunc_klcd_wrapper(_request, 1);

	ret = fipc_get_reg0(_request);

	pr_debug("%s, returned %d\n", __func__, ret);

	return ret;
}

int pci_unregister_driver_callee(struct fipc_message *_request)
{
	struct pci_driver_container *drv_container;
	int ret;
	struct trampoline_hidden_args *drv_probe_hidden_args;
	struct trampoline_hidden_args *drv_remove_hidden_args;

	ret = glue_cap_lookup_pci_driver_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&drv_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	LIBLCD_MSG("Calling pci_unregister_driver");

	pci_unregister_driver(( &drv_container->pci_driver ));

	glue_cap_remove(c_cspace, drv_container->my_ref);

	/* XXX: Do not do anything like this! read the comments
	 * under pci_unregister_driver
	 * atomic_dec_if_positive(&drv_container->pci_driver.driver.owner->refcnt);
	 */
	drv_probe_hidden_args = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(
					drv_container->pci_driver.probe);
	drv_remove_hidden_args = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(
					drv_container->pci_driver.remove);
	kfree(drv_probe_hidden_args->t_handle);
	kfree(drv_probe_hidden_args);
	kfree(drv_remove_hidden_args->t_handle);
	kfree(drv_remove_hidden_args);
	kfree(drv_container);

fail_lookup:
	return ret;
}

void remove(struct pci_dev *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *dev_container = NULL;
	struct pci_driver_container *pdrv_container =
		hidden_args->struct_container;

	INIT_IPC_MSG(&r);

	glue_lookup_pdev_hash(__cptr((unsigned long) dev), &dev_container);

	async_msg_set_fn_type(_request, REMOVE);
	fipc_set_reg0(_request, pdrv_container->other_ref.cptr);
	fipc_set_reg1(_request, dev_container->other_ref.cptr);

	vmfunc_klcd_wrapper(_request, 1);

	return;
}

int device_set_wakeup_enable_callee(struct fipc_message *_request)
{
	struct device *dev;
	bool enable;
	int ret = 0;

	int func_ret;

	dev = &g_pdev->dev;
	enable = fipc_get_reg1(_request);
	func_ret = device_set_wakeup_enable(dev,
		enable);

	fipc_set_reg1(_request,
			func_ret);

	return ret;
}

int pci_disable_pcie_error_reporting_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	func_ret = pci_disable_pcie_error_reporting(pdev);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int pci_bus_read_config_word_callee(struct fipc_message *_request)
{
	struct pci_dev *pdev;
	struct pci_bus *bus;
	unsigned int devfn;
	int where;
	unsigned short val;
	int func_ret;

	pdev = g_pdev;
	bus = pdev->bus;

	devfn = fipc_get_reg0(_request);
	where = fipc_get_reg1(_request);

	func_ret = pci_bus_read_config_word(bus, devfn, where, &val);

	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, val);

	return 0;
}

int pci_bus_write_config_word_callee(struct fipc_message *_request)
{
	struct pci_dev *pdev;
	struct pci_bus *bus;
	unsigned int devfn;
	int where;
	unsigned short val;
	int func_ret;

	pdev = g_pdev;
	bus = pdev->bus;

	devfn = fipc_get_reg0(_request);
	where = fipc_get_reg1(_request);
	val = fipc_get_reg2(_request);

	func_ret = pci_bus_write_config_word(bus, devfn, where, val);

	fipc_set_reg0(_request, func_ret);

	return 0;
}

int pci_cleanup_aer_uncorrect_error_status_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	func_ret = pci_cleanup_aer_uncorrect_error_status(pdev);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int pci_disable_device_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	pci_disable_device(pdev);

	return ret;
}

int pci_device_is_present_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	func_ret = pci_device_is_present(pdev);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int pci_restore_state_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	pci_restore_state(pdev);

	return ret;
}

int pci_enable_pcie_error_reporting_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	func_ret = pci_enable_pcie_error_reporting(pdev);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int pcie_capability_read_word_callee(struct fipc_message *_request)
{
	int pos;
	unsigned short val;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	pos = fipc_get_reg1(_request);
	val = fipc_get_reg2(_request);

	func_ret = pcie_capability_read_word(pdev, pos, &val);

	fipc_set_reg0(_request, func_ret);

	fipc_set_reg2(_request, val);

	return ret;
}

int pcie_get_minimum_link_callee(struct fipc_message *_request)
{
	enum pci_bus_speed speed;
	enum pcie_link_width width;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	speed = fipc_get_reg1(_request);
	width = fipc_get_reg2(_request);

	func_ret = pcie_get_minimum_link(pdev, &speed, &width);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int pci_enable_device_mem_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	func_ret = pci_enable_device_mem(pdev);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int pci_request_selected_regions_callee(struct fipc_message *_request)
{
	int type;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	type = fipc_get_reg1(_request);

	func_ret = pci_request_selected_regions(pdev,
				type,
				DRVNAME);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int pci_request_selected_regions_exclusive_callee(struct fipc_message *_request)
{
	int type;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	type = fipc_get_reg1(_request);

	func_ret = pci_request_selected_regions_exclusive(pdev,
				type,
				DRVNAME);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int pci_set_master_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	pci_set_master(pdev);

	return ret;
}

int pci_save_state_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	func_ret = pci_save_state(pdev);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int pci_release_selected_regions_callee(struct fipc_message *_request)
{
	int r;
	int ret = 0;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	r = fipc_get_reg1(_request);
	pci_release_selected_regions(pdev, r);

	return ret;
}

int pci_select_bars_callee(struct fipc_message *_request)
{
	unsigned long flags;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	flags = fipc_get_reg1(_request);

	func_ret = pci_select_bars(pdev, flags);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int pci_wake_from_d3_callee(struct fipc_message *_request)
{
	bool enable;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;
	enable = fipc_get_reg1(_request);

	func_ret = pci_wake_from_d3(pdev, enable);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int trigger_exit_to_lcd(struct thc_channel *_channel, enum dispatch_t disp)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	INIT_IPC_MSG(&r);

	async_msg_set_fn_type(_request,
			disp);

	/* No need to wait for a response here */
	vmfunc_klcd_wrapper(_request, 1);

	return 0;
}

irqreturn_t msix_vector_handler(int irq, void *data)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct irqhandler_t_container *irqhandler_container;
	irqreturn_t irqret;

	INIT_IPC_MSG(&r);

	WARN_ONCE(!irqs_disabled(),"irqs enabled in %s\n", __func__);

	irqhandler_container = (struct irqhandler_t_container*) data;

	async_msg_set_fn_type(_request, MSIX_IRQ_HANDLER);

	fipc_set_reg0(_request, irq);

	/* pass this irqhandler's other ref cptr */
	fipc_set_reg1(_request, irqhandler_container->other_ref.cptr);

#ifdef HANDLE_IRQ_LOCALLY
	irqret = IRQ_HANDLED;
	printk("%s, got irq# %d\n", __func__, irq);
#else
#ifdef CONFIG_LCD_TRACE_BUFFER
	//add_trace_entry(EVENT_MSIX_HANDLER, async_msg_get_fn_type(_request));
#endif
	vmfunc_klcd_wrapper(_request, 1);

	irqret = fipc_get_reg0(_request);
#endif
	return irqret;
}

struct irq_handler_data_map {
	int irq;
	void *data;
	struct irqhandler_t_container *irqhandler_data;
}irq_map[32];

static int reg_irqs = 0;

int request_threaded_irq_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret = 0;
	int irq;
	unsigned long flags;
	struct irqhandler_t_container *irqhandler_container;
	unsigned char *vector_name;
	void *data;

	irqhandler_container = kzalloc(sizeof(struct irqhandler_t_container),
					GFP_KERNEL);
	if (!irqhandler_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	vector_name = kzalloc(sizeof(unsigned long) * 3, GFP_KERNEL);

	if (!vector_name) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	irq = fipc_get_reg0(_request);
	irqhandler_container->other_ref.cptr = fipc_get_reg1(_request);
	flags = fipc_get_reg2(_request);
	data = (void*) fipc_get_reg6(_request);

	memcpy(vector_name, (void *)&_request->regs[3], sizeof(unsigned long) * 3);

	LIBLCD_MSG("%s, request_threaded_irq for %d | name: %s | data %p",
			__func__, irq, vector_name, data);

	func_ret = request_threaded_irq(irq, msix_vector_handler,
				NULL, flags,
				vector_name, (void*) irqhandler_container);

	printk("%s, storing in irq_map at idx: %d\n", __func__, reg_irqs);

	irq_map[reg_irqs].irq = irq;
	irq_map[reg_irqs].data = data;
	irq_map[reg_irqs].irqhandler_data = irqhandler_container;

	reg_irqs++;
	fipc_set_reg0(_request, func_ret);

fail_alloc:
	return ret;
}

int free_irq_callee(struct fipc_message *_request)
{
	unsigned int irq;
	struct irqhandler_t_container *irqhandler_container = NULL;
	int ret = 0;
	void *data;
	int i;

	irq = fipc_get_reg0(_request);
	data = (void*) fipc_get_reg1(_request);

	LIBLCD_MSG("%s, freeing irq %d | data %p", __func__, irq, data);

	for (i = 0; i < 32; i++) {
		if ((irq_map[i].irq == irq) && (irq_map[i].data == data)) {
			irqhandler_container = irq_map[i].irqhandler_data;
			break;
		}
	}

	if (!irqhandler_container) {
		printk("%s unable to retrieve container data for irq %d",
				__func__, irq);
		goto exit;
	}
	free_irq(irq, irqhandler_container);
	reg_irqs--;
exit:
	return ret;
}

int synchronize_irq_callee(struct fipc_message *_request)
{
	unsigned int irq;

	irq = fipc_get_reg0(_request);

	synchronize_irq(irq);

	return 0;
}

int __unregister_chrdev_callee(struct fipc_message *_request)
{
	unsigned 	int major = 0;
	unsigned 	int baseminor = 0;
	unsigned 	int count = 0;
	int ret = 0;

	major = fipc_get_reg0(_request);
	baseminor = fipc_get_reg1(_request);
	count = fipc_get_reg2(_request);

	__unregister_chrdev(major, baseminor, count, CHRDEV_NAME);

	return ret;
}

int fops_open(struct inode *inode,
		struct file *file,
		struct trampoline_hidden_args *hidden_args)
{
	int ret;
	int func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct file_ptr_container *file_c;
	struct file_operations_container *fops_container =
		hidden_args->struct_container;

	file_c = kzalloc(sizeof(*file_c), GFP_KERNEL);

	if (!file_c) {
		LIBLCD_ERR("kzalloc");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	file_c->file = file;

	glue_insert_file(file_c);

	async_msg_set_fn_type(_request, FOPS_OPEN_FN);

	fipc_set_reg0(_request, fops_container->other_ref.cptr);
	fipc_set_reg1(_request, file_c->my_ref.cptr);
	fipc_set_reg2(_request, iminor(inode));

	ret = vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);
	file_c->other_ref.cptr = fipc_get_reg1(_request);

	return func_ret;
fail_alloc:
	return ret;
}


LCD_TRAMPOLINE_DATA(fops_open_trampoline);
int LCD_TRAMPOLINE_LINKAGE(fops_open_trampoline)
fops_open_trampoline(struct inode *inode, struct file *file)
{
	int ( *volatile fops_open_fn_fp )(struct inode*, struct file*,
			struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, fops_open_trampoline);
	fops_open_fn_fp = fops_open;
	return fops_open_fn_fp(inode, file, hidden_args);
}

int fops_release(struct inode *inode,
		struct file *file,
		struct trampoline_hidden_args *hidden_args)
{
	int ret;
	int func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct file_ptr_container *file_c = NULL;
	struct file_operations_container *fops_container =
		hidden_args->struct_container;

	glue_lookup_file(__cptr((unsigned long) file), &file_c);

	assert(file_c != NULL);

	async_msg_set_fn_type(_request, FOPS_RELEASE_FN);

	fipc_set_reg0(_request, fops_container->other_ref.cptr);
	fipc_set_reg1(_request, file_c->other_ref.cptr);

	ret = vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

LCD_TRAMPOLINE_DATA(fops_release_trampoline);
int LCD_TRAMPOLINE_LINKAGE(fops_release_trampoline)
fops_release_trampoline(struct inode *inode, struct file *file)
{
	int ( *volatile fops_release_fn_fp )(struct inode*, struct file*,
			struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, fops_release_trampoline);
	fops_release_fn_fp = fops_release;
	return fops_release_fn_fp(inode, file, hidden_args);
}

long fops_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long
		arg, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	long func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct file_ptr_container *file_c = NULL;
	struct file_operations_container *fops_container =
		hidden_args->struct_container;

	glue_lookup_file(__cptr((unsigned long) file), &file_c);

	assert(file_c != NULL);

	async_msg_set_fn_type(_request, FOPS_UNLOCKED_IOCTL_FN);
	fipc_set_reg0(_request, fops_container->other_ref.cptr);
	fipc_set_reg1(_request, file_c->other_ref.cptr);
	fipc_set_reg2(_request, cmd);
	fipc_set_reg3(_request, arg);

	ret = vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

LCD_TRAMPOLINE_DATA(fops_unlocked_ioctl_trampoline);
long LCD_TRAMPOLINE_LINKAGE(fops_unlocked_ioctl_trampoline)
fops_unlocked_ioctl_trampoline(struct file *file, unsigned int cmd, unsigned long arg)
{
	long ( *volatile fops_unlocked_ioctl_fn_fp )(struct file*, unsigned int,
			unsigned long,
			struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, fops_unlocked_ioctl_trampoline);
	fops_unlocked_ioctl_fn_fp = fops_unlocked_ioctl;
	return fops_unlocked_ioctl_fn_fp(file, cmd, arg, hidden_args);
}


void setup_fops_container(struct file_operations_container *fops_container)
{
	int ret;
	struct trampoline_hidden_args *fops_open_hargs;
	struct trampoline_hidden_args *fops_release_hargs;
	struct trampoline_hidden_args *fops_unlocked_ioctl_hargs;
	struct trampoline_hidden_args *fops_compat_ioctl_hargs;

	fops_open_hargs = kzalloc(sizeof(struct trampoline_hidden_args),
					GFP_KERNEL);
	fops_release_hargs = kzalloc(sizeof(struct trampoline_hidden_args),
					GFP_KERNEL);
	fops_unlocked_ioctl_hargs = kzalloc(sizeof(struct trampoline_hidden_args),
					GFP_KERNEL);
	fops_compat_ioctl_hargs = kzalloc(sizeof(struct trampoline_hidden_args),
					GFP_KERNEL);

	/* open */
	fops_open_hargs->t_handle = LCD_DUP_TRAMPOLINE(fops_open_trampoline);
	if (!fops_open_hargs->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	fops_open_hargs->t_handle->hidden_args = fops_open_hargs;
	fops_open_hargs->struct_container = fops_container;
	fops_container->fops.open =
		LCD_HANDLE_TO_TRAMPOLINE(fops_open_hargs->t_handle);
	ret = set_memory_x(( ( unsigned  long   )fops_open_hargs->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(fops_open_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	/* release */
	fops_release_hargs->t_handle = LCD_DUP_TRAMPOLINE(fops_release_trampoline);
	if (!fops_release_hargs->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	fops_release_hargs->t_handle->hidden_args = fops_release_hargs;
	fops_release_hargs->struct_container = fops_container;
	fops_container->fops.release =
		LCD_HANDLE_TO_TRAMPOLINE(fops_release_hargs->t_handle);
	ret = set_memory_x(( ( unsigned  long   )fops_release_hargs->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(fops_release_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	/* unlocked_ioctl */
	fops_unlocked_ioctl_hargs->t_handle = LCD_DUP_TRAMPOLINE(fops_unlocked_ioctl_trampoline);
	if (!fops_unlocked_ioctl_hargs->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	fops_unlocked_ioctl_hargs->t_handle->hidden_args = fops_unlocked_ioctl_hargs;
	fops_unlocked_ioctl_hargs->struct_container = fops_container;
	fops_container->fops.unlocked_ioctl =
		LCD_HANDLE_TO_TRAMPOLINE(fops_unlocked_ioctl_hargs->t_handle);
	ret = set_memory_x(( ( unsigned  long   )fops_unlocked_ioctl_hargs->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(fops_unlocked_ioctl_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	fops_container->fops.compat_ioctl =
		LCD_HANDLE_TO_TRAMPOLINE(fops_unlocked_ioctl_hargs->t_handle);

fail_dup1:
	return;
}

int __register_chrdev_callee(struct fipc_message *_request)
{
	unsigned 	int major = 0;
	unsigned 	int baseminor = 0;
	unsigned 	int count = 0;
	struct file_operations_container *fops_container;
	int ret = 0;
	int func_ret = 0;

	fops_container = kzalloc(sizeof( *fops_container ), GFP_KERNEL);

	if (!fops_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_file_operations_type(c_cspace, fops_container,
				&fops_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	fops_container->other_ref.cptr = fipc_get_reg0(_request);
	major = fipc_get_reg1(_request);
	baseminor = fipc_get_reg2(_request);
	count = fipc_get_reg3(_request);

	setup_fops_container(fops_container);

	func_ret = __register_chrdev(major, baseminor, count, CHRDEV_NAME,
			&fops_container->fops);
	fipc_set_reg0(_request, fops_container->my_ref.cptr);
	fipc_set_reg1(_request, func_ret);
fail_alloc:
fail_insert:
	return ret;
}

int ida_destroy_callee(struct fipc_message *_request)
{
	struct ida *ida = NULL;
	int ret = 0;
	struct ida_container *ida_container;

	ret = glue_cap_lookup_ida_type(c_cspace, __cptr(fipc_get_reg0(_request)),
				&ida_container);
	if (ret) {
		LIBLCD_ERR("lcd lookup");
		goto fail_lookup;
	}

	ida = &ida_container->ida;

	ida_destroy(ida);
fail_lookup:
	return ret;
}

int ida_get_new_above_callee(struct fipc_message *_request)
{
	int starting_id;
	int p_id;
	int ret = 0;
	int func_ret = 0;
	struct ida *ida = NULL;
	struct ida_container *ida_container;

	ret = glue_cap_lookup_ida_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ida_container);
	if (ret) {
		LIBLCD_ERR("lcd lookup");
		goto fail_lookup;
	}

	ida = &ida_container->ida;
	starting_id = fipc_get_reg1(_request);

	func_ret = ida_get_new_above(ida, starting_id, &p_id);

	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, p_id);

fail_lookup:
	return ret;
}

int ida_init_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct ida_container *ida_container;

	ida_container = kzalloc(sizeof( struct ida_container   ), GFP_KERNEL);

	if (!ida_container) {
		LIBLCD_ERR("kzalloc");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	ret = glue_cap_insert_ida_type(c_cspace, ida_container,
			&ida_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	ida_container->other_ref.cptr = fipc_get_reg0(_request);

	ida_init(( &ida_container->ida ));

	fipc_set_reg0(_request, ida_container->my_ref.cptr);


fail_alloc:
fail_insert:
	return ret;
}

int ida_pre_get_callee(struct fipc_message *_request)
{
	unsigned long gfp_mask;
	int ret;
	int func_ret;
	struct ida *ida = NULL;
	struct ida_container *ida_container;

	ida_container = kzalloc(sizeof( struct ida_container   ), GFP_KERNEL);

	if (!ida_container) {
		LIBLCD_ERR("kzalloc");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	ida_container->ida.free_bitmap = NULL;
	ida_container->ida.idr.lock = __SPIN_LOCK_UNLOCKED(ida_container->ida.idr.lock);

	ret = glue_cap_insert_ida_type(c_cspace, ida_container,
			&ida_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	ida_container->other_ref.cptr = fipc_get_reg0(_request);
	ida = &ida_container->ida;

	gfp_mask = fipc_get_reg1(_request);
	func_ret = ida_pre_get(ida, gfp_mask);
	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, ida_container->my_ref.cptr);

fail_alloc:
fail_insert:
	return ret;
}

int ida_remove_callee(struct fipc_message *_request)
{
	int id;
	int ret;
	struct ida *ida = NULL;
	struct ida_container *ida_container;

	ret = glue_cap_lookup_ida_type(c_cspace, __cptr(fipc_get_reg0(_request)),
				&ida_container);
	if (ret) {
		LIBLCD_ERR("lcd lookup");
		goto fail_lookup;
	}

	ida = &ida_container->ida;
	id = fipc_get_reg1(_request);
	ida_remove(ida, id);

fail_lookup:
	return ret;
}

int ida_simple_get_callee(struct fipc_message *_request)
{
	unsigned 	int start;
	unsigned 	int end;
	unsigned 	long gfp_mask;
	int ret;
	int func_ret;
	struct ida *ida = NULL;
	struct ida_container *ida_container;

	ret = glue_cap_lookup_ida_type(c_cspace, __cptr(fipc_get_reg0(_request)),
				&ida_container);
	if (ret) {
		LIBLCD_ERR("lcd lookup");
		goto fail_lookup;
	}

	ida = &ida_container->ida;
	start = fipc_get_reg1(_request);
	end = fipc_get_reg2(_request);
	gfp_mask = fipc_get_reg3(_request);

	func_ret = ida_simple_get(ida, start, end, gfp_mask);

	fipc_set_reg0(_request, func_ret);
fail_lookup:
	return ret;
}

int ida_simple_remove_callee(struct fipc_message *_request)
{
	unsigned int id;
	int ret;
	struct ida *ida = NULL;
	struct ida_container *ida_container;

	ret = glue_cap_lookup_ida_type(c_cspace, __cptr(fipc_get_reg0(_request)),
				&ida_container);
	if (ret) {
		LIBLCD_ERR("lcd lookup");
		goto fail_lookup;
	}

	ida = &ida_container->ida;
	id = fipc_get_reg1(_request);

	ida_simple_remove(ida, id);

fail_lookup:
	return ret;
}

int blk_set_queue_dying_callee(struct fipc_message *_request)
{
	struct request_queue *q = NULL;
	struct request_queue_container *q_container;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	blk_set_queue_dying(q);

fail_lookup:
	return ret;
}

int blk_rq_unmap_user_callee(struct fipc_message *_request)
{
	struct bio *bio = NULL;
	int ret = 0;
	int func_ret = 0;
	bio = kzalloc(sizeof( *bio ),
		GFP_KERNEL);
	if (!bio) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = blk_rq_unmap_user(bio);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int blk_queue_write_cache_callee(struct fipc_message *_request)
{
	int ret;
	bool wc;
	bool fua;
	struct request_queue *q = NULL;
	struct request_queue_container *q_container;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	wc = fipc_get_reg1(_request);
	fua = fipc_get_reg2(_request);

	blk_queue_write_cache(q, wc, fua);
fail_lookup:
	return ret;
}

int blk_queue_virt_boundary_callee(struct fipc_message *_request)
{
	struct request_queue *q = NULL;
	struct request_queue_container *q_container;
	unsigned long mask;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;
	mask = fipc_get_reg1(_request);

	blk_queue_virt_boundary(q, mask);
	fipc_set_reg0(_request, q->limits.virt_boundary_mask);

fail_lookup:
	return ret;
}

int blk_queue_max_segments_callee(struct fipc_message *_request)
{
	struct request_queue *q = NULL;
	struct request_queue_container *q_container;
	unsigned short max_segments;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;
	max_segments = fipc_get_reg1(_request);

	blk_queue_max_segments(q, max_segments);
	fipc_set_reg0(_request, q->limits.virt_boundary_mask);

fail_lookup:
	return ret;
}

int blk_queue_max_hw_sectors_callee(struct fipc_message *_request)
{
	struct request_queue *q = NULL;
	struct request_queue_container *q_container;
	unsigned int max_hw_sectors;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;
	max_hw_sectors = fipc_get_reg1(_request);

	blk_queue_max_hw_sectors(q, max_hw_sectors);

fail_lookup:
	return ret;
}

int blk_queue_max_discard_sectors_callee(struct fipc_message *_request)
{
	struct request_queue *q = NULL;
	struct request_queue_container *q_container;
	unsigned int max_discard_sectors;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;
	max_discard_sectors = fipc_get_reg1(_request);

	blk_queue_max_discard_sectors(q, max_discard_sectors);

fail_lookup:
	return ret;
}

int blk_queue_chunk_sectors_callee(struct fipc_message *_request)
{
	struct request_queue *q = NULL;
	struct request_queue_container *q_container;
	unsigned int chunk_sectors;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;
	chunk_sectors = fipc_get_reg1(_request);

	blk_queue_chunk_sectors(q, chunk_sectors);

fail_lookup:
	return ret;
}

int blk_mq_unfreeze_queue_callee(struct fipc_message *_request)
{
	struct request_queue *q = NULL;
	struct request_queue_container *q_container;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	blk_mq_unfreeze_queue(q);

fail_lookup:
	return ret;
}

int blk_mq_requeue_request_callee(struct fipc_message *_request)
{
	struct request *rq = NULL;
	int ret = 0;
	rq = kzalloc(sizeof( *rq ),
		GFP_KERNEL);
	if (!rq) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	blk_mq_requeue_request(rq);
fail_alloc:
	return ret;

}

int blk_mq_request_started_callee(struct fipc_message *_request)
{
	struct request *rq = NULL;
	int ret = 0;
	int func_ret = 0;
	rq = kzalloc(sizeof( *rq ),
		GFP_KERNEL);
	if (!rq) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = blk_mq_request_started(rq);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int blk_mq_kick_requeue_list_callee(struct fipc_message *_request)
{
	struct request_queue *q = NULL;
	struct request_queue_container *q_container;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	blk_mq_kick_requeue_list(q);

fail_lookup:
	return ret;
}

int blk_mq_freeze_queue_callee(struct fipc_message *_request)
{
	struct request_queue *q = NULL;
	struct request_queue_container *q_container;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	blk_mq_freeze_queue(q);

fail_lookup:
	return ret;
}

int blk_mq_cancel_requeue_work_callee(struct fipc_message *_request)
{
	struct request_queue *q = NULL;
	struct request_queue_container *q_container;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	blk_mq_cancel_requeue_work(q);

fail_lookup:
	return ret;
}

int blk_mq_alloc_request_hctx_callee(struct fipc_message *_request)
{
	int rw;
	unsigned int flags;
	unsigned int hctx_idx;
	int ret;
	struct request_container *func_ret_container = NULL;
	struct request *func_ret = NULL;
	struct request_queue_container *q_container;
	struct request_queue *q;

	func_ret_container = kzalloc(sizeof( struct request_container   ),
			GFP_KERNEL);

	if (!func_ret_container) {
		LIBLCD_ERR("kzalloc");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	rw = fipc_get_reg2(_request);
	flags = fipc_get_reg3(_request);
	hctx_idx = fipc_get_reg4(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg1(_request);

	func_ret = blk_mq_alloc_request_hctx(q, rw, flags, hctx_idx);

	func_ret_container->req = func_ret;

	ret = glue_cap_insert_request_type(c_cspace, func_ret_container,
			&func_ret_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg0(_request, func_ret_container->my_ref.cptr);

fail_alloc:
fail_lookup:
fail_insert:
	return ret;
}

int blk_mq_alloc_request_callee(struct fipc_message *_request)
{
	int rw;
	unsigned int flags;
	int ret;
	struct request *rq;
	struct request_queue_container *q_container;
	struct request_queue *q;
	struct blk_mq_ops_container *ops_container;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	rw = fipc_get_reg2(_request);
	flags = fipc_get_reg3(_request);

	ops_container = container_of(q->mq_ops, struct blk_mq_ops_container,
			blk_mq_ops);
	rq = blk_mq_alloc_request(q, rw, flags);

	printk("%s, rq: %p, q: %p, rq->q %p\n", __func__, rq, q, rq->q);

	fipc_set_reg0(_request, get_queue_num(rq));
	fipc_set_reg1(_request, rq->tag);
	fipc_set_reg2(_request, q->limits.max_hw_sectors);
	fipc_set_reg3(_request, q->limits.max_segments);
	fipc_set_reg4(_request, ops_container->other_ref.cptr);

fail_lookup:
	return ret;
}

int blk_mq_abort_requeue_list_callee(struct fipc_message *_request)
{
	struct request_queue *q = NULL;
	struct request_queue_container *q_container;
	int ret;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	blk_mq_abort_requeue_list(q);

fail_lookup:
	return ret;
}

int blk_mq_free_request_callee(struct fipc_message *_request)
{
	struct request *rq = NULL;
	//struct request_container *req_container = NULL;
	struct request_queue_container *q_container;
	struct request_queue *q;
	int ret;
	int tag;

#if 0
	ret = glue_cap_lookup_request_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &req_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	rq = &req_container->request;
#endif

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg1(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	tag = fipc_get_reg2(_request);
	rq = blk_mq_get_rq_from_tag(q, tag);
	//printk("%s, rq: %p\n", __func__, rq);
#if 0
	rq = q->queue_hw_ctx[0]->tags->rqs[tag];
#endif

	blk_mq_free_request(rq);
fail_lookup:
	return ret;
}

int blk_rq_map_kern_callee(struct fipc_message *_request)
{
	unsigned 	long mem_order;
	unsigned 	long p_offset;
	cptr_t p_cptr, lcd_cptr;
	gva_t p_gva;
	struct request_queue *q = NULL;
	struct request *rq = NULL;
	struct request_queue_container *q_container;
	struct request_container *rq_container = NULL;
	int ret;
	int func_ret;
	void *kbuf;
	unsigned int len;
	gfp_t gfp_mask;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &q_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &q_container->request_queue;

	printk("%s, request_queue %p | cptr: %lx\n", __func__, q,
						fipc_get_reg0(_request));

	ret = glue_cap_lookup_request_type(c_cspace,
			__cptr(fipc_get_reg1(_request)), &rq_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	rq = rq_container->req;

	ret = lcd_cptr_alloc(&p_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_cptr;
	}

	mem_order = fipc_get_reg2(_request);
	p_offset = fipc_get_reg3(_request);
	lcd_cptr = __cptr(fipc_get_reg4(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_cptr,
			p_cptr);

	ret = lcd_map_virt(p_cptr, mem_order, &p_gva);

	if (ret) {
		LIBLCD_ERR("failed to map void *p");
		goto fail_virt;
	}

	kbuf = (void*)(gva_val(p_gva) + p_offset);

	len = fipc_get_reg5(_request);
	gfp_mask = fipc_get_reg6(_request);

	printk("%s, rq: %p, q: %p, rq->q %p\n", __func__, rq, q, rq->q);
	printk("%s, calling blk_rq_map_kern with q: %p, rq: %p, kbuf: %p\n",
				__func__, q, rq, kbuf);

	func_ret = blk_rq_map_kern(q, rq, kbuf, len, gfp_mask);

	LIBLCD_MSG("%s, returned %d", __func__, func_ret);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
fail_virt:
fail_cptr:
	return ret;
}


int blk_execute_rq_callee(struct fipc_message *_request)
{
	struct request_queue *q = NULL;
	struct gendisk *bd_disk = NULL;
	struct request *rq = NULL;
	int at_head = 0;
	int ret = 0;
	int func_ret = 0;
	struct request_queue_container *rq_container;
	//struct request_container *req_container = NULL;
	struct gendisk_container *gdisk_container;
	cptr_t gdisk_cptr;
	int tag;

	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &rq_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	q = &rq_container->request_queue;

#if 0
	ret = glue_cap_lookup_request_type(c_cspace,
			__cptr(fipc_get_reg1(_request)), &req_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	rq = req_container->req;
#endif
	tag = fipc_get_reg2(_request);
	rq = blk_mq_get_rq_from_tag(q, tag);
	printk("%s, rq: %p\n", __func__, rq);

	gdisk_cptr.cptr = fipc_get_reg2(_request);

	if (gdisk_cptr.cptr) {
		ret = glue_cap_lookup_gendisk_type(c_cspace,
				gdisk_cptr,
				&gdisk_container);

		if (ret) {
			LIBLCD_ERR("lookup");
			goto fail_lookup;
		}
		bd_disk = &gdisk_container->gendisk;
	}

	at_head = fipc_get_reg3(_request);

	func_ret = blk_execute_rq(q, bd_disk, rq, at_head);
	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;

}

int bdput_callee(struct fipc_message *_request)
{
	struct block_device *bdev = NULL;
	int ret = 0;
	struct block_device_container *bdev_container;

	ret = glue_cap_lookup_block_device_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&bdev_container);

	if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	bdev = bdev_container->bdev;
	bdput(bdev);

fail_lookup:
	return ret;
}

int bdget_disk_callee(struct fipc_message *_request)
{
	struct gendisk *disk = NULL;
	int partno = 0;
	int ret = 0;
	struct block_device_container *bdev_container = NULL;
	struct block_device *bdev;
	struct gendisk_container *gdisk_container;

	ret = glue_cap_lookup_gendisk_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &gdisk_container);

	if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	partno = fipc_get_reg1(_request);

	disk = gdisk_container->gdisk;

	bdev = bdget_disk(disk, partno);

	glue_lookup_bdevice(__cptr((unsigned long) bdev), &bdev_container);

	fipc_set_reg0(_request, bdev_container->other_ref.cptr);

fail_lookup:
	return ret;
}

int __class_create_callee(struct fipc_message *_request)
{
	struct class_container *cls_container = NULL;
	struct lock_class_key key;
	int ret = 0;
	struct class *class;

	cls_container = kzalloc(sizeof( *cls_container ), GFP_KERNEL);

	if (!cls_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	class = __class_create(THIS_MODULE, DRVNAME, &key);

	cls_container->class_p = class;
	cls_container->other_ref.cptr = fipc_get_reg0(_request);

	ret = glue_cap_insert_class_type(c_cspace, cls_container,
				&cls_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	fipc_set_reg0(_request, cls_container->my_ref.cptr);
fail_alloc:
fail_insert:
	return ret;
}

int class_destroy_callee(struct fipc_message *_request)
{
	struct class_container *cls_container = NULL;
	int ret = 0;

	ret = glue_cap_lookup_class_type(c_cspace, __cptr(fipc_get_reg0(_request)),
				&cls_container);
	if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	class_destroy(cls_container->class_p);

fail_lookup:
	return ret;
}

int device_create_callee(struct fipc_message *_request)
{
	struct device *parent = NULL;
	unsigned long devt = 0;
	void *drvdata = NULL;
	struct device_container *func_ret_container = NULL;
	struct device *func_ret = NULL;
	struct class_container *cls_container = NULL;
	struct device_container *device_container = NULL;
	int ret = 0;
	struct class *class;
	int instance;

	ret = glue_cap_lookup_class_type(c_cspace, __cptr(fipc_get_reg0(_request)),
				&cls_container);
	if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	class = cls_container->class_p;
	func_ret_container = kzalloc(sizeof( struct device_container   ),
		GFP_KERNEL);
	if (!func_ret_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	func_ret_container->other_ref.cptr = fipc_get_reg3(_request);

	glue_lookup_device(__cptr(fipc_get_reg1(_request)), &device_container);

	parent = device_container->dev;

	instance = fipc_get_reg4(_request);

	func_ret = device_create(class, parent, devt, drvdata, "nvme_lcd%d", instance);
	func_ret_container->dev = func_ret;

	glue_insert_device(func_ret_container);

	fipc_set_reg0(_request, func_ret_container->my_ref.cptr);

fail_lookup:
fail_alloc:
	return ret;
}

int device_destroy_callee(struct fipc_message *_request)
{
	struct class *class = NULL;
	struct class_container *cls_container;
	unsigned long devt = 0;
	int ret = 0;

	ret = glue_cap_lookup_class_type(c_cspace, __cptr(fipc_get_reg0(_request)),
				&cls_container);
	if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }


	devt = fipc_get_reg1(_request);
	device_destroy(class, devt);

fail_lookup:
	return ret;
}

int capable_callee(struct fipc_message *_request)
{
	int ret = 0;
	int cap;
	bool yes;

	cap = fipc_get_reg0(_request);
	yes = capable(cap);

	fipc_set_reg0(_request, yes);

	return ret;
}

int revalidate_disk_callee(struct fipc_message *_request)
{
	int ret;
	int func_ret;
	struct gendisk *disk = NULL;
	struct gendisk_container *gdisk_container;

	ret = glue_cap_lookup_gendisk_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &gdisk_container);

	if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	disk = gdisk_container->gdisk;

	func_ret = revalidate_disk(disk);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}
