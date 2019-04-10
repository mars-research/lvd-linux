#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/trampoline.h>
#include "../../glue_helper.h"
#include "../../benchmark.h"
#include "../nullb_callee.h"
#include <asm/cacheflush.h>
#include <linux/atomic.h>

#include <lcd_domains/user_interface.h>

#include <lcd_config/post_hook.h>

/* hacks for unregistering */
int null_major;
struct gendisk *disk_g;
struct request_queue *rq_g;
struct blk_mq_tag_set *set_g;

extern struct request_queue *queue_nullb;

/* hack for init_request */
int hw_depth;
struct request **rq_map;

struct async_item {
	int pid;
	struct task_struct *task;
	struct list_head list;
};

static LIST_HEAD(async_list);

/* file operations for setting up data memory from user space*/
struct file_operations nullb_user_fops;

/* info as private_data field for user communication */
#define MAX_INFO_ENTRIES 100
struct lcd_user_info {
	struct page *p[MAX_INFO_ENTRIES];
	unsigned int order[MAX_INFO_ENTRIES];
};

static DEFINE_SPINLOCK(lock);

/* to handle null ptstate */
bool init_ptstate = false;

DEFINE_MUTEX(async_ctx_lock);
/* benchmarking */
INIT_BENCHMARK_DATA(async_reply);

//INIT_MARKER(blk_mq_end_req);

struct trampoline_hidden_args {
	void *struct_container;
	struct glue_cspace *cspace;
	struct lcd_trampoline_handle *t_handle;
	struct thc_channel *async_chnl;
	cptr_t sync_ep;
};

static struct glue_cspace *c_cspace;
extern struct cspace *klcd_cspace;

static int setup_new_channel(struct thc_channel *async_channel, cptr_t sync_ep);

static void add_async_item(struct async_item *item)
{
	INIT_LIST_HEAD(&item->list);
	list_add_tail(&item->list, &async_list);
}

int glue_blk_init(void)
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
	return 0;
fail2:
	glue_cap_exit();
fail1:
	return ret;

}

void glue_blk_exit(void)
{
	BENCH_COMPUTE_STAT(async_reply);
	//BENCH_DUMP_DATA(ipc_call);
	//MARKER_DUMP(blk_mq_end_req);
	glue_cap_destroy(c_cspace);
	glue_cap_exit();

}

/* Send a async message for graceful exit */
void destroy_lcd(struct thc_channel *chnl)
{
	printk("cleanup...\n");
	unregister_blkdev(null_major, "nullb"); 

	if(disk_g) {
		printk("calling del_gendisk \n");
		del_gendisk(disk_g);
	}
	
	if(rq_g) {
		printk("calling blk_cleanup \n");
		blk_cleanup_queue(rq_g);
	}
	
	queue_nullb = NULL;
	if(set_g) {
		printk("calling free tag set \n");
		blk_mq_free_tag_set(set_g);
	}

	if(disk_g) {
		printk("calling put_disk \n");
		put_disk(disk_g);
	}

	lcd_teardown_chardev();
	printk("cleanup done\n");
}	

void blk_exit(struct thc_channel *channel) 
{
	destroy_lcd(channel);
}

int blk_mq_init_queue_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	struct fipc_message *response;
	unsigned int request_cookie;
	struct request_queue *rq;
	cptr_t set_ref = __cptr(fipc_get_reg0(request));
	int ret = 0;
	struct blk_mq_tag_set_container *set_container;
        struct request_queue_container *rq_container;

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = glue_cap_lookup_blk_mq_tag_set_type(c_cspace, set_ref, &set_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	printk("in blk glue - calling the real blk_mq_init \n");

	rq = blk_mq_init_queue(&set_container->blk_mq_tag_set);
	if(!rq) {
		LIBLCD_ERR("blk layer returned bad address!");
		goto fail_blk;
	}

	/* Hack for remove */
	rq_g = rq;
	queue_nullb = rq;
	//queue_nullb = NULL;

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		goto fail_async;
	}
	
        rq_container = container_of(rq, struct request_queue_container,
                                                request_queue);

	printk("inserting cap of rq_container \n");
	ret = glue_cap_insert_request_queue_type(c_cspace, rq_container, &rq_container->my_ref);
        if (ret) {
                LIBLCD_ERR("lcd insert");
                goto fail_insert;
        }
	rq_container->other_ref.cptr = fipc_get_reg1(request);

	thc_ipc_reply(channel, request_cookie, response);
	fipc_set_reg0(response, rq_container->my_ref.cptr);
	printk("blk klcd done! \n");	
	return ret;

fail_async:
fail_blk:	
fail_lookup:
	glue_cap_remove(c_cspace, rq_container->my_ref);	
fail_insert:
	return ret;	
}

int blk_mq_end_request_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	struct request *rq;
	//struct fipc_message *response;
	//unsigned int request_cookie;
	int ret = 0;
	int error = 0;

	//request_cookie = thc_get_request_cookie(request);
	
	rq = rq_map[fipc_get_reg0(request)];	

	error = fipc_get_reg1(request);
	
	//printk("[KLCD] END_REQ for rq_map_tag %ld rq->tag %d req: %p\n", fipc_get_reg0(request), rq->tag, rq);	
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	//BENCH_BEGIN(async_reply);
	blk_mq_end_request(rq, error);
	//BENCH_END(async_reply);
	
	//printk("[KLCD] END_REQ_B4_SND_START slot: %ld \n", atomic64_read(&thc_channel_to_fipc(channel)->tx.slot));
	//BENCH_BEGIN(async_reply);
	//if (async_msg_blocking_send_start(channel, &response)) {
	//	LIBLCD_ERR("error getting response msg");
	//	ret = -EIO;
	//	goto async_ipc;

	//}
	
	//async_msg_set_fn_type(response, BLK_MQ_END_REQUEST);
	//printk("[KLCD] END_REQ_A4_SND_START slot: %ld \n", atomic64_read(&thc_channel_to_fipc(channel)->tx.slot));
	//ret = thc_ipc_reply(channel, request_cookie, response);
	//if(ret) {
	//	LIBLCD_ERR("error thc_ipc_reply");
	//	goto async_reply;
	//}

	//BENCH_END(async_reply);
	//printk("[KLCD] END_REQ done \n");

//async_reply:	
//async_ipc:
	return ret;
}

int blk_mq_free_tag_set_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{

	struct fipc_message *response;
	unsigned int request_cookie;
	struct blk_mq_tag_set_container *set_container;
	struct blk_mq_ops_container *ops_container;
	cptr_t set_ref = __cptr(fipc_get_reg0(request));
	int ret;
	
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

        ret = glue_cap_lookup_blk_mq_tag_set_type(c_cspace, set_ref, &set_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }
	ret = glue_cap_lookup_blk_mq_ops_type(c_cspace, set_ref, &ops_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }
	
	blk_mq_free_tag_set(&set_container->blk_mq_tag_set);
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		ret = -EIO;
		goto fail_async;
	}
	thc_ipc_reply(channel, request_cookie, response);

	glue_cap_remove(c_cspace, set_container->my_ref);
	glue_cap_remove(c_cspace, ops_container->my_ref);
	kfree(ops_container);
	kfree(set_container);

	return ret;

fail_async:
fail_lookup:
	return ret;
}

int blk_mq_start_request_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{

	struct request *rq;
	//struct fipc_message *response;
	//unsigned int request_cookie;
	int ret = 0;
	int tag = fipc_get_reg0(request);
	//MARKER_BEGIN(blk_mq_end_req);	
	//request_cookie = thc_get_request_cookie(request);
	//BENCH_BEGIN(async_reply);

	//WARN_ON(tag >= hw_depth);

	rq = rq_map[tag];
	
	//printk("[KLCD] START_REQ for rq_map_tag %ld rq->tag %d rq: %p \n", fipc_get_reg0(request), rq->tag, rq);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
	//BENCH_END(async_reply);
	//BENCH_BEGIN(async_reply);
	blk_mq_start_request(rq);
	
	//blk_mq_end_request(rq, 0);
	//BENCH_END(async_reply);
	

	//printk("[KLCD] START_REQ_B4_SND_START slot: %ld \n", atomic64_read(&thc_channel_to_fipc(channel)->tx.slot));
	//if (async_msg_blocking_send_start(channel, &response)) {
	//	LIBLCD_ERR("error getting response msg");
	//ret = -EIO;
	//	goto async_exit;
	//}
	
	//printk("[KLCD] START_REQ_A4_SND_START slot: %ld \n", atomic64_read(&thc_channel_to_fipc(channel)->tx.slot));
	//ret = thc_ipc_reply(channel, request_cookie, response);
	//if(ret) {
	//LIBLCD_ERR("error thc_ipc_reply");
	//	goto async_reply;
	//}
	
	//printk("[KLCD] START_REQ done\n");

//async_reply:
//async_exit:

	//BENCH_END(async_reply);
	return ret;
}

int blk_mq_map_queue_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
#if 0
	struct request_queue *rq;
	int ctx_index;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	struct blk_mq_hw_ctx *func_ret;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long driver_data_offset;
	cptr_t driver_data_cptr;
	gva_t driver_data_gva;

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
	rq = kzalloc(*sizeof( rq ), GFP_KERNEL);
	if (!rq) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ctx_index = fipc_get_reg2(request);
	func_ret = kzalloc(*sizeof( func_ret ), GFP_KERNEL);
	if (!func_ret) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = blk_mq_map_queue(rq, ctx_index);
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg3(response, func_ret->driver_data);
	thc_ipc_reply(channel, request_cookie, response);
	return ret;
#endif
	return 0;
}

