#ifndef COMMON_H
#define COMMON_H

#include <liblcd/trampoline.h>
#include <libfipc.h>
#include <liblcd/boot_info.h>
#include <asm/cacheflush.h>
#include <lcd_domains/microkernel.h>
#include <liblcd/liblcd.h>

#include "glue_user.h"

#define verbose_debug 1
#define glue_pack(pos, msg, ext, value) glue_pack_impl((pos), (msg), (ext), (uint64_t)(value))
#define glue_pack_shadow(pos, msg, ext, value) glue_pack_shadow_impl((pos), (msg), (ext), (value))
#define glue_unpack(pos, msg, ext, type) (type)glue_unpack_impl((pos), (msg), (ext))
#define glue_unpack_shadow(pos, msg, ext, type) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack shadow for type %s\n", __func__, __LINE__, __stringify(type)); \
	(type)glue_unpack_shadow_impl(glue_unpack(pos, msg, ext, void*)); })

#define glue_unpack_new_shadow(pos, msg, ext, type, size) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack new shadow for type %s | size %llu\n", __func__, __LINE__, __stringify(type), (uint64_t) size); \
	(type)glue_unpack_new_shadow_impl(glue_unpack(pos, msg, ext, void*), size); })

#define glue_unpack_bind_or_new_shadow(pos, msg, ext, type, size) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack or bind new shadow for type %s | size %llu\n", __func__, __LINE__, __stringify(type), (uint64_t) size); \
	(type)glue_unpack_bind_or_new_shadow_impl(glue_unpack(pos, msg, ext, void*), size); })

#ifndef LCD_ISOLATE
#define glue_unpack_rpc_ptr(pos, msg, ext, name) \
	glue_peek(pos, msg, ext) ? (fptr_##name)glue_unpack_rpc_ptr_impl(glue_unpack(pos, msg, ext, void*), LCD_DUP_TRAMPOLINE(trmp_##name), LCD_TRAMPOLINE_SIZE(trmp_##name)) : NULL

#else
#define glue_unpack_rpc_ptr(pos, msg, ext, name) NULL; glue_user_panic("Trampolines cannot be used on LCD side")
#endif

#define glue_peek(pos, msg, ext) glue_peek_impl(pos, msg, ext)
#define glue_call_server(pos, msg, rpc_id) \
	msg->regs[0] = *pos; *pos = 0; glue_user_call_server(msg, rpc_id);

#define glue_remove_shadow(shadow) glue_user_remove_shadow(shadow)
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
void glue_user_remove_shadow(void* shadow);

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
	void* shadow = 0;
	if (!ptr)
		return NULL;

	shadow = glue_user_alloc(size);
	glue_user_add_shadow(ptr, shadow);
	return shadow;
}

static inline void* glue_unpack_bind_or_new_shadow_impl(const void* ptr, size_t size)
{
	void* shadow = 0;
	if (!ptr)
		return NULL;

	shadow = glue_user_map_to_shadow(ptr);
	if (!shadow) {
		shadow = glue_user_alloc(size);
		glue_user_add_shadow(ptr, shadow);
	}
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

#ifdef LCD_ISOLATE
void shared_mem_init(void);
#else
void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext);
#endif	/* LCD_ISOLATE */


enum RPC_ID {
	MODULE_INIT,
	MODULE_EXIT,
	RPC_ID_shared_mem_init,
	RPC_ID_release,
	RPC_ID_register_blkdev,
	RPC_ID_del_gendisk,
	RPC_ID_device_add_disk,
	RPC_ID_blk_cleanup_queue,
	RPC_ID_blk_mq_end_request,
	RPC_ID_complete,
	RPC_ID_open,
	RPC_ID_alloc_disk_node,
	RPC_ID_queue_rq,
	RPC_ID_init_hctx,
	RPC_ID_map_queue,
	RPC_ID_set,
	RPC_ID_param_get_int,
	RPC_ID_prepare_to_wait,
	RPC_ID_finish_wait,
	RPC_ID_autoremove_wake_function,
	RPC_ID_unregister_blkdev,
	RPC_ID_blk_mq_free_tag_set,
	RPC_ID_blk_mq_alloc_tag_set,
	RPC_ID_blk_mq_init_queue,
	RPC_ID_blk_mq_map_queue,
	RPC_ID_blk_mq_start_request,
	RPC_ID_blk_mq_complete_request,
	RPC_ID_blk_queue_physical_block_size,
	RPC_ID_blk_queue_logical_block_size,
};

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext);

