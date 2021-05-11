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
	RPC_ID___alloc_workqueue_key,
	RPC_ID___bitmap_weight,
	RPC_ID___pci_register_driver,
	RPC_ID_blk_cleanup_queue,
	RPC_ID_done,
	RPC_ID_blk_execute_rq_nowait,
	RPC_ID_blk_get_queue,
	RPC_ID_blk_mq_alloc_tag_set,
	RPC_ID_blk_mq_complete_request,
	RPC_ID_blk_mq_end_request,
	RPC_ID_blk_mq_free_request,
	RPC_ID_blk_mq_free_tag_set,
	RPC_ID_blk_mq_init_queue,
	RPC_ID_blk_mq_ops_map_queue,
	RPC_ID_blk_mq_start_request,
	RPC_ID_blk_mq_start_stopped_hw_queues,
	RPC_ID_blk_mq_stop_hw_queues,
	RPC_ID_tag_iter_fn,
	RPC_ID_blk_mq_tagset_busy_iter,
	RPC_ID_blk_mq_update_nr_hw_queues,
	RPC_ID_blk_put_queue,
	RPC_ID_complete,
	RPC_ID_del_timer_sync,
	RPC_ID_destroy_workqueue,
	RPC_ID_device_release_driver,
	RPC_ID_flush_work,
	RPC_ID_free_irq,
	RPC_ID_get_device,
	RPC_ID_init_timer_key,
	RPC_ID_ioremap_nocache,
	RPC_ID_irq_set_affinity_hint,
	RPC_ID_mod_timer,
	RPC_ID_blk_mq_ops_exit_hctx,
	RPC_ID_blk_mq_ops_init_hctx,
	RPC_ID_blk_mq_ops_init_request,
	RPC_ID_nvme_alloc_request,
	RPC_ID_nvme_cancel_request,
	RPC_ID_nvme_change_ctrl_state,
	RPC_ID_nvme_complete_async_event,
	RPC_ID_blk_mq_ops_complete,
	RPC_ID_nvme_disable_ctrl,
	RPC_ID_nvme_enable_ctrl,
	RPC_ID_pci_cleanup_aer_uncorrect_error_status,
	RPC_ID_pci_error_handlers_error_detected,
	RPC_ID_pci_error_handlers_resume,
	RPC_ID_nvme_init_ctrl,
	RPC_ID_nvme_init_identify,
	RPC_ID_nvme_kill_queues,
	RPC_ID_pci_driver_sriov_configure,
	RPC_ID_blk_mq_ops_poll,
	RPC_ID_pci_driver_probe,
	RPC_ID_nvme_put_ctrl,
	RPC_ID_nvme_queue_async_events,
	RPC_ID_blk_mq_ops_queue_rq,
	RPC_ID_nvme_queue_scan,
	RPC_ID_pci_driver_remove,
	RPC_ID_nvme_remove_namespaces,
	RPC_ID_nvme_requeue_req,
	RPC_ID_pci_error_handlers_reset_notify,
	RPC_ID_nvme_set_queue_count,
	RPC_ID_nvme_setup_cmd,
	RPC_ID_pci_driver_shutdown,
	RPC_ID_nvme_shutdown_ctrl,
	RPC_ID_pci_error_handlers_slot_reset,
	RPC_ID_nvme_start_queues,
	RPC_ID_nvme_stop_queues,
	RPC_ID_nvme_submit_sync_cmd,
	RPC_ID_blk_mq_ops_timeout,
	RPC_ID_nvme_uninit_ctrl,
	RPC_ID_pci_device_is_present,
	RPC_ID_pci_disable_device,
	RPC_ID_pci_disable_msi,
	RPC_ID_pci_disable_msix,
	RPC_ID_pci_enable_device_mem,
	RPC_ID_pci_enable_msi_range,
	RPC_ID_pci_enable_msix,
	RPC_ID_pci_enable_msix_range,
	RPC_ID_pci_enable_pcie_error_reporting,
	RPC_ID_pci_disable_pcie_error_reporting,
	RPC_ID_pci_release_selected_regions,
	RPC_ID_pci_request_selected_regions,
	RPC_ID_pci_restore_state,
	RPC_ID_pci_save_state,
	RPC_ID_pci_select_bars,
	RPC_ID_pci_set_master,
	RPC_ID_pci_unregister_driver,
	RPC_ID_put_device,
	RPC_ID_queue_work_on,
	RPC_ID_thread_fn,
	RPC_ID_handler,
	RPC_ID_request_threaded_irq,
	RPC_ID_round_jiffies,
	RPC_ID_wait_for_completion_io_timeout,
	RPC_ID_work_busy,
	RPC_ID_blk_mq_map_queue,
};

int try_dispatch(enum RPC_ID id, struct fipc_message* __msg, struct ext_registers* __ext);

typedef void (*fptr_done)(struct request* rq, int id);
typedef void (*fptr_impl_done)(fptr_done target, struct request* rq, int id);

LCD_TRAMPOLINE_DATA(trmp_done)
void LCD_TRAMPOLINE_LINKAGE(trmp_done) trmp_done(struct request* rq, int id);

typedef struct blk_mq_hw_ctx* (*fptr_blk_mq_ops_map_queue)(struct request_queue* q, int cpu);
typedef struct blk_mq_hw_ctx* (*fptr_impl_blk_mq_ops_map_queue)(fptr_blk_mq_ops_map_queue target, struct request_queue* q, int cpu);

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_map_queue)
struct blk_mq_hw_ctx* LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_map_queue) trmp_blk_mq_ops_map_queue(struct request_queue* q, int cpu);

typedef void (*fptr_tag_iter_fn)(struct request* rq, void* data, bool enable);
typedef void (*fptr_impl_tag_iter_fn)(fptr_tag_iter_fn target, struct request* rq, void* data, bool enable);

LCD_TRAMPOLINE_DATA(trmp_tag_iter_fn)
void LCD_TRAMPOLINE_LINKAGE(trmp_tag_iter_fn) trmp_tag_iter_fn(struct request* rq, void* data, bool enable);

typedef void (*fptr_blk_mq_ops_exit_hctx)(struct blk_mq_hw_ctx* hctx, unsigned int hctx_idx);
typedef void (*fptr_impl_blk_mq_ops_exit_hctx)(fptr_blk_mq_ops_exit_hctx target, struct blk_mq_hw_ctx* hctx, unsigned int hctx_idx);

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_exit_hctx)
void LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_exit_hctx) trmp_blk_mq_ops_exit_hctx(struct blk_mq_hw_ctx* hctx, unsigned int hctx_idx);

typedef int (*fptr_blk_mq_ops_init_hctx)(struct blk_mq_hw_ctx* hctx, void* data, unsigned int hctx_idx);
typedef int (*fptr_impl_blk_mq_ops_init_hctx)(fptr_blk_mq_ops_init_hctx target, struct blk_mq_hw_ctx* hctx, void* data, unsigned int hctx_idx);

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_init_hctx)
int LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_init_hctx) trmp_blk_mq_ops_init_hctx(struct blk_mq_hw_ctx* hctx, void* data, unsigned int hctx_idx);