int blk_queue_logical_block_size_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	unsigned short size;
	struct fipc_message *response;
	unsigned int request_cookie;
	struct request_queue_container *rq_container;
	int ret;
	
	request_cookie = thc_get_request_cookie(request);
	
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
	
	size = fipc_get_reg0(request);
	ret = glue_cap_lookup_request_queue_type(c_cspace, __cptr(fipc_get_reg1(request)),
					&rq_container);
	if(ret) {
		 LIBLCD_ERR("lookup");
		 goto fail_lookup;
	}	

	blk_queue_logical_block_size(&rq_container->request_queue, size);
	
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		goto fail_async;
	}
	
	thc_ipc_reply(channel, request_cookie, response);
	return ret;

fail_async:
fail_lookup:
	return ret;
}

int blk_queue_physical_block_size_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	unsigned int size;
	struct fipc_message *response;
	unsigned int request_cookie;
	struct request_queue_container *rq_container;
	int ret;

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
	
	size = fipc_get_reg0(request);
	ret = glue_cap_lookup_request_queue_type(c_cspace, __cptr(fipc_get_reg1(request)),
			&rq_container);
	if(ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	blk_queue_physical_block_size(&rq_container->request_queue, size);
	
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		goto fail_async;
	}
	thc_ipc_reply(channel, request_cookie, response);
	
	return ret;

fail_async:
fail_lookup:
	return ret;
}

int alloc_disk_node_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	int minors;
	int node_id;
	int ret;
	unsigned int request_cookie;
	struct fipc_message *response;
	struct gendisk *disk;
	struct gendisk_container *disk_container;

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	minors = fipc_get_reg0(request);
	node_id = fipc_get_reg1(request);
	
	disk = alloc_disk_node(minors, node_id);
	if(!disk) {
		LIBLCD_ERR("call to alloc_disk_node failed");
		goto fail_alloc;
	}
	printk("address of disk returned by the kernel %p \n",disk);
	disk_container = container_of(disk, struct gendisk_container, gendisk);

	/* Hack for remove */
	disk_g = disk;

	ret = glue_cap_insert_gendisk_type(c_cspace, disk_container, &disk_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	disk_container->other_ref.cptr = fipc_get_reg2(request);
	
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		goto fail_async;
	}

	fipc_set_reg0(response, disk_container->my_ref.cptr);
	thc_ipc_reply(channel, request_cookie, response);

	return ret;

fail_async:
fail_insert:
fail_alloc:
	glue_cap_remove(c_cspace, disk_container->my_ref);
	return ret;
}


int put_disk_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	struct gendisk *disk;
	struct gendisk_container *disk_container;
	struct block_device_operations_container *blo_container;
	struct module_container *module_container;
	struct fipc_message *response;
	unsigned int request_cookie;
	int ret;

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

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

 	/* disk_container may be deleted after the call to put_disk,
	 * so remove from cspace here */
	glue_cap_remove(c_cspace, disk_container->my_ref);

	put_disk(disk);

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		ret = -EIO;
		goto fail_async;
	}

	thc_ipc_reply(channel, request_cookie, response);
	glue_cap_remove(c_cspace, blo_container->my_ref);
	glue_cap_remove(c_cspace, module_container->my_ref);
	kfree(blo_container);
	kfree(module_container);

	return ret;

fail_async:
fail_lookup:
	return ret;
}

int del_gendisk_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	struct gendisk *gp;
	struct gendisk_container *disk_container;
	struct fipc_message *response;
	unsigned int request_cookie;
	int ret;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
	
        ret = glue_cap_lookup_gendisk_type(c_cspace, __cptr(fipc_get_reg0(request)),
                                        &disk_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	gp = &disk_container->gendisk;

	del_gendisk(gp);
	
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(channel, request_cookie, response);

	return ret;

fail_lookup:
	return ret;
}

static int setup_async_fs_ring_channel(cptr_t tx, cptr_t rx,
                                struct thc_channel **chnl_out)
{
        gva_t tx_gva, rx_gva;
        int ret;
        struct fipc_ring_channel *fchnl;
        struct thc_channel *chnl;
        unsigned int pg_order = PMFS_ASYNC_RPC_BUFFER_ORDER - PAGE_SHIFT;
        /*
         * Map tx and rx buffers (caller has already prep'd buffers)
         */
        ret = lcd_map_virt(tx, pg_order, &tx_gva);
        if (ret) {
                LIBLCD_ERR("failed to map tx");
                goto fail1;
        }
        ret = lcd_map_virt(rx, pg_order, &rx_gva);
        if (ret) {
                LIBLCD_ERR("failed to map rx");
                goto fail2;
        }
        /*
         * Alloc and init channel header
         */
        fchnl = kmalloc(sizeof(*fchnl), GFP_KERNEL);
        if (!fchnl) {
                ret = -ENOMEM;
                LIBLCD_ERR("malloc failed");
                goto fail3;
        }
        ret = fipc_ring_channel_init(fchnl,
                                PMFS_ASYNC_RPC_BUFFER_ORDER,
                                /* (note: gva == hva for non-isolated) */
                                (void *)gva_val(tx_gva),
                                (void *)gva_val(rx_gva));
        if (ret) {
                LIBLCD_ERR("channel init failed");
                goto fail4;
        }
        /*
         * Add to async channel group
         */
        chnl = kzalloc(sizeof(*chnl), GFP_KERNEL);
        if (!chnl) {
                ret = -ENOMEM;
                LIBLCD_ERR("malloc failed");
                goto fail5;
        }
        ret = thc_channel_init(chnl, fchnl);
        if (ret) {
                LIBLCD_ERR("async group item init failed");
                goto fail6;
        }

        *chnl_out = chnl;
        return 0;

fail6:
        kfree(chnl);
fail5:
fail4:
        kfree(fchnl);
fail3:
        lcd_unmap_virt(rx_gva, pg_order);
fail2:
        lcd_unmap_virt(tx_gva, pg_order);
fail1:
        return ret;
}

static void destroy_async_fs_ring_channel(struct thc_channel *chnl)
{
        cptr_t tx, rx;
        gva_t tx_gva, rx_gva;
        unsigned long unused1, unused2;
        int ret;
        unsigned int pg_order = PMFS_ASYNC_RPC_BUFFER_ORDER - PAGE_SHIFT;
        /*
         * Translate ring buffers to cptrs
         */
        tx_gva = __gva((unsigned long)thc_channel_to_fipc(chnl)->tx.buffer);
        rx_gva = __gva((unsigned long)thc_channel_to_fipc(chnl)->rx.buffer);
        ret = lcd_virt_to_cptr(tx_gva, &tx, &unused1, &unused2);
        if (ret) {
                LIBLCD_ERR("failed to translate tx to cptr");
                goto fail1;
        }
        ret = lcd_virt_to_cptr(rx_gva, &rx, &unused1, &unused2);
        if (ret) {
                LIBLCD_ERR("failed to translate rx to cptr");
                goto fail2;
        }
        /*
         * Unmap and kill tx/rx
         */
        lcd_unmap_virt(tx_gva, pg_order);
        lcd_cap_delete(tx);
        lcd_unmap_virt(rx_gva, pg_order);
        lcd_cap_delete(rx);
        /*
         * Free chnl header
         */
        kfree(thc_channel_to_fipc(chnl));
        /*
         * Free the thc channel
         *
         * XXX: We are assuming this is called *from the dispatch loop*
         * (i.e., as part of handling a callee function), so no one
         * else (no other awe) is going to try to use the channel
         * after we kill it. (For the PMFS LCD, this is not possible,
         * because the unregister happens from a *caller context*.)
         */
        kfree(chnl);

        return;

fail2:
fail1:
        return;
}

static int nullbu_open(struct inode *inode, struct file *filp)
{
	struct lcd_user_info *info;

	printk("dev opened \n");

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if(!info) {
		printk("cannot create user info \n");
		return -ENOMEM;
	}

	filp->private_data = info;

	return 0;
}

static int nullbu_close (struct inode *inode, struct file *filp)
{
	struct lcd_user_info *info;
	int i = 0;

	info = (struct lcd_user_info *)filp->private_data;
	
	printk("dev closed \n");
	
	for (i = 0; i < MAX_INFO_ENTRIES; i++) {
		if (info->p[i]) {
			free_pages((unsigned long)page_address(info->p[i]), info->order[i]);
		}
	}

	kfree(info);

	return 0;
}

int grant_sync_ep(cptr_t *sync_end, cptr_t ha_sync_ep)
{
        int ret;
        struct cspace *curr_cspace = get_current_cspace(current);

        lcd_cptr_alloc(sync_end);
        ret = cap_grant(klcd_cspace, ha_sync_ep, curr_cspace, *sync_end);

        return ret;
}

