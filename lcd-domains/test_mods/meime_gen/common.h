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

static inline void* glue_unpack_bind_or_new_shadow_impl(const void* ptr, size_t size)
{
	void* shadow = 0;
	if (!ptr)
		return NULL;

	shadow = glue_user_map_to_shadow(ptr, false);
	if (!shadow) {
		shadow = glue_user_alloc(size, DEFAULT_GFP_FLAGS);
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
	RPC_ID_pci_driver_probe,
	RPC_ID_pci_driver_remove,
	RPC_ID_pci_driver_shutdown,
	RPC_ID_pci_set_master,
	RPC_ID_pci_request_regions,
	RPC_ID_pci_release_regions,
	RPC_ID_pci_dev_run_wake,
	RPC_ID_pci_enable_device,
	RPC_ID_pci_unregister_driver,
	RPC_ID___pci_register_driver,
	RPC_ID_mei_hbm_pg_resume,
	RPC_ID_mei_hbm_pg,
	RPC_ID_mei_device_init,
	RPC_ID_mei_stop,
	RPC_ID_mei_cancel_work,
	RPC_ID_mei_start,
	RPC_ID_pci_iomap,
	RPC_ID_pci_bus_read_config_dword,
	RPC_ID_mei_hw_ops_host_is_ready,
	RPC_ID_mei_hw_ops_hw_is_ready,
	RPC_ID_mei_hw_ops_hw_reset,
	RPC_ID_mei_hw_ops_hw_start,
	RPC_ID_mei_hw_ops_hw_config,
	RPC_ID_mei_hw_ops_fw_status,
	RPC_ID_mei_hw_ops_pg_state,
	RPC_ID_mei_hw_ops_pg_in_transition,
	RPC_ID_mei_hw_ops_pg_is_enabled,
	RPC_ID_mei_hw_ops_intr_clear,
	RPC_ID_mei_hw_ops_intr_enable,
	RPC_ID_mei_hw_ops_intr_disable,
	RPC_ID_mei_hw_ops_hbuf_free_slots,
	RPC_ID_mei_hw_ops_hbuf_is_ready,
	RPC_ID_mei_hw_ops_hbuf_max_len,
	RPC_ID_mei_hw_ops_write,
	RPC_ID_mei_hw_ops_rdbuf_full_slots,
	RPC_ID_mei_hw_ops_read_hdr,
	RPC_ID_mei_hw_ops_read,
	RPC_ID_schedule_timeout,
	RPC_ID_finish_wait,
	RPC_ID_mei_deregister,
	RPC_ID_mei_register,
	RPC_ID_prepare_to_wait_event,
	RPC_ID___msecs_to_jiffies,
	RPC_ID_queue_delayed_work_on,
	RPC_ID_mei_irq_write_handler,
	RPC_ID_mei_irq_read_handler,
	RPC_ID_mei_irq_compl_handler,
	RPC_ID_pci_disable_msi,
	RPC_ID_queue_work_on,
	RPC_ID_thread_fn,
	RPC_ID_handler,
	RPC_ID_request_threaded_irq,
	RPC_ID_free_irq,
	RPC_ID_pci_disable_device,
	RPC_ID_pci_enable_msi_range,
	RPC_ID_pci_iounmap,
	RPC_ID___wake_up,
};

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext);

typedef int (*fptr_pci_driver_probe)(struct pci_dev* pdev, struct pci_device_id* ent);
typedef int (*fptr_impl_pci_driver_probe)(fptr_pci_driver_probe target, struct pci_dev* pdev, struct pci_device_id* ent);

LCD_TRAMPOLINE_DATA(trmp_pci_driver_probe)
int LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_probe) trmp_pci_driver_probe(struct pci_dev* pdev, struct pci_device_id* ent);

typedef void (*fptr_pci_driver_remove)(struct pci_dev* pdev);
typedef void (*fptr_impl_pci_driver_remove)(fptr_pci_driver_remove target, struct pci_dev* pdev);

