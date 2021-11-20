#ifndef COMMON_H
#define COMMON_H

#include <liblcd/trampoline.h>
#include <libfipc.h>
#include <liblcd/boot_info.h>
#include <asm/cacheflush.h>
#include <lcd_domains/microkernel.h>
#include <liblcd/liblcd.h>

#include "glue_user.h"

#define DEFAULT_GFP_FLAGS  (GFP_KERNEL)
#define verbose_debug 1
#define glue_pack(pos, msg, ext, value) glue_pack_impl((pos), (msg), (ext), (uint64_t)(value))
#define glue_pack_shadow(pos, msg, ext, value) glue_pack_shadow_impl((pos), (msg), (ext), (value))
#define glue_unpack(pos, msg, ext, type) (type)glue_unpack_impl((pos), (msg), (ext))
#define glue_unpack_shadow(pos, msg, ext, type) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack shadow for type %s\n", __func__, __LINE__, __stringify(type)); \
	(type)glue_unpack_shadow_impl(glue_unpack(pos, msg, ext, void*)); })

#define glue_unpack_new_shadow(pos, msg, ext, type, size, flags) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack new shadow for type %s | size %llu\n", __func__, __LINE__, __stringify(type), (uint64_t) size); \
	(type)glue_unpack_new_shadow_impl(glue_unpack(pos, msg, ext, void*), size, flags); })

#define glue_unpack_bind_or_new_shadow(pos, msg, ext, type, size, flags) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack or bind new shadow for type %s | size %llu\n", __func__, __LINE__, __stringify(type), (uint64_t) size); \
	(type)glue_unpack_bind_or_new_shadow_impl(glue_unpack(pos, msg, ext, void*), size, flags); })

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
void* glue_user_alloc(size_t size, gfp_t flags);
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
	if (*pos >= 512)
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

static inline void* glue_unpack_new_shadow_impl(const void* ptr, size_t size, gfp_t flags)
{
	void* shadow = 0;
	if (!ptr)
		return NULL;

	shadow = glue_user_alloc(size, flags);
	glue_user_add_shadow(ptr, shadow);
	return shadow;
}

static inline void* glue_unpack_bind_or_new_shadow_impl(const void* ptr, size_t size, gfp_t flags)
{
	void* shadow = 0;
	if (!ptr)
		return NULL;

	shadow = glue_user_map_to_shadow(ptr, false);
	if (!shadow) {
		shadow = glue_user_alloc(size, flags);
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
	RPC_ID_add_timer,
	RPC_ID_complete,
	RPC_ID_del_timer,
	RPC_ID_del_timer_sync,
	RPC_ID_free_irq,
	RPC_ID_init_timer_key,
	RPC_ID_mod_timer,
	RPC_ID_pci_disable_msi,
	RPC_ID_pci_disable_msix,
	RPC_ID_pci_enable_msi_range,
	RPC_ID_pci_enable_msix_range,
	RPC_ID_pci_set_power_state,
	RPC_ID_thread_fn,
	RPC_ID_handler,
	RPC_ID_request_threaded_irq,
	RPC_ID_wait_for_completion,
	RPC_ID_wait_for_completion_timeout,
	RPC_ID_get_quirks,
	RPC_ID_xhci_gen_setup,
	RPC_ID_xhci_init_driver,
	RPC_ID_xhci_run,
	RPC_ID_usb_disable_xhci_ports,
	RPC_ID_dmi_get_system_info,
	RPC_ID_usb_acpi_power_manageable,
	RPC_ID_usb_acpi_set_power_state,
	RPC_ID_usb_disabled,
	RPC_ID_usb_hc_died,
	RPC_ID_usb_hcd_check_unlink_urb,
	RPC_ID_usb_hcd_giveback_urb,
	RPC_ID_usb_hcd_irq,
	RPC_ID_usb_hcd_is_primary_hcd,
	RPC_ID_usb_hcd_link_urb_to_ep,
	RPC_ID_usb_hcd_poll_rh_status,
	RPC_ID_usb_hcd_unlink_urb_from_ep,
	RPC_ID_usb_wakeup_notification,
	RPC_ID_hc_driver_start,
	RPC_ID_hc_driver_enable_device,
	RPC_ID_hc_driver_update_hub_device,
	RPC_ID_hc_driver_reset_device,
	RPC_ID_hc_driver_update_device,
	RPC_ID_hc_driver_set_usb2_hw_lpm,
	RPC_ID_hc_driver_enable_usb3_lpm_timeout,
	RPC_ID_hc_driver_disable_usb3_lpm_timeout,
	RPC_ID_hc_driver_find_raw_port_number,
	RPC_ID_hc_driver_stop,
	RPC_ID_hc_driver_shutdown,
	RPC_ID_hc_driver_get_frame_number,
	RPC_ID_hc_driver_urb_enqueue,
	RPC_ID_hc_driver_urb_dequeue,
	RPC_ID_hc_driver_endpoint_reset,
	RPC_ID_hc_driver_alloc_dev,
	RPC_ID_hc_driver_free_dev,
	RPC_ID_hc_driver_alloc_streams,
	RPC_ID_hc_driver_free_streams,
	RPC_ID_hc_driver_add_endpoint,
	RPC_ID_hc_driver_drop_endpoint,
	RPC_ID_hc_driver_check_bandwidth,
	RPC_ID_hc_driver_reset_bandwidth,
	RPC_ID_hc_driver_address_device,
	RPC_ID_hc_driver_hub_status_data,
	RPC_ID_hc_driver_irq,
	RPC_ID_hc_driver_hub_control,
	RPC_ID_get_loops_per_jiffy,
	RPC_ID_get_jiffies,
};

int try_dispatch(enum RPC_ID id, struct fipc_message* __msg, struct ext_registers* __ext);

typedef unsigned int (*fptr_thread_fn)(int irq, void* id);
typedef unsigned int (*fptr_impl_thread_fn)(fptr_thread_fn target, int irq, void* id);

LCD_TRAMPOLINE_DATA(trmp_thread_fn)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_thread_fn) trmp_thread_fn(int irq, void* id);

typedef unsigned int (*fptr_handler)(int irq, void* id);
typedef unsigned int (*fptr_impl_handler)(fptr_handler target, int irq, void* id);

LCD_TRAMPOLINE_DATA(trmp_handler)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_handler) trmp_handler(int irq, void* id);

