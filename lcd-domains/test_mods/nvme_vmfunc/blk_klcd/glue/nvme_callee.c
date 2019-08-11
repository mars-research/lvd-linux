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
#include <linux/sort.h>
#include <linux/aer.h>
#include <lcd_domains/microkernel.h>

#include <lcd_config/post_hook.h>

#define HANDLE_IRQ_LOCALLY

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
struct request **rq_map;

#ifndef CONFIG_LVD
struct trampoline_hidden_args {
	void *struct_container;
	struct glue_cspace *cspace;
	struct lcd_trampoline_handle *t_handle;
	struct thc_channel *async_chnl;
	cptr_t sync_ep;
};
#endif

static struct glue_cspace *c_cspace;
extern struct cspace *klcd_cspace;

#define IOCB_POOL_SIZE		(256UL << 20)
#define IOCB_POOL_PAGES		(IOCB_POOL_SIZE >> PAGE_SHIFT)
#define IOCB_POOL_ORDER		ilog2(IOCB_POOL_PAGES)
#define QUEUE_RQ_BUF_SIZE	4096

/* Create a shadow copy of the user data into LCDs */
#define CONFIG_COPY_USER_DATA
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

int nvme_pci_reg_read32(struct nvme_ctrl *ctrl, u32 off, u32 *val,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct nvme_ctrl_ops_container *nvme_ctrl_ops_ctr =
			hidden_args->struct_container;
	struct nvme_ctrl_container *nvme_ctrl_c;

	nvme_ctrl_c = container_of(ctrl, struct nvme_ctrl_container, nvme_ctrl);

	async_msg_set_fn_type(request, NVME_PCI_REG_READ32);
	fipc_set_reg0(request, nvme_ctrl_ops_ctr->other_ref.cptr);
	fipc_set_reg1(request, nvme_ctrl_c->other_ref.cptr);
	fipc_set_reg2(request, off);

	vmfunc_klcd_wrapper(request, 1);
	*val =  fipc_get_reg0(request);
	return 0;
}

LCD_TRAMPOLINE_DATA(reg_read32_trampoline);
int LCD_TRAMPOLINE_LINKAGE(reg_read32_trampoline) reg_read32_trampoline(struct nvme_ctrl *ctrl, u32 off, u32 *val)

{
	int ( *volatile pci_reg_read32_fp )(struct nvme_ctrl *, u32, u32 *,
				struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, reg_read32_trampoline);
	pci_reg_read32_fp = nvme_pci_reg_read32;
	return pci_reg_read32_fp(ctrl, off, val, hidden_args);
}

int nvme_pci_reg_write32(struct nvme_ctrl *ctrl, u32 off, u32 val,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct nvme_ctrl_ops_container *nvme_ctrl_ops_ctr =
			hidden_args->struct_container;
	struct nvme_ctrl_container *nvme_ctrl_c;

	nvme_ctrl_c = container_of(ctrl, struct nvme_ctrl_container,
			nvme_ctrl);

	async_msg_set_fn_type(request, NVME_PCI_REG_WRITE32);
	fipc_set_reg0(request, nvme_ctrl_ops_ctr->other_ref.cptr);
	fipc_set_reg1(request, nvme_ctrl_c->other_ref.cptr);
	fipc_set_reg2(request, off);
	fipc_set_reg3(request, val);

	vmfunc_klcd_wrapper(request, 1);
	return 0;
}

LCD_TRAMPOLINE_DATA(reg_write32_trampoline);
int LCD_TRAMPOLINE_LINKAGE(reg_write32_trampoline) reg_write32_trampoline(struct nvme_ctrl *ctrl, u32 off, u32 val)

{
	int ( *volatile pci_reg_write32_fp )(struct nvme_ctrl *, u32, u32,
				struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, reg_write32_trampoline);
	pci_reg_write32_fp = nvme_pci_reg_write32;
	return pci_reg_write32_fp(ctrl, off, val, hidden_args);
}

int nvme_pci_reg_read64(struct nvme_ctrl *ctrl, u32 off, u64 *val,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct nvme_ctrl_ops_container *nvme_ctrl_ops_ctr =
			hidden_args->struct_container;
	struct nvme_ctrl_container *nvme_ctrl_c;

	nvme_ctrl_c = container_of(ctrl, struct nvme_ctrl_container,
			nvme_ctrl);

	async_msg_set_fn_type(request, NVME_PCI_REG_READ64);
	fipc_set_reg0(request, nvme_ctrl_ops_ctr->other_ref.cptr);
	fipc_set_reg1(request, nvme_ctrl_c->other_ref.cptr);
	fipc_set_reg2(request, off);

	vmfunc_klcd_wrapper(request, 1);
	*val =  fipc_get_reg0(request);
	return 0;
}

LCD_TRAMPOLINE_DATA(reg_read64_trampoline);
int LCD_TRAMPOLINE_LINKAGE(reg_read64_trampoline) reg_read64_trampoline(struct nvme_ctrl *ctrl, u32 off, u64 *val)

{
	int ( *volatile pci_reg_read64_fp )(struct nvme_ctrl *, u32, u64 *,
				struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, reg_read64_trampoline);
	pci_reg_read64_fp = nvme_pci_reg_read64;
	return pci_reg_read64_fp(ctrl, off, val, hidden_args);
}

int nvme_pci_reset_ctrl(struct nvme_ctrl *ctrl,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct nvme_ctrl_ops_container *nvme_ctrl_ops_ctr =
			hidden_args->struct_container;
	struct nvme_ctrl_container *nvme_ctrl_c;

	nvme_ctrl_c = container_of(ctrl, struct nvme_ctrl_container,
			nvme_ctrl);

	async_msg_set_fn_type(request, NVME_PCI_RESET_CTRL);

	fipc_set_reg0(request, nvme_ctrl_ops_ctr->other_ref.cptr);
	fipc_set_reg1(request, nvme_ctrl_c->other_ref.cptr);

	vmfunc_klcd_wrapper(request, 1);

	return 0;
}