LCD_TRAMPOLINE_DATA(trmp_pci_driver_remove)
void LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_remove) trmp_pci_driver_remove(struct pci_dev* pdev);

typedef void (*fptr_pci_driver_shutdown)(struct pci_dev* pdev);
typedef void (*fptr_impl_pci_driver_shutdown)(fptr_pci_driver_shutdown target, struct pci_dev* pdev);

LCD_TRAMPOLINE_DATA(trmp_pci_driver_shutdown)
void LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_shutdown) trmp_pci_driver_shutdown(struct pci_dev* pdev);

typedef bool (*fptr_mei_hw_ops_host_is_ready)(struct mei_device* dev);
typedef bool (*fptr_impl_mei_hw_ops_host_is_ready)(fptr_mei_hw_ops_host_is_ready target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_host_is_ready)
bool LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_host_is_ready) trmp_mei_hw_ops_host_is_ready(struct mei_device* dev);

typedef bool (*fptr_mei_hw_ops_hw_is_ready)(struct mei_device* dev);
typedef bool (*fptr_impl_mei_hw_ops_hw_is_ready)(fptr_mei_hw_ops_hw_is_ready target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hw_is_ready)
bool LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hw_is_ready) trmp_mei_hw_ops_hw_is_ready(struct mei_device* dev);

typedef int (*fptr_mei_hw_ops_hw_reset)(struct mei_device* dev, bool intr_enable);
typedef int (*fptr_impl_mei_hw_ops_hw_reset)(fptr_mei_hw_ops_hw_reset target, struct mei_device* dev, bool intr_enable);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hw_reset)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hw_reset) trmp_mei_hw_ops_hw_reset(struct mei_device* dev, bool intr_enable);

typedef int (*fptr_mei_hw_ops_hw_start)(struct mei_device* dev);
typedef int (*fptr_impl_mei_hw_ops_hw_start)(fptr_mei_hw_ops_hw_start target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hw_start)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hw_start) trmp_mei_hw_ops_hw_start(struct mei_device* dev);

typedef void (*fptr_mei_hw_ops_hw_config)(struct mei_device* dev);
typedef void (*fptr_impl_mei_hw_ops_hw_config)(fptr_mei_hw_ops_hw_config target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hw_config)
void LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hw_config) trmp_mei_hw_ops_hw_config(struct mei_device* dev);

typedef int (*fptr_mei_hw_ops_fw_status)(struct mei_device* dev, struct mei_fw_status* fw_status);
typedef int (*fptr_impl_mei_hw_ops_fw_status)(fptr_mei_hw_ops_fw_status target, struct mei_device* dev, struct mei_fw_status* fw_status);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_fw_status)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_fw_status) trmp_mei_hw_ops_fw_status(struct mei_device* dev, struct mei_fw_status* fw_status);

typedef unsigned int (*fptr_mei_hw_ops_pg_state)(struct mei_device* dev);
typedef unsigned int (*fptr_impl_mei_hw_ops_pg_state)(fptr_mei_hw_ops_pg_state target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_pg_state)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_pg_state) trmp_mei_hw_ops_pg_state(struct mei_device* dev);

typedef bool (*fptr_mei_hw_ops_pg_in_transition)(struct mei_device* dev);
typedef bool (*fptr_impl_mei_hw_ops_pg_in_transition)(fptr_mei_hw_ops_pg_in_transition target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_pg_in_transition)
bool LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_pg_in_transition) trmp_mei_hw_ops_pg_in_transition(struct mei_device* dev);

typedef bool (*fptr_mei_hw_ops_pg_is_enabled)(struct mei_device* dev);
typedef bool (*fptr_impl_mei_hw_ops_pg_is_enabled)(fptr_mei_hw_ops_pg_is_enabled target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_pg_is_enabled)
bool LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_pg_is_enabled) trmp_mei_hw_ops_pg_is_enabled(struct mei_device* dev);

