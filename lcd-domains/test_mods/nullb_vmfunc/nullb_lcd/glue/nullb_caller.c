#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include "../nullb_caller.h"
#include <lcd_config/post_hook.h>

#include "../../benchmark.h"
static struct glue_cspace *c_cspace;

void *iocb_data_pool;
struct blk_mq_hw_ctx_container *ctx_container_g; 
struct blk_mq_ops_container *ops_container_g;

struct lcd_request_container {
	struct request rq;
};

void null_softirq_done_fn(struct request *rq);

int null_open(struct block_device *bdev, fmode_t mode);
void null_release(struct gendisk *disk, fmode_t mode);

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

int blk_mq_alloc_tag_set(struct blk_mq_tag_set *set)
{
	struct blk_mq_tag_set_container *set_container;
	struct blk_mq_ops_container *ops_container;
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;
	int func_ret = 0;

	set_container = container_of(set, struct blk_mq_tag_set_container, tag_set);
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
	
	async_msg_set_fn_type(request, BLK_MQ_ALLOC_TAG_SET);

	fipc_set_reg0(request, set_container->my_ref.cptr);
	fipc_set_reg1(request, set->nr_hw_queues);
	fipc_set_reg2(request, set->queue_depth);
	fipc_set_reg3(request, set->reserved_tags);
	fipc_set_reg4(request, set->cmd_size);
	fipc_set_reg5(request, set->flags);
	fipc_set_reg6(request, ops_container->my_ref.cptr);
	
	vmfunc_wrapper(request);

	set_container->other_ref.cptr = fipc_get_reg0(request);
	ops_container->other_ref.cptr = fipc_get_reg1(request);
	func_ret = fipc_get_reg2(request);

	LIBLCD_MSG("%s received %d from", __func__, func_ret);

	return func_ret;

fail_insert1:
fail_insert2:
	return func_ret;
}

struct request_queue *blk_mq_init_queue(struct blk_mq_tag_set *set)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;

	struct blk_mq_tag_set_container *set_container;
	struct request_queue_container *rq_container;

	/* XXX Scary! request_queue size can vary from inside and outside
	 * LCDs? This is a bit fragile! */
	rq_container = kzalloc((sizeof(*rq_container)), GFP_KERNEL);

	if (!rq_container) {
		LIBLCD_ERR("kzalloc failed");
		goto fail_alloc;
	}

	ret = glue_cap_insert_request_queue_type(c_cspace, rq_container, &rq_container->my_ref);

        if (ret) {
		LIBLCD_ERR("lcd insert");
                goto fail_insert;
        }
	
	set_container = container_of(set, struct blk_mq_tag_set_container, tag_set);

	async_msg_set_fn_type(request, BLK_MQ_INIT_QUEUE);
	fipc_set_reg0(request, set_container->other_ref.cptr);
	fipc_set_reg1(request, rq_container->my_ref.cptr);
	
	vmfunc_wrapper(request);
	
	rq_container->other_ref.cptr = fipc_get_reg0(request);

	printk("%s returns local request queue struct!! \n", __func__);
	return &rq_container->request_queue;

fail_insert:
	kfree(rq_container);
fail_alloc:
	return NULL;
}

void blk_cleanup_queue(struct request_queue *q)
{
	struct request_queue_container *rq_container;
	struct fipc_message r;
	struct fipc_message *request = &r;

	rq_container = container_of(q, struct request_queue_container,
						request_queue);
	async_msg_set_fn_type(request, BLK_CLEANUP_QUEUE);

	fipc_set_reg0(request, rq_container->other_ref.cptr);
	
	vmfunc_wrapper(request);

	glue_cap_remove(c_cspace, rq_container->my_ref);
	kfree(rq_container);
	return;
}

void blk_mq_end_request(struct request *rq, int error)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct lcd_request_container *rq_c;

	rq_c = container_of(rq, struct lcd_request_container, rq);

	async_msg_set_fn_type(request, BLK_MQ_END_REQUEST);

	fipc_set_reg0(request, rq->tag);	
	fipc_set_reg1(request, error);

	vmfunc_wrapper(request);

	return;
}

void blk_mq_free_tag_set(struct blk_mq_tag_set *set)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct blk_mq_tag_set_container *set_container;
	struct blk_mq_ops_container *ops_container;

	set_container = container_of(set, struct blk_mq_tag_set_container, tag_set);

	ops_container = container_of(set->ops, struct blk_mq_ops_container,
				blk_mq_ops);

	async_msg_set_fn_type(request, BLK_MQ_FREE_TAG_SET);
	
	fipc_set_reg0(request, set_container->other_ref.cptr);
	fipc_set_reg1(request, ops_container->other_ref.cptr);

	vmfunc_wrapper(request);

	glue_cap_remove(c_cspace, set_container->my_ref);
	glue_cap_remove(c_cspace, ops_container->my_ref);
	return;
}