static int nullbu_mmap(struct file *filp, struct vm_area_struct *vma, struct trampoline_hidden_args *hidden_args)
{
	int ret = 0;
	struct lcd_user_info *info;
      	struct page *p;
	cptr_t sync_end;
  	unsigned long size = PAGE_ALIGN(vma->vm_end - vma->vm_start);
	unsigned long order = get_order(size);
        unsigned long new_order = 0;
        unsigned long rem_order = 0;
	unsigned long vma_start = vma->vm_start;
	int i = 0;
	int j = 0;

	info = (struct lcd_user_info *)filp->private_data;
        //printk("[MMAP_DEV] begin: %lx end: %lx mmap_size: %ld pgoff: %ld order: %ld\n", vma->vm_start, vma->vm_end, size, vma->vm_pgoff, order);

	lcd_enter();
	ret = grant_sync_ep(&sync_end, hidden_args->sync_ep);

        if(order >= MAX_ORDER) {
                new_order = MAX_ORDER - 1;
                rem_order = order - new_order;
        } else {
                new_order = order;
                rem_order = 0;
        }


        for (i = 0; i < (1 << rem_order); i++) {

                //unsigned long va = 0;
		struct fipc_message *async_request;
		struct fipc_message *async_response;
		cptr_t pg;
		unsigned int request_cookie;

                //Allocate physical pages for the requested size
                //printk("alloc_pages \n");
                p = alloc_pages(GFP_KERNEL | __GFP_ZERO, new_order);
                if(!p) {
                        printk("alloc_page failed \n");
                        ret = -ENOMEM;
                        goto fail_alloc;
                }

                info->p[i] = p;
                info->order[i] = new_order;
                //va = (unsigned long) page_to_virt(p);

                //printk("remapping range \n");
                for(j = 0; j < (1 << new_order); j++) {

                        //printk("vma->flags before: %lx \n", vma->vm_flags);

                        //va += PAGE_SIZE * j;
                        page_ref_inc(p+j);

                        ret = vm_insert_page(vma, vma->vm_start, p+j);
                        //ret = vm_insert_page(vma, vma->vm_start, virt_to_page(va));
                        if (ret < 0) {
                                printk("insert page failed ***************************** %d \n", ret);
                                goto fail_alloc;
                        }
                        vma->vm_flags &= ~(VM_IO | VM_PFNMAP);
                        vma->vm_start += PAGE_SIZE;
                       // printk("vma->flags cleared: %lx \n", vma->vm_flags);
                       // printk("mapping done! \n");
                }

		//Volunteer memory
		//printk("volunteer pages \n");
		ret = lcd_volunteer_pages(p, new_order, &pg);
		if (ret) {
			LIBLCD_ERR("failed to volunteer memory");
			ret = -ENOMEM;
			goto fail_volunteer;
		}

		//printk("volunteer pages done \n");
		ret = async_msg_blocking_send_start(hidden_args->async_chnl, &async_request);
		if (ret) {
			LIBLCD_ERR("failed to get a send slot");
			ret = -EIO;
			goto fail_async;
		}
		
		async_msg_set_fn_type(async_request, MMAP_CHARDEV);
		fipc_set_reg1(async_request, new_order);

		ret = thc_ipc_send_request(hidden_args->async_chnl, async_request, &request_cookie);
		if (ret) {
			LIBLCD_ERR("thc_ipc_call");
			ret = -EIO;
			goto fail_ipc;
		}
		
		lcd_set_cr0(pg);
		ret = lcd_sync_send(sync_end);
		lcd_set_cr0(CAP_CPTR_NULL);
		if (ret) {
			LIBLCD_ERR("failed to send");
			ret = -EIO;
			goto fail_sync;
		}

		//spin_lock(&lock);	
		/* recieve a confirmation from LCD that remap successfully finished,
		 * then map the memory to user process */
		LCD_MAIN(
			DO_FINISH_(mmap_user, {
			  ASYNC_({
			    ret = thc_ipc_recv_response(hidden_args->async_chnl, request_cookie, &async_response);
			  }, mmap_user);
			});  
		);
		if (ret) {
			LIBLCD_ERR("thc_ipc_call");
			ret = -EIO;
			goto fail_reply;
		}    
		
		//spin_unlock(&lock);	
		ret = fipc_get_reg1(async_response);
		if(ret) {
			LIBLCD_ERR("LCD failed to map memory");
			ret = -EIO;
			goto fail_mmap;
		}

		//printk("setting protection \n");
		//vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
		//printk("setting protection done \n");
		fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), async_response);

                //printk("mapping done! \n");
        }

        vma->vm_start = vma_start;
        //printk("[MMAP_DONE] start: %lx end: %lx \n",vma->vm_start, vma->vm_end);

/*
	for (i = 0; i < (1 << order); i++) {
		
		struct fipc_message *async_request;
		struct fipc_message *async_response;
		cptr_t pg;
		unsigned int request_cookie;
		
		//Allocate physical pages for the requested size
		printk("alloc_pages \n");
		p = alloc_pages(GFP_KERNEL | __GFP_ZERO, 0);
		if(!p) {
			printk("alloc_page failed \n");
			ret = -ENOMEM;
			goto fail_alloc;
		}
			
		info->p[i] = p;
		info->order[i] = 0;
		
		printk("vma->flags before: %lx \n", vma->vm_flags);
                ret = vm_insert_page(vma, vma->vm_start, p);
                if (ret < 0) { 
                        printk("insert page failed ***************************** \n");
                        goto fail_alloc;
                }
                vma->vm_flags &= ~(VM_IO | VM_PFNMAP);
                vma->vm_start += PAGE_SIZE;
                printk("vma->flags cleared: %lx \n", vma->vm_flags);
		
		//Volunteer memory
		printk("volunteer pages \n");
		ret = lcd_volunteer_pages(p, 0, &pg);
		if (ret) {
			LIBLCD_ERR("failed to volunteer memory");
			ret = -ENOMEM;
			goto fail_volunteer;
		}

		printk("volunteer pages done \n");
		ret = async_msg_blocking_send_start(hidden_args->async_chnl, &async_request);
		if (ret) {
			LIBLCD_ERR("failed to get a send slot");
			ret = -EIO;
			goto fail_async;
		}
		
		async_msg_set_fn_type(async_request, MMAP_CHARDEV);
		fipc_set_reg1(async_request, 0);

		ret = thc_ipc_send_request(hidden_args->async_chnl, async_request, &request_cookie);
		if (ret) {
			LIBLCD_ERR("thc_ipc_call");
			ret = -EIO;
			goto fail_ipc;
		}
		
		lcd_set_cr0(pg);
		ret = lcd_sync_send(sync_end);
		lcd_set_cr0(CAP_CPTR_NULL);
		if (ret) {
			LIBLCD_ERR("failed to send");
			ret = -EIO;
			goto fail_sync;
		}

		//recieve a confirmation from LCD that remap successfully finished,
		//then map the memory to user process
		LCD_MAIN(
			DO_FINISH_(mmap_user, {
			  ASYNC_({
			    ret = thc_ipc_recv_response(hidden_args->async_chnl, request_cookie, &async_response);
			  }, mmap_user);
			});  
		);
		if (ret) {
			LIBLCD_ERR("thc_ipc_call");
			ret = -EIO;
			goto fail_reply;
		}    
		
		ret = fipc_get_reg1(async_response);
		if(ret) {
			LIBLCD_ERR("LCD failed to map memory");
			ret = -EIO;
			goto fail_mmap;
		}

		//printk("setting protection \n");
		//vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
		//printk("setting protection done \n");
		fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), async_response);
                printk("mapping done! \n");
	}

	vma->vm_start = vma_start;
	printk("[MMAP_DONE] start: %lx end: %lx \n",vma->vm_start, vma->vm_end);
*/
	lcd_cap_delete(sync_end);
	lcd_exit(0);
	return 0;

fail_mmap:
fail_reply:
fail_alloc:
fail_volunteer:
	for(i = 0; i < MAX_INFO_ENTRIES; i++) {
		if(p) {
			free_pages((unsigned long)page_address(p), order);
		}
	}
fail_async:
fail_ipc:
fail_sync:
	lcd_exit(0);
	return ret;
}

LCD_TRAMPOLINE_DATA(mmap_fops_trampoline);
int LCD_TRAMPOLINE_LINKAGE(mmap_fops_trampoline) mmap_fops_trampoline(struct file *filp, struct vm_area_struct *vma)

{
	int ( *volatile mmap_fp )(struct file *filp, struct vm_area_struct *vma, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, mmap_fops_trampoline);
	mmap_fp = nullbu_mmap;
	return mmap_fp(filp, vma, hidden_args);

}

//const struct file_operations nullb_user_fops = {
//        .owner  = THIS_MODULE,
//        .open   = nullbu_open,
//        .release = nullbu_close,
//        .mmap = nullbu_mmap,
//};

int register_blkdev_callee(void)
{
        cptr_t tx, rx;
        //cptr_t tx_aux, rx_aux;
        struct thc_channel *chnl;
        struct thc_channel *aux_chnl = NULL;

        cptr_t sync_endpoint;
        int ret = 0;
        int major;
        struct drv_info *drv_info;
	struct thc_channel_group_item *disp_item;

        sync_endpoint = lcd_cr0();
        tx = lcd_cr1(); rx = lcd_cr2();
        //tx_aux = lcd_cr3(); rx_aux = lcd_cr4();

	/*
         * Set up async ring channel
         */
        ret = setup_async_fs_ring_channel(tx, rx, &chnl);
        if (ret) {
                LIBLCD_ERR("error setting up ring channel");
                goto fail1;
        }
	
	//ret = setup_async_fs_ring_channel(tx_aux, rx_aux, &aux_chnl);
        //if (ret) {
          //      LIBLCD_ERR("error setting up ring channel");
            //    goto fail_aux;
        //}

       	disp_item = kzalloc(sizeof(*disp_item), GFP_KERNEL);
       	if(!disp_item) {
		 LIBLCD_ERR("no mem for disp_item");
		 goto fail2;
	}
	
	/* Store the pointer to thc_channel */
	disp_item->channel = chnl;

	/*
         * Add to dispatch loop
         */
        drv_info = add_drv(disp_item, c_cspace, sync_endpoint, aux_chnl);
        if (!drv_info) {
                LIBLCD_ERR("error adding to dispatch loop");
                goto fail3;
        }
	
	/* add disp_item to the channel group */
	add_chnl_group_item(disp_item, drv_info->ch_grp);

	/* Hardcoded string for now! */
	LIBLCD_MSG("Calling register_blkdev");
	ret = register_blkdev(lcd_r1(), "nullb");
	LIBLCD_MSG("register_blkdev returns %d", ret);
	if(ret < 0) {
		LIBLCD_ERR("Real call to register_blkdev failed!");
		goto fail4;
	} else {
		/* register_blkdev can return the major number of the device,
		 * which can be a large +ve number but the ret value passed
		 * above if found +ve is treated as an error */
		major = ret;
		null_major = major;
		ret = 0;
	}

	goto out;

fail4:
	remove_drv(drv_info);
fail3:
	kfree(disp_item);
fail2:
	destroy_async_fs_ring_channel(chnl);

//fail_aux:
//	destroy_async_fs_ring_channel(aux_chnl);
	//TODO AB - I see a kfree(chnl) in pmfs right after this call,
	//but chnl is already freed inside this function?
fail1:
out:
        /*
         * Flush capability registers
         */
        lcd_set_cr0(CAP_CPTR_NULL);
        lcd_set_cr1(CAP_CPTR_NULL);
        lcd_set_cr2(CAP_CPTR_NULL);

        lcd_set_r0(major);

        if (lcd_sync_reply())
                LIBLCD_ERR("double fault?");
        return ret; 
	
}