typedef void (*fptr_mei_hw_ops_intr_clear)(struct mei_device* dev);
typedef void (*fptr_impl_mei_hw_ops_intr_clear)(fptr_mei_hw_ops_intr_clear target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_intr_clear)
void LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_intr_clear) trmp_mei_hw_ops_intr_clear(struct mei_device* dev);

typedef void (*fptr_mei_hw_ops_intr_enable)(struct mei_device* dev);
typedef void (*fptr_impl_mei_hw_ops_intr_enable)(fptr_mei_hw_ops_intr_enable target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_intr_enable)
void LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_intr_enable) trmp_mei_hw_ops_intr_enable(struct mei_device* dev);

typedef void (*fptr_mei_hw_ops_intr_disable)(struct mei_device* dev);
typedef void (*fptr_impl_mei_hw_ops_intr_disable)(fptr_mei_hw_ops_intr_disable target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_intr_disable)
void LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_intr_disable) trmp_mei_hw_ops_intr_disable(struct mei_device* dev);

typedef int (*fptr_mei_hw_ops_hbuf_free_slots)(struct mei_device* dev);
typedef int (*fptr_impl_mei_hw_ops_hbuf_free_slots)(fptr_mei_hw_ops_hbuf_free_slots target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hbuf_free_slots)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hbuf_free_slots) trmp_mei_hw_ops_hbuf_free_slots(struct mei_device* dev);

typedef bool (*fptr_mei_hw_ops_hbuf_is_ready)(struct mei_device* dev);
typedef bool (*fptr_impl_mei_hw_ops_hbuf_is_ready)(fptr_mei_hw_ops_hbuf_is_ready target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hbuf_is_ready)
bool LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hbuf_is_ready) trmp_mei_hw_ops_hbuf_is_ready(struct mei_device* dev);

typedef unsigned long (*fptr_mei_hw_ops_hbuf_max_len)(struct mei_device* dev);
typedef unsigned long (*fptr_impl_mei_hw_ops_hbuf_max_len)(fptr_mei_hw_ops_hbuf_max_len target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hbuf_max_len)
unsigned long LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hbuf_max_len) trmp_mei_hw_ops_hbuf_max_len(struct mei_device* dev);

typedef int (*fptr_mei_hw_ops_write)(struct mei_device* dev, struct mei_msg_hdr* header, unsigned char* buf);
typedef int (*fptr_impl_mei_hw_ops_write)(fptr_mei_hw_ops_write target, struct mei_device* dev, struct mei_msg_hdr* header, unsigned char* buf);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_write)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_write) trmp_mei_hw_ops_write(struct mei_device* dev, struct mei_msg_hdr* header, unsigned char* buf);

typedef int (*fptr_mei_hw_ops_rdbuf_full_slots)(struct mei_device* dev);
typedef int (*fptr_impl_mei_hw_ops_rdbuf_full_slots)(fptr_mei_hw_ops_rdbuf_full_slots target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_rdbuf_full_slots)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_rdbuf_full_slots) trmp_mei_hw_ops_rdbuf_full_slots(struct mei_device* dev);

typedef unsigned int (*fptr_mei_hw_ops_read_hdr)(struct mei_device* dev);
typedef unsigned int (*fptr_impl_mei_hw_ops_read_hdr)(fptr_mei_hw_ops_read_hdr target, struct mei_device* dev);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_read_hdr)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_read_hdr) trmp_mei_hw_ops_read_hdr(struct mei_device* dev);

typedef int (*fptr_mei_hw_ops_read)(struct mei_device* dev, unsigned char* buffer, unsigned long buffer_length);
typedef int (*fptr_impl_mei_hw_ops_read)(fptr_mei_hw_ops_read target, struct mei_device* dev, unsigned char* buffer, unsigned long buffer_length);

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_read)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_read) trmp_mei_hw_ops_read(struct mei_device* dev, unsigned char* buffer, unsigned long buffer_length);