typedef void (*fptr_release)(struct gendisk* disk, unsigned int mode);
typedef void (*fptr_impl_release)(fptr_release target, struct gendisk* disk, unsigned int mode);

LCD_TRAMPOLINE_DATA(trmp_release)
void LCD_TRAMPOLINE_LINKAGE(trmp_release) trmp_release(struct gendisk* disk, unsigned int mode);

typedef void (*fptr_complete)(struct request* rq);
typedef void (*fptr_impl_complete)(fptr_complete target, struct request* rq);

LCD_TRAMPOLINE_DATA(trmp_complete)
void LCD_TRAMPOLINE_LINKAGE(trmp_complete) trmp_complete(struct request* rq);

typedef int (*fptr_open)(struct block_device* bdev, unsigned int mode);
typedef int (*fptr_impl_open)(fptr_open target, struct block_device* bdev, unsigned int mode);

LCD_TRAMPOLINE_DATA(trmp_open)
int LCD_TRAMPOLINE_LINKAGE(trmp_open) trmp_open(struct block_device* bdev, unsigned int mode);

typedef int (*fptr_queue_rq)(struct blk_mq_hw_ctx* hctx, struct blk_mq_queue_data* bd);
typedef int (*fptr_impl_queue_rq)(fptr_queue_rq target, struct blk_mq_hw_ctx* hctx, struct blk_mq_queue_data* bd);

LCD_TRAMPOLINE_DATA(trmp_queue_rq)
int LCD_TRAMPOLINE_LINKAGE(trmp_queue_rq) trmp_queue_rq(struct blk_mq_hw_ctx* hctx, struct blk_mq_queue_data* bd);

typedef int (*fptr_init_hctx)(struct blk_mq_hw_ctx* hctx, void* data, unsigned int index);
typedef int (*fptr_impl_init_hctx)(fptr_init_hctx target, struct blk_mq_hw_ctx* hctx, void* data, unsigned int index);

LCD_TRAMPOLINE_DATA(trmp_init_hctx)
int LCD_TRAMPOLINE_LINKAGE(trmp_init_hctx) trmp_init_hctx(struct blk_mq_hw_ctx* hctx, void* data, unsigned int index);

typedef struct blk_mq_hw_ctx* (*fptr_map_queue)(struct request_queue* rq, int cpu);
typedef struct blk_mq_hw_ctx* (*fptr_impl_map_queue)(fptr_map_queue target, struct request_queue* rq, int cpu);

LCD_TRAMPOLINE_DATA(trmp_map_queue)
struct blk_mq_hw_ctx* LCD_TRAMPOLINE_LINKAGE(trmp_map_queue) trmp_map_queue(struct request_queue* rq, int cpu);

typedef int (*fptr_set)(signed char* str, struct kernel_param* kp);
typedef int (*fptr_impl_set)(fptr_set target, signed char* str, struct kernel_param* kp);

LCD_TRAMPOLINE_DATA(trmp_set)
int LCD_TRAMPOLINE_LINKAGE(trmp_set) trmp_set(signed char* str, struct kernel_param* kp);

struct release_call_ctx {
	struct gendisk* disk;
	unsigned int mode;
};

struct register_blkdev_call_ctx {
	unsigned int major;
	char const* name;
};

struct del_gendisk_call_ctx {
	struct gendisk* disk;
};

struct device_add_disk_call_ctx {
	struct device* parent;
	struct gendisk* disk;
};

struct blk_cleanup_queue_call_ctx {
	struct request_queue* q;
};

struct blk_mq_end_request_call_ctx {
	struct request* rq;
	int error;
};

struct complete_call_ctx {
	struct request* rq;
};

struct open_call_ctx {
	struct block_device* bdev;
	unsigned int mode;
};

struct alloc_disk_node_call_ctx {
	int minors;
	int node_id;
};

