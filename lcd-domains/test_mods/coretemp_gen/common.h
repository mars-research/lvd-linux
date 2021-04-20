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
void* glue_user_map_to_shadow(const void* obj, bool fail);
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

	shadow = glue_user_map_to_shadow(ptr, false);
	if (!shadow) {
		shadow = glue_user_alloc(size);
		glue_user_add_shadow(ptr, shadow);
	}
	return shadow;
}

static inline void* glue_unpack_shadow_impl(const void* ptr)
{
	return ptr ? glue_user_map_to_shadow(ptr, true) : NULL;
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
	RPC_ID_sysfs_remove_group,
	RPC_ID_probe,
	RPC_ID_remove,
	RPC_ID_sysfs_create_group,
	RPC_ID_rdmsr_safe_on_cpu,
	RPC_ID_rdmsr_on_cpu,
	RPC_ID_cpu_maps_update_begin,
	RPC_ID_cpu_maps_update_done,
	RPC_ID_platform_driver_unregister,
	RPC_ID___platform_driver_register,
	RPC_ID_platform_device_alloc,
	RPC_ID_platform_device_add,
	RPC_ID_platform_device_put,
	RPC_ID_platform_device_unregister,
	RPC_ID_x86_match_cpu,
	RPC_ID_pci_get_domain_bus_and_slot,
	RPC_ID_devm_hwmon_device_register_with_groups,
};

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext);

typedef int (*fptr_probe)(struct platform_device* pdev);
typedef int (*fptr_impl_probe)(fptr_probe target, struct platform_device* pdev);

LCD_TRAMPOLINE_DATA(trmp_probe)
int LCD_TRAMPOLINE_LINKAGE(trmp_probe) trmp_probe(struct platform_device* pdev);

typedef int (*fptr_remove)(struct platform_device* pdev);
typedef int (*fptr_impl_remove)(fptr_remove target, struct platform_device* pdev);

LCD_TRAMPOLINE_DATA(trmp_remove)
int LCD_TRAMPOLINE_LINKAGE(trmp_remove) trmp_remove(struct platform_device* pdev);

struct sysfs_remove_group_call_ctx {
	struct kobject* dev;
	struct attribute_group const* grp;
};

struct probe_call_ctx {
	struct platform_device* pdev;
};

struct remove_call_ctx {
	struct platform_device* pdev;
};

struct sysfs_create_group_call_ctx {
	struct kobject* dev;
	struct attribute_group const* grp;
};

struct rdmsr_safe_on_cpu_call_ctx {
	unsigned int cpu;
	unsigned int msr_no;
	unsigned int* l;
	unsigned int* h;
};

struct rdmsr_on_cpu_call_ctx {
	unsigned int cpu;
	unsigned int msr_no;
	unsigned int* l;
	unsigned int* h;
};

struct cpu_maps_update_begin_call_ctx {
};

struct cpu_maps_update_done_call_ctx {
};

struct platform_driver_unregister_call_ctx {
	struct platform_driver* drv;
};

struct __platform_driver_register_call_ctx {
	struct platform_driver* drv;
	struct module* owner;
};

struct platform_device_alloc_call_ctx {
	char const* name;
	int id;
};

struct platform_device_add_call_ctx {
	struct platform_device* pdev;
};

struct platform_device_put_call_ctx {
	struct platform_device* pdev;
};

struct platform_device_unregister_call_ctx {
	struct platform_device* pdev;
};

struct x86_match_cpu_call_ctx {
	struct x86_cpu_id const* match;
};

struct pci_get_domain_bus_and_slot_call_ctx {
	int domain;
	unsigned int bus;
	unsigned int devfn;
};

struct devm_hwmon_device_register_with_groups_call_ctx {
	struct device* dev;
	char const* name;
	void* drvdata;
	struct attribute_group const** groups;
};

void caller_marshal_kernel__sysfs_remove_group__kobj__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* call_ctx,
	struct kobject const* ptr);

void callee_unmarshal_kernel__sysfs_remove_group__kobj__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* call_ctx,
	struct kobject* ptr);

void callee_marshal_kernel__sysfs_remove_group__kobj__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* call_ctx,
	struct kobject const* ptr);

void caller_unmarshal_kernel__sysfs_remove_group__kobj__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* call_ctx,
	struct kobject* ptr);

void caller_marshal_kernel__sysfs_remove_group__grp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* call_ctx,
	struct attribute_group const* ptr);

