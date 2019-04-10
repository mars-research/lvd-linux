#include <lcd_config/pre_hook.h>

//#include <linux/fs.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include "../nullb_caller.h"
#include <lcd_config/post_hook.h>

#include "../../benchmark.h"
extern cptr_t blk_sync_endpoint;
extern cptr_t blk_register_chnl;
static struct glue_cspace *c_cspace;
extern struct thc_channel *blk_async_chl;
extern struct thc_channel_group ch_grp;

/* This is where auxiliary channel will be added */
struct thc_channel *disp_chnl[2];

struct blk_mq_hw_ctx_container *ctx_container_g; 
struct blk_mq_ops_container *ops_container_g;

struct lcd_request_container {
	struct request rq;
	void *channel;
	unsigned int cookie;
};

//INIT_BENCHMARK_DATA_LCD(queue_rq);

int glue_nullb_init(void)
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

void glue_nullb_exit(void)
{
	glue_cap_destroy(c_cspace);
	glue_cap_exit();

}

static int setup_async_channel(cptr_t *buf1_cptr_out, cptr_t *buf2_cptr_out,
                        struct thc_channel **chnl_out)
{
        int ret;
        cptr_t buf1_cptr, buf2_cptr;
        gva_t buf1_addr, buf2_addr;
        struct fipc_ring_channel *fchnl;
        struct thc_channel *chnl;
        unsigned int pg_order = PMFS_ASYNC_RPC_BUFFER_ORDER - PAGE_SHIFT;
        /*
         * Allocate buffers
         *
         * (We use the lower level alloc. If we used the heap, even though
         * we may alloc only 1 - 2 pages, we would end up sharing around
         * 4 MB chunks of memory, since the heap uses coarse microkernel
         * allocations.)
         */
        ret = _lcd_alloc_pages(GFP_KERNEL, pg_order, &buf1_cptr);
        if (ret) {
                LIBLCD_ERR("buf1 alloc");
                goto fail1;
        }
        ret = _lcd_alloc_pages(GFP_KERNEL, pg_order, &buf2_cptr);
        if (ret) {
                LIBLCD_ERR("buf2 alloc");
                goto fail2;
        }
        /*
         * Map them somewhere
         */
        ret = lcd_map_virt(buf1_cptr, pg_order, &buf1_addr);
        if (ret) {
                LIBLCD_ERR("error mapping buf1");
                goto fail3;
        }
        ret = lcd_map_virt(buf2_cptr, pg_order, &buf2_addr);
        if (ret) {
                LIBLCD_ERR("error mapping buf2");
                goto fail4;
        }
        /*
         * Prep buffers for rpc
         */
        ret = fipc_prep_buffers(PMFS_ASYNC_RPC_BUFFER_ORDER,
                                (void *)gva_val(buf1_addr),
                                (void *)gva_val(buf2_addr));
        if (ret) {
                LIBLCD_ERR("prep buffers");
                goto fail5;
        }
        /*
         * Alloc and init channel header
         */
        fchnl = kmalloc(sizeof(*fchnl), GFP_KERNEL);
        if (!fchnl) {
                ret = -ENOMEM;
                LIBLCD_ERR("chnl alloc");
                goto fail6;
        }
        ret = fipc_ring_channel_init(fchnl, PMFS_ASYNC_RPC_BUFFER_ORDER,
                                (void *)gva_val(buf1_addr),
                                (void *)gva_val(buf2_addr));
        if (ret) {
                LIBLCD_ERR("ring chnl init");
                goto fail7;
        }
        /*
         * Install async channel in async dispatch loop
         */
        chnl = kzalloc(sizeof(*chnl), GFP_KERNEL);
        if (!chnl) {
                ret = -ENOMEM;
                LIBLCD_ERR("alloc failed");
                goto fail8;
        }
        ret = thc_channel_init(chnl, fchnl);
        if (ret) {
                LIBLCD_ERR("error init'ing async channel group item");
                goto fail9;
        }

        *buf1_cptr_out = buf1_cptr;
        *buf2_cptr_out = buf2_cptr;
        *chnl_out = chnl;

        return 0;

fail9:
        kfree(chnl);
fail8:
fail7:
        kfree(fchnl);
fail6:
fail5:
        lcd_unmap_virt(buf1_addr, pg_order);
fail4:
        lcd_unmap_virt(buf1_addr, pg_order);
fail3:
        lcd_cap_delete(buf2_cptr);
fail2:
        lcd_cap_delete(buf1_cptr);
fail1:
        return ret;
}