LCD_TRAMPOLINE_DATA(reset_ctrl_trampoline);
int LCD_TRAMPOLINE_LINKAGE(reset_ctrl_trampoline) reset_ctrl_trampoline(struct nvme_ctrl *ctrl)

{
	int ( *volatile pci_reset_ctrl_fp )(struct nvme_ctrl *,
				struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, reset_ctrl_trampoline);
	pci_reset_ctrl_fp = nvme_pci_reset_ctrl;
	return pci_reset_ctrl_fp(ctrl, hidden_args);
}

int nvme_pci_free_ctrl(struct nvme_ctrl *ctrl,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct nvme_ctrl_ops_container *nvme_ctrl_ops_ctr =
			hidden_args->struct_container;
	struct nvme_ctrl_container *nvme_ctrl_c;

	nvme_ctrl_c = container_of(ctrl, struct nvme_ctrl_container,
			nvme_ctrl);

	async_msg_set_fn_type(request, NVME_PCI_FREE_CTRL);

	fipc_set_reg0(request, nvme_ctrl_ops_ctr->other_ref.cptr);
	fipc_set_reg1(request, nvme_ctrl_c->other_ref.cptr);

	vmfunc_klcd_wrapper(request, 1);

	return 0;
}

LCD_TRAMPOLINE_DATA(free_ctrl_trampoline);
int LCD_TRAMPOLINE_LINKAGE(free_ctrl_trampoline) free_ctrl_trampoline(struct nvme_ctrl *ctrl)

{
	int ( *volatile pci_free_ctrl_fp )(struct nvme_ctrl *,
				struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, free_ctrl_trampoline);
	pci_free_ctrl_fp = nvme_pci_free_ctrl;
	return pci_free_ctrl_fp(ctrl, hidden_args);
}

int nvme_pci_post_scan(struct nvme_ctrl *ctrl,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct nvme_ctrl_ops_container *nvme_ctrl_ops_ctr =
			hidden_args->struct_container;
	struct nvme_ctrl_container *nvme_ctrl_c;

	nvme_ctrl_c = container_of(ctrl, struct nvme_ctrl_container,
			nvme_ctrl);

	async_msg_set_fn_type(request, NVME_PCI_POST_SCAN);

	fipc_set_reg0(request, nvme_ctrl_ops_ctr->other_ref.cptr);
	fipc_set_reg1(request, nvme_ctrl_c->other_ref.cptr);

	vmfunc_klcd_wrapper(request, 1);

	return 0;
}

LCD_TRAMPOLINE_DATA(post_scan_trampoline);
int LCD_TRAMPOLINE_LINKAGE(post_scan_trampoline) post_scan_trampoline(struct nvme_ctrl *ctrl)

{
	int ( *volatile pci_post_scan_fp )(struct nvme_ctrl *,
				struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, post_scan_trampoline);
	pci_post_scan_fp = nvme_pci_post_scan;
	return pci_post_scan_fp(ctrl, hidden_args);
}

int nvme_pci_submit_async_event(struct nvme_ctrl *ctrl, int aer_idx,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct nvme_ctrl_ops_container *nvme_ctrl_ops_ctr =
			hidden_args->struct_container;
	struct nvme_ctrl_container *nvme_ctrl_c;

	nvme_ctrl_c = container_of(ctrl, struct nvme_ctrl_container,
			nvme_ctrl);

	async_msg_set_fn_type(request, NVME_PCI_SUBMIT_ASYNC_EVENT);

	fipc_set_reg0(request, nvme_ctrl_ops_ctr->other_ref.cptr);
	fipc_set_reg1(request, nvme_ctrl_c->other_ref.cptr);
	fipc_set_reg1(request, aer_idx);

	vmfunc_klcd_wrapper(request, 1);

	return 0;
}

LCD_TRAMPOLINE_DATA(submit_async_event_trampoline);
int LCD_TRAMPOLINE_LINKAGE(submit_async_event_trampoline) submit_async_event_trampoline(struct nvme_ctrl *ctrl, int aer_idx)

{
	int ( *volatile pci_submit_async_event_fp )(struct nvme_ctrl *, int,
				struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, submit_async_event_trampoline);
	pci_submit_async_event_fp = nvme_pci_submit_async_event;
	return pci_submit_async_event_fp(ctrl, aer_idx, hidden_args);
}