typedef int (*fptr_blk_mq_ops_init_request)(void* data, struct request* req, unsigned int hctx_idx, unsigned int rq_idx, unsigned int numa_node);
typedef int (*fptr_impl_blk_mq_ops_init_request)(fptr_blk_mq_ops_init_request target, void* data, struct request* req, unsigned int hctx_idx, unsigned int rq_idx, unsigned int numa_node);

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_init_request)
int LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_init_request) trmp_blk_mq_ops_init_request(void* data, struct request* req, unsigned int hctx_idx, unsigned int rq_idx, unsigned int numa_node);

typedef void (*fptr_blk_mq_ops_complete)(struct request* req);
typedef void (*fptr_impl_blk_mq_ops_complete)(fptr_blk_mq_ops_complete target, struct request* req);

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_complete)
void LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_complete) trmp_blk_mq_ops_complete(struct request* req);

typedef unsigned int (*fptr_pci_error_handlers_error_detected)(struct pci_dev* pdev, unsigned int state);
typedef unsigned int (*fptr_impl_pci_error_handlers_error_detected)(fptr_pci_error_handlers_error_detected target, struct pci_dev* pdev, unsigned int state);

LCD_TRAMPOLINE_DATA(trmp_pci_error_handlers_error_detected)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_pci_error_handlers_error_detected) trmp_pci_error_handlers_error_detected(struct pci_dev* pdev, unsigned int state);

typedef void (*fptr_pci_error_handlers_resume)(struct pci_dev* pdev);
typedef void (*fptr_impl_pci_error_handlers_resume)(fptr_pci_error_handlers_resume target, struct pci_dev* pdev);

LCD_TRAMPOLINE_DATA(trmp_pci_error_handlers_resume)
void LCD_TRAMPOLINE_LINKAGE(trmp_pci_error_handlers_resume) trmp_pci_error_handlers_resume(struct pci_dev* pdev);

typedef int (*fptr_pci_driver_sriov_configure)(struct pci_dev* pdev, int numvfs);
typedef int (*fptr_impl_pci_driver_sriov_configure)(fptr_pci_driver_sriov_configure target, struct pci_dev* pdev, int numvfs);

LCD_TRAMPOLINE_DATA(trmp_pci_driver_sriov_configure)
int LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_sriov_configure) trmp_pci_driver_sriov_configure(struct pci_dev* pdev, int numvfs);

typedef int (*fptr_blk_mq_ops_poll)(struct blk_mq_hw_ctx* hctx, unsigned int tag);
typedef int (*fptr_impl_blk_mq_ops_poll)(fptr_blk_mq_ops_poll target, struct blk_mq_hw_ctx* hctx, unsigned int tag);

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_poll)
int LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_poll) trmp_blk_mq_ops_poll(struct blk_mq_hw_ctx* hctx, unsigned int tag);

typedef int (*fptr_pci_driver_probe)(struct pci_dev* pdev, struct pci_device_id* ent);
typedef int (*fptr_impl_pci_driver_probe)(fptr_pci_driver_probe target, struct pci_dev* pdev, struct pci_device_id* ent);

LCD_TRAMPOLINE_DATA(trmp_pci_driver_probe)
int LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_probe) trmp_pci_driver_probe(struct pci_dev* pdev, struct pci_device_id* ent);

typedef int (*fptr_blk_mq_ops_queue_rq)(struct blk_mq_hw_ctx* hctx, struct blk_mq_queue_data* bd);
typedef int (*fptr_impl_blk_mq_ops_queue_rq)(fptr_blk_mq_ops_queue_rq target, struct blk_mq_hw_ctx* hctx, struct blk_mq_queue_data* bd);

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_queue_rq)
int LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_queue_rq) trmp_blk_mq_ops_queue_rq(struct blk_mq_hw_ctx* hctx, struct blk_mq_queue_data* bd);

typedef void (*fptr_pci_driver_remove)(struct pci_dev* pdev);
typedef void (*fptr_impl_pci_driver_remove)(fptr_pci_driver_remove target, struct pci_dev* pdev);

LCD_TRAMPOLINE_DATA(trmp_pci_driver_remove)
void LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_remove) trmp_pci_driver_remove(struct pci_dev* pdev);

typedef void (*fptr_pci_error_handlers_reset_notify)(struct pci_dev* pdev, bool prepare);
typedef void (*fptr_impl_pci_error_handlers_reset_notify)(fptr_pci_error_handlers_reset_notify target, struct pci_dev* pdev, bool prepare);

LCD_TRAMPOLINE_DATA(trmp_pci_error_handlers_reset_notify)
void LCD_TRAMPOLINE_LINKAGE(trmp_pci_error_handlers_reset_notify) trmp_pci_error_handlers_reset_notify(struct pci_dev* pdev, bool prepare);

typedef void (*fptr_pci_driver_shutdown)(struct pci_dev* pdev);
typedef void (*fptr_impl_pci_driver_shutdown)(fptr_pci_driver_shutdown target, struct pci_dev* pdev);

LCD_TRAMPOLINE_DATA(trmp_pci_driver_shutdown)
void LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_shutdown) trmp_pci_driver_shutdown(struct pci_dev* pdev);

typedef unsigned int (*fptr_pci_error_handlers_slot_reset)(struct pci_dev* pdev);
typedef unsigned int (*fptr_impl_pci_error_handlers_slot_reset)(fptr_pci_error_handlers_slot_reset target, struct pci_dev* pdev);

LCD_TRAMPOLINE_DATA(trmp_pci_error_handlers_slot_reset)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_pci_error_handlers_slot_reset) trmp_pci_error_handlers_slot_reset(struct pci_dev* pdev);

typedef unsigned int (*fptr_blk_mq_ops_timeout)(struct request* req, bool reserved);
typedef unsigned int (*fptr_impl_blk_mq_ops_timeout)(fptr_blk_mq_ops_timeout target, struct request* req, bool reserved);

LCD_TRAMPOLINE_DATA(trmp_blk_mq_ops_timeout)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_blk_mq_ops_timeout) trmp_blk_mq_ops_timeout(struct request* req, bool reserved);

typedef unsigned int (*fptr_thread_fn)(int irq, void* id);
typedef unsigned int (*fptr_impl_thread_fn)(fptr_thread_fn target, int irq, void* id);

LCD_TRAMPOLINE_DATA(trmp_thread_fn)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_thread_fn) trmp_thread_fn(int irq, void* id);

typedef unsigned int (*fptr_handler)(int irq, void* id);
typedef unsigned int (*fptr_impl_handler)(fptr_handler target, int irq, void* id);

LCD_TRAMPOLINE_DATA(trmp_handler)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_handler) trmp_handler(int irq, void* id);