struct queue_rq_call_ctx {
	struct blk_mq_hw_ctx* hctx;
	struct blk_mq_queue_data* bd;
};

struct init_hctx_call_ctx {
	struct blk_mq_hw_ctx* hctx;
	void* data;
	unsigned int index;
};

struct map_queue_call_ctx {
	struct request_queue* rq;
	int cpu;
};

struct set_call_ctx {
	signed char* str;
	struct kernel_param* kp;
};

struct param_get_int_call_ctx {
	char* buffer;
	struct kernel_param const* kp;
};

struct prepare_to_wait_call_ctx {
	struct __wait_queue_head* q;
	struct __wait_queue* wait;
	int state;
};

struct finish_wait_call_ctx {
	struct __wait_queue_head* q;
	struct __wait_queue* wait;
};

struct autoremove_wake_function_call_ctx {
	struct __wait_queue* wait;
	unsigned int mode;
	int sync;
	void* key;
};

struct unregister_blkdev_call_ctx {
	unsigned int major;
	char const* name;
};

struct blk_mq_free_tag_set_call_ctx {
	struct blk_mq_tag_set* set;
};

struct blk_mq_alloc_tag_set_call_ctx {
	struct blk_mq_tag_set* set;
};

struct blk_mq_init_queue_call_ctx {
	struct blk_mq_tag_set* set;
};

struct blk_mq_map_queue_call_ctx {
	struct request_queue* q;
	int cpu;
};

struct blk_mq_start_request_call_ctx {
	struct request* rq;
};

struct blk_mq_complete_request_call_ctx {
	struct request* rq;
	int error;
};

struct blk_queue_physical_block_size_call_ctx {
	struct request_queue* q;
	unsigned int size;
};

struct blk_queue_logical_block_size_call_ctx {
	struct request_queue* q;
	unsigned short size;
};

void caller_marshal_kernel__release__disk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct release_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void callee_unmarshal_kernel__release__disk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct release_call_ctx const* call_ctx,
	struct gendisk* ptr);

void callee_marshal_kernel__release__disk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct release_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void caller_unmarshal_kernel__release__disk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct release_call_ctx const* call_ctx,
	struct gendisk* ptr);

void caller_marshal_kernel__del_gendisk__disk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct del_gendisk_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void callee_unmarshal_kernel__del_gendisk__disk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct del_gendisk_call_ctx const* call_ctx,
	struct gendisk* ptr);

void callee_marshal_kernel__del_gendisk__disk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct del_gendisk_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void caller_unmarshal_kernel__del_gendisk__disk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct del_gendisk_call_ctx const* call_ctx,
	struct gendisk* ptr);

void caller_marshal_kernel__del_gendisk__hd_struct__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct del_gendisk_call_ctx const* call_ctx,
	struct hd_struct const* ptr);

void callee_unmarshal_kernel__del_gendisk__hd_struct__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct del_gendisk_call_ctx const* call_ctx,
	struct hd_struct* ptr);

void callee_marshal_kernel__del_gendisk__hd_struct__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct del_gendisk_call_ctx const* call_ctx,
	struct hd_struct const* ptr);

void caller_unmarshal_kernel__del_gendisk__hd_struct__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct del_gendisk_call_ctx const* call_ctx,
	struct hd_struct* ptr);

void caller_marshal_kernel__device_add_disk__parent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__device_add_disk__parent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__device_add_disk__parent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__device_add_disk__parent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__device_add_disk__disk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void callee_unmarshal_kernel__device_add_disk__disk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct gendisk* ptr);

void callee_marshal_kernel__device_add_disk__disk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void caller_unmarshal_kernel__device_add_disk__disk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct gendisk* ptr);

void caller_marshal_kernel__device_add_disk__hd_struct__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct hd_struct const* ptr);

void callee_unmarshal_kernel__device_add_disk__hd_struct__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct hd_struct* ptr);

void callee_marshal_kernel__device_add_disk__hd_struct__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct hd_struct const* ptr);

void caller_unmarshal_kernel__device_add_disk__hd_struct__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct hd_struct* ptr);

void caller_marshal_kernel__device_add_disk__fops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct block_device_operations const* ptr);

void callee_unmarshal_kernel__device_add_disk__fops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct block_device_operations* ptr);