int blk_mq_init_queue_callee(struct fipc_message *request)
{
	struct request_queue *rq;
	int ret = 0;
	struct blk_mq_tag_set_container *set_container;
        struct request_queue_container *rq_container;
	struct nvme_ctrl_container *nvme_ctrl_container;
	struct nvme_ctrl *ctrl;

	ret = glue_cap_lookup_blk_mq_tag_set_type(c_cspace,
			__cptr(fipc_get_reg0(request)), &set_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg2(request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	ctrl = &nvme_ctrl_container->nvme_ctrl;
	ctrl->admin_q = rq = blk_mq_init_queue(&set_container->tag_set);

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

	rq_container->other_ref.cptr = fipc_get_reg1(request);

	fipc_set_reg0(request, rq_container->my_ref.cptr);

	return ret;

fail_blk:
fail_lookup:
fail_insert:
	return ret;
}

int blk_mq_end_request_callee(struct fipc_message *request)
{
	struct request *rq;
	int ret = 0;
	int error = 0;

	rq = rq_map[fipc_get_reg0(request)];

	error = fipc_get_reg1(request);

	blk_mq_end_request(rq, error);
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

int blk_mq_start_request_callee(struct fipc_message *request)
{

	struct request *rq;
	int ret = 0;
	int tag = fipc_get_reg0(request);
	rq = rq_map[tag];

	blk_mq_start_request(rq);

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

	async_msg_set_fn_type(request, BLK_MQ_START_STOPPED_HW_QUEUES);

	async = fipc_get_reg1(request);

	vmfunc_klcd_wrapper(request, 1);

	blk_mq_start_stopped_hw_queues(&rq_container->request_queue, async);

fail_lookup:
	return ret;
}

int blk_mq_complete_request_callee(struct fipc_message *request)
{
    struct request *rq;
	int ret = 0;
    int error;
	int tag = fipc_get_reg0(request);
    rq = rq_map[tag];
    error = fipc_get_reg1(request);

    blk_mq_complete_request(rq, error);

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
	struct request_queue_container *rq_container;
	int ret;

	size = fipc_get_reg0(request);
	ret = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg1(request)), &rq_container);

	if (ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}

	blk_queue_logical_block_size(&rq_container->request_queue, size);

fail_lookup:
	return ret;
}

int blk_mq_tagset_busy_iter_callee(struct fipc_message *_request)
{
	LCD_MSG("%s, called", __func__);
	return 0;
}

/*

int blk_mq_tags_cpumask_callee(struct fipc_message *request)
{

    struct blk_mq_tags_container *tags_c = container_of(tags, struct blk_mq_tags_container, blk_mq_tags);

    fipc_set_reg0(request, tags_c->other_ref.cptr);

    async_msg_set_fn_type(request, BLK_MQ_TAGS_CPUMASK);

    vmfunc_klcd_wrapper(request, 1);

    return (int*)fipc_get_reg0(request);
}
*/

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
	struct module_container *module_container;
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

	ret = glue_cap_lookup_module_type(c_cspace, __cptr(fipc_get_reg2(request)),
                                        &module_container);
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
	glue_cap_remove(c_cspace, module_container->my_ref);
	kfree(blo_container);
	kfree(module_container);

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

	glue_cap_remove(c_cspace, rq_container->my_ref);
fail_lookup:
	return ret;
}

int _queue_rq_fn(struct blk_mq_hw_ctx *ctx, const struct blk_mq_queue_data *bd, struct trampoline_hidden_args *hidden_args)
{
        int ret;
	struct fipc_message r;
        struct fipc_message *request = &r;
	struct bio_vec_queue *bvec_queue = &per_cpu(bio_vec_queues, smp_processor_id());
	struct bio_vec *bvec_array = bvec_queue->bvec_array;
#ifdef CONFIG_COPY_USER_DATA
	int i = 0;
#endif

#ifndef CONFIG_LVD
        struct blk_mq_ops_container *ops_container;
#endif
        struct blk_mq_hw_ctx_container *ctx_container;

        /*XXX Beware!! hwctx can be unique per hw context of the driver, if multiple
         * exists, then we need one cspace insert function per hwctx. Should be handled
         * in the init_hctx routine */

        ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container,
					blk_mq_hw_ctx);
#ifndef CONFIG_LVD
        ops_container = (struct blk_mq_ops_container *)hidden_args->struct_container;
#endif

        async_msg_set_fn_type(request, QUEUE_RQ_FN);

        fipc_set_reg0(request, ctx->queue_num);
        fipc_set_reg1(request, ctx_container->other_ref.cptr);

#ifndef CONFIG_LVD
        fipc_set_reg2(request, ops_container->other_ref.cptr);
#endif
	fipc_set_reg3(request, bd->rq->tag);

#ifdef CONFIG_COPY_USER_DATA
	{
		struct bio *bio = bd->rq->bio;
		struct bio_vec bvec;
		struct bvec_iter iter;

		for_each_bio(bio) {
			bio_for_each_segment(bvec, bio, iter) {
				void *buf = page_address(bvec.bv_page);
				void *lcd_buf = priv_alloc(BLK_USER_BUF_POOL);
				if (lcd_buf)
					memcpy(lcd_buf, buf + bvec.bv_offset, bvec.bv_len);
				bvec_array[i] = bvec;
				bvec_array[i].bv_page = (struct page *) lcd_buf;
				i++;
			}
		}
	}
	i--;
#endif

	vmfunc_klcd_wrapper(request, 1);

#ifdef CONFIG_COPY_USER_DATA
	{
		for (; i >= 0; i--) {
			void *lcd_buf = (void*) bvec_array[i].bv_page;
			if (lcd_buf)
				priv_free(lcd_buf, BLK_USER_BUF_POOL);
		}
	}
#endif
        ret = fipc_get_reg0(request);

        return ret;
}

int _init_request_fn(void *data, struct request *req, unsigned int hctx_idx,
			unsigned int rq_idx,
			unsigned int numa_node,
			struct trampoline_hidden_args *hidden_args)
{
	static int init_done = 0;

	if(!init_done) {
		rq_map = kzalloc((sizeof(struct request *) * (hw_depth + 1)),
				GFP_KERNEL);
		if(!rq_map) {
			LIBLCD_ERR("cannot allocate mem for rq_map");
			return -1;
		}
		init_done = 1;
	}

	rq_map[rq_idx] = req;
	printk("init_request: req %p req->tag %d rq_idx %d \n",req, req->tag, rq_idx);
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
	struct blk_mq_ops_container *ops_container = hidden_args->struct_container;

	ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container, blk_mq_hw_ctx);
	ops_container = (struct blk_mq_ops_container *)hidden_args->struct_container;

	ret = glue_cap_insert_blk_mq_hw_ctx_type(c_cspace, ctx_container, &ctx_container->my_ref);
	if (ret) {
		LIBLCD_ERR("klcd insert");
		goto fail_insert;
	}

	vmfunc_klcd_wrapper(request, 1);

	async_msg_set_fn_type(request, INIT_HCTX_FN);
	fipc_set_reg0(request, ctx_container->my_ref.cptr);
	fipc_set_reg1(request, index);

	/* ops container is passed to call ops.init_hctx in the LCD glue */
	fipc_set_reg2(request, ops_container->other_ref.cptr);

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