struct __alloc_workqueue_key_call_ctx {
	char* fmt;
	unsigned int flags;
	int max_active;
	struct lock_class_key* key;
	char* lock_name;
};

struct __bitmap_weight_call_ctx {
	unsigned long* bitmap;
	unsigned int bits;
};

struct __pci_register_driver_call_ctx {
	struct pci_driver* drv;
	struct module* owner;
	char const* mod_name;
};

struct blk_cleanup_queue_call_ctx {
	struct request_queue* q;
};

struct done_call_ctx {
	struct request* rq;
	int id;
};

struct blk_execute_rq_nowait_call_ctx {
	struct request_queue* q;
	struct gendisk* bd_disk;
	struct request* rq;
	int at_head;
	fptr_done done;
};

struct blk_get_queue_call_ctx {
	struct request_queue* q;
};

struct blk_mq_alloc_tag_set_call_ctx {
	struct blk_mq_tag_set* set;
};

struct blk_mq_complete_request_call_ctx {
	struct request* rq;
	int error;
};

struct blk_mq_end_request_call_ctx {
	struct request* rq;
	int error;
};

struct blk_mq_free_request_call_ctx {
	struct request* rq;
};

struct blk_mq_free_tag_set_call_ctx {
	struct blk_mq_tag_set* set;
};

struct blk_mq_init_queue_call_ctx {
	struct blk_mq_tag_set* set;
};

struct blk_mq_ops_map_queue_call_ctx {
	struct request_queue* q;
	int cpu;
};

struct blk_mq_start_request_call_ctx {
	struct request* rq;
};

struct blk_mq_start_stopped_hw_queues_call_ctx {
	struct request_queue* q;
	bool async;
};

struct blk_mq_stop_hw_queues_call_ctx {
	struct request_queue* q;
};

struct tag_iter_fn_call_ctx {
	struct request* rq;
	void* data;
	bool enable;
};

struct blk_mq_tagset_busy_iter_call_ctx {
	struct blk_mq_tag_set* tagset;
	fptr_tag_iter_fn fn;
	void* priv;
};

struct blk_mq_update_nr_hw_queues_call_ctx {
	struct blk_mq_tag_set* set;
	int nr_hw_queues;
};

struct blk_put_queue_call_ctx {
	struct request_queue* q;
};

struct complete_call_ctx {
	struct completion* x;
};

struct del_timer_sync_call_ctx {
	struct timer_list* timer;
};

struct destroy_workqueue_call_ctx {
	struct workqueue_struct* wq;
};

struct device_release_driver_call_ctx {
	struct device* dev;
};

struct flush_work_call_ctx {
	struct work_struct* work;
};

struct free_irq_call_ctx {
	unsigned int irq;
	void* dev_id;
};

struct get_device_call_ctx {
	struct device* dev;
};

struct init_timer_key_call_ctx {
	struct timer_list* timer;
	unsigned int flags;
	char* name;
	struct lock_class_key* key;
};

struct ioremap_nocache_call_ctx {
	unsigned long long phys_addr;
	unsigned long size;
};

struct irq_set_affinity_hint_call_ctx {
	unsigned int irq;
	struct cpumask* m;
};

struct mod_timer_call_ctx {
	struct timer_list* timer;
	unsigned long expires;
};

struct blk_mq_ops_exit_hctx_call_ctx {
	struct blk_mq_hw_ctx* hctx;
	unsigned int hctx_idx;
};

struct blk_mq_ops_init_hctx_call_ctx {
	struct blk_mq_hw_ctx* hctx;
	void* data;
	unsigned int hctx_idx;
};

struct blk_mq_ops_init_request_call_ctx {
	void* data;
	struct request* req;
	unsigned int hctx_idx;
	unsigned int rq_idx;
	unsigned int numa_node;
};

struct nvme_alloc_request_call_ctx {
	struct request_queue* q;
	struct nvme_command* cmd;
	unsigned int flags;
	int qid;
};

struct nvme_cancel_request_call_ctx {
	struct request* req;
	void* data;
	bool reserved;
};

struct nvme_change_ctrl_state_call_ctx {
	struct nvme_ctrl* ctrl;
	unsigned int new_state;
};

struct nvme_complete_async_event_call_ctx {
	struct nvme_ctrl* ctrl;
	struct nvme_completion* cqe;
};

struct blk_mq_ops_complete_call_ctx {
	struct request* req;
};

struct nvme_disable_ctrl_call_ctx {
	struct nvme_ctrl* ctrl;
	unsigned long long cap;
};

struct nvme_enable_ctrl_call_ctx {
	struct nvme_ctrl* ctrl;
	unsigned long long cap;
};

struct pci_cleanup_aer_uncorrect_error_status_call_ctx {
	struct pci_dev* dev;
};

struct pci_error_handlers_error_detected_call_ctx {
	struct pci_dev* pdev;
	unsigned int state;
};

struct pci_error_handlers_resume_call_ctx {
	struct pci_dev* pdev;
};

struct nvme_init_ctrl_call_ctx {
	struct nvme_ctrl* ctrl;
	struct device* dev;
	struct nvme_ctrl_ops* ops;
	unsigned long quirks;
};

struct nvme_init_identify_call_ctx {
	struct nvme_ctrl* ctrl;
};

struct nvme_kill_queues_call_ctx {
	struct nvme_ctrl* ctrl;
};

struct pci_driver_sriov_configure_call_ctx {
	struct pci_dev* pdev;
	int numvfs;
};

struct blk_mq_ops_poll_call_ctx {
	struct blk_mq_hw_ctx* hctx;
	unsigned int tag;
};

struct pci_driver_probe_call_ctx {
	struct pci_dev* pdev;
	struct pci_device_id* ent;
};

struct nvme_put_ctrl_call_ctx {
	struct nvme_ctrl* ctrl;
};

struct nvme_queue_async_events_call_ctx {
	struct nvme_ctrl* ctrl;
};

struct blk_mq_ops_queue_rq_call_ctx {
	struct blk_mq_hw_ctx* hctx;
	struct blk_mq_queue_data* bd;
};

struct nvme_queue_scan_call_ctx {
	struct nvme_ctrl* ctrl;
};

struct pci_driver_remove_call_ctx {
	struct pci_dev* pdev;
};

struct nvme_remove_namespaces_call_ctx {
	struct nvme_ctrl* ctrl;
};

struct nvme_requeue_req_call_ctx {
	struct request* req;
};

struct pci_error_handlers_reset_notify_call_ctx {
	struct pci_dev* pdev;
	bool prepare;
};

struct nvme_set_queue_count_call_ctx {
	struct nvme_ctrl* ctrl;
	int* count;
};

struct nvme_setup_cmd_call_ctx {
	struct nvme_ns* ns;
	struct request* req;
	struct nvme_command* cmd;
};

struct pci_driver_shutdown_call_ctx {
	struct pci_dev* pdev;
};

