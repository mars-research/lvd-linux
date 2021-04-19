#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void trmp_impl_release(fptr_release target, struct gendisk* disk, unsigned int mode)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	
	__maybe_unused const struct release_call_ctx call_ctx = {disk, mode};
	__maybe_unused const struct release_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	glue_call_client(__pos, msg, RPC_ID_release);

	*__pos = 0;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_release)
void LCD_TRAMPOLINE_LINKAGE(trmp_release) trmp_release(struct gendisk* disk, unsigned int mode)
{
	volatile fptr_impl_release impl;
	fptr_release target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_release);
	impl = trmp_impl_release;
	return impl(target, disk, mode);
}

void register_blkdev_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int major = 0;
	char const* name = 0;
	unsigned int* major_ptr = &major;
	char const** name_ptr = &name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct register_blkdev_call_ctx call_ctx = {major, name};
	__maybe_unused struct register_blkdev_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*major_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, sizeof(char) * glue_peek(__pos, msg, ext));
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, msg, ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, char);
			}

		}

	}

	ret = register_blkdev(major, name);

	*__pos = 0;
	{
	}

	{
		(void)name_ptr;
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void del_gendisk_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct gendisk* disk = 0;
	struct gendisk** disk_ptr = &disk;
	
	__maybe_unused struct del_gendisk_call_ctx call_ctx = {disk};
	__maybe_unused struct del_gendisk_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*disk_ptr = glue_unpack(__pos, msg, ext, struct gendisk*);
		if (*disk_ptr) {
			callee_unmarshal_kernel__del_gendisk__disk__in(__pos, msg, ext, ctx, *disk_ptr);
		}

	}

	del_gendisk(disk);

	*__pos = 0;
	{
		if (*disk_ptr) {
			callee_marshal_kernel__del_gendisk__disk__in(__pos, msg, ext, ctx, *disk_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void device_add_disk_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device* parent = 0;
	struct gendisk* disk = 0;
	struct gendisk** disk_ptr = &disk;
	
	__maybe_unused struct device_add_disk_call_ctx call_ctx = {parent, disk};
	__maybe_unused struct device_add_disk_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*disk_ptr = glue_unpack(__pos, msg, ext, struct gendisk*);
		if (*disk_ptr) {
			callee_unmarshal_kernel__device_add_disk__disk__in(__pos, msg, ext, ctx, *disk_ptr);
		}

	}

	device_add_disk(parent, disk);

	*__pos = 0;
	{
		if (*disk_ptr) {
			callee_marshal_kernel__device_add_disk__disk__in(__pos, msg, ext, ctx, *disk_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_cleanup_queue_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue* q = 0;
	struct request_queue** q_ptr = &q;
	
	__maybe_unused struct blk_cleanup_queue_call_ctx call_ctx = {q};
	__maybe_unused struct blk_cleanup_queue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*q_ptr = glue_unpack(__pos, msg, ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_cleanup_queue__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	blk_cleanup_queue(q);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__blk_cleanup_queue__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_end_request_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request* rq = 0;
	int error = 0;
	struct request** rq_ptr = &rq;
	int* error_ptr = &error;
	
	__maybe_unused struct blk_mq_end_request_call_ctx call_ctx = {rq, error};
	__maybe_unused struct blk_mq_end_request_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*rq_ptr = glue_unpack(__pos, msg, ext, struct request*);
		if (*rq_ptr) {
			callee_unmarshal_kernel__blk_mq_end_request__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	{
		*error_ptr = glue_unpack(__pos, msg, ext, int);
	}

	blk_mq_end_request(rq, error);

	*__pos = 0;
	{
		if (*rq_ptr) {
			callee_marshal_kernel__blk_mq_end_request__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void trmp_impl_complete(fptr_complete target, struct request* rq)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** rq_ptr = &rq;
	
	__maybe_unused const struct complete_call_ctx call_ctx = {rq};
	__maybe_unused const struct complete_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *rq_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__complete__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_complete);

	*__pos = 0;
	{
		if (*rq_ptr) {
			caller_unmarshal_kernel__complete__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_complete)
void LCD_TRAMPOLINE_LINKAGE(trmp_complete) trmp_complete(struct request* rq)
{
	volatile fptr_impl_complete impl;
	fptr_complete target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_complete);
	impl = trmp_impl_complete;
	return impl(target, rq);
}

int trmp_impl_open(fptr_open target, struct block_device* bdev, unsigned int mode)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* mode_ptr = &mode;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct open_call_ctx call_ctx = {bdev, mode};
	__maybe_unused const struct open_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		glue_pack(__pos, msg, ext, *mode_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_open);

	*__pos = 0;
	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_open)
int LCD_TRAMPOLINE_LINKAGE(trmp_open) trmp_open(struct block_device* bdev, unsigned int mode)
{
	volatile fptr_impl_open impl;
	fptr_open target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_open);
	impl = trmp_impl_open;
	return impl(target, bdev, mode);
}

void alloc_disk_node_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int minors = 0;
	int node_id = 0;
	int* minors_ptr = &minors;
	int* node_id_ptr = &node_id;
	struct gendisk* ret = 0;
	struct gendisk** ret_ptr = &ret;
	
	__maybe_unused struct alloc_disk_node_call_ctx call_ctx = {minors, node_id};
	__maybe_unused struct alloc_disk_node_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*minors_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*node_id_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = alloc_disk_node(minors, node_id);

	*__pos = 0;
	{
	}

	{
	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__alloc_disk_node__ret_gendisk__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int trmp_impl_queue_rq(fptr_queue_rq target, struct blk_mq_hw_ctx* hctx, struct blk_mq_queue_data* bd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_hw_ctx** hctx_ptr = &hctx;
	struct blk_mq_queue_data** bd_ptr = &bd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct queue_rq_call_ctx call_ctx = {hctx, bd};
	__maybe_unused const struct queue_rq_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *hctx_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*hctx_ptr) {
			caller_marshal_kernel__queue_rq__hctx__in(__pos, msg, ext, ctx, *hctx_ptr);
		}

	}

	{
		const void* __adjusted = *bd_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*bd_ptr) {
			caller_marshal_kernel__queue_rq__bd__in(__pos, msg, ext, ctx, *bd_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_queue_rq);

	*__pos = 0;
	{
		if (*hctx_ptr) {
			caller_unmarshal_kernel__queue_rq__hctx__in(__pos, msg, ext, ctx, *hctx_ptr);
		}

	}

	{
		if (*bd_ptr) {
			caller_unmarshal_kernel__queue_rq__bd__in(__pos, msg, ext, ctx, *bd_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_queue_rq)
int LCD_TRAMPOLINE_LINKAGE(trmp_queue_rq) trmp_queue_rq(struct blk_mq_hw_ctx* hctx, struct blk_mq_queue_data* bd)
{
	volatile fptr_impl_queue_rq impl;
	fptr_queue_rq target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_queue_rq);
	impl = trmp_impl_queue_rq;
	return impl(target, hctx, bd);
}

int trmp_impl_init_hctx(fptr_init_hctx target, struct blk_mq_hw_ctx* hctx, void* data, unsigned int index)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_hw_ctx** hctx_ptr = &hctx;
	void** data_ptr = &data;
	unsigned int* index_ptr = &index;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct init_hctx_call_ctx call_ctx = {hctx, data, index};
	__maybe_unused const struct init_hctx_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *hctx_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*hctx_ptr) {
			caller_marshal_kernel__init_hctx__hctx__in(__pos, msg, ext, ctx, *hctx_ptr);
		}

	}

	{
		const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*data_ptr) {
		}

	}

	{
		glue_pack(__pos, msg, ext, *index_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_init_hctx);

	*__pos = 0;
	{
		if (*hctx_ptr) {
			caller_unmarshal_kernel__init_hctx__hctx__in(__pos, msg, ext, ctx, *hctx_ptr);
		}

	}

	{
		(void)data_ptr;
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_init_hctx)
int LCD_TRAMPOLINE_LINKAGE(trmp_init_hctx) trmp_init_hctx(struct blk_mq_hw_ctx* hctx, void* data, unsigned int index)
{
	volatile fptr_impl_init_hctx impl;
	fptr_init_hctx target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_init_hctx);
	impl = trmp_impl_init_hctx;
	return impl(target, hctx, data, index);
}

struct blk_mq_hw_ctx* trmp_impl_map_queue(fptr_map_queue target, struct request_queue* rq, int cpu)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** rq_ptr = &rq;
	int* cpu_ptr = &cpu;
	struct blk_mq_hw_ctx* ret = 0;
	struct blk_mq_hw_ctx** ret_ptr = &ret;
	
	__maybe_unused const struct map_queue_call_ctx call_ctx = {rq, cpu};
	__maybe_unused const struct map_queue_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *rq_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__map_queue__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *cpu_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_map_queue);

	*__pos = 0;
	{
		if (*rq_ptr) {
			caller_unmarshal_kernel__map_queue__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, struct blk_mq_hw_ctx*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__map_queue__blk_mq_hw_ctx__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_map_queue)
struct blk_mq_hw_ctx* LCD_TRAMPOLINE_LINKAGE(trmp_map_queue) trmp_map_queue(struct request_queue* rq, int cpu)
{
	volatile fptr_impl_map_queue impl;
	fptr_map_queue target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_map_queue);
	impl = trmp_impl_map_queue;
	return impl(target, rq, cpu);
}

int trmp_impl_set(fptr_set target, signed char* str, struct kernel_param* kp)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	signed char** str_ptr = &str;
	struct kernel_param** kp_ptr = &kp;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct set_call_ctx call_ctx = {str, kp};
	__maybe_unused const struct set_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *str_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*str_ptr) {
			glue_pack(__pos, msg, ext, **str_ptr);
		}

	}

	{
		const void* __adjusted = *kp_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*kp_ptr) {
			caller_marshal_kernel__set__kp__in(__pos, msg, ext, ctx, *kp_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_set);

	*__pos = 0;
	{
		(void)str_ptr;
	}

	{
		if (*kp_ptr) {
			caller_unmarshal_kernel__set__kp__in(__pos, msg, ext, ctx, *kp_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_set)
int LCD_TRAMPOLINE_LINKAGE(trmp_set) trmp_set(signed char* str, struct kernel_param* kp)
{
	volatile fptr_impl_set impl;
	fptr_set target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_set);
	impl = trmp_impl_set;
	return impl(target, str, kp);
}

void param_get_int_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	char* buffer = 0;
	struct kernel_param const* kp = 0;
	char** buffer_ptr = &buffer;
	struct kernel_param const** kp_ptr = &kp;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct param_get_int_call_ctx call_ctx = {buffer, kp};
	__maybe_unused struct param_get_int_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*buffer_ptr = glue_unpack(__pos, msg, ext, char*);
		if (*buffer_ptr) {
			**buffer_ptr = glue_unpack(__pos, msg, ext, char);
		}

	}

	{
		*kp_ptr = glue_unpack(__pos, msg, ext, struct kernel_param const*);
		if (*kp_ptr) {
			struct kernel_param* writable = (struct kernel_param*)*kp_ptr;
			callee_unmarshal_kernel__param_get_int__kp__in(__pos, msg, ext, ctx, writable);
		}

	}

	ret = param_get_int(buffer, kp);

	*__pos = 0;
	{
		(void)buffer_ptr;
	}

	{
		if (*kp_ptr) {
			callee_marshal_kernel__param_get_int__kp__in(__pos, msg, ext, ctx, *kp_ptr);
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

void prepare_to_wait_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct __wait_queue_head* q = 0;
	struct __wait_queue* wait = 0;
	int state = 0;
	struct __wait_queue_head** q_ptr = &q;
	struct __wait_queue** wait_ptr = &wait;
	int* state_ptr = &state;
	
	__maybe_unused struct prepare_to_wait_call_ctx call_ctx = {q, wait, state};
	__maybe_unused struct prepare_to_wait_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*q_ptr = glue_unpack(__pos, msg, ext, struct __wait_queue_head*);
		if (*q_ptr) {
			callee_unmarshal_kernel__prepare_to_wait__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		*wait_ptr = glue_unpack(__pos, msg, ext, struct __wait_queue*);
		if (*wait_ptr) {
			callee_unmarshal_kernel__prepare_to_wait__wait__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	{
		*state_ptr = glue_unpack(__pos, msg, ext, int);
	}

	prepare_to_wait(q, wait, state);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__prepare_to_wait__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		if (*wait_ptr) {
			callee_marshal_kernel__prepare_to_wait__wait__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void finish_wait_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct __wait_queue_head* q = 0;
	struct __wait_queue* wait = 0;
	struct __wait_queue_head** q_ptr = &q;
	struct __wait_queue** wait_ptr = &wait;
	
	__maybe_unused struct finish_wait_call_ctx call_ctx = {q, wait};
	__maybe_unused struct finish_wait_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*q_ptr = glue_unpack(__pos, msg, ext, struct __wait_queue_head*);
		if (*q_ptr) {
			callee_unmarshal_kernel__finish_wait__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		*wait_ptr = glue_unpack(__pos, msg, ext, struct __wait_queue*);
		if (*wait_ptr) {
			callee_unmarshal_kernel__finish_wait__wait__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	finish_wait(q, wait);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__finish_wait__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		if (*wait_ptr) {
			callee_marshal_kernel__finish_wait__wait__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void autoremove_wake_function_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct __wait_queue* wait = 0;
	unsigned int mode = 0;
	int sync = 0;
	void* key = 0;
	struct __wait_queue** wait_ptr = &wait;
	unsigned int* mode_ptr = &mode;
	int* sync_ptr = &sync;
	void** key_ptr = &key;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct autoremove_wake_function_call_ctx call_ctx = {wait, mode, sync, key};
	__maybe_unused struct autoremove_wake_function_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*wait_ptr = glue_unpack(__pos, msg, ext, struct __wait_queue*);
		if (*wait_ptr) {
			callee_unmarshal_kernel__autoremove_wake_function__wait__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	{
		*mode_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*sync_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*key_ptr = glue_unpack(__pos, msg, ext, void*);
		if (*key_ptr) {
		}

	}

	ret = autoremove_wake_function(wait, mode, sync, key);

	*__pos = 0;
	{
		if (*wait_ptr) {
			callee_marshal_kernel__autoremove_wake_function__wait__in(__pos, msg, ext, ctx, *wait_ptr);
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
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void unregister_blkdev_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int major = 0;
	char const* name = 0;
	unsigned int* major_ptr = &major;
	char const** name_ptr = &name;
	
	__maybe_unused struct unregister_blkdev_call_ctx call_ctx = {major, name};
	__maybe_unused struct unregister_blkdev_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*major_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, sizeof(char) * glue_peek(__pos, msg, ext));
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, msg, ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, char);
			}

		}

	}

	unregister_blkdev(major, name);

	*__pos = 0;
	{
	}

	{
		(void)name_ptr;
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_free_tag_set_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_tag_set* set = 0;
	struct blk_mq_tag_set** set_ptr = &set;
	
	__maybe_unused struct blk_mq_free_tag_set_call_ctx call_ctx = {set};
	__maybe_unused struct blk_mq_free_tag_set_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*set_ptr = glue_unpack_shadow(__pos, msg, ext, struct blk_mq_tag_set*);
		if (*set_ptr) {
			callee_unmarshal_kernel__blk_mq_free_tag_set__set__in(__pos, msg, ext, ctx, *set_ptr);
		}

	}

	blk_mq_free_tag_set(set);

	*__pos = 0;
	{
		if (*set_ptr) {
			callee_marshal_kernel__blk_mq_free_tag_set__set__in(__pos, msg, ext, ctx, *set_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_alloc_tag_set_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_tag_set* set = 0;
	struct blk_mq_tag_set** set_ptr = &set;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct blk_mq_alloc_tag_set_call_ctx call_ctx = {set};
	__maybe_unused struct blk_mq_alloc_tag_set_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*set_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct blk_mq_tag_set*, sizeof(struct blk_mq_tag_set));
		if (*set_ptr) {
			callee_unmarshal_kernel__blk_mq_alloc_tag_set__set__in(__pos, msg, ext, ctx, *set_ptr);
		}

	}

	ret = blk_mq_alloc_tag_set(set);

	*__pos = 0;
	{
		if (*set_ptr) {
			callee_marshal_kernel__blk_mq_alloc_tag_set__set__in(__pos, msg, ext, ctx, *set_ptr);
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

void blk_mq_init_queue_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_tag_set* set = 0;
	struct blk_mq_tag_set** set_ptr = &set;
	struct request_queue* ret = 0;
	struct request_queue** ret_ptr = &ret;
	
	__maybe_unused struct blk_mq_init_queue_call_ctx call_ctx = {set};
	__maybe_unused struct blk_mq_init_queue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*set_ptr = glue_unpack_shadow(__pos, msg, ext, struct blk_mq_tag_set*);
		if (*set_ptr) {
			callee_unmarshal_kernel__blk_mq_init_queue__set__in(__pos, msg, ext, ctx, *set_ptr);
		}

	}

	ret = blk_mq_init_queue(set);

	*__pos = 0;
	{
		if (*set_ptr) {
			callee_marshal_kernel__blk_mq_init_queue__set__in(__pos, msg, ext, ctx, *set_ptr);
		}

	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__blk_mq_init_queue__ret_request_queue__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_map_queue_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue* q = 0;
	int cpu = 0;
	struct request_queue** q_ptr = &q;
	int* cpu_ptr = &cpu;
	struct blk_mq_hw_ctx* ret = 0;
	struct blk_mq_hw_ctx** ret_ptr = &ret;
	
	__maybe_unused struct blk_mq_map_queue_call_ctx call_ctx = {q, cpu};
	__maybe_unused struct blk_mq_map_queue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*q_ptr = glue_unpack(__pos, msg, ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_mq_map_queue__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		*cpu_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = blk_mq_map_queue(q, cpu);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__blk_mq_map_queue__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_start_request_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request* rq = 0;
	struct request** rq_ptr = &rq;
	
	__maybe_unused struct blk_mq_start_request_call_ctx call_ctx = {rq};
	__maybe_unused struct blk_mq_start_request_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*rq_ptr = glue_unpack(__pos, msg, ext, struct request*);
		if (*rq_ptr) {
			callee_unmarshal_kernel__blk_mq_start_request__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	blk_mq_start_request(rq);

	*__pos = 0;
	{
		if (*rq_ptr) {
			callee_marshal_kernel__blk_mq_start_request__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_complete_request_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request* rq = 0;
	int error = 0;
	struct request** rq_ptr = &rq;
	int* error_ptr = &error;
	
	__maybe_unused struct blk_mq_complete_request_call_ctx call_ctx = {rq, error};
	__maybe_unused struct blk_mq_complete_request_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*rq_ptr = glue_unpack(__pos, msg, ext, struct request*);
		if (*rq_ptr) {
			callee_unmarshal_kernel__blk_mq_complete_request__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	{
		*error_ptr = glue_unpack(__pos, msg, ext, int);
	}

	blk_mq_complete_request(rq, error);

	*__pos = 0;
	{
		if (*rq_ptr) {
			callee_marshal_kernel__blk_mq_complete_request__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_queue_physical_block_size_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue* q = 0;
	unsigned int size = 0;
	struct request_queue** q_ptr = &q;
	unsigned int* size_ptr = &size;
	
	__maybe_unused struct blk_queue_physical_block_size_call_ctx call_ctx = {q, size};
	__maybe_unused struct blk_queue_physical_block_size_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*q_ptr = glue_unpack(__pos, msg, ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_queue_physical_block_size__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		*size_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	blk_queue_physical_block_size(q, size);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__blk_queue_physical_block_size__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_queue_logical_block_size_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue* q = 0;
	unsigned short size = 0;
	struct request_queue** q_ptr = &q;
	unsigned short* size_ptr = &size;
	
	__maybe_unused struct blk_queue_logical_block_size_call_ctx call_ctx = {q, size};
	__maybe_unused struct blk_queue_logical_block_size_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*q_ptr = glue_unpack(__pos, msg, ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_queue_logical_block_size__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
		*size_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	blk_queue_logical_block_size(q, size);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__blk_queue_logical_block_size__q__in(__pos, msg, ext, ctx, *q_ptr);
		}

	}

	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext)
{
	switch(id) {
	case RPC_ID_shared_mem_init:
		glue_user_trace("shared_mem_init\n");
		shared_mem_init_callee(msg, ext);
		break;

	case RPC_ID_register_blkdev:
		glue_user_trace("register_blkdev\n");
		register_blkdev_callee(msg, ext);
		break;

	case RPC_ID_del_gendisk:
		glue_user_trace("del_gendisk\n");
		del_gendisk_callee(msg, ext);
		break;

	case RPC_ID_device_add_disk:
		glue_user_trace("device_add_disk\n");
		device_add_disk_callee(msg, ext);
		break;

	case RPC_ID_blk_cleanup_queue:
		glue_user_trace("blk_cleanup_queue\n");
		blk_cleanup_queue_callee(msg, ext);
		break;

	case RPC_ID_blk_mq_end_request:
		glue_user_trace("blk_mq_end_request\n");
		blk_mq_end_request_callee(msg, ext);
		break;

	case RPC_ID_alloc_disk_node:
		glue_user_trace("alloc_disk_node\n");
		alloc_disk_node_callee(msg, ext);
		break;

	case RPC_ID_param_get_int:
		glue_user_trace("param_get_int\n");
		param_get_int_callee(msg, ext);
		break;

	case RPC_ID_prepare_to_wait:
		glue_user_trace("prepare_to_wait\n");
		prepare_to_wait_callee(msg, ext);
		break;

	case RPC_ID_finish_wait:
		glue_user_trace("finish_wait\n");
		finish_wait_callee(msg, ext);
		break;

	case RPC_ID_autoremove_wake_function:
		glue_user_trace("autoremove_wake_function\n");
		autoremove_wake_function_callee(msg, ext);
		break;

	case RPC_ID_unregister_blkdev:
		glue_user_trace("unregister_blkdev\n");
		unregister_blkdev_callee(msg, ext);
		break;

	case RPC_ID_blk_mq_free_tag_set:
		glue_user_trace("blk_mq_free_tag_set\n");
		blk_mq_free_tag_set_callee(msg, ext);
		break;

	case RPC_ID_blk_mq_alloc_tag_set:
		glue_user_trace("blk_mq_alloc_tag_set\n");
		blk_mq_alloc_tag_set_callee(msg, ext);
		break;

	case RPC_ID_blk_mq_init_queue:
		glue_user_trace("blk_mq_init_queue\n");
		blk_mq_init_queue_callee(msg, ext);
		break;

	case RPC_ID_blk_mq_map_queue:
		glue_user_trace("blk_mq_map_queue\n");
		blk_mq_map_queue_callee(msg, ext);
		break;

	case RPC_ID_blk_mq_start_request:
		glue_user_trace("blk_mq_start_request\n");
		blk_mq_start_request_callee(msg, ext);
		break;

	case RPC_ID_blk_mq_complete_request:
		glue_user_trace("blk_mq_complete_request\n");
		blk_mq_complete_request_callee(msg, ext);
		break;

	case RPC_ID_blk_queue_physical_block_size:
		glue_user_trace("blk_queue_physical_block_size\n");
		blk_queue_physical_block_size_callee(msg, ext);
		break;

	case RPC_ID_blk_queue_logical_block_size:
		glue_user_trace("blk_queue_logical_block_size\n");
		blk_queue_logical_block_size_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