typedef unsigned int (*fptr_thread_fn)(int irq, void* id);
typedef unsigned int (*fptr_impl_thread_fn)(fptr_thread_fn target, int irq, void* id);

LCD_TRAMPOLINE_DATA(trmp_thread_fn)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_thread_fn) trmp_thread_fn(int irq, void* id);

typedef unsigned int (*fptr_handler)(int irq, void* id);
typedef unsigned int (*fptr_impl_handler)(fptr_handler target, int irq, void* id);

LCD_TRAMPOLINE_DATA(trmp_handler)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_handler) trmp_handler(int irq, void* id);

struct pci_driver_probe_call_ctx {
	struct pci_dev* pdev;
	struct pci_device_id* ent;
};

struct pci_driver_remove_call_ctx {
	struct pci_dev* pdev;
};

struct pci_driver_shutdown_call_ctx {
	struct pci_dev* pdev;
};

struct pci_set_master_call_ctx {
	struct pci_dev* dev;
};

struct pci_request_regions_call_ctx {
	struct pci_dev* pdev;
	char const* res_name;
};

struct pci_release_regions_call_ctx {
	struct pci_dev* pdev;
};

struct pci_dev_run_wake_call_ctx {
	struct pci_dev* dev;
};

struct pci_enable_device_call_ctx {
	struct pci_dev* dev;
};

struct pci_unregister_driver_call_ctx {
	struct pci_driver* drv;
};

struct __pci_register_driver_call_ctx {
	struct pci_driver* drv;
	struct module* owner;
	char const* mod_name;
};

struct mei_hbm_pg_resume_call_ctx {
	struct mei_device* dev;
};

struct mei_hbm_pg_call_ctx {
	struct mei_device* dev;
	unsigned char pg_cmd;
};

struct mei_device_init_call_ctx {
	struct mei_device* dev;
	struct device* device;
	struct mei_hw_ops const* ops;
};

struct mei_stop_call_ctx {
	struct mei_device* dev;
};

struct mei_cancel_work_call_ctx {
	struct mei_device* dev;
};

struct mei_start_call_ctx {
	struct mei_device* dev;
};

struct pci_iomap_call_ctx {
	struct pci_dev* pdev;
	int bar;
	unsigned long maxlen;
};

struct pci_bus_read_config_dword_call_ctx {
	struct pci_bus* bus;
	unsigned int devfn;
	int pos;
	unsigned int* value;
};

struct mei_hw_ops_host_is_ready_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_hw_is_ready_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_hw_reset_call_ctx {
	struct mei_device* dev;
	bool intr_enable;
};

struct mei_hw_ops_hw_start_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_hw_config_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_fw_status_call_ctx {
	struct mei_device* dev;
	struct mei_fw_status* fw_status;
};

struct mei_hw_ops_pg_state_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_pg_in_transition_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_pg_is_enabled_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_intr_clear_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_intr_enable_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_intr_disable_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_hbuf_free_slots_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_hbuf_is_ready_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_hbuf_max_len_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_write_call_ctx {
	struct mei_device* dev;
	struct mei_msg_hdr* header;
	unsigned char* buf;
};

struct mei_hw_ops_rdbuf_full_slots_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_read_hdr_call_ctx {
	struct mei_device* dev;
};

struct mei_hw_ops_read_call_ctx {
	struct mei_device* dev;
	unsigned char* buffer;
	unsigned long buffer_length;
};

struct schedule_timeout_call_ctx {
	long timeout;
};

struct finish_wait_call_ctx {
	struct __wait_queue_head* wq;
	struct __wait_queue* wait;
};

struct mei_deregister_call_ctx {
	struct mei_device* dev;
};

struct mei_register_call_ctx {
	struct mei_device* dev;
	struct device* parent;
};

struct prepare_to_wait_event_call_ctx {
	struct __wait_queue_head* wq;
	struct __wait_queue* wait;
	int state;
};

