#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

unsigned long __global_init_var_jiffies(void);

unsigned char __global_init_var_nvme_io_timeout(void);

unsigned int __global_init_var_nvme_max_retries(void);

unsigned char __global_init_var_admin_timeout(void);

struct workqueue_struct* __global_init_var_system_wq(void);

void lvd_alloc_workqueue_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	char const* fmt = 0;
	unsigned int flags = 0;
	int max_active = 0;
	char const** fmt_ptr = &fmt;
	unsigned int* flags_ptr = &flags;
	int* max_active_ptr = &max_active;
	struct workqueue_struct* ret = 0;
	struct workqueue_struct** ret_ptr = &ret;
	
	__maybe_unused struct lvd_alloc_workqueue_call_ctx call_ctx = {fmt, flags, max_active};
	__maybe_unused struct lvd_alloc_workqueue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*fmt_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*fmt_ptr) {
			char* writable = (char*)*fmt_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, __msg, __ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*max_active_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = lvd_alloc_workqueue(fmt, flags, max_active);

	*__pos = 0;
	{
		(void)fmt_ptr;
	}

	{
	}

	{
	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__lvd_alloc_workqueue__ret_workqueue_struct__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __pci_register_driver_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_driver* drv = 0;
	struct module* owner = 0;
	char const* mod_name = 0;
	struct pci_driver** drv_ptr = &drv;
	struct module** owner_ptr = &owner;
	char const** mod_name_ptr = &mod_name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct __pci_register_driver_call_ctx call_ctx = {drv, owner, mod_name};
	__maybe_unused struct __pci_register_driver_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct pci_driver);
		*drv_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct pci_driver*, (__size), (DEFAULT_GFP_FLAGS));
		if (*drv_ptr) {
			callee_unmarshal_kernel___global_pci_driver__in(__pos, __msg, __ext, *drv_ptr);
		}

	}

	{
		size_t __size = sizeof(struct module);
		*owner_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct module*, (__size), (DEFAULT_GFP_FLAGS));
		if (*owner_ptr) {
			callee_unmarshal_kernel____pci_register_driver__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*mod_name_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*mod_name_ptr) {
			char* writable = (char*)*mod_name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, __msg, __ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	ret = __pci_register_driver(drv, THIS_MODULE, mod_name);

	*__pos = 0;
	{
		if (*drv_ptr) {
			callee_marshal_kernel___global_pci_driver__in(__pos, __msg, __ext, *drv_ptr);
		}

	}

	{
		if (*owner_ptr) {
			callee_marshal_kernel____pci_register_driver__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

	{
		(void)mod_name_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_cleanup_queue_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
		*q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_cleanup_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	blk_cleanup_queue(q);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__blk_cleanup_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void trmp_impl_done(fptr_done target, struct request* rq, int id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** rq_ptr = &rq;
	int* id_ptr = &id;
	
	__maybe_unused const struct done_call_ctx call_ctx = {rq, id};
	__maybe_unused const struct done_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__done__rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *id_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_done);

	*__pos = 0;
	{
		if (*rq_ptr) {
			caller_unmarshal_kernel__done__rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_done)
void LCD_TRAMPOLINE_LINKAGE(trmp_done) trmp_done(struct request* rq, int id)
{
	volatile fptr_impl_done impl;
	fptr_done target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_done);
	impl = trmp_impl_done;
	return impl(target, rq, id);
}

void blk_execute_rq_nowait_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue* q = 0;
	struct gendisk* bd_disk = 0;
	struct request* rq = 0;
	int at_head = 0;
	fptr_done done = 0;
	struct request_queue** q_ptr = &q;
	struct request** rq_ptr = &rq;
	int* at_head_ptr = &at_head;
	fptr_done* done_ptr = &done;
	
	__maybe_unused struct blk_execute_rq_nowait_call_ctx call_ctx = {q, bd_disk, rq, at_head, done};
	__maybe_unused struct blk_execute_rq_nowait_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_execute_rq_nowait__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*rq_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request*);
		if (*rq_ptr) {
			callee_unmarshal_kernel__blk_execute_rq_nowait__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
		*at_head_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*done_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, done);
	}

	blk_execute_rq_nowait(q, bd_disk, rq, at_head, done);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__blk_execute_rq_nowait__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_ptr) {
			callee_marshal_kernel__blk_execute_rq_nowait__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_get_queue_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue* q = 0;
	struct request_queue** q_ptr = &q;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct blk_get_queue_call_ctx call_ctx = {q};
	__maybe_unused struct blk_get_queue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_get_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	ret = blk_get_queue(q);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__blk_get_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_alloc_tag_set_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
		size_t __size = sizeof(struct blk_mq_tag_set);
		*set_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct blk_mq_tag_set*, (__size), (DEFAULT_GFP_FLAGS));
		if (*set_ptr) {
			callee_unmarshal_kernel__blk_mq_alloc_tag_set__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	ret = blk_mq_alloc_tag_set(set);

	*__pos = 0;
	{
		if (*set_ptr) {
			callee_marshal_kernel__blk_mq_alloc_tag_set__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_complete_request_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
		*rq_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request*);
		if (*rq_ptr) {
			callee_unmarshal_kernel__blk_mq_complete_request__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
		*error_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	blk_mq_complete_request(rq, error);

	*__pos = 0;
	{
		__maybe_unused const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_ptr) {
			callee_marshal_kernel__blk_mq_complete_request__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_end_request_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
		*rq_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request*);
		if (*rq_ptr) {
			callee_unmarshal_kernel__blk_mq_end_request__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
		*error_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	blk_mq_end_request(rq, error);

	*__pos = 0;
	{
		__maybe_unused const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_ptr) {
			callee_marshal_kernel__blk_mq_end_request__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_free_request_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request* rq = 0;
	struct request** rq_ptr = &rq;
	
	__maybe_unused struct blk_mq_free_request_call_ctx call_ctx = {rq};
	__maybe_unused struct blk_mq_free_request_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*rq_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request*);
		if (*rq_ptr) {
			callee_unmarshal_kernel__blk_mq_free_request__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	blk_mq_free_request(rq);

	*__pos = 0;
	{
		__maybe_unused const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_ptr) {
			callee_marshal_kernel__blk_mq_free_request__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_free_tag_set_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
		*set_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct blk_mq_tag_set*);
		if (*set_ptr) {
			callee_unmarshal_kernel__blk_mq_free_tag_set__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	blk_mq_free_tag_set(set);

	*__pos = 0;
	{
		if (*set_ptr) {
			callee_marshal_kernel__blk_mq_free_tag_set__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_init_queue_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
		*set_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct blk_mq_tag_set*);
		if (*set_ptr) {
			callee_unmarshal_kernel__blk_mq_init_queue__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	ret = blk_mq_init_queue(set);

	*__pos = 0;
	{
		if (*set_ptr) {
			callee_marshal_kernel__blk_mq_init_queue__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__blk_mq_init_queue__ret_request_queue__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct blk_mq_hw_ctx* trmp_impl_blk_mq_ops_map_queue(fptr_blk_mq_ops_map_queue target, struct request_queue* q, int cpu)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** q_ptr = &q;
	int* cpu_ptr = &cpu;
	struct blk_mq_hw_ctx* ret = 0;
	struct blk_mq_hw_ctx** ret_ptr = &ret;
	
	__maybe_unused const struct blk_mq_ops_map_queue_call_ctx call_ctx = {q, cpu};
	__maybe_unused const struct blk_mq_ops_map_queue_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_mq_ops_map_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cpu_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_blk_mq_ops_map_queue);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_mq_ops_map_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, struct blk_mq_hw_ctx*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__blk_mq_ops_map_queue__ret_blk_mq_hw_ctx__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_map_queue)
struct blk_mq_hw_ctx* LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_map_queue) trmp_blk_mq_ops_map_queue(struct request_queue* q, int cpu)
{
	volatile fptr_impl_blk_mq_ops_map_queue impl;
	fptr_blk_mq_ops_map_queue target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_blk_mq_ops_map_queue);
	impl = trmp_impl_blk_mq_ops_map_queue;
	return impl(target, q, cpu);
}

void blk_mq_start_request_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
		*rq_ptr = glue_unpack(__pos, __msg, __ext, struct request*);
		if (*rq_ptr) {
			callee_unmarshal_kernel__blk_mq_start_request__rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	blk_mq_start_request(rq);

	*__pos = 0;
	{
		if (*rq_ptr) {
			callee_marshal_kernel__blk_mq_start_request__rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_start_stopped_hw_queues_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue* q = 0;
	bool async = 0;
	struct request_queue** q_ptr = &q;
	bool* async_ptr = &async;
	
	__maybe_unused struct blk_mq_start_stopped_hw_queues_call_ctx call_ctx = {q, async};
	__maybe_unused struct blk_mq_start_stopped_hw_queues_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_mq_start_stopped_hw_queues__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*async_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	blk_mq_start_stopped_hw_queues(q, async);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__blk_mq_start_stopped_hw_queues__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_stop_hw_queues_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue* q = 0;
	struct request_queue** q_ptr = &q;
	
	__maybe_unused struct blk_mq_stop_hw_queues_call_ctx call_ctx = {q};
	__maybe_unused struct blk_mq_stop_hw_queues_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_mq_stop_hw_queues__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	blk_mq_stop_hw_queues(q);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__blk_mq_stop_hw_queues__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void trmp_impl_tag_iter_fn(fptr_tag_iter_fn target, struct request* rq, void* data, bool enable)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** rq_ptr = &rq;
	void** data_ptr = &data;
	bool* enable_ptr = &enable;
	
	__maybe_unused const struct tag_iter_fn_call_ctx call_ctx = {rq, data, enable};
	__maybe_unused const struct tag_iter_fn_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__tag_iter_fn__rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *enable_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_tag_iter_fn);

	*__pos = 0;
	{
		if (*rq_ptr) {
			caller_unmarshal_kernel__tag_iter_fn__rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
		(void)data_ptr;
	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_tag_iter_fn)
void LCD_TRAMPOLINE_LINKAGE(trmp_tag_iter_fn) trmp_tag_iter_fn(struct request* rq, void* data, bool enable)
{
	volatile fptr_impl_tag_iter_fn impl;
	fptr_tag_iter_fn target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_tag_iter_fn);
	impl = trmp_impl_tag_iter_fn;
	return impl(target, rq, data, enable);
}

void blk_mq_tagset_busy_iter_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_tag_set* tagset = 0;
	fptr_tag_iter_fn fn = 0;
	void* priv = 0;
	struct blk_mq_tag_set** tagset_ptr = &tagset;
	fptr_tag_iter_fn* fn_ptr = &fn;
	void** priv_ptr = &priv;
	
	__maybe_unused struct blk_mq_tagset_busy_iter_call_ctx call_ctx = {tagset, fn, priv};
	__maybe_unused struct blk_mq_tagset_busy_iter_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*tagset_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct blk_mq_tag_set*);
		if (*tagset_ptr) {
			callee_unmarshal_kernel__blk_mq_tagset_busy_iter__tagset__in(__pos, __msg, __ext, ctx, *tagset_ptr);
		}

	}

	{
		*fn_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, tag_iter_fn);
	}

	{
		size_t __size = sizeof(void*);
		*priv_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, void*, (__size), (DEFAULT_GFP_FLAGS));
		if (*priv_ptr) {
		}

	}

	blk_mq_tagset_busy_iter(tagset, fn, priv);

	*__pos = 0;
	{
		if (*tagset_ptr) {
			callee_marshal_kernel__blk_mq_tagset_busy_iter__tagset__in(__pos, __msg, __ext, ctx, *tagset_ptr);
		}

	}

	{
	}

	{
		(void)priv_ptr;
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_update_nr_hw_queues_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_tag_set* set = 0;
	int nr_hw_queues = 0;
	struct blk_mq_tag_set** set_ptr = &set;
	int* nr_hw_queues_ptr = &nr_hw_queues;
	
	__maybe_unused struct blk_mq_update_nr_hw_queues_call_ctx call_ctx = {set, nr_hw_queues};
	__maybe_unused struct blk_mq_update_nr_hw_queues_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*set_ptr = glue_unpack(__pos, __msg, __ext, struct blk_mq_tag_set*);
		if (*set_ptr) {
			callee_unmarshal_kernel__blk_mq_update_nr_hw_queues__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	{
		*nr_hw_queues_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	blk_mq_update_nr_hw_queues(set, nr_hw_queues);

	*__pos = 0;
	{
		if (*set_ptr) {
			callee_marshal_kernel__blk_mq_update_nr_hw_queues__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_put_queue_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue* q = 0;
	struct request_queue** q_ptr = &q;
	
	__maybe_unused struct blk_put_queue_call_ctx call_ctx = {q};
	__maybe_unused struct blk_put_queue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_put_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	blk_put_queue(q);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__blk_put_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void complete_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct completion* x = 0;
	struct completion** x_ptr = &x;
	
	__maybe_unused struct complete_call_ctx call_ctx = {x};
	__maybe_unused struct complete_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*x_ptr = glue_unpack(__pos, __msg, __ext, struct completion*);
		if (*x_ptr) {
			callee_unmarshal_kernel__complete__x__in(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	complete(x);

	*__pos = 0;
	{
		if (*x_ptr) {
			callee_marshal_kernel__complete__x__in(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void del_timer_sync_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list* timer = 0;
	struct timer_list** timer_ptr = &timer;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct del_timer_sync_call_ctx call_ctx = {timer};
	__maybe_unused struct del_timer_sync_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*timer_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct timer_list*);
		if (*timer_ptr) {
			callee_unmarshal_kernel__del_timer_sync__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	ret = del_timer_sync(timer);

	*__pos = 0;
	{
		if (*timer_ptr) {
			callee_marshal_kernel__del_timer_sync__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void destroy_workqueue_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct workqueue_struct* wq = 0;
	struct workqueue_struct** wq_ptr = &wq;
	
	__maybe_unused struct destroy_workqueue_call_ctx call_ctx = {wq};
	__maybe_unused struct destroy_workqueue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*wq_ptr = glue_unpack(__pos, __msg, __ext, struct workqueue_struct*);
		if (*wq_ptr) {
			callee_unmarshal_kernel__destroy_workqueue__wq__in(__pos, __msg, __ext, ctx, *wq_ptr);
		}

	}

	destroy_workqueue(wq);

	*__pos = 0;
	{
		if (*wq_ptr) {
			callee_marshal_kernel__destroy_workqueue__wq__in(__pos, __msg, __ext, ctx, *wq_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void device_release_driver_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device* dev = 0;
	struct device** dev_ptr = &dev;
	
	__maybe_unused struct device_release_driver_call_ctx call_ctx = {dev};
	__maybe_unused struct device_release_driver_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct pci_dev* __pci_dev = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		*dev_ptr = &__pci_dev->dev;
		if (*dev_ptr) {
			callee_unmarshal_kernel__device_release_driver__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	device_release_driver(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__device_release_driver__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void flush_work_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct work_struct* work = 0;
	struct work_struct** work_ptr = &work;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct flush_work_call_ctx call_ctx = {work};
	__maybe_unused struct flush_work_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*work_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct work_struct*);
		if (*work_ptr) {
			callee_unmarshal_kernel__flush_work__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	ret = flush_work(work);

	*__pos = 0;
	{
		if (*work_ptr) {
			callee_marshal_kernel__flush_work__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void free_irq_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int irq = 0;
	void* dev_id = 0;
	unsigned int* irq_ptr = &irq;
	void** dev_id_ptr = &dev_id;
	
	__maybe_unused struct free_irq_call_ctx call_ctx = {irq, dev_id};
	__maybe_unused struct free_irq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*dev_id_ptr = glue_unpack_shadow(__pos, __msg, __ext, void*);
		if (*dev_id_ptr) {
		}

	}

	free_irq(irq, dev_id);

	*__pos = 0;
	{
	}

	{
		(void)dev_id_ptr;
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void trmp_impl_timer_func(fptr_timer_func target, unsigned long data)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long* data_ptr = &data;
	
	__maybe_unused const struct timer_func_call_ctx call_ctx = {data};
	__maybe_unused const struct timer_func_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		glue_pack(__pos, __msg, __ext, *data_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_timer_func);

	*__pos = 0;
	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_timer_func)
void LCD_TRAMPOLINE_LINKAGE(trmp_timer_func) trmp_timer_func(unsigned long data)
{
	volatile fptr_impl_timer_func impl;
	fptr_timer_func target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_timer_func);
	impl = trmp_impl_timer_func;
	return impl(target, data);
}

void get_device_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device* dev = 0;
	struct device** dev_ptr = &dev;
	struct device* ret = 0;
	//struct device** ret_ptr = &ret;
	
	__maybe_unused struct get_device_call_ctx call_ctx = {dev};
	__maybe_unused struct get_device_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct pci_dev* __pci_dev = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		*dev_ptr = &__pci_dev->dev;
		if (*dev_ptr) {
			callee_unmarshal_kernel__get_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	ret = get_device(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__get_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_setup_timer_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list* timer = 0;
	fptr_timer_func func = 0;
	unsigned long data = 0;
	struct timer_list** timer_ptr = &timer;
	fptr_timer_func* func_ptr = &func;
	unsigned long* data_ptr = &data;
	
	__maybe_unused struct lvd_setup_timer_call_ctx call_ctx = {timer, func, data};
	__maybe_unused struct lvd_setup_timer_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct timer_list);
		*timer_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct timer_list*, (__size), (DEFAULT_GFP_FLAGS));
		if (*timer_ptr) {
			callee_unmarshal_kernel__lvd_setup_timer__timer__io(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		*func_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, timer_func);
	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	lvd_setup_timer(timer, func, data);

	*__pos = 0;
	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			callee_marshal_kernel__lvd_setup_timer__timer__io(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_init_work_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct work_struct* work = 0;
	fptr_work_fn work_fn = 0;
	struct work_struct** work_ptr = &work;
	fptr_work_fn* work_fn_ptr = &work_fn;
	
	__maybe_unused struct lvd_init_work_call_ctx call_ctx = {work, work_fn};
	__maybe_unused struct lvd_init_work_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct work_struct);
		*work_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct work_struct*, (__size), (DEFAULT_GFP_FLAGS));
		if (*work_ptr) {
			callee_unmarshal_kernel__lvd_init_work__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	{
		*work_fn_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, work_fn);
	}

	lvd_init_work(work, work_fn);

	*__pos = 0;
	{
		if (*work_ptr) {
			callee_marshal_kernel__lvd_init_work__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ioremap_nocache_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long long phys_addr = 0;
	unsigned long size = 0;
	unsigned long long* phys_addr_ptr = &phys_addr;
	unsigned long* size_ptr = &size;
	cptr_t resource_cptr;
	__maybe_unused unsigned int resource_len;
	cptr_t lcd_resource_cptr;
	void* ret = 0;
	void** ret_ptr = &ret;
	
	__maybe_unused struct ioremap_nocache_call_ctx call_ctx = {phys_addr, size};
	__maybe_unused struct ioremap_nocache_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		lcd_resource_cptr.cptr = glue_unpack(__pos, __msg, __ext, uint64_t);
	}

	{
		*phys_addr_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*size_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	//ret = ioremap_nocache(phys_addr, size);
	ret = (void*) phys_addr;

	*__pos = 0;

	{
		lcd_volunteer_dev_mem(__gpa((uint64_t)*ret_ptr), get_order(size), &resource_cptr);
		copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, resource_cptr, lcd_resource_cptr);
		glue_pack(__pos, __msg, __ext, size);
	}

	{
	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void irq_set_affinity_hint_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int irq = 0;
	struct cpumask const* m = 0;
	unsigned int* irq_ptr = &irq;
	struct cpumask const** m_ptr = &m;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct irq_set_affinity_hint_call_ctx call_ctx = {irq, m};
	__maybe_unused struct irq_set_affinity_hint_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		size_t __size = sizeof(struct cpumask);
		*m_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct cpumask const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*m_ptr) {
			struct cpumask* writable = (struct cpumask*)*m_ptr;
			callee_unmarshal_kernel__irq_set_affinity_hint__m__in(__pos, __msg, __ext, ctx, writable);
		}

	}

	ret = irq_set_affinity_hint(irq, m);

	*__pos = 0;
	{
	}

	{
		if (*m_ptr) {
			callee_marshal_kernel__irq_set_affinity_hint__m__in(__pos, __msg, __ext, ctx, *m_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mod_timer_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list* timer = 0;
	unsigned long expires = 0;
	struct timer_list** timer_ptr = &timer;
	unsigned long* expires_ptr = &expires;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mod_timer_call_ctx call_ctx = {timer, expires};
	__maybe_unused struct mod_timer_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*timer_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct timer_list*);
		if (*timer_ptr) {
			callee_unmarshal_kernel__mod_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		*expires_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	ret = mod_timer(timer, expires);

	*__pos = 0;
	{
		if (*timer_ptr) {
			callee_marshal_kernel__mod_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void trmp_impl_blk_mq_ops_exit_hctx(fptr_blk_mq_ops_exit_hctx target, struct blk_mq_hw_ctx* hctx, unsigned int hctx_idx)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_hw_ctx** hctx_ptr = &hctx;
	
	__maybe_unused const struct blk_mq_ops_exit_hctx_call_ctx call_ctx = {hctx, hctx_idx};
	__maybe_unused const struct blk_mq_ops_exit_hctx_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hctx_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*hctx_ptr) {
			caller_marshal_kernel__blk_mq_ops_exit_hctx__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_blk_mq_ops_exit_hctx);

	*__pos = 0;
	{
		if (*hctx_ptr) {
			caller_unmarshal_kernel__blk_mq_ops_exit_hctx__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_exit_hctx)
void LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_exit_hctx) trmp_blk_mq_ops_exit_hctx(struct blk_mq_hw_ctx* hctx, unsigned int hctx_idx)
{
	volatile fptr_impl_blk_mq_ops_exit_hctx impl;
	fptr_blk_mq_ops_exit_hctx target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_blk_mq_ops_exit_hctx);
	impl = trmp_impl_blk_mq_ops_exit_hctx;
	return impl(target, hctx, hctx_idx);
}

int trmp_impl_blk_mq_ops_init_hctx(fptr_blk_mq_ops_init_hctx target, struct blk_mq_hw_ctx* hctx, void* data, unsigned int hctx_idx)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_hw_ctx** hctx_ptr = &hctx;
	void** data_ptr = &data;
	unsigned int* hctx_idx_ptr = &hctx_idx;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct blk_mq_ops_init_hctx_call_ctx call_ctx = {hctx, data, hctx_idx};
	__maybe_unused const struct blk_mq_ops_init_hctx_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		//printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hctx_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*hctx_ptr) {
			caller_marshal_kernel__blk_mq_ops_init_hctx__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *hctx_idx_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_blk_mq_ops_init_hctx);

	*__pos = 0;
	{
		if (*hctx_ptr) {
			caller_unmarshal_kernel__blk_mq_ops_init_hctx__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	{
		(void)data_ptr;
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		//printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_init_hctx)
int LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_init_hctx) trmp_blk_mq_ops_init_hctx(struct blk_mq_hw_ctx* hctx, void* data, unsigned int hctx_idx)
{
	volatile fptr_impl_blk_mq_ops_init_hctx impl;
	fptr_blk_mq_ops_init_hctx target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_blk_mq_ops_init_hctx);
	impl = trmp_impl_blk_mq_ops_init_hctx;
	return impl(target, hctx, data, hctx_idx);
}

int trmp_impl_blk_mq_ops_init_request(fptr_blk_mq_ops_init_request target, void* data, struct request* req, unsigned int hctx_idx, unsigned int rq_idx, unsigned int numa_node)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	void** data_ptr = &data;
	struct request** req_ptr = &req;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct blk_mq_ops_init_request_call_ctx call_ctx = {data, req, hctx_idx, rq_idx, numa_node};
	__maybe_unused const struct blk_mq_ops_init_request_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		//printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
		}

	}

	{
		__maybe_unused const void* __adjusted = *req_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*req_ptr) {
			caller_marshal_kernel__blk_mq_ops_init_request__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_blk_mq_ops_init_request);

	*__pos = 0;
	{
		(void)data_ptr;
	}

	{
		if (*req_ptr) {
			caller_unmarshal_kernel__blk_mq_ops_init_request__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		//printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_init_request)
int LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_init_request) trmp_blk_mq_ops_init_request(void* data, struct request* req, unsigned int hctx_idx, unsigned int rq_idx, unsigned int numa_node)
{
	volatile fptr_impl_blk_mq_ops_init_request impl;
	fptr_blk_mq_ops_init_request target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_blk_mq_ops_init_request);
	impl = trmp_impl_blk_mq_ops_init_request;
	return impl(target, data, req, hctx_idx, rq_idx, numa_node);
}

void nvme_alloc_request_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue* q = 0;
	unsigned int flags = 0;
	int qid = 0;
	struct request_queue** q_ptr = &q;
	struct nvme_command __cmd;
	struct nvme_command* cmd = &__cmd;
	struct nvme_command** cmd_ptr = &cmd;
	unsigned int* flags_ptr = &flags;
	int* qid_ptr = &qid;
	struct request* ret = 0;
	struct request** ret_ptr = &ret;
	
	__maybe_unused struct nvme_alloc_request_call_ctx call_ctx = {q, cmd, flags, qid};
	__maybe_unused struct nvme_alloc_request_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__nvme_alloc_request__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		if (*cmd_ptr) {
			callee_unmarshal_kernel__nvme_alloc_request__cmd__io(__pos, __msg, __ext, ctx, *cmd_ptr);
		}

	}

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*qid_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = nvme_alloc_request(q, cmd, flags, qid);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__nvme_alloc_request__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		if (*cmd_ptr) {
			callee_marshal_kernel__nvme_alloc_request__cmd__io(__pos, __msg, __ext, ctx, *cmd_ptr);
		}

	}

	{
	}

	{
	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__nvme_alloc_request__ret_request__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_cancel_request_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request* req = 0;
	void* data = 0;
	bool reserved = 0;
	struct request** req_ptr = &req;
	
	__maybe_unused struct nvme_cancel_request_call_ctx call_ctx = {req, data, reserved};
	__maybe_unused struct nvme_cancel_request_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*req_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request*);
		if (*req_ptr) {
			callee_unmarshal_kernel__nvme_cancel_request__req__io(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	nvme_cancel_request(req, data, reserved);

	*__pos = 0;
	{
		__maybe_unused const void* __adjusted = *req_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*req_ptr) {
			callee_marshal_kernel__nvme_cancel_request__req__io(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_change_ctrl_state_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	unsigned int new_state = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	unsigned int* new_state_ptr = &new_state;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct nvme_change_ctrl_state_call_ctx call_ctx = {ctrl, new_state};
	__maybe_unused struct nvme_change_ctrl_state_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_change_ctrl_state__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		*new_state_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = nvme_change_ctrl_state(ctrl, new_state);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_change_ctrl_state__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_complete_async_event_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	struct nvme_completion __cqe;
	struct nvme_completion* cqe = &__cqe;
	struct nvme_completion** cqe_ptr = &cqe;
	
	__maybe_unused struct nvme_complete_async_event_call_ctx call_ctx = {ctrl, cqe};
	__maybe_unused struct nvme_complete_async_event_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_complete_async_event__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		if (*cqe_ptr) {
			callee_unmarshal_kernel__nvme_complete_async_event__cqe__in(__pos, __msg, __ext, ctx, *cqe_ptr);
		}

	}

	nvme_complete_async_event(ctrl, cqe);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_complete_async_event__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		if (*cqe_ptr) {
			callee_marshal_kernel__nvme_complete_async_event__cqe__in(__pos, __msg, __ext, ctx, *cqe_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void trmp_impl_blk_mq_ops_complete(fptr_blk_mq_ops_complete target, struct request* req)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** req_ptr = &req;
	
	__maybe_unused const struct blk_mq_ops_complete_call_ctx call_ctx = {req};
	__maybe_unused const struct blk_mq_ops_complete_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *req_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*req_ptr) {
			caller_marshal_kernel__blk_mq_ops_complete__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_blk_mq_ops_complete);

	*__pos = 0;
	{
		if (*req_ptr) {
			caller_unmarshal_kernel__blk_mq_ops_complete__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_complete)
void LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_complete) trmp_blk_mq_ops_complete(struct request* req)
{
	volatile fptr_impl_blk_mq_ops_complete impl;
	fptr_blk_mq_ops_complete target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_blk_mq_ops_complete);
	impl = trmp_impl_blk_mq_ops_complete;
	return impl(target, req);
}

void nvme_disable_ctrl_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	unsigned long long cap = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	unsigned long long* cap_ptr = &cap;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct nvme_disable_ctrl_call_ctx call_ctx = {ctrl, cap};
	__maybe_unused struct nvme_disable_ctrl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_disable_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		*cap_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	ret = nvme_disable_ctrl(ctrl, cap);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_disable_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_enable_ctrl_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	unsigned long long cap = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	unsigned long long* cap_ptr = &cap;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct nvme_enable_ctrl_call_ctx call_ctx = {ctrl, cap};
	__maybe_unused struct nvme_enable_ctrl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_enable_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		*cap_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	ret = nvme_enable_ctrl(ctrl, cap);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_enable_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_cleanup_aer_uncorrect_error_status_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_cleanup_aer_uncorrect_error_status_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_cleanup_aer_uncorrect_error_status_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_cleanup_aer_uncorrect_error_status__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	ret = pci_cleanup_aer_uncorrect_error_status(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_cleanup_aer_uncorrect_error_status__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned int trmp_impl_pci_error_handlers_error_detected(fptr_pci_error_handlers_error_detected target, struct pci_dev* pdev, unsigned int state)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	unsigned int* state_ptr = &state;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_error_handlers_error_detected_call_ctx call_ctx = {pdev, state};
	__maybe_unused const struct pci_error_handlers_error_detected_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_error_handlers_error_detected__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_pci_error_handlers_error_detected);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_error_handlers_error_detected__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_pci_error_handlers_error_detected)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_pci_error_handlers_error_detected) trmp_pci_error_handlers_error_detected(struct pci_dev* pdev, unsigned int state)
{
	volatile fptr_impl_pci_error_handlers_error_detected impl;
	fptr_pci_error_handlers_error_detected target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_pci_error_handlers_error_detected);
	impl = trmp_impl_pci_error_handlers_error_detected;
	return impl(target, pdev, state);
}

void trmp_impl_pci_error_handlers_resume(fptr_pci_error_handlers_resume target, struct pci_dev* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused const struct pci_error_handlers_resume_call_ctx call_ctx = {pdev};
	__maybe_unused const struct pci_error_handlers_resume_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_error_handlers_resume__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_pci_error_handlers_resume);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_error_handlers_resume__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_pci_error_handlers_resume)
void LCD_TRAMPOLINE_LINKAGE(trmp_pci_error_handlers_resume) trmp_pci_error_handlers_resume(struct pci_dev* pdev)
{
	volatile fptr_impl_pci_error_handlers_resume impl;
	fptr_pci_error_handlers_resume target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_pci_error_handlers_resume);
	impl = trmp_impl_pci_error_handlers_resume;
	return impl(target, pdev);
}

int trmp_impl_reg_read32(fptr_reg_read32 target, struct nvme_ctrl* ctrl, unsigned int off, unsigned int* val)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	unsigned int* off_ptr = &off;
	unsigned int** val_ptr = &val;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct reg_read32_call_ctx call_ctx = {ctrl, off, val};
	__maybe_unused const struct reg_read32_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__reg_read32__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *off_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *val_ptr;
		(void)val_ptr;
	}

	glue_call_client(__pos, __msg, RPC_ID_reg_read32);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__reg_read32__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
	}

	{
		if (*val_ptr) {
			**val_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_reg_read32)
int LCD_TRAMPOLINE_LINKAGE(trmp_reg_read32) trmp_reg_read32(struct nvme_ctrl* ctrl, unsigned int off, unsigned int* val)
{
	volatile fptr_impl_reg_read32 impl;
	fptr_reg_read32 target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_reg_read32);
	impl = trmp_impl_reg_read32;
	return impl(target, ctrl, off, val);
}

int trmp_impl_reg_write32(fptr_reg_write32 target, struct nvme_ctrl* ctrl, unsigned int off, unsigned int val)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	unsigned int* off_ptr = &off;
	unsigned int* val_ptr = &val;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct reg_write32_call_ctx call_ctx = {ctrl, off, val};
	__maybe_unused const struct reg_write32_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__reg_write32__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *off_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *val_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_reg_write32);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__reg_write32__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_reg_write32)
int LCD_TRAMPOLINE_LINKAGE(trmp_reg_write32) trmp_reg_write32(struct nvme_ctrl* ctrl, unsigned int off, unsigned int val)
{
	volatile fptr_impl_reg_write32 impl;
	fptr_reg_write32 target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_reg_write32);
	impl = trmp_impl_reg_write32;
	return impl(target, ctrl, off, val);
}

int trmp_impl_reg_read64(fptr_reg_read64 target, struct nvme_ctrl* ctrl, unsigned int off, unsigned long long* val)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	unsigned int* off_ptr = &off;
	unsigned long long** val_ptr = &val;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct reg_read64_call_ctx call_ctx = {ctrl, off, val};
	__maybe_unused const struct reg_read64_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__reg_read64__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *off_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *val_ptr;
		(void)val_ptr;
	}

	glue_call_client(__pos, __msg, RPC_ID_reg_read64);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__reg_read64__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
	}

	{
		if (*val_ptr) {
			**val_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_reg_read64)
int LCD_TRAMPOLINE_LINKAGE(trmp_reg_read64) trmp_reg_read64(struct nvme_ctrl* ctrl, unsigned int off, unsigned long long* val)
{
	volatile fptr_impl_reg_read64 impl;
	fptr_reg_read64 target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_reg_read64);
	impl = trmp_impl_reg_read64;
	return impl(target, ctrl, off, val);
}

int trmp_impl_reset_ctrl(fptr_reset_ctrl target, struct nvme_ctrl* ctrl)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct reset_ctrl_call_ctx call_ctx = {ctrl};
	__maybe_unused const struct reset_ctrl_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__reset_ctrl__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_reset_ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__reset_ctrl__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_reset_ctrl)
int LCD_TRAMPOLINE_LINKAGE(trmp_reset_ctrl) trmp_reset_ctrl(struct nvme_ctrl* ctrl)
{
	volatile fptr_impl_reset_ctrl impl;
	fptr_reset_ctrl target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_reset_ctrl);
	impl = trmp_impl_reset_ctrl;
	return impl(target, ctrl);
}

void trmp_impl_free_ctrl(fptr_free_ctrl target, struct nvme_ctrl* ctrl)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused const struct free_ctrl_call_ctx call_ctx = {ctrl};
	__maybe_unused const struct free_ctrl_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__free_ctrl__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_free_ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__free_ctrl__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_free_ctrl)
void LCD_TRAMPOLINE_LINKAGE(trmp_free_ctrl) trmp_free_ctrl(struct nvme_ctrl* ctrl)
{
	volatile fptr_impl_free_ctrl impl;
	fptr_free_ctrl target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_free_ctrl);
	impl = trmp_impl_free_ctrl;
	return impl(target, ctrl);
}

void trmp_impl_post_scan(fptr_post_scan target, struct nvme_ctrl* ctrl)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused const struct post_scan_call_ctx call_ctx = {ctrl};
	__maybe_unused const struct post_scan_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__post_scan__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_post_scan);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__post_scan__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_post_scan)
void LCD_TRAMPOLINE_LINKAGE(trmp_post_scan) trmp_post_scan(struct nvme_ctrl* ctrl)
{
	volatile fptr_impl_post_scan impl;
	fptr_post_scan target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_post_scan);
	impl = trmp_impl_post_scan;
	return impl(target, ctrl);
}

void trmp_impl_submit_async_event(fptr_submit_async_event target, struct nvme_ctrl* ctrl, int aer_idx)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	int* aer_idx_ptr = &aer_idx;
	
	__maybe_unused const struct submit_async_event_call_ctx call_ctx = {ctrl, aer_idx};
	__maybe_unused const struct submit_async_event_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__submit_async_event__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *aer_idx_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_submit_async_event);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__submit_async_event__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_submit_async_event)
void LCD_TRAMPOLINE_LINKAGE(trmp_submit_async_event) trmp_submit_async_event(struct nvme_ctrl* ctrl, int aer_idx)
{
	volatile fptr_impl_submit_async_event impl;
	fptr_submit_async_event target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_submit_async_event);
	impl = trmp_impl_submit_async_event;
	return impl(target, ctrl, aer_idx);
}

void nvme_init_ctrl_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	struct device* dev = 0;
	struct nvme_ctrl_ops const* ops = 0;
	unsigned long quirks = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	struct device** dev_ptr = &dev;
	struct nvme_ctrl_ops const** ops_ptr = &ops;
	unsigned long* quirks_ptr = &quirks;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct nvme_init_ctrl_call_ctx call_ctx = {ctrl, dev, ops, quirks};
	__maybe_unused struct nvme_init_ctrl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct nvme_ctrl);
		*ctrl_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct nvme_ctrl*, (__size), (DEFAULT_GFP_FLAGS));
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_init_ctrl__ctrl__io(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		struct pci_dev* __pci_dev = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		*dev_ptr = &__pci_dev->dev;
		if (*dev_ptr) {
			callee_unmarshal_kernel__nvme_init_ctrl__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct nvme_ctrl_ops);
		*ops_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct nvme_ctrl_ops const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*ops_ptr) {
			struct nvme_ctrl_ops* writable = (struct nvme_ctrl_ops*)*ops_ptr;
			callee_unmarshal_kernel__nvme_init_ctrl__ops__in(__pos, __msg, __ext, ctx, writable);
		}

	}

	{
		*quirks_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	ret = nvme_init_ctrl(ctrl, dev, ops, quirks);

	*__pos = 0;
	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_init_ctrl__ctrl__io(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		if (*dev_ptr) {
			callee_marshal_kernel__nvme_init_ctrl__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*ops_ptr) {
			callee_marshal_kernel__nvme_init_ctrl__ops__in(__pos, __msg, __ext, ctx, *ops_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_init_identify_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct nvme_init_identify_call_ctx call_ctx = {ctrl};
	__maybe_unused struct nvme_init_identify_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_init_identify__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	ret = nvme_init_identify(ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_init_identify__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_kill_queues_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused struct nvme_kill_queues_call_ctx call_ctx = {ctrl};
	__maybe_unused struct nvme_kill_queues_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_kill_queues__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	nvme_kill_queues(ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_kill_queues__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int trmp_impl_pci_driver_sriov_configure(fptr_pci_driver_sriov_configure target, struct pci_dev* pdev, int numvfs)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	int* numvfs_ptr = &numvfs;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_driver_sriov_configure_call_ctx call_ctx = {pdev, numvfs};
	__maybe_unused const struct pci_driver_sriov_configure_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_driver_sriov_configure__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *numvfs_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_pci_driver_sriov_configure);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_driver_sriov_configure__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_pci_driver_sriov_configure)
int LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_sriov_configure) trmp_pci_driver_sriov_configure(struct pci_dev* pdev, int numvfs)
{
	volatile fptr_impl_pci_driver_sriov_configure impl;
	fptr_pci_driver_sriov_configure target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_pci_driver_sriov_configure);
	impl = trmp_impl_pci_driver_sriov_configure;
	return impl(target, pdev, numvfs);
}

int trmp_impl_blk_mq_ops_poll(fptr_blk_mq_ops_poll target, struct blk_mq_hw_ctx* hctx, unsigned int tag)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_hw_ctx** hctx_ptr = &hctx;
	unsigned int* tag_ptr = &tag;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct blk_mq_ops_poll_call_ctx call_ctx = {hctx, tag};
	__maybe_unused const struct blk_mq_ops_poll_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hctx_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hctx_ptr) {
			caller_marshal_kernel__blk_mq_ops_poll__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tag_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_blk_mq_ops_poll);

	*__pos = 0;
	{
		if (*hctx_ptr) {
			caller_unmarshal_kernel__blk_mq_ops_poll__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_poll)
int LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_poll) trmp_blk_mq_ops_poll(struct blk_mq_hw_ctx* hctx, unsigned int tag)
{
	volatile fptr_impl_blk_mq_ops_poll impl;
	fptr_blk_mq_ops_poll target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_blk_mq_ops_poll);
	impl = trmp_impl_blk_mq_ops_poll;
	return impl(target, hctx, tag);
}