int unregister_blkdev_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	unsigned int devno;
	struct fipc_message *response;
	unsigned int request_cookie;
	int ret = 0;

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
	
	devno = fipc_get_reg0(request);
	LIBLCD_MSG("calling unregister blk_dev");
	unregister_blkdev(devno, "nullb");
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(channel, request_cookie, response);

	return ret;
}

int blk_cleanup_queue_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep) 
{
        struct fipc_message *response;
        unsigned int request_cookie;
	struct request_queue_container *rq_container;
        int ret = 0; 

        request_cookie = thc_get_request_cookie(request);
        fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
             
        ret = glue_cap_lookup_request_queue_type(c_cspace, __cptr(fipc_get_reg0(request)),
                                        &rq_container);
        if(ret) {
                 LIBLCD_ERR("lookup");
                 goto fail_lookup;
        }

	blk_cleanup_queue(&rq_container->request_queue);

        if (async_msg_blocking_send_start(channel, &response)) {
                LIBLCD_ERR("error getting response msg");
                return -EIO;
        }
        thc_ipc_reply(channel, request_cookie, response);

        return ret;

fail_lookup:
	return ret;
}

/*queue rq to handle under a different context */
int _queue_rq_fn_ctx(struct blk_mq_hw_ctx *ctx, const struct blk_mq_queue_data *bd, struct trampoline_hidden_args *hidden_args)
{
        int ret = 0;
        struct fipc_message *request;
        struct fipc_message *response;
        struct blk_mq_ops_container *ops_container;
        struct blk_mq_hw_ctx_container *ctx_container;


        thc_init();

        /*XXX Beware!! hwctx can be unique per hw context of the driver, if multiple
         * exists, then we need one cspace insert function per hwctx. Should be handled
         * in the init_hctx routine */
        ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container, blk_mq_hw_ctx);
        ops_container = (struct blk_mq_ops_container *)hidden_args->struct_container;

        printk("hctx %p | cptr %lu\n",ctx, ctx_container->other_ref.cptr);
        ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
        if (ret) {
                LIBLCD_ERR("ctx failed to get a send slot");
                goto fail_async;
        }

        async_msg_set_fn_type(request, QUEUE_RQ_FN);

        fipc_set_reg0(request, ctx->queue_num);
        fipc_set_reg1(request, ctx_container->other_ref.cptr);
        fipc_set_reg2(request, ops_container->other_ref.cptr);

        DO_FINISH_(queue_rq_fn, {
                ASYNC_( {
                ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
                }, queue_rq_fn);
        });

        if (ret) {
                LIBLCD_ERR("ctx thc_ipc_call");
                goto fail_ipc;
        }

        blk_mq_start_request(bd->rq);
        blk_mq_end_request(bd->rq, 0);

        /* function ret -  makes no sense now but keeping it this way! */
        ret = fipc_get_reg0(response);
        fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);

        lcd_exit(0);
        return ret;

fail_async:
fail_ipc:
        lcd_exit(0);
        return ret;
}

int _queue_rq_fn(struct blk_mq_hw_ctx *ctx, const struct blk_mq_queue_data *bd, struct trampoline_hidden_args *hidden_args)
{
        int ret;
        struct fipc_message *request;
        struct fipc_message *response;
        struct blk_mq_ops_container *ops_container;
        struct blk_mq_hw_ctx_container *ctx_container;

        if(!PTS()) {
                LCD_MAIN({
                ret = _queue_rq_fn_ctx(ctx, bd, hidden_args);
                });
                return ret;
        }

        printk("^^^^^^^^^^ [Klcd-queue-rq] enter ^^^^^^^^^^^ \n");
        /*XXX Beware!! hwctx can be unique per hw context of the driver, if multiple
         * exists, then we need one cspace insert function per hwctx. Should be handled
         * in the init_hctx routine */

        ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container, blk_mq_hw_ctx);
        ops_container = (struct blk_mq_ops_container *)hidden_args->struct_container;

        printk("hctx %p | cptr %lu\n",ctx, ctx_container->other_ref.cptr);

        ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
        if (ret) {
                LIBLCD_ERR("failed to get a send slot");
                goto fail_async;
        }

        async_msg_set_fn_type(request, QUEUE_RQ_FN);

        fipc_set_reg0(request, ctx->queue_num);
        fipc_set_reg1(request, ctx_container->other_ref.cptr);
        fipc_set_reg2(request, ops_container->other_ref.cptr);

        ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
        if (ret) {
                LIBLCD_ERR("thc_ipc_call");
                goto fail_ipc;
        }

        blk_mq_start_request(bd->rq);
        blk_mq_end_request(bd->rq, 0);

        /* function ret -  makes no sense now but keeping it this way! */
        ret = fipc_get_reg0(response);
        fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);

        printk("^^^^^ [Klcd-queue-rq] done ^^^^^^ \n");
        return BLK_MQ_RQ_QUEUE_OK;
        //return ret;

fail_async:
fail_ipc:
        printk("[Klcd-queue-rq] done with err \n");
        return ret;
}

//#if 0
static void queue_rq_async_noyield(struct blk_mq_hw_ctx *ctx, struct blk_mq_queue_data_async *bd_async, 
		struct trampoline_hidden_args *hidden_args, struct thc_channel *chnl)
{
	int func_ret;
	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container;
	int count = 0;

	ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container, blk_mq_hw_ctx);
	ops_container = (struct blk_mq_ops_container *)hidden_args->struct_container;

	//BENCH_BEGIN(async_reply);
	while(!list_empty(bd_async->rq_list)) {
		struct request *rq;
		struct blk_mq_queue_data bd;
		struct fipc_message *request;
		struct fipc_message *response;
		int ret; 
	
		//printk("[KLCD] ^^async inside do_fin \n");
		rq = list_first_entry(bd_async->rq_list, struct request, queuelist);
		list_del_init(&rq->queuelist);

		bd.rq = rq;
		bd.list = bd_async->list;
		bd.last = list_empty(bd_async->rq_list);
	
		//WARN_ON(rq_map[rq->tag] != rq);
		
		//printk("[KLCD] ^^async get async slot for rq->tag %d ??? \n", rq->tag);
		//printk("[KLCD] QUEUE_RQ_ASYNC_B4 slot: %ld \n", atomic64_read(&thc_channel_to_fipc(channel)->tx.slot));
		//BENCH_BEGIN(async_reply);
		ret = async_msg_blocking_send_start(chnl, &request);
		if (ret) {
			LIBLCD_ERR("ctx failed to get a send slot");
			goto fail_async;
		}
		
		//printk("[KLCD] ^^async got async slot for rq->tag %d \n", rq->tag);
		async_msg_set_fn_type(request, QUEUE_RQ_FN);

		fipc_set_reg0(request, ctx->queue_num);
		fipc_set_reg1(request, ctx_container->other_ref.cptr);
		fipc_set_reg2(request, ops_container->other_ref.cptr);
		fipc_set_reg3(request, rq->tag);

		/* send message */
		thc_set_msg_type(request, msg_type_request);
    		fipc_send_msg_end(thc_channel_to_fipc(chnl), request);

#ifdef SENDER_DISPATCH_LOOP
		/* for blk_mq_start_request */
    		ret = thc_ipc_recv_resp_noyield(chnl, &response);
	    	if (ret) {
			printk(KERN_ERR "error receiving response\n");
			goto fail_async;
	    	}

//		printk("%s:%d, got msg type %d\n",
//				__func__, __LINE__,
//				async_msg_get_fn_type(response));

		dispatch_async_loop(chnl, response, c_cspace,
				hidden_args->sync_ep);

		/* for blk_mq_end_request */
    		ret = thc_ipc_recv_resp_noyield(chnl, &response);
	    	if (ret) {
			printk(KERN_ERR "error receiving response\n");
			goto fail_async;
	    	}

//		printk("%s:%d, got msg type %d\n",
//				__func__, __LINE__,
//				async_msg_get_fn_type(response));

		dispatch_async_loop(chnl, response, c_cspace,
				hidden_args->sync_ep);
#endif
		/* receive message */
		//BENCH_BEGIN(async_reply);
    		ret = thc_ipc_recv_resp_noyield(chnl, &response);
	    	if (ret) {
			printk(KERN_ERR "thc_ipc_call: error receiving response\n");
			goto fail_async;
	    	}
		//BENCH_END(async_reply);

		/* This should not execute when ipc_call is blocked on reply */
		func_ret = fipc_get_reg0(response);
		fipc_recv_msg_end(thc_channel_to_fipc(chnl), response);
	
		//BENCH_END(async_reply);
		//BENCH_END(async_reply);
		switch (func_ret) {
		case BLK_MQ_RQ_QUEUE_OK:
			bd_async->queued++;
			break;
		case BLK_MQ_RQ_QUEUE_BUSY:
			list_add(&rq->queuelist, bd_async->rq_list);
			__blk_mq_requeue_request(rq);
			break;
		default:
			pr_err("blk-mq: bad return on queue: %d\n", ret);
		case BLK_MQ_RQ_QUEUE_ERROR:
			if(rq) {
				rq->errors = -EIO;
				blk_mq_end_request(rq, rq->errors);
			}
			break;
		}

		if (func_ret == BLK_MQ_RQ_QUEUE_BUSY)
			break;

		/*
		 * We've done the first request. If we have more than 1
		 * left in the list, set dptr to defer issue.
		 */
		if (!bd_async->list && bd_async->rq_list->next != bd_async->rq_list->prev)
			bd_async->list = bd_async->drv_list;

				
				//printk("[KLCD] ^^async out of async \n");
			count++;
	}
	//printk("[KLCD] ^^async out of do fin \n");
	//printk("noyield count %d \n", count);
	//BENCH_END(async_reply);
	
	//BENCH_BEGIN(async_reply);
	//BENCH_END(async_reply);
	return;