typedef void (*fptr_get_quirks)(struct device* dev, struct xhci_hcd* xhci_hcd);
typedef void (*fptr_impl_get_quirks)(fptr_get_quirks target, struct device* dev, struct xhci_hcd* xhci_hcd);

LCD_TRAMPOLINE_DATA(trmp_get_quirks)
void LCD_TRAMPOLINE_LINKAGE(trmp_get_quirks) trmp_get_quirks(struct device* dev, struct xhci_hcd* xhci_hcd);

typedef int (*fptr_hc_driver_start)(struct usb_hcd* hcd);
typedef int (*fptr_impl_hc_driver_start)(fptr_hc_driver_start target, struct usb_hcd* hcd);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_start)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_start) trmp_hc_driver_start(struct usb_hcd* hcd);

typedef int (*fptr_hc_driver_enable_device)(struct usb_hcd* hcd, struct usb_device* udev);
typedef int (*fptr_impl_hc_driver_enable_device)(fptr_hc_driver_enable_device target, struct usb_hcd* hcd, struct usb_device* udev);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_enable_device)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_enable_device) trmp_hc_driver_enable_device(struct usb_hcd* hcd, struct usb_device* udev);

typedef int (*fptr_hc_driver_update_hub_device)(struct usb_hcd* hcd, struct usb_device* hdev, struct usb_tt* tt, unsigned int mem_flags);
typedef int (*fptr_impl_hc_driver_update_hub_device)(fptr_hc_driver_update_hub_device target, struct usb_hcd* hcd, struct usb_device* hdev, struct usb_tt* tt, unsigned int mem_flags);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_update_hub_device)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_update_hub_device) trmp_hc_driver_update_hub_device(struct usb_hcd* hcd, struct usb_device* hdev, struct usb_tt* tt, unsigned int mem_flags);

typedef int (*fptr_hc_driver_reset_device)(struct usb_hcd* hcd, struct usb_device* udev);
typedef int (*fptr_impl_hc_driver_reset_device)(fptr_hc_driver_reset_device target, struct usb_hcd* hcd, struct usb_device* udev);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_reset_device)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_reset_device) trmp_hc_driver_reset_device(struct usb_hcd* hcd, struct usb_device* udev);

typedef int (*fptr_hc_driver_update_device)(struct usb_hcd* hcd, struct usb_device* udev);
typedef int (*fptr_impl_hc_driver_update_device)(fptr_hc_driver_update_device target, struct usb_hcd* hcd, struct usb_device* udev);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_update_device)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_update_device) trmp_hc_driver_update_device(struct usb_hcd* hcd, struct usb_device* udev);

typedef int (*fptr_hc_driver_set_usb2_hw_lpm)(struct usb_hcd* hcd, struct usb_device* udev, int enable);
typedef int (*fptr_impl_hc_driver_set_usb2_hw_lpm)(fptr_hc_driver_set_usb2_hw_lpm target, struct usb_hcd* hcd, struct usb_device* udev, int enable);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_set_usb2_hw_lpm)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_set_usb2_hw_lpm) trmp_hc_driver_set_usb2_hw_lpm(struct usb_hcd* hcd, struct usb_device* udev, int enable);

typedef int (*fptr_hc_driver_enable_usb3_lpm_timeout)(struct usb_hcd* hcd, struct usb_device* udev, unsigned int state);
typedef int (*fptr_impl_hc_driver_enable_usb3_lpm_timeout)(fptr_hc_driver_enable_usb3_lpm_timeout target, struct usb_hcd* hcd, struct usb_device* udev, unsigned int state);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_enable_usb3_lpm_timeout)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_enable_usb3_lpm_timeout) trmp_hc_driver_enable_usb3_lpm_timeout(struct usb_hcd* hcd, struct usb_device* udev, unsigned int state);

typedef int (*fptr_hc_driver_disable_usb3_lpm_timeout)(struct usb_hcd* hcd, struct usb_device* udev, unsigned int state);
typedef int (*fptr_impl_hc_driver_disable_usb3_lpm_timeout)(fptr_hc_driver_disable_usb3_lpm_timeout target, struct usb_hcd* hcd, struct usb_device* udev, unsigned int state);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_disable_usb3_lpm_timeout)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_disable_usb3_lpm_timeout) trmp_hc_driver_disable_usb3_lpm_timeout(struct usb_hcd* hcd, struct usb_device* udev, unsigned int state);

typedef int (*fptr_hc_driver_find_raw_port_number)(struct usb_hcd* hcd, int port1);
typedef int (*fptr_impl_hc_driver_find_raw_port_number)(fptr_hc_driver_find_raw_port_number target, struct usb_hcd* hcd, int port1);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_find_raw_port_number)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_find_raw_port_number) trmp_hc_driver_find_raw_port_number(struct usb_hcd* hcd, int port1);

typedef void (*fptr_hc_driver_stop)(struct usb_hcd* hcd);
typedef void (*fptr_impl_hc_driver_stop)(fptr_hc_driver_stop target, struct usb_hcd* hcd);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_stop)
void LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_stop) trmp_hc_driver_stop(struct usb_hcd* hcd);

typedef void (*fptr_hc_driver_shutdown)(struct usb_hcd* hcd);
typedef void (*fptr_impl_hc_driver_shutdown)(fptr_hc_driver_shutdown target, struct usb_hcd* hcd);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_shutdown)
void LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_shutdown) trmp_hc_driver_shutdown(struct usb_hcd* hcd);

typedef int (*fptr_hc_driver_get_frame_number)(struct usb_hcd* hcd);
typedef int (*fptr_impl_hc_driver_get_frame_number)(fptr_hc_driver_get_frame_number target, struct usb_hcd* hcd);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_get_frame_number)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_get_frame_number) trmp_hc_driver_get_frame_number(struct usb_hcd* hcd);

typedef int (*fptr_hc_driver_urb_enqueue)(struct usb_hcd* hcd, struct urb* urb, unsigned int mem_flags);
typedef int (*fptr_impl_hc_driver_urb_enqueue)(fptr_hc_driver_urb_enqueue target, struct usb_hcd* hcd, struct urb* urb, unsigned int mem_flags);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_urb_enqueue)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_urb_enqueue) trmp_hc_driver_urb_enqueue(struct usb_hcd* hcd, struct urb* urb, unsigned int mem_flags);