int trmp_impl_pci_driver_probe(fptr_pci_driver_probe target, struct pci_dev* pdev, struct pci_device_id* ent)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	struct pci_device_id** ent_ptr = &ent;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_driver_probe_call_ctx call_ctx = {pdev, ent};
	__maybe_unused const struct pci_driver_probe_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_driver_probe__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ent_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ent_ptr) {
			caller_marshal_kernel__pci_driver_probe__ent__in(__pos, __msg, __ext, ctx, *ent_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_pci_driver_probe);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_driver_probe__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		if (*ent_ptr) {
			caller_unmarshal_kernel__pci_driver_probe__ent__in(__pos, __msg, __ext, ctx, *ent_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_pci_driver_probe)
int LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_probe) trmp_pci_driver_probe(struct pci_dev* pdev, struct pci_device_id* ent)
{
	volatile fptr_impl_pci_driver_probe impl;
	fptr_pci_driver_probe target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_pci_driver_probe);
	impl = trmp_impl_pci_driver_probe;
	return impl(target, pdev, ent);
}

void nvme_put_ctrl_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused struct nvme_put_ctrl_call_ctx call_ctx = {ctrl};
	__maybe_unused struct nvme_put_ctrl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_put_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	nvme_put_ctrl(ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_put_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_queue_async_events_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused struct nvme_queue_async_events_call_ctx call_ctx = {ctrl};
	__maybe_unused struct nvme_queue_async_events_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_queue_async_events__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	nvme_queue_async_events(ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_queue_async_events__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

#define MAX_RQ_BUFS	64
#define SHARED_POOL_SIZE	(256UL << 20)

bool is_in_range(void *mem) {
	if ((mem < bdata_data_pool) || (mem > (bdata_data_pool + SHARED_POOL_SIZE))) {
		return false;
	}
	return true;
}

int trmp_impl_blk_mq_ops_queue_rq(fptr_blk_mq_ops_queue_rq target, struct blk_mq_hw_ctx* hctx, struct blk_mq_queue_data* bd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_hw_ctx** hctx_ptr = &hctx;
	struct blk_mq_queue_data** bd_ptr = &bd;
	int ret = 0;
	int* ret_ptr = &ret;
	void *lcd_buf[MAX_RQ_BUFS] = {0};
	
	__maybe_unused const struct blk_mq_ops_queue_rq_call_ctx call_ctx = {hctx, bd};
	__maybe_unused const struct blk_mq_ops_queue_rq_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hctx_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*hctx_ptr) {
			caller_marshal_kernel__blk_mq_ops_queue_rq__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *bd_ptr;
		if (*bd_ptr) {
			caller_marshal_kernel__blk_mq_ops_queue_rq__bd__in(__pos, __msg, __ext, ctx, *bd_ptr);
		}

		if (blk_rq_bytes(bd->rq)) {
			struct req_iterator iter;
			struct bio_vec bvec;
			int i = 0;
			{

				struct req_iterator iter;
				struct bio_vec bvec;
				int num_segs = 0;
				rq_for_each_segment(bvec, bd->rq, iter) {
					num_segs++;
				}
				// pack num_segments
				glue_pack(__pos, __msg, __ext, num_segs);
			}

			rq_for_each_segment(bvec, bd->rq, iter) {
				void *buf = page_address(bvec.bv_page);
				lcd_buf[i] = priv_alloc(BLK_USER_BUF_POOL);
				if (0)
					printk("%s, pool_base: %p alloc from priv[%d]: %p offset: 0x%lx\n",
							__func__, bdata_data_pool,
							smp_processor_id(),
							lcd_buf[i],
							(unsigned long)(lcd_buf[i] - bdata_data_pool));
				if (lcd_buf[i] && is_in_range(lcd_buf[i]))
					memcpy(lcd_buf[i], buf + bvec.bv_offset, bvec.bv_len);
				else {
					LIBLCD_ERR("%s Allocated buffer not in range", __func__);
					continue;
				}
				glue_pack(__pos, __msg, __ext, (lcd_buf[i] - bdata_data_pool));
				i++;
			}
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_blk_mq_ops_queue_rq);

	*__pos = 0;
	{
		if (*hctx_ptr) {
			caller_unmarshal_kernel__blk_mq_ops_queue_rq__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	{
		if (*bd_ptr) {
			caller_unmarshal_kernel__blk_mq_ops_queue_rq__bd__in(__pos, __msg, __ext, ctx, *bd_ptr);
		}

		if (blk_rq_bytes(bd->rq)) {
			struct req_iterator iter;
			struct bio_vec bvec;
			int i = 0;

			if (!bd->rq->bio->bi_io_vec) {
				printk("%s, io_vec list empty for rq: %p\n", __func__, bd->rq);
				goto done;
			}
			rq_for_each_segment(bvec, bd->rq, iter) {
				void *buf = page_address(bvec.bv_page);
				if (lcd_buf[i] && buf) {
					memcpy(buf + bvec.bv_offset, lcd_buf[i], bvec.bv_len);
					priv_free(lcd_buf[i], BLK_USER_BUF_POOL);
					i++;
				}
			}
		}

	}
done:
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_queue_rq)
int LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_queue_rq) trmp_blk_mq_ops_queue_rq(struct blk_mq_hw_ctx* hctx, struct blk_mq_queue_data* bd)
{
	volatile fptr_impl_blk_mq_ops_queue_rq impl;
	fptr_blk_mq_ops_queue_rq target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_blk_mq_ops_queue_rq);
	impl = trmp_impl_blk_mq_ops_queue_rq;
	return impl(target, hctx, bd);
}

void nvme_queue_scan_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused struct nvme_queue_scan_call_ctx call_ctx = {ctrl};
	__maybe_unused struct nvme_queue_scan_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_queue_scan__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	nvme_queue_scan(ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_queue_scan__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void trmp_impl_pci_driver_remove(fptr_pci_driver_remove target, struct pci_dev* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused const struct pci_driver_remove_call_ctx call_ctx = {pdev};
	__maybe_unused const struct pci_driver_remove_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_driver_remove__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_pci_driver_remove);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_driver_remove__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_pci_driver_remove)
void LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_remove) trmp_pci_driver_remove(struct pci_dev* pdev)
{
	volatile fptr_impl_pci_driver_remove impl;
	fptr_pci_driver_remove target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_pci_driver_remove);
	impl = trmp_impl_pci_driver_remove;
	return impl(target, pdev);
}