fail_async:
//fail_ipc:
	printk("**** this is dangerous **** \n");
	return;

}
//#endif

/* TODO: contains duplicate code. Needs to be merged with other ctx functions */
void queue_rq_async_ctx_klcd(struct blk_mq_hw_ctx *ctx, struct blk_mq_queue_data_async *bd_async, 
		struct trampoline_hidden_args *hidden_args, struct thc_channel *chnl)
{
	int func_ret;
	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container;
	int count = 0;

	ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container, blk_mq_hw_ctx);
	ops_container = (struct blk_mq_ops_container *)hidden_args->struct_container;

	while(!list_empty(bd_async->rq_list)) {
		struct request *rq;
		struct blk_mq_queue_data bd;
		struct fipc_message *request;
		struct fipc_message *response;
		unsigned int request_cookie;
		int ret; 
	
		rq = list_first_entry(bd_async->rq_list, struct request, queuelist);
		list_del_init(&rq->queuelist);

		bd.rq = rq;
		bd.list = bd_async->list;
		bd.last = list_empty(bd_async->rq_list);
		
		ret = async_msg_blocking_send_start(chnl, &request);
		if (ret) {
			LIBLCD_ERR("ctx failed to get a send slot");
			goto fail_async;
		}
		
		async_msg_set_fn_type(request, QUEUE_RQ_FN);

		fipc_set_reg0(request, ctx->queue_num);
		fipc_set_reg1(request, ctx_container->other_ref.cptr);
		fipc_set_reg2(request, ops_container->other_ref.cptr);
		fipc_set_reg3(request, rq->tag);

		ret = thc_ipc_send_request(chnl,
					request, &request_cookie);

		if (ret)
			LIBLCD_ERR("***************async_ctx thc_ipc_send_request *****************");
		ret = thc_ipc_recv_response(chnl, request_cookie, &response);

		if (ret) {
			LIBLCD_ERR("***************async_ctx thc_ipc_call*****************");
				//goto fail_ipc;
		}
			
			/* This should not execute when ipc_call is blocked on reply */
		func_ret = fipc_get_reg0(response);
		fipc_recv_msg_end(thc_channel_to_fipc(chnl), response);
	
		switch (func_ret) {
		case BLK_MQ_RQ_QUEUE_OK:
			bd_async->queued++;
			break;
		case BLK_MQ_RQ_QUEUE_BUSY:
			list_add(&rq->queuelist, bd_async->rq_list);
			__blk_mq_requeue_request(rq);
			break;
		default:
			pr_err("blk-mq: bad return on queue: %d\n", ret);
		case BLK_MQ_RQ_QUEUE_ERROR:
			if(rq) {
				rq->errors = -EIO;
				blk_mq_end_request(rq, rq->errors);
			}
			break;
		}

		if (func_ret == BLK_MQ_RQ_QUEUE_BUSY)
			break;

		/*
		 * We've done the first request. If we have more than 1
		 * left in the list, set dptr to defer issue.
		 */
		if (!bd_async->list && bd_async->rq_list->next != bd_async->rq_list->prev)
			bd_async->list = bd_async->drv_list;

		count++;
	}
	return;

fail_async:
	return;
}

struct cptr sync_ep;
/* sender side dispatch loop callback - called by thc_ipc_recv_req_resp */
int sender_dispatch(struct thc_channel *chnl, struct fipc_message *out, void *arg)
{
	return dispatch_async_loop(chnl, out, c_cspace, sync_ep); 
}


/* Async variant of queue_rq which does DO_FINISH of
 * passing requests to LCD */
void queue_rq_async_ctx(struct blk_mq_hw_ctx *ctx, struct blk_mq_queue_data_async *bd_async, 
		struct trampoline_hidden_args *hidden_args, struct thc_channel *chnl)
{
	int func_ret;
	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container;
	//struct list_head *item;
	int count = 0;
	//int items = 0;

	ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container, blk_mq_hw_ctx);
	ops_container = (struct blk_mq_ops_container *)hidden_args->struct_container;

	//list_for_each(item, bd_async->rq_list) {
	//	items++;
	//	if(items >= 2) {
	//		break;
	//	}
	//}

	DO_FINISH(	
		while(!list_empty(bd_async->rq_list)) {
			struct request *rq;
			struct blk_mq_queue_data bd;
		      	struct fipc_message *request;
        		struct fipc_message *response;
			unsigned int request_cookie;
			int ret; 
		
			//printk("[KLCD] ^^async inside do_fin \n");
			rq = list_first_entry(bd_async->rq_list, struct request, queuelist);
			list_del_init(&rq->queuelist);

			bd.rq = rq;
			bd.list = bd_async->list;
			bd.last = list_empty(bd_async->rq_list);
			
			//WARN_ON(rq_map[rq->tag] != rq);

			//printk("[KLCD] ^^async get async slot for rq->tag %d ??? \n", rq->tag);
			//printk("[KLCD] QUEUE_RQ_ASYNC_B4 slot: %ld \n", atomic64_read(&thc_channel_to_fipc(channel)->tx.slot));
			ret = async_msg_blocking_send_start(chnl, &request);
			if (ret) {
				LIBLCD_ERR("ctx failed to get a send slot");
				goto fail_async;
			}
			
			//printk("[KLCD] ^^async got async slot for rq->tag %d \n", rq->tag);
			async_msg_set_fn_type(request, QUEUE_RQ_FN);

			fipc_set_reg0(request, ctx->queue_num);
			fipc_set_reg1(request, ctx_container->other_ref.cptr);
			fipc_set_reg2(request, ops_container->other_ref.cptr);
			fipc_set_reg3(request, rq->tag);


			ASYNC_( {
				
				//printk("[KLCD] ^^async ipc call -> tag:%d \n",rq->tag);
				/* If KLCD or more than 1 items in list - take the ipc_call route */
				//if(chnl == hidden_args->async_chnl || items > 1) {
				//BENCH_BEGIN(async_reply);
#ifdef SENDER_DISPATCH_LOOP
				ret = thc_ipc_send_request(chnl, request, &request_cookie);
				ret = thc_ipc_recv_req_resp(chnl, &response, request_cookie, sender_dispatch, (void*)rq);

				fipc_recv_msg_end(thc_channel_to_fipc(chnl), response);
#else
				ret = thc_ipc_call(chnl, request, &response);
#endif
				//ret = thc_ipc_call(chnl, request, &response);
				//} else {
					//BENCH_BEGIN(async_reply);
				//	ret = thc_ipc_call_noyield_single_chnl(chnl, request, &response);
				//BENCH_END(async_reply);
				//}
				if (ret) {
					LIBLCD_ERR("***************async_ctx thc_ipc_call*****************");
					//goto fail_ipc;
				}
				
				/* This should not execute when ipc_call is blocked on reply */
				func_ret = fipc_get_reg0(response);
				fipc_recv_msg_end(thc_channel_to_fipc(chnl), response);

				switch (func_ret) {
				case BLK_MQ_RQ_QUEUE_OK:
					bd_async->queued++;
					break;
				case BLK_MQ_RQ_QUEUE_BUSY:
					list_add(&rq->queuelist, bd_async->rq_list);
					__blk_mq_requeue_request(rq);
					break;
				default:
					pr_err("blk-mq: bad return on queue: %d\n", ret);
				case BLK_MQ_RQ_QUEUE_ERROR:
					if(rq) {
						rq->errors = -EIO;
						blk_mq_end_request(rq, rq->errors);
					}
					break;
				}

				if (func_ret == BLK_MQ_RQ_QUEUE_BUSY)
					break;

				/*
				 * We've done the first request. If we have more than 1
				 * left in the list, set dptr to defer issue.
				 */
				if (!bd_async->list && bd_async->rq_list->next != bd_async->rq_list->prev)
					bd_async->list = bd_async->drv_list;

			}, queue_rq_fn_async);
		
	
			//printk("[KLCD] ^^async out of async \n");
			count++;
		}
	);
	//printk("[KLCD] ^^async out of do fin \n");
	//printk("count %d \n", count);

	//BENCH_END(async_reply);
	return;

fail_async:
//fail_ipc:
	printk("**** this is dangerous **** \n");
	return;
}

static void create_async_item(struct task_struct *task)
{
	struct async_item *item;

	item = kzalloc(sizeof(*item), GFP_KERNEL);
	if(!item) {
		printk(KERN_WARNING "alloc failed for async item, release won't release resources \n");
		return;
	}

	//item->info = kzalloc(sizeof(struct lcd_info), GFP_KERNEL);
	//if(!item->info) {
	//	printk(KERN_WARNING "alloc failed for lcdinfo, release won't release resources \n");
//		goto exit;
//	}

//	item->info->lcd = current->lcd;
//	item->info->t[0] = current->lcd_resource_trees[0];
//	item->info->t[1] = current->lcd_resource_trees[1];
//	item->info->ptstate = PTS();
	item->task = current;
	item->pid = current->pid;

	//printk("adding item: %p pid: %d task: %p \n", item, item->pid, item->task);
	//printk("lcd: %p, cptr_cache: %p, t0: %p, t1: %p, ptstate: %p", 
	//		item->task->lcd, item->task->cptr_cache, item->task->lcd_resource_trees[0],
	//		item->task->lcd_resource_trees[1], item->task->ptstate);
	add_async_item(item);

	return;
//exit:
//	kfree(item);
}