struct nvme_shutdown_ctrl_call_ctx {
	struct nvme_ctrl* ctrl;
};

struct pci_error_handlers_slot_reset_call_ctx {
	struct pci_dev* pdev;
};

struct nvme_start_queues_call_ctx {
	struct nvme_ctrl* ctrl;
};

struct nvme_stop_queues_call_ctx {
	struct nvme_ctrl* ctrl;
};

struct nvme_submit_sync_cmd_call_ctx {
	struct request_queue* q;
	struct nvme_command* cmd;
	void* buffer;
	unsigned int bufflen;
};

struct blk_mq_ops_timeout_call_ctx {
	struct request* req;
	bool reserved;
};

struct nvme_uninit_ctrl_call_ctx {
	struct nvme_ctrl* ctrl;
};

struct pci_device_is_present_call_ctx {
	struct pci_dev* pdev;
};

struct pci_disable_device_call_ctx {
	struct pci_dev* dev;
};

struct pci_disable_msi_call_ctx {
	struct pci_dev* dev;
};

struct pci_disable_msix_call_ctx {
	struct pci_dev* dev;
};

struct pci_enable_device_mem_call_ctx {
	struct pci_dev* dev;
};

struct pci_enable_msi_range_call_ctx {
	struct pci_dev* dev;
	int minvec;
	int maxvec;
};

struct pci_enable_msix_call_ctx {
	struct pci_dev* dev;
	struct msix_entry* entries;
	int minvec;
	int maxvec;
};

struct pci_enable_msix_range_call_ctx {
	struct pci_dev* dev;
	struct msix_entry* entries;
	int minvec;
	int maxvec;
};

struct pci_enable_pcie_error_reporting_call_ctx {
	struct pci_dev* dev;
};

struct pci_disable_pcie_error_reporting_call_ctx {
	struct pci_dev* dev;
};

struct pci_release_selected_regions_call_ctx {
	struct pci_dev* pdev;
	int bars;
};

struct pci_request_selected_regions_call_ctx {
	struct pci_dev* pdev;
	int bars;
	char const* res_name;
};

struct pci_restore_state_call_ctx {
	struct pci_dev* dev;
};

struct pci_save_state_call_ctx {
	struct pci_dev* dev;
};

struct pci_select_bars_call_ctx {
	struct pci_dev* dev;
	unsigned long flags;
};

struct pci_set_master_call_ctx {
	struct pci_dev* dev;
};

struct pci_unregister_driver_call_ctx {
	struct pci_driver* drv;
};

struct put_device_call_ctx {
	struct device* dev;
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

struct round_jiffies_call_ctx {
	unsigned long j;
};

struct wait_for_completion_io_timeout_call_ctx {
	struct completion* x;
	unsigned long timeout;
};

struct work_busy_call_ctx {
	struct work_struct* work;
};

struct blk_mq_map_queue_call_ctx {
	struct request_queue* q;
	int cpu;
};

void caller_marshal_kernel____alloc_workqueue_key__ret_workqueue_struct__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __alloc_workqueue_key_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void callee_unmarshal_kernel____alloc_workqueue_key__ret_workqueue_struct__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __alloc_workqueue_key_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void callee_marshal_kernel____alloc_workqueue_key__ret_workqueue_struct__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __alloc_workqueue_key_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void caller_unmarshal_kernel____alloc_workqueue_key__ret_workqueue_struct__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __alloc_workqueue_key_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void caller_marshal_kernel____alloc_workqueue_key__key__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __alloc_workqueue_key_call_ctx const* call_ctx,
	struct lock_class_key const* ptr);

void callee_unmarshal_kernel____alloc_workqueue_key__key__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __alloc_workqueue_key_call_ctx const* call_ctx,
	struct lock_class_key* ptr);

void callee_marshal_kernel____alloc_workqueue_key__key__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __alloc_workqueue_key_call_ctx const* call_ctx,
	struct lock_class_key const* ptr);

void caller_unmarshal_kernel____alloc_workqueue_key__key__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __alloc_workqueue_key_call_ctx const* call_ctx,
	struct lock_class_key* ptr);

void caller_marshal_kernel___global_pci_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver const* ptr);

void callee_unmarshal_kernel___global_pci_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver* ptr);

void callee_marshal_kernel___global_pci_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver const* ptr);

void caller_unmarshal_kernel___global_pci_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver* ptr);

void caller_marshal_kernel__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_device_id const* ptr);

void callee_unmarshal_kernel__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_device_id* ptr);

void callee_marshal_kernel__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_device_id const* ptr);

void caller_unmarshal_kernel__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_device_id* ptr);

void caller_marshal_kernel___global_pci_error_handlers__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers const* ptr);

void callee_unmarshal_kernel___global_pci_error_handlers__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers* ptr);

void callee_marshal_kernel___global_pci_error_handlers__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers const* ptr);

void caller_unmarshal_kernel___global_pci_error_handlers__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers* ptr);

void caller_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module const* ptr);

void callee_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module* ptr);

void callee_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module const* ptr);

void caller_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module* ptr);

void caller_marshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_cleanup_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_cleanup_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_cleanup_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_cleanup_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__done__rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct done_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__done__rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct done_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__done__rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct done_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__done__rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct done_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__blk_execute_rq_nowait__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_execute_rq_nowait__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_execute_rq_nowait__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_execute_rq_nowait__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_execute_rq_nowait__bd_disk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void callee_unmarshal_kernel__blk_execute_rq_nowait__bd_disk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct gendisk* ptr);

void callee_marshal_kernel__blk_execute_rq_nowait__bd_disk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void caller_unmarshal_kernel__blk_execute_rq_nowait__bd_disk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct gendisk* ptr);

void caller_marshal_kernel__blk_execute_rq_nowait__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__blk_execute_rq_nowait__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__blk_execute_rq_nowait__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__blk_execute_rq_nowait__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__blk_execute_rq_nowait__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_execute_rq_nowait__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_execute_rq_nowait__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_execute_rq_nowait__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_get_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_get_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_get_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_get_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_get_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_get_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_get_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_get_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_mq_alloc_tag_set__set__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void callee_unmarshal_kernel__blk_mq_alloc_tag_set__set__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void callee_marshal_kernel__blk_mq_alloc_tag_set__set__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void caller_unmarshal_kernel__blk_mq_alloc_tag_set__set__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void caller_marshal_kernel___global_blk_mq_ops__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops const* ptr);

void callee_unmarshal_kernel___global_blk_mq_ops__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops* ptr);

void callee_marshal_kernel___global_blk_mq_ops__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops const* ptr);

void caller_unmarshal_kernel___global_blk_mq_ops__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops* ptr);

void caller_marshal_kernel__blk_mq_alloc_tag_set__blk_mq_tag_set_tags__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tags const* ptr);

