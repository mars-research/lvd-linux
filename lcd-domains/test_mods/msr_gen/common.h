#ifndef COMMON_H
#define COMMON_H

#include <liblcd/trampoline.h>
#include <asm/cacheflush.h>

#include "glue_user.h"

#define verbose_debug 0
#define GLUE_MAX_SLOTS 128
#define glue_pack(msg, value) glue_pack_impl((msg), (uint64_t)(value))
#define glue_pack_shadow(msg, value) glue_pack_shadow_impl((msg), (value))
#define glue_unpack(msg, type) (type)glue_unpack_impl((msg))
#define glue_unpack_shadow(msg, type) (type)glue_unpack_shadow_impl(glue_unpack(msg, void*));
#define glue_unpack_new_shadow(msg, type, size) \
	(type)glue_unpack_new_shadow_impl(glue_unpack(msg, void*), size)

#define glue_unpack_rpc_ptr(msg, name) \
	glue_peek(msg) ? (fptr_##name)glue_unpack_rpc_ptr_impl(glue_unpack(msg, void*), LCD_DUP_TRAMPOLINE(trmp_##name), LCD_TRAMPOLINE_SIZE(trmp_##name)) : NULL

#define glue_peek(msg) glue_peek_impl(msg)
#define glue_call_server(msg, rpc_id) \
	msg->slots[0] = msg->position; msg->position = 0; glue_user_call_server(msg->slots, rpc_id);

#define glue_call_client(msg, rpc_id) \
	msg->slots[0] = msg->position; msg->position = 0; glue_user_call_client(msg->slots, rpc_id);


void glue_user_init(void);
void glue_user_panic(const char* msg);
void glue_user_trace(const char* msg);
void* glue_user_map_to_shadow(const void* obj);
const void* glue_user_map_from_shadow(const void* shadow);
void glue_user_add_shadow(const void* ptr, void* shadow);
void* glue_user_alloc(size_t size);
void glue_user_free(void* ptr);
void glue_user_call_server(uint64_t* data, size_t rpc_id);
void glue_user_call_client(uint64_t* data, size_t rpc_id);

struct glue_message {
	uint64_t slots[GLUE_MAX_SLOTS];
	uint64_t position;
};

extern struct glue_message shared_buffer;

static inline struct glue_message* glue_init_msg(void)
{
	shared_buffer.position = 0;
	return &shared_buffer;
}

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

static inline void glue_pack_impl(struct glue_message* msg, uint64_t value)
{
	if (msg->position >= GLUE_MAX_SLOTS)
		glue_user_panic("Glue message was too large");
	msg->slots[msg->position++ + 1] = value;
}

static inline uint64_t glue_unpack_impl(struct glue_message* msg)
{
	if (msg->position >= msg->slots[0])
		glue_user_panic("Unpacked past end of glue message");
	return msg->slots[msg->position++ + 1];
}

static inline uint64_t glue_peek_impl(struct glue_message* msg)
{
	if (msg->position >= msg->slots[0])
		glue_user_panic("Peeked past end of glue message");
	return msg->slots[msg->position + 2];
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

static inline void glue_pack_shadow_impl(struct glue_message* msg, const void* ptr)
{
	glue_pack(msg, ptr ? glue_user_map_from_shadow(ptr) : NULL);
}

enum RPC_ID {
	MODULE_INIT,
	MODULE_EXIT,
	RPC_ID_read,
	RPC_ID_write,
	RPC_ID_compat_ioctl,
	RPC_ID_open,
	RPC_ID___register_chrdev,
	RPC_ID___unregister_chrdev,
	RPC_ID_cpu_maps_update_begin,
	RPC_ID_cpu_maps_update_done,
	RPC_ID_wrmsr_safe_regs_on_cpu,
	RPC_ID_rdmsr_safe_regs_on_cpu,
	RPC_ID_wrmsr_safe_on_cpu,
	RPC_ID_rdmsr_safe_on_cpu,
	RPC_ID_capable,
	RPC_ID_no_seek_end_llseek,
};

int try_dispatch(enum RPC_ID id, struct glue_message* msg);

void caller_marshal_kernel__read__file__in(
	struct glue_message*,
	struct file const*);

void callee_unmarshal_kernel__read__file__in(
	struct glue_message*,
	struct file*);

void callee_marshal_kernel__read__file__in(
	struct glue_message*,
	struct file const*);

void caller_unmarshal_kernel__read__file__in(
	struct glue_message*,
	struct file*);

void caller_marshal_kernel__read__inode__in(
	struct glue_message*,
	struct inode const*);

void callee_unmarshal_kernel__read__inode__in(
	struct glue_message*,
	struct inode*);

void callee_marshal_kernel__read__inode__in(
	struct glue_message*,
	struct inode const*);

void caller_unmarshal_kernel__read__inode__in(
	struct glue_message*,
	struct inode*);

void caller_marshal_kernel__write__file__in(
	struct glue_message*,
	struct file const*);

void callee_unmarshal_kernel__write__file__in(
	struct glue_message*,
	struct file*);

void callee_marshal_kernel__write__file__in(
	struct glue_message*,
	struct file const*);

void caller_unmarshal_kernel__write__file__in(
	struct glue_message*,
	struct file*);

void caller_marshal_kernel__write__inode__in(
	struct glue_message*,
	struct inode const*);

void callee_unmarshal_kernel__write__inode__in(
	struct glue_message*,
	struct inode*);

void callee_marshal_kernel__write__inode__in(
	struct glue_message*,
	struct inode const*);

void caller_unmarshal_kernel__write__inode__in(
	struct glue_message*,
	struct inode*);

void caller_marshal_kernel__compat_ioctl__file__in(
	struct glue_message*,
	struct file const*);

void callee_unmarshal_kernel__compat_ioctl__file__in(
	struct glue_message*,
	struct file*);

void callee_marshal_kernel__compat_ioctl__file__in(
	struct glue_message*,
	struct file const*);

void caller_unmarshal_kernel__compat_ioctl__file__in(
	struct glue_message*,
	struct file*);

void caller_marshal_kernel__compat_ioctl__inode__in(
	struct glue_message*,
	struct inode const*);

void callee_unmarshal_kernel__compat_ioctl__inode__in(
	struct glue_message*,
	struct inode*);

void callee_marshal_kernel__compat_ioctl__inode__in(
	struct glue_message*,
	struct inode const*);

void caller_unmarshal_kernel__compat_ioctl__inode__in(
	struct glue_message*,
	struct inode*);

void caller_marshal_kernel__open__inode__in(
	struct glue_message*,
	struct inode const*);

void callee_unmarshal_kernel__open__inode__in(
	struct glue_message*,
	struct inode*);

void callee_marshal_kernel__open__inode__in(
	struct glue_message*,
	struct inode const*);

void caller_unmarshal_kernel__open__inode__in(
	struct glue_message*,
	struct inode*);

void caller_marshal_kernel__open__file__in(
	struct glue_message*,
	struct file const*);

void callee_unmarshal_kernel__open__file__in(
	struct glue_message*,
	struct file*);

void callee_marshal_kernel__open__file__in(
	struct glue_message*,
	struct file const*);

void caller_unmarshal_kernel__open__file__in(
	struct glue_message*,
	struct file*);

void caller_marshal_kernel____register_chrdev__fops__in(
	struct glue_message*,
	struct file_operations const*);

void callee_unmarshal_kernel____register_chrdev__fops__in(
	struct glue_message*,
	struct file_operations*);

void callee_marshal_kernel____register_chrdev__fops__in(
	struct glue_message*,
	struct file_operations const*);

void caller_unmarshal_kernel____register_chrdev__fops__in(
	struct glue_message*,
	struct file_operations*);

void caller_marshal_kernel____register_chrdev__owner__in(
	struct glue_message*,
	struct module const*);

void callee_unmarshal_kernel____register_chrdev__owner__in(
	struct glue_message*,
	struct module*);

void callee_marshal_kernel____register_chrdev__owner__in(
	struct glue_message*,
	struct module const*);

void caller_unmarshal_kernel____register_chrdev__owner__in(
	struct glue_message*,
	struct module*);

void caller_marshal_kernel__no_seek_end_llseek__file__in(
	struct glue_message*,
	struct file const*);

void callee_unmarshal_kernel__no_seek_end_llseek__file__in(
	struct glue_message*,
	struct file*);

void callee_marshal_kernel__no_seek_end_llseek__file__in(
	struct glue_message*,
	struct file const*);

void caller_unmarshal_kernel__no_seek_end_llseek__file__in(
	struct glue_message*,
	struct file*);

typedef long (*fptr_read)(struct file* file, char* buf, unsigned long count, long long* ppos);
typedef long (*fptr_impl_read)(fptr_read target, struct file* file, char* buf, unsigned long count, long long* ppos);

LCD_TRAMPOLINE_DATA(trmp_read)
long LCD_TRAMPOLINE_LINKAGE(trmp_read) trmp_read(struct file* file, char* buf, unsigned long count, long long* ppos);

typedef long (*fptr_write)(struct file* file, char const* buf, unsigned long count, long long* ppos);
typedef long (*fptr_impl_write)(fptr_write target, struct file* file, char const* buf, unsigned long count, long long* ppos);

LCD_TRAMPOLINE_DATA(trmp_write)
long LCD_TRAMPOLINE_LINKAGE(trmp_write) trmp_write(struct file* file, char const* buf, unsigned long count, long long* ppos);

typedef long (*fptr_compat_ioctl)(struct file* file, unsigned int ioc, unsigned long arg);
typedef long (*fptr_impl_compat_ioctl)(fptr_compat_ioctl target, struct file* file, unsigned int ioc, unsigned long arg);

LCD_TRAMPOLINE_DATA(trmp_compat_ioctl)
long LCD_TRAMPOLINE_LINKAGE(trmp_compat_ioctl) trmp_compat_ioctl(struct file* file, unsigned int ioc, unsigned long arg);

typedef int (*fptr_open)(struct inode* inode, struct file* file);
typedef int (*fptr_impl_open)(fptr_open target, struct inode* inode, struct file* file);

LCD_TRAMPOLINE_DATA(trmp_open)
int LCD_TRAMPOLINE_LINKAGE(trmp_open) trmp_open(struct inode* inode, struct file* file);


#endif