typedef int (*fptr_hc_driver_urb_dequeue)(struct usb_hcd* hcd, struct urb* urb, int status);
typedef int (*fptr_impl_hc_driver_urb_dequeue)(fptr_hc_driver_urb_dequeue target, struct usb_hcd* hcd, struct urb* urb, int status);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_urb_dequeue)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_urb_dequeue) trmp_hc_driver_urb_dequeue(struct usb_hcd* hcd, struct urb* urb, int status);

typedef void (*fptr_hc_driver_endpoint_reset)(struct usb_hcd* hcd, struct usb_host_endpoint* ep);
typedef void (*fptr_impl_hc_driver_endpoint_reset)(fptr_hc_driver_endpoint_reset target, struct usb_hcd* hcd, struct usb_host_endpoint* ep);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_endpoint_reset)
void LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_endpoint_reset) trmp_hc_driver_endpoint_reset(struct usb_hcd* hcd, struct usb_host_endpoint* ep);

typedef int (*fptr_hc_driver_alloc_dev)(struct usb_hcd* hcd, struct usb_device* udev);
typedef int (*fptr_impl_hc_driver_alloc_dev)(fptr_hc_driver_alloc_dev target, struct usb_hcd* hcd, struct usb_device* udev);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_alloc_dev)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_alloc_dev) trmp_hc_driver_alloc_dev(struct usb_hcd* hcd, struct usb_device* udev);

typedef void (*fptr_hc_driver_free_dev)(struct usb_hcd* hcd, struct usb_device* udev);
typedef void (*fptr_impl_hc_driver_free_dev)(fptr_hc_driver_free_dev target, struct usb_hcd* hcd, struct usb_device* udev);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_free_dev)
void LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_free_dev) trmp_hc_driver_free_dev(struct usb_hcd* hcd, struct usb_device* udev);

typedef int (*fptr_hc_driver_alloc_streams)(struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint** eps, unsigned int num_eps, unsigned int num_streams, unsigned int mem_flags);
typedef int (*fptr_impl_hc_driver_alloc_streams)(fptr_hc_driver_alloc_streams target, struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint** eps, unsigned int num_eps, unsigned int num_streams, unsigned int mem_flags);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_alloc_streams)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_alloc_streams) trmp_hc_driver_alloc_streams(struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint** eps, unsigned int num_eps, unsigned int num_streams, unsigned int mem_flags);

typedef int (*fptr_hc_driver_free_streams)(struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint** eps, unsigned int num_eps, unsigned int mem_flags);
typedef int (*fptr_impl_hc_driver_free_streams)(fptr_hc_driver_free_streams target, struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint** eps, unsigned int num_eps, unsigned int mem_flags);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_free_streams)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_free_streams) trmp_hc_driver_free_streams(struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint** eps, unsigned int num_eps, unsigned int mem_flags);

typedef int (*fptr_hc_driver_add_endpoint)(struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint* ep);
typedef int (*fptr_impl_hc_driver_add_endpoint)(fptr_hc_driver_add_endpoint target, struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint* ep);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_add_endpoint)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_add_endpoint) trmp_hc_driver_add_endpoint(struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint* ep);

typedef int (*fptr_hc_driver_drop_endpoint)(struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint* ep);
typedef int (*fptr_impl_hc_driver_drop_endpoint)(fptr_hc_driver_drop_endpoint target, struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint* ep);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_drop_endpoint)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_drop_endpoint) trmp_hc_driver_drop_endpoint(struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint* ep);

typedef int (*fptr_hc_driver_check_bandwidth)(struct usb_hcd* hcd, struct usb_device* udev);
typedef int (*fptr_impl_hc_driver_check_bandwidth)(fptr_hc_driver_check_bandwidth target, struct usb_hcd* hcd, struct usb_device* udev);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_check_bandwidth)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_check_bandwidth) trmp_hc_driver_check_bandwidth(struct usb_hcd* hcd, struct usb_device* udev);

typedef void (*fptr_hc_driver_reset_bandwidth)(struct usb_hcd* hcd, struct usb_device* udev);
typedef void (*fptr_impl_hc_driver_reset_bandwidth)(fptr_hc_driver_reset_bandwidth target, struct usb_hcd* hcd, struct usb_device* udev);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_reset_bandwidth)
void LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_reset_bandwidth) trmp_hc_driver_reset_bandwidth(struct usb_hcd* hcd, struct usb_device* udev);

typedef int (*fptr_hc_driver_address_device)(struct usb_hcd* hcd, struct usb_device* udev);
typedef int (*fptr_impl_hc_driver_address_device)(fptr_hc_driver_address_device target, struct usb_hcd* hcd, struct usb_device* udev);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_address_device)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_address_device) trmp_hc_driver_address_device(struct usb_hcd* hcd, struct usb_device* udev);

typedef int (*fptr_hc_driver_hub_status_data)(struct usb_hcd* hcd, char* buf);
typedef int (*fptr_impl_hc_driver_hub_status_data)(fptr_hc_driver_hub_status_data target, struct usb_hcd* hcd, char* buf);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_hub_status_data)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_hub_status_data) trmp_hc_driver_hub_status_data(struct usb_hcd* hcd, char* buf);

typedef unsigned int (*fptr_hc_driver_irq)(struct usb_hcd* hcd);
typedef unsigned int (*fptr_impl_hc_driver_irq)(fptr_hc_driver_irq target, struct usb_hcd* hcd);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_irq)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_irq) trmp_hc_driver_irq(struct usb_hcd* hcd);

typedef int (*fptr_hc_driver_hub_control)(struct usb_hcd* hcd, unsigned short typeReq, unsigned short wValue, unsigned short wIndex, char* buf, unsigned short wLength);
typedef int (*fptr_impl_hc_driver_hub_control)(fptr_hc_driver_hub_control target, struct usb_hcd* hcd, unsigned short typeReq, unsigned short wValue, unsigned short wIndex, char* buf, unsigned short wLength);

LCD_TRAMPOLINE_DATA(trmp_hc_driver_hub_control)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_hub_control) trmp_hc_driver_hub_control(struct usb_hcd* hcd, unsigned short typeReq, unsigned short wValue, unsigned short wIndex, char* buf, unsigned short wLength);

struct add_timer_call_ctx {
	struct timer_list* timer;
};

struct complete_call_ctx {
	struct completion* x;
};