void callee_marshal_kernel__device_add_disk__fops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct block_device_operations const* ptr);

void caller_unmarshal_kernel__device_add_disk__fops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct block_device_operations* ptr);

void caller_marshal_kernel__device_add_disk__queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__device_add_disk__queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__device_add_disk__queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__device_add_disk__queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_cleanup_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_cleanup_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_cleanup_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_cleanup_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_mq_end_request__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__blk_mq_end_request__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__blk_mq_end_request__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__blk_mq_end_request__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__complete__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct complete_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__complete__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct complete_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__complete__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct complete_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__complete__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct complete_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__open__bdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct open_call_ctx const* call_ctx,
	struct block_device const* ptr);

void callee_unmarshal_kernel__open__bdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct open_call_ctx const* call_ctx,
	struct block_device* ptr);

void callee_marshal_kernel__open__bdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct open_call_ctx const* call_ctx,
	struct block_device const* ptr);

void caller_unmarshal_kernel__open__bdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct open_call_ctx const* call_ctx,
	struct block_device* ptr);

void caller_marshal_kernel__alloc_disk_node__ret_gendisk__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void callee_unmarshal_kernel__alloc_disk_node__ret_gendisk__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* call_ctx,
	struct gendisk* ptr);

void callee_marshal_kernel__alloc_disk_node__ret_gendisk__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void caller_unmarshal_kernel__alloc_disk_node__ret_gendisk__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* call_ctx,
	struct gendisk* ptr);

void caller_marshal_kernel__alloc_disk_node__hd_struct__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* call_ctx,
	struct hd_struct const* ptr);

void callee_unmarshal_kernel__alloc_disk_node__hd_struct__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* call_ctx,
	struct hd_struct* ptr);

void callee_marshal_kernel__alloc_disk_node__hd_struct__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* call_ctx,
	struct hd_struct const* ptr);

void caller_unmarshal_kernel__alloc_disk_node__hd_struct__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* call_ctx,
	struct hd_struct* ptr);

void caller_marshal_kernel__queue_rq__hctx__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_rq_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void callee_unmarshal_kernel__queue_rq__hctx__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_rq_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void callee_marshal_kernel__queue_rq__hctx__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_rq_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void caller_unmarshal_kernel__queue_rq__hctx__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_rq_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void caller_marshal_kernel__queue_rq__bd__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_rq_call_ctx const* call_ctx,
	struct blk_mq_queue_data const* ptr);

void callee_unmarshal_kernel__queue_rq__bd__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_rq_call_ctx const* call_ctx,
	struct blk_mq_queue_data* ptr);

void callee_marshal_kernel__queue_rq__bd__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_rq_call_ctx const* call_ctx,
	struct blk_mq_queue_data const* ptr);

void caller_unmarshal_kernel__queue_rq__bd__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_rq_call_ctx const* call_ctx,
	struct blk_mq_queue_data* ptr);

void caller_marshal_kernel__queue_rq__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_rq_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__queue_rq__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_rq_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__queue_rq__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_rq_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__queue_rq__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_rq_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__init_hctx__hctx__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct init_hctx_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void callee_unmarshal_kernel__init_hctx__hctx__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct init_hctx_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void callee_marshal_kernel__init_hctx__hctx__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct init_hctx_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void caller_unmarshal_kernel__init_hctx__hctx__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct init_hctx_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void caller_marshal_kernel__map_queue__blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void callee_unmarshal_kernel__map_queue__blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void callee_marshal_kernel__map_queue__blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void caller_unmarshal_kernel__map_queue__blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void caller_marshal_kernel__map_queue__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct map_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__map_queue__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct map_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__map_queue__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct map_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__map_queue__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct map_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__set__kp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct set_call_ctx const* call_ctx,
	struct kernel_param const* ptr);

void callee_unmarshal_kernel__set__kp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct set_call_ctx const* call_ctx,
	struct kernel_param* ptr);

void callee_marshal_kernel__set__kp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct set_call_ctx const* call_ctx,
	struct kernel_param const* ptr);

void caller_unmarshal_kernel__set__kp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct set_call_ctx const* call_ctx,
	struct kernel_param* ptr);

