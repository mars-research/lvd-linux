#ifndef COMMON_H
#define COMMON_H

#include <liblcd/trampoline.h>
#include <libfipc.h>
#include <liblcd/boot_info.h>
#include <asm/cacheflush.h>

#include "glue_user.h"

#define verbose_debug 0
#define glue_pack(pos, msg, ext, value) glue_pack_impl((pos), (msg), (ext), (uint64_t)(value))
#define glue_pack_shadow(pos, msg, ext, value) glue_pack_shadow_impl((pos), (msg), (ext), (value))
#define glue_unpack(pos, msg, ext, type) (type)glue_unpack_impl((pos), (msg), (ext))
#define glue_unpack_shadow(pos, msg, ext, type) \
(type)glue_unpack_shadow_impl(glue_unpack(pos, msg, ext, void*));

#define glue_unpack_new_shadow(pos, msg, ext, type, size) \
	(type)glue_unpack_new_shadow_impl(glue_unpack(pos, msg, ext, void*), size)

#ifndef LCD_ISOLATE
#define glue_unpack_rpc_ptr(pos, msg, ext, name) \
	glue_peek(pos, msg, ext) ? (fptr_##name)glue_unpack_rpc_ptr_impl(glue_unpack(pos, msg, ext, void*), LCD_DUP_TRAMPOLINE(trmp_##name), LCD_TRAMPOLINE_SIZE(trmp_##name)) : NULL

#else
#define glue_unpack_rpc_ptr(pos, msg, ext, name) NULL; glue_user_panic("Trampolines cannot be used on LCD side")
#endif

#define glue_peek(pos, msg, ext) glue_peek_impl(pos, msg, ext)
#define glue_call_server(pos, msg, rpc_id) \
	msg->regs[0] = *pos; *pos = 0; glue_user_call_server(msg, rpc_id);

#define glue_call_client(pos, msg, rpc_id) \
	msg->regs[0] = *pos; *pos = 0; glue_user_call_client(msg, rpc_id);

void glue_user_init(void);
void glue_user_panic(const char* msg);
void glue_user_trace(const char* msg);
void* glue_user_map_to_shadow(const void* obj);
const void* glue_user_map_from_shadow(const void* shadow);
void glue_user_add_shadow(const void* ptr, void* shadow);
void* glue_user_alloc(size_t size);
void glue_user_free(void* ptr);
void glue_user_call_server(struct fipc_message* msg, size_t rpc_id);
void glue_user_call_client(struct fipc_message* msg, size_t rpc_id);

static inline void* glue_unpack_rpc_ptr_impl(void* target, struct lcd_trampoline_handle* handle, size_t size)
{
	if (!target)
		glue_user_panic("Target was NULL");

	if (!handle)
		glue_user_panic("Trmp was NULL");

	set_memory_x(((unsigned long)handle) & PAGE_MASK, ALIGN(size, PAGE_SIZE) >> PAGE_SHIFT);
	handle->hidden_args = target;
	return LCD_HANDLE_TO_TRAMPOLINE(handle);
}

static inline void
glue_pack_impl(size_t* pos, struct fipc_message* msg, struct ext_registers* ext, uint64_t value)
{
	if (*pos >= 128)
		glue_user_panic("Glue message was too large");
	if (*pos < 6)
		msg->regs[(*pos)++ + 1] = value;
	else
		ext->regs[(*pos)++ + 1] = value;
}

static inline uint64_t
glue_unpack_impl(size_t* pos, const struct fipc_message* msg, const struct ext_registers* ext)
{
	if (*pos >= msg->regs[0])
		glue_user_panic("Unpacked past end of glue message");
	if (*pos < 6)
		return msg->regs[(*pos)++ + 1];
	else
		return ext->regs[(*pos)++ + 1];
}

static inline uint64_t
glue_peek_impl(size_t* pos, const struct fipc_message* msg, const struct ext_registers* ext)
{
	if (*pos >= msg->regs[0])
		glue_user_panic("Peeked past end of glue message");
	if (*pos < 5)
		return msg->regs[*pos + 2];
	else
		return ext->regs[*pos + 2];
}

static inline void* glue_unpack_new_shadow_impl(const void* ptr, size_t size)
{
	void* shadow = 0;	if (!ptr)
		return NULL;

	shadow = glue_user_alloc(size);
	glue_user_add_shadow(ptr, shadow);
	return shadow;
}

static inline void* glue_unpack_shadow_impl(const void* ptr)
{
	return ptr ? glue_user_map_to_shadow(ptr) : NULL;
}

static inline void glue_pack_shadow_impl(size_t* pos, struct fipc_message* msg, struct ext_registers* ext, const void* ptr)
{
	glue_pack(pos, msg, ext, ptr ? glue_user_map_from_shadow(ptr) : NULL);
}

enum RPC_ID {
	MODULE_INIT,
	MODULE_EXIT,
	RPC_ID_ctr,
	RPC_ID_map,
	RPC_ID_zero_fill_bio,
	RPC_ID_dm_unregister_target,
	RPC_ID_dm_register_target,
};

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext);

