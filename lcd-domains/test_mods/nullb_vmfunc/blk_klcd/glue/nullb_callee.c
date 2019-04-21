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
#include <linux/priv_mempool.h>
#include <linux/sort.h>
#include <lcd_domains/microkernel.h>

#include <lcd_config/post_hook.h>

/* hacks for unregistering */
int null_major;
struct gendisk *disk_g;
struct request_queue *rq_g;
struct blk_mq_tag_set *set_g;

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

priv_pool_t *iocb_pool;
void *pool_base = NULL;
size_t pool_size = 0;
static unsigned long pool_pfn_start, pool_pfn_end;

static struct glue_cspace *c_cspace;
extern struct cspace *klcd_cspace;

#define MAX_POOLS	20

char *base_pools[MAX_POOLS];
int pool_order = 10;
int start_idx[MAX_POOLS/2] = {-1}, end_idx[MAX_POOLS/2] = {-1};
unsigned int best_diff = 0;
int best_idx = -1;
int pool_idx = 0;
struct {
	int start_idx;
	int end_idx;
	size_t size;
	bool valid;
} pools[MAX_POOLS] = { {0} };

int compare_addr(const void *a, const void *b)
{
	return *(unsigned int *)a - *(unsigned int *)b;
}

int pool_pick(void)
{
	int i;
	for (i = 0; i < MAX_POOLS; i++) {
		base_pools[i] = (char*) __get_free_pages(GFP_KERNEL | __GFP_ZERO,
	                            pool_order);
	}

	sort(base_pools, MAX_POOLS, sizeof(char*), compare_addr, NULL);

	printk("%s, sorted order:\n", __func__);
	for (i = 0; i < MAX_POOLS; i++) {
		printk("%s, got pool %p\n", __func__, base_pools[i]);
	}

	pools[pool_idx].start_idx = 0;
	pools[pool_idx].end_idx = MAX_POOLS - 1;
	pools[pool_idx].valid = true;
	for (i = 0; i < MAX_POOLS - 1; i++) {
		printk("%s, comparing pool[%d]=%llx and pool[%d]=%llx\n", __func__,
					i+1, (uint64_t)base_pools[i+1], i, (uint64_t) base_pools[i]);
		if (((uint64_t) base_pools[i+1] - (uint64_t) base_pools[i]) != ((1 << pool_order) * PAGE_SIZE)) {
			printk("%s, found discontinuity @ i %d\n", __func__, i);
			pools[pool_idx].valid = true;
			pools[pool_idx++].end_idx = i;
			pools[pool_idx].start_idx = i + 1;
		}
	}

	/* if there is no discontinuity, then we will have a huge chunk until the end */
	pools[pool_idx].valid = true;
	pools[pool_idx].end_idx = MAX_POOLS - 1;

	for (i = 0; i < MAX_POOLS; i++) {
		printk("%s, pool %d: start idx = %d | end idx = %d\n",
				__func__, i, pools[i].start_idx, pools[i].end_idx);
		if (!pools[i].valid)
			continue;
		if ((pools[i].end_idx - pools[i].start_idx + 1) > best_diff) {
			best_idx = i;
			best_diff = pools[i].end_idx - pools[i].start_idx + 1;
		}
	}
	printk("%s, best diff %u | best idx %d | start = %d | end = %d\n",
			__func__, best_diff, best_idx, pools[best_idx].start_idx, pools[best_idx].end_idx);
	return best_idx;
}


void iocb_data_pool_init(void)
{
	pool_base = base_pools[pools[pool_pick()].start_idx];
	pool_size = best_diff * ((1 << pool_order) * PAGE_SIZE);
	iocb_pool = priv_pool_init(BLK_USER_BUF_POOL, (void*) pool_base, pool_size, BLK_USER_BUF_SIZE);
}

void iocb_data_pool_free(void)
{
	if (iocb_pool)
		priv_pool_destroy(iocb_pool);
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

	iocb_data_pool_init();
	return 0;
fail2:
	glue_cap_exit();
fail1:
	return ret;

}