struct del_timer_call_ctx {
	struct timer_list* timer;
};

struct del_timer_sync_call_ctx {
	struct timer_list* timer;
};

struct free_irq_call_ctx {
	unsigned int irq;
	void* dev_id;
};

struct init_timer_key_call_ctx {
	struct timer_list* timer;
	unsigned int flags;
	const char* name;
	struct lock_class_key* key;
};

struct get_loops_per_jiffy_call_ctx {
};

struct get_jiffies_call_ctx {
};

struct mod_timer_call_ctx {
	struct timer_list* timer;
	unsigned long expires;
};

struct pci_disable_msi_call_ctx {
	struct pci_dev* dev;
};

struct pci_disable_msix_call_ctx {
	struct pci_dev* dev;
};

struct pci_enable_msi_range_call_ctx {
	struct pci_dev* dev;
	int minvec;
	int maxvec;
};

struct pci_enable_msix_range_call_ctx {
	struct pci_dev* dev;
	struct msix_entry* entries;
	int minvec;
	int maxvec;
};

struct pci_set_power_state_call_ctx {
	struct pci_dev* dev;
	int state;
};

struct thread_fn_call_ctx {
	int irq;
	void* id;
};

struct handler_call_ctx {
	int irq;
	void* id;
};

struct request_threaded_irq_call_ctx {
	unsigned int irq;
	fptr_handler handler;
	fptr_thread_fn thread_fn;
	unsigned long irqflags;
	const char* devname;
	void* dev_id;
};

struct wait_for_completion_call_ctx {
	struct completion* x;
};

struct wait_for_completion_timeout_call_ctx {
	struct completion* x;
	unsigned long timeout;
};

struct get_quirks_call_ctx {
	struct device* dev;
	struct xhci_hcd* xhci_hcd;
};

struct xhci_gen_setup_call_ctx {
	struct usb_hcd* hcd;
	fptr_get_quirks get_quirks;
};

struct xhci_init_driver_call_ctx {
	struct hc_driver* drv;
	const struct xhci_driver_overrides* over;
};

struct xhci_run_call_ctx {
	struct usb_hcd* hcd;
};

struct usb_disable_xhci_ports_call_ctx {
	struct pci_dev* xhci_pdev;
};

struct dmi_get_system_info_call_ctx {
	int field;
};

struct usb_acpi_power_manageable_call_ctx {
	struct usb_device* hdev;
	int index;
};

struct usb_acpi_set_power_state_call_ctx {
	struct usb_device* hdev;
	int index;
	bool enable;
};

struct usb_disabled_call_ctx {
};

struct usb_hc_died_call_ctx {
	struct usb_hcd* hcd;
};

struct usb_hcd_check_unlink_urb_call_ctx {
	struct usb_hcd* hcd;
	struct urb* urb;
	int status;
};

struct usb_hcd_giveback_urb_call_ctx {
	struct usb_hcd* hcd;
	struct urb* urb;
	int status;
};

struct usb_hcd_irq_call_ctx {
	int irq;
	void* __hcd;
};

struct usb_hcd_is_primary_hcd_call_ctx {
	struct usb_hcd* hcd;
};

struct usb_hcd_link_urb_to_ep_call_ctx {
	struct usb_hcd* hcd;
	struct urb* urb;
};

struct usb_hcd_poll_rh_status_call_ctx {
	struct usb_hcd* hcd;
};

struct usb_hcd_unlink_urb_from_ep_call_ctx {
	struct usb_hcd* hcd;
	struct urb* urb;
};

struct usb_wakeup_notification_call_ctx {
	struct usb_device* hdev;
	unsigned int portnum;
};

struct hc_driver_start_call_ctx {
	struct usb_hcd* hcd;
};

struct hc_driver_enable_device_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
};

struct hc_driver_update_hub_device_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* hdev;
	struct usb_tt* tt;
	unsigned int mem_flags;
};

struct hc_driver_reset_device_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
};

struct hc_driver_update_device_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
};

struct hc_driver_set_usb2_hw_lpm_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
	int enable;
};

struct hc_driver_enable_usb3_lpm_timeout_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
	unsigned int state;
};

struct hc_driver_disable_usb3_lpm_timeout_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
	unsigned int state;
};

struct hc_driver_find_raw_port_number_call_ctx {
	struct usb_hcd* hcd;
	int port1;
};

struct hc_driver_stop_call_ctx {
	struct usb_hcd* hcd;
};

struct hc_driver_shutdown_call_ctx {
	struct usb_hcd* hcd;
};

struct hc_driver_get_frame_number_call_ctx {
	struct usb_hcd* hcd;
};

struct hc_driver_urb_enqueue_call_ctx {
	struct usb_hcd* hcd;
	struct urb* urb;
	unsigned int mem_flags;
};

struct hc_driver_urb_dequeue_call_ctx {
	struct usb_hcd* hcd;
	struct urb* urb;
	int status;
};

struct hc_driver_endpoint_reset_call_ctx {
	struct usb_hcd* hcd;
	struct usb_host_endpoint* ep;
};

struct hc_driver_alloc_dev_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
};

struct hc_driver_free_dev_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
};

struct hc_driver_alloc_streams_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
	struct usb_host_endpoint** eps;
	unsigned int num_eps;
	unsigned int num_streams;
	unsigned int mem_flags;
};

struct hc_driver_free_streams_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
	struct usb_host_endpoint** eps;
	unsigned int num_eps;
	unsigned int mem_flags;
};

struct hc_driver_add_endpoint_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
	struct usb_host_endpoint* ep;
};

struct hc_driver_drop_endpoint_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
	struct usb_host_endpoint* ep;
};

struct hc_driver_check_bandwidth_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
};

struct hc_driver_reset_bandwidth_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
};

struct hc_driver_address_device_call_ctx {
	struct usb_hcd* hcd;
	struct usb_device* udev;
};

struct hc_driver_hub_status_data_call_ctx {
	struct usb_hcd* hcd;
	char* buf;
};

struct hc_driver_irq_call_ctx {
	struct usb_hcd* hcd;
};

struct hc_driver_hub_control_call_ctx {
	struct usb_hcd* hcd;
	unsigned short typeReq;
	unsigned short wValue;
	unsigned short wIndex;
	char* buf;
	unsigned short wLength;
};

