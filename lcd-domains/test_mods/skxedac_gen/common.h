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
	if (*pos >= 500)
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
	RPC_ID_notifier_call,
	RPC_ID_pci_enable_device,
	RPC_ID_x86_match_cpu,
	RPC_ID_pci_dev_get,
	RPC_ID_pci_dev_put,
	RPC_ID_mce_unregister_decode_chain,
	RPC_ID_mce_register_decode_chain,
	RPC_ID_edac_mc_handle_error,
	RPC_ID_edac_mc_del_mc,
	RPC_ID_edac_mc_add_mc_with_groups,
	RPC_ID_edac_mc_free,
	RPC_ID_edac_mc_alloc,
	RPC_ID_pci_get_device,
	RPC_ID_pci_bus_read_config_dword,
};

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext);

typedef int (*fptr_notifier_call)(struct notifier_block* nb, unsigned long val, void* data);
typedef int (*fptr_impl_notifier_call)(fptr_notifier_call target, struct notifier_block* nb, unsigned long val, void* data);

LCD_TRAMPOLINE_DATA(trmp_notifier_call)
int LCD_TRAMPOLINE_LINKAGE(trmp_notifier_call) trmp_notifier_call(struct notifier_block* nb, unsigned long val, void* data);

struct notifier_call_call_ctx {
	struct notifier_block* nb;
	unsigned long val;
	void* data;
};

struct pci_enable_device_call_ctx {
	struct pci_dev* dev;
};

struct x86_match_cpu_call_ctx {
	struct x86_cpu_id const* match;
};

struct pci_dev_get_call_ctx {
	struct pci_dev* dev;
};

struct pci_dev_put_call_ctx {
	struct pci_dev* dev;
};

struct mce_unregister_decode_chain_call_ctx {
	struct notifier_block* nb;
};

struct mce_register_decode_chain_call_ctx {
	struct notifier_block* nb;
};

struct edac_mc_handle_error_call_ctx {
	unsigned int type;
	struct mem_ctl_info* mci;
	unsigned short error_count;
	unsigned long page_frame_number;
	unsigned long offset_in_page;
	unsigned long syndrome;
	int top_layer;
	int mid_layer;
	int low_layer;
	char const* _msg;
	char const* other_detail;
};

struct edac_mc_del_mc_call_ctx {
	struct device* dev;
};

struct edac_mc_add_mc_with_groups_call_ctx {
	struct mem_ctl_info* mci;
	struct attribute_group const** groups;
};

struct edac_mc_free_call_ctx {
	struct mem_ctl_info* mci;
};

struct edac_mc_alloc_call_ctx {
	unsigned int mc_num;
	unsigned int n_layers;
	struct edac_mc_layer* layers;
	unsigned int sz_pvt;
};

struct pci_get_device_call_ctx {
	unsigned int vendor;
	unsigned int device;
	struct pci_dev* from;
};

struct pci_bus_read_config_dword_call_ctx {
	struct pci_bus* bus;
	unsigned int devfn;
	int pos;
	unsigned int* value;
};

void caller_marshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct notifier_call_call_ctx const* call_ctx,
	struct notifier_block const* ptr);

void callee_unmarshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct notifier_call_call_ctx const* call_ctx,
	struct notifier_block* ptr);

void callee_marshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct notifier_call_call_ctx const* call_ctx,
	struct notifier_block const* ptr);

void caller_unmarshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct notifier_call_call_ctx const* call_ctx,
	struct notifier_block* ptr);

void caller_marshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

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

void caller_marshal_kernel__pci_dev_get__ret_pci_dev__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_dev_get__ret_pci_dev__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_dev_get__ret_pci_dev__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_dev_get__ret_pci_dev__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_dev_get__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_dev_get__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_dev_get__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_dev_get__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_put_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_put_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_put_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_put_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__mce_unregister_decode_chain__notifier_block__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mce_unregister_decode_chain_call_ctx const* call_ctx,
	struct notifier_block const* ptr);

void callee_unmarshal_kernel__mce_unregister_decode_chain__notifier_block__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mce_unregister_decode_chain_call_ctx const* call_ctx,
	struct notifier_block* ptr);