void callee_unmarshal_kernel__blk_mq_alloc_tag_set__blk_mq_tag_set_tags__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tags* ptr);

void callee_marshal_kernel__blk_mq_alloc_tag_set__blk_mq_tag_set_tags__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tags const* ptr);

void caller_unmarshal_kernel__blk_mq_alloc_tag_set__blk_mq_tag_set_tags__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tags* ptr);

void caller_marshal_kernel__blk_mq_complete_request__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__blk_mq_complete_request__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__blk_mq_complete_request__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__blk_mq_complete_request__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__blk_mq_complete_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_mq_complete_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_mq_complete_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_mq_complete_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_mq_complete_request__request_bio__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct bio const* ptr);

void callee_unmarshal_kernel__blk_mq_complete_request__request_bio__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct bio* ptr);

void callee_marshal_kernel__blk_mq_complete_request__request_bio__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct bio const* ptr);

void caller_unmarshal_kernel__blk_mq_complete_request__request_bio__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct bio* ptr);

void caller_marshal_kernel__blk_mq_complete_request__request_rq_disk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void callee_unmarshal_kernel__blk_mq_complete_request__request_rq_disk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct gendisk* ptr);

void callee_marshal_kernel__blk_mq_complete_request__request_rq_disk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void caller_unmarshal_kernel__blk_mq_complete_request__request_rq_disk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* call_ctx,
	struct gendisk* ptr);

void caller_marshal_kernel__blk_mq_end_request__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__blk_mq_end_request__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__blk_mq_end_request__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__blk_mq_end_request__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__blk_mq_end_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_mq_end_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_mq_end_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_mq_end_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_mq_end_request__request_bio__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct bio const* ptr);

void callee_unmarshal_kernel__blk_mq_end_request__request_bio__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct bio* ptr);

void callee_marshal_kernel__blk_mq_end_request__request_bio__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct bio const* ptr);

void caller_unmarshal_kernel__blk_mq_end_request__request_bio__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct bio* ptr);

void caller_marshal_kernel__blk_mq_end_request__request_rq_disk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void callee_unmarshal_kernel__blk_mq_end_request__request_rq_disk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct gendisk* ptr);

void callee_marshal_kernel__blk_mq_end_request__request_rq_disk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void caller_unmarshal_kernel__blk_mq_end_request__request_rq_disk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* call_ctx,
	struct gendisk* ptr);

void caller_marshal_kernel__blk_mq_free_request__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__blk_mq_free_request__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__blk_mq_free_request__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__blk_mq_free_request__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__blk_mq_free_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_mq_free_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_mq_free_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_mq_free_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_free_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void callee_unmarshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_free_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void callee_marshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_free_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void caller_unmarshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_free_tag_set_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void caller_marshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void callee_unmarshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void callee_marshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void caller_unmarshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void caller_marshal_kernel__blk_mq_ops_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void callee_unmarshal_kernel__blk_mq_ops_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void callee_marshal_kernel__blk_mq_ops_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void caller_unmarshal_kernel__blk_mq_ops_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void caller_marshal_kernel__blk_mq_ops_map_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_mq_ops_map_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_mq_ops_map_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_mq_ops_map_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__blk_mq_start_request__request_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_mq_start_request__request_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_mq_start_request__request_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_mq_start_request__request_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_mq_start_stopped_hw_queues__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_start_stopped_hw_queues_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_mq_start_stopped_hw_queues__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_start_stopped_hw_queues_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_mq_start_stopped_hw_queues__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_start_stopped_hw_queues_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_mq_start_stopped_hw_queues__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_start_stopped_hw_queues_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_mq_stop_hw_queues__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_stop_hw_queues_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_mq_stop_hw_queues__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_stop_hw_queues_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_mq_stop_hw_queues__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_stop_hw_queues_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_mq_stop_hw_queues__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_stop_hw_queues_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__tag_iter_fn__rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct tag_iter_fn_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__tag_iter_fn__rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct tag_iter_fn_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__tag_iter_fn__rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct tag_iter_fn_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__tag_iter_fn__rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct tag_iter_fn_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__blk_mq_tagset_busy_iter__tagset__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_tagset_busy_iter_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void callee_unmarshal_kernel__blk_mq_tagset_busy_iter__tagset__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_tagset_busy_iter_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void callee_marshal_kernel__blk_mq_tagset_busy_iter__tagset__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_tagset_busy_iter_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void caller_unmarshal_kernel__blk_mq_tagset_busy_iter__tagset__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_tagset_busy_iter_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void caller_marshal_kernel__blk_mq_update_nr_hw_queues__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_update_nr_hw_queues_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void callee_unmarshal_kernel__blk_mq_update_nr_hw_queues__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_update_nr_hw_queues_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void callee_marshal_kernel__blk_mq_update_nr_hw_queues__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_update_nr_hw_queues_call_ctx const* call_ctx,
	struct blk_mq_tag_set const* ptr);

void caller_unmarshal_kernel__blk_mq_update_nr_hw_queues__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_update_nr_hw_queues_call_ctx const* call_ctx,
	struct blk_mq_tag_set* ptr);

void caller_marshal_kernel__blk_put_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_put_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_put_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_put_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_put_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_put_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_put_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_put_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__complete__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct complete_call_ctx const* call_ctx,
	struct completion const* ptr);

void callee_unmarshal_kernel__complete__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct complete_call_ctx const* call_ctx,
	struct completion* ptr);

void callee_marshal_kernel__complete__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct complete_call_ctx const* call_ctx,
	struct completion const* ptr);

void caller_unmarshal_kernel__complete__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct complete_call_ctx const* call_ctx,
	struct completion* ptr);

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

void caller_marshal_kernel__destroy_workqueue__wq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct destroy_workqueue_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void callee_unmarshal_kernel__destroy_workqueue__wq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct destroy_workqueue_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void callee_marshal_kernel__destroy_workqueue__wq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct destroy_workqueue_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void caller_unmarshal_kernel__destroy_workqueue__wq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct destroy_workqueue_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void caller_marshal_kernel__device_release_driver__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct device_release_driver_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__device_release_driver__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct device_release_driver_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__device_release_driver__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct device_release_driver_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__device_release_driver__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct device_release_driver_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__flush_work__work__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct flush_work_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void callee_unmarshal_kernel__flush_work__work__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct flush_work_call_ctx const* call_ctx,
	struct work_struct* ptr);

void callee_marshal_kernel__flush_work__work__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct flush_work_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void caller_unmarshal_kernel__flush_work__work__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct flush_work_call_ctx const* call_ctx,
	struct work_struct* ptr);

void caller_marshal_kernel__flush_work__list_head__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct flush_work_call_ctx const* call_ctx,
	struct list_head const* ptr);

void callee_unmarshal_kernel__flush_work__list_head__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct flush_work_call_ctx const* call_ctx,
	struct list_head* ptr);