void caller_marshal_kernel__add_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct add_timer_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void callee_unmarshal_kernel__add_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct add_timer_call_ctx const* call_ctx,
	struct timer_list* ptr);

void callee_marshal_kernel__add_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct add_timer_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void caller_unmarshal_kernel__add_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct add_timer_call_ctx const* call_ctx,
	struct timer_list* ptr);

void caller_marshal_kernel__add_timer__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct add_timer_call_ctx const* call_ctx,
	struct hlist_node const* ptr);

void callee_unmarshal_kernel__add_timer__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct add_timer_call_ctx const* call_ctx,
	struct hlist_node* ptr);

void callee_marshal_kernel__add_timer__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct add_timer_call_ctx const* call_ctx,
	struct hlist_node const* ptr);

void caller_unmarshal_kernel__add_timer__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct add_timer_call_ctx const* call_ctx,
	struct hlist_node* ptr);

void caller_marshal_kernel__complete__x__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct complete_call_ctx const* call_ctx,
	struct completion const* ptr);

void callee_unmarshal_kernel__complete__x__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct complete_call_ctx const* call_ctx,
	struct completion* ptr);

void callee_marshal_kernel__complete__x__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct complete_call_ctx const* call_ctx,
	struct completion const* ptr);

void caller_unmarshal_kernel__complete__x__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct complete_call_ctx const* call_ctx,
	struct completion* ptr);

void caller_marshal_kernel__del_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void callee_unmarshal_kernel__del_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_call_ctx const* call_ctx,
	struct timer_list* ptr);

void callee_marshal_kernel__del_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void caller_unmarshal_kernel__del_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_call_ctx const* call_ctx,
	struct timer_list* ptr);

void caller_marshal_kernel__del_timer__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_call_ctx const* call_ctx,
	struct hlist_node const* ptr);

void callee_unmarshal_kernel__del_timer__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_call_ctx const* call_ctx,
	struct hlist_node* ptr);

void callee_marshal_kernel__del_timer__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_call_ctx const* call_ctx,
	struct hlist_node const* ptr);

void caller_unmarshal_kernel__del_timer__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_call_ctx const* call_ctx,
	struct hlist_node* ptr);

void caller_marshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void callee_unmarshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* call_ctx,
	struct timer_list* ptr);

void callee_marshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void caller_unmarshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* call_ctx,
	struct timer_list* ptr);

void caller_marshal_kernel__del_timer_sync__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* call_ctx,
	struct hlist_node const* ptr);

void callee_unmarshal_kernel__del_timer_sync__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* call_ctx,
	struct hlist_node* ptr);

void callee_marshal_kernel__del_timer_sync__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* call_ctx,
	struct hlist_node const* ptr);

void caller_unmarshal_kernel__del_timer_sync__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* call_ctx,
	struct hlist_node* ptr);

void caller_marshal_kernel__init_timer_key__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void callee_unmarshal_kernel__init_timer_key__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* call_ctx,
	struct timer_list* ptr);

void callee_marshal_kernel__init_timer_key__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void caller_unmarshal_kernel__init_timer_key__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* call_ctx,
	struct timer_list* ptr);

void caller_marshal_kernel__init_timer_key__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* call_ctx,
	struct hlist_node const* ptr);

void callee_unmarshal_kernel__init_timer_key__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* call_ctx,
	struct hlist_node* ptr);

void callee_marshal_kernel__init_timer_key__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* call_ctx,
	struct hlist_node const* ptr);

void caller_unmarshal_kernel__init_timer_key__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* call_ctx,
	struct hlist_node* ptr);

void caller_marshal_kernel__init_timer_key__key__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* call_ctx,
	struct lock_class_key const* ptr);

void callee_unmarshal_kernel__init_timer_key__key__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* call_ctx,
	struct lock_class_key* ptr);

void callee_marshal_kernel__init_timer_key__key__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* call_ctx,
	struct lock_class_key const* ptr);

void caller_unmarshal_kernel__init_timer_key__key__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* call_ctx,
	struct lock_class_key* ptr);

void caller_marshal_kernel__mod_timer__timer__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void callee_unmarshal_kernel__mod_timer__timer__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* call_ctx,
	struct timer_list* ptr);

void callee_marshal_kernel__mod_timer__timer__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void caller_unmarshal_kernel__mod_timer__timer__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* call_ctx,
	struct timer_list* ptr);

void caller_marshal_kernel__mod_timer__hlist_node__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* call_ctx,
	struct hlist_node const* ptr);

void callee_unmarshal_kernel__mod_timer__hlist_node__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* call_ctx,
	struct hlist_node* ptr);

void callee_marshal_kernel__mod_timer__hlist_node__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* call_ctx,
	struct hlist_node const* ptr);

void caller_unmarshal_kernel__mod_timer__hlist_node__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* call_ctx,
	struct hlist_node* ptr);

void caller_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_enable_msix_range__entries__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct msix_entry const* ptr);

void callee_unmarshal_kernel__pci_enable_msix_range__entries__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct msix_entry* ptr);

void callee_marshal_kernel__pci_enable_msix_range__entries__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct msix_entry const* ptr);

void caller_unmarshal_kernel__pci_enable_msix_range__entries__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct msix_entry* ptr);

void caller_marshal_kernel__pci_set_power_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_power_state_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_set_power_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_power_state_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_set_power_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_power_state_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_set_power_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_power_state_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__wait_for_completion__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct wait_for_completion_call_ctx const* call_ctx,
	struct completion const* ptr);

void callee_unmarshal_kernel__wait_for_completion__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct wait_for_completion_call_ctx const* call_ctx,
	struct completion* ptr);

void callee_marshal_kernel__wait_for_completion__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct wait_for_completion_call_ctx const* call_ctx,
	struct completion const* ptr);

void caller_unmarshal_kernel__wait_for_completion__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct wait_for_completion_call_ctx const* call_ctx,
	struct completion* ptr);

void caller_marshal_kernel__wait_for_completion_timeout__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct wait_for_completion_timeout_call_ctx const* call_ctx,
	struct completion const* ptr);

void callee_unmarshal_kernel__wait_for_completion_timeout__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct wait_for_completion_timeout_call_ctx const* call_ctx,
	struct completion* ptr);

void callee_marshal_kernel__wait_for_completion_timeout__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct wait_for_completion_timeout_call_ctx const* call_ctx,
	struct completion const* ptr);

