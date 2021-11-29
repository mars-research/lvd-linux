#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

int trmp_impl_ctr(fptr_ctr target, struct dm_target* ti, unsigned int argc, char** argv)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct dm_target** ti_ptr = &ti;
	unsigned int* argc_ptr = &argc;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ctr_call_ctx call_ctx = {ti, argc, argv};
	__maybe_unused const struct ctr_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *ti_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ti_ptr) {
			caller_marshal_kernel__ctr__ti__in(__pos, msg, ext, ctx, *ti_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *argc_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_ctr);

	*__pos = 0;
	{
		if (*ti_ptr) {
			caller_unmarshal_kernel__ctr__ti__in(__pos, msg, ext, ctx, *ti_ptr);
		}

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

LCD_TRAMPOLINE_DATA(trmp_ctr)
int LCD_TRAMPOLINE_LINKAGE(trmp_ctr) trmp_ctr(struct dm_target* ti, unsigned int argc, char** argv)
{
	volatile fptr_impl_ctr impl;
	fptr_ctr target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ctr);
	impl = trmp_impl_ctr;
	return impl(target, ti, argc, argv);
}

int trmp_impl_map(fptr_map target, struct dm_target* ti, struct bio* bio)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct bio** bio_ptr = &bio;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct map_call_ctx call_ctx = {ti, bio};
	__maybe_unused const struct map_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *bio_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*bio_ptr) {
			caller_marshal_kernel__map__bio__in(__pos, msg, ext, ctx, *bio_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_map);

	*__pos = 0;
	{
		if (*bio_ptr) {
			caller_unmarshal_kernel__map__bio__in(__pos, msg, ext, ctx, *bio_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_map)
int LCD_TRAMPOLINE_LINKAGE(trmp_map) trmp_map(struct dm_target* ti, struct bio* bio)
{
	volatile fptr_impl_map impl;
	fptr_map target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_map);
	impl = trmp_impl_map;
	return impl(target, ti, bio);
}

void zero_fill_bio_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct bio* bio = 0;
	struct bio** bio_ptr = &bio;
	
	__maybe_unused struct zero_fill_bio_call_ctx call_ctx = {bio};
	__maybe_unused struct zero_fill_bio_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*bio_ptr = glue_unpack(__pos, msg, ext, struct bio*);
		if (*bio_ptr) {
			callee_unmarshal_kernel__zero_fill_bio__bio__in(__pos, msg, ext, ctx, *bio_ptr);
		}

	}

	zero_fill_bio(bio);

	*__pos = 0;
	{
		if (*bio_ptr) {
			callee_marshal_kernel__zero_fill_bio__bio__in(__pos, msg, ext, ctx, *bio_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void bio_endio_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct bio* bio = 0;
	struct bio** bio_ptr = &bio;
	
	__maybe_unused struct bio_endio_call_ctx call_ctx = {bio};
	__maybe_unused struct bio_endio_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*bio_ptr = glue_unpack(__pos, msg, ext, struct bio*);
		if (*bio_ptr) {
			callee_unmarshal_kernel__bio_endio__bio__in(__pos, msg, ext, ctx, *bio_ptr);
		}

	}

	bio_endio(bio);

	*__pos = 0;
	{
		if (*bio_ptr) {
			callee_marshal_kernel__bio_endio__bio__in(__pos, msg, ext, ctx, *bio_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void dm_unregister_target_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct target_type* tt = 0;
	struct target_type** tt_ptr = &tt;
	
	__maybe_unused struct dm_unregister_target_call_ctx call_ctx = {tt};
	__maybe_unused struct dm_unregister_target_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*tt_ptr = glue_unpack_shadow(__pos, msg, ext, struct target_type*);
		if (*tt_ptr) {
			callee_unmarshal_kernel__dm_unregister_target__tt__in(__pos, msg, ext, ctx, *tt_ptr);
		}

	}

	dm_unregister_target(tt);

	*__pos = 0;
	{
		if (*tt_ptr) {
			callee_marshal_kernel__dm_unregister_target__tt__in(__pos, msg, ext, ctx, *tt_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void dm_register_target_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct target_type* tt = 0;
	struct target_type** tt_ptr = &tt;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct dm_register_target_call_ctx call_ctx = {tt};
	__maybe_unused struct dm_register_target_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*tt_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct target_type*, (sizeof(struct target_type)), (DEFAULT_GFP_FLAGS));
		if (*tt_ptr) {
			callee_unmarshal_kernel__dm_register_target__tt__in(__pos, msg, ext, ctx, *tt_ptr);
		}

	}

	tt->module = THIS_MODULE;
	printk("%s, tt->name %s | tt->version {%u, %u, %u} | tt->module %p | tt->ctr %p | tt->map %p\n",
				__func__, tt->name, tt->version[0], tt->version[1], tt->version[2], tt->module, tt->ctr, tt->map);
	ret = dm_register_target(tt);

	*__pos = 0;
	{
		if (*tt_ptr) {
			callee_marshal_kernel__dm_register_target__tt__in(__pos, msg, ext, ctx, *tt_ptr);
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

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext)
{
	switch(id) {
	case RPC_ID_shared_mem_init:
		glue_user_trace("shared_mem_init\n");
		shared_mem_init_callee(msg, ext);
		break;

	case RPC_ID_zero_fill_bio:
		glue_user_trace("zero_fill_bio\n");
		zero_fill_bio_callee(msg, ext);
		break;

	case RPC_ID_bio_endio:
		glue_user_trace("bio_endio\n");
		bio_endio_callee(msg, ext);
		break;

	case RPC_ID_dm_unregister_target:
		glue_user_trace("dm_unregister_target\n");
		dm_unregister_target_callee(msg, ext);
		break;

	case RPC_ID_dm_register_target:
		glue_user_trace("dm_register_target\n");
		dm_register_target_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