void callee_marshal_kernel__mce_unregister_decode_chain__notifier_block__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mce_unregister_decode_chain_call_ctx const* call_ctx,
	struct notifier_block const* ptr);

void caller_unmarshal_kernel__mce_unregister_decode_chain__notifier_block__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mce_unregister_decode_chain_call_ctx const* call_ctx,
	struct notifier_block* ptr);

void caller_marshal_kernel__mce_register_decode_chain__notifier_block__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mce_register_decode_chain_call_ctx const* call_ctx,
	struct notifier_block const* ptr);

void callee_unmarshal_kernel__mce_register_decode_chain__notifier_block__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mce_register_decode_chain_call_ctx const* call_ctx,
	struct notifier_block* ptr);

void callee_marshal_kernel__mce_register_decode_chain__notifier_block__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mce_register_decode_chain_call_ctx const* call_ctx,
	struct notifier_block const* ptr);

void caller_unmarshal_kernel__mce_register_decode_chain__notifier_block__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mce_register_decode_chain_call_ctx const* call_ctx,
	struct notifier_block* ptr);

void caller_marshal_kernel__edac_mc_handle_error__mci__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* call_ctx,
	struct mem_ctl_info const* ptr);

void callee_unmarshal_kernel__edac_mc_handle_error__mci__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* call_ctx,
	struct mem_ctl_info* ptr);

void callee_marshal_kernel__edac_mc_handle_error__mci__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* call_ctx,
	struct mem_ctl_info const* ptr);

void caller_unmarshal_kernel__edac_mc_handle_error__mci__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* call_ctx,
	struct mem_ctl_info* ptr);

void caller_marshal_kernel__edac_mc_handle_error__layers__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* call_ctx,
	struct edac_mc_layer const* ptr);

void callee_unmarshal_kernel__edac_mc_handle_error__layers__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* call_ctx,
	struct edac_mc_layer* ptr);

void callee_marshal_kernel__edac_mc_handle_error__layers__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* call_ctx,
	struct edac_mc_layer const* ptr);

void caller_unmarshal_kernel__edac_mc_handle_error__layers__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* call_ctx,
	struct edac_mc_layer* ptr);

void caller_marshal_kernel__edac_mc_handle_error__dimms__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* call_ctx,
	struct dimm_info const* ptr);

void callee_unmarshal_kernel__edac_mc_handle_error__dimms__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* call_ctx,
	struct dimm_info* ptr);

void callee_marshal_kernel__edac_mc_handle_error__dimms__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* call_ctx,
	struct dimm_info const* ptr);

void caller_unmarshal_kernel__edac_mc_handle_error__dimms__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* call_ctx,
	struct dimm_info* ptr);

void caller_marshal_kernel__edac_mc_del_mc__ret_mem_ctl_info__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* call_ctx,
	struct mem_ctl_info const* ptr);

void callee_unmarshal_kernel__edac_mc_del_mc__ret_mem_ctl_info__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* call_ctx,
	struct mem_ctl_info* ptr);

void callee_marshal_kernel__edac_mc_del_mc__ret_mem_ctl_info__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* call_ctx,
	struct mem_ctl_info const* ptr);

void caller_unmarshal_kernel__edac_mc_del_mc__ret_mem_ctl_info__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* call_ctx,
	struct mem_ctl_info* ptr);

void caller_marshal_kernel__edac_mc_del_mc__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__edac_mc_del_mc__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__edac_mc_del_mc__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__edac_mc_del_mc__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__edac_mc_add_mc_with_groups__mci__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct mem_ctl_info const* ptr);

void callee_unmarshal_kernel__edac_mc_add_mc_with_groups__mci__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct mem_ctl_info* ptr);

void callee_marshal_kernel__edac_mc_add_mc_with_groups__mci__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct mem_ctl_info const* ptr);

void caller_unmarshal_kernel__edac_mc_add_mc_with_groups__mci__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct mem_ctl_info* ptr);

void caller_marshal_kernel__edac_mc_add_mc_with_groups__dimms__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct dimm_info const* ptr);

void callee_unmarshal_kernel__edac_mc_add_mc_with_groups__dimms__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct dimm_info* ptr);