int _exit_hctx_fn(struct blk_mq_hw_ctx *ctx, unsigned int index, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;
	int func_ret = 0;
	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container = hidden_args->struct_container;

	ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container, blk_mq_hw_ctx);
	ops_container = (struct blk_mq_ops_container *)hidden_args->struct_container;

	ret = glue_cap_insert_blk_mq_hw_ctx_type(c_cspace, ctx_container,
			&ctx_container->my_ref);
	if (ret) {
		LIBLCD_ERR("klcd insert");
		goto fail_insert;
	}

	vmfunc_klcd_wrapper(request, 1);

	async_msg_set_fn_type(request, EXIT_HCTX_FN);
	fipc_set_reg0(request, ctx_container->my_ref.cptr);
	fipc_set_reg1(request, index);

	/* ops container is passed to call ops.exit_hctx in the LCD glue */
	fipc_set_reg2(request, ops_container->other_ref.cptr);

	vmfunc_klcd_wrapper(request, 1);

	ctx_container->other_ref.cptr = fipc_get_reg0(request);
	func_ret = fipc_get_reg1(request);

	printk("exit_hctx hctx %p | cptr %lu\n", ctx, ctx_container->other_ref.cptr);

fail_insert:
	return func_ret;
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

void _complete_fn(struct request *request,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	async_msg_set_fn_type(_request, SOFTIRQ_DONE_FN);

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

void _poll_fn(struct request *request, bool reserved,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	async_msg_set_fn_type(_request, POLL_FN);
	fipc_set_reg1(_request, reserved);

	vmfunc_klcd_wrapper(_request, 1);

	return;
}

LCD_TRAMPOLINE_DATA(poll_fn_trampoline);
void LCD_TRAMPOLINE_LINKAGE(poll_fn_trampoline) poll_fn_trampoline(struct request
			*request, bool reserved)

{
	void ( *volatile poll_fn_fp )(struct request *, bool,
				struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, poll_fn_trampoline);
	poll_fn_fp = _poll_fn;
	return poll_fn_fp(request, reserved, hidden_args);
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
	g_blk_mq_ops_container = ops_container = kzalloc(sizeof(struct blk_mq_ops_container), GFP_KERNEL);
	if (!set_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc2;
	}

	err = glue_cap_insert_blk_mq_ops_type(c_cspace, ops_container, &ops_container->my_ref);
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

int open(struct block_device *device, fmode_t mode,
			struct trampoline_hidden_args *hidden_args)
{
	int ret = 0;
	struct fipc_message r;
	struct fipc_message *request = &r;

	async_msg_set_fn_type(request, OPEN);
	fipc_set_reg0(request, mode);

	printk("%s, %s:%d on cpu:%d\n", __func__, current->comm, current->pid,
				smp_processor_id());
	vmfunc_klcd_wrapper(request, 1);

	ret = fipc_get_reg0(request);

	return ret;
}

void release(struct gendisk *disk, fmode_t mode,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct gendisk_container *disk_container;

	disk_container = container_of(disk, struct gendisk_container, gendisk);

	printk("%s, %s:%d on cpu:%d\n", __func__, current->comm, current->pid,
				smp_processor_id());

	async_msg_set_fn_type(request, RELEASE);
	fipc_set_reg0(request, disk_container->other_ref.cptr);
	fipc_set_reg1(request, mode);

	vmfunc_klcd_wrapper(request, 1);

	return;
}

void setup_nvme_ctrl_ops(struct nvme_ctrl_ops_container *
			nvme_ctrl_ops_container)
{
	struct trampoline_hidden_args *drv_reg_read32_hargs;
	struct trampoline_hidden_args *drv_reg_write32_hargs;
	struct trampoline_hidden_args *drv_reg_read64_hargs;
	struct trampoline_hidden_args *drv_reset_ctrl_hargs;
	struct trampoline_hidden_args *drv_free_ctrl_hargs;
	struct trampoline_hidden_args *drv_post_scan_hargs;
	struct trampoline_hidden_args *drv_submit_async_event_hargs;
	int ret;

	drv_reg_read32_hargs = kzalloc(sizeof(struct trampoline_hidden_args),
					GFP_KERNEL);
	drv_reg_write32_hargs = kzalloc(sizeof(struct trampoline_hidden_args),
					GFP_KERNEL);
	drv_reg_read64_hargs = kzalloc(sizeof(struct trampoline_hidden_args),
					GFP_KERNEL);
	drv_reset_ctrl_hargs = kzalloc(sizeof(struct trampoline_hidden_args),
					GFP_KERNEL);
	drv_free_ctrl_hargs = kzalloc(sizeof(struct trampoline_hidden_args),
					GFP_KERNEL);
	drv_post_scan_hargs = kzalloc(sizeof(struct trampoline_hidden_args),
					GFP_KERNEL);
	drv_submit_async_event_hargs = kzalloc(sizeof(struct trampoline_hidden_args),
					GFP_KERNEL);

	/* reg_read32 */
	drv_reg_read32_hargs->t_handle = LCD_DUP_TRAMPOLINE(reg_read32_trampoline);
	if (!drv_reg_read32_hargs->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	drv_reg_read32_hargs->t_handle->hidden_args = drv_reg_read32_hargs;
	drv_reg_read32_hargs->struct_container = nvme_ctrl_ops_container;
	nvme_ctrl_ops_container->nvme_ctrl_ops.reg_read32 =
		LCD_HANDLE_TO_TRAMPOLINE(drv_reg_read32_hargs->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_reg_read32_hargs->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(reg_read32_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	/* reg_write32 */
	drv_reg_write32_hargs->t_handle = LCD_DUP_TRAMPOLINE(reg_write32_trampoline);
	if (!drv_reg_write32_hargs->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	drv_reg_write32_hargs->t_handle->hidden_args = drv_reg_write32_hargs;
	drv_reg_write32_hargs->struct_container = nvme_ctrl_ops_container;
	nvme_ctrl_ops_container->nvme_ctrl_ops.reg_write32 =
		LCD_HANDLE_TO_TRAMPOLINE(drv_reg_write32_hargs->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_reg_write32_hargs->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(reg_write32_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	/* reg_read64 */
	drv_reg_read64_hargs->t_handle = LCD_DUP_TRAMPOLINE(reg_read64_trampoline);
	if (!drv_reg_read64_hargs->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}

	drv_reg_read64_hargs->t_handle->hidden_args = drv_reg_read64_hargs;
	drv_reg_read64_hargs->struct_container = nvme_ctrl_ops_container;
	nvme_ctrl_ops_container->nvme_ctrl_ops.reg_read64 =
		LCD_HANDLE_TO_TRAMPOLINE(drv_reg_read64_hargs->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_reg_read64_hargs->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(reg_read64_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	/* reset_ctrl */
	drv_reset_ctrl_hargs->t_handle = LCD_DUP_TRAMPOLINE(reset_ctrl_trampoline);
	if (!drv_reset_ctrl_hargs->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	drv_reset_ctrl_hargs->t_handle->hidden_args = drv_reset_ctrl_hargs;
	drv_reset_ctrl_hargs->struct_container = nvme_ctrl_ops_container;
	nvme_ctrl_ops_container->nvme_ctrl_ops.reset_ctrl =
		LCD_HANDLE_TO_TRAMPOLINE(drv_reset_ctrl_hargs->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_reset_ctrl_hargs->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(reset_ctrl_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	/* free_ctrl */
	drv_free_ctrl_hargs->t_handle = LCD_DUP_TRAMPOLINE(free_ctrl_trampoline);
	if (!drv_free_ctrl_hargs->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	drv_free_ctrl_hargs->t_handle->hidden_args = drv_free_ctrl_hargs;
	drv_free_ctrl_hargs->struct_container = nvme_ctrl_ops_container;
	nvme_ctrl_ops_container->nvme_ctrl_ops.free_ctrl =
		LCD_HANDLE_TO_TRAMPOLINE(drv_free_ctrl_hargs->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_free_ctrl_hargs->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(free_ctrl_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	/* post_scan */
	drv_post_scan_hargs->t_handle = LCD_DUP_TRAMPOLINE(post_scan_trampoline);
	if (!drv_post_scan_hargs->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	drv_post_scan_hargs->t_handle->hidden_args = drv_post_scan_hargs;
	drv_post_scan_hargs->struct_container = nvme_ctrl_ops_container;
	nvme_ctrl_ops_container->nvme_ctrl_ops.post_scan =
		LCD_HANDLE_TO_TRAMPOLINE(drv_post_scan_hargs->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_post_scan_hargs->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(post_scan_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	/* submit_async_event */
	drv_submit_async_event_hargs->t_handle = LCD_DUP_TRAMPOLINE(submit_async_event_trampoline);
	if (!drv_submit_async_event_hargs->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	drv_submit_async_event_hargs->t_handle->hidden_args = drv_submit_async_event_hargs;
	drv_submit_async_event_hargs->struct_container = nvme_ctrl_ops_container;
	nvme_ctrl_ops_container->nvme_ctrl_ops.submit_async_event =
		LCD_HANDLE_TO_TRAMPOLINE(drv_submit_async_event_hargs->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_submit_async_event_hargs->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(submit_async_event_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
fail_dup1:
	return;
}

void tear_down_nvme_ctrl_ops(struct nvme_ctrl_ops_container *
			nvme_ctrl_ops_container)
{
	struct trampoline_hidden_args *drv_reg_read32_hargs;
	struct trampoline_hidden_args *drv_reg_write32_hargs;
	struct trampoline_hidden_args *drv_reg_read64_hargs;
	struct trampoline_hidden_args *drv_reset_ctrl_hargs;
	struct trampoline_hidden_args *drv_free_ctrl_hargs;
	struct trampoline_hidden_args *drv_post_scan_hargs;
	struct trampoline_hidden_args *drv_submit_async_event_hargs;
	struct nvme_ctrl_ops *ctrl_ops = &nvme_ctrl_ops_container->nvme_ctrl_ops;

	/* reg_read32 */
	drv_reg_read32_hargs = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(
			ctrl_ops->reg_read32);

	kfree(drv_reg_read32_hargs->t_handle);
	kfree(drv_reg_read32_hargs);

	/* reg_write32 */
	drv_reg_write32_hargs = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(
			ctrl_ops->reg_write32);

	kfree(drv_reg_write32_hargs->t_handle);
	kfree(drv_reg_write32_hargs);

	/* reg_read64 */
	drv_reg_read64_hargs = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(
			ctrl_ops->reg_read64);

	kfree(drv_reg_read64_hargs->t_handle);
	kfree(drv_reg_read64_hargs);

	/* reset_ctrl */
	drv_reset_ctrl_hargs = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(
			ctrl_ops->reset_ctrl);

	kfree(drv_reset_ctrl_hargs->t_handle);
	kfree(drv_reset_ctrl_hargs);

	/* free_ctrl */
	drv_free_ctrl_hargs = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(
			ctrl_ops->free_ctrl);

	kfree(drv_free_ctrl_hargs->t_handle);
	kfree(drv_free_ctrl_hargs);

	/* post_scan */
	drv_post_scan_hargs = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(
			ctrl_ops->post_scan);

	kfree(drv_post_scan_hargs->t_handle);
	kfree(drv_post_scan_hargs);

	/* submit_async_event */
	drv_submit_async_event_hargs = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(
			ctrl_ops->submit_async_event);

	kfree(drv_submit_async_event_hargs->t_handle);
	kfree(drv_submit_async_event_hargs);

	/* free the ctrl_ops_container at the end */
	kfree(nvme_ctrl_ops_container);
}

int nvme_init_ctrl_callee(struct fipc_message *_request)
{
	struct pci_dev_container *pdev_container = NULL;
	struct device *dev;
	int ret = 0;
	kernel_ulong_t driver_data;
	struct nvme_ctrl_ops_container *nvme_ctrl_ops_container;
	struct nvme_ctrl_container *nvme_ctrl_container;

	nvme_ctrl_ops_container = kzalloc(sizeof(*nvme_ctrl_ops_container),
				GFP_KERNEL);

	if (!nvme_ctrl_ops_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc1;
	}

	ret = glue_cap_insert_nvme_ctrl_ops_type(c_cspace, nvme_ctrl_ops_container,
				&nvme_ctrl_ops_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	nvme_ctrl_ops_container->other_ref.cptr = fipc_get_reg0(_request);

	nvme_ctrl_container = kzalloc(sizeof(*nvme_ctrl_container),
				GFP_KERNEL);

	if (!nvme_ctrl_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_nvme_ctrl_type(c_cspace, nvme_ctrl_container,
				&nvme_ctrl_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	nvme_ctrl_container->other_ref.cptr = fipc_get_reg1(_request);

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg2(_request)),
			&pdev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	dev = &pdev_container->pdev->dev;
	driver_data = fipc_get_reg3(_request);

	setup_nvme_ctrl_ops(nvme_ctrl_ops_container);

	ret = nvme_init_ctrl(&nvme_ctrl_container->nvme_ctrl, dev,
			&nvme_ctrl_ops_container->nvme_ctrl_ops, driver_data);
	fipc_set_reg0(_request, ret);
	fipc_set_reg1(_request, nvme_ctrl_ops_container->my_ref.cptr);
	fipc_set_reg2(_request, nvme_ctrl_container->my_ref.cptr);
fail_insert:
fail_lookup:
fail_alloc:
fail_alloc1:
	return 0;
}

int nvme_uninit_ctrl_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_ops_container *nvme_ctrl_ops_container;
	struct nvme_ctrl_container *nvme_ctrl_container;
	int ret = 0;

	ret = glue_cap_lookup_nvme_ctrl_ops_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_ops_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg1(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	nvme_uninit_ctrl(&nvme_ctrl_container->nvme_ctrl);

	tear_down_nvme_ctrl_ops(nvme_ctrl_ops_container);

fail_lookup:
	return ret;
}

int nvme_change_ctrl_state_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	int new_state;
	bool changed;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	new_state = fipc_get_reg1(_request);

	changed = nvme_change_ctrl_state(&nvme_ctrl_container->nvme_ctrl,
			new_state);

	fipc_set_reg0(_request, changed);
fail_lookup:
	return ret;
}

int nvme_stop_queues_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	nvme_stop_queues(&nvme_ctrl_container->nvme_ctrl);

fail_lookup:
	return ret;
}

int nvme_put_ctrl_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	nvme_put_ctrl(&nvme_ctrl_container->nvme_ctrl);

fail_lookup:
	return ret;
}

int nvme_queue_async_events_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	nvme_queue_async_events(&nvme_ctrl_container->nvme_ctrl);

fail_lookup:
	return ret;
}

int nvme_kill_queues_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	nvme_kill_queues(&nvme_ctrl_container->nvme_ctrl);

fail_lookup:
	return ret;
}

int nvme_enable_ctrl_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	struct nvme_ctrl *ctrl;
	u64 cap;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	cap = fipc_get_reg1(_request);

	ctrl = &nvme_ctrl_container->nvme_ctrl;
	ret = nvme_enable_ctrl(ctrl, cap);

	fipc_set_reg0(_request, ret);
	fipc_set_reg1(_request, ctrl->page_size);
fail_lookup:
	return ret;
}

int nvme_disable_ctrl_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	u64 cap;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	cap = fipc_get_reg1(_request);

	ret = nvme_disable_ctrl(&nvme_ctrl_container->nvme_ctrl, cap);

	fipc_set_reg0(_request, ret);
fail_lookup:
	return ret;
}

int nvme_shutdown_ctrl_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	ret = nvme_shutdown_ctrl(&nvme_ctrl_container->nvme_ctrl);

	fipc_set_reg0(_request, ret);
fail_lookup:
	return ret;
}

int nvme_init_identify_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	ret = nvme_init_identify(&nvme_ctrl_container->nvme_ctrl);

	fipc_set_reg0(_request, ret);

fail_lookup:
	return ret;
}

int nvme_set_queue_count_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	int count;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	count = fipc_get_reg1(_request);

	ret = nvme_set_queue_count(&nvme_ctrl_container->nvme_ctrl, &count);

	fipc_set_reg0(_request, ret);
	fipc_set_reg1(_request, count);

fail_lookup:
	return ret;
}

int nvme_queue_scan_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	nvme_queue_scan(&nvme_ctrl_container->nvme_ctrl);

fail_lookup:
	return ret;
}

int nvme_start_queues_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	nvme_start_queues(&nvme_ctrl_container->nvme_ctrl);

fail_lookup:
	return ret;
}

int nvme_remove_namespaces_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	nvme_remove_namespaces(&nvme_ctrl_container->nvme_ctrl);

fail_lookup:
	return ret;
}

int blk_mq_stop_hw_queues_callee(struct fipc_message *_request)
{
	struct nvme_ctrl_container *nvme_ctrl_container;
	struct request_queue *rq;
	int ret;

	ret = glue_cap_lookup_nvme_ctrl_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&nvme_ctrl_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	rq = nvme_ctrl_container->nvme_ctrl.admin_q;

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
	struct pci_dev_container *pdev_container = NULL;
	int ret;

	LIBLCD_MSG("%s, called", __func__);

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&pdev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	dev = &pdev_container->pdev->dev;

	func_ret = get_device(dev);

fail_lookup:
	return ret;
}


int put_device_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	struct pci_dev_container *pdev_container = NULL;
	int ret;

	LIBLCD_MSG("%s, called", __func__);

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&pdev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	dev = &pdev_container->pdev->dev;

	put_device(dev);

fail_lookup:
	return ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(open_trampoline);
int LCD_TRAMPOLINE_LINKAGE(open_trampoline) open_trampoline(struct block_device *device, fmode_t mode)

{
	int ( *volatile open_fp )(struct block_device *, fmode_t , struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, open_trampoline);
	open_fp = open;
	return open_fp(device, mode, hidden_args);
}
#endif

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(release_trampoline);
void LCD_TRAMPOLINE_LINKAGE(release_trampoline) release_trampoline(struct gendisk *disk, fmode_t mode)

{
	void ( *volatile rel_fp )(struct gendisk *, fmode_t , struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, release_trampoline);
	rel_fp = release;
	return rel_fp(disk, mode, hidden_args);
}
#endif

int device_add_disk_callee(struct fipc_message *request)
{
	struct gendisk_container *disk_container;
	struct block_device_operations_container *blo_container;
	struct request_queue_container *rq_container;
	struct gendisk *disk;
#ifndef CONFIG_LVD
	struct trampoline_hidden_args *open_hargs;
	struct trampoline_hidden_args *rel_hargs;
#endif
	struct module_container *module_container;

	int ret;
	char disk_name[DISK_NAME_LEN];

	/* hack for now - hardcoding it here, ran out of regs! */
	sector_t size;

	ret = glue_cap_lookup_gendisk_type(c_cspace, __cptr(fipc_get_reg0(request)),
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

	blo_container->other_ref.cptr = fipc_get_reg1(request);

	ret = glue_cap_insert_blk_dev_ops_type(c_cspace, blo_container, &blo_container->my_ref);
	if(ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert1;
	}

	module_container =  kzalloc(sizeof(*module_container), GFP_KERNEL);
	if(!module_container) {
		LIBLCD_ERR("alloc failed");
		goto fail_alloc2;
	}

	module_container->other_ref.cptr = fipc_get_reg2(request);

    /*
     * Some special module inits required:
     *
     *   -- module refcnt = reference count (changed to atomic, no
     *   percpu alloc needed as in 3.12 (pmfs))
     *   -- module state = MODULE_STATE_LIVE
     *   -- module name = "pmfs"
     *
     * These are normally done by the module loader. But since we
     * are creating our own struct module instance, we need to do
     * the initialization ourselves.
     */
    module_container->module.state = MODULE_STATE_LIVE;

	/* without this add_disk will fail */
	atomic_inc(&module_container->module.refcnt);

    strcpy(module_container->module.name, "nvme");

	ret = glue_cap_insert_module_type( c_cspace, module_container,
			&module_container->my_ref);
    if (ret) {
            LIBLCD_ERR("insert");
            goto fail_insert2;
    }

	/* setup the fops */
	blo_container->block_device_operations.owner = &module_container->module;

#ifndef CONFIG_LVD
        open_hargs = kzalloc(sizeof(*open_hargs), GFP_KERNEL);
        if (!open_hargs) {
                LIBLCD_ERR("kzalloc hidden args");
                goto fail_alloc4;
        }

        open_hargs->t_handle = LCD_DUP_TRAMPOLINE(open_trampoline);
        if (!open_hargs->t_handle) {
                LIBLCD_ERR("duplicate trampoline");
                goto fail_dup1;
        }

        open_hargs->t_handle->hidden_args = open_hargs;
        open_hargs->struct_container = blo_container;
        blo_container->block_device_operations.open = LCD_HANDLE_TO_TRAMPOLINE(open_hargs->t_handle);

        ret = set_memory_x(((unsigned long)open_hargs->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(open_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem nx");
                goto fail_x1;
        }

	rel_hargs = kzalloc(sizeof(*rel_hargs), GFP_KERNEL);
        if (!rel_hargs) {
                LIBLCD_ERR("kzalloc hidden args");
                goto fail_alloc5;
        }

        rel_hargs->t_handle = LCD_DUP_TRAMPOLINE(release_trampoline);
        if (!rel_hargs->t_handle) {
                LIBLCD_ERR("duplicate trampoline");
                goto fail_dup2;
        }

	rel_hargs->t_handle->hidden_args = rel_hargs;
        rel_hargs->struct_container = blo_container;

        blo_container->block_device_operations.release = LCD_HANDLE_TO_TRAMPOLINE(rel_hargs->t_handle);

        ret = set_memory_x(((unsigned long)rel_hargs->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(release_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem nx");
                goto fail_x2;
        }
#endif
	disk = &disk_container->gendisk;
	printk("address of disk before calling add_diks %p \n",disk);

	disk->flags = fipc_get_reg4(request);
	disk->major = fipc_get_reg5(request);
	disk->first_minor = fipc_get_reg6(request);
	disk->queue = &rq_container->request_queue;

	size = 250 * 1024 * 1024 * 1024ULL;
	set_capacity(disk, size >> 9);
#if 0
	blo_container->block_device_operations.open = open;
	blo_container->block_device_operations.release = release;
#endif
	disk->fops = &blo_container->block_device_operations;
	sprintf(disk_name, "nvme%d", disk->first_minor);
	strncpy(disk->disk_name, disk_name, DISK_NAME_LEN);

	printk("Calling add_disk on cpu: %d, disk %p \n", raw_smp_processor_id(), disk);

	add_disk(disk);

	{
		struct kobject *kobj = &disk_to_dev(disk)->kobj;
		printk("%s, disk_to_dev(disk) %p, kobj %p, kobj->sd %p\n", __func__,
				disk_to_dev(disk), kobj, kobj->sd);
	}
	fipc_set_reg0(request, blo_container->my_ref.cptr);
	fipc_set_reg1(request, module_container->my_ref.cptr);

	return ret;

#ifndef CONFIG_LVD
fail_x2:
fail_dup2:
	kfree(rel_hargs);
fail_alloc5:
fail_x1:
fail_dup1:
	kfree(open_hargs);
fail_alloc4:
#endif
	glue_cap_remove(c_cspace, module_container->my_ref);
fail_insert2:
	kfree(module_container);
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
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;
	int ret;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	pci_disable_msi(pdev);

	LIBLCD_MSG("%s returned");
fail_lookup:
	return ret;
}

int pci_disable_msix_callee(struct fipc_message *_request)
{
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;
	int ret;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	pci_disable_msix(pdev);

	LIBLCD_MSG("%s returned");
fail_lookup:
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
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg4(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

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

fail_lookup:
	return ret;
}

int device_release_driver_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct pci_dev_container *dev_container;
	struct device *dev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
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
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
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
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	LCD_MSG("%s Looking up with pdev_other.cptr %lx", __func__,
			__cptr(fipc_get_reg5(_request)));

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg5(_request)),
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
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	LIBLCD_MSG("%s, called", __func__);

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg2(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

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

fail_lookup:
fail_vol:
	return 0;
}

int probe(struct pci_dev *dev,
		const struct pci_device_id *id,
		struct trampoline_hidden_args *hidden_args)
{
	struct pci_dev_container *dev_container;
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

	/*
	 * Kernel does not give us pci_dev with a container wrapped in.
	 * So, let's have a pointer!
	 */
	ret = glue_cap_insert_pci_dev_type(c_cspace, dev_container,
			&dev_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, PROBE);

	fipc_set_reg0(_request, pdrv_container->other_ref.cptr);
	fipc_set_reg1(_request, dev_container->my_ref.cptr);
	fipc_set_reg2(_request, *dev->dev.dma_mask);
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

	return func_ret;

fail_insert:
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
	INIT_IPC_MSG(&r);

	async_msg_set_fn_type(_request, REMOVE);

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
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	func_ret = pci_disable_pcie_error_reporting(pdev);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
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
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	func_ret = pci_cleanup_aer_uncorrect_error_status(pdev);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_disable_device_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	pci_disable_device(pdev);

fail_lookup:
	return ret;
}

int pci_enable_pcie_error_reporting_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	func_ret = pci_enable_pcie_error_reporting(pdev);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pcie_capability_read_word_callee(struct fipc_message *_request)
{
	int pos;
	unsigned short val;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	pos = fipc_get_reg1(_request);
	val = fipc_get_reg2(_request);

	func_ret = pcie_capability_read_word(pdev, pos, &val);

	fipc_set_reg0(_request, func_ret);

	fipc_set_reg2(_request, val);

fail_lookup:
	return ret;
}

int pcie_get_minimum_link_callee(struct fipc_message *_request)
{
	enum pci_bus_speed speed;
	enum pcie_link_width width;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	speed = fipc_get_reg1(_request);
	width = fipc_get_reg2(_request);

	func_ret = pcie_get_minimum_link(pdev, &speed, &width);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_enable_device_mem_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	func_ret = pci_enable_device_mem(pdev);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_request_selected_regions_callee(struct fipc_message *_request)
{
	int type;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	type = fipc_get_reg1(_request);

	func_ret = pci_request_selected_regions(pdev,
				type,
				DRVNAME);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_request_selected_regions_exclusive_callee(struct fipc_message *_request)
{
	int type;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	type = fipc_get_reg1(_request);

	func_ret = pci_request_selected_regions_exclusive(pdev,
				type,
				DRVNAME);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_set_master_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	pci_set_master(pdev);

fail_lookup:
	return ret;
}

int pci_save_state_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	func_ret = pci_save_state(pdev);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_release_selected_regions_callee(struct fipc_message *_request)
{
	int r;
	int ret = 0;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	r = fipc_get_reg1(_request);
	pci_release_selected_regions(pdev, r);

fail_lookup:
	return ret;
}

int pci_select_bars_callee(struct fipc_message *_request)
{
	unsigned long flags;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	flags = fipc_get_reg1(_request);

	func_ret = pci_select_bars(pdev, flags);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_wake_from_d3_callee(struct fipc_message *_request)
{
	bool enable;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;
	enable = fipc_get_reg1(_request);

	func_ret = pci_wake_from_d3(pdev, enable);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
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
	struct irqhandler_t_container *irqhandler_data;
}irq_map[32];

int reg_irqs;

int request_threaded_irq_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret = 0;
	int irq;
	unsigned long flags;
	struct irqhandler_t_container *irqhandler_container;
	unsigned char *vector_name;

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

	memcpy(vector_name, (void *)&_request->regs[3], sizeof(unsigned long) * 3);

	LIBLCD_MSG("%s, request_threaded_irq for %d | name: %s",
			__func__, irq, vector_name);

#if 1
	func_ret = request_threaded_irq(irq, msix_vector_handler,
				NULL, flags,
				vector_name, (void*) irqhandler_container);
#endif
	func_ret = 0;
	irq_map[reg_irqs].irq = irq;
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
	int i;

	irq = fipc_get_reg0(_request);

	LIBLCD_MSG("%s, freeing irq %d", __func__, irq);

	for (i = 0; i < 32; i++) {
		if (irq_map[i].irq == irq) {
			irqhandler_container = irq_map[i].irqhandler_data;
			break;
		}
	}

	if (!irqhandler_container)
		printk("%s unable to retrieve container data for irq %d",
				__func__, irq);
	free_irq(irq, irqhandler_container);
	reg_irqs--;

	return ret;
}

int synchronize_irq_callee(struct fipc_message *_request)
{
	unsigned int irq;

	irq = fipc_get_reg0(_request);

	synchronize_irq(irq);

	return 0;
}