void queue_rq_async(struct blk_mq_hw_ctx *ctx, struct blk_mq_queue_data_async *bd_async, struct trampoline_hidden_args *hidden_args)
{
	struct blk_mq_hw_ctx_container *ctx_container;
	struct thc_channel *chnl;
	
	//BENCH_BEGIN(async_reply);
	ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container, blk_mq_hw_ctx);
       	
	if(!PTS()) {
		printk("async thread --> current: %p pid: %d name: %s \n",current, current->pid, current->comm);
		spin_lock(&lock);	
		if(setup_new_channel(hidden_args->async_chnl, hidden_args->sync_ep)) {
			BUG_ON("channel setup failed");
		}
		create_async_item(current);
		//hidden_args->async_chnl = thc_pts_get_chnl();
		chnl = thc_pts_get_chnl();
		spin_unlock(&lock);

       	      	LCD_MAIN({
			queue_rq_async_ctx(ctx, bd_async, hidden_args, chnl);
              	});
        }
        else {
		if(thc_pts_get_state() == true) {
		
			struct list_head *item;
			int items = 0;

			chnl = thc_pts_get_chnl();
			
			list_for_each(item, bd_async->rq_list) {
				items++;
				if(items >= 2) {
					break;
				}	
			}
			
			if (items == 1) {
				queue_rq_async_noyield(ctx, bd_async, hidden_args, chnl);
			 	//LCD_MAIN({
				//hidden_args->async_chnl = thc_pts_get_chnl();
                      		//queue_rq_async_ctx(ctx, bd_async, hidden_args, chnl);                           
              		//});
			//BENCH_END(async_reply);
				return;
			}

		 	LCD_MAIN({
				//hidden_args->async_chnl = thc_pts_get_chnl();
                      		queue_rq_async_ctx(ctx, bd_async, hidden_args, chnl);                           
              		});
		} else {
                	/* TODO fix klcd's async_channel. Value has got modified above */
			printk("queue_rq_async called in klcd ctx \n");
			chnl = hidden_args->async_chnl;
			queue_rq_async_ctx_klcd(ctx, bd_async, hidden_args, chnl);
		}
	}
}

int init_request(void *data, struct request *req, unsigned int hctx_idx, unsigned int rq_idx,
			 unsigned int numa_node)
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

LCD_TRAMPOLINE_DATA(queue_rq_async_fn_trampoline);
void LCD_TRAMPOLINE_LINKAGE(queue_rq_async_fn_trampoline)
queue_rq_async_fn_trampoline(struct blk_mq_hw_ctx *ctx, struct blk_mq_queue_data_async *bd_async) 
{
	void ( *volatile queue_rq_async_fn_fp )(struct blk_mq_hw_ctx *, struct blk_mq_queue_data_async *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, queue_rq_async_fn_trampoline);
	queue_rq_async_fn_fp = queue_rq_async;
	return queue_rq_async_fn_fp(ctx, bd_async, hidden_args);

}

struct blk_mq_hw_ctx *_map_queue_fn(struct request_queue *rq, int m, struct trampoline_hidden_args *hidden_args)
{
	/* In kernel v4.9, this function is no longer registered
	 * in the ops field. Because the kernel itself handles
	 * the map. All the kernel does is to call blk_mq_map_queue.
	 * So instead of going to the LCD, I am going to call
	 * blk_mq_map_queue here! */
	return blk_mq_map_queue(rq, m);
}

LCD_TRAMPOLINE_DATA(map_queue_fn_trampoline);
struct blk_mq_hw_ctx *LCD_TRAMPOLINE_LINKAGE(map_queue_fn_trampoline) map_queue_fn_trampoline(struct request_queue *req_queue, int m)

{
	struct blk_mq_hw_ctx* ( *volatile map_queue_fn_fp )(struct request_queue *, int , struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, map_queue_fn_trampoline);
	map_queue_fn_fp = _map_queue_fn;
	return map_queue_fn_fp(req_queue, m, hidden_args);

}

int _init_hctx_fn_ctx(struct blk_mq_hw_ctx *ctx, void *data, unsigned int index, struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message *request;
	struct fipc_message *response;
	int func_ret;
	int ret;
	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container;
	
	thc_init();
	ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container, blk_mq_hw_ctx); 
	ops_container = (struct blk_mq_ops_container *)hidden_args->struct_container;

	ret = glue_cap_insert_blk_mq_hw_ctx_type(c_cspace, ctx_container, &ctx_container->my_ref);
	if (ret) {
		LIBLCD_ERR("klcd insert");
		goto fail_insert;
	}

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	
	async_msg_set_fn_type(request, INIT_HCTX_FN);
	fipc_set_reg0(request, ctx_container->my_ref.cptr);
	fipc_set_reg1(request, index);
	
	/* ops container is passed to call ops.init_hctx in the LCD glue */
	fipc_set_reg2(request, ops_container->other_ref.cptr);

        DO_FINISH_(init_hctx_fn, {
                ASYNC_( {
                ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
                }, init_hctx_fn);
        });  

        if (ret) {
                LIBLCD_ERR("thc_ipc_call");
                goto fail_ipc;
        }

	ctx_container->other_ref.cptr = fipc_get_reg0(response);
	func_ret = fipc_get_reg1(response);
	
	printk("init_hctx_ctx hctx %p | cptr %lu\n",ctx, ctx_container->other_ref.cptr);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);
	printk("end of init_hctx procedure \n");

	lcd_exit(0);
	return func_ret;
fail_async:
fail_ipc:
fail_insert:
	lcd_exit(0);
	return func_ret;

}

int _init_hctx_fn(struct blk_mq_hw_ctx *ctx, void *data, unsigned int index, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	int func_ret;
	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container;

	if(!PTS()) {
		LIBLCD_MSG("init_hctx from a different context \n");
		LCD_MAIN({
			ret = _init_hctx_fn_ctx(ctx, data, index, hidden_args);				
		});
		return ret;
	}

	ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container, blk_mq_hw_ctx); 
	ops_container = (struct blk_mq_ops_container *)hidden_args->struct_container;

	ret = glue_cap_insert_blk_mq_hw_ctx_type(c_cspace, ctx_container, &ctx_container->my_ref);
	if (ret) {
		LIBLCD_ERR("klcd insert");
		goto fail_insert;
	}

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	
	async_msg_set_fn_type(request, INIT_HCTX_FN);
	fipc_set_reg0(request, ctx_container->my_ref.cptr);
	fipc_set_reg1(request, index);
	
	/* ops container is passed to call ops.init_hctx in the LCD glue */
	fipc_set_reg2(request, ops_container->other_ref.cptr);

	printk("calling lcd's glue \n");
	ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	ctx_container->other_ref.cptr = fipc_get_reg0(response);
	func_ret = fipc_get_reg1(response);
	
	printk("init_hctx hctx %p | cptr %lu\n",ctx, ctx_container->other_ref.cptr);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);
	printk("end of init_hctx procedure \n");
	
	return func_ret;
fail_async:
fail_ipc:
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

void _softirq_done_fn(struct request *request, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *async_request;
	struct fipc_message *async_response;
	
	printk("why is sirq being called \n");
	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &async_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(async_request, SOFTIRQ_DONE_FN);
	ret = thc_ipc_call(hidden_args->async_chnl, async_request, &async_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), async_response);
	return;
fail_async:
fail_ipc:
	return;

}

LCD_TRAMPOLINE_DATA(softirq_done_fn_trampoline);
void LCD_TRAMPOLINE_LINKAGE(softirq_done_fn_trampoline) softirq_done_fn_trampoline(struct request *request)

{
	void ( *volatile softirq_done_fn_fp )(struct request *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, softirq_done_fn_trampoline);
	softirq_done_fn_fp = _softirq_done_fn;
	return softirq_done_fn_fp(request, hidden_args);

}


static int handle_sync(cptr_t sync_ep, cptr_t *tx, cptr_t *rx)
{
	int ret = 0;
	cptr_t sync_end;
	
        ret = grant_sync_ep(&sync_end, sync_ep);

	ret = lcd_cptr_alloc(tx);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	
	ret = lcd_cptr_alloc(rx);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail2;
	}
	/*
	 * Set up regs and poll
	 */
	lcd_set_cr0(*tx);
	lcd_set_cr1(*rx);

	ret = lcd_sync_recv(sync_end);
	
	lcd_set_cr0(CAP_CPTR_NULL); /* flush cr0 */
	lcd_set_cr1(CAP_CPTR_NULL); /* flush cr0 */

	lcd_cap_delete(sync_end);	

	if (ret) {
		LIBLCD_ERR("sync recv failed");
		goto fail3;
	}

	return ret;

fail3:
	lcd_cptr_free(*rx);
fail2:
	lcd_cptr_free(*tx);
fail1:
	return -1;
}

/* setsup new async channel between lcd and klcd for every thread
 * that lacks ptstate */
static int setup_new_channel(struct thc_channel *async_chnl, cptr_t sync_ep)
{	
	cptr_t tx, rx;
	struct thc_channel *chnl;
	struct fipc_message *request;
	struct fipc_message *response;
	int ret = 0;
	uint32_t request_cookie;

	//spin_lock(&lock);
	/* Init thc context, setup cspace */
	lcd_enter();

	/* call into LCD so that LCD prepares ring buffers */
	ret = async_msg_blocking_send_start(async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(request, OPEN);
	fipc_set_reg0(request, current->pid);

	ret = thc_ipc_send_request(async_chnl, request, &request_cookie);
	if (ret) {
		LIBLCD_ERR("thc_ipc_send");
		goto fail_send;
	}
	
	/* Do a sync receive to obtain cap to tx and rx channels */
	ret = handle_sync(sync_ep, &tx, &rx);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail_sync;
	}
	
	/*
	 * Set up async ring channel
	 */
	ret = setup_async_fs_ring_channel(tx, rx, &chnl);
	if (ret) {
		LIBLCD_ERR("error setting up ring channel");
		goto fail1;
	}

	thc_pts_set_chnl(chnl);
	thc_pts_set_state(true);

	LCD_MAIN(	
		DO_FINISH_(open, {
			ASYNC_({
				ret = thc_ipc_recv_response(async_chnl, 
						request_cookie, 
						&response);
			}, open);
		});
	);
	
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	fipc_recv_msg_end(thc_channel_to_fipc(async_chnl), response);
	//spin_unlock(&lock);
	return ret;

fail1:
fail_ipc:
fail_sync:
fail_send:
fail_async:
	//spin_unlock(&lock);
	lcd_exit(0);
	return ret;
}