void nvme_remove_namespaces_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused struct nvme_remove_namespaces_call_ctx call_ctx = {ctrl};
	__maybe_unused struct nvme_remove_namespaces_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_remove_namespaces__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	nvme_remove_namespaces(ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_remove_namespaces__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_requeue_req_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request* req = 0;
	struct request** req_ptr = &req;
	
	__maybe_unused struct nvme_requeue_req_call_ctx call_ctx = {req};
	__maybe_unused struct nvme_requeue_req_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*req_ptr = glue_unpack(__pos, __msg, __ext, struct request*);
		if (*req_ptr) {
			callee_unmarshal_kernel__nvme_requeue_req__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	nvme_requeue_req(req);

	*__pos = 0;
	{
		if (*req_ptr) {
			callee_marshal_kernel__nvme_requeue_req__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void trmp_impl_pci_error_handlers_reset_notify(fptr_pci_error_handlers_reset_notify target, struct pci_dev* pdev, bool prepare)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	bool* prepare_ptr = &prepare;
	
	__maybe_unused const struct pci_error_handlers_reset_notify_call_ctx call_ctx = {pdev, prepare};
	__maybe_unused const struct pci_error_handlers_reset_notify_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_error_handlers_reset_notify__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *prepare_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_pci_error_handlers_reset_notify);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_error_handlers_reset_notify__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_pci_error_handlers_reset_notify)
void LCD_TRAMPOLINE_LINKAGE(trmp_pci_error_handlers_reset_notify) trmp_pci_error_handlers_reset_notify(struct pci_dev* pdev, bool prepare)
{
	volatile fptr_impl_pci_error_handlers_reset_notify impl;
	fptr_pci_error_handlers_reset_notify target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_pci_error_handlers_reset_notify);
	impl = trmp_impl_pci_error_handlers_reset_notify;
	return impl(target, pdev, prepare);
}

void nvme_set_queue_count_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	int __count;
	int* count = &__count;
	int** count_ptr = &count;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct nvme_set_queue_count_call_ctx call_ctx = {ctrl, count};
	__maybe_unused struct nvme_set_queue_count_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_set_queue_count__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		if (*count_ptr) {
			**count_ptr = glue_unpack(__pos, __msg, __ext, int);
		}

	}

	ret = nvme_set_queue_count(ctrl, count);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_set_queue_count__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		if (*count_ptr) {
			glue_pack(__pos, __msg, __ext, **count_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_setup_cmd_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ns* ns = 0;
	struct request* req = 0;
	struct nvme_ns** ns_ptr = &ns;
	struct request** req_ptr = &req;
	struct nvme_command __cmd;
	struct nvme_command* cmd = &__cmd;
	struct nvme_command** cmd_ptr = &cmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct nvme_setup_cmd_call_ctx call_ctx = {ns, req, cmd};
	__maybe_unused struct nvme_setup_cmd_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ns_ptr = glue_unpack(__pos, __msg, __ext, struct nvme_ns*);
		if (*ns_ptr) {
			callee_unmarshal_kernel__nvme_setup_cmd__ns__in(__pos, __msg, __ext, ctx, *ns_ptr);
		}

	}

	{
		*req_ptr = glue_unpack(__pos, __msg, __ext, struct request*);
		if (*req_ptr) {
			callee_unmarshal_kernel__nvme_setup_cmd__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	{
		if (*cmd_ptr) {
			callee_unmarshal_kernel__nvme_setup_cmd__cmd__out(__pos, __msg, __ext, ctx, *cmd_ptr);
		}

	}

	ret = nvme_setup_cmd(ns, req, cmd);

	*__pos = 0;
	{
		if (*ns_ptr) {
			callee_marshal_kernel__nvme_setup_cmd__ns__in(__pos, __msg, __ext, ctx, *ns_ptr);
		}

	}

	{
		if (*req_ptr) {
			callee_marshal_kernel__nvme_setup_cmd__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	{
		if (*cmd_ptr) {
			callee_marshal_kernel__nvme_setup_cmd__cmd__out(__pos, __msg, __ext, ctx, *cmd_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void trmp_impl_pci_driver_shutdown(fptr_pci_driver_shutdown target, struct pci_dev* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused const struct pci_driver_shutdown_call_ctx call_ctx = {pdev};
	__maybe_unused const struct pci_driver_shutdown_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_driver_shutdown__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_pci_driver_shutdown);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_driver_shutdown__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_pci_driver_shutdown)
void LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_shutdown) trmp_pci_driver_shutdown(struct pci_dev* pdev)
{
	volatile fptr_impl_pci_driver_shutdown impl;
	fptr_pci_driver_shutdown target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_pci_driver_shutdown);
	impl = trmp_impl_pci_driver_shutdown;
	return impl(target, pdev);
}

void nvme_shutdown_ctrl_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct nvme_shutdown_ctrl_call_ctx call_ctx = {ctrl};
	__maybe_unused struct nvme_shutdown_ctrl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_shutdown_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	ret = nvme_shutdown_ctrl(ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_shutdown_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned int trmp_impl_pci_error_handlers_slot_reset(fptr_pci_error_handlers_slot_reset target, struct pci_dev* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_error_handlers_slot_reset_call_ctx call_ctx = {pdev};
	__maybe_unused const struct pci_error_handlers_slot_reset_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_error_handlers_slot_reset__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_pci_error_handlers_slot_reset);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_error_handlers_slot_reset__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_pci_error_handlers_slot_reset)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_pci_error_handlers_slot_reset) trmp_pci_error_handlers_slot_reset(struct pci_dev* pdev)
{
	volatile fptr_impl_pci_error_handlers_slot_reset impl;
	fptr_pci_error_handlers_slot_reset target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_pci_error_handlers_slot_reset);
	impl = trmp_impl_pci_error_handlers_slot_reset;
	return impl(target, pdev);
}

void nvme_start_queues_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused struct nvme_start_queues_call_ctx call_ctx = {ctrl};
	__maybe_unused struct nvme_start_queues_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_start_queues__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	nvme_start_queues(ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_start_queues__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_stop_queues_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused struct nvme_stop_queues_call_ctx call_ctx = {ctrl};
	__maybe_unused struct nvme_stop_queues_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_stop_queues__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	nvme_stop_queues(ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_stop_queues__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_submit_sync_cmd_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue* q = 0;
	void* buffer = 0;
	unsigned int bufflen = 0;
	struct request_queue** q_ptr = &q;
	struct nvme_command __cmd;
	struct nvme_command* cmd = &__cmd;
	struct nvme_command** cmd_ptr = &cmd;
	unsigned int* bufflen_ptr = &bufflen;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct nvme_submit_sync_cmd_call_ctx call_ctx = {q, cmd, buffer, bufflen};
	__maybe_unused struct nvme_submit_sync_cmd_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__nvme_submit_sync_cmd__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		if (*cmd_ptr) {
			callee_unmarshal_kernel__nvme_submit_sync_cmd__cmd__in(__pos, __msg, __ext, ctx, *cmd_ptr);
		}

	}

	{
		*bufflen_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = nvme_submit_sync_cmd(q, cmd, buffer, bufflen);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__nvme_submit_sync_cmd__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		if (*cmd_ptr) {
			callee_marshal_kernel__nvme_submit_sync_cmd__cmd__in(__pos, __msg, __ext, ctx, *cmd_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned int trmp_impl_blk_mq_ops_timeout(fptr_blk_mq_ops_timeout target, struct request* req, bool reserved)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** req_ptr = &req;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct blk_mq_ops_timeout_call_ctx call_ctx = {req, reserved};
	__maybe_unused const struct blk_mq_ops_timeout_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *req_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*req_ptr) {
			caller_marshal_kernel__blk_mq_ops_timeout__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_blk_mq_ops_timeout);

	*__pos = 0;
	{
		if (*req_ptr) {
			caller_unmarshal_kernel__blk_mq_ops_timeout__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_timeout)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_timeout) trmp_blk_mq_ops_timeout(struct request* req, bool reserved)
{
	volatile fptr_impl_blk_mq_ops_timeout impl;
	fptr_blk_mq_ops_timeout target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_blk_mq_ops_timeout);
	impl = trmp_impl_blk_mq_ops_timeout;
	return impl(target, req, reserved);
}

void nvme_uninit_ctrl_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused struct nvme_uninit_ctrl_call_ctx call_ctx = {ctrl};
	__maybe_unused struct nvme_uninit_ctrl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__nvme_uninit_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	nvme_uninit_ctrl(ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__nvme_uninit_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_device_is_present_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* pdev = 0;
	struct pci_dev** pdev_ptr = &pdev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct pci_device_is_present_call_ctx call_ctx = {pdev};
	__maybe_unused struct pci_device_is_present_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_device_is_present__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	ret = pci_device_is_present(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_device_is_present__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_device_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_disable_device_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_disable_device_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_disable_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	pci_disable_device(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_disable_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_msi_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_disable_msi_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_disable_msi_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_disable_msi__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	pci_disable_msi(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_disable_msi__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_msix_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_disable_msix_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_disable_msix_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_disable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	pci_disable_msix(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_disable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_enable_device_mem_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_enable_device_mem_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_enable_device_mem_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_enable_device_mem__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	ret = pci_enable_device_mem(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_enable_device_mem__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_enable_msi_range_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	int minvec = 0;
	int maxvec = 0;
	struct pci_dev** dev_ptr = &dev;
	int* minvec_ptr = &minvec;
	int* maxvec_ptr = &maxvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_enable_msi_range_call_ctx call_ctx = {dev, minvec, maxvec};
	__maybe_unused struct pci_enable_msi_range_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_enable_msi_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*minvec_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*maxvec_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = pci_enable_msi_range(dev, minvec, maxvec);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_enable_msi_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_enable_msix_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct msix_entry* entries = 0;
	int nvec = 0;
	struct pci_dev** dev_ptr = &dev;
	struct msix_entry** entries_ptr = &entries;
	int* nvec_ptr = &nvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_enable_msix_call_ctx call_ctx = {dev, entries, nvec};
	__maybe_unused struct pci_enable_msix_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_enable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct msix_entry) * glue_peek(__pos, __msg, __ext);
		*entries_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct msix_entry*, (__size), (DEFAULT_GFP_FLAGS));
		if (*entries_ptr) {
			int i;
			struct msix_entry* array = *entries_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct msix_entry* element = &array[i];
				callee_unmarshal_kernel__pci_enable_msix__entries__io(__pos, __msg, __ext, ctx, element);
			}

		}

	}

	{
		*nvec_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = pci_enable_msix(dev, entries, nvec);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_enable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *entries_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*entries_ptr) {
			size_t i, len = (nvec);
			struct msix_entry* array = *entries_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct msix_entry* element = &array[i];
				callee_marshal_kernel__pci_enable_msix__entries__io(__pos, __msg, __ext, ctx, element);
			}

		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_enable_msix_range_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct msix_entry* entries = 0;
	int minvec = 0;
	int maxvec = 0;
	struct pci_dev** dev_ptr = &dev;
	struct msix_entry** entries_ptr = &entries;
	int* minvec_ptr = &minvec;
	int* maxvec_ptr = &maxvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_enable_msix_range_call_ctx call_ctx = {dev, entries, minvec, maxvec};
	__maybe_unused struct pci_enable_msix_range_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_enable_msix_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct msix_entry) * glue_peek(__pos, __msg, __ext);
		*entries_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct msix_entry*, (__size), (DEFAULT_GFP_FLAGS));
		if (*entries_ptr) {
			int i;
			struct msix_entry* array = *entries_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct msix_entry* element = &array[i];
				callee_unmarshal_kernel__pci_enable_msix_range__entries__io(__pos, __msg, __ext, ctx, element);
			}

		}

	}

	{
		*minvec_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*maxvec_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = pci_enable_msix_range(dev, entries, minvec, maxvec);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_enable_msix_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *entries_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*entries_ptr) {
			size_t i, len = (ctx->maxvec);
			struct msix_entry* array = *entries_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct msix_entry* element = &array[i];
				callee_marshal_kernel__pci_enable_msix_range__entries__io(__pos, __msg, __ext, ctx, element);
			}

		}

	}

	{
	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_enable_pcie_error_reporting_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_enable_pcie_error_reporting_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_enable_pcie_error_reporting_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	ret = pci_enable_pcie_error_reporting(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_pcie_error_reporting_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_disable_pcie_error_reporting_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_disable_pcie_error_reporting_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	ret = pci_disable_pcie_error_reporting(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_release_selected_regions_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* pdev = 0;
	int bars = 0;
	struct pci_dev** pdev_ptr = &pdev;
	int* bars_ptr = &bars;
	
	__maybe_unused struct pci_release_selected_regions_call_ctx call_ctx = {pdev, bars};
	__maybe_unused struct pci_release_selected_regions_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_release_selected_regions__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		*bars_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	pci_release_selected_regions(pdev, bars);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_release_selected_regions__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_request_selected_regions_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* pdev = 0;
	int bars = 0;
	char const* res_name = 0;
	struct pci_dev** pdev_ptr = &pdev;
	int* bars_ptr = &bars;
	char const** res_name_ptr = &res_name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_request_selected_regions_call_ctx call_ctx = {pdev, bars, res_name};
	__maybe_unused struct pci_request_selected_regions_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_request_selected_regions__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		*bars_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*res_name_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*res_name_ptr) {
			char* writable = (char*)*res_name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, __msg, __ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	ret = pci_request_selected_regions(pdev, bars, res_name);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_request_selected_regions__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	{
		(void)res_name_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_restore_state_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_restore_state_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_restore_state_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_restore_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	pci_restore_state(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_restore_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_save_state_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_save_state_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_save_state_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_save_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	ret = pci_save_state(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_save_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_select_bars_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	unsigned long flags = 0;
	struct pci_dev** dev_ptr = &dev;
	unsigned long* flags_ptr = &flags;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_select_bars_call_ctx call_ctx = {dev, flags};
	__maybe_unused struct pci_select_bars_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_select_bars__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	ret = pci_select_bars(dev, flags);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_select_bars__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_set_master_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_set_master_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_set_master_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_set_master__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	pci_set_master(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_set_master__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_unregister_driver_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_driver* drv = 0;
	struct pci_driver** drv_ptr = &drv;
	
	__maybe_unused struct pci_unregister_driver_call_ctx call_ctx = {drv};
	__maybe_unused struct pci_unregister_driver_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*drv_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct pci_driver*);
		if (*drv_ptr) {
			callee_unmarshal_kernel__pci_unregister_driver__drv__in(__pos, __msg, __ext, ctx, *drv_ptr);
		}

	}

	pci_unregister_driver(drv);

	*__pos = 0;
	{
		if (*drv_ptr) {
			callee_marshal_kernel__pci_unregister_driver__drv__in(__pos, __msg, __ext, ctx, *drv_ptr);
		}

		glue_remove_shadow(*drv_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void put_device_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device* dev = 0;
	struct device** dev_ptr = &dev;
	
	__maybe_unused struct put_device_call_ctx call_ctx = {dev};
	__maybe_unused struct put_device_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__put_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	put_device(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__put_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void trmp_impl_work_fn(fptr_work_fn target, struct work_struct* work)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct work_struct** work_ptr = &work;
	
	__maybe_unused const struct work_fn_call_ctx call_ctx = {work};
	__maybe_unused const struct work_fn_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *work_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*work_ptr) {
			caller_marshal_kernel__work_fn__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_work_fn);

	*__pos = 0;
	{
		if (*work_ptr) {
			caller_unmarshal_kernel__work_fn__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_work_fn)
void LCD_TRAMPOLINE_LINKAGE(trmp_work_fn) trmp_work_fn(struct work_struct* work)
{
	volatile fptr_impl_work_fn impl;
	fptr_work_fn target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_work_fn);
	impl = trmp_impl_work_fn;
	return impl(target, work);
}

void lvd_queue_work_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct workqueue_struct* wq = 0;
	struct work_struct* work = 0;
	struct workqueue_struct** wq_ptr = &wq;
	struct work_struct** work_ptr = &work;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct lvd_queue_work_call_ctx call_ctx = {wq, work};
	__maybe_unused struct lvd_queue_work_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*wq_ptr = glue_unpack(__pos, __msg, __ext, struct workqueue_struct*);
		if (*wq_ptr) {
			callee_unmarshal_kernel__lvd_queue_work__wq__in(__pos, __msg, __ext, ctx, *wq_ptr);
		}

	}

	{
		*work_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct work_struct*);
		if (*work_ptr) {
			callee_unmarshal_kernel__lvd_queue_work__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	ret = lvd_queue_work(wq, work);

	*__pos = 0;
	{
		if (*wq_ptr) {
			callee_marshal_kernel__lvd_queue_work__wq__in(__pos, __msg, __ext, ctx, *wq_ptr);
		}

	}

	{
		if (*work_ptr) {
			callee_marshal_kernel__lvd_queue_work__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned int trmp_impl_thread_fn(fptr_thread_fn target, int irq, void* id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* irq_ptr = &irq;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct thread_fn_call_ctx call_ctx = {irq, id};
	__maybe_unused const struct thread_fn_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_thread_fn);

	*__pos = 0;
	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_thread_fn)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_thread_fn) trmp_thread_fn(int irq, void* id)
{
	volatile fptr_impl_thread_fn impl;
	fptr_thread_fn target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_thread_fn);
	impl = trmp_impl_thread_fn;
	return impl(target, irq, id);
}

unsigned int trmp_impl_handler(fptr_handler target, int irq, void* id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* irq_ptr = &irq;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct handler_call_ctx call_ctx = {irq, id};
	__maybe_unused const struct handler_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_handler);

	*__pos = 0;
	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_handler)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_handler) trmp_handler(int irq, void* id)
{
	volatile fptr_impl_handler impl;
	fptr_handler target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_handler);
	impl = trmp_impl_handler;
	return impl(target, irq, id);
}

void request_threaded_irq_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int irq = 0;
	fptr_handler handler = 0;
	fptr_thread_fn thread_fn = 0;
	unsigned long irqflags = 0;
	char const* devname = 0;
	void* dev_id = 0;
	unsigned int* irq_ptr = &irq;
	fptr_handler* handler_ptr = &handler;
	unsigned long* irqflags_ptr = &irqflags;
	char const** devname_ptr = &devname;
	void** dev_id_ptr = &dev_id;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct request_threaded_irq_call_ctx call_ctx = {irq, handler, thread_fn, irqflags, devname, dev_id};
	__maybe_unused struct request_threaded_irq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*handler_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, handler);
	}

	{
		*irqflags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*devname_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*devname_ptr) {
			char* writable = (char*)*devname_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, __msg, __ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	{
		size_t __size = sizeof(void*);
		*dev_id_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, void*, (__size), (DEFAULT_GFP_FLAGS));
		if (*dev_id_ptr) {
		}

	}

	ret = request_threaded_irq(irq, handler, thread_fn, irqflags, devname, dev_id);

	*__pos = 0;
	{
	}

	{
	}

	{
	}

	{
		(void)devname_ptr;
	}

	{
		(void)dev_id_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void round_jiffies_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long j = 0;
	unsigned long* j_ptr = &j;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused struct round_jiffies_call_ctx call_ctx = {j};
	__maybe_unused struct round_jiffies_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*j_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	ret = round_jiffies(j);

	*__pos = 0;
	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void wait_for_completion_io_timeout_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct completion* x = 0;
	unsigned long timeout = 0;
	struct completion** x_ptr = &x;
	unsigned long* timeout_ptr = &timeout;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused struct wait_for_completion_io_timeout_call_ctx call_ctx = {x, timeout};
	__maybe_unused struct wait_for_completion_io_timeout_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*x_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct completion*);
		if (*x_ptr) {
			callee_unmarshal_kernel__wait_for_completion_io_timeout__x__io(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	{
		*timeout_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	ret = wait_for_completion_io_timeout(x, timeout);

	*__pos = 0;
	{
		__maybe_unused const void* __adjusted = *x_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*x_ptr) {
			callee_marshal_kernel__wait_for_completion_io_timeout__x__io(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *timeout_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void work_busy_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct work_struct* work = 0;
	struct work_struct** work_ptr = &work;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct work_busy_call_ctx call_ctx = {work};
	__maybe_unused struct work_busy_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*work_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct work_struct*);
		if (*work_ptr) {
			callee_unmarshal_kernel__work_busy__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	ret = work_busy(work);

	*__pos = 0;
	{
		if (*work_ptr) {
			callee_marshal_kernel__work_busy__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_map_queue_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
		*q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_mq_map_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cpu_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = blk_mq_map_queue(q, cpu);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__blk_mq_map_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __global_init_var_jiffies_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused struct __global_init_var_jiffies_call_ctx call_ctx = {};
	__maybe_unused struct __global_init_var_jiffies_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = __global_init_var_jiffies();

	*__pos = 0;
	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __global_init_var_nvme_io_timeout_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned char ret = 0;
	unsigned char* ret_ptr = &ret;
	
	__maybe_unused struct __global_init_var_nvme_io_timeout_call_ctx call_ctx = {};
	__maybe_unused struct __global_init_var_nvme_io_timeout_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = __global_init_var_nvme_io_timeout();

	*__pos = 0;
	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __global_init_var_nvme_max_retries_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct __global_init_var_nvme_max_retries_call_ctx call_ctx = {};
	__maybe_unused struct __global_init_var_nvme_max_retries_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = __global_init_var_nvme_max_retries();

	*__pos = 0;
	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __global_init_var_admin_timeout_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned char ret = 0;
	unsigned char* ret_ptr = &ret;
	
	__maybe_unused struct __global_init_var_admin_timeout_call_ctx call_ctx = {};
	__maybe_unused struct __global_init_var_admin_timeout_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = __global_init_var_admin_timeout();

	*__pos = 0;
	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __global_init_var_system_wq_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct workqueue_struct* ret = 0;
	struct workqueue_struct** ret_ptr = &ret;
	
	__maybe_unused struct __global_init_var_system_wq_call_ctx call_ctx = {};
	__maybe_unused struct __global_init_var_system_wq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = __global_init_var_system_wq();

	*__pos = 0;
	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		(void)ret_ptr;
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned long __global_init_var_jiffies()
{
	return jiffies;
}

unsigned char __global_init_var_nvme_io_timeout()
{
	return nvme_io_timeout;
}

unsigned int __global_init_var_nvme_max_retries()
{
	return nvme_max_retries;
}

unsigned char __global_init_var_admin_timeout()
{
	return admin_timeout;
}

struct workqueue_struct* __global_init_var_system_wq()
{
	return system_wq;
}

int try_dispatch(enum RPC_ID id, struct fipc_message* __msg, struct ext_registers* __ext)
{
	switch(id) {
	case RPC_ID_shared_mem_init:
		glue_user_trace("shared_mem_init\n");
		shared_mem_init_callee(__msg, __ext);
		break;

	case RPC_ID_lvd_alloc_workqueue:
		glue_user_trace("lvd_alloc_workqueue\n");
		lvd_alloc_workqueue_callee(__msg, __ext);
		break;

	case RPC_ID___pci_register_driver:
		glue_user_trace("__pci_register_driver\n");
		__pci_register_driver_callee(__msg, __ext);
		break;

	case RPC_ID_blk_cleanup_queue:
		glue_user_trace("blk_cleanup_queue\n");
		blk_cleanup_queue_callee(__msg, __ext);
		break;

	case RPC_ID_blk_execute_rq_nowait:
		glue_user_trace("blk_execute_rq_nowait\n");
		blk_execute_rq_nowait_callee(__msg, __ext);
		break;

	case RPC_ID_blk_get_queue:
		glue_user_trace("blk_get_queue\n");
		blk_get_queue_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_alloc_tag_set:
		glue_user_trace("blk_mq_alloc_tag_set\n");
		blk_mq_alloc_tag_set_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_complete_request:
		glue_user_trace("blk_mq_complete_request\n");
		blk_mq_complete_request_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_end_request:
		glue_user_trace("blk_mq_end_request\n");
		blk_mq_end_request_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_free_request:
		glue_user_trace("blk_mq_free_request\n");
		blk_mq_free_request_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_free_tag_set:
		glue_user_trace("blk_mq_free_tag_set\n");
		blk_mq_free_tag_set_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_init_queue:
		glue_user_trace("blk_mq_init_queue\n");
		blk_mq_init_queue_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_start_request:
		glue_user_trace("blk_mq_start_request\n");
		blk_mq_start_request_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_start_stopped_hw_queues:
		glue_user_trace("blk_mq_start_stopped_hw_queues\n");
		blk_mq_start_stopped_hw_queues_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_stop_hw_queues:
		glue_user_trace("blk_mq_stop_hw_queues\n");
		blk_mq_stop_hw_queues_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_tagset_busy_iter:
		glue_user_trace("blk_mq_tagset_busy_iter\n");
		blk_mq_tagset_busy_iter_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_update_nr_hw_queues:
		glue_user_trace("blk_mq_update_nr_hw_queues\n");
		blk_mq_update_nr_hw_queues_callee(__msg, __ext);
		break;

	case RPC_ID_blk_put_queue:
		glue_user_trace("blk_put_queue\n");
		blk_put_queue_callee(__msg, __ext);
		break;

	case RPC_ID_complete:
		glue_user_trace("complete\n");
		complete_callee(__msg, __ext);
		break;

	case RPC_ID_del_timer_sync:
		glue_user_trace("del_timer_sync\n");
		del_timer_sync_callee(__msg, __ext);
		break;

	case RPC_ID_destroy_workqueue:
		glue_user_trace("destroy_workqueue\n");
		destroy_workqueue_callee(__msg, __ext);
		break;

	case RPC_ID_device_release_driver:
		glue_user_trace("device_release_driver\n");
		device_release_driver_callee(__msg, __ext);
		break;

	case RPC_ID_flush_work:
		glue_user_trace("flush_work\n");
		flush_work_callee(__msg, __ext);
		break;

	case RPC_ID_free_irq:
		glue_user_trace("free_irq\n");
		free_irq_callee(__msg, __ext);
		break;

	case RPC_ID_get_device:
		glue_user_trace("get_device\n");
		get_device_callee(__msg, __ext);
		break;

	case RPC_ID_lvd_setup_timer:
		glue_user_trace("lvd_setup_timer\n");
		lvd_setup_timer_callee(__msg, __ext);
		break;

	case RPC_ID_lvd_init_work:
		glue_user_trace("lvd_init_work\n");
		lvd_init_work_callee(__msg, __ext);
		break;

	case RPC_ID_ioremap_nocache:
		glue_user_trace("ioremap_nocache\n");
		ioremap_nocache_callee(__msg, __ext);
		break;

	case RPC_ID_irq_set_affinity_hint:
		glue_user_trace("irq_set_affinity_hint\n");
		irq_set_affinity_hint_callee(__msg, __ext);
		break;

	case RPC_ID_mod_timer:
		glue_user_trace("mod_timer\n");
		mod_timer_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_alloc_request:
		glue_user_trace("nvme_alloc_request\n");
		nvme_alloc_request_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_cancel_request:
		glue_user_trace("nvme_cancel_request\n");
		nvme_cancel_request_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_change_ctrl_state:
		glue_user_trace("nvme_change_ctrl_state\n");
		nvme_change_ctrl_state_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_complete_async_event:
		glue_user_trace("nvme_complete_async_event\n");
		nvme_complete_async_event_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_disable_ctrl:
		glue_user_trace("nvme_disable_ctrl\n");
		nvme_disable_ctrl_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_enable_ctrl:
		glue_user_trace("nvme_enable_ctrl\n");
		nvme_enable_ctrl_callee(__msg, __ext);
		break;

	case RPC_ID_pci_cleanup_aer_uncorrect_error_status:
		glue_user_trace("pci_cleanup_aer_uncorrect_error_status\n");
		pci_cleanup_aer_uncorrect_error_status_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_init_ctrl:
		glue_user_trace("nvme_init_ctrl\n");
		nvme_init_ctrl_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_init_identify:
		glue_user_trace("nvme_init_identify\n");
		nvme_init_identify_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_kill_queues:
		glue_user_trace("nvme_kill_queues\n");
		nvme_kill_queues_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_put_ctrl:
		glue_user_trace("nvme_put_ctrl\n");
		nvme_put_ctrl_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_queue_async_events:
		glue_user_trace("nvme_queue_async_events\n");
		nvme_queue_async_events_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_queue_scan:
		glue_user_trace("nvme_queue_scan\n");
		nvme_queue_scan_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_remove_namespaces:
		glue_user_trace("nvme_remove_namespaces\n");
		nvme_remove_namespaces_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_requeue_req:
		glue_user_trace("nvme_requeue_req\n");
		nvme_requeue_req_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_set_queue_count:
		glue_user_trace("nvme_set_queue_count\n");
		nvme_set_queue_count_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_setup_cmd:
		glue_user_trace("nvme_setup_cmd\n");
		nvme_setup_cmd_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_shutdown_ctrl:
		glue_user_trace("nvme_shutdown_ctrl\n");
		nvme_shutdown_ctrl_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_start_queues:
		glue_user_trace("nvme_start_queues\n");
		nvme_start_queues_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_stop_queues:
		glue_user_trace("nvme_stop_queues\n");
		nvme_stop_queues_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_submit_sync_cmd:
		glue_user_trace("nvme_submit_sync_cmd\n");
		nvme_submit_sync_cmd_callee(__msg, __ext);
		break;

	case RPC_ID_nvme_uninit_ctrl:
		glue_user_trace("nvme_uninit_ctrl\n");
		nvme_uninit_ctrl_callee(__msg, __ext);
		break;

	case RPC_ID_pci_device_is_present:
		glue_user_trace("pci_device_is_present\n");
		pci_device_is_present_callee(__msg, __ext);
		break;

	case RPC_ID_pci_disable_device:
		glue_user_trace("pci_disable_device\n");
		pci_disable_device_callee(__msg, __ext);
		break;

	case RPC_ID_pci_disable_msi:
		glue_user_trace("pci_disable_msi\n");
		pci_disable_msi_callee(__msg, __ext);
		break;

	case RPC_ID_pci_disable_msix:
		glue_user_trace("pci_disable_msix\n");
		pci_disable_msix_callee(__msg, __ext);
		break;

	case RPC_ID_pci_enable_device_mem:
		glue_user_trace("pci_enable_device_mem\n");
		pci_enable_device_mem_callee(__msg, __ext);
		break;

	case RPC_ID_pci_enable_msi_range:
		glue_user_trace("pci_enable_msi_range\n");
		pci_enable_msi_range_callee(__msg, __ext);
		break;

	case RPC_ID_pci_enable_msix:
		glue_user_trace("pci_enable_msix\n");
		pci_enable_msix_callee(__msg, __ext);
		break;

	case RPC_ID_pci_enable_msix_range:
		glue_user_trace("pci_enable_msix_range\n");
		pci_enable_msix_range_callee(__msg, __ext);
		break;

	case RPC_ID_pci_enable_pcie_error_reporting:
		glue_user_trace("pci_enable_pcie_error_reporting\n");
		pci_enable_pcie_error_reporting_callee(__msg, __ext);
		break;

	case RPC_ID_pci_disable_pcie_error_reporting:
		glue_user_trace("pci_disable_pcie_error_reporting\n");
		pci_disable_pcie_error_reporting_callee(__msg, __ext);
		break;

	case RPC_ID_pci_release_selected_regions:
		glue_user_trace("pci_release_selected_regions\n");
		pci_release_selected_regions_callee(__msg, __ext);
		break;

	case RPC_ID_pci_request_selected_regions:
		glue_user_trace("pci_request_selected_regions\n");
		pci_request_selected_regions_callee(__msg, __ext);
		break;

	case RPC_ID_pci_restore_state:
		glue_user_trace("pci_restore_state\n");
		pci_restore_state_callee(__msg, __ext);
		break;

	case RPC_ID_pci_save_state:
		glue_user_trace("pci_save_state\n");
		pci_save_state_callee(__msg, __ext);
		break;

	case RPC_ID_pci_select_bars:
		glue_user_trace("pci_select_bars\n");
		pci_select_bars_callee(__msg, __ext);
		break;

	case RPC_ID_pci_set_master:
		glue_user_trace("pci_set_master\n");
		pci_set_master_callee(__msg, __ext);
		break;

	case RPC_ID_pci_unregister_driver:
		glue_user_trace("pci_unregister_driver\n");
		pci_unregister_driver_callee(__msg, __ext);
		break;

	case RPC_ID_put_device:
		glue_user_trace("put_device\n");
		put_device_callee(__msg, __ext);
		break;

	case RPC_ID_lvd_queue_work:
		glue_user_trace("lvd_queue_work\n");
		lvd_queue_work_callee(__msg, __ext);
		break;

	case RPC_ID_request_threaded_irq:
		glue_user_trace("request_threaded_irq\n");
		request_threaded_irq_callee(__msg, __ext);
		break;

	case RPC_ID_round_jiffies:
		glue_user_trace("round_jiffies\n");
		round_jiffies_callee(__msg, __ext);
		break;

	case RPC_ID_wait_for_completion_io_timeout:
		glue_user_trace("wait_for_completion_io_timeout\n");
		wait_for_completion_io_timeout_callee(__msg, __ext);
		break;

	case RPC_ID_work_busy:
		glue_user_trace("work_busy\n");
		work_busy_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_map_queue:
		glue_user_trace("blk_mq_map_queue\n");
		blk_mq_map_queue_callee(__msg, __ext);
		break;

	case RPC_ID___global_init_var_jiffies:
		glue_user_trace("__global_init_var_jiffies\n");
		__global_init_var_jiffies_callee(__msg, __ext);
		break;

	case RPC_ID___global_init_var_nvme_io_timeout:
		glue_user_trace("__global_init_var_nvme_io_timeout\n");
		__global_init_var_nvme_io_timeout_callee(__msg, __ext);
		break;

	case RPC_ID___global_init_var_nvme_max_retries:
		glue_user_trace("__global_init_var_nvme_max_retries\n");
		__global_init_var_nvme_max_retries_callee(__msg, __ext);
		break;

	case RPC_ID___global_init_var_admin_timeout:
		glue_user_trace("__global_init_var_admin_timeout\n");
		__global_init_var_admin_timeout_callee(__msg, __ext);
		break;

	case RPC_ID___global_init_var_system_wq:
		glue_user_trace("__global_init_var_system_wq\n");
		__global_init_var_system_wq_callee(__msg, __ext);
		break;

	default:
		return 0;
	}

	return 1;
}