void caller_unmarshal_kernel__wait_for_completion_timeout__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct wait_for_completion_timeout_call_ctx const* call_ctx,
	struct completion* ptr);

void caller_marshal_kernel__get_quirks__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_quirks_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__get_quirks__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_quirks_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__get_quirks__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_quirks_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__get_quirks__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_quirks_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__get_quirks__xhci_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_quirks_call_ctx const* call_ctx,
	struct xhci_hcd const* ptr);

void callee_unmarshal_kernel__get_quirks__xhci_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_quirks_call_ctx const* call_ctx,
	struct xhci_hcd* ptr);

void callee_marshal_kernel__get_quirks__xhci_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_quirks_call_ctx const* call_ctx,
	struct xhci_hcd const* ptr);

void caller_unmarshal_kernel__get_quirks__xhci_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_quirks_call_ctx const* call_ctx,
	struct xhci_hcd* ptr);

void caller_marshal_kernel__xhci_gen_setup__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__xhci_gen_setup__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__xhci_gen_setup__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__xhci_gen_setup__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel___global_hc_driver__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver const* ptr);

void callee_unmarshal_kernel___global_hc_driver__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver* ptr);

void callee_marshal_kernel___global_hc_driver__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver const* ptr);

void caller_unmarshal_kernel___global_hc_driver__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver* ptr);

void caller_marshal_kernel__xhci_init_driver__over__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* call_ctx,
	struct xhci_driver_overrides const* ptr);

void callee_unmarshal_kernel__xhci_init_driver__over__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* call_ctx,
	struct xhci_driver_overrides* ptr);

void callee_marshal_kernel__xhci_init_driver__over__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* call_ctx,
	struct xhci_driver_overrides const* ptr);

void caller_unmarshal_kernel__xhci_init_driver__over__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* call_ctx,
	struct xhci_driver_overrides* ptr);

void caller_marshal_kernel__xhci_run__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_run_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__xhci_run__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_run_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__xhci_run__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_run_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__xhci_run__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_run_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__usb_disable_xhci_ports__pci_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_disable_xhci_ports_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__usb_disable_xhci_ports__pci_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_disable_xhci_ports_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__usb_disable_xhci_ports__pci_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_disable_xhci_ports_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__usb_disable_xhci_ports__pci_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_disable_xhci_ports_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__usb_acpi_power_manageable__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_acpi_power_manageable_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__usb_acpi_power_manageable__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_acpi_power_manageable_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__usb_acpi_power_manageable__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_acpi_power_manageable_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__usb_acpi_power_manageable__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_acpi_power_manageable_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__usb_acpi_set_power_state__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_acpi_set_power_state_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__usb_acpi_set_power_state__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_acpi_set_power_state_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__usb_acpi_set_power_state__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_acpi_set_power_state_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__usb_acpi_set_power_state__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_acpi_set_power_state_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__usb_hc_died__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__usb_hc_died__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__usb_hc_died__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__usb_hc_died__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__usb_hc_died__usb_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* call_ctx,
	struct usb_bus const* ptr);

void callee_unmarshal_kernel__usb_hc_died__usb_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* call_ctx,
	struct usb_bus* ptr);

void callee_marshal_kernel__usb_hc_died__usb_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* call_ctx,
	struct usb_bus const* ptr);

void caller_unmarshal_kernel__usb_hc_died__usb_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* call_ctx,
	struct usb_bus* ptr);

void caller_marshal_kernel__usb_hc_died__usb_bus_root_hub__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__usb_hc_died__usb_bus_root_hub__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__usb_hc_died__usb_bus_root_hub__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__usb_hc_died__usb_bus_root_hub__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__usb_hcd_check_unlink_urb__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__usb_hcd_check_unlink_urb__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__usb_hcd_check_unlink_urb__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__usb_hcd_check_unlink_urb__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__usb_hcd_check_unlink_urb__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* call_ctx,
	struct urb const* ptr);

void callee_unmarshal_kernel__usb_hcd_check_unlink_urb__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* call_ctx,
	struct urb* ptr);

void callee_marshal_kernel__usb_hcd_check_unlink_urb__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* call_ctx,
	struct urb const* ptr);

void caller_unmarshal_kernel__usb_hcd_check_unlink_urb__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* call_ctx,
	struct urb* ptr);

void caller_marshal_kernel__usb_hcd_check_unlink_urb__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void callee_unmarshal_kernel__usb_hcd_check_unlink_urb__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void callee_marshal_kernel__usb_hcd_check_unlink_urb__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void caller_unmarshal_kernel__usb_hcd_check_unlink_urb__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void caller_marshal_kernel__usb_hcd_giveback_urb__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__usb_hcd_giveback_urb__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__usb_hcd_giveback_urb__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__usb_hcd_giveback_urb__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__usb_hcd_giveback_urb__usb_hcd_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct hc_driver const* ptr);

void callee_unmarshal_kernel__usb_hcd_giveback_urb__usb_hcd_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct hc_driver* ptr);

void callee_marshal_kernel__usb_hcd_giveback_urb__usb_hcd_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct hc_driver const* ptr);

void caller_unmarshal_kernel__usb_hcd_giveback_urb__usb_hcd_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct hc_driver* ptr);

void caller_marshal_kernel__usb_hcd_giveback_urb__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct urb const* ptr);

void callee_unmarshal_kernel__usb_hcd_giveback_urb__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct urb* ptr);

void callee_marshal_kernel__usb_hcd_giveback_urb__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct urb const* ptr);

void caller_unmarshal_kernel__usb_hcd_giveback_urb__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct urb* ptr);

void caller_marshal_kernel__usb_hcd_giveback_urb__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__usb_hcd_giveback_urb__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__usb_hcd_giveback_urb__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__usb_hcd_giveback_urb__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__usb_hcd_giveback_urb__usb_device_parent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__usb_hcd_giveback_urb__usb_device_parent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__usb_hcd_giveback_urb__usb_device_parent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__usb_hcd_giveback_urb__usb_device_parent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__usb_hcd_is_primary_hcd__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__usb_hcd_is_primary_hcd__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__usb_hcd_is_primary_hcd__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__usb_hcd_is_primary_hcd__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__usb_hcd_is_primary_hcd__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__usb_hcd_is_primary_hcd__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__usb_hcd_is_primary_hcd__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__usb_hcd_is_primary_hcd__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__usb_hcd_link_urb_to_ep__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__usb_hcd_link_urb_to_ep__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__usb_hcd_link_urb_to_ep__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__usb_hcd_link_urb_to_ep__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__usb_hcd_link_urb_to_ep__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct urb const* ptr);