void blk_mq_start_request(struct request *rq)
{
	struct fipc_message r;
	struct fipc_message *request = &r;

	struct lcd_request_container *rq_c;

	rq_c = container_of(rq, struct lcd_request_container, rq);

	async_msg_set_fn_type(request, BLK_MQ_START_REQUEST);

	fipc_set_reg0(request, rq->tag);

	vmfunc_wrapper(request);

	return;
}

struct blk_mq_hw_ctx *blk_mq_map_queue(struct request_queue *rq, int ctx_index)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct blk_mq_hw_ctx *func_ret = NULL;

	async_msg_set_fn_type(request, BLK_MQ_MAP_QUEUE);
	fipc_set_reg0(request, ctx_index);

	vmfunc_wrapper(request);

	/* TODO: sync wrapper */

	return func_ret;
}

void blk_queue_logical_block_size(struct request_queue *rq, unsigned short size)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct request_queue_container *rq_container;

	rq_container = container_of(rq, struct request_queue_container,
					request_queue);
	
	async_msg_set_fn_type(request, BLK_QUEUE_LOGICAL_BLOCK_SIZE);
	fipc_set_reg0(request, size);
	fipc_set_reg1(request, rq_container->other_ref.cptr);

	vmfunc_wrapper(request);

	return;
}

void blk_queue_physical_block_size(struct request_queue *rq, unsigned int size)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct request_queue_container *rq_container;

	rq_container = container_of(rq, struct request_queue_container,
					request_queue);
	async_msg_set_fn_type(request, BLK_QUEUE_PHYSICAL_BLOCK_SIZE);

	fipc_set_reg0(request, size);
	fipc_set_reg1(request, rq_container->other_ref.cptr);

	vmfunc_wrapper(request);

	return;
}

struct gendisk *alloc_disk_node(int minors, int node_id) 
{
	struct gendisk_container *disk_container;
	struct fipc_message r;
	struct fipc_message *request = &r;
	int ret;

	disk_container = kzalloc(sizeof(struct gendisk_container), GFP_KERNEL);

	if (!disk_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_gendisk_type(c_cspace, disk_container, &disk_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(request, ALLOC_DISK);	
	fipc_set_reg0(request, minors);
	fipc_set_reg1(request, node_id);
	fipc_set_reg2(request, disk_container->my_ref.cptr);

	vmfunc_wrapper(request);

	disk_container->other_ref.cptr = fipc_get_reg0(request);

	return &disk_container->gendisk;

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
	struct fipc_message r;
	struct fipc_message *request = &r;

	disk_container = container_of(disk, struct gendisk_container, gendisk);

	blo_container = container_of(disk->fops, 
				struct block_device_operations_container,
				block_device_operations);

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
	
	async_msg_set_fn_type(request, DEVICE_ADD_DISK);
	fipc_set_reg0(request, disk_container->other_ref.cptr);
	fipc_set_reg1(request, blo_container->my_ref.cptr);
	fipc_set_reg2(request, module_container->my_ref.cptr);
	fipc_set_reg3(request, rq_container->other_ref.cptr);
	fipc_set_reg4(request, disk->flags);
	fipc_set_reg5(request, disk->major);
	fipc_set_reg6(request, disk->first_minor);

	/*
	 * Ran out of registers to marshall the string, so hardcoding it
	 * in the klcd
	 */
	vmfunc_wrapper(request);
	
	blo_container->other_ref.cptr = fipc_get_reg0(request);
	module_container->other_ref.cptr = fipc_get_reg1(request);

	return;

fail_insert3:
fail_insert2:
fail_insert1:
	return;
}

void put_disk(struct gendisk *disk)
{
	struct fipc_message r;
	struct fipc_message *request = &r;

	struct gendisk_container *disk_container;
	struct module_container *module_container;
	struct block_device_operations_container *blo_container;

	disk_container = container_of(disk, struct gendisk_container, gendisk);

	blo_container = container_of(disk->fops,
			struct block_device_operations_container, block_device_operations);

	module_container = container_of(disk->fops->owner, struct
			module_container, module);

	async_msg_set_fn_type(request, PUT_DISK);

	fipc_set_reg0(request, disk_container->other_ref.cptr);
	fipc_set_reg1(request, blo_container->other_ref.cptr);
	fipc_set_reg2(request, module_container->other_ref.cptr);
	
	vmfunc_wrapper(request);

	glue_cap_remove(c_cspace, disk_container->my_ref);
	glue_cap_remove(c_cspace, blo_container->my_ref);
	glue_cap_remove(c_cspace, module_container->my_ref);

	kfree(disk_container);
	return;
}

void del_gendisk(struct gendisk *gp)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct gendisk_container *disk_container;

	disk_container = container_of(gp, struct gendisk_container, gendisk);

	async_msg_set_fn_type(request, DEL_GENDISK);
	fipc_set_reg0(request, disk_container->other_ref.cptr);
	
	vmfunc_wrapper(request);
	
	return;
}

int register_blkdev(unsigned int devno, const char *name)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;