struct __msecs_to_jiffies_call_ctx {
	unsigned int m;
};

struct queue_delayed_work_on_call_ctx {
	int cpu;
	struct workqueue_struct* wq;
	struct delayed_work* dwork;
	unsigned long delay;
};

struct mei_irq_write_handler_call_ctx {
	struct mei_device* dev;
	struct mei_cl_cb* cmpl_list;
};

struct mei_irq_read_handler_call_ctx {
	struct mei_device* dev;
	struct mei_cl_cb* cmpl_list;
	int* slots;
};

struct mei_irq_compl_handler_call_ctx {
	struct mei_device* dev;
	struct mei_cl_cb* compl_list;
};

struct pci_disable_msi_call_ctx {
	struct pci_dev* dev;
};

struct queue_work_on_call_ctx {
	int cpu;
	struct workqueue_struct* wq;
	struct work_struct* work;
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
	char const* devname;
	void* dev_id;
};

struct free_irq_call_ctx {
	unsigned int irq;
	void* dev_id;
};

struct pci_disable_device_call_ctx {
	struct pci_dev* dev;
};

struct pci_enable_msi_range_call_ctx {
	struct pci_dev* dev;
	int minvec;
	int maxvec;
};

struct pci_iounmap_call_ctx {
	struct pci_dev* dev;
	void* addr;
};

struct __wake_up_call_ctx {
	struct __wait_queue_head* wq;
	unsigned int mode;
	int nr_exclusive;
	void* key;
};

void caller_marshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void callee_unmarshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void callee_marshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void caller_unmarshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void caller_marshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_remove_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_remove_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_remove_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_remove_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_shutdown_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_shutdown_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_shutdown_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_shutdown_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_set_master__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pci_set_master__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pci_set_master__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pci_set_master__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__pci_request_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_request_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_request_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_request_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_request_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_request_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_request_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_request_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_release_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_release_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_release_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_release_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_release_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_release_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_release_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_release_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_dev_run_wake__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_dev_run_wake__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_dev_run_wake__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_dev_run_wake__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_dev_run_wake__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pci_dev_run_wake__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pci_dev_run_wake__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pci_dev_run_wake__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* call_ctx,
	struct pci_bus* ptr);

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

void caller_marshal_kernel__pci_enable_device__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pci_enable_device__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pci_enable_device__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pci_enable_device__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void callee_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void callee_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void caller_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void caller_marshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void callee_unmarshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void callee_marshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void caller_unmarshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void caller_marshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void callee_unmarshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void callee_marshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void caller_unmarshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void caller_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module const* ptr);

void callee_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module* ptr);

void callee_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module const* ptr);

void caller_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module* ptr);

void caller_marshal_kernel__mei_hbm_pg_resume__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hbm_pg_resume_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hbm_pg_resume__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hbm_pg_resume_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hbm_pg_resume__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hbm_pg_resume_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hbm_pg_resume__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hbm_pg_resume_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hbm_pg__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hbm_pg_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hbm_pg__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hbm_pg_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hbm_pg__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hbm_pg_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hbm_pg__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hbm_pg_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_device_init__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_device_init__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_device_init__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_device_init__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_device_init____wait_queue_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void callee_unmarshal_kernel__mei_device_init____wait_queue_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void callee_marshal_kernel__mei_device_init____wait_queue_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void caller_unmarshal_kernel__mei_device_init____wait_queue_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void caller_marshal_kernel__mei_device_init__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__mei_device_init__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__mei_device_init__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__mei_device_init__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel___global_mei_hw_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops const* ptr);

void callee_unmarshal_kernel___global_mei_hw_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops* ptr);

void callee_marshal_kernel___global_mei_hw_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops const* ptr);

void caller_unmarshal_kernel___global_mei_hw_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops* ptr);

