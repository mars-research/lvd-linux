#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void release_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_release function_ptr = glue_unpack(__pos, msg, ext, fptr_release);
	struct gendisk* disk = 0;
	unsigned int mode = 0;
	
	__maybe_unused struct release_call_ctx call_ctx = {disk, mode};
	__maybe_unused struct release_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	function_ptr(disk, mode);

	*__pos = 0;
	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int register_blkdev(unsigned int major, char const* name)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* major_ptr = &major;
	char const** name_ptr = &name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct register_blkdev_call_ctx call_ctx = {major, name};
	__maybe_unused const struct register_blkdev_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *major_ptr);
	}

	{
		const void* __adjusted = *name_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	glue_call_server(__pos, msg, RPC_ID_register_blkdev);

	*__pos = 0;
	{
	}

	{
		(void)name_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void del_gendisk(struct gendisk* disk)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct gendisk** disk_ptr = &disk;
	
	__maybe_unused const struct del_gendisk_call_ctx call_ctx = {disk};
	__maybe_unused const struct del_gendisk_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *disk_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*disk_ptr) {
			caller_marshal_kernel__del_gendisk__disk__in(__pos, msg, ext, ctx, *disk_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_del_gendisk);

	*__pos = 0;
	{
		if (*disk_ptr) {
			caller_unmarshal_kernel__del_gendisk__disk__in(__pos, msg, ext, ctx, *disk_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void device_add_disk(struct device* parent, struct gendisk* disk)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct gendisk** disk_ptr = &disk;
	
	__maybe_unused const struct device_add_disk_call_ctx call_ctx = {parent, disk};
	__maybe_unused const struct device_add_disk_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *disk_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*disk_ptr) {
			caller_marshal_kernel__device_add_disk__disk__in(__pos, msg, ext, ctx, *disk_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_device_add_disk);

	*__pos = 0;
	{
		if (*disk_ptr) {
			caller_unmarshal_kernel__device_add_disk__disk__in(__pos, msg, ext, ctx, *disk_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_cleanup_queue(struct request_queue* q)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** q_ptr = &q;
	
	__maybe_unused const struct blk_cleanup_queue_call_ctx call_ctx = {q};
	__maybe_unused const struct blk_cleanup_queue_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_cleanup_queue__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_blk_cleanup_queue);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_cleanup_queue__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_end_request(struct request* rq, int error)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** rq_ptr = &rq;
	int* error_ptr = &error;
	
	__maybe_unused const struct blk_mq_end_request_call_ctx call_ctx = {rq, error};
	__maybe_unused const struct blk_mq_end_request_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__blk_mq_end_request__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *error_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_blk_mq_end_request);

	*__pos = 0;
	{
		if (*rq_ptr) {
			caller_unmarshal_kernel__blk_mq_end_request__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void complete_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_complete function_ptr = glue_unpack(__pos, msg, ext, fptr_complete);
	struct request* rq = 0;
	struct request** rq_ptr = &rq;
	
	__maybe_unused struct complete_call_ctx call_ctx = {rq};
	__maybe_unused struct complete_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*rq_ptr = glue_unpack_shadow(__pos, msg, ext, struct request*);
		if (*rq_ptr) {
			callee_unmarshal_kernel__complete__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	function_ptr(rq);

	*__pos = 0;
	{
		if (*rq_ptr) {
			callee_marshal_kernel__complete__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void open_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_open function_ptr = glue_unpack(__pos, msg, ext, fptr_open);
	struct block_device* bdev = 0;
	unsigned int mode = 0;
	unsigned int* mode_ptr = &mode;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct open_call_ctx call_ctx = {bdev, mode};
	__maybe_unused struct open_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*mode_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = function_ptr(bdev, mode);

	*__pos = 0;
	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct gendisk* alloc_disk_node(int minors, int node_id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* minors_ptr = &minors;
	int* node_id_ptr = &node_id;
	struct gendisk* ret = 0;
	struct gendisk** ret_ptr = &ret;
	
	__maybe_unused const struct alloc_disk_node_call_ctx call_ctx = {minors, node_id};
	__maybe_unused const struct alloc_disk_node_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *minors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *node_id_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_alloc_disk_node);

	*__pos = 0;
	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct gendisk*, sizeof(struct gendisk));
		if (*ret_ptr) {
			caller_unmarshal_kernel__alloc_disk_node__ret_gendisk__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void queue_rq_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_queue_rq function_ptr = glue_unpack(__pos, msg, ext, fptr_queue_rq);
	struct blk_mq_hw_ctx* hctx = 0;
	struct blk_mq_queue_data* bd = 0;
	struct blk_mq_hw_ctx** hctx_ptr = &hctx;
	struct blk_mq_queue_data** bd_ptr = &bd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct queue_rq_call_ctx call_ctx = {hctx, bd};
	__maybe_unused struct queue_rq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hctx_ptr = glue_unpack_shadow(__pos, msg, ext, struct blk_mq_hw_ctx*);
		if (*hctx_ptr) {
			callee_unmarshal_kernel__queue_rq__hctx__in(__pos, msg, ext, ctx, *hctx_ptr);
		}

	}

	{
		*bd_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct blk_mq_queue_data*, sizeof(struct blk_mq_queue_data));
		if (*bd_ptr) {
			callee_unmarshal_kernel__queue_rq__bd__in(__pos, msg, ext, ctx, *bd_ptr);
		}

	}

	ret = function_ptr(hctx, bd);

	*__pos = 0;
	{
		if (*hctx_ptr) {
			callee_marshal_kernel__queue_rq__hctx__in(__pos, msg, ext, ctx, *hctx_ptr);
		}

	}

	{
		if (*bd_ptr) {
			callee_marshal_kernel__queue_rq__bd__in(__pos, msg, ext, ctx, *bd_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void init_hctx_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_init_hctx function_ptr = glue_unpack(__pos, msg, ext, fptr_init_hctx);
	struct blk_mq_hw_ctx* hctx = 0;
	void* data = 0;
	unsigned int index = 0;
	struct blk_mq_hw_ctx** hctx_ptr = &hctx;
	void** data_ptr = &data;
	unsigned int* index_ptr = &index;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct init_hctx_call_ctx call_ctx = {hctx, data, index};
	__maybe_unused struct init_hctx_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hctx_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct blk_mq_hw_ctx*, sizeof(struct blk_mq_hw_ctx));
		if (*hctx_ptr) {
			callee_unmarshal_kernel__init_hctx__hctx__in(__pos, msg, ext, ctx, *hctx_ptr);
		}

	}

	{
		*data_ptr = glue_unpack(__pos, msg, ext, void*);
		if (*data_ptr) {
		}

	}

	{
		*index_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = function_ptr(hctx, data, index);

	*__pos = 0;
	{
		if (*hctx_ptr) {
			callee_marshal_kernel__init_hctx__hctx__in(__pos, msg, ext, ctx, *hctx_ptr);
		}

	}

	{
		(void)data_ptr;
	}

	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void map_queue_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_map_queue function_ptr = glue_unpack(__pos, msg, ext, fptr_map_queue);
	struct request_queue* rq = 0;
	int cpu = 0;
	struct request_queue** rq_ptr = &rq;
	int* cpu_ptr = &cpu;
	struct blk_mq_hw_ctx* ret = 0;
	struct blk_mq_hw_ctx** ret_ptr = &ret;
	
	__maybe_unused struct map_queue_call_ctx call_ctx = {rq, cpu};
	__maybe_unused struct map_queue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*rq_ptr = glue_unpack_bind_or_new_shadow(__pos, msg, ext, struct request_queue*, sizeof(struct request_queue));
		if (*rq_ptr) {
			callee_unmarshal_kernel__map_queue__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	{
		*cpu_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = function_ptr(rq, cpu);

	*__pos = 0;
	{
		if (*rq_ptr) {
			callee_marshal_kernel__map_queue__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	{
	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__map_queue__blk_mq_hw_ctx__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void set_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_set function_ptr = glue_unpack(__pos, msg, ext, fptr_set);
	signed char* str = 0;
	struct kernel_param* kp = 0;
	signed char** str_ptr = &str;
	struct kernel_param** kp_ptr = &kp;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct set_call_ctx call_ctx = {str, kp};
	__maybe_unused struct set_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*str_ptr = glue_unpack(__pos, msg, ext, signed char*);
		if (*str_ptr) {
			**str_ptr = glue_unpack(__pos, msg, ext, signed char);
		}

	}

	{
		*kp_ptr = glue_unpack(__pos, msg, ext, struct kernel_param*);
		if (*kp_ptr) {
			callee_unmarshal_kernel__set__kp__in(__pos, msg, ext, ctx, *kp_ptr);
		}

	}

	ret = function_ptr(str, kp);

	*__pos = 0;
	{
		(void)str_ptr;
	}

	{
		if (*kp_ptr) {
			callee_marshal_kernel__set__kp__in(__pos, msg, ext, ctx, *kp_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int param_get_int(char* buffer, struct kernel_param const* kp)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	char** buffer_ptr = &buffer;
	struct kernel_param const** kp_ptr = &kp;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct param_get_int_call_ctx call_ctx = {buffer, kp};
	__maybe_unused const struct param_get_int_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *buffer_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*buffer_ptr) {
			glue_pack(__pos, msg, ext, **buffer_ptr);
		}

	}

	{
		const void* __adjusted = *kp_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*kp_ptr) {
			caller_marshal_kernel__param_get_int__kp__in(__pos, msg, ext, ctx, *kp_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_param_get_int);

	*__pos = 0;
	{
		(void)buffer_ptr;
	}

	{
		if (*kp_ptr) {
			struct kernel_param* writable = (struct kernel_param*)*kp_ptr;
			caller_unmarshal_kernel__param_get_int__kp__in(__pos, msg, ext, ctx, writable);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void prepare_to_wait(struct __wait_queue_head* q, struct __wait_queue* wait, int state)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct __wait_queue_head** q_ptr = &q;
	struct __wait_queue** wait_ptr = &wait;
	int* state_ptr = &state;
	
	__maybe_unused const struct prepare_to_wait_call_ctx call_ctx = {q, wait, state};
	__maybe_unused const struct prepare_to_wait_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__prepare_to_wait__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		const void* __adjusted = *wait_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*wait_ptr) {
			caller_marshal_kernel__prepare_to_wait__wait__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_prepare_to_wait);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__prepare_to_wait__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		if (*wait_ptr) {
			caller_unmarshal_kernel__prepare_to_wait__wait__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void finish_wait(struct __wait_queue_head* q, struct __wait_queue* wait)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct __wait_queue_head** q_ptr = &q;
	struct __wait_queue** wait_ptr = &wait;
	
	__maybe_unused const struct finish_wait_call_ctx call_ctx = {q, wait};
	__maybe_unused const struct finish_wait_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__finish_wait__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		const void* __adjusted = *wait_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*wait_ptr) {
			caller_marshal_kernel__finish_wait__wait__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_finish_wait);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__finish_wait__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		if (*wait_ptr) {
			caller_unmarshal_kernel__finish_wait__wait__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int autoremove_wake_function(struct __wait_queue* wait, unsigned int mode, int sync, void* key)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct __wait_queue** wait_ptr = &wait;
	unsigned int* mode_ptr = &mode;
	int* sync_ptr = &sync;
	void** key_ptr = &key;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct autoremove_wake_function_call_ctx call_ctx = {wait, mode, sync, key};
	__maybe_unused const struct autoremove_wake_function_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *wait_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*wait_ptr) {
			caller_marshal_kernel__autoremove_wake_function__wait__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *mode_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *sync_ptr);
	}

	{
		const void* __adjusted = *key_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*key_ptr) {
		}

	}

	glue_call_server(__pos, msg, RPC_ID_autoremove_wake_function);

	*__pos = 0;
	{
		if (*wait_ptr) {
			caller_unmarshal_kernel__autoremove_wake_function__wait__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	{
	}

	{
	}

	{
		(void)key_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void unregister_blkdev(unsigned int major, char const* name)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* major_ptr = &major;
	char const** name_ptr = &name;
	
	__maybe_unused const struct unregister_blkdev_call_ctx call_ctx = {major, name};
	__maybe_unused const struct unregister_blkdev_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *major_ptr);
	}

	{
		const void* __adjusted = *name_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	glue_call_server(__pos, msg, RPC_ID_unregister_blkdev);

	*__pos = 0;
	{
	}

	{
		(void)name_ptr;
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void put_disk(struct gendisk* disk)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct gendisk** disk_ptr = &disk;
	
	__maybe_unused const struct put_disk_call_ctx call_ctx = {disk};
	__maybe_unused const struct put_disk_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *disk_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*disk_ptr) {
			caller_marshal_kernel__put_disk__disk__in(__pos, msg, ext, ctx, *disk_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_put_disk);

	*__pos = 0;
	{
		if (*disk_ptr) {
			caller_unmarshal_kernel__put_disk__disk__in(__pos, msg, ext, ctx, *disk_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_free_tag_set(struct blk_mq_tag_set* set)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_tag_set** set_ptr = &set;
	
	__maybe_unused const struct blk_mq_free_tag_set_call_ctx call_ctx = {set};
	__maybe_unused const struct blk_mq_free_tag_set_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *set_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*set_ptr) {
			caller_marshal_kernel__blk_mq_free_tag_set__set__in(__pos, msg, ext, ctx, *set_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_blk_mq_free_tag_set);

	*__pos = 0;
	{
		if (*set_ptr) {
			caller_unmarshal_kernel__blk_mq_free_tag_set__set__in(__pos, msg, ext, ctx, *set_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int blk_mq_alloc_tag_set(struct blk_mq_tag_set* set)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_tag_set** set_ptr = &set;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct blk_mq_alloc_tag_set_call_ctx call_ctx = {set};
	__maybe_unused const struct blk_mq_alloc_tag_set_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *set_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*set_ptr) {
			caller_marshal_kernel__blk_mq_alloc_tag_set__set__in(__pos, msg, ext, ctx, *set_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_blk_mq_alloc_tag_set);

	*__pos = 0;
	{
		if (*set_ptr) {
			caller_unmarshal_kernel__blk_mq_alloc_tag_set__set__in(__pos, msg, ext, ctx, *set_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct request_queue* blk_mq_init_queue(struct blk_mq_tag_set* set)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_tag_set** set_ptr = &set;
	struct request_queue* ret = 0;
	struct request_queue** ret_ptr = &ret;
	
	__maybe_unused const struct blk_mq_init_queue_call_ctx call_ctx = {set};
	__maybe_unused const struct blk_mq_init_queue_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *set_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*set_ptr) {
			caller_marshal_kernel__blk_mq_init_queue__set__in(__pos, msg, ext, ctx, *set_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_blk_mq_init_queue);

	*__pos = 0;
	{
		if (*set_ptr) {
			caller_unmarshal_kernel__blk_mq_init_queue__set__in(__pos, msg, ext, ctx, *set_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct request_queue*, sizeof(struct request_queue));
		if (*ret_ptr) {
			caller_unmarshal_kernel__blk_mq_init_queue__ret_request_queue__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct blk_mq_hw_ctx* blk_mq_map_queue(struct request_queue* q, int cpu)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** q_ptr = &q;
	int* cpu_ptr = &cpu;
	struct blk_mq_hw_ctx* ret = 0;
	struct blk_mq_hw_ctx** ret_ptr = &ret;
	
	__maybe_unused const struct blk_mq_map_queue_call_ctx call_ctx = {q, cpu};
	__maybe_unused const struct blk_mq_map_queue_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_mq_map_queue__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *cpu_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_blk_mq_map_queue);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_mq_map_queue__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack_shadow(__pos, msg, ext, struct blk_mq_hw_ctx*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void blk_mq_start_request(struct request* rq)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** rq_ptr = &rq;
	
	__maybe_unused const struct blk_mq_start_request_call_ctx call_ctx = {rq};
	__maybe_unused const struct blk_mq_start_request_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__blk_mq_start_request__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_blk_mq_start_request);

	*__pos = 0;
	{
		if (*rq_ptr) {
			caller_unmarshal_kernel__blk_mq_start_request__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_complete_request(struct request* rq, int error)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** rq_ptr = &rq;
	int* error_ptr = &error;
	
	__maybe_unused const struct blk_mq_complete_request_call_ctx call_ctx = {rq, error};
	__maybe_unused const struct blk_mq_complete_request_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__blk_mq_complete_request__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *error_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_blk_mq_complete_request);

	*__pos = 0;
	{
		if (*rq_ptr) {
			caller_unmarshal_kernel__blk_mq_complete_request__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_queue_physical_block_size(struct request_queue* q, unsigned int size)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** q_ptr = &q;
	unsigned int* size_ptr = &size;
	
	__maybe_unused const struct blk_queue_physical_block_size_call_ctx call_ctx = {q, size};
	__maybe_unused const struct blk_queue_physical_block_size_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_queue_physical_block_size__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *size_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_blk_queue_physical_block_size);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_queue_physical_block_size__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_queue_logical_block_size(struct request_queue* q, unsigned short size)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** q_ptr = &q;
	unsigned short* size_ptr = &size;
	
	__maybe_unused const struct blk_queue_logical_block_size_call_ctx call_ctx = {q, size};
	__maybe_unused const struct blk_queue_logical_block_size_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_queue_logical_block_size__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *size_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_blk_queue_logical_block_size);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_queue_logical_block_size__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext)
{
	switch(id) {
	case MODULE_INIT:
		glue_user_trace("MODULE_INIT");
		__module_lcd_init();
		shared_mem_init();
		break;

	case MODULE_EXIT:
		glue_user_trace("MODULE_EXIT");
		__module_lcd_exit();
		break;

	case RPC_ID_release:
		glue_user_trace("release\n");
		release_callee(msg, ext);
		break;

	case RPC_ID_complete:
		glue_user_trace("complete\n");
		complete_callee(msg, ext);
		break;

	case RPC_ID_open:
		glue_user_trace("open\n");
		open_callee(msg, ext);
		break;

	case RPC_ID_queue_rq:
		glue_user_trace("queue_rq\n");
		queue_rq_callee(msg, ext);
		break;

	case RPC_ID_init_hctx:
		glue_user_trace("init_hctx\n");
		init_hctx_callee(msg, ext);
		break;

	case RPC_ID_map_queue:
		glue_user_trace("map_queue\n");
		map_queue_callee(msg, ext);
		break;

	case RPC_ID_set:
		glue_user_trace("set\n");
		set_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