void callee_marshal_kernel__flush_work__list_head__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct flush_work_call_ctx const* call_ctx,
	struct list_head const* ptr);

void caller_unmarshal_kernel__flush_work__list_head__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct flush_work_call_ctx const* call_ctx,
	struct list_head* ptr);

void caller_marshal_kernel__get_device__ret_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_device_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__get_device__ret_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_device_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__get_device__ret_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_device_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__get_device__ret_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_device_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__get_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_device_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__get_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_device_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__get_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_device_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__get_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_device_call_ctx const* call_ctx,
	struct device* ptr);

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

void caller_marshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* call_ctx,
	struct cpumask const* ptr);

void callee_unmarshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* call_ctx,
	struct cpumask* ptr);

void callee_marshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* call_ctx,
	struct cpumask const* ptr);

void caller_unmarshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* call_ctx,
	struct cpumask* ptr);

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

void caller_marshal_kernel__blk_mq_ops_exit_hctx__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_exit_hctx_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void callee_unmarshal_kernel__blk_mq_ops_exit_hctx__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_exit_hctx_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void callee_marshal_kernel__blk_mq_ops_exit_hctx__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_exit_hctx_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void caller_unmarshal_kernel__blk_mq_ops_exit_hctx__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_exit_hctx_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void caller_marshal_kernel__blk_mq_ops_init_hctx__hctx__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_init_hctx_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void callee_unmarshal_kernel__blk_mq_ops_init_hctx__hctx__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_init_hctx_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void callee_marshal_kernel__blk_mq_ops_init_hctx__hctx__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_init_hctx_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void caller_unmarshal_kernel__blk_mq_ops_init_hctx__hctx__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_init_hctx_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void caller_marshal_kernel__blk_mq_ops_init_request__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_init_request_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__blk_mq_ops_init_request__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_init_request_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__blk_mq_ops_init_request__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_init_request_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__blk_mq_ops_init_request__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_init_request_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__nvme_alloc_request__ret_request__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__nvme_alloc_request__ret_request__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__nvme_alloc_request__ret_request__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__nvme_alloc_request__ret_request__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__nvme_alloc_request__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__nvme_alloc_request__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__nvme_alloc_request__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__nvme_alloc_request__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__nvme_alloc_request__cmd__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* call_ctx,
	struct nvme_command const* ptr);

void callee_unmarshal_kernel__nvme_alloc_request__cmd__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* call_ctx,
	struct nvme_command* ptr);

void callee_marshal_kernel__nvme_alloc_request__cmd__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* call_ctx,
	struct nvme_command const* ptr);

void caller_unmarshal_kernel__nvme_alloc_request__cmd__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* call_ctx,
	struct nvme_command* ptr);

void caller_marshal_kernel__nvme_cancel_request__req__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__nvme_cancel_request__req__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__nvme_cancel_request__req__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__nvme_cancel_request__req__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__nvme_cancel_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__nvme_cancel_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__nvme_cancel_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__nvme_cancel_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__nvme_cancel_request__request_bio__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct bio const* ptr);

void callee_unmarshal_kernel__nvme_cancel_request__request_bio__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct bio* ptr);

void callee_marshal_kernel__nvme_cancel_request__request_bio__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct bio const* ptr);

void caller_unmarshal_kernel__nvme_cancel_request__request_bio__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct bio* ptr);

void caller_marshal_kernel__nvme_cancel_request__request_rq_disk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void callee_unmarshal_kernel__nvme_cancel_request__request_rq_disk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct gendisk* ptr);

void callee_marshal_kernel__nvme_cancel_request__request_rq_disk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct gendisk const* ptr);

void caller_unmarshal_kernel__nvme_cancel_request__request_rq_disk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* call_ctx,
	struct gendisk* ptr);

void caller_marshal_kernel__nvme_change_ctrl_state__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_change_ctrl_state_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_change_ctrl_state__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_change_ctrl_state_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_change_ctrl_state__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_change_ctrl_state_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_change_ctrl_state__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_change_ctrl_state_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__nvme_complete_async_event__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_complete_async_event__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_complete_async_event__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_complete_async_event__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__nvme_complete_async_event__cqe__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* call_ctx,
	struct nvme_completion const* ptr);

void callee_unmarshal_kernel__nvme_complete_async_event__cqe__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* call_ctx,
	struct nvme_completion* ptr);

void callee_marshal_kernel__nvme_complete_async_event__cqe__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* call_ctx,
	struct nvme_completion const* ptr);

void caller_unmarshal_kernel__nvme_complete_async_event__cqe__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* call_ctx,
	struct nvme_completion* ptr);

void caller_marshal_kernel__blk_mq_ops_complete__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_complete_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__blk_mq_ops_complete__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_complete_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__blk_mq_ops_complete__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_complete_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__blk_mq_ops_complete__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_complete_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__nvme_disable_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_disable_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_disable_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_disable_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_disable_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_disable_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_disable_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_disable_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__nvme_enable_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_enable_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_enable_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_enable_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_enable_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_enable_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_enable_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_enable_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__pci_cleanup_aer_uncorrect_error_status__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_cleanup_aer_uncorrect_error_status_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_cleanup_aer_uncorrect_error_status__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_cleanup_aer_uncorrect_error_status_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_cleanup_aer_uncorrect_error_status__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_cleanup_aer_uncorrect_error_status_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_cleanup_aer_uncorrect_error_status__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_cleanup_aer_uncorrect_error_status_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_error_handlers_error_detected__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_error_detected_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_error_handlers_error_detected__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_error_detected_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_error_handlers_error_detected__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_error_detected_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_error_handlers_error_detected__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_error_detected_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_error_handlers_resume__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_resume_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_error_handlers_resume__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_resume_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_error_handlers_resume__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_resume_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_error_handlers_resume__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_resume_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__nvme_init_ctrl__ctrl__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_init_ctrl__ctrl__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_init_ctrl__ctrl__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_init_ctrl__ctrl__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__nvme_init_ctrl__kref__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct kref const* ptr);

void callee_unmarshal_kernel__nvme_init_ctrl__kref__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct kref* ptr);

void callee_marshal_kernel__nvme_init_ctrl__kref__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct kref const* ptr);

void caller_unmarshal_kernel__nvme_init_ctrl__kref__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct kref* ptr);

void caller_marshal_kernel__nvme_init_ctrl__nvme_ctrl_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__nvme_init_ctrl__nvme_ctrl_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__nvme_init_ctrl__nvme_ctrl_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__nvme_init_ctrl__nvme_ctrl_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__nvme_init_ctrl__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__nvme_init_ctrl__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__nvme_init_ctrl__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__nvme_init_ctrl__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__nvme_init_ctrl__ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl_ops const* ptr);

void callee_unmarshal_kernel__nvme_init_ctrl__ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl_ops* ptr);