void caller_marshal_kernel__mei_stop__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_stop_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_stop__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_stop_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_stop__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_stop_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_stop__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_stop_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_cancel_work__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_cancel_work_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_cancel_work__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_cancel_work_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_cancel_work__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_cancel_work_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_cancel_work__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_cancel_work_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_start__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_start_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_start__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_start_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_start__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_start_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_start__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_start_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__pci_iomap__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_iomap_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_iomap__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_iomap_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_iomap__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_iomap_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_iomap__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_iomap_call_ctx const* call_ctx,
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

void caller_marshal_kernel__mei_hw_ops_host_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_host_is_ready_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_host_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_host_is_ready_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_host_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_host_is_ready_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_host_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_host_is_ready_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_hw_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_is_ready_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_hw_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_is_ready_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_hw_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_is_ready_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_hw_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_is_ready_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_hw_reset__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_hw_reset__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_hw_reset__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_hw_reset__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_hw_reset____wait_queue_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_hw_reset____wait_queue_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void callee_marshal_kernel__mei_hw_ops_hw_reset____wait_queue_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_hw_reset____wait_queue_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void caller_marshal_kernel__mei_hw_ops_hw_start__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_hw_start__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_hw_start__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_hw_start__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_hw_start____wait_queue_head__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_hw_start____wait_queue_head__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void callee_marshal_kernel__mei_hw_ops_hw_start____wait_queue_head__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_hw_start____wait_queue_head__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void caller_marshal_kernel__mei_hw_ops_hw_config__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_config_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_hw_config__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_config_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_hw_config__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_config_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_hw_config__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_config_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_fw_status__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_fw_status__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_fw_status__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_fw_status__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_fw_status__fw_status__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* call_ctx,
	struct mei_fw_status const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_fw_status__fw_status__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* call_ctx,
	struct mei_fw_status* ptr);

void callee_marshal_kernel__mei_hw_ops_fw_status__fw_status__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* call_ctx,
	struct mei_fw_status const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_fw_status__fw_status__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* call_ctx,
	struct mei_fw_status* ptr);

void caller_marshal_kernel__mei_hw_ops_pg_state__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_pg_state_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_pg_state__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_pg_state_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_pg_state__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_pg_state_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_pg_state__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_pg_state_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_pg_in_transition__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_pg_in_transition_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_pg_in_transition__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_pg_in_transition_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_pg_in_transition__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_pg_in_transition_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_pg_in_transition__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_pg_in_transition_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_pg_is_enabled__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_pg_is_enabled_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_pg_is_enabled__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_pg_is_enabled_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_pg_is_enabled__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_pg_is_enabled_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_pg_is_enabled__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_pg_is_enabled_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_intr_clear__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_intr_clear_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_intr_clear__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_intr_clear_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_intr_clear__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_intr_clear_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_intr_clear__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_intr_clear_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_intr_enable__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_intr_enable_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_intr_enable__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_intr_enable_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_intr_enable__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_intr_enable_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_intr_enable__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_intr_enable_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_intr_disable__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_intr_disable_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_intr_disable__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_intr_disable_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_intr_disable__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_intr_disable_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_intr_disable__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_intr_disable_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_hbuf_free_slots__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hbuf_free_slots_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_hbuf_free_slots__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hbuf_free_slots_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_hbuf_free_slots__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hbuf_free_slots_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_hbuf_free_slots__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hbuf_free_slots_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_hbuf_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hbuf_is_ready_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_hbuf_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hbuf_is_ready_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_hbuf_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hbuf_is_ready_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_hbuf_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hbuf_is_ready_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_hbuf_max_len__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hbuf_max_len_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_hbuf_max_len__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hbuf_max_len_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_hbuf_max_len__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hbuf_max_len_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_hbuf_max_len__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hbuf_max_len_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_write__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_write__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_write__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_write__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_write__header__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* call_ctx,
	struct mei_msg_hdr const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_write__header__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* call_ctx,
	struct mei_msg_hdr* ptr);