void callee_marshal_kernel__edac_mc_add_mc_with_groups__dimms__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct dimm_info const* ptr);

void caller_unmarshal_kernel__edac_mc_add_mc_with_groups__dimms__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct dimm_info* ptr);

void caller_marshal_kernel__edac_mc_add_mc_with_groups__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__edac_mc_add_mc_with_groups__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__edac_mc_add_mc_with_groups__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__edac_mc_add_mc_with_groups__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__edac_mc_add_mc_with_groups__groups__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct attribute_group const* ptr);

void callee_unmarshal_kernel__edac_mc_add_mc_with_groups__groups__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct attribute_group* ptr);

void callee_marshal_kernel__edac_mc_add_mc_with_groups__groups__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct attribute_group const* ptr);

void caller_unmarshal_kernel__edac_mc_add_mc_with_groups__groups__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* call_ctx,
	struct attribute_group* ptr);

void caller_marshal_kernel__edac_mc_free__mci__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_free_call_ctx const* call_ctx,
	struct mem_ctl_info const* ptr);

void callee_unmarshal_kernel__edac_mc_free__mci__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_free_call_ctx const* call_ctx,
	struct mem_ctl_info* ptr);

void callee_marshal_kernel__edac_mc_free__mci__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_free_call_ctx const* call_ctx,
	struct mem_ctl_info const* ptr);

void caller_unmarshal_kernel__edac_mc_free__mci__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_free_call_ctx const* call_ctx,
	struct mem_ctl_info* ptr);

void caller_marshal_kernel__edac_mc_alloc__ret_mem_ctl_info__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct mem_ctl_info const* ptr);

void callee_unmarshal_kernel__edac_mc_alloc__ret_mem_ctl_info__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct mem_ctl_info* ptr);

void callee_marshal_kernel__edac_mc_alloc__ret_mem_ctl_info__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct mem_ctl_info const* ptr);

void caller_unmarshal_kernel__edac_mc_alloc__ret_mem_ctl_info__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct mem_ctl_info* ptr);

void caller_marshal_kernel__edac_mc_alloc__ret_layers__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct edac_mc_layer const* ptr);

void callee_unmarshal_kernel__edac_mc_alloc__ret_layers__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct edac_mc_layer* ptr);

void callee_marshal_kernel__edac_mc_alloc__ret_layers__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct edac_mc_layer const* ptr);

void caller_unmarshal_kernel__edac_mc_alloc__ret_layers__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct edac_mc_layer* ptr);

void caller_marshal_kernel__edac_mc_alloc__ret_dimms__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct dimm_info const* ptr);

void callee_unmarshal_kernel__edac_mc_alloc__ret_dimms__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct dimm_info* ptr);

void callee_marshal_kernel__edac_mc_alloc__ret_dimms__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct dimm_info const* ptr);

void caller_unmarshal_kernel__edac_mc_alloc__ret_dimms__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct dimm_info* ptr);

void caller_marshal_kernel__edac_mc_alloc__layers__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct edac_mc_layer const* ptr);

void callee_unmarshal_kernel__edac_mc_alloc__layers__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct edac_mc_layer* ptr);

void callee_marshal_kernel__edac_mc_alloc__layers__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct edac_mc_layer const* ptr);

void caller_unmarshal_kernel__edac_mc_alloc__layers__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* call_ctx,
	struct edac_mc_layer* ptr);

void caller_marshal_kernel__pci_get_device__ret_pci_dev__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_get_device__ret_pci_dev__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_get_device__ret_pci_dev__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_get_device__ret_pci_dev__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_get_device__bus__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_device_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pci_get_device__bus__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_device_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pci_get_device__bus__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_device_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pci_get_device__bus__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_device_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__pci_get_device__from__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_get_device__from__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_get_device__from__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_get_device__from__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_bus_read_config_dword__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_bus_read_config_dword_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pci_bus_read_config_dword__bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_bus_read_config_dword_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pci_bus_read_config_dword__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_bus_read_config_dword_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pci_bus_read_config_dword__bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_bus_read_config_dword_call_ctx const* call_ctx,
	struct pci_bus* ptr);


#endif