int open(struct block_device *device, fmode_t mode, struct trampoline_hidden_args *hidden_args)
{
	int ret = 0;

	if(strcmp(current->comm,"systemd-udevd") == 0) {
		return -ENODEV;
	}

	printk("***** [nullb-open] current: %p name: %s pid: %d ptstate: %p cpu:%d \n", current, current->comm, current->pid, current->ptstate, smp_processor_id());

	spin_lock(&lock);
	if(!PTS()) {
		ret = setup_new_channel(hidden_args->async_chnl, hidden_args->sync_ep);
		if (ret) {
			goto fail;
		}
	printk("--> [nullb-open-SETUP_CHNL] current: %p name: %s pid: %d ptstate: %p cpu:%d\n", current, current->comm, current->pid, current->ptstate, smp_processor_id());
	}

	spin_unlock(&lock);
	printk("***** [nullb-open] done \n");
	return ret;

fail:
	spin_unlock(&lock);
	return -ENODEV;
}

static int cleanup_channel(struct thc_channel *channel, int pid)
{
	struct fipc_message *request;
	//struct fipc_message *response;
	int ret = 0;

	ret = async_msg_blocking_send_start(channel, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(request, RELEASE);
	fipc_set_reg0(request, pid);
	thc_set_msg_type(request, msg_type_request);
	fipc_send_msg_end (thc_channel_to_fipc(channel), request);


	//LCD_MAIN(
	//	DO_FINISH_(release, {
	//		ASYNC_({
	//			ret = thc_ipc_call(channel, request, &response);
	//		}, release);
	//	});
	//);

	//if (ret) {
	//	LIBLCD_ERR("thc_ipc_call");
	//	goto fail_ipc;
	//}

	//fipc_recv_msg_end(thc_channel_to_fipc(channel), response);
	
	return ret;
	/* TODO cleanup already handled by the LCD! so unmap complains that resource doesnt exist */
	//destroy_async_fs_ring_channel(thc_pts_get_chnl());
//fail_ipc:
fail_async:
	return ret;
}

void release(struct gendisk *disk, fmode_t mode, struct trampoline_hidden_args *hidden_args)
{
	struct async_item *item, *next;
	
	printk("[nullb-release] current: %p name: %s ptstate: %p cpu:%d \n", current, current->comm, current->ptstate, smp_processor_id());

	if(thc_pts_get_state() == true) {
		
		spin_lock(&lock);
	printk("-->cleanup ch current: %p name: %s ptstate: %p cpu:%d\n", current, current->comm, current->ptstate, smp_processor_id());
		if(cleanup_channel(hidden_args->async_chnl, current->pid)) {
			spin_unlock(&lock);
			return;
		}
		spin_unlock(&lock);
		/* TODO cleanup already handled by the LCD! so unmap complains that resource doesnt exist */
		//destroy_async_fs_ring_channel(thc_pts_get_chnl());
		thc_pts_set_state(false);
	
		if(!list_empty(&async_list)) {

			list_for_each_entry_safe(item, next, &async_list, list) {


				if(cleanup_channel(hidden_args->async_chnl, item->pid)) {
					printk("cleaning list entries----------> error!!! \n");
					lcd_exit(0);
					return;
				}
				lcd_exit_info(item->task);
				kfree(item->task->ptstate);
				item->task->ptstate = NULL;
				list_del_init(&item->list);
				kfree(item);
				item = NULL;
			}
		}
		lcd_exit(0);
	}
	
	printk("[nullb-release] cleaning-up current: %p name: %s pid: %d ptstate: %p \n", current, current->comm, current->pid, current->ptstate);
	return;
}

LCD_TRAMPOLINE_DATA(open_trampoline);
int LCD_TRAMPOLINE_LINKAGE(open_trampoline) open_trampoline(struct block_device *device, fmode_t mode)

{
	int ( *volatile open_fp )(struct block_device *, fmode_t , struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, open_trampoline);
	open_fp = open;
	return open_fp(device, mode, hidden_args);

}

LCD_TRAMPOLINE_DATA(release_trampoline);
void LCD_TRAMPOLINE_LINKAGE(release_trampoline) release_trampoline(struct gendisk *disk, fmode_t mode)

{
	void ( *volatile rel_fp )(struct gendisk *, fmode_t , struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, release_trampoline);
	rel_fp = release;
	return rel_fp(disk, mode, hidden_args);

}

/* This is the only lcd internal function that needs trampoline to be setup */
int lcd_register_chardev_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	int ret = 0;
	int err = 0;
	struct trampoline_hidden_args *fops_mmap_args;
	struct fipc_message *response;
	unsigned int request_cookie;

	request_cookie = thc_get_request_cookie(request);

	nullb_user_fops.owner = THIS_MODULE;
	nullb_user_fops.open = nullbu_open;
	nullb_user_fops.release = nullbu_close;

	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	/* setting up trampoline for mmap, other handlers are dummy */
	fops_mmap_args = kzalloc(sizeof(*fops_mmap_args), GFP_KERNEL);
	if (!fops_mmap_args) {
		LIBLCD_ERR("kzalloc hidden args");
		ret = -ENOMEM;
		goto fail_alloc1;
	}
	
	fops_mmap_args->t_handle = LCD_DUP_TRAMPOLINE(mmap_fops_trampoline);
	if (!fops_mmap_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		ret = -ENOMEM;
		goto fail_dup1;
	}

	fops_mmap_args->t_handle->hidden_args = fops_mmap_args;
	fops_mmap_args->struct_container = NULL;
	fops_mmap_args->cspace = c_cspace;
	fops_mmap_args->async_chnl = channel;
	fops_mmap_args->sync_ep = sync_ep;

	nullb_user_fops.mmap = LCD_HANDLE_TO_TRAMPOLINE(fops_mmap_args->t_handle);

	err = set_memory_x(((unsigned long)fops_mmap_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(mmap_fops_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (err) {
                LIBLCD_ERR("set mem nx");
		ret = -ENOMEM;
                goto fail_x1;
        }

	/* setup char dev region */
	ret = lcd_setup_chardev("nullb_user", &nullb_user_fops);
	if(ret) {
		printk("setting up chardev failed \n");
		ret = -ENODEV;
		goto fail_setup;
	}
	
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		goto fail_recv;
	}

	thc_ipc_reply(channel, request_cookie, response);

	return ret;

fail_recv:
	lcd_teardown_chardev();
fail_setup:
fail_x1:
	kfree(fops_mmap_args->t_handle);
fail_dup1:
	kfree(fops_mmap_args);
fail_alloc1:
	return ret;
}

int blk_mq_alloc_tag_set_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	struct blk_mq_tag_set_container *set_container;
	struct blk_mq_ops_container *ops_container;
	struct fipc_message *response;
	unsigned	int request_cookie;
	struct trampoline_hidden_args *queue_rq_hidden_args;
	struct trampoline_hidden_args *queue_rq_async_hidden_args;
	struct trampoline_hidden_args *map_queue_hidden_args;
	struct trampoline_hidden_args *init_hctx_hidden_args;
	struct trampoline_hidden_args *sirq_done_hidden_args;
	int func_ret;
	int err;
	//int sync_ret;
	//unsigned 	long mem_order;
	//unsigned 	long driver_data_offset;
	//cptr_t driver_data_cptr;
	//gva_t driver_data_gva;

	request_cookie = thc_get_request_cookie(request);

	/* This marks the message as received so sender sees a free slot! */
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	/* Allocate tag_set_container struct here */
	set_container = kzalloc(sizeof(struct blk_mq_tag_set_container), GFP_KERNEL);
	if (!set_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc1;
	}
	err = glue_cap_insert_blk_mq_tag_set_type(c_cspace, set_container, &set_container->my_ref);
	if (err) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert1;
	}
	
	/* Store the caller's reference! */
	set_container->other_ref.cptr = fipc_get_reg0(request);
	printk("set_other_ref %ld \n",set_container->other_ref.cptr);

	/* Allocate ops_container */
	ops_container = kzalloc(sizeof(struct blk_mq_ops_container), GFP_KERNEL);
	if (!set_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc2;
	}
	err = glue_cap_insert_blk_mq_ops_type(c_cspace, ops_container, &ops_container->my_ref);
	if (err) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert2;
	}

	ops_container->other_ref.cptr = fipc_get_reg6(request);
	printk("ops_other_ref %ld \n",ops_container->other_ref.cptr);
	
	/* This is required because, the blk_mq_tag_set which is passed to blk layer is this one */
	set_container->blk_mq_tag_set.ops = &ops_container->blk_mq_ops;

	/* Setup function pointers and trampolines - TODO better to move this to a separate fn */
	queue_rq_hidden_args = kzalloc(sizeof(*queue_rq_hidden_args), GFP_KERNEL);
	if (!queue_rq_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc3;
	}
	queue_rq_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(queue_rq_fn_trampoline);
	if (!queue_rq_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	queue_rq_hidden_args->t_handle->hidden_args = queue_rq_hidden_args;
	queue_rq_hidden_args->struct_container = ops_container;
	queue_rq_hidden_args->cspace = c_cspace;
	queue_rq_hidden_args->async_chnl = channel;
	ops_container->blk_mq_ops.queue_rq = LCD_HANDLE_TO_TRAMPOLINE(queue_rq_hidden_args->t_handle);

	err = set_memory_x(((unsigned long)queue_rq_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(queue_rq_fn_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (err) {
                LIBLCD_ERR("set mem nx");
                goto fail_x1;
        }

	/* Introduce new function pointer interface to batch requests to lcd */
	queue_rq_async_hidden_args = kzalloc(sizeof(*queue_rq_async_hidden_args), GFP_KERNEL);
	if (!queue_rq_async_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc_async;
	}
	queue_rq_async_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(queue_rq_async_fn_trampoline);
	if (!queue_rq_async_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup_async;
	}
	queue_rq_async_hidden_args->t_handle->hidden_args = queue_rq_async_hidden_args;
	queue_rq_async_hidden_args->struct_container = ops_container;
	queue_rq_async_hidden_args->cspace = c_cspace;
	queue_rq_async_hidden_args->async_chnl = channel;
	queue_rq_async_hidden_args->sync_ep = sync_ep;
	ops_container->blk_mq_ops.queue_rq_async = LCD_HANDLE_TO_TRAMPOLINE(queue_rq_async_hidden_args->t_handle);
	
	err = set_memory_x(((unsigned long)queue_rq_async_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(queue_rq_async_fn_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (err) {
                LIBLCD_ERR("set mem nx");
                goto fail_x_async;
        }

	/* init request */
	ops_container->blk_mq_ops.init_request = init_request;
	
	map_queue_hidden_args = kzalloc(sizeof( *map_queue_hidden_args ), GFP_KERNEL);
	if (!map_queue_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc4;
	}
	map_queue_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(map_queue_fn_trampoline);
	if (!map_queue_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup2;
	}
	map_queue_hidden_args->t_handle->hidden_args = map_queue_hidden_args;
	map_queue_hidden_args->struct_container = ops_container;
	map_queue_hidden_args->cspace = c_cspace;
	map_queue_hidden_args->async_chnl = channel;
	ops_container->blk_mq_ops.map_queue = LCD_HANDLE_TO_TRAMPOLINE(map_queue_hidden_args->t_handle);
	printk("map_queue in setup %p \n",ops_container->blk_mq_ops.map_queue);

	err = set_memory_x(((unsigned long)map_queue_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(map_queue_fn_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (err) {
                LIBLCD_ERR("set mem nx");
                goto fail_x2;
        }

	init_hctx_hidden_args = kzalloc(sizeof( *init_hctx_hidden_args ), GFP_KERNEL);
	if (!init_hctx_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc5;
	}
	init_hctx_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(init_hctx_fn_trampoline);
	if (!init_hctx_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup3;
	}
	init_hctx_hidden_args->t_handle->hidden_args = init_hctx_hidden_args;
	init_hctx_hidden_args->struct_container = ops_container;
	init_hctx_hidden_args->cspace = c_cspace;
	init_hctx_hidden_args->async_chnl = channel;
	ops_container->blk_mq_ops.init_hctx = LCD_HANDLE_TO_TRAMPOLINE(init_hctx_hidden_args->t_handle);

	printk("init_hctx in setup %p \n",ops_container->blk_mq_ops.init_hctx);
        err = set_memory_x(((unsigned long)init_hctx_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(init_hctx_fn_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (err) {
                LIBLCD_ERR("set mem nx");
                goto fail_x3;
        }

	sirq_done_hidden_args = kzalloc(sizeof( *sirq_done_hidden_args ), GFP_KERNEL);
	if (!sirq_done_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc6;
	}
	sirq_done_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(softirq_done_fn_trampoline);
	if (!sirq_done_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup4;
	}
	sirq_done_hidden_args->t_handle->hidden_args = sirq_done_hidden_args;
	sirq_done_hidden_args->struct_container = ops_container;
	sirq_done_hidden_args->cspace = c_cspace;
	sirq_done_hidden_args->async_chnl = channel;
	ops_container->blk_mq_ops.complete = LCD_HANDLE_TO_TRAMPOLINE(sirq_done_hidden_args->t_handle);

        err = set_memory_x(((unsigned long)sirq_done_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(softirq_done_fn_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (err) {
                LIBLCD_ERR("set mem nx");
                goto fail_x4;
        }

	/* Get the rest of the members from LCD */
	set_container->blk_mq_tag_set.nr_hw_queues = fipc_get_reg1(request);
	set_container->blk_mq_tag_set.queue_depth = fipc_get_reg2(request);
	hw_depth = set_container->blk_mq_tag_set.queue_depth;
	set_container->blk_mq_tag_set.reserved_tags = fipc_get_reg3(request);
	set_container->blk_mq_tag_set.cmd_size = fipc_get_reg4(request);
	set_container->blk_mq_tag_set.flags = fipc_get_reg5(request);
	//set_container->blk_mq_tag_set.timeout = 10000000 * HZ;
	//sync_ret = lcd_cptr_alloc(&driver_data_cptr);
	//if (sync_ret) {
	//	LIBLCD_ERR("failed to get cptr");
	//	lcd_exit(-1);
	//}
	//lcd_set_cr0(driver_data_cptr);
	//sync_ret = lcd_sync_recv(sync_ep);
	//lcd_set_cr0(CAP_CPTR_NULL);
	//if (sync_ret) {
	//	LIBLCD_ERR("failed to recv");
	//	lcd_exit(-1);
	//}
	//mem_order = lcd_r0();
	//driver_data_offset = lcd_r1();
	//sync_ret = lcd_map_virt(driver_data_cptr, mem_order, &driver_data_gva);
	//if (sync_ret) {
	//	LIBLCD_ERR("failed to map void *driver_data");
	//	lcd_exit(-1);
	//}
	/* call the real function */
	
	func_ret = blk_mq_alloc_tag_set((&set_container->blk_mq_tag_set));
	printk("*****block_alloc_tag set returns %d \n",func_ret);
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		goto fail_recv;
	}

	/* Hack for remove */
	set_g = &set_container->blk_mq_tag_set;
	fipc_set_reg0(response, set_container->my_ref.cptr);
	fipc_set_reg1(response, ops_container->my_ref.cptr);
	fipc_set_reg3(response, func_ret);
	thc_ipc_reply(channel, request_cookie, response);
	return func_ret;

fail_recv:
	kfree(sirq_done_hidden_args->t_handle);
fail_x4:
fail_dup4:
	kfree(sirq_done_hidden_args);
fail_alloc6:
	kfree(init_hctx_hidden_args->t_handle);
fail_x3:
fail_dup3:
	kfree(init_hctx_hidden_args);
fail_alloc5:
	kfree(map_queue_hidden_args->t_handle);
fail_x2:
fail_dup2:
	kfree(map_queue_hidden_args);
fail_alloc4:
	kfree(queue_rq_async_hidden_args->t_handle);
fail_x_async:
fail_dup_async:
	kfree(queue_rq_async_hidden_args);
fail_alloc_async:
	kfree(queue_rq_hidden_args->t_handle);
fail_x1:
fail_dup1:
	kfree(queue_rq_hidden_args);
fail_alloc3:
	glue_cap_remove(c_cspace, ops_container->my_ref);
fail_insert2:
	kfree(ops_container);
fail_alloc2:
	glue_cap_remove(c_cspace, set_container->my_ref);
fail_insert1:
	kfree(set_container);
fail_alloc1:
	return func_ret;

}

int add_disk_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{

	struct gendisk_container *disk_container;
	struct block_device_operations_container *blo_container;
	struct request_queue_container *rq_container;
	struct gendisk *disk;
	struct trampoline_hidden_args *open_hargs;
	struct trampoline_hidden_args *rel_hargs;
	struct module_container *module_container;
	struct fipc_message *response;
	unsigned int request_cookie;
	int ret;
	char disk_name[DISK_NAME_LEN];

	/* hack for now - hardcoding it here, ran out of regs! */
	sector_t size;

	request_cookie = thc_get_request_cookie(request);

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
        
	//module_container->module.refptr = alloc_percpu(struct module_ref);
        //if (!module_container->module.refptr) {
        //        LIBLCD_ERR("alloc percpu refptr failed");
        //        ret = -ENOMEM;
        //        goto fail_alloc3;
        //}

        module_container->module.state = MODULE_STATE_LIVE;
	
	/* without this add_disk will fail */
	atomic_inc(&module_container->module.refcnt);

        strcpy(module_container->module.name, "nullb");

        ret = glue_cap_insert_module_type(
                c_cspace,
                module_container,
                &module_container->my_ref);
        if (ret) {
                LIBLCD_ERR("insert");
                goto fail_insert2;
        }

	/* setup the fops */	
	blo_container->block_device_operations.owner = &module_container->module;

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
        open_hargs->cspace = c_cspace;
        open_hargs->async_chnl = channel;
	open_hargs->sync_ep = sync_ep;
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
        rel_hargs->cspace = c_cspace;
        rel_hargs->async_chnl = channel;
	rel_hargs->sync_ep = sync_ep;
        blo_container->block_device_operations.release = LCD_HANDLE_TO_TRAMPOLINE(rel_hargs->t_handle);

        ret = set_memory_x(((unsigned long)rel_hargs->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(release_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem nx");
                goto fail_x2;
        }

	disk = &disk_container->gendisk;
	printk("address of disk before calling add_diks %p \n",disk);

	disk->flags = fipc_get_reg4(request);
	disk->major = fipc_get_reg5(request);
	disk->first_minor = fipc_get_reg6(request);
	disk->queue = &rq_container->request_queue;
	
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
	size = 250 * 1024 * 1024 * 1024ULL;
	set_capacity(disk, size >> 9);
	
	disk->fops = &blo_container->block_device_operations;
	sprintf(disk_name, "nullb%d", disk->first_minor);
	strncpy(disk->disk_name, disk_name, DISK_NAME_LEN);

	printk("cpuid in call to add_disk %d \n", raw_smp_processor_id());
	add_disk(disk);
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}

	fipc_set_reg0(response, blo_container->my_ref.cptr);
	fipc_set_reg1(response, module_container->my_ref.cptr);
	printk("calling ipc \n");
	thc_ipc_reply(channel, request_cookie, response);
	
	printk("endof add_disk \n");
	return ret;

fail_x2:
fail_dup2:
	kfree(rel_hargs);
fail_alloc5:
fail_x1:
fail_dup1:
	kfree(open_hargs);
fail_alloc4:
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