	async_msg_set_fn_type(request, REGISTER_BLKDEV);
	fipc_set_reg0(request, devno);

	vmfunc_wrapper(request);

        ret = fipc_get_reg0(request);

	return ret;
}

void unregister_blkdev(unsigned int devno, const char *name)
{
	struct fipc_message r;
	struct fipc_message *request = &r;

	LIBLCD_MSG("unreg blkdev caller glue");

	async_msg_set_fn_type(request, UNREGISTER_BLKDEV);
	
	fipc_set_reg0(request, devno);
	vmfunc_wrapper(request);

	return;
}

int queue_rq_fn_callee(struct fipc_message *request)
{	struct blk_mq_hw_ctx_container *ctx_container = ctx_container_g;
	struct blk_mq_ops_container *ops_container = ops_container_g;
	struct blk_mq_queue_data bd;
	struct lcd_request_container rq_c;
	struct request *rq = &rq_c.rq;
	int ret = 0;
	int func_ret;

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

	func_ret = ops_container->blk_mq_ops.queue_rq(&ctx_container->blk_mq_hw_ctx,
				&bd);
	
	fipc_set_reg0(request, func_ret);

//fail_lookup:
	return ret;
}

int map_queue_fn_callee(struct fipc_message *request)
{
	struct request_queue_container *req_queue_container;
	int cpu;
	int err;

	struct blk_mq_hw_ctx *func_ret;

	err = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg4(request)), &req_queue_container);
	if (err) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	cpu = fipc_get_reg1(request);

	func_ret = kzalloc(sizeof( *func_ret ), GFP_KERNEL);

	if (!func_ret) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	/* TODO sync wrapper */
#if 0
	func_ret = map_queue_fn(( &req_queue_container->request_queue ), cpu);
#endif
fail_lookup:
fail_alloc:
	return err;
}

int init_hctx_fn_callee(struct fipc_message *request)
{

	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container;
	unsigned int index;
	cptr_t pool_cptr;
	gva_t pool_addr;
	unsigned int pool_ord;
	int ret = 0;

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

	/* receive shared data pool */
	ret = lcd_cptr_alloc(&pool_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_cptr;
	}

	fipc_set_reg0(request, cptr_val(pool_cptr));
	vmfunc_sync_call(request, INIT_HCTX_SYNC);
	pool_ord = fipc_get_reg0(request);

	ret = lcd_map_virt(pool_cptr, pool_ord, &pool_addr);
	if (ret) {
		LIBLCD_ERR("failed to map pool");
		goto fail_pool;
	}

	LIBLCD_MSG("%s, mapping private pool %p | ord %d", __func__,
			gva_val(pool_addr), pool_ord);

	iocb_data_pool = (void*)gva_val(pool_addr);

	/* Passing NULL to data arg, hack to get nullb's address within the driver */
	ret = ops_container->blk_mq_ops.init_hctx(&ctx_container->blk_mq_hw_ctx, NULL, index);
	if(ret) {
	        LIBLCD_ERR("call to init_hctx failed");
                goto fail_hctx;
	}

	fipc_set_reg0(request, ctx_container->my_ref.cptr);
	fipc_set_reg1(request, ret);
	return ret;

fail_pool:
fail_cptr:
fail_hctx:
fail_lookup:
	glue_cap_remove(c_cspace, ctx_container->my_ref);
fail_insert:
	kfree(ctx_container);
fail_alloc:
	return ret;
}

int softirq_done_fn_callee(struct fipc_message *_request)
{
#if 0
	struct request *rq;
	struct request_container *rq_c;

	null_softirq_done_fn(rq);
#endif

	printk("%s, called", __func__);
	return 0;
}

int open_callee(struct fipc_message *_request)
{
	int ret;

	fmode_t mode = fipc_get_reg0(_request);

	ret = null_open(NULL, mode);

	fipc_set_reg0(_request, ret);

	return ret;
}

int release_callee(struct fipc_message *_request)
{
	struct gendisk_container *disk_container;
	int ret;
	fmode_t mode;

	ret = glue_cap_lookup_gendisk_type(c_cspace, __cptr(fipc_get_reg0(_request)),
					&disk_container);
	mode = fipc_get_reg1(_request);

	null_release(&disk_container->gendisk, mode);

	return 0;
}
