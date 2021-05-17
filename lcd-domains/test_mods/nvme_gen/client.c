#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

volatile unsigned long jiffies;

unsigned char nvme_io_timeout;

unsigned int nvme_max_retries;

unsigned char admin_timeout;

struct workqueue_struct* system_wq;

struct workqueue_struct* lvd_alloc_workqueue(char const* fmt, unsigned int flags, int max_active)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	char const** fmt_ptr = &fmt;
	unsigned int* flags_ptr = &flags;
	int* max_active_ptr = &max_active;
	struct workqueue_struct* ret = 0;
	struct workqueue_struct** ret_ptr = &ret;
	
	__maybe_unused const struct lvd_alloc_workqueue_call_ctx call_ctx = {fmt, flags, max_active};
	__maybe_unused const struct lvd_alloc_workqueue_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *fmt_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*fmt_ptr) {
			size_t i, len;
			char const* array = *fmt_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *max_active_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_lvd_alloc_workqueue);

	*__pos = 0;
	{
		(void)fmt_ptr;
	}

	{
	}

	{
	}

	{
		size_t __size = sizeof(void *);
		*ret_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct workqueue_struct*, (__size), ((DEFAULT_GFP_FLAGS)));
		if (*ret_ptr) {
			caller_unmarshal_kernel__lvd_alloc_workqueue__ret_workqueue_struct__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int __pci_register_driver(struct pci_driver* drv, struct module* owner, char const* mod_name)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_driver** drv_ptr = &drv;
	struct module** owner_ptr = &owner;
	char const** mod_name_ptr = &mod_name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct __pci_register_driver_call_ctx call_ctx = {drv, owner, mod_name};
	__maybe_unused const struct __pci_register_driver_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *drv_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*drv_ptr) {
			caller_marshal_kernel___global_pci_driver__in(__pos, __msg, __ext, *drv_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *owner_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*owner_ptr) {
			caller_marshal_kernel____pci_register_driver__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *mod_name_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*mod_name_ptr) {
			size_t i, len;
			char const* array = *mod_name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	glue_call_server(__pos, __msg, RPC_ID___pci_register_driver);

	*__pos = 0;
	{
		if (*drv_ptr) {
			caller_unmarshal_kernel___global_pci_driver__in(__pos, __msg, __ext, *drv_ptr);
		}

	}

	{
		if (*owner_ptr) {
			caller_unmarshal_kernel____pci_register_driver__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

	{
		(void)mod_name_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void blk_cleanup_queue(struct request_queue* q)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** q_ptr = &q;
	
	__maybe_unused const struct blk_cleanup_queue_call_ctx call_ctx = {q};
	__maybe_unused const struct blk_cleanup_queue_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_cleanup_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_blk_cleanup_queue);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_cleanup_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void done_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_done function_ptr = glue_unpack(__pos, __msg, __ext, fptr_done);
	struct request* rq = 0;
	int id = 0;
	struct request** rq_ptr = &rq;
	int* id_ptr = &id;
	
	__maybe_unused struct done_call_ctx call_ctx = {rq, id};
	__maybe_unused struct done_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*rq_ptr = glue_unpack(__pos, __msg, __ext, struct request*);
		if (*rq_ptr) {
			callee_unmarshal_kernel__done__rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
		*id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	function_ptr(rq, id);

	*__pos = 0;
	{
		if (*rq_ptr) {
			callee_marshal_kernel__done__rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_execute_rq_nowait(struct request_queue* q, struct gendisk* bd_disk, struct request* rq, int at_head, fptr_done done)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** q_ptr = &q;
	struct request** rq_ptr = &rq;
	int* at_head_ptr = &at_head;
	fptr_done* done_ptr = &done;
	
	__maybe_unused const struct blk_execute_rq_nowait_call_ctx call_ctx = {q, bd_disk, rq, at_head, done};
	__maybe_unused const struct blk_execute_rq_nowait_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_execute_rq_nowait__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__blk_execute_rq_nowait__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *at_head_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *done_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_blk_execute_rq_nowait);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_execute_rq_nowait__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*rq_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request*);
		if (*rq_ptr) {
			caller_unmarshal_kernel__blk_execute_rq_nowait__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

bool blk_get_queue(struct request_queue* q)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** q_ptr = &q;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct blk_get_queue_call_ctx call_ctx = {q};
	__maybe_unused const struct blk_get_queue_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_get_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_blk_get_queue);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_get_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int blk_mq_alloc_tag_set(struct blk_mq_tag_set* set)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_tag_set** set_ptr = &set;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct blk_mq_alloc_tag_set_call_ctx call_ctx = {set};
	__maybe_unused const struct blk_mq_alloc_tag_set_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *set_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*set_ptr) {
			caller_marshal_kernel__blk_mq_alloc_tag_set__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_blk_mq_alloc_tag_set);

	*__pos = 0;
	{
		if (*set_ptr) {
			caller_unmarshal_kernel__blk_mq_alloc_tag_set__set__in(__pos, __msg, __ext, ctx, *set_ptr);
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

void blk_mq_complete_request(struct request* rq, int error)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** rq_ptr = &rq;
	int* error_ptr = &error;
	
	__maybe_unused const struct blk_mq_complete_request_call_ctx call_ctx = {rq, error};
	__maybe_unused const struct blk_mq_complete_request_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__blk_mq_complete_request__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *error_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_blk_mq_complete_request);

	*__pos = 0;
	{
		*rq_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request*);
		if (*rq_ptr) {
			caller_unmarshal_kernel__blk_mq_complete_request__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_end_request(struct request* rq, int error)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** rq_ptr = &rq;
	int* error_ptr = &error;
	
	__maybe_unused const struct blk_mq_end_request_call_ctx call_ctx = {rq, error};
	__maybe_unused const struct blk_mq_end_request_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__blk_mq_end_request__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *error_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_blk_mq_end_request);

	*__pos = 0;
	{
		*rq_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request*);
		if (*rq_ptr) {
			caller_unmarshal_kernel__blk_mq_end_request__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_free_request(struct request* rq)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** rq_ptr = &rq;
	
	__maybe_unused const struct blk_mq_free_request_call_ctx call_ctx = {rq};
	__maybe_unused const struct blk_mq_free_request_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__blk_mq_free_request__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_blk_mq_free_request);

	*__pos = 0;
	{
		*rq_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request*);
		if (*rq_ptr) {
			caller_unmarshal_kernel__blk_mq_free_request__rq__io(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_free_tag_set(struct blk_mq_tag_set* set)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_tag_set** set_ptr = &set;
	
	__maybe_unused const struct blk_mq_free_tag_set_call_ctx call_ctx = {set};
	__maybe_unused const struct blk_mq_free_tag_set_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *set_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*set_ptr) {
			caller_marshal_kernel__blk_mq_free_tag_set__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_blk_mq_free_tag_set);

	*__pos = 0;
	{
		if (*set_ptr) {
			caller_unmarshal_kernel__blk_mq_free_tag_set__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct request_queue* blk_mq_init_queue(struct blk_mq_tag_set* set)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_tag_set** set_ptr = &set;
	struct request_queue* ret = 0;
	struct request_queue** ret_ptr = &ret;
	
	__maybe_unused const struct blk_mq_init_queue_call_ctx call_ctx = {set};
	__maybe_unused const struct blk_mq_init_queue_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *set_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*set_ptr) {
			caller_marshal_kernel__blk_mq_init_queue__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_blk_mq_init_queue);

	*__pos = 0;
	{
		if (*set_ptr) {
			caller_unmarshal_kernel__blk_mq_init_queue__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	{
		size_t __size = sizeof(struct request_queue);
		*ret_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct request_queue*, (__size), (DEFAULT_GFP_FLAGS));
		if (*ret_ptr) {
			caller_unmarshal_kernel__blk_mq_init_queue__ret_request_queue__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void blk_mq_ops_map_queue_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_blk_mq_ops_map_queue function_ptr = glue_unpack(__pos, __msg, __ext, fptr_blk_mq_ops_map_queue);
	struct request_queue* q = 0;
	int cpu = 0;
	struct request_queue** q_ptr = &q;
	int* cpu_ptr = &cpu;
	struct blk_mq_hw_ctx* ret = 0;
	struct blk_mq_hw_ctx** ret_ptr = &ret;
	
	__maybe_unused struct blk_mq_ops_map_queue_call_ctx call_ctx = {q, cpu};
	__maybe_unused struct blk_mq_ops_map_queue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct request_queue);
		*q_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct request_queue*, __size);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_mq_ops_map_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cpu_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = function_ptr(q, cpu);

	*__pos = 0;
	{
		if (*q_ptr) {
			callee_marshal_kernel__blk_mq_ops_map_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__blk_mq_ops_map_queue__ret_blk_mq_hw_ctx__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_start_request(struct request* rq)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** rq_ptr = &rq;
	
	__maybe_unused const struct blk_mq_start_request_call_ctx call_ctx = {rq};
	__maybe_unused const struct blk_mq_start_request_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *rq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__blk_mq_start_request__rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_blk_mq_start_request);

	*__pos = 0;
	{
		if (*rq_ptr) {
			caller_unmarshal_kernel__blk_mq_start_request__rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_start_stopped_hw_queues(struct request_queue* q, bool async)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** q_ptr = &q;
	bool* async_ptr = &async;
	
	__maybe_unused const struct blk_mq_start_stopped_hw_queues_call_ctx call_ctx = {q, async};
	__maybe_unused const struct blk_mq_start_stopped_hw_queues_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_mq_start_stopped_hw_queues__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *async_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_blk_mq_start_stopped_hw_queues);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_mq_start_stopped_hw_queues__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_stop_hw_queues(struct request_queue* q)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** q_ptr = &q;
	
	__maybe_unused const struct blk_mq_stop_hw_queues_call_ctx call_ctx = {q};
	__maybe_unused const struct blk_mq_stop_hw_queues_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_mq_stop_hw_queues__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_blk_mq_stop_hw_queues);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_mq_stop_hw_queues__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void tag_iter_fn_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_tag_iter_fn function_ptr = glue_unpack(__pos, __msg, __ext, fptr_tag_iter_fn);
	struct request* rq = 0;
	void* data = 0;
	bool enable = 0;
	struct request** rq_ptr = &rq;
	void** data_ptr = &data;
	bool* enable_ptr = &enable;
	
	__maybe_unused struct tag_iter_fn_call_ctx call_ctx = {rq, data, enable};
	__maybe_unused struct tag_iter_fn_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*rq_ptr = glue_unpack(__pos, __msg, __ext, struct request*);
		if (*rq_ptr) {
			callee_unmarshal_kernel__tag_iter_fn__rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*data_ptr) {
		}

	}

	{
		*enable_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	function_ptr(rq, data, enable);

	*__pos = 0;
	{
		if (*rq_ptr) {
			callee_marshal_kernel__tag_iter_fn__rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
		(void)data_ptr;
	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_tagset_busy_iter(struct blk_mq_tag_set* tagset, fptr_tag_iter_fn fn, void* priv)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_tag_set** tagset_ptr = &tagset;
	fptr_tag_iter_fn* fn_ptr = &fn;
	void** priv_ptr = &priv;
	
	__maybe_unused const struct blk_mq_tagset_busy_iter_call_ctx call_ctx = {tagset, fn, priv};
	__maybe_unused const struct blk_mq_tagset_busy_iter_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *tagset_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*tagset_ptr) {
			caller_marshal_kernel__blk_mq_tagset_busy_iter__tagset__in(__pos, __msg, __ext, ctx, *tagset_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *fn_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *priv_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*priv_ptr) {
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_blk_mq_tagset_busy_iter);

	*__pos = 0;
	{
		if (*tagset_ptr) {
			caller_unmarshal_kernel__blk_mq_tagset_busy_iter__tagset__in(__pos, __msg, __ext, ctx, *tagset_ptr);
		}

	}

	{
	}

	{
		(void)priv_ptr;
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_update_nr_hw_queues(struct blk_mq_tag_set* set, int nr_hw_queues)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct blk_mq_tag_set** set_ptr = &set;
	int* nr_hw_queues_ptr = &nr_hw_queues;
	
	__maybe_unused const struct blk_mq_update_nr_hw_queues_call_ctx call_ctx = {set, nr_hw_queues};
	__maybe_unused const struct blk_mq_update_nr_hw_queues_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *set_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*set_ptr) {
			caller_marshal_kernel__blk_mq_update_nr_hw_queues__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *nr_hw_queues_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_blk_mq_update_nr_hw_queues);

	*__pos = 0;
	{
		if (*set_ptr) {
			caller_unmarshal_kernel__blk_mq_update_nr_hw_queues__set__in(__pos, __msg, __ext, ctx, *set_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_put_queue(struct request_queue* q)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** q_ptr = &q;
	
	__maybe_unused const struct blk_put_queue_call_ctx call_ctx = {q};
	__maybe_unused const struct blk_put_queue_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_put_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_blk_put_queue);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_put_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void complete(struct completion* x)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct completion** x_ptr = &x;
	
	__maybe_unused const struct complete_call_ctx call_ctx = {x};
	__maybe_unused const struct complete_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *x_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*x_ptr) {
			caller_marshal_kernel__complete__x__in(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_complete);

	*__pos = 0;
	{
		if (*x_ptr) {
			caller_unmarshal_kernel__complete__x__in(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int del_timer_sync(struct timer_list* timer)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list** timer_ptr = &timer;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct del_timer_sync_call_ctx call_ctx = {timer};
	__maybe_unused const struct del_timer_sync_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			caller_marshal_kernel__del_timer_sync__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_del_timer_sync);

	*__pos = 0;
	{
		if (*timer_ptr) {
			caller_unmarshal_kernel__del_timer_sync__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
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

void destroy_workqueue(struct workqueue_struct* wq)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct workqueue_struct** wq_ptr = &wq;
	
	__maybe_unused const struct destroy_workqueue_call_ctx call_ctx = {wq};
	__maybe_unused const struct destroy_workqueue_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *wq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*wq_ptr) {
			caller_marshal_kernel__destroy_workqueue__wq__in(__pos, __msg, __ext, ctx, *wq_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_destroy_workqueue);

	*__pos = 0;
	{
		if (*wq_ptr) {
			caller_unmarshal_kernel__destroy_workqueue__wq__in(__pos, __msg, __ext, ctx, *wq_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void device_release_driver(struct device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device** dev_ptr = &dev;
	
	__maybe_unused const struct device_release_driver_call_ctx call_ctx = {dev};
	__maybe_unused const struct device_release_driver_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct pci_dev* __adjusted = container_of(*dev_ptr, struct pci_dev, dev);
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__device_release_driver__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_device_release_driver);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__device_release_driver__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

bool flush_work(struct work_struct* work)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct work_struct** work_ptr = &work;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct flush_work_call_ctx call_ctx = {work};
	__maybe_unused const struct flush_work_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *work_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*work_ptr) {
			caller_marshal_kernel__flush_work__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_flush_work);

	*__pos = 0;
	{
		if (*work_ptr) {
			caller_unmarshal_kernel__flush_work__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void free_irq(unsigned int irq, void* dev_id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* irq_ptr = &irq;
	void** dev_id_ptr = &dev_id;
	
	__maybe_unused const struct free_irq_call_ctx call_ctx = {irq, dev_id};
	__maybe_unused const struct free_irq_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *dev_id_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_id_ptr) {
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_free_irq);

	*__pos = 0;
	{
	}

	{
		(void)dev_id_ptr;
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void timer_func_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_timer_func function_ptr = glue_unpack(__pos, __msg, __ext, fptr_timer_func);
	unsigned long data = 0;
	unsigned long* data_ptr = &data;
	
	__maybe_unused struct timer_func_call_ctx call_ctx = {data};
	__maybe_unused struct timer_func_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	function_ptr(data);

	*__pos = 0;
	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct device* get_device(struct device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device** dev_ptr = &dev;
	struct device* ret = dev;
	//struct device** ret_ptr = &ret;
	
	__maybe_unused const struct get_device_call_ctx call_ctx = {dev};
	__maybe_unused const struct get_device_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct pci_dev* __adjusted = container_of(*dev_ptr, struct pci_dev, dev);
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__get_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_get_device);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__get_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void lvd_setup_timer(struct timer_list* timer, fptr_timer_func func, unsigned long data)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list** timer_ptr = &timer;
	fptr_timer_func* func_ptr = &func;
	unsigned long* data_ptr = &data;
	
	__maybe_unused const struct lvd_setup_timer_call_ctx call_ctx = {timer, func, data};
	__maybe_unused const struct lvd_setup_timer_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			caller_marshal_kernel__lvd_setup_timer__timer__io(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *func_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_lvd_setup_timer);

	*__pos = 0;
	{
		*timer_ptr = glue_unpack(__pos, __msg, __ext, struct timer_list*);
		if (*timer_ptr) {
			caller_unmarshal_kernel__lvd_setup_timer__timer__io(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_init_work(struct work_struct* work, fptr_work_fn work_fn)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct work_struct** work_ptr = &work;
	fptr_work_fn* work_fn_ptr = &work_fn;
	
	__maybe_unused const struct lvd_init_work_call_ctx call_ctx = {work, work_fn};
	__maybe_unused const struct lvd_init_work_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *work_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*work_ptr) {
			caller_marshal_kernel__lvd_init_work__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *work_fn_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_lvd_init_work);

	*__pos = 0;
	{
		if (*work_ptr) {
			caller_unmarshal_kernel__lvd_init_work__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void* ioremap_nocache(unsigned long long phys_addr, unsigned long size)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long long* phys_addr_ptr = &phys_addr;
	unsigned long* size_ptr = &size;
	cptr_t ioremap_cptr;
	gpa_t ioremap_gpa;
	uint64_t ioremap_len;
	void* ret = 0;
	void** ret_ptr = &ret;
	
	__maybe_unused const struct ioremap_nocache_call_ctx call_ctx = {phys_addr, size};
	__maybe_unused const struct ioremap_nocache_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		lcd_cptr_alloc(&ioremap_cptr);
		glue_pack(__pos, __msg, __ext, cptr_val(ioremap_cptr));
	}

	{
		glue_pack(__pos, __msg, __ext, *phys_addr_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *size_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_ioremap_nocache);

	*__pos = 0;
	{
		ioremap_len = glue_unpack(__pos, __msg, __ext, uint64_t);
		lcd_ioremap_phys(ioremap_cptr, ioremap_len, &ioremap_gpa);
		*ret_ptr = lcd_ioremap(gpa_val(ioremap_gpa), ioremap_len);
	}

	{
	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int irq_set_affinity_hint(unsigned int irq, struct cpumask const* m)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* irq_ptr = &irq;
	struct cpumask const** m_ptr = &m;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct irq_set_affinity_hint_call_ctx call_ctx = {irq, m};
	__maybe_unused const struct irq_set_affinity_hint_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *m_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*m_ptr) {
			caller_marshal_kernel__irq_set_affinity_hint__m__in(__pos, __msg, __ext, ctx, *m_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_irq_set_affinity_hint);

	*__pos = 0;
	{
	}

	{
		if (*m_ptr) {
			struct cpumask* writable = (struct cpumask*)*m_ptr;
			caller_unmarshal_kernel__irq_set_affinity_hint__m__in(__pos, __msg, __ext, ctx, writable);
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

int mod_timer(struct timer_list* timer, unsigned long expires)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list** timer_ptr = &timer;
	unsigned long* expires_ptr = &expires;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mod_timer_call_ctx call_ctx = {timer, expires};
	__maybe_unused const struct mod_timer_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			caller_marshal_kernel__mod_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *expires_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_mod_timer);

	*__pos = 0;
	{
		if (*timer_ptr) {
			caller_unmarshal_kernel__mod_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
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

void blk_mq_ops_exit_hctx_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_blk_mq_ops_exit_hctx function_ptr = glue_unpack(__pos, __msg, __ext, fptr_blk_mq_ops_exit_hctx);
	struct blk_mq_hw_ctx* hctx = 0;
	unsigned int hctx_idx = 0;
	struct blk_mq_hw_ctx** hctx_ptr = &hctx;
	
	__maybe_unused struct blk_mq_ops_exit_hctx_call_ctx call_ctx = {hctx, hctx_idx};
	__maybe_unused struct blk_mq_ops_exit_hctx_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hctx_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct blk_mq_hw_ctx*);
		if (*hctx_ptr) {
			callee_unmarshal_kernel__blk_mq_ops_exit_hctx__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	function_ptr(hctx, hctx_idx);

	*__pos = 0;
	{
		if (*hctx_ptr) {
			callee_marshal_kernel__blk_mq_ops_exit_hctx__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_ops_init_hctx_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_blk_mq_ops_init_hctx function_ptr = glue_unpack(__pos, __msg, __ext, fptr_blk_mq_ops_init_hctx);
	struct blk_mq_hw_ctx* hctx = 0;
	void* data = 0;
	unsigned int hctx_idx = 0;
	struct blk_mq_hw_ctx** hctx_ptr = &hctx;
	void** data_ptr = &data;
	unsigned int* hctx_idx_ptr = &hctx_idx;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct blk_mq_ops_init_hctx_call_ctx call_ctx = {hctx, data, hctx_idx};
	__maybe_unused struct blk_mq_ops_init_hctx_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		//printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct blk_mq_hw_ctx);
		*hctx_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct blk_mq_hw_ctx*, (__size), (DEFAULT_GFP_FLAGS));
		if (*hctx_ptr) {
			callee_unmarshal_kernel__blk_mq_ops_init_hctx__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*data_ptr) {
		}

	}

	{
		*hctx_idx_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = function_ptr(hctx, data, hctx_idx);

	*__pos = 0;
	{
		if (*hctx_ptr) {
			callee_marshal_kernel__blk_mq_ops_init_hctx__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	{
		(void)data_ptr;
	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		//printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_ops_init_request_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_blk_mq_ops_init_request function_ptr = glue_unpack(__pos, __msg, __ext, fptr_blk_mq_ops_init_request);
	void* data = 0;
	struct request* req = 0;
	unsigned int hctx_idx = 0;
	unsigned int rq_idx = 0;
	unsigned int numa_node = 0;
	void** data_ptr = &data;
	struct request** req_ptr = &req;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct blk_mq_ops_init_request_call_ctx call_ctx = {data, req, hctx_idx, rq_idx, numa_node};
	__maybe_unused struct blk_mq_ops_init_request_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		//printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*data_ptr) {
		}

	}

	{
		size_t __size = sizeof(struct request);
		*req_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct request*, (__size), (DEFAULT_GFP_FLAGS));
		if (*req_ptr) {
			callee_unmarshal_kernel__blk_mq_ops_init_request__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	ret = function_ptr(data, req, hctx_idx, rq_idx, numa_node);

	*__pos = 0;
	{
		(void)data_ptr;
	}

	{
		if (*req_ptr) {
			callee_marshal_kernel__blk_mq_ops_init_request__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		//printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct request* nvme_alloc_request(struct request_queue* q, struct nvme_command* cmd, unsigned int flags, int qid)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** q_ptr = &q;
	struct nvme_command** cmd_ptr = &cmd;
	unsigned int* flags_ptr = &flags;
	int* qid_ptr = &qid;
	struct request* ret = 0;
	struct request** ret_ptr = &ret;
	
	__maybe_unused const struct nvme_alloc_request_call_ctx call_ctx = {q, cmd, flags, qid};
	__maybe_unused const struct nvme_alloc_request_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__nvme_alloc_request__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *cmd_ptr;
		if (*cmd_ptr) {
			caller_marshal_kernel__nvme_alloc_request__cmd__io(__pos, __msg, __ext, ctx, *cmd_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *qid_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_alloc_request);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__nvme_alloc_request__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		if (*cmd_ptr) {
			caller_unmarshal_kernel__nvme_alloc_request__cmd__io(__pos, __msg, __ext, ctx, *cmd_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__nvme_alloc_request__ret_request__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void nvme_cancel_request(struct request* req, void* data, bool reserved)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** req_ptr = &req;
	
	__maybe_unused const struct nvme_cancel_request_call_ctx call_ctx = {req, data, reserved};
	__maybe_unused const struct nvme_cancel_request_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *req_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*req_ptr) {
			caller_marshal_kernel__nvme_cancel_request__req__io(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_cancel_request);

	*__pos = 0;
	{
		*req_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request*);
		if (*req_ptr) {
			caller_unmarshal_kernel__nvme_cancel_request__req__io(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

bool nvme_change_ctrl_state(struct nvme_ctrl* ctrl, unsigned int new_state)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	unsigned int* new_state_ptr = &new_state;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct nvme_change_ctrl_state_call_ctx call_ctx = {ctrl, new_state};
	__maybe_unused const struct nvme_change_ctrl_state_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_change_ctrl_state__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *new_state_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_change_ctrl_state);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_change_ctrl_state__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void nvme_complete_async_event(struct nvme_ctrl* ctrl, struct nvme_completion* cqe)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	struct nvme_completion** cqe_ptr = &cqe;
	
	__maybe_unused const struct nvme_complete_async_event_call_ctx call_ctx = {ctrl, cqe};
	__maybe_unused const struct nvme_complete_async_event_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_complete_async_event__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *cqe_ptr;
		if (*cqe_ptr) {
			caller_marshal_kernel__nvme_complete_async_event__cqe__in(__pos, __msg, __ext, ctx, *cqe_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_complete_async_event);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_complete_async_event__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		if (*cqe_ptr) {
			caller_unmarshal_kernel__nvme_complete_async_event__cqe__in(__pos, __msg, __ext, ctx, *cqe_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_ops_complete_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_blk_mq_ops_complete function_ptr = glue_unpack(__pos, __msg, __ext, fptr_blk_mq_ops_complete);
	struct request* req = 0;
	struct request** req_ptr = &req;
	
	__maybe_unused struct blk_mq_ops_complete_call_ctx call_ctx = {req};
	__maybe_unused struct blk_mq_ops_complete_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*req_ptr = glue_unpack(__pos, __msg, __ext, struct request*);
		if (*req_ptr) {
			callee_unmarshal_kernel__blk_mq_ops_complete__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	function_ptr(req);

	*__pos = 0;
	{
		if (*req_ptr) {
			callee_marshal_kernel__blk_mq_ops_complete__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int nvme_disable_ctrl(struct nvme_ctrl* ctrl, unsigned long long cap)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	unsigned long long* cap_ptr = &cap;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct nvme_disable_ctrl_call_ctx call_ctx = {ctrl, cap};
	__maybe_unused const struct nvme_disable_ctrl_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_disable_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cap_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_disable_ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_disable_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
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

int nvme_enable_ctrl(struct nvme_ctrl* ctrl, unsigned long long cap)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	unsigned long long* cap_ptr = &cap;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct nvme_enable_ctrl_call_ctx call_ctx = {ctrl, cap};
	__maybe_unused const struct nvme_enable_ctrl_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_enable_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cap_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_enable_ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_enable_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
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

int pci_cleanup_aer_uncorrect_error_status(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_cleanup_aer_uncorrect_error_status_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_cleanup_aer_uncorrect_error_status_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_cleanup_aer_uncorrect_error_status__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_cleanup_aer_uncorrect_error_status);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_cleanup_aer_uncorrect_error_status__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

void pci_error_handlers_error_detected_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_pci_error_handlers_error_detected function_ptr = glue_unpack(__pos, __msg, __ext, fptr_pci_error_handlers_error_detected);
	struct pci_dev* pdev = 0;
	unsigned int state = 0;
	struct pci_dev** pdev_ptr = &pdev;
	unsigned int* state_ptr = &state;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct pci_error_handlers_error_detected_call_ctx call_ctx = {pdev, state};
	__maybe_unused struct pci_error_handlers_error_detected_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_error_handlers_error_detected__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = function_ptr(pdev, state);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_error_handlers_error_detected__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
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

void pci_error_handlers_resume_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_pci_error_handlers_resume function_ptr = glue_unpack(__pos, __msg, __ext, fptr_pci_error_handlers_resume);
	struct pci_dev* pdev = 0;
	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused struct pci_error_handlers_resume_call_ctx call_ctx = {pdev};
	__maybe_unused struct pci_error_handlers_resume_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_error_handlers_resume__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	function_ptr(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_error_handlers_resume__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void reg_read32_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_reg_read32 function_ptr = glue_unpack(__pos, __msg, __ext, fptr_reg_read32);
	struct nvme_ctrl* ctrl = 0;
	unsigned int off = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	unsigned int* off_ptr = &off;
	unsigned int __val;
	unsigned int* val = &__val;
	unsigned int** val_ptr = &val;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct reg_read32_call_ctx call_ctx = {ctrl, off, val};
	__maybe_unused struct reg_read32_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__reg_read32__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		*off_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		(void)val_ptr;
	}

	ret = function_ptr(ctrl, off, val);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__reg_read32__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
	}

	{
		if (*val_ptr) {
			glue_pack(__pos, __msg, __ext, **val_ptr);
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

void reg_write32_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_reg_write32 function_ptr = glue_unpack(__pos, __msg, __ext, fptr_reg_write32);
	struct nvme_ctrl* ctrl = 0;
	unsigned int off = 0;
	unsigned int val = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	unsigned int* off_ptr = &off;
	unsigned int* val_ptr = &val;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct reg_write32_call_ctx call_ctx = {ctrl, off, val};
	__maybe_unused struct reg_write32_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__reg_write32__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		*off_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*val_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = function_ptr(ctrl, off, val);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__reg_write32__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
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

void reg_read64_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_reg_read64 function_ptr = glue_unpack(__pos, __msg, __ext, fptr_reg_read64);
	struct nvme_ctrl* ctrl = 0;
	unsigned int off = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	unsigned int* off_ptr = &off;
	unsigned long long __val;
	unsigned long long* val = &__val;
	unsigned long long** val_ptr = &val;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct reg_read64_call_ctx call_ctx = {ctrl, off, val};
	__maybe_unused struct reg_read64_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__reg_read64__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		*off_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		(void)val_ptr;
	}

	ret = function_ptr(ctrl, off, val);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__reg_read64__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
	}

	{
		if (*val_ptr) {
			glue_pack(__pos, __msg, __ext, **val_ptr);
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

void reset_ctrl_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_reset_ctrl function_ptr = glue_unpack(__pos, __msg, __ext, fptr_reset_ctrl);
	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct reset_ctrl_call_ctx call_ctx = {ctrl};
	__maybe_unused struct reset_ctrl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__reset_ctrl__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	ret = function_ptr(ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__reset_ctrl__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
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

void free_ctrl_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_free_ctrl function_ptr = glue_unpack(__pos, __msg, __ext, fptr_free_ctrl);
	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused struct free_ctrl_call_ctx call_ctx = {ctrl};
	__maybe_unused struct free_ctrl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__free_ctrl__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	function_ptr(ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__free_ctrl__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void post_scan_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_post_scan function_ptr = glue_unpack(__pos, __msg, __ext, fptr_post_scan);
	struct nvme_ctrl* ctrl = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused struct post_scan_call_ctx call_ctx = {ctrl};
	__maybe_unused struct post_scan_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__post_scan__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	function_ptr(ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__post_scan__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void submit_async_event_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_submit_async_event function_ptr = glue_unpack(__pos, __msg, __ext, fptr_submit_async_event);
	struct nvme_ctrl* ctrl = 0;
	int aer_idx = 0;
	struct nvme_ctrl** ctrl_ptr = &ctrl;
	int* aer_idx_ptr = &aer_idx;
	
	__maybe_unused struct submit_async_event_call_ctx call_ctx = {ctrl, aer_idx};
	__maybe_unused struct submit_async_event_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ctrl_ptr = glue_unpack(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			callee_unmarshal_kernel__submit_async_event__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		*aer_idx_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	function_ptr(ctrl, aer_idx);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			callee_marshal_kernel__submit_async_event__nvme_ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int nvme_init_ctrl(struct nvme_ctrl* ctrl, struct device* dev, struct nvme_ctrl_ops const* ops, unsigned long quirks)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	struct device** dev_ptr = &dev;
	struct nvme_ctrl_ops const** ops_ptr = &ops;
	unsigned long* quirks_ptr = &quirks;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct nvme_init_ctrl_call_ctx call_ctx = {ctrl, dev, ops, quirks};
	__maybe_unused const struct nvme_init_ctrl_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_init_ctrl__ctrl__io(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		struct pci_dev* __adjusted = container_of(*dev_ptr, struct pci_dev, dev);
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__nvme_init_ctrl__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ops_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ops_ptr) {
			caller_marshal_kernel__nvme_init_ctrl__ops__in(__pos, __msg, __ext, ctx, *ops_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *quirks_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_init_ctrl);

	*__pos = 0;
	{
		*ctrl_ptr = glue_unpack(__pos, __msg, __ext, struct nvme_ctrl*);
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_init_ctrl__ctrl__io(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__nvme_init_ctrl__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*ops_ptr) {
			struct nvme_ctrl_ops* writable = (struct nvme_ctrl_ops*)*ops_ptr;
			caller_unmarshal_kernel__nvme_init_ctrl__ops__in(__pos, __msg, __ext, ctx, writable);
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

int nvme_init_identify(struct nvme_ctrl* ctrl)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct nvme_init_identify_call_ctx call_ctx = {ctrl};
	__maybe_unused const struct nvme_init_identify_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_init_identify__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_init_identify);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_init_identify__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
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

void nvme_kill_queues(struct nvme_ctrl* ctrl)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused const struct nvme_kill_queues_call_ctx call_ctx = {ctrl};
	__maybe_unused const struct nvme_kill_queues_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_kill_queues__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_kill_queues);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_kill_queues__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_driver_sriov_configure_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_pci_driver_sriov_configure function_ptr = glue_unpack(__pos, __msg, __ext, fptr_pci_driver_sriov_configure);
	struct pci_dev* pdev = 0;
	int numvfs = 0;
	struct pci_dev** pdev_ptr = &pdev;
	int* numvfs_ptr = &numvfs;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_driver_sriov_configure_call_ctx call_ctx = {pdev, numvfs};
	__maybe_unused struct pci_driver_sriov_configure_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_driver_sriov_configure__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		*numvfs_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = function_ptr(pdev, numvfs);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_driver_sriov_configure__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
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

void blk_mq_ops_poll_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_blk_mq_ops_poll function_ptr = glue_unpack(__pos, __msg, __ext, fptr_blk_mq_ops_poll);
	struct blk_mq_hw_ctx* hctx = 0;
	unsigned int tag = 0;
	struct blk_mq_hw_ctx** hctx_ptr = &hctx;
	unsigned int* tag_ptr = &tag;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct blk_mq_ops_poll_call_ctx call_ctx = {hctx, tag};
	__maybe_unused struct blk_mq_ops_poll_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hctx_ptr = glue_unpack(__pos, __msg, __ext, struct blk_mq_hw_ctx*);
		if (*hctx_ptr) {
			callee_unmarshal_kernel__blk_mq_ops_poll__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	{
		*tag_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = function_ptr(hctx, tag);

	*__pos = 0;
	{
		if (*hctx_ptr) {
			callee_marshal_kernel__blk_mq_ops_poll__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
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

void pci_driver_probe_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_pci_driver_probe function_ptr = glue_unpack(__pos, __msg, __ext, fptr_pci_driver_probe);
	struct pci_dev* pdev = 0;
	struct pci_device_id* ent = 0;
	struct pci_dev** pdev_ptr = &pdev;
	struct pci_device_id** ent_ptr = &ent;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_driver_probe_call_ctx call_ctx = {pdev, ent};
	__maybe_unused struct pci_driver_probe_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct pci_dev);
		*pdev_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct pci_dev*, (__size), (DEFAULT_GFP_FLAGS));
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_driver_probe__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct pci_device_id);
		*ent_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct pci_device_id*, (__size), (DEFAULT_GFP_FLAGS));
		if (*ent_ptr) {
			callee_unmarshal_kernel__pci_driver_probe__ent__in(__pos, __msg, __ext, ctx, *ent_ptr);
		}

	}

	ret = function_ptr(pdev, ent);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_driver_probe__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		if (*ent_ptr) {
			callee_marshal_kernel__pci_driver_probe__ent__in(__pos, __msg, __ext, ctx, *ent_ptr);
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

void nvme_put_ctrl(struct nvme_ctrl* ctrl)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused const struct nvme_put_ctrl_call_ctx call_ctx = {ctrl};
	__maybe_unused const struct nvme_put_ctrl_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_put_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_put_ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_put_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_queue_async_events(struct nvme_ctrl* ctrl)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused const struct nvme_queue_async_events_call_ctx call_ctx = {ctrl};
	__maybe_unused const struct nvme_queue_async_events_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_queue_async_events__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_queue_async_events);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_queue_async_events__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void blk_mq_ops_queue_rq_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_blk_mq_ops_queue_rq function_ptr = glue_unpack(__pos, __msg, __ext, fptr_blk_mq_ops_queue_rq);
	struct blk_mq_hw_ctx* hctx = 0;
	struct blk_mq_hw_ctx** hctx_ptr = &hctx;
	struct blk_mq_queue_data __bd;
	struct blk_mq_queue_data* bd = &__bd;
	struct blk_mq_queue_data** bd_ptr = &bd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct blk_mq_ops_queue_rq_call_ctx call_ctx = {hctx, bd};
	__maybe_unused struct blk_mq_ops_queue_rq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hctx_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct blk_mq_hw_ctx*);
		if (*hctx_ptr) {
			callee_unmarshal_kernel__blk_mq_ops_queue_rq__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	{
		if (*bd_ptr) {
			callee_unmarshal_kernel__blk_mq_ops_queue_rq__bd__in(__pos, __msg, __ext, ctx, *bd_ptr);
		}

	}

	ret = function_ptr(hctx, bd);

	*__pos = 0;
	{
		if (*hctx_ptr) {
			callee_marshal_kernel__blk_mq_ops_queue_rq__hctx__in(__pos, __msg, __ext, ctx, *hctx_ptr);
		}

	}

	{
		if (*bd_ptr) {
			callee_marshal_kernel__blk_mq_ops_queue_rq__bd__in(__pos, __msg, __ext, ctx, *bd_ptr);
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

void nvme_queue_scan(struct nvme_ctrl* ctrl)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused const struct nvme_queue_scan_call_ctx call_ctx = {ctrl};
	__maybe_unused const struct nvme_queue_scan_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_queue_scan__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_queue_scan);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_queue_scan__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_driver_remove_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_pci_driver_remove function_ptr = glue_unpack(__pos, __msg, __ext, fptr_pci_driver_remove);
	struct pci_dev* pdev = 0;
	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused struct pci_driver_remove_call_ctx call_ctx = {pdev};
	__maybe_unused struct pci_driver_remove_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_driver_remove__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	function_ptr(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_driver_remove__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_remove_namespaces(struct nvme_ctrl* ctrl)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused const struct nvme_remove_namespaces_call_ctx call_ctx = {ctrl};
	__maybe_unused const struct nvme_remove_namespaces_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_remove_namespaces__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_remove_namespaces);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_remove_namespaces__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_requeue_req(struct request* req)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request** req_ptr = &req;
	
	__maybe_unused const struct nvme_requeue_req_call_ctx call_ctx = {req};
	__maybe_unused const struct nvme_requeue_req_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *req_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*req_ptr) {
			caller_marshal_kernel__nvme_requeue_req__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_requeue_req);

	*__pos = 0;
	{
		if (*req_ptr) {
			caller_unmarshal_kernel__nvme_requeue_req__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_error_handlers_reset_notify_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_pci_error_handlers_reset_notify function_ptr = glue_unpack(__pos, __msg, __ext, fptr_pci_error_handlers_reset_notify);
	struct pci_dev* pdev = 0;
	bool prepare = 0;
	struct pci_dev** pdev_ptr = &pdev;
	bool* prepare_ptr = &prepare;
	
	__maybe_unused struct pci_error_handlers_reset_notify_call_ctx call_ctx = {pdev, prepare};
	__maybe_unused struct pci_error_handlers_reset_notify_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_error_handlers_reset_notify__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		*prepare_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	function_ptr(pdev, prepare);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_error_handlers_reset_notify__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int nvme_set_queue_count(struct nvme_ctrl* ctrl, int* count)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	int** count_ptr = &count;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct nvme_set_queue_count_call_ctx call_ctx = {ctrl, count};
	__maybe_unused const struct nvme_set_queue_count_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_set_queue_count__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *count_ptr;
		if (*count_ptr) {
			glue_pack(__pos, __msg, __ext, **count_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_set_queue_count);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_set_queue_count__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	{
		if (*count_ptr) {
			**count_ptr = glue_unpack(__pos, __msg, __ext, int);
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

int nvme_setup_cmd(struct nvme_ns* ns, struct request* req, struct nvme_command* cmd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ns** ns_ptr = &ns;
	struct request** req_ptr = &req;
	struct nvme_command** cmd_ptr = &cmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct nvme_setup_cmd_call_ctx call_ctx = {ns, req, cmd};
	__maybe_unused const struct nvme_setup_cmd_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ns_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ns_ptr) {
			caller_marshal_kernel__nvme_setup_cmd__ns__in(__pos, __msg, __ext, ctx, *ns_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *req_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*req_ptr) {
			caller_marshal_kernel__nvme_setup_cmd__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *cmd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*cmd_ptr) {
			caller_marshal_kernel__nvme_setup_cmd__cmd__io(__pos, __msg, __ext, ctx, *cmd_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_setup_cmd);

	*__pos = 0;
	{
		if (*ns_ptr) {
			caller_unmarshal_kernel__nvme_setup_cmd__ns__in(__pos, __msg, __ext, ctx, *ns_ptr);
		}

	}

	{
		if (*req_ptr) {
			caller_unmarshal_kernel__nvme_setup_cmd__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	{
		*cmd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct nvme_command*);
		if (*cmd_ptr) {
			caller_unmarshal_kernel__nvme_setup_cmd__cmd__io(__pos, __msg, __ext, ctx, *cmd_ptr);
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

void pci_driver_shutdown_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_pci_driver_shutdown function_ptr = glue_unpack(__pos, __msg, __ext, fptr_pci_driver_shutdown);
	struct pci_dev* pdev = 0;
	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused struct pci_driver_shutdown_call_ctx call_ctx = {pdev};
	__maybe_unused struct pci_driver_shutdown_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_driver_shutdown__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	function_ptr(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_driver_shutdown__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int nvme_shutdown_ctrl(struct nvme_ctrl* ctrl)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct nvme_shutdown_ctrl_call_ctx call_ctx = {ctrl};
	__maybe_unused const struct nvme_shutdown_ctrl_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_shutdown_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_shutdown_ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_shutdown_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
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

void pci_error_handlers_slot_reset_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_pci_error_handlers_slot_reset function_ptr = glue_unpack(__pos, __msg, __ext, fptr_pci_error_handlers_slot_reset);
	struct pci_dev* pdev = 0;
	struct pci_dev** pdev_ptr = &pdev;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct pci_error_handlers_slot_reset_call_ctx call_ctx = {pdev};
	__maybe_unused struct pci_error_handlers_slot_reset_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_error_handlers_slot_reset__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	ret = function_ptr(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_error_handlers_slot_reset__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
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

void nvme_start_queues(struct nvme_ctrl* ctrl)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused const struct nvme_start_queues_call_ctx call_ctx = {ctrl};
	__maybe_unused const struct nvme_start_queues_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_start_queues__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_start_queues);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_start_queues__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void nvme_stop_queues(struct nvme_ctrl* ctrl)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused const struct nvme_stop_queues_call_ctx call_ctx = {ctrl};
	__maybe_unused const struct nvme_stop_queues_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_stop_queues__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_stop_queues);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_stop_queues__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int nvme_submit_sync_cmd(struct request_queue* q, struct nvme_command* cmd, void* buffer, unsigned int bufflen)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct request_queue** q_ptr = &q;
	struct nvme_command** cmd_ptr = &cmd;
	unsigned int* bufflen_ptr = &bufflen;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct nvme_submit_sync_cmd_call_ctx call_ctx = {q, cmd, buffer, bufflen};
	__maybe_unused const struct nvme_submit_sync_cmd_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__nvme_submit_sync_cmd__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *cmd_ptr;
		if (*cmd_ptr) {
			caller_marshal_kernel__nvme_submit_sync_cmd__cmd__in(__pos, __msg, __ext, ctx, *cmd_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *bufflen_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_submit_sync_cmd);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__nvme_submit_sync_cmd__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		if (*cmd_ptr) {
			caller_unmarshal_kernel__nvme_submit_sync_cmd__cmd__in(__pos, __msg, __ext, ctx, *cmd_ptr);
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

void blk_mq_ops_timeout_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_blk_mq_ops_timeout function_ptr = glue_unpack(__pos, __msg, __ext, fptr_blk_mq_ops_timeout);
	struct request* req = 0;
	bool reserved = 0;
	struct request** req_ptr = &req;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct blk_mq_ops_timeout_call_ctx call_ctx = {req, reserved};
	__maybe_unused struct blk_mq_ops_timeout_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*req_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request*);
		if (*req_ptr) {
			callee_unmarshal_kernel__blk_mq_ops_timeout__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	ret = function_ptr(req, reserved);

	*__pos = 0;
	{
		if (*req_ptr) {
			callee_marshal_kernel__blk_mq_ops_timeout__req__in(__pos, __msg, __ext, ctx, *req_ptr);
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

void nvme_uninit_ctrl(struct nvme_ctrl* ctrl)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nvme_ctrl** ctrl_ptr = &ctrl;
	
	__maybe_unused const struct nvme_uninit_ctrl_call_ctx call_ctx = {ctrl};
	__maybe_unused const struct nvme_uninit_ctrl_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ctrl_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ctrl_ptr) {
			caller_marshal_kernel__nvme_uninit_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_nvme_uninit_ctrl);

	*__pos = 0;
	{
		if (*ctrl_ptr) {
			caller_unmarshal_kernel__nvme_uninit_ctrl__ctrl__in(__pos, __msg, __ext, ctx, *ctrl_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

bool pci_device_is_present(struct pci_dev* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct pci_device_is_present_call_ctx call_ctx = {pdev};
	__maybe_unused const struct pci_device_is_present_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_device_is_present__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_device_is_present);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_device_is_present__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void pci_disable_device(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_disable_device_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_disable_device_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_disable_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_disable_device);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_disable_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_msi(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_disable_msi_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_disable_msi_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_disable_msi__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_disable_msi);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_disable_msi__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_msix(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_disable_msix_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_disable_msix_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_disable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_disable_msix);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_disable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int pci_enable_device_mem(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_device_mem_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_enable_device_mem_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_device_mem__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_enable_device_mem);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_device_mem__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

int pci_enable_msi_range(struct pci_dev* dev, int minvec, int maxvec)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int* minvec_ptr = &minvec;
	int* maxvec_ptr = &maxvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_msi_range_call_ctx call_ctx = {dev, minvec, maxvec};
	__maybe_unused const struct pci_enable_msi_range_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_msi_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *minvec_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *maxvec_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_enable_msi_range);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_msi_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

int pci_enable_msix(struct pci_dev* dev, struct msix_entry* entries, int nvec)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	struct msix_entry** entries_ptr = &entries;
	int* nvec_ptr = &nvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_msix_call_ctx call_ctx = {dev, entries, nvec};
	__maybe_unused const struct pci_enable_msix_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *entries_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*entries_ptr) {
			size_t i, len = (nvec);
			struct msix_entry* array = *entries_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct msix_entry* element = &array[i];
				caller_marshal_kernel__pci_enable_msix__entries__io(__pos, __msg, __ext, ctx, element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *nvec_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_enable_msix);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*entries_ptr = glue_unpack(__pos, __msg, __ext, struct msix_entry*);
		if (*entries_ptr) {
			int i;
			struct msix_entry* array = *entries_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct msix_entry* element = &array[i];
				caller_unmarshal_kernel__pci_enable_msix__entries__io(__pos, __msg, __ext, ctx, element);
			}

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

int pci_enable_msix_range(struct pci_dev* dev, struct msix_entry* entries, int minvec, int maxvec)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	struct msix_entry** entries_ptr = &entries;
	int* minvec_ptr = &minvec;
	int* maxvec_ptr = &maxvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_msix_range_call_ctx call_ctx = {dev, entries, minvec, maxvec};
	__maybe_unused const struct pci_enable_msix_range_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_msix_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *entries_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*entries_ptr) {
			size_t i, len = (ctx->maxvec);
			struct msix_entry* array = *entries_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct msix_entry* element = &array[i];
				caller_marshal_kernel__pci_enable_msix_range__entries__io(__pos, __msg, __ext, ctx, element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *minvec_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *maxvec_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_enable_msix_range);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_msix_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*entries_ptr = glue_unpack(__pos, __msg, __ext, struct msix_entry*);
		if (*entries_ptr) {
			int i;
			struct msix_entry* array = *entries_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct msix_entry* element = &array[i];
				caller_unmarshal_kernel__pci_enable_msix_range__entries__io(__pos, __msg, __ext, ctx, element);
			}

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

int pci_enable_pcie_error_reporting(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_pcie_error_reporting_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_enable_pcie_error_reporting_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_enable_pcie_error_reporting);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

int pci_disable_pcie_error_reporting(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_disable_pcie_error_reporting_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_disable_pcie_error_reporting_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_disable_pcie_error_reporting);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

void pci_release_selected_regions(struct pci_dev* pdev, int bars)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	int* bars_ptr = &bars;
	
	__maybe_unused const struct pci_release_selected_regions_call_ctx call_ctx = {pdev, bars};
	__maybe_unused const struct pci_release_selected_regions_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_release_selected_regions__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *bars_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_release_selected_regions);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_release_selected_regions__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int pci_request_selected_regions(struct pci_dev* pdev, int bars, char const* res_name)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	int* bars_ptr = &bars;
	char const** res_name_ptr = &res_name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_request_selected_regions_call_ctx call_ctx = {pdev, bars, res_name};
	__maybe_unused const struct pci_request_selected_regions_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_request_selected_regions__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *bars_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *res_name_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*res_name_ptr) {
			size_t i, len;
			char const* array = *res_name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_request_selected_regions);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_request_selected_regions__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	{
		(void)res_name_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void pci_restore_state(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_restore_state_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_restore_state_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_restore_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_restore_state);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_restore_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int pci_save_state(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_save_state_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_save_state_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_save_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_save_state);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_save_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

int pci_select_bars(struct pci_dev* dev, unsigned long flags)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	unsigned long* flags_ptr = &flags;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_select_bars_call_ctx call_ctx = {dev, flags};
	__maybe_unused const struct pci_select_bars_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_select_bars__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_select_bars);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_select_bars__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

void pci_set_master(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_set_master_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_set_master_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_set_master__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_set_master);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_set_master__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_unregister_driver(struct pci_driver* drv)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_driver** drv_ptr = &drv;
	
	__maybe_unused const struct pci_unregister_driver_call_ctx call_ctx = {drv};
	__maybe_unused const struct pci_unregister_driver_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *drv_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*drv_ptr) {
			caller_marshal_kernel__pci_unregister_driver__drv__in(__pos, __msg, __ext, ctx, *drv_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_unregister_driver);

	*__pos = 0;
	{
		if (*drv_ptr) {
			caller_unmarshal_kernel__pci_unregister_driver__drv__in(__pos, __msg, __ext, ctx, *drv_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void put_device(struct device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device** dev_ptr = &dev;
	
	__maybe_unused const struct put_device_call_ctx call_ctx = {dev};
	__maybe_unused const struct put_device_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__put_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_put_device);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__put_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void work_fn_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_work_fn function_ptr = glue_unpack(__pos, __msg, __ext, fptr_work_fn);
	struct work_struct* work = 0;
	struct work_struct** work_ptr = &work;
	
	__maybe_unused struct work_fn_call_ctx call_ctx = {work};
	__maybe_unused struct work_fn_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*work_ptr = glue_unpack(__pos, __msg, __ext, struct work_struct*);
		if (*work_ptr) {
			callee_unmarshal_kernel__work_fn__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	function_ptr(work);

	*__pos = 0;
	{
		if (*work_ptr) {
			callee_marshal_kernel__work_fn__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

bool lvd_queue_work(struct workqueue_struct* wq, struct work_struct* work)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct workqueue_struct** wq_ptr = &wq;
	struct work_struct** work_ptr = &work;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct lvd_queue_work_call_ctx call_ctx = {wq, work};
	__maybe_unused const struct lvd_queue_work_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *wq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*wq_ptr) {
			caller_marshal_kernel__lvd_queue_work__wq__in(__pos, __msg, __ext, ctx, *wq_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *work_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*work_ptr) {
			caller_marshal_kernel__lvd_queue_work__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_lvd_queue_work);

	*__pos = 0;
	{
		if (*wq_ptr) {
			caller_unmarshal_kernel__lvd_queue_work__wq__in(__pos, __msg, __ext, ctx, *wq_ptr);
		}

	}

	{
		if (*work_ptr) {
			caller_unmarshal_kernel__lvd_queue_work__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void thread_fn_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_thread_fn function_ptr = glue_unpack(__pos, __msg, __ext, fptr_thread_fn);
	int irq = 0;
	void* id = 0;
	int* irq_ptr = &irq;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct thread_fn_call_ctx call_ctx = {irq, id};
	__maybe_unused struct thread_fn_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = function_ptr(irq, id);

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

void handler_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_handler function_ptr = glue_unpack(__pos, __msg, __ext, fptr_handler);
	int irq = 0;
	void* id = 0;
	int* irq_ptr = &irq;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct handler_call_ctx call_ctx = {irq, id};
	__maybe_unused struct handler_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = function_ptr(irq, id);

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

int request_threaded_irq(unsigned int irq, fptr_handler handler, fptr_thread_fn thread_fn, unsigned long irqflags, char const* devname, void* dev_id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* irq_ptr = &irq;
	fptr_handler* handler_ptr = &handler;
	unsigned long* irqflags_ptr = &irqflags;
	char const** devname_ptr = &devname;
	void** dev_id_ptr = &dev_id;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct request_threaded_irq_call_ctx call_ctx = {irq, handler, thread_fn, irqflags, devname, dev_id};
	__maybe_unused const struct request_threaded_irq_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *handler_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *irqflags_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *devname_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*devname_ptr) {
			size_t i, len;
			char const* array = *devname_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		__maybe_unused const void* __adjusted = *dev_id_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_id_ptr) {
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_request_threaded_irq);

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
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned long round_jiffies(unsigned long j)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long* j_ptr = &j;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct round_jiffies_call_ctx call_ctx = {j};
	__maybe_unused const struct round_jiffies_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *j_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_round_jiffies);

	*__pos = 0;
	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned long wait_for_completion_io_timeout(struct completion* x, unsigned long timeout)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct completion** x_ptr = &x;
	unsigned long* timeout_ptr = &timeout;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct wait_for_completion_io_timeout_call_ctx call_ctx = {x, timeout};
	__maybe_unused const struct wait_for_completion_io_timeout_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *x_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*x_ptr) {
			caller_marshal_kernel__wait_for_completion_io_timeout__x__io(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *timeout_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_wait_for_completion_io_timeout);

	*__pos = 0;
	{
		*x_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct completion*);
		if (*x_ptr) {
			caller_unmarshal_kernel__wait_for_completion_io_timeout__x__io(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	{
		*timeout_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned int work_busy(struct work_struct* work)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct work_struct** work_ptr = &work;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct work_busy_call_ctx call_ctx = {work};
	__maybe_unused const struct work_busy_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *work_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*work_ptr) {
			caller_marshal_kernel__work_busy__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_work_busy);

	*__pos = 0;
	{
		if (*work_ptr) {
			caller_unmarshal_kernel__work_busy__work__in(__pos, __msg, __ext, ctx, *work_ptr);
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

struct blk_mq_hw_ctx* blk_mq_map_queue(struct request_queue* q, int cpu)
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
	
	__maybe_unused const struct blk_mq_map_queue_call_ctx call_ctx = {q, cpu};
	__maybe_unused const struct blk_mq_map_queue_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_mq_map_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cpu_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_blk_mq_map_queue);

	*__pos = 0;
	{
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_mq_map_queue__q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct blk_mq_hw_ctx*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned long __global_init_var_jiffies(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct __global_init_var_jiffies_call_ctx call_ctx = {};
	__maybe_unused const struct __global_init_var_jiffies_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID___global_init_var_jiffies);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned char __global_init_var_nvme_io_timeout(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned char ret = 0;
	unsigned char* ret_ptr = &ret;
	
	__maybe_unused const struct __global_init_var_nvme_io_timeout_call_ctx call_ctx = {};
	__maybe_unused const struct __global_init_var_nvme_io_timeout_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID___global_init_var_nvme_io_timeout);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned int __global_init_var_nvme_max_retries(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct __global_init_var_nvme_max_retries_call_ctx call_ctx = {};
	__maybe_unused const struct __global_init_var_nvme_max_retries_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID___global_init_var_nvme_max_retries);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned char __global_init_var_admin_timeout(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned char ret = 0;
	unsigned char* ret_ptr = &ret;
	
	__maybe_unused const struct __global_init_var_admin_timeout_call_ctx call_ctx = {};
	__maybe_unused const struct __global_init_var_admin_timeout_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID___global_init_var_admin_timeout);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct workqueue_struct* __global_init_var_system_wq(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct workqueue_struct* ret = 0;
	struct workqueue_struct** ret_ptr = &ret;
	
	__maybe_unused const struct __global_init_var_system_wq_call_ctx call_ctx = {};
	__maybe_unused const struct __global_init_var_system_wq_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID___global_init_var_system_wq);

	*__pos = 0;
	{
		size_t __size = 64;
		*ret_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct workqueue_struct*, (__size), (DEFAULT_GFP_FLAGS));
		(void)ret_ptr;
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int try_dispatch(enum RPC_ID id, struct fipc_message* __msg, struct ext_registers* __ext)
{
	switch(id) {
	case MODULE_INIT:
		glue_user_trace("MODULE_INIT");
		jiffies = __global_init_var_jiffies();
		nvme_io_timeout = __global_init_var_nvme_io_timeout();
		nvme_max_retries = __global_init_var_nvme_max_retries();
		admin_timeout = __global_init_var_admin_timeout();
		__module_lcd_init();
		shared_mem_init();
		break;

	case MODULE_EXIT:
		glue_user_trace("MODULE_EXIT");
		__module_lcd_exit();
		break;

	case RPC_ID_done:
		glue_user_trace("done\n");
		done_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_ops_map_queue:
		glue_user_trace("blk_mq_ops_map_queue\n");
		blk_mq_ops_map_queue_callee(__msg, __ext);
		break;

	case RPC_ID_tag_iter_fn:
		glue_user_trace("tag_iter_fn\n");
		tag_iter_fn_callee(__msg, __ext);
		break;

	case RPC_ID_timer_func:
		glue_user_trace("timer_func\n");
		timer_func_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_ops_exit_hctx:
		glue_user_trace("blk_mq_ops_exit_hctx\n");
		blk_mq_ops_exit_hctx_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_ops_init_hctx:
		//glue_user_trace("blk_mq_ops_init_hctx\n");
		blk_mq_ops_init_hctx_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_ops_init_request:
		//glue_user_trace("blk_mq_ops_init_request\n");
		blk_mq_ops_init_request_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_ops_complete:
		glue_user_trace("blk_mq_ops_complete\n");
		blk_mq_ops_complete_callee(__msg, __ext);
		break;

	case RPC_ID_pci_error_handlers_error_detected:
		glue_user_trace("pci_error_handlers_error_detected\n");
		pci_error_handlers_error_detected_callee(__msg, __ext);
		break;

	case RPC_ID_pci_error_handlers_resume:
		glue_user_trace("pci_error_handlers_resume\n");
		pci_error_handlers_resume_callee(__msg, __ext);
		break;

	case RPC_ID_reg_read32:
		glue_user_trace("reg_read32\n");
		reg_read32_callee(__msg, __ext);
		break;

	case RPC_ID_reg_write32:
		glue_user_trace("reg_write32\n");
		reg_write32_callee(__msg, __ext);
		break;

	case RPC_ID_reg_read64:
		glue_user_trace("reg_read64\n");
		reg_read64_callee(__msg, __ext);
		break;

	case RPC_ID_reset_ctrl:
		glue_user_trace("reset_ctrl\n");
		reset_ctrl_callee(__msg, __ext);
		break;

	case RPC_ID_free_ctrl:
		glue_user_trace("free_ctrl\n");
		free_ctrl_callee(__msg, __ext);
		break;

	case RPC_ID_post_scan:
		glue_user_trace("post_scan\n");
		post_scan_callee(__msg, __ext);
		break;

	case RPC_ID_submit_async_event:
		glue_user_trace("submit_async_event\n");
		submit_async_event_callee(__msg, __ext);
		break;

	case RPC_ID_pci_driver_sriov_configure:
		glue_user_trace("pci_driver_sriov_configure\n");
		pci_driver_sriov_configure_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_ops_poll:
		glue_user_trace("blk_mq_ops_poll\n");
		blk_mq_ops_poll_callee(__msg, __ext);
		break;

	case RPC_ID_pci_driver_probe:
		glue_user_trace("pci_driver_probe\n");
		pci_driver_probe_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_ops_queue_rq:
		glue_user_trace("blk_mq_ops_queue_rq\n");
		blk_mq_ops_queue_rq_callee(__msg, __ext);
		break;

	case RPC_ID_pci_driver_remove:
		glue_user_trace("pci_driver_remove\n");
		pci_driver_remove_callee(__msg, __ext);
		break;

	case RPC_ID_pci_error_handlers_reset_notify:
		glue_user_trace("pci_error_handlers_reset_notify\n");
		pci_error_handlers_reset_notify_callee(__msg, __ext);
		break;

	case RPC_ID_pci_driver_shutdown:
		glue_user_trace("pci_driver_shutdown\n");
		pci_driver_shutdown_callee(__msg, __ext);
		break;

	case RPC_ID_pci_error_handlers_slot_reset:
		glue_user_trace("pci_error_handlers_slot_reset\n");
		pci_error_handlers_slot_reset_callee(__msg, __ext);
		break;

	case RPC_ID_blk_mq_ops_timeout:
		glue_user_trace("blk_mq_ops_timeout\n");
		blk_mq_ops_timeout_callee(__msg, __ext);
		break;

	case RPC_ID_work_fn:
		glue_user_trace("work_fn\n");
		work_fn_callee(__msg, __ext);
		break;

	case RPC_ID_thread_fn:
		glue_user_trace("thread_fn\n");
		thread_fn_callee(__msg, __ext);
		break;

	case RPC_ID_handler:
		glue_user_trace("handler\n");
		handler_callee(__msg, __ext);
		break;

	default:
		return 0;
	}

	return 1;
}