void callee_marshal_kernel__mei_hw_ops_write__header__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* call_ctx,
	struct mei_msg_hdr const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_write__header__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* call_ctx,
	struct mei_msg_hdr* ptr);

void caller_marshal_kernel__mei_hw_ops_rdbuf_full_slots__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_rdbuf_full_slots_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_rdbuf_full_slots__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_rdbuf_full_slots_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_rdbuf_full_slots__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_rdbuf_full_slots_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_rdbuf_full_slots__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_rdbuf_full_slots_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_read_hdr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_read_hdr_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_read_hdr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_read_hdr_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_read_hdr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_read_hdr_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_read_hdr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_read_hdr_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_hw_ops_read__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_read_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_hw_ops_read__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_read_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_hw_ops_read__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_read_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_hw_ops_read__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_read_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__finish_wait__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void callee_unmarshal_kernel__finish_wait__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void callee_marshal_kernel__finish_wait__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void caller_unmarshal_kernel__finish_wait__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void caller_marshal_kernel__finish_wait____wait_queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue const* ptr);

void callee_unmarshal_kernel__finish_wait____wait_queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue* ptr);

void callee_marshal_kernel__finish_wait____wait_queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue const* ptr);

void caller_unmarshal_kernel__finish_wait____wait_queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* call_ctx,
	struct __wait_queue* ptr);

void caller_marshal_kernel__mei_deregister__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_deregister_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_deregister__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_deregister_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_deregister__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_deregister_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_deregister__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_deregister_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_register__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_register_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_register__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_register_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_register__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_register_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_register__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_register_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_register__parent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_register_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__mei_register__parent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_register_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__mei_register__parent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_register_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__mei_register__parent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_register_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__prepare_to_wait_event__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void callee_unmarshal_kernel__prepare_to_wait_event__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void callee_marshal_kernel__prepare_to_wait_event__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void caller_unmarshal_kernel__prepare_to_wait_event__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void caller_marshal_kernel__prepare_to_wait_event____wait_queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* call_ctx,
	struct __wait_queue const* ptr);

void callee_unmarshal_kernel__prepare_to_wait_event____wait_queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* call_ctx,
	struct __wait_queue* ptr);

void callee_marshal_kernel__prepare_to_wait_event____wait_queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* call_ctx,
	struct __wait_queue const* ptr);

void caller_unmarshal_kernel__prepare_to_wait_event____wait_queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* call_ctx,
	struct __wait_queue* ptr);

void caller_marshal_kernel__queue_delayed_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void callee_unmarshal_kernel__queue_delayed_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void callee_marshal_kernel__queue_delayed_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void caller_unmarshal_kernel__queue_delayed_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void caller_marshal_kernel__queue_delayed_work_on__dwork__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* call_ctx,
	struct delayed_work const* ptr);

void callee_unmarshal_kernel__queue_delayed_work_on__dwork__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* call_ctx,
	struct delayed_work* ptr);

void callee_marshal_kernel__queue_delayed_work_on__dwork__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* call_ctx,
	struct delayed_work const* ptr);

void caller_unmarshal_kernel__queue_delayed_work_on__dwork__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* call_ctx,
	struct delayed_work* ptr);

void caller_marshal_kernel__mei_irq_write_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_irq_write_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_irq_write_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_irq_write_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_irq_write_handler__cmpl_list__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* call_ctx,
	struct mei_cl_cb const* ptr);

void callee_unmarshal_kernel__mei_irq_write_handler__cmpl_list__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* call_ctx,
	struct mei_cl_cb* ptr);

void callee_marshal_kernel__mei_irq_write_handler__cmpl_list__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* call_ctx,
	struct mei_cl_cb const* ptr);

void caller_unmarshal_kernel__mei_irq_write_handler__cmpl_list__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* call_ctx,
	struct mei_cl_cb* ptr);

void caller_marshal_kernel__mei_irq_write_handler__list_head__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* call_ctx,
	struct list_head const* ptr);

