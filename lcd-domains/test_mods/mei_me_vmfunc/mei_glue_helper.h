#ifndef __MEI_GLUE_HELPER_H__
#define __MEI_GLUE_HELPER_H__

#include <linux/kthread.h>
#include <linux/module.h>
#include <libcap.h>
#include <libfipc.h>
#include <thc_ipc.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/pci.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <linux/interrupt.h>

#include "mei_dev.h"
#include "hw-me.h"

struct pcidev_info {
	unsigned int domain, bus, slot, fn;
};

enum dispatch_t {
	_COND_RESCHED,
	FREE_IRQ,
	MEI_CANCEL_WORK,
	MEI_DEREGISTER,
	MEI_DEVICE_INIT,
	MEI_HBM_PG,
	MEI_HBM_PG_RESUME,
	MEI_IRQ_COMPL_HANDLER,
	MEI_IRQ_READ_HANDLER,
	MEI_IRQ_WRITE_HANDLER,
	MEI_REGISTER,
	MEI_RESTART,
	MEI_START,
	MEI_STOP,
	MEI_WRITE_IS_IDLE,
	PCI_DISABLE_DEVICE,
	PCI_DISABLE_MSI,
	PCI_ENABLE_DEVICE,
	PCI_ENABLE_MSI_RANGE,
	PCI_IOMAP,
	PCI_IOUNMAP,
	__PCI_REGISTER_DRIVER,
	PCI_UNREGISTER_DRIVER,
	PCI_RELEASE_REGIONS,
	PCI_REQUEST_REGIONS,
	PCI_SET_MASTER,
	FINISH_WAIT,
	PREPARE_TO_WAIT_EVENT,
	QUEUE_DELAYED_WORK_ON,
	QUEUE_WORK_ON,
	REQUEST_THREADED_IRQ,
	SCHEDULE_TIMEOUT,
	__WAKE_UP,
	HOST_IS_READY,
	HW_IS_READY,
	HW_RESET,
	HW_START,
	HW_CONFIG,
	FW_STATUS,
	PG_STATE,
	PG_IN_TRANSITION,
	PG_IS_ENABLED,
	INTR_CLEAR,
	INTR_ENABLE,
	INTR_DISABLE,
	HBUF_FREE_SLOTS,
	HBUF_IS_READY,
	HBUF_MAX_LEN,
	MEI_WRITE,
	RDBUF_FULL_SLOTS,
	READ_HDR,
	MEI_READ,
	IRQ_HANDLER_T,
	PROBE,
	REMOVE,
	MODULE_INIT,
	MODULE_EXIT,
	MEI_IRQ_HANDLER,
};