void caller_marshal_kernel__ctr__ti__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dm_target const* ptr);

void callee_unmarshal_kernel__ctr__ti__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dm_target* ptr);

void callee_marshal_kernel__ctr__ti__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dm_target const* ptr);

void caller_unmarshal_kernel__ctr__ti__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dm_target* ptr);

void caller_marshal_kernel__map__ti__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dm_target const* ptr);

void callee_unmarshal_kernel__map__ti__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dm_target* ptr);

void callee_marshal_kernel__map__ti__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dm_target const* ptr);

void caller_unmarshal_kernel__map__ti__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dm_target* ptr);

void caller_marshal_kernel__map__bio__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct bio const* ptr);

void callee_unmarshal_kernel__map__bio__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct bio* ptr);

void callee_marshal_kernel__map__bio__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct bio const* ptr);

void caller_unmarshal_kernel__map__bio__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct bio* ptr);

void caller_marshal_kernel__zero_fill_bio__bio__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct bio const* ptr);

void callee_unmarshal_kernel__zero_fill_bio__bio__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct bio* ptr);

void callee_marshal_kernel__zero_fill_bio__bio__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct bio const* ptr);

void caller_unmarshal_kernel__zero_fill_bio__bio__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct bio* ptr);

void caller_marshal_kernel__dm_unregister_target__tt__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct target_type const* ptr);

void callee_unmarshal_kernel__dm_unregister_target__tt__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct target_type* ptr);

void callee_marshal_kernel__dm_unregister_target__tt__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct target_type const* ptr);

void caller_unmarshal_kernel__dm_unregister_target__tt__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct target_type* ptr);

void caller_marshal_kernel__dm_register_target__tt__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct target_type const* ptr);

void callee_unmarshal_kernel__dm_register_target__tt__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct target_type* ptr);

void callee_marshal_kernel__dm_register_target__tt__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct target_type const* ptr);

void caller_unmarshal_kernel__dm_register_target__tt__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct target_type* ptr);

typedef int (*fptr_ctr)(struct dm_target* ti, unsigned int argc, char** argv);
typedef int (*fptr_impl_ctr)(fptr_ctr target, struct dm_target* ti, unsigned int argc, char** argv);

LCD_TRAMPOLINE_DATA(trmp_ctr)
int LCD_TRAMPOLINE_LINKAGE(trmp_ctr) trmp_ctr(struct dm_target* ti, unsigned int argc, char** argv);

typedef int (*fptr_map)(struct dm_target* ti, struct bio* bio);
typedef int (*fptr_impl_map)(fptr_map target, struct dm_target* ti, struct bio* bio);

LCD_TRAMPOLINE_DATA(trmp_map)
int LCD_TRAMPOLINE_LINKAGE(trmp_map) trmp_map(struct dm_target* ti, struct bio* bio);


#endif