void glue_blk_exit(void)
{
	glue_cap_destroy(c_cspace);
	glue_cap_exit();
	iocb_data_pool_free();
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

	ret = register_blkdev(devno, "nullb");

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
	unregister_blkdev(devno, "nullb");
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

#ifdef CONFIG_LVD
int _queue_rq_fn(struct blk_mq_hw_ctx *ctx, const struct blk_mq_queue_data *bd)
#else
int _queue_rq_fn(struct blk_mq_hw_ctx *ctx, const struct blk_mq_queue_data *bd, struct trampoline_hidden_args *hidden_args)
#endif
{
        int ret;
	struct fipc_message r;
        struct fipc_message *request = &r;

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

	vmfunc_klcd_wrapper(request, 1);

        ret = fipc_get_reg0(request);

        return ret;
}

int init_request(void *data, struct request *req, unsigned int hctx_idx,
			unsigned int rq_idx,
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

#ifndef CONFIG_LVD
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
#endif

#ifdef CONFIG_LVD
struct blk_mq_hw_ctx *_map_queue_fn(struct request_queue *rq, int m)
#else
struct blk_mq_hw_ctx *_map_queue_fn(struct request_queue *rq, int m, struct trampoline_hidden_args *hidden_args)
#endif
{
	/* In kernel v4.9, this function is no longer registered
	 * in the ops field. Because the kernel itself handles
	 * the map. All the kernel does is to call blk_mq_map_queue.
	 * So instead of going to the LCD, I am going to call
	 * blk_mq_map_queue here! */
	/* TODO: Call to LCD and call this function from there */
	return blk_mq_map_queue(rq, m);
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(map_queue_fn_trampoline);
struct blk_mq_hw_ctx *LCD_TRAMPOLINE_LINKAGE(map_queue_fn_trampoline) map_queue_fn_trampoline(struct request_queue *req_queue, int m)

{
	struct blk_mq_hw_ctx* ( *volatile map_queue_fn_fp )(struct request_queue *, int , struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, map_queue_fn_trampoline);
	map_queue_fn_fp = _map_queue_fn;
	return map_queue_fn_fp(req_queue, m, hidden_args);

}
#endif

int init_hctx_sync_callee(struct fipc_message *_request)
{
	int ret;
	struct page *p;
	unsigned int pool_ord;
	cptr_t pool_cptr;
	cptr_t lcd_pool_cptr;

	/* get LCD's pool cptr */
	lcd_pool_cptr = __cptr(fipc_get_reg0(_request));

	p = virt_to_head_page(iocb_pool->pool);

        pool_ord = ilog2(roundup_pow_of_two((
				iocb_pool->total_pages * PAGE_SIZE)
				>> PAGE_SHIFT));

        ret = lcd_volunteer_pages(p, pool_ord, &pool_cptr);

	if (ret) {
		LIBLCD_ERR("volunteer shared data pool");
		goto fail_vol;
	}

	pool_pfn_start = (unsigned long)iocb_pool->pool >> PAGE_SHIFT;
	pool_pfn_end = pool_pfn_start + iocb_pool->total_pages;

	printk("%s, pool pfn start %lu | end %lu\n", __func__,
			pool_pfn_start, pool_pfn_end);

	copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, pool_cptr, lcd_pool_cptr);

	fipc_set_reg0(_request, pool_ord);

fail_vol:
	return 0;
}

#ifdef CONFIG_LVD
int _init_hctx_fn(struct blk_mq_hw_ctx *ctx, void *data, unsigned int index)
#else
int _init_hctx_fn(struct blk_mq_hw_ctx *ctx, void *data, unsigned int index, struct trampoline_hidden_args *hidden_args)
#endif
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;
	int func_ret = 0;
	struct blk_mq_hw_ctx_container *ctx_container;
	/* TODO: Do we need hidden_args to pull this off? */
	struct blk_mq_ops_container *ops_container;

	ctx_container = container_of(ctx, struct blk_mq_hw_ctx_container, blk_mq_hw_ctx); 
#ifdef CONFIG_LVD
	ops_container = g_blk_mq_ops_container;
#else
	ops_container = (struct blk_mq_ops_container *)hidden_args->struct_container;
#endif

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

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(init_hctx_fn_trampoline);
int LCD_TRAMPOLINE_LINKAGE(init_hctx_fn_trampoline) init_hctx_fn_trampoline(struct blk_mq_hw_ctx *ctx, void *data, unsigned int index)

{
	int ( *volatile init_hctx_fn_fp )(struct blk_mq_hw_ctx *, void *, unsigned int, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, init_hctx_fn_trampoline);
	init_hctx_fn_fp = _init_hctx_fn;
	return init_hctx_fn_fp(ctx, data, index, hidden_args);

}
#endif

#ifdef CONFIG_LVD
void _softirq_done_fn(struct request *request)
#else
void _softirq_done_fn(struct request *request, struct trampoline_hidden_args *hidden_args)
#endif
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	async_msg_set_fn_type(_request, SOFTIRQ_DONE_FN);

	vmfunc_klcd_wrapper(_request, 1);

	return;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(softirq_done_fn_trampoline);
void LCD_TRAMPOLINE_LINKAGE(softirq_done_fn_trampoline) softirq_done_fn_trampoline(struct request *request)

{
	void ( *volatile softirq_done_fn_fp )(struct request *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, softirq_done_fn_trampoline);
	softirq_done_fn_fp = _softirq_done_fn;
	return softirq_done_fn_fp(request, hidden_args);

}
#endif

#ifdef CONFIG_LVD
int open(struct block_device *device, fmode_t mode)
#else
int open(struct block_device *device, fmode_t mode, struct trampoline_hidden_args *hidden_args)
#endif
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

#ifdef CONFIG_LVD
void release(struct gendisk *disk, fmode_t mode)
#else
void release(struct gendisk *disk, fmode_t mode, struct trampoline_hidden_args *hidden_args)
#endif
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

int blk_mq_alloc_tag_set_callee(struct fipc_message *request)
{
	struct blk_mq_tag_set_container *set_container;
	struct blk_mq_ops_container *ops_container;

#ifndef CONFIG_LVD
	struct trampoline_hidden_args *queue_rq_hidden_args;
	struct trampoline_hidden_args *queue_rq_async_hidden_args;
	struct trampoline_hidden_args *map_queue_hidden_args;
	struct trampoline_hidden_args *init_hctx_hidden_args;
	struct trampoline_hidden_args *sirq_done_hidden_args;
#endif
	int func_ret = 0;
	int err;

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

	ops_container->other_ref.cptr = fipc_get_reg6(request);
	printk("ops_other_ref %ld \n",ops_container->other_ref.cptr);
	
	/* This is required because, the blk_mq_tag_set which is passed to blk layer is this one */
	set_container->tag_set.ops = &ops_container->blk_mq_ops;

#ifndef CONFIG_LVD
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

#endif
	/* Get the rest of the members from LCD */
	set_container->tag_set.nr_hw_queues = fipc_get_reg1(request);
	set_container->tag_set.queue_depth = fipc_get_reg2(request);
	hw_depth = set_container->tag_set.queue_depth;
	set_container->tag_set.reserved_tags = fipc_get_reg3(request);
	set_container->tag_set.cmd_size = fipc_get_reg4(request);
	set_container->tag_set.flags = fipc_get_reg5(request);

	/* XXX: Populate function pointers including init_request */
	ops_container->blk_mq_ops.init_request = init_request;
	ops_container->blk_mq_ops.queue_rq = _queue_rq_fn;
	ops_container->blk_mq_ops.map_queue = _map_queue_fn;
	ops_container->blk_mq_ops.init_hctx = _init_hctx_fn;
	ops_container->blk_mq_ops.complete = _softirq_done_fn;

	/* call the real function */
	func_ret = blk_mq_alloc_tag_set((&set_container->tag_set));
	LIBLCD_MSG("block_alloc_tag set returns %d",func_ret);

	/* Hack for remove */
	set_g = &set_container->tag_set;
	fipc_set_reg0(request, set_container->my_ref.cptr);
	fipc_set_reg1(request, ops_container->my_ref.cptr);
	fipc_set_reg2(request, func_ret);

	return func_ret;

#ifndef CONFIG_LVD
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
#endif
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

        strcpy(module_container->module.name, "nullb");

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
#endif
	disk = &disk_container->gendisk;
	printk("address of disk before calling add_diks %p \n",disk);

	disk->flags = fipc_get_reg4(request);
	disk->major = fipc_get_reg5(request);
	disk->first_minor = fipc_get_reg6(request);
	disk->queue = &rq_container->request_queue;
	
	size = 250 * 1024 * 1024 * 1024ULL;
	set_capacity(disk, size >> 9);

	blo_container->block_device_operations.open = open;
	blo_container->block_device_operations.release = release;

	disk->fops = &blo_container->block_device_operations;
	sprintf(disk_name, "nullb%d", disk->first_minor);
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