void callee_unmarshal_kernel__sysfs_remove_group__grp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* call_ctx,
	struct attribute_group* ptr);

void callee_marshal_kernel__sysfs_remove_group__grp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* call_ctx,
	struct attribute_group const* ptr);

void caller_unmarshal_kernel__sysfs_remove_group__grp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* call_ctx,
	struct attribute_group* ptr);

void caller_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct platform_device const* ptr);

void callee_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct platform_device* ptr);

void callee_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct platform_device const* ptr);

void caller_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct platform_device* ptr);

void caller_marshal_kernel__probe__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__probe__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__probe__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__probe__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct remove_call_ctx const* call_ctx,
	struct platform_device const* ptr);

void callee_unmarshal_kernel__remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct remove_call_ctx const* call_ctx,
	struct platform_device* ptr);

void callee_marshal_kernel__remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct remove_call_ctx const* call_ctx,
	struct platform_device const* ptr);

void caller_unmarshal_kernel__remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct remove_call_ctx const* call_ctx,
	struct platform_device* ptr);

void caller_marshal_kernel__sysfs_create_group__kobj__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* call_ctx,
	struct kobject const* ptr);

void callee_unmarshal_kernel__sysfs_create_group__kobj__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* call_ctx,
	struct kobject* ptr);

void callee_marshal_kernel__sysfs_create_group__kobj__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* call_ctx,
	struct kobject const* ptr);

void caller_unmarshal_kernel__sysfs_create_group__kobj__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* call_ctx,
	struct kobject* ptr);

void caller_marshal_kernel__sysfs_create_group__grp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* call_ctx,
	struct attribute_group const* ptr);

void callee_unmarshal_kernel__sysfs_create_group__grp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* call_ctx,
	struct attribute_group* ptr);

void callee_marshal_kernel__sysfs_create_group__grp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* call_ctx,
	struct attribute_group const* ptr);

void caller_unmarshal_kernel__sysfs_create_group__grp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* call_ctx,
	struct attribute_group* ptr);

void caller_marshal_kernel__sysfs_create_group__attr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* call_ctx,
	struct attribute const* ptr);

void callee_unmarshal_kernel__sysfs_create_group__attr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* call_ctx,
	struct attribute* ptr);

void callee_marshal_kernel__sysfs_create_group__attr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* call_ctx,
	struct attribute const* ptr);

void caller_unmarshal_kernel__sysfs_create_group__attr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* call_ctx,
	struct attribute* ptr);

void caller_marshal_kernel__platform_driver_unregister__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_driver_unregister_call_ctx const* call_ctx,
	struct platform_driver const* ptr);

void callee_unmarshal_kernel__platform_driver_unregister__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_driver_unregister_call_ctx const* call_ctx,
	struct platform_driver* ptr);

void callee_marshal_kernel__platform_driver_unregister__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_driver_unregister_call_ctx const* call_ctx,
	struct platform_driver const* ptr);

void caller_unmarshal_kernel__platform_driver_unregister__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_driver_unregister_call_ctx const* call_ctx,
	struct platform_driver* ptr);

void caller_marshal_kernel____platform_driver_register__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* call_ctx,
	struct platform_driver const* ptr);

void callee_unmarshal_kernel____platform_driver_register__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* call_ctx,
	struct platform_driver* ptr);

void callee_marshal_kernel____platform_driver_register__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* call_ctx,
	struct platform_driver const* ptr);

void caller_unmarshal_kernel____platform_driver_register__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* call_ctx,
	struct platform_driver* ptr);

void caller_marshal_kernel____platform_driver_register__owner__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* call_ctx,
	struct module const* ptr);

void callee_unmarshal_kernel____platform_driver_register__owner__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* call_ctx,
	struct module* ptr);

void callee_marshal_kernel____platform_driver_register__owner__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* call_ctx,
	struct module const* ptr);

void caller_unmarshal_kernel____platform_driver_register__owner__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* call_ctx,
	struct module* ptr);

void caller_marshal_kernel__platform_device_alloc__ret_platform_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_alloc_call_ctx const* call_ctx,
	struct platform_device const* ptr);

void callee_unmarshal_kernel__platform_device_alloc__ret_platform_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_alloc_call_ctx const* call_ctx,
	struct platform_device* ptr);