static void destroy_async_channel(struct thc_channel *chnl)
{
        unsigned int pg_order = PMFS_ASYNC_RPC_BUFFER_ORDER - PAGE_SHIFT;
        gva_t tx_gva, rx_gva;
        cptr_t tx, rx;
        int ret;
        unsigned long unused1, unused2;
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
         * Free the async channel
         *
         * XXX: This is ok to do because there is no dispatch loop
         * polling on the channel when we free it.
         */
        kfree(chnl);

        return;

fail2:
fail1:
        return;
}

/* These are left dummy for the sake of completeness */
static int nullbu_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int nullbu_close (struct inode *inode, struct file *filp)
{
	return 0;
}

static int nullbu_mmap(struct file *filp, struct vm_area_struct *vma)
{
	return 0;
}

/* mmap for the chardev will land here */
int nullbu_mmap_user(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	struct fipc_message *response;
	unsigned int request_cookie;
	int ret = 0;
	unsigned int func_ret = 0;
	cptr_t page;
	unsigned long order = 0;
	gva_t page_gva;
	unsigned long *page_virt;
	
	//printk("mmap lcd begins \n");
	request_cookie = thc_get_request_cookie(request);
	order = fipc_get_reg1(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = lcd_cptr_alloc(&page);
	if(ret) {
		LIBLCD_ERR("failed to get cptr");
		func_ret = 1;
		goto fail_sync;
	}
	
	lcd_set_cr0(page);
	ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (ret) {
		LIBLCD_ERR("failed to recv");
		func_ret = 1;
		goto fail_recv;
	}
	
	//printk("map virt \n");
	/* map the page into ram map area for now */
	ret = lcd_map_virt(page, order, &page_gva);
	if (ret) { 
		LIBLCD_ERR("failed to map void *addr"); 
		func_ret = 1;
		goto fail_map;
	}

	//printk("map virt done \n");
	page_virt = (void *)gva_val(page_gva);
	//*(page_virt) = 0xAA11BBCCDD;

fail_map:
fail_recv:
fail_sync:
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		ret = -EIO;
	}
	fipc_set_reg1(response, func_ret);
	thc_ipc_reply(channel, request_cookie, response);

	//printk("mmap lcd ends \n");
	return ret;
}


