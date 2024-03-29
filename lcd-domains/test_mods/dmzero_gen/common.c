#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__ctr__ti__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ctr_call_ctx const* ctx,
	struct dm_target const* ptr)
{
	
}

void callee_unmarshal_kernel__ctr__ti__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ctr_call_ctx const* ctx,
	struct dm_target* ptr)
{
	
}

void callee_marshal_kernel__ctr__ti__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ctr_call_ctx const* ctx,
	struct dm_target const* ptr)
{
	unsigned int const* num_discard_bios_ptr = &ptr->num_discard_bios;
	
	{
		glue_pack(__pos, msg, ext, *num_discard_bios_ptr);
	}

}

void caller_unmarshal_kernel__ctr__ti__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ctr_call_ctx const* ctx,
	struct dm_target* ptr)
{
	unsigned int* num_discard_bios_ptr = &ptr->num_discard_bios;
	
	{
		*num_discard_bios_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel__map__ti__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct map_call_ctx const* ctx,
	struct dm_target const* ptr)
{
	
}

void callee_unmarshal_kernel__map__ti__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct map_call_ctx const* ctx,
	struct dm_target* ptr)
{
	
}

void callee_marshal_kernel__map__ti__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct map_call_ctx const* ctx,
	struct dm_target const* ptr)
{
	
}

void caller_unmarshal_kernel__map__ti__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct map_call_ctx const* ctx,
	struct dm_target* ptr)
{
	
}

void caller_marshal_kernel__map__bio__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct map_call_ctx const* ctx,
	struct bio const* ptr)
{
	unsigned int const* bi_opf_ptr = &ptr->bi_opf;
	
	{
		glue_pack(__pos, msg, ext, *bi_opf_ptr);
	}

}

void callee_unmarshal_kernel__map__bio__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct map_call_ctx const* ctx,
	struct bio* ptr)
{
	unsigned int* bi_opf_ptr = &ptr->bi_opf;
	
	{
		*bi_opf_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__map__bio__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct map_call_ctx const* ctx,
	struct bio const* ptr)
{
	
}

void caller_unmarshal_kernel__map__bio__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct map_call_ctx const* ctx,
	struct bio* ptr)
{
	
}

void caller_marshal_kernel__zero_fill_bio__bio__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct zero_fill_bio_call_ctx const* ctx,
	struct bio const* ptr)
{
	
}

void callee_unmarshal_kernel__zero_fill_bio__bio__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct zero_fill_bio_call_ctx const* ctx,
	struct bio* ptr)
{
	
}

void callee_marshal_kernel__zero_fill_bio__bio__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct zero_fill_bio_call_ctx const* ctx,
	struct bio const* ptr)
{
	
}

void caller_unmarshal_kernel__zero_fill_bio__bio__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct zero_fill_bio_call_ctx const* ctx,
	struct bio* ptr)
{
	
}

void caller_marshal_kernel__bio_endio__bio__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct bio_endio_call_ctx const* ctx,
	struct bio const* ptr)
{
	
}

void callee_unmarshal_kernel__bio_endio__bio__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct bio_endio_call_ctx const* ctx,
	struct bio* ptr)
{
	
}

void callee_marshal_kernel__bio_endio__bio__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct bio_endio_call_ctx const* ctx,
	struct bio const* ptr)
{
	
}

void caller_unmarshal_kernel__bio_endio__bio__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct bio_endio_call_ctx const* ctx,
	struct bio* ptr)
{
	
}

void caller_marshal_kernel__dm_unregister_target__tt__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dm_unregister_target_call_ctx const* ctx,
	struct target_type const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	unsigned int const* version_ptr = ptr->version;
	fptr_ctr const* ctr_ptr = &ptr->ctr;
	fptr_map const* map_ptr = &ptr->map;
	
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

	{
		size_t i, len = 3;
		unsigned int const* array = version_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ctr_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *map_ptr);
	}

}

void callee_unmarshal_kernel__dm_unregister_target__tt__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dm_unregister_target_call_ctx const* ctx,
	struct target_type* ptr)
{
	char const** name_ptr = &ptr->name;
	unsigned int* version_ptr = ptr->version;
	fptr_ctr* ctr_ptr = &ptr->ctr;
	fptr_map* map_ptr = &ptr->map;
	
	{
		*name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, (sizeof(char) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
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

	{
		int i;
		unsigned int* array = version_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	{
		*ctr_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, ctr);
	}

	{
		*map_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, map);
	}

}

void callee_marshal_kernel__dm_unregister_target__tt__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dm_unregister_target_call_ctx const* ctx,
	struct target_type const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	
	{
		(void)name_ptr;
	}

}

void caller_unmarshal_kernel__dm_unregister_target__tt__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dm_unregister_target_call_ctx const* ctx,
	struct target_type* ptr)
{
	char const** name_ptr = &ptr->name;
	
	{
		(void)name_ptr;
	}

}

void caller_marshal_kernel__dm_register_target__tt__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dm_register_target_call_ctx const* ctx,
	struct target_type const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	unsigned int const* version_ptr = ptr->version;
	fptr_ctr const* ctr_ptr = &ptr->ctr;
	fptr_map const* map_ptr = &ptr->map;
	
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

	{
		size_t i, len = 3;
		unsigned int const* array = version_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ctr_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *map_ptr);
	}

}

void callee_unmarshal_kernel__dm_register_target__tt__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dm_register_target_call_ctx const* ctx,
	struct target_type* ptr)
{
	char const** name_ptr = &ptr->name;
	unsigned int* version_ptr = ptr->version;
	fptr_ctr* ctr_ptr = &ptr->ctr;
	fptr_map* map_ptr = &ptr->map;
	
	{
		*name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, (sizeof(char) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
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

	{
		int i;
		unsigned int* array = version_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	{
		*ctr_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, ctr);
	}

	{
		*map_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, map);
	}

}

void callee_marshal_kernel__dm_register_target__tt__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dm_register_target_call_ctx const* ctx,
	struct target_type const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	
	{
		(void)name_ptr;
	}

}

void caller_unmarshal_kernel__dm_register_target__tt__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dm_register_target_call_ctx const* ctx,
	struct target_type* ptr)
{
	char const** name_ptr = &ptr->name;
	
	{
		(void)name_ptr;
	}

}


#ifdef LCD_ISOLATE
__attribute__((weak)) void shared_mem_init(void) {
	LIBLCD_MSG("Weak shared_mem_init does nothing! Override if you want!");
}
#else
__attribute__((weak)) void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext) {
	LIBLCD_MSG("Weak shared_mem_init_callee does nothing! Override if you want!");
}
#endif	/* LCD_ISOLATE */

