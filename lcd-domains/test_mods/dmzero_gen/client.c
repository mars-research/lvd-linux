#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void ctr_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ctr function_ptr = glue_unpack(__pos, msg, ext, fptr_ctr);
	struct dm_target* ti = 0;
	unsigned int argc = 0;
	char** argv = 0;
	struct dm_target** ti_ptr = &ti;
	unsigned int* argc_ptr = &argc;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ctr_call_ctx call_ctx = {ti, argc, argv};
	__maybe_unused struct ctr_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ti_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct dm_target*, (sizeof(struct dm_target)), (DEFAULT_GFP_FLAGS));
		if (*ti_ptr) {
			callee_unmarshal_kernel__ctr__ti__in(__pos, msg, ext, ctx, *ti_ptr);
		}

	}

	{
		*argc_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = function_ptr(ti, argc, argv);

	*__pos = 0;
	{
		if (*ti_ptr) {
			callee_marshal_kernel__ctr__ti__in(__pos, msg, ext, ctx, *ti_ptr);
		}

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

void map_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_map function_ptr = glue_unpack(__pos, msg, ext, fptr_map);
	struct dm_target* ti = 0;
	struct bio* bio = 0;
	struct bio** bio_ptr = &bio;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct map_call_ctx call_ctx = {ti, bio};
	__maybe_unused struct map_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*bio_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct bio*, (sizeof(struct bio)), (DEFAULT_GFP_FLAGS));
		if (*bio_ptr) {
			callee_unmarshal_kernel__map__bio__in(__pos, msg, ext, ctx, *bio_ptr);
		}

	}

	ret = function_ptr(ti, bio);

	*__pos = 0;
	{
		if (*bio_ptr) {
			callee_marshal_kernel__map__bio__in(__pos, msg, ext, ctx, *bio_ptr);
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

void zero_fill_bio(struct bio* bio)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct bio** bio_ptr = &bio;
	
	__maybe_unused const struct zero_fill_bio_call_ctx call_ctx = {bio};
	__maybe_unused const struct zero_fill_bio_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *bio_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*bio_ptr) {
			caller_marshal_kernel__zero_fill_bio__bio__in(__pos, msg, ext, ctx, *bio_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_zero_fill_bio);

	*__pos = 0;
	{
		if (*bio_ptr) {
			caller_unmarshal_kernel__zero_fill_bio__bio__in(__pos, msg, ext, ctx, *bio_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void bio_endio(struct bio* bio)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct bio** bio_ptr = &bio;
	
	__maybe_unused const struct bio_endio_call_ctx call_ctx = {bio};
	__maybe_unused const struct bio_endio_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *bio_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*bio_ptr) {
			caller_marshal_kernel__bio_endio__bio__in(__pos, msg, ext, ctx, *bio_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_bio_endio);

	*__pos = 0;
	{
		if (*bio_ptr) {
			caller_unmarshal_kernel__bio_endio__bio__in(__pos, msg, ext, ctx, *bio_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void dm_unregister_target(struct target_type* tt)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct target_type** tt_ptr = &tt;
	
	__maybe_unused const struct dm_unregister_target_call_ctx call_ctx = {tt};
	__maybe_unused const struct dm_unregister_target_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *tt_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*tt_ptr) {
			caller_marshal_kernel__dm_unregister_target__tt__in(__pos, msg, ext, ctx, *tt_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_dm_unregister_target);

	*__pos = 0;
	{
		if (*tt_ptr) {
			caller_unmarshal_kernel__dm_unregister_target__tt__in(__pos, msg, ext, ctx, *tt_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int dm_register_target(struct target_type* tt)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct target_type** tt_ptr = &tt;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct dm_register_target_call_ctx call_ctx = {tt};
	__maybe_unused const struct dm_register_target_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *tt_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*tt_ptr) {
			caller_marshal_kernel__dm_register_target__tt__in(__pos, msg, ext, ctx, *tt_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_dm_register_target);

	*__pos = 0;
	{
		if (*tt_ptr) {
			caller_unmarshal_kernel__dm_register_target__tt__in(__pos, msg, ext, ctx, *tt_ptr);
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

	case RPC_ID_ctr:
		glue_user_trace("ctr\n");
		ctr_callee(msg, ext);
		break;

	case RPC_ID_map:
		glue_user_trace("map\n");
		map_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