void callee_marshal_kernel__nvme_init_ctrl__ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl_ops const* ptr);

void caller_unmarshal_kernel__nvme_init_ctrl__ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl_ops* ptr);

void caller_marshal_kernel__nvme_init_identify__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_init_identify__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_init_identify__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_init_identify__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__nvme_init_identify__nvme_ctrl_admin_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__nvme_init_identify__nvme_ctrl_admin_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__nvme_init_identify__nvme_ctrl_admin_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__nvme_init_identify__nvme_ctrl_admin_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__nvme_init_identify__nvme_ctrl_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__nvme_init_identify__nvme_ctrl_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__nvme_init_identify__nvme_ctrl_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__nvme_init_identify__nvme_ctrl_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__nvme_init_identify__atomic64_t__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct atomic64_t const* ptr);

void callee_unmarshal_kernel__nvme_init_identify__atomic64_t__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct atomic64_t* ptr);

void callee_marshal_kernel__nvme_init_identify__atomic64_t__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct atomic64_t const* ptr);

void caller_unmarshal_kernel__nvme_init_identify__atomic64_t__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* call_ctx,
	struct atomic64_t* ptr);

void caller_marshal_kernel__nvme_kill_queues__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_kill_queues_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_kill_queues__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_kill_queues_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_kill_queues__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_kill_queues_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_kill_queues__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_kill_queues_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__pci_driver_sriov_configure__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_sriov_configure_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_driver_sriov_configure__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_sriov_configure_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_driver_sriov_configure__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_sriov_configure_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_driver_sriov_configure__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_sriov_configure_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__blk_mq_ops_poll__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_poll_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void callee_unmarshal_kernel__blk_mq_ops_poll__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_poll_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void callee_marshal_kernel__blk_mq_ops_poll__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_poll_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void caller_unmarshal_kernel__blk_mq_ops_poll__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_poll_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void caller_marshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__pci_driver_probe__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__pci_driver_probe__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__pci_driver_probe__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__pci_driver_probe__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__pci_driver_probe__kobject__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct kobject const* ptr);

void callee_unmarshal_kernel__pci_driver_probe__kobject__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct kobject* ptr);

void callee_marshal_kernel__pci_driver_probe__kobject__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct kobject const* ptr);

void caller_unmarshal_kernel__pci_driver_probe__kobject__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct kobject* ptr);

void caller_marshal_kernel__pci_driver_probe__resource__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct resource const* ptr);

void callee_unmarshal_kernel__pci_driver_probe__resource__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct resource* ptr);

void callee_marshal_kernel__pci_driver_probe__resource__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct resource const* ptr);

void caller_unmarshal_kernel__pci_driver_probe__resource__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct resource* ptr);

void caller_marshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void callee_unmarshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void callee_marshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void caller_unmarshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void caller_marshal_kernel__nvme_put_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_put_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_put_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_put_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__nvme_put_ctrl__kref__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* call_ctx,
	struct kref const* ptr);

void callee_unmarshal_kernel__nvme_put_ctrl__kref__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* call_ctx,
	struct kref* ptr);

void callee_marshal_kernel__nvme_put_ctrl__kref__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* call_ctx,
	struct kref const* ptr);

void caller_unmarshal_kernel__nvme_put_ctrl__kref__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* call_ctx,
	struct kref* ptr);

void caller_marshal_kernel__nvme_queue_async_events__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_queue_async_events_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_queue_async_events__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_queue_async_events_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_queue_async_events__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_queue_async_events_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_queue_async_events__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_queue_async_events_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__blk_mq_ops_queue_rq__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void callee_unmarshal_kernel__blk_mq_ops_queue_rq__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void callee_marshal_kernel__blk_mq_ops_queue_rq__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void caller_unmarshal_kernel__blk_mq_ops_queue_rq__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void caller_marshal_kernel__blk_mq_ops_queue_rq__blk_mq_hw_ctx_queue__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_mq_ops_queue_rq__blk_mq_hw_ctx_queue__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_mq_ops_queue_rq__blk_mq_hw_ctx_queue__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_mq_ops_queue_rq__blk_mq_hw_ctx_queue__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__blk_mq_ops_queue_rq__bd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct blk_mq_queue_data const* ptr);

void callee_unmarshal_kernel__blk_mq_ops_queue_rq__bd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct blk_mq_queue_data* ptr);

void callee_marshal_kernel__blk_mq_ops_queue_rq__bd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct blk_mq_queue_data const* ptr);

void caller_unmarshal_kernel__blk_mq_ops_queue_rq__bd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct blk_mq_queue_data* ptr);

void caller_marshal_kernel__blk_mq_ops_queue_rq__blk_mq_queue_data_rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__blk_mq_ops_queue_rq__blk_mq_queue_data_rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__blk_mq_ops_queue_rq__blk_mq_queue_data_rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__blk_mq_ops_queue_rq__blk_mq_queue_data_rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__nvme_queue_scan__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_queue_scan_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_queue_scan__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_queue_scan_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_queue_scan__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_queue_scan_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_queue_scan__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_queue_scan_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_remove_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_remove_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_remove_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_remove_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__nvme_remove_namespaces__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_remove_namespaces_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_remove_namespaces__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_remove_namespaces_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_remove_namespaces__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_remove_namespaces_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_remove_namespaces__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_remove_namespaces_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__nvme_requeue_req__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__nvme_requeue_req__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__nvme_requeue_req__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__nvme_requeue_req__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__nvme_requeue_req__request_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__nvme_requeue_req__request_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__nvme_requeue_req__request_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__nvme_requeue_req__request_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__pci_error_handlers_reset_notify__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_reset_notify_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_error_handlers_reset_notify__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_reset_notify_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_error_handlers_reset_notify__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_reset_notify_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_error_handlers_reset_notify__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_reset_notify_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__nvme_set_queue_count__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_set_queue_count__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_set_queue_count__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_set_queue_count__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__nvme_set_queue_count__nvme_ctrl_admin_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__nvme_set_queue_count__nvme_ctrl_admin_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__nvme_set_queue_count__nvme_ctrl_admin_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__nvme_set_queue_count__nvme_ctrl_admin_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__nvme_setup_cmd__ns__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct nvme_ns const* ptr);

void callee_unmarshal_kernel__nvme_setup_cmd__ns__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct nvme_ns* ptr);

void callee_marshal_kernel__nvme_setup_cmd__ns__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct nvme_ns const* ptr);

void caller_unmarshal_kernel__nvme_setup_cmd__ns__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct nvme_ns* ptr);

void caller_marshal_kernel__nvme_setup_cmd__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__nvme_setup_cmd__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__nvme_setup_cmd__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__nvme_setup_cmd__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__nvme_setup_cmd__request_bio__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct bio const* ptr);

void callee_unmarshal_kernel__nvme_setup_cmd__request_bio__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct bio* ptr);