void lcd_register_chardev(const char* name, struct file_operations *fops)
{
	int ret = 0;
	struct fipc_message *request;
	struct fipc_message *response;

	/* we dont use the fops now, neither the name! we can handle it 
	 * later if needed */
	ret = async_msg_blocking_send_start(blk_async_chl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	/* KLCD will setup a char device for us */
	async_msg_set_fn_type(request, REGISTER_CHARDEV);

	ret = thc_ipc_call(blk_async_chl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);

fail_async:
fail_ipc:
	return;
}

struct file_operations nullb_user_fops = {
        .open   = nullbu_open,
      	.release = nullbu_close,
        .mmap = nullbu_mmap,
};

int blk_mq_alloc_tag_set(struct blk_mq_tag_set *set)
{
	struct blk_mq_tag_set_container *set_container;
	struct blk_mq_ops_container *ops_container;
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	int func_ret;
//	int sync_ret;
//	unsigned	long driver_data_mem_sz;
//	unsigned 	long driver_data_offset;
//	cptr_t driver_data_cptr;

	/* AB: This additional register function is needed to deal with data 
	 * allocation directly within LCDs */
	lcd_register_chardev("nullb_user", &nullb_user_fops);

	set_container = container_of(set, struct blk_mq_tag_set_container, blk_mq_tag_set);
	ops_container = container_of(set->ops, struct blk_mq_ops_container, blk_mq_ops);
	ops_container_g = ops_container;

	ret = glue_cap_insert_blk_mq_ops_type(c_cspace, ops_container, &ops_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert1;
	}
	
	ret = glue_cap_insert_blk_mq_tag_set_type(c_cspace, set_container, &set_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert2;
	}
	
	ret = async_msg_blocking_send_start(blk_async_chl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, BLK_MQ_ALLOC_TAG_SET);
	fipc_set_reg0(request, set_container->my_ref.cptr);
	fipc_set_reg1(request, set->nr_hw_queues);
	fipc_set_reg2(request, set->queue_depth);
	fipc_set_reg3(request, set->reserved_tags);
	fipc_set_reg4(request, set->cmd_size);
	fipc_set_reg5(request, set->flags);
	fipc_set_reg6(request, ops_container->my_ref.cptr);
	
	//sync_ret = lcd_virt_to_cptr(__gva((unsigned long)driver_data), &driver_data_cptr, &driver_data_mem_sz, &driver_data_offset);
	//if (sync_ret) {
//		LIBLCD_ERR("virt to cptr failed");
//		lcd_exit(-1);
//	}
	ret = thc_ipc_call(blk_async_chl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	set_container->other_ref.cptr = fipc_get_reg0(response);
	ops_container->other_ref.cptr = fipc_get_reg1(response);
	func_ret = fipc_get_reg3(response);
	printk("LCD received %d from block_al-tg-set \n",func_ret);
	fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);
	return func_ret;

fail_insert1:
fail_insert2:
fail_async:
fail_ipc:
	return func_ret;
}

struct request_queue *blk_mq_init_queue(struct blk_mq_tag_set *set)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct blk_mq_tag_set_container *set_container;
	struct request_queue_container *rq_container;

	/* XXX Scary! request_queue size can vary from inside and outside
	 * LCDs? This is a bit fragile! */
	rq_container = kzalloc((sizeof(*rq_container)), GFP_KERNEL);
	if(!rq_container) {
		LIBLCD_ERR("kzalloc failed");
		goto fail_alloc;
	}
	
	ret = glue_cap_insert_request_queue_type(c_cspace, rq_container, &rq_container->my_ref);
        if (ret) {
		LIBLCD_ERR("lcd insert");
                goto fail_insert;
        }
	
	ret = async_msg_blocking_send_start(blk_async_chl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	
	set_container = container_of(set, struct blk_mq_tag_set_container, blk_mq_tag_set);

	async_msg_set_fn_type(request, BLK_MQ_INIT_QUEUE);
	fipc_set_reg0(request, set_container->other_ref.cptr);
	fipc_set_reg1(request, rq_container->my_ref.cptr);
	
	printk("making IPC call for blk_mq_init \n");
	ret = thc_ipc_call(blk_async_chl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	
	rq_container->other_ref.cptr = fipc_get_reg0(response);
	fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);

	printk("blk_mq_init returns local request queue struct!! \n");	
	return &rq_container->request_queue;
fail_ipc:
fail_async:
	glue_cap_remove(c_cspace, rq_container->my_ref);
fail_insert:
	kfree(rq_container);
fail_alloc:
	return NULL;
}

void blk_cleanup_queue(struct request_queue *q)
{
	struct request_queue_container *rq_container;
	struct fipc_message *request;
	struct fipc_message *response;
	int ret;

	ret = async_msg_blocking_send_start(blk_async_chl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	
	rq_container = container_of(q, struct request_queue_container,
						request_queue);

	async_msg_set_fn_type(request, BLK_CLEANUP_QUEUE);
	fipc_set_reg0(request, rq_container->other_ref.cptr);
	
	ret = thc_ipc_call(blk_async_chl, request, &response);
	if (ret) {
	 	LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	
	fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);
	glue_cap_remove(c_cspace, rq_container->my_ref);
	kfree(rq_container);
	return;

fail_ipc:
fail_async:
	return;
}

void blk_mq_end_request(struct request *rq, int error)
{
	int ret;
	struct fipc_message *request;
	struct lcd_request_container *rq_c;
	struct thc_channel *async_chnl;

#ifndef SENDER_DISPATCH_LOOP
	static int index = 0;
#endif
	//static int count = 0;
	//struct fipc_message *response;
	//uint32_t request_cookie;

	//printk("[LCD_GLUE] END_REQ glue begins \n");
	//BENCH_BEGIN_LCD(queue_rq);
	//count ++;
	rq_c = container_of(rq,
			struct lcd_request_container, rq);

#ifdef SENDER_DISPATCH_LOOP
	async_chnl = rq_c->channel;
#else
	async_chnl = disp_chnl[index];
#endif

	ret = async_msg_blocking_send_start(async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	
	//printk("[LCD_GLUE] END_REQ got a free slot for tag %d \n",rq->tag);
	async_msg_set_fn_type(request, BLK_MQ_END_REQUEST);

	fipc_set_reg0(request, rq->tag);	
	fipc_set_reg1(request, error);
	
	thc_set_msg_type(request, msg_type_request);
#ifdef SENDER_DISPATCH_LOOP
	thc_set_msg_id(request, rq_c->cookie);
#endif
	fipc_send_msg_end (thc_channel_to_fipc(async_chnl), request);

	//if(count == 2) {
		/* toggle index */
	//	index ^= 1;
	//	count = 0;
	//}

	//printk("[LCD_GLUE] END_REQ ipc call-> rq->tag: %d \n", rq->tag);
	//ret = thc_ipc_call(blk_async_chl, request, &response);
	//if (ret) {
	//	LIBLCD_ERR("thc_ipc_call");
	//	goto fail_ipc;
	//}
	
	//thc_ipc_send_request(blk_async_chl, request, &request_cookie);
	//if (ret) {
	//	printk("send_req failed \n");
	//	goto fail_send;
	//}

	//ret = thc_ipc_recv_response_lcd(blk_async_chl, request_cookie, &response);
	//if (ret) {
	//	printk("recv_resp failed \n");
	//	goto fail_send;
	//}
	
	//fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);

	//BENCH_END_LCD(queue_rq);
		//printk("[LCD_GLUE] END_REQ glue ends -> ra->tag: %d\n", rq->tag);
	return;

//fail_send:
fail_async:
//fail_ipc:
	return;

}

void blk_mq_free_tag_set(struct blk_mq_tag_set *set)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct blk_mq_tag_set_container *set_container;
	struct blk_mq_ops_container *ops_container;

	set_container = container_of(set, struct blk_mq_tag_set_container, 
						blk_mq_tag_set);

	ops_container = container_of(set->ops, struct blk_mq_ops_container, 
						blk_mq_ops); 

	ret = async_msg_blocking_send_start(blk_async_chl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, BLK_MQ_FREE_TAG_SET);
	
	fipc_set_reg0(request, set_container->other_ref.cptr);
	fipc_set_reg1(request, ops_container->other_ref.cptr);
	ret = thc_ipc_call(blk_async_chl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);
	glue_cap_remove(c_cspace, set_container->my_ref);
	glue_cap_remove(c_cspace, ops_container->my_ref);
	return;

fail_ipc:
fail_async:
	return;
}


void blk_mq_start_request(struct request *rq)
{
	int ret;
	struct fipc_message *request;
	struct lcd_request_container *rq_c;
	struct thc_channel *async_chnl;
	//struct fipc_message *response;
	//uint32_t request_cookie;

	rq_c = container_of(rq,
			struct lcd_request_container, rq);

#ifdef SENDER_DISPATCH_LOOP
	async_chnl = rq_c->channel;
#else
	async_chnl = blk_async_chl;
#endif
	//printk("[LCD_GLUE] START_REQ glue begins \n");
	ret = async_msg_blocking_send_start(async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	
	//printk("[LCD_GLUE] START_REQ slot obtained for tag %d \n",rq->tag);
	async_msg_set_fn_type(request, BLK_MQ_START_REQUEST);
	fipc_set_reg0(request, rq->tag);

	//printk("[LCD_GLUE] START_REQ ipc_call-> rq->tag: %d \n",rq->tag);
	//ret = thc_ipc_call(blk_async_chl, request, &response);
	//if (ret) {
	//	LIBLCD_ERR("thc_ipc_call");
	//	goto fail_ipc;
	//}

	//fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);

	//printk("[LCD_GLUE] START_REQ glue ends-> rq->tag: %d \n", rq->tag);
	//ret = thc_ipc_send_request(blk_async_chl, request, &request_cookie);
	//if (ret) {
	//	LIBLCD_ERR("thc_ipc_call");
	//	goto fail_ipc;
	//}
	
	thc_set_msg_type(request, msg_type_request);
#ifdef SENDER_DISPATCH_LOOP
	thc_set_msg_id(request, rq_c->cookie);
#endif
	fipc_send_msg_end (thc_channel_to_fipc(async_chnl), request);
	return;

fail_async:
//fail_ipc:
	return;
}

#if 0
struct blk_mq_hw_ctx *blk_mq_map_queue(struct request_queue *rq, int ctx_index)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct blk_mq_hw_ctx *func_ret;
	ret = async_msg_blocking_send_start(blk_async_chl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, BLK_MQ_MAP_QUEUE);
	fipc_set_reg2(request, ctx_index);
	ret = thc_ipc_call(blk_async_chl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);
	return func_ret;
fail_async:

}
#endif 

void blk_queue_logical_block_size(struct request_queue *rq, unsigned short size)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct request_queue_container *rq_container;

	rq_container = container_of(rq, struct request_queue_container,
					request_queue);
	
	ret = async_msg_blocking_send_start(blk_async_chl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(request, BLK_QUEUE_LOGICAL_BLOCK_SIZE);
	
	fipc_set_reg0(request, size);
	fipc_set_reg1(request, rq_container->other_ref.cptr);

	ret = thc_ipc_call(blk_async_chl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);

	return;

fail_async:
fail_ipc:
	return;
}

void blk_queue_physical_block_size(struct request_queue *rq, unsigned int size)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct request_queue_container *rq_container;

	rq_container = container_of(rq, struct request_queue_container,
					request_queue);

	ret = async_msg_blocking_send_start(blk_async_chl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(request, BLK_QUEUE_PHYSICAL_BLOCK_SIZE);

	fipc_set_reg0(request, size);
	fipc_set_reg1(request, rq_container->other_ref.cptr);

	ret = thc_ipc_call(blk_async_chl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	
	fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);
	
	return;

fail_async:
fail_ipc:
	return;
}

struct gendisk *alloc_disk_node(int minors, int node_id) 
{
	struct gendisk_container *disk_container;
	struct fipc_message *request;
	struct fipc_message *response;	
	int ret;

	disk_container = kzalloc(sizeof(struct gendisk_container), GFP_KERNEL);
	if(!disk_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	
	ret = glue_cap_insert_gendisk_type(c_cspace, disk_container, &disk_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	ret = async_msg_blocking_send_start(blk_async_chl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(request, ALLOC_DISK);	
	fipc_set_reg0(request, minors);
	fipc_set_reg1(request, node_id);
	fipc_set_reg2(request, disk_container->my_ref.cptr);

	ret = thc_ipc_call(blk_async_chl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	disk_container->other_ref.cptr = fipc_get_reg0(response);

	fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);

	return &disk_container->gendisk;

fail_ipc:
fail_async:
	glue_cap_remove(c_cspace, disk_container->my_ref);
fail_insert:
	kfree(disk_container);
fail_alloc:
	return NULL;
}

void device_add_disk(struct device *parent, struct gendisk *disk)
{
	struct gendisk_container *disk_container;
	struct block_device_operations_container *blo_container;
	struct module_container *module_container;
	struct request_queue_container *rq_container;
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;

	disk_container = container_of(disk, struct gendisk_container, gendisk);

	blo_container = container_of(disk->fops, 
			struct block_device_operations_container, block_device_operations);

	module_container = container_of(disk->fops->owner, struct module_container,
				module);

	rq_container = container_of(disk->queue, struct request_queue_container,
				request_queue);

	ret = glue_cap_insert_module_type(c_cspace, module_container, &module_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert1;
	}
	
	ret = glue_cap_insert_gendisk_type(c_cspace, disk_container, &disk_container->my_ref);
	if (ret) {
		 LIBLCD_ERR("lcd insert");
		 goto fail_insert2;
	}

	ret = glue_cap_insert_blk_dev_ops_type(c_cspace, blo_container, &blo_container->my_ref);
	if(ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert3;
	}
	
	ret = async_msg_blocking_send_start(blk_async_chl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(request, ADD_DISK);
	fipc_set_reg0(request, disk_container->other_ref.cptr);
	fipc_set_reg1(request, blo_container->my_ref.cptr);
	fipc_set_reg2(request, module_container->my_ref.cptr);
	fipc_set_reg3(request, rq_container->other_ref.cptr);
	fipc_set_reg4(request, disk->flags);
	fipc_set_reg5(request, disk->major);
	fipc_set_reg6(request, disk->first_minor);

	/* Ran out of registers to marshall the string, so hardcoding it
	 * in the klcd */

	ret = thc_ipc_call(blk_async_chl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	
	blo_container->other_ref.cptr = fipc_get_reg0(response);
	module_container->other_ref.cptr = fipc_get_reg0(response);
	fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);
	printk("add_disk ends here in lcd glue \n");
	return;
fail_ipc:
fail_async:
fail_insert3:
fail_insert2:
fail_insert1:
	return;
}

void put_disk(struct gendisk *disk)
{
	int ret;
	struct fipc_message *request;
	struct gendisk_container *disk_container;
	struct module_container *module_container;
	struct block_device_operations_container *blo_container;
	struct fipc_message *response;

	disk_container = container_of(disk, struct gendisk_container, gendisk);

	blo_container = container_of(disk->fops,
			struct block_device_operations_container, block_device_operations);

	module_container = container_of(disk->fops->owner, struct module_container,
			 module);

	ret = async_msg_blocking_send_start(blk_async_chl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, DEL_GENDISK);

	fipc_set_reg0(request, disk_container->other_ref.cptr);
	fipc_set_reg1(request, blo_container->other_ref.cptr);
	fipc_set_reg2(request, module_container->other_ref.cptr);
	
	ret = thc_ipc_call(blk_async_chl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);
	
	glue_cap_remove(c_cspace, disk_container->my_ref);
	glue_cap_remove(c_cspace, blo_container->my_ref);
	glue_cap_remove(c_cspace, module_container->my_ref);
	kfree(disk_container);
	return;

fail_async:
fail_ipc:
	return;
}

void del_gendisk(struct gendisk *gp)
{
	int ret;
	struct fipc_message *request;
	struct gendisk_container *disk_container;
	struct fipc_message *response;

	disk_container = container_of(gp, struct gendisk_container, gendisk);

	ret = async_msg_blocking_send_start(blk_async_chl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, DEL_GENDISK);

	fipc_set_reg0(request, disk_container->other_ref.cptr);
	
	ret = thc_ipc_call(blk_async_chl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);
	
	glue_cap_remove(c_cspace, disk_container->my_ref);
	return;

fail_async:
fail_ipc:
	return;
}

int register_blkdev(unsigned int devno, const char *name)
{
	int ret;
        cptr_t tx, rx;
        struct thc_channel *chnl;
	//cptr_t tx_aux, rx_aux;
        struct thc_channel *aux_chnl = NULL;

	/* dispatch item */
	struct thc_channel_group_item *disp_item;

        /*
         * Set up async and sync channels
         */
        ret = lcd_create_sync_endpoint(&blk_sync_endpoint);
        if (ret) {
                LIBLCD_ERR("lcd_create_sync_endpoint");
                goto fail1;
        }
        ret = setup_async_channel(&tx, &rx, &chnl);
        if (ret) {
                LIBLCD_ERR("async chnl setup failed");
                goto fail2;
        }
	
	//ret = setup_async_channel(&tx_aux, &rx_aux, &aux_chnl);
        //if (ret) {
          //      LIBLCD_ERR("async chnl setup failed");
          //      goto fail_aux;
        //}

	disp_item = kzalloc(sizeof(*disp_item), GFP_KERNEL);
	if(!disp_item) {
		LIBLCD_ERR("no mem for disp_item");
		goto fail3;	
	}
	//printk("cptr_vals tx: %ld rx: %ld tx_aux.cptr: %ld rx_aux.cptr: %ld\n",tx.cptr, rx.cptr, tx_aux.cptr, rx_aux.cptr);
	/*
         * Do rpc, sending:
         *
         *    -- r1: our ref to fs type
         *    -- r2: our ref to module
         *    -- cr0: cap to pmfs_sync_endpoint
         *    -- cr1: cap to buffer for callee to use for tx (this is our rx)
         *    -- cr2: cap to buffer for callee to use for rx (this is our tx)
         */
        lcd_set_r0(REGISTER_BLKDEV);
        lcd_set_r1((u64)devno);
        lcd_set_cr0(blk_sync_endpoint);
        lcd_set_cr1(rx);
        lcd_set_cr2(tx);
        //lcd_set_cr3(rx_aux);
        //lcd_set_cr4(tx_aux);
	
	/* TODO find a way to marshall the string! 
	 * for this simple case kstrtou64() should work 
	 * but what is it for other direction. Or else
	 * we have to map a page and copy contents onto it */
        //lcd_set_r2((name);

        ret = lcd_sync_call(blk_register_chnl);
        /*
         * Flush cap registers
         */
        lcd_set_cr0(CAP_CPTR_NULL);
        lcd_set_cr1(CAP_CPTR_NULL);
        lcd_set_cr2(CAP_CPTR_NULL);
        //lcd_set_cr3(CAP_CPTR_NULL);
        //lcd_set_cr4(CAP_CPTR_NULL);
        if (ret) {
                LIBLCD_ERR("lcd_call");
                goto fail4;
        }
        /*
         * Reply:
         *
         *    -- r0: register_blkdev return value
         */
        ret = lcd_r0();
        if (ret < 0) {
                LIBLCD_ERR("remote register fs failed");
                goto fail5;
        }

        /*
         * Kick off async recv
         */
        blk_async_chl = chnl;
	disp_chnl[0] = chnl;
	disp_chnl[1] = aux_chnl;
	disp_item->channel = chnl;
	disp_item->channel_id = 0; //dispatch item has 0 id
	add_chnl_group_item(disp_item, &ch_grp);


	return ret;

fail5:
fail4:
	kfree(disp_item);
fail3:
	//destroy_async_channel(aux_chnl);
//fail_aux:
	destroy_async_channel(chnl);

fail2:
        lcd_cap_delete(blk_sync_endpoint);
fail1:
	return ret;
}

void unregister_blkdev(unsigned int devno, const char *name)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;

	LIBLCD_MSG("unreg blkdev caller glue");
	ret = async_msg_blocking_send_start(blk_async_chl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	
	async_msg_set_fn_type(request, UNREGISTER_BLKDEV);
	
	fipc_set_reg0(request, devno);
	//TODO Not marshalling the string for now! hardcoded in klcd
	ret = thc_ipc_call(blk_async_chl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(blk_async_chl), response);
	return;

fail_async:
fail_ipc:
	return;
}
int null_exit_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	struct fipc_message *response;
	unsigned int request_cookie;
	int ret;
	
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
	
	null_exit();

        if (async_msg_blocking_send_start(channel, &response)) {
                LIBLCD_ERR("error getting response msg");
                ret = -EIO;
                goto fail_async;
        }

        thc_ipc_reply(channel, request_cookie, response);

fail_async:
	return ret;
}

int queue_rq_fn_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	struct fipc_message *response;
	unsigned int request_cookie;
	struct blk_mq_hw_ctx_container *ctx_container = ctx_container_g;	
	struct blk_mq_ops_container *ops_container = ops_container_g;
	struct blk_mq_queue_data bd;
	struct lcd_request_container rq_c;
	struct request *rq = &rq_c.rq;
	int ret;
	int func_ret;
	
	request_cookie = thc_get_request_cookie(request);
	
//	ret = glue_cap_lookup_blk_mq_hw_ctx_type(c_cspace, __cptr(fipc_get_reg1(request)),
//						&ctx_container);
//	if(ret) {
//		LIBLCD_ERR("lookup");
//		goto fail_lookup;
//	}
	
	ctx_container->blk_mq_hw_ctx.queue_num = fipc_get_reg0(request);

//	ret = glue_cap_lookup_blk_mq_ops_type(c_cspace, __cptr(fipc_get_reg2(request)),
//				&ops_container);
//	if(ret) {
//		LIBLCD_ERR("lookup");
//		goto fail_lookup;
//	}

	rq->tag = fipc_get_reg3(request);
	bd.rq = rq;

	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
#ifdef SENDER_DISPATCH_LOOP
	rq_c.channel = channel;
	rq_c.cookie = request_cookie;
#endif
	func_ret = ops_container->blk_mq_ops.queue_rq(&ctx_container->blk_mq_hw_ctx,
				&bd);
	

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		ret = -EIO;
		goto fail_async;
	}

	fipc_set_reg0(response, func_ret);
	ret = thc_ipc_reply(channel, request_cookie, response);
	if(ret) {
		LIBLCD_ERR("error thc_ipc_reply");
		goto fail_reply;
	}
	
	//printk("[LCD_GLUE] - queue_rq: ends! \n");
	return ret;

fail_reply:
fail_async:
//fail_lookup:

	return ret;
}

int map_queue_fn_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
#if 0
	struct request_queue_container *req_queue_container;
	int m;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	struct blk_mq_hw_ctx *func_ret;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
	err = glue_cap_lookup_request_queue_type(cspace, __cptr(fipc_get_reg4(request)), &req_queue_container);
	if (err) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	m = fipc_get_reg5(request);
	func_ret = kzalloc(*sizeof( func_ret ), GFP_KERNEL);
	if (!func_ret) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long driver_data_offset;
	cptr_t driver_data_cptr;
	gva_t driver_data_gva;
	func_ret = map_queue_fn(( &req_queue_container->request_queue ), m);
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

int init_hctx_fn_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{

	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container;
	unsigned int index;
	struct fipc_message *response;
	unsigned int request_cookie;
	int ret;

	printk("%s, $$$$$$$$$$$$$4 called $$$$$$$$$$$$$$", __func__);
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ctx_container = kzalloc(sizeof(*ctx_container), GFP_KERNEL);
	if (!ctx_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ctx_container_g = ctx_container;	
	ret = glue_cap_insert_blk_mq_hw_ctx_type(c_cspace, ctx_container, &ctx_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	
	ctx_container->other_ref.cptr = fipc_get_reg0(request); 
	index = fipc_get_reg1(request);

	ret = glue_cap_lookup_blk_mq_ops_type(c_cspace,
			__cptr(fipc_get_reg2(request)), &ops_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }
	
	/* Passing NULL to data arg, hack to get nullb's address within the driver */
	ret = ops_container->blk_mq_ops.init_hctx(&ctx_container->blk_mq_hw_ctx, NULL, index);
	if(ret) {
	        LIBLCD_ERR("call to init_hctx failed");
                goto fail_hctx;
	}
	
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		goto fail_async;
	}

	LIBLCD_MSG("lcd ctx.cptr %lu", ctx_container->my_ref.cptr);
	fipc_set_reg0(response, ctx_container->my_ref.cptr);
	fipc_set_reg1(response, ret);
	thc_ipc_reply(channel, request_cookie, response);
	return ret;

fail_async:
fail_hctx:
fail_lookup:
	glue_cap_remove(c_cspace, ctx_container->my_ref);
fail_insert:
	kfree(ctx_container);
fail_alloc:
	return ret;
}

int softirq_done_fn_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
#if 0
	struct request *request;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
	request = kzalloc(*sizeof( request ), GFP_KERNEL);
	if (!request) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	softirq_done_fn(request);
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(channel, request_cookie, response);
	return ret;
#endif
	return 0;
}

static int setup_sync(cptr_t sync_ep, cptr_t tx, cptr_t rx)
{
	int ret = 0;

	/* rx first and tx second */
	lcd_set_cr0(rx);
        lcd_set_cr1(tx);
	
	//printk("calling sync send \n");	
	ret = lcd_sync_send(blk_sync_endpoint);
	lcd_set_cr0(CAP_CPTR_NULL); /* flush cr0 after send */
	lcd_set_cr1(CAP_CPTR_NULL); /* flush cr0 after send */
	if (ret) {          
                LIBLCD_ERR("async chnl setup failed");
		ret = -1;
                goto fail_send;
        }
	return ret;

fail_send:
	return ret;	

}

int open_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	struct fipc_message *response;
	unsigned int request_cookie;
	int ret = 0;
	cptr_t tx, rx;
	struct thc_channel *chnl;
	struct thc_channel_group_item *item;
	int channel_id = 0;
	
	request_cookie = thc_get_request_cookie(request);
	channel_id = fipc_get_reg0(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	//printk("setting up async channel \n");	
	/* setup new channel for the incomming process/thread */
        ret = setup_async_channel(&tx, &rx, &chnl);
        if (ret) {          
                LIBLCD_ERR("async chnl setup failed");
                goto fail1;
        }           
	
	//printk("setting up sync ipc \n");	
	/* Setup a sync IPC */
	ret = setup_sync(sync_ep, tx, rx);
        if (ret) {          
                LIBLCD_ERR("async chnl setup failed");
                goto fail_sync;
        }           

	//printk("setting up sync ipc done \n");	
	/* add the channel to the global list to listen on */
	item = kzalloc(sizeof(*item), GFP_KERNEL);
	if(!item) {
		LIBLCD_ERR("no mem for item");
		goto fail2;
	}
		
	item->channel = chnl;
	item->channel_id = channel_id; 
	//printk("adding item:%p to group with id:%d \n", item, channel_id);	
	add_chnl_group_item(item, &ch_grp);

		
	//printk(" send reply back to klcd \n");	
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		ret = -EIO;
		goto fail3;
	}

	thc_ipc_reply(channel, request_cookie, response);

	return ret;
fail3:
	kfree(item);
fail2:
fail_sync:
	destroy_async_channel(chnl);
	
fail1:
	return ret;

}

int release_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	//struct fipc_message *response;
	unsigned int request_cookie;
	int ret = 0;
	struct thc_channel *chnl = NULL;
	int channel_id = 0;
	
	//BENCH_COMPUTE_STAT(queue_rq);
	//dump_data();
	//thc_ipc_dump_stat();

	request_cookie = thc_get_request_cookie(request);
	channel_id = fipc_get_reg0(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	chnl = get_chnl_from_id(channel_id, &ch_grp);
	if(chnl) {
		//printk("calling destroy asyn_channel \n");
		destroy_async_channel(chnl);
	}

	remove_chnl_group_item(channel_id, &ch_grp);

	//if (async_msg_blocking_send_start(channel, &response)) {
	//	LIBLCD_ERR("error getting response msg");
	//	ret = -EIO;
	//	goto fail1;
	//}

	//thc_ipc_reply(channel, request_cookie, response);

	return ret;
		
//fail1:
//	return ret;

}
