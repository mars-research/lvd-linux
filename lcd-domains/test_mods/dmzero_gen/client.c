#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void ctr_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_ctr function_ptr = glue_unpack(pos, msg, ext, fptr_ctr);
	struct dm_target* ti = 0;
	unsigned int argc = 0;
	char** argv = 0;
	struct dm_target** ti_ptr = &ti;
	unsigned int* argc_ptr = &argc;
	char*** argv_ptr = &argv;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ti_ptr = glue_unpack_new_shadow(pos, msg, ext, struct dm_target*, sizeof(struct dm_target));
		if (*ti_ptr) {
			callee_unmarshal_kernel__ctr__ti__in(pos, msg, ext, *ti_ptr);
		}

	}

	{
		*argc_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	if (0)
	{
		*argv_ptr = glue_unpack(pos, msg, ext, char**);
		if (*argv_ptr) {
			**argv_ptr = glue_unpack(pos, msg, ext, char*);
			if (**argv_ptr) {
				***argv_ptr = glue_unpack(pos, msg, ext, char);
			}

		}

	}

	printk("%s:%d calling ctr with ti %p argc %u argv %p\n", __func__, __LINE__, ti, argc, argv);
	ret = function_ptr(ti, argc, argv);

	*pos = 0;
	{
		if (*ti_ptr) {
			callee_marshal_kernel__ctr__ti__in(pos, msg, ext, *ti_ptr);
		}

	}

	{
	}

	if (0)
	{
		if (*argv_ptr) {
			(void)*argv_ptr;
		}

	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void map_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_map function_ptr = glue_unpack(pos, msg, ext, fptr_map);
	struct dm_target* ti = 0;
	struct bio* bio = 0;
	struct dm_target** ti_ptr = &ti;
	struct bio** bio_ptr = &bio;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ti_ptr = glue_unpack_shadow(pos, msg, ext, struct dm_target*);
		if (*ti_ptr) {
			callee_unmarshal_kernel__map__ti__in(pos, msg, ext, *ti_ptr);
		}

	}

	{
		*bio_ptr = glue_unpack_new_shadow(pos, msg, ext, struct bio*, sizeof(struct bio));
		if (*bio_ptr) {
			callee_unmarshal_kernel__map__bio__in(pos, msg, ext, *bio_ptr);
		}

	}

	printk("%s:%d calling map with ti %p bio %p\n", __func__, __LINE__, ti, bio);
	ret = function_ptr(ti, bio);

	*pos = 0;
	{
		if (*ti_ptr) {
			callee_marshal_kernel__map__ti__in(pos, msg, ext, *ti_ptr);
		}

	}

	{
		if (*bio_ptr) {
			callee_marshal_kernel__map__bio__in(pos, msg, ext, *bio_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void zero_fill_bio(struct bio* bio)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct bio** bio_ptr = &bio;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack_shadow(pos, msg, ext, *bio_ptr);
		if (*bio_ptr) {
			caller_marshal_kernel__zero_fill_bio__bio__in(pos, msg, ext, *bio_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_zero_fill_bio);

	*pos = 0;
	{
		if (*bio_ptr) {
			caller_unmarshal_kernel__zero_fill_bio__bio__in(pos, msg, ext, *bio_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void bio_endio(struct bio* bio)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct bio** bio_ptr = &bio;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack_shadow(pos, msg, ext, *bio_ptr);
		if (*bio_ptr) {
			caller_marshal_kernel__bio_endio__bio__in(pos, msg, ext, *bio_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_bio_endio);

	*pos = 0;
	{
		if (*bio_ptr) {
			caller_unmarshal_kernel__bio_endio__bio__in(pos, msg, ext, *bio_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void dm_unregister_target(struct target_type* tt)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct target_type** tt_ptr = &tt;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *tt_ptr);
		if (*tt_ptr) {
			caller_marshal_kernel__dm_unregister_target__tt__in(pos, msg, ext, *tt_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_dm_unregister_target);

	*pos = 0;
	{
		if (*tt_ptr) {
			caller_unmarshal_kernel__dm_unregister_target__tt__in(pos, msg, ext, *tt_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int dm_register_target(struct target_type* tt)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct target_type** tt_ptr = &tt;
	int ret = 0;
	int* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *tt_ptr);
		if (*tt_ptr) {
			caller_marshal_kernel__dm_register_target__tt__in(pos, msg, ext, *tt_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_dm_register_target);

	*pos = 0;
	{
		if (*tt_ptr) {
			caller_unmarshal_kernel__dm_register_target__tt__in(pos, msg, ext, *tt_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, int);
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