void caller_marshal_kernel__param_get_int__kp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct param_get_int_call_ctx const* call_ctx,
	struct kernel_param const* ptr);

void callee_unmarshal_kernel__param_get_int__kp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct param_get_int_call_ctx const* call_ctx,
	struct kernel_param* ptr);

void callee_marshal_kernel__param_get_int__kp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct param_get_int_call_ctx const* call_ctx,
	struct kernel_param const* ptr);

void caller_unmarshal_kernel__param_get_int__kp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct param_get_int_call_ctx const* call_ctx,
	struct kernel_param* ptr);

void caller_marshal_kernel__prepare_to_wait__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void callee_unmarshal_kernel__prepare_to_wait__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void callee_marshal_kernel__prepare_to_wait__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void caller_unmarshal_kernel__prepare_to_wait__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void caller_marshal_kernel__prepare_to_wait__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* call_ctx,
	struct __wait_queue const* ptr);

void callee_unmarshal_kernel__prepare_to_wait__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* call_ctx,
	struct __wait_queue* ptr);

void callee_marshal_kernel__prepare_to_wait__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* call_ctx,
	struct __wait_queue const* ptr);

void caller_unmarshal_kernel__prepare_to_wait__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* call_ctx,
	struct __wait_queue* ptr);

void caller_marshal_kernel__finish_wait__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void callee_unmarshal_kernel__finish_wait__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void callee_marshal_kernel__finish_wait__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void caller_unmarshal_kernel__finish_wait__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void caller_marshal_kernel__finish_wait__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue const* ptr);

void callee_unmarshal_kernel__finish_wait__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue* ptr);

void callee_marshal_kernel__finish_wait__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue const* ptr);

void caller_unmarshal_kernel__finish_wait__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue* ptr);

void caller_marshal_kernel__autoremove_wake_function__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct autoremove_wake_function_call_ctx const* call_ctx,
	struct __wait_queue const* ptr);

void callee_unmarshal_kernel__autoremove_wake_function__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct autoremove_wake_function_call_ctx const* call_ctx,
	struct __wait_queue* ptr);

void callee_marshal_kernel__autoremove_wake_function__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct autoremove_wake_function_call_ctx const* call_ctx,
	struct __wait_queue const* ptr);

void caller_unmarshal_kernel__autoremove_wake_function__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct autoremove_wake_function_call_ctx const* call_ctx,
	struct __wait_queue* ptr);

void caller_marshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_free_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void callee_unmarshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_free_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void callee_marshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_free_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void caller_unmarshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_free_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void caller_marshal_kernel__blk_mq_alloc_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_alloc_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void callee_unmarshal_kernel__blk_mq_alloc_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_alloc_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void callee_marshal_kernel__blk_mq_alloc_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_alloc_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void caller_unmarshal_kernel__blk_mq_alloc_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_alloc_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void caller_marshal_kernel___global_blk_mq_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_ops const* ptr);

void callee_unmarshal_kernel___global_blk_mq_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_ops* ptr);

void callee_marshal_kernel___global_blk_mq_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_ops const* ptr);

void caller_unmarshal_kernel___global_blk_mq_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_ops* ptr);

void caller_marshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void callee_unmarshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void callee_marshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void caller_unmarshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void caller_marshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void callee_unmarshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void callee_marshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void caller_unmarshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void caller_marshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_start_request_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_start_request_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_start_request_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_start_request_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__blk_mq_complete_request__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__blk_mq_complete_request__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__blk_mq_complete_request__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__blk_mq_complete_request__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__blk_queue_physical_block_size__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_queue_physical_block_size_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_queue_physical_block_size__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_queue_physical_block_size_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_queue_physical_block_size__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_queue_physical_block_size_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_queue_physical_block_size__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_queue_physical_block_size_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_queue_logical_block_size__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_queue_logical_block_size_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_queue_logical_block_size__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_queue_logical_block_size_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_queue_logical_block_size__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_queue_logical_block_size_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_queue_logical_block_size__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_queue_logical_block_size_call_ctx const* call_ctx,
	struct request_queue* ptr);


#endif