void callee_marshal_kernel__nvme_setup_cmd__request_bio__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct bio const* ptr);

void caller_unmarshal_kernel__nvme_setup_cmd__request_bio__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct bio* ptr);

void caller_marshal_kernel__nvme_setup_cmd__cmd__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct nvme_command const* ptr);

void callee_unmarshal_kernel__nvme_setup_cmd__cmd__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct nvme_command* ptr);

void callee_marshal_kernel__nvme_setup_cmd__cmd__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct nvme_command const* ptr);

void caller_unmarshal_kernel__nvme_setup_cmd__cmd__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* call_ctx,
	struct nvme_command* ptr);

void caller_marshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_shutdown_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_shutdown_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_shutdown_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_shutdown_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__nvme_shutdown_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_shutdown_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_shutdown_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_shutdown_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_shutdown_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_shutdown_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_shutdown_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_shutdown_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__pci_error_handlers_slot_reset__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_slot_reset_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_error_handlers_slot_reset__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_slot_reset_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_error_handlers_slot_reset__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_slot_reset_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_error_handlers_slot_reset__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_slot_reset_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__nvme_start_queues__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_start_queues_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_start_queues__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_start_queues_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_start_queues__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_start_queues_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_start_queues__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_start_queues_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__nvme_stop_queues__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_stop_queues_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_stop_queues__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_stop_queues_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_stop_queues__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_stop_queues_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_stop_queues__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_stop_queues_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__nvme_submit_sync_cmd__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__nvme_submit_sync_cmd__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__nvme_submit_sync_cmd__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__nvme_submit_sync_cmd__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* call_ctx,
	struct request_queue* ptr);

void caller_marshal_kernel__nvme_submit_sync_cmd__cmd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* call_ctx,
	struct nvme_command const* ptr);

void callee_unmarshal_kernel__nvme_submit_sync_cmd__cmd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* call_ctx,
	struct nvme_command* ptr);

void callee_marshal_kernel__nvme_submit_sync_cmd__cmd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* call_ctx,
	struct nvme_command const* ptr);

void caller_unmarshal_kernel__nvme_submit_sync_cmd__cmd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* call_ctx,
	struct nvme_command* ptr);

void caller_marshal_kernel__blk_mq_ops_timeout__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_timeout_call_ctx const* call_ctx,
	struct request const* ptr);

void callee_unmarshal_kernel__blk_mq_ops_timeout__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_timeout_call_ctx const* call_ctx,
	struct request* ptr);

void callee_marshal_kernel__blk_mq_ops_timeout__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_timeout_call_ctx const* call_ctx,
	struct request const* ptr);

void caller_unmarshal_kernel__blk_mq_ops_timeout__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_timeout_call_ctx const* call_ctx,
	struct request* ptr);

void caller_marshal_kernel__nvme_uninit_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_uninit_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void callee_unmarshal_kernel__nvme_uninit_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_uninit_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void callee_marshal_kernel__nvme_uninit_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_uninit_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl const* ptr);

void caller_unmarshal_kernel__nvme_uninit_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_uninit_ctrl_call_ctx const* call_ctx,
	struct nvme_ctrl* ptr);

void caller_marshal_kernel__pci_device_is_present__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_device_is_present_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_device_is_present__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_device_is_present_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_device_is_present__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_device_is_present_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_device_is_present__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_device_is_present_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_disable_device__atomic64_t__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct atomic64_t const* ptr);

void callee_unmarshal_kernel__pci_disable_device__atomic64_t__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct atomic64_t* ptr);

void callee_marshal_kernel__pci_disable_device__atomic64_t__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct atomic64_t const* ptr);

void caller_unmarshal_kernel__pci_disable_device__atomic64_t__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct atomic64_t* ptr);

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

void caller_marshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_enable_device_mem__atomic64_t__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct atomic64_t const* ptr);

void callee_unmarshal_kernel__pci_enable_device_mem__atomic64_t__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct atomic64_t* ptr);

void callee_marshal_kernel__pci_enable_device_mem__atomic64_t__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct atomic64_t const* ptr);

void caller_unmarshal_kernel__pci_enable_device_mem__atomic64_t__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct atomic64_t* ptr);

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

void caller_marshal_kernel__pci_enable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_enable_msix__entries__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* call_ctx,
	struct msix_entry const* ptr);

void callee_unmarshal_kernel__pci_enable_msix__entries__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* call_ctx,
	struct msix_entry* ptr);

void callee_marshal_kernel__pci_enable_msix__entries__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* call_ctx,
	struct msix_entry const* ptr);

void caller_unmarshal_kernel__pci_enable_msix__entries__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* call_ctx,
	struct msix_entry* ptr);

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

void caller_marshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct msix_entry const* ptr);

void callee_unmarshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct msix_entry* ptr);

void callee_marshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct msix_entry const* ptr);

void caller_unmarshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct msix_entry* ptr);

void caller_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void callee_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void callee_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void caller_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void caller_marshal_kernel__put_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct put_device_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__put_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct put_device_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__put_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct put_device_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__put_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct put_device_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void callee_unmarshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void callee_marshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void caller_unmarshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void caller_marshal_kernel__queue_work_on__work__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void callee_unmarshal_kernel__queue_work_on__work__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct* ptr);

void callee_marshal_kernel__queue_work_on__work__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void caller_unmarshal_kernel__queue_work_on__work__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct* ptr);

void caller_marshal_kernel__wait_for_completion_io_timeout__x__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct wait_for_completion_io_timeout_call_ctx const* call_ctx,
	struct completion const* ptr);

void callee_unmarshal_kernel__wait_for_completion_io_timeout__x__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct wait_for_completion_io_timeout_call_ctx const* call_ctx,
	struct completion* ptr);

void callee_marshal_kernel__wait_for_completion_io_timeout__x__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct wait_for_completion_io_timeout_call_ctx const* call_ctx,
	struct completion const* ptr);

void caller_unmarshal_kernel__wait_for_completion_io_timeout__x__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct wait_for_completion_io_timeout_call_ctx const* call_ctx,
	struct completion* ptr);

void caller_marshal_kernel__work_busy__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct work_busy_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void callee_unmarshal_kernel__work_busy__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct work_busy_call_ctx const* call_ctx,
	struct work_struct* ptr);

void callee_marshal_kernel__work_busy__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct work_busy_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void caller_unmarshal_kernel__work_busy__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct work_busy_call_ctx const* call_ctx,
	struct work_struct* ptr);

void caller_marshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void callee_unmarshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void callee_marshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx const* ptr);

void caller_unmarshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct blk_mq_hw_ctx* ptr);

void caller_marshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void callee_unmarshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);

void callee_marshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct request_queue const* ptr);

void caller_unmarshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* call_ctx,
	struct request_queue* ptr);


#endif