void callee_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct urb* ptr);

void callee_marshal_kernel__usb_hcd_link_urb_to_ep__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct urb const* ptr);

void caller_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct urb* ptr);

void caller_marshal_kernel__usb_hcd_link_urb_to_ep__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__usb_hcd_link_urb_to_ep__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__usb_hcd_link_urb_to_ep__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void callee_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void callee_marshal_kernel__usb_hcd_link_urb_to_ep__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void caller_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void caller_marshal_kernel__usb_hcd_poll_rh_status__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__usb_hcd_poll_rh_status__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__usb_hcd_poll_rh_status__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__usb_hcd_poll_rh_status__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__usb_hcd_poll_rh_status__usb_hcd_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* call_ctx,
	struct hc_driver const* ptr);

void callee_unmarshal_kernel__usb_hcd_poll_rh_status__usb_hcd_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* call_ctx,
	struct hc_driver* ptr);

void callee_marshal_kernel__usb_hcd_poll_rh_status__usb_hcd_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* call_ctx,
	struct hc_driver const* ptr);

void caller_unmarshal_kernel__usb_hcd_poll_rh_status__usb_hcd_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* call_ctx,
	struct hc_driver* ptr);

void caller_marshal_kernel__usb_hcd_unlink_urb_from_ep__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__usb_hcd_unlink_urb_from_ep__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__usb_hcd_unlink_urb_from_ep__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__usb_hcd_unlink_urb_from_ep__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__usb_hcd_unlink_urb_from_ep__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* call_ctx,
	struct urb const* ptr);

void callee_unmarshal_kernel__usb_hcd_unlink_urb_from_ep__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* call_ctx,
	struct urb* ptr);

void callee_marshal_kernel__usb_hcd_unlink_urb_from_ep__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* call_ctx,
	struct urb const* ptr);

void caller_unmarshal_kernel__usb_hcd_unlink_urb_from_ep__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* call_ctx,
	struct urb* ptr);

void caller_marshal_kernel__usb_wakeup_notification__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_wakeup_notification_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__usb_wakeup_notification__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_wakeup_notification_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__usb_wakeup_notification__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_wakeup_notification_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__usb_wakeup_notification__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_wakeup_notification_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_start__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_start__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_start__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_start__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_start__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_start__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_start__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_start__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_enable_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_enable_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_enable_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_enable_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_enable_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_enable_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_enable_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_enable_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_enable_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_enable_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_enable_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_enable_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_update_hub_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_update_hub_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_update_hub_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_update_hub_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_update_hub_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_update_hub_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_update_hub_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_update_hub_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_update_hub_device__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_update_hub_device__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_update_hub_device__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_update_hub_device__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_update_hub_device__tt__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_tt const* ptr);

void callee_unmarshal_kernel__hc_driver_update_hub_device__tt__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_tt* ptr);

void callee_marshal_kernel__hc_driver_update_hub_device__tt__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_tt const* ptr);

void caller_unmarshal_kernel__hc_driver_update_hub_device__tt__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* call_ctx,
	struct usb_tt* ptr);

void caller_marshal_kernel__hc_driver_reset_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_reset_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_reset_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_reset_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_reset_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_reset_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_reset_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_reset_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_reset_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_reset_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_reset_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_reset_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_update_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_update_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_update_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_update_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_update_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_update_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_update_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_update_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_set_usb2_hw_lpm__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_set_usb2_hw_lpm__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_set_usb2_hw_lpm__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_set_usb2_hw_lpm__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_set_usb2_hw_lpm__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_set_usb2_hw_lpm__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_set_usb2_hw_lpm__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_set_usb2_hw_lpm__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_enable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_enable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_enable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_enable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_enable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_enable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_enable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_enable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_disable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_disable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_disable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_disable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_disable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_disable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_disable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_disable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_find_raw_port_number__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_find_raw_port_number__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_find_raw_port_number__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_find_raw_port_number__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_find_raw_port_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_find_raw_port_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_find_raw_port_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_find_raw_port_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_stop__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_stop__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_stop__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_stop__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_stop__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_stop__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_stop__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_stop__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_shutdown__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_shutdown__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_shutdown__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_shutdown__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_shutdown__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_shutdown__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_shutdown__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_shutdown__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_get_frame_number__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_get_frame_number__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_get_frame_number__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_get_frame_number__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_get_frame_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_get_frame_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_get_frame_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_get_frame_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_urb_enqueue__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_urb_enqueue__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_urb_enqueue__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_urb_enqueue__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_urb_enqueue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_urb_enqueue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_urb_enqueue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_urb_enqueue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_urb_enqueue__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct urb const* ptr);

void callee_unmarshal_kernel__hc_driver_urb_enqueue__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct urb* ptr);

void callee_marshal_kernel__hc_driver_urb_enqueue__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct urb const* ptr);

void caller_unmarshal_kernel__hc_driver_urb_enqueue__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct urb* ptr);

void caller_marshal_kernel__hc_driver_urb_enqueue__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_urb_enqueue__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_urb_enqueue__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_urb_enqueue__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_urb_enqueue__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void callee_unmarshal_kernel__hc_driver_urb_enqueue__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void callee_marshal_kernel__hc_driver_urb_enqueue__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void caller_unmarshal_kernel__hc_driver_urb_enqueue__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void caller_marshal_kernel__hc_driver_urb_enqueue__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor const* ptr);

void callee_unmarshal_kernel__hc_driver_urb_enqueue__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor* ptr);

void callee_marshal_kernel__hc_driver_urb_enqueue__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor const* ptr);

void caller_unmarshal_kernel__hc_driver_urb_enqueue__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor* ptr);

void caller_marshal_kernel__hc_driver_urb_dequeue__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_urb_dequeue__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_urb_dequeue__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_urb_dequeue__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_urb_dequeue__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct urb const* ptr);

void callee_unmarshal_kernel__hc_driver_urb_dequeue__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct urb* ptr);

void callee_marshal_kernel__hc_driver_urb_dequeue__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct urb const* ptr);