void callee_marshal_kernel__platform_device_alloc__ret_platform_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_alloc_call_ctx const* call_ctx,
	struct platform_device const* ptr);

void caller_unmarshal_kernel__platform_device_alloc__ret_platform_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_alloc_call_ctx const* call_ctx,
	struct platform_device* ptr);

void caller_marshal_kernel__platform_device_add__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_add_call_ctx const* call_ctx,
	struct platform_device const* ptr);

void callee_unmarshal_kernel__platform_device_add__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_add_call_ctx const* call_ctx,
	struct platform_device* ptr);

void callee_marshal_kernel__platform_device_add__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_add_call_ctx const* call_ctx,
	struct platform_device const* ptr);

void caller_unmarshal_kernel__platform_device_add__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_add_call_ctx const* call_ctx,
	struct platform_device* ptr);

void caller_marshal_kernel__platform_device_put__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_put_call_ctx const* call_ctx,
	struct platform_device const* ptr);

void callee_unmarshal_kernel__platform_device_put__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_put_call_ctx const* call_ctx,
	struct platform_device* ptr);

void callee_marshal_kernel__platform_device_put__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_put_call_ctx const* call_ctx,
	struct platform_device const* ptr);

void caller_unmarshal_kernel__platform_device_put__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_put_call_ctx const* call_ctx,
	struct platform_device* ptr);

void caller_marshal_kernel__platform_device_unregister__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_unregister_call_ctx const* call_ctx,
	struct platform_device const* ptr);

void callee_unmarshal_kernel__platform_device_unregister__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_unregister_call_ctx const* call_ctx,
	struct platform_device* ptr);

void callee_marshal_kernel__platform_device_unregister__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_unregister_call_ctx const* call_ctx,
	struct platform_device const* ptr);

void caller_unmarshal_kernel__platform_device_unregister__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_unregister_call_ctx const* call_ctx,
	struct platform_device* ptr);

void caller_marshal_kernel__x86_match_cpu__ret_x86_cpu_id__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* call_ctx,
	struct x86_cpu_id const* ptr);

void callee_unmarshal_kernel__x86_match_cpu__ret_x86_cpu_id__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* call_ctx,
	struct x86_cpu_id* ptr);

void callee_marshal_kernel__x86_match_cpu__ret_x86_cpu_id__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* call_ctx,
	struct x86_cpu_id const* ptr);

void caller_unmarshal_kernel__x86_match_cpu__ret_x86_cpu_id__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* call_ctx,
	struct x86_cpu_id* ptr);

void caller_marshal_kernel__x86_match_cpu__match__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* call_ctx,
	struct x86_cpu_id const* ptr);

void callee_unmarshal_kernel__x86_match_cpu__match__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* call_ctx,
	struct x86_cpu_id* ptr);

void callee_marshal_kernel__x86_match_cpu__match__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* call_ctx,
	struct x86_cpu_id const* ptr);

void caller_unmarshal_kernel__x86_match_cpu__match__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* call_ctx,
	struct x86_cpu_id* ptr);

void caller_marshal_kernel__pci_get_domain_bus_and_slot__ret_pci_dev__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_domain_bus_and_slot_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_get_domain_bus_and_slot__ret_pci_dev__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_domain_bus_and_slot_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_get_domain_bus_and_slot__ret_pci_dev__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_domain_bus_and_slot_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_get_domain_bus_and_slot__ret_pci_dev__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_domain_bus_and_slot_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__devm_hwmon_device_register_with_groups__ret_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__devm_hwmon_device_register_with_groups__ret_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__devm_hwmon_device_register_with_groups__ret_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__devm_hwmon_device_register_with_groups__ret_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__devm_hwmon_device_register_with_groups__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__devm_hwmon_device_register_with_groups__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__devm_hwmon_device_register_with_groups__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__devm_hwmon_device_register_with_groups__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__devm_hwmon_device_register_with_groups__groups__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* call_ctx,
	struct attribute_group const* ptr);

void callee_unmarshal_kernel__devm_hwmon_device_register_with_groups__groups__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* call_ctx,
	struct attribute_group* ptr);

void callee_marshal_kernel__devm_hwmon_device_register_with_groups__groups__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* call_ctx,
	struct attribute_group const* ptr);

void caller_unmarshal_kernel__devm_hwmon_device_register_with_groups__groups__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* call_ctx,
	struct attribute_group* ptr);


#endif