struct delayed_work_container {
	struct delayed_work delayed_work;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct device_container {
	struct device *dev;
	struct cptr other_ref;
	struct cptr my_ref;
	struct hlist_node hentry;
};

struct irq_handler_t_container {
	int (*irq_handler_t)(int, void *);
	struct cptr other_ref;
	struct cptr my_ref;
};
struct mei_cl_cb_container {
	struct mei_cl_cb mei_cl_cb;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct mei_device_container {
	struct mei_device *mei;
	struct cptr other_ref;
	struct cptr my_ref;
	struct hlist_node hentry;
};
struct mei_fw_status_container {
	struct mei_fw_status mei_fw_status;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct mei_hw_ops_container {
	struct mei_hw_ops mei_hw_ops;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct mei_msg_hdr_container {
	struct mei_msg_hdr mei_msg_hdr;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct module_container {
	struct module module;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_dev_container {
	struct pci_dev *pdev;
	struct cptr other_ref;
	struct cptr my_ref;
	struct hlist_node hentry;
};
struct pci_driver_container {
	struct pci_driver pci_driver;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct work_struct_container {
	struct work_struct work_struct;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct irqhandler_t_container {
	irqreturn_t (*irqhandler)(int, void *);
	void *data;
	cptr_t my_ref;
	cptr_t other_ref;
};


/*struct workqueue_projection_container {
	struct workqueue_projection workqueue_projection;
	struct cptr other_ref;
	struct cptr my_ref;
};*/
struct __wait_queue_container {
	struct __wait_queue __wait_queue;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct __wait_queue_head_container {
	struct __wait_queue_head __wait_queue_head;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct trampoline_hidden_args {
	void *struct_container;
	struct glue_cspace *cspace;
	struct lcd_trampoline_handle *t_handle;
	struct thc_channel *async_chnl;
	struct cptr sync_ep;
};
int glue_cap_insert_delayed_work_type(struct glue_cspace *cspace,
		struct delayed_work_container *delayed_work_container,
		struct cptr *c_out);
int glue_cap_insert_device_type(struct glue_cspace *cspace,
		struct device_container *device_container,
		struct cptr *c_out);
int glue_cap_insert_irq_handler_t_type(struct glue_cspace *cspace,
		struct irq_handler_t_container *irq_handler_t_container,
		struct cptr *c_out);
int glue_cap_insert_mei_cl_cb_type(struct glue_cspace *cspace,
		struct mei_cl_cb_container *mei_cl_cb_container,
		struct cptr *c_out);
int glue_cap_insert_mei_device_type(struct glue_cspace *cspace,
		struct mei_device_container *mei_device_container,
		struct cptr *c_out);
int glue_cap_insert_mei_fw_status_type(struct glue_cspace *cspace,
		struct mei_fw_status_container *mei_fw_status_container,
		struct cptr *c_out);
int glue_cap_insert_mei_hw_ops_type(struct glue_cspace *cspace,
		struct mei_hw_ops_container *mei_hw_ops_container,
		struct cptr *c_out);
int glue_cap_insert_mei_msg_hdr_type(struct glue_cspace *cspace,
		struct mei_msg_hdr_container *mei_msg_hdr_container,
		struct cptr *c_out);
int glue_cap_insert_module_type(struct glue_cspace *cspace,
		struct module_container *module_container,
		struct cptr *c_out);
int glue_cap_insert_pci_dev_type(struct glue_cspace *cspace,
		struct pci_dev_container *pci_dev_container,
		struct cptr *c_out);
int glue_cap_insert_pci_driver_type(struct glue_cspace *cspace,
		struct pci_driver_container *pci_driver_container,
		struct cptr *c_out);
int glue_cap_insert_work_struct_type(struct glue_cspace *cspace,
		struct work_struct_container *work_struct_container,
		struct cptr *c_out);
/*int glue_cap_insert_workqueue_projection_type(struct glue_cspace *cspace,
		struct workqueue_projection_container *workqueue_projection_container,
		struct cptr *c_out);
		*/
int glue_cap_insert___wait_queue_type(struct glue_cspace *cspace,
		struct __wait_queue_container *__wait_queue_container,
		struct cptr *c_out);
int glue_cap_insert___wait_queue_head_type(struct glue_cspace *cspace,
		struct __wait_queue_head_container *__wait_queue_head_container,
		struct cptr *c_out);
int glue_cap_lookup_delayed_work_type(struct glue_cspace *cspace,
		struct cptr c,
		struct delayed_work_container **delayed_work_container);
int glue_cap_lookup_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_container **device_container);
int glue_cap_lookup_irq_handler_t_type(struct glue_cspace *cspace,
		struct cptr c,
		struct irq_handler_t_container **irq_handler_t_container);
int glue_cap_lookup_mei_cl_cb_type(struct glue_cspace *cspace,
		struct cptr c,
		struct mei_cl_cb_container **mei_cl_cb_container);
int glue_cap_lookup_mei_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct mei_device_container **mei_device_container);
int glue_cap_lookup_mei_fw_status_type(struct glue_cspace *cspace,
		struct cptr c,
		struct mei_fw_status_container **mei_fw_status_container);
int glue_cap_lookup_mei_hw_ops_type(struct glue_cspace *cspace,
		struct cptr c,
		struct mei_hw_ops_container **mei_hw_ops_container);
int glue_cap_lookup_mei_msg_hdr_type(struct glue_cspace *cspace,
		struct cptr c,
		struct mei_msg_hdr_container **mei_msg_hdr_container);
int glue_cap_lookup_module_type(struct glue_cspace *cspace,
		struct cptr c,
		struct module_container **module_container);
int glue_cap_lookup_pci_dev_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_dev_container **pci_dev_container);
int glue_cap_lookup_pci_driver_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_driver_container **pci_driver_container);
int glue_cap_lookup_work_struct_type(struct glue_cspace *cspace,
		struct cptr c,
		struct work_struct_container **work_struct_container);
/*int glue_cap_lookup_workqueue_projection_type(struct glue_cspace *cspace,
		struct cptr c,
		struct workqueue_projection_container **workqueue_projection_container);
		*/
int glue_cap_lookup___wait_queue_type(struct glue_cspace *cspace,
		struct cptr c,
		struct __wait_queue_container **__wait_queue_container);
int glue_cap_lookup___wait_queue_head_type(struct glue_cspace *cspace,
		struct cptr c,
		struct __wait_queue_head_container **__wait_queue_head_container);
int glue_cap_insert_irqhandler_type(struct glue_cspace *cspace,
		struct irqhandler_t_container *irqhandler_container,
		struct cptr *c_out);
int glue_cap_lookup_irqhandler_type(struct glue_cspace *cspace,
		struct cptr c,
		struct irqhandler_t_container **irqhandler_container);

#define INIT_FIPC_MSG(msg)             memset(msg, 0x0, sizeof(*msg))

/* CSPACES ------------------------------------------------------------ */
int glue_cap_init(void);

int glue_cap_create(struct glue_cspace **cspace);

void glue_cap_destroy(struct glue_cspace *cspace);

void glue_cap_exit(void);

void glue_cap_remove( struct glue_cspace *cspace, cptr_t c);

#define INIT_IPC_MSG(m)		memset(m, 0x0, sizeof(*m))
/* ASYNC HELPERS -------------------------------------------------- */
static inline
int
async_msg_get_fn_type(struct fipc_message *msg)
{
	return msg->rpc_id;
}

static inline
void
async_msg_set_fn_type(struct fipc_message *msg, int type)
{
	msg->vmfunc_id = VMFUNC_RPC_CALL;
	msg->rpc_id = type;
}
#endif	/* __MEI_GLUE_HELPER_H__ */