void caller_unmarshal_kernel__hc_driver_urb_dequeue__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct urb* ptr);

void caller_marshal_kernel__hc_driver_urb_dequeue__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_urb_dequeue__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_urb_dequeue__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_urb_dequeue__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_urb_dequeue__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void callee_unmarshal_kernel__hc_driver_urb_dequeue__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void callee_marshal_kernel__hc_driver_urb_dequeue__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void caller_unmarshal_kernel__hc_driver_urb_dequeue__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void caller_marshal_kernel__hc_driver_urb_dequeue__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor const* ptr);

void callee_unmarshal_kernel__hc_driver_urb_dequeue__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor* ptr);

void callee_marshal_kernel__hc_driver_urb_dequeue__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor const* ptr);

void caller_unmarshal_kernel__hc_driver_urb_dequeue__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor* ptr);

void caller_marshal_kernel__hc_driver_endpoint_reset__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_endpoint_reset__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_endpoint_reset__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_endpoint_reset__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_endpoint_reset__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_endpoint_reset__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_endpoint_reset__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_endpoint_reset__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_endpoint_reset__ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void callee_unmarshal_kernel__hc_driver_endpoint_reset__ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void callee_marshal_kernel__hc_driver_endpoint_reset__ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void caller_unmarshal_kernel__hc_driver_endpoint_reset__ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void caller_marshal_kernel__hc_driver_endpoint_reset__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor const* ptr);

void callee_unmarshal_kernel__hc_driver_endpoint_reset__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor* ptr);

void callee_marshal_kernel__hc_driver_endpoint_reset__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor const* ptr);

void caller_unmarshal_kernel__hc_driver_endpoint_reset__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor* ptr);

void caller_marshal_kernel__hc_driver_alloc_dev__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_alloc_dev__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_alloc_dev__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_alloc_dev__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_alloc_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_alloc_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_alloc_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_alloc_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_alloc_dev__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_alloc_dev__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_alloc_dev__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_alloc_dev__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_free_dev__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_free_dev__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_free_dev__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_free_dev__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_free_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_free_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_free_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_free_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_free_dev__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_free_dev__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_free_dev__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_free_dev__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_alloc_streams__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_alloc_streams__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_alloc_streams__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_alloc_streams__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_alloc_streams__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_alloc_streams__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_alloc_streams__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_alloc_streams__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_alloc_streams__eps__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void callee_unmarshal_kernel__hc_driver_alloc_streams__eps__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void callee_marshal_kernel__hc_driver_alloc_streams__eps__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void caller_unmarshal_kernel__hc_driver_alloc_streams__eps__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void caller_marshal_kernel__hc_driver_free_streams__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_free_streams__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_free_streams__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_free_streams__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_free_streams__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_free_streams__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_free_streams__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_free_streams__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_free_streams__eps__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void callee_unmarshal_kernel__hc_driver_free_streams__eps__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void callee_marshal_kernel__hc_driver_free_streams__eps__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void caller_unmarshal_kernel__hc_driver_free_streams__eps__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void caller_marshal_kernel__hc_driver_add_endpoint__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_add_endpoint__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_add_endpoint__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_add_endpoint__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_add_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_add_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_add_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_add_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_add_endpoint__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_add_endpoint__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_add_endpoint__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_add_endpoint__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_add_endpoint__ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void callee_unmarshal_kernel__hc_driver_add_endpoint__ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void callee_marshal_kernel__hc_driver_add_endpoint__ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void caller_unmarshal_kernel__hc_driver_add_endpoint__ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void caller_marshal_kernel__hc_driver_add_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor const* ptr);

void callee_unmarshal_kernel__hc_driver_add_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor* ptr);

void callee_marshal_kernel__hc_driver_add_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor const* ptr);

void caller_unmarshal_kernel__hc_driver_add_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor* ptr);

void caller_marshal_kernel__hc_driver_drop_endpoint__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_drop_endpoint__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_drop_endpoint__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_drop_endpoint__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_drop_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_drop_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_drop_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_drop_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_drop_endpoint__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_drop_endpoint__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_drop_endpoint__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_drop_endpoint__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_drop_endpoint__ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void callee_unmarshal_kernel__hc_driver_drop_endpoint__ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void callee_marshal_kernel__hc_driver_drop_endpoint__ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_host_endpoint const* ptr);

void caller_unmarshal_kernel__hc_driver_drop_endpoint__ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_host_endpoint* ptr);

void caller_marshal_kernel__hc_driver_drop_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor const* ptr);

void callee_unmarshal_kernel__hc_driver_drop_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor* ptr);

void callee_marshal_kernel__hc_driver_drop_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor const* ptr);

void caller_unmarshal_kernel__hc_driver_drop_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* call_ctx,
	struct usb_endpoint_descriptor* ptr);

void caller_marshal_kernel__hc_driver_check_bandwidth__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_check_bandwidth__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_check_bandwidth__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_check_bandwidth__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_check_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_check_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_check_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_check_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_check_bandwidth__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_check_bandwidth__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_check_bandwidth__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_check_bandwidth__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_reset_bandwidth__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_reset_bandwidth__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_reset_bandwidth__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_reset_bandwidth__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_reset_bandwidth__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_reset_bandwidth__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_reset_bandwidth__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_reset_bandwidth__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_address_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_address_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_address_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_address_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_address_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_address_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_address_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_address_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_address_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void callee_unmarshal_kernel__hc_driver_address_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* call_ctx,
	struct usb_device* ptr);

void callee_marshal_kernel__hc_driver_address_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* call_ctx,
	struct usb_device const* ptr);

void caller_unmarshal_kernel__hc_driver_address_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* call_ctx,
	struct usb_device* ptr);

void caller_marshal_kernel__hc_driver_hub_status_data__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_status_data_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_hub_status_data__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_status_data_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_hub_status_data__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_status_data_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_hub_status_data__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_status_data_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_irq__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_irq_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_irq__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_irq_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_irq__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_irq_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_irq__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_irq_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void caller_marshal_kernel__hc_driver_hub_control__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void callee_unmarshal_kernel__hc_driver_hub_control__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* call_ctx,
	struct usb_hcd* ptr);

void callee_marshal_kernel__hc_driver_hub_control__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* call_ctx,
	struct usb_hcd const* ptr);

void caller_unmarshal_kernel__hc_driver_hub_control__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* call_ctx,
	struct usb_hcd* ptr);


#endif