void callee_unmarshal_kernel__mei_irq_write_handler__list_head__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* call_ctx,
	struct list_head* ptr);

void callee_marshal_kernel__mei_irq_write_handler__list_head__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* call_ctx,
	struct list_head const* ptr);

void caller_unmarshal_kernel__mei_irq_write_handler__list_head__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* call_ctx,
	struct list_head* ptr);

void caller_marshal_kernel__mei_irq_read_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_irq_read_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_irq_read_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_irq_read_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_irq_read_handler__cmpl_list__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* call_ctx,
	struct mei_cl_cb const* ptr);

void callee_unmarshal_kernel__mei_irq_read_handler__cmpl_list__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* call_ctx,
	struct mei_cl_cb* ptr);

void callee_marshal_kernel__mei_irq_read_handler__cmpl_list__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* call_ctx,
	struct mei_cl_cb const* ptr);

void caller_unmarshal_kernel__mei_irq_read_handler__cmpl_list__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* call_ctx,
	struct mei_cl_cb* ptr);

void caller_marshal_kernel__mei_irq_read_handler__list_head__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* call_ctx,
	struct list_head const* ptr);

void callee_unmarshal_kernel__mei_irq_read_handler__list_head__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* call_ctx,
	struct list_head* ptr);

void callee_marshal_kernel__mei_irq_read_handler__list_head__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* call_ctx,
	struct list_head const* ptr);

void caller_unmarshal_kernel__mei_irq_read_handler__list_head__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* call_ctx,
	struct list_head* ptr);

void caller_marshal_kernel__mei_irq_compl_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void callee_unmarshal_kernel__mei_irq_compl_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct mei_device* ptr);

void callee_marshal_kernel__mei_irq_compl_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct mei_device const* ptr);

void caller_unmarshal_kernel__mei_irq_compl_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct mei_device* ptr);

void caller_marshal_kernel__mei_irq_compl_handler__mei_device_dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__mei_irq_compl_handler__mei_device_dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__mei_irq_compl_handler__mei_device_dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__mei_irq_compl_handler__mei_device_dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__mei_irq_compl_handler__compl_list__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct mei_cl_cb const* ptr);

void callee_unmarshal_kernel__mei_irq_compl_handler__compl_list__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct mei_cl_cb* ptr);

void callee_marshal_kernel__mei_irq_compl_handler__compl_list__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct mei_cl_cb const* ptr);

void caller_unmarshal_kernel__mei_irq_compl_handler__compl_list__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct mei_cl_cb* ptr);

void caller_marshal_kernel__mei_irq_compl_handler__list_head__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct list_head const* ptr);

void callee_unmarshal_kernel__mei_irq_compl_handler__list_head__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct list_head* ptr);

void callee_marshal_kernel__mei_irq_compl_handler__list_head__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct list_head const* ptr);

void caller_unmarshal_kernel__mei_irq_compl_handler__list_head__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* call_ctx,
	struct list_head* ptr);

void caller_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_disable_msi__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pci_disable_msi__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pci_disable_msi__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pci_disable_msi__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void callee_unmarshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void callee_marshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void caller_unmarshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void caller_marshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void callee_unmarshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct* ptr);

void callee_marshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void caller_unmarshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct* ptr);

void caller_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_enable_msi_range__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pci_enable_msi_range__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pci_enable_msi_range__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pci_enable_msi_range__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__pci_iounmap__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_iounmap_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_iounmap__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_iounmap_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_iounmap__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_iounmap_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_iounmap__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_iounmap_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel____wake_up__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __wake_up_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void callee_unmarshal_kernel____wake_up__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __wake_up_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);

void callee_marshal_kernel____wake_up__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __wake_up_call_ctx const* call_ctx,
	struct __wait_queue_head const* ptr);

void caller_unmarshal_kernel____wake_up__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __wake_up_call_ctx const* call_ctx,
	struct __wait_queue_head* ptr);


#endif
