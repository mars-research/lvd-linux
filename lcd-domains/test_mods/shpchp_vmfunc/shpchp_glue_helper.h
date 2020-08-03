#ifndef __SHPCHP_GLUE_HELPER_H__
#define __SHPCHP_GLUE_HELPER_H__

#include <linux/kthread.h>
#include <linux/module.h>
#include <libcap.h>
#include <libfipc.h>
#include <thc_ipc.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/pci.h>
#include <linux/pci_hotplug.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <linux/interrupt.h>

enum dispatch_t {
	ADD_TIMER,
	DEL_TIMER,
	INIT_TIMER_KEY,
	DEVICE_CREATE_FILE,
	DEVICE_REMOVE_FILE,
	IOREMAP_NOCACHE,
	IOUNMAP,
	PCI_BUS_ADD_DEVICES,
	ACPI_GET_HP_HW_CONTROL_FROM_FIRMWARE,
	PCI_ASSIGN_UNASSIGNED_BRIDGE_RESOURCES,
	PCI_BUS_RESOURCE_N,
	PCI_DEV_GET,
	PCI_DEV_PUT,
	PCIE_BUS_CONFIGURE_SETTINGS,
	PCI_ENABLE_DEVICE,
	PCI_FIND_BUS,
	PCI_FIND_CAPABILITY,
	PCI_GET_SLOT,
	PCI_HP_ADD_BRIDGE,
	PCI_HP_CHANGE_SLOT_INFO,
	PCI_HP_DEREGISTER,
	__PCI_HP_REGISTER,
	PCI_LOCK_RESCAN_REMOVE,
	__PCI_REGISTER_DRIVER,
	PCI_SCAN_SLOT,
	PCI_STOP_AND_REMOVE_BUS_DEVICE,
	PCI_UNLOCK_RESCAN_REMOVE,
	PCI_UNREGISTER_DRIVER,
	__RELEASE_REGION,
	__REQUEST_REGION,
	REQUEST_THREADED_IRQ,
	FREE_IRQ,
	SCHEDULE_TIMEOUT,
	IRQ_HANDLER,
	IRQ_HANDLER_TNREAD,
	MODULE_INIT,
	MODULE_EXIT,
};

struct device_attribute_container {
	struct device_attribute device_attribute;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct device_container {
	struct device device;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct hotplug_slot_container {
	struct hotplug_slot hotplug_slot;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct hotplug_slot_info_container {
	struct hotplug_slot_info hotplug_slot_info;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct irq_handler_container {
	int ( *irq_handler )(int, void *) ;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct lock_class_key_container {
	struct lock_class_key lock_class_key;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct module_container {
	struct module module;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_bus_container {
	struct pci_bus pci_bus;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_dev_container {
	struct pci_dev pci_dev;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_driver_container {
	struct pci_driver pci_driver;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct resource_container {
	struct resource resource;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct timer_list_container {
	struct timer_list timer_list;
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
int glue_cap_insert_device_attribute_type(struct glue_cspace *cspace,
		struct device_attribute_container *device_attribute_container,
		struct cptr *c_out);
int glue_cap_insert_device_type(struct glue_cspace *cspace,
		struct device_container *device_container,
		struct cptr *c_out);
int glue_cap_insert_hotplug_slot_type(struct glue_cspace *cspace,
		struct hotplug_slot_container *hotplug_slot_container,
		struct cptr *c_out);
int glue_cap_insert_hotplug_slot_info_type(struct glue_cspace *cspace,
		struct hotplug_slot_info_container *hotplug_slot_info_container,
		struct cptr *c_out);
int glue_cap_insert_irq_handler_type(struct glue_cspace *cspace,
		struct irq_handler_container *irq_handler_container,
		struct cptr *c_out);
int glue_cap_insert_lock_class_key_type(struct glue_cspace *cspace,
		struct lock_class_key_container *lock_class_key_container,
		struct cptr *c_out);
int glue_cap_insert_module_type(struct glue_cspace *cspace,
		struct module_container *module_container,
		struct cptr *c_out);
int glue_cap_insert_pci_bus_type(struct glue_cspace *cspace,
		struct pci_bus_container *pci_bus_container,
		struct cptr *c_out);
int glue_cap_insert_pci_dev_type(struct glue_cspace *cspace,
		struct pci_dev_container *pci_dev_container,
		struct cptr *c_out);
int glue_cap_insert_pci_driver_type(struct glue_cspace *cspace,
		struct pci_driver_container *pci_driver_container,
		struct cptr *c_out);
int glue_cap_insert_resource_type(struct glue_cspace *cspace,
		struct resource_container *resource_container,
		struct cptr *c_out);
int glue_cap_insert_timer_list_type(struct glue_cspace *cspace,
		struct timer_list_container *timer_list_container,
		struct cptr *c_out);
int glue_cap_lookup_device_attribute_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_attribute_container **device_attribute_container);
int glue_cap_lookup_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_container **device_container);
int glue_cap_lookup_hotplug_slot_type(struct glue_cspace *cspace,
		struct cptr c,
		struct hotplug_slot_container **hotplug_slot_container);
int glue_cap_lookup_hotplug_slot_info_type(struct glue_cspace *cspace,
		struct cptr c,
		struct hotplug_slot_info_container **hotplug_slot_info_container);
int glue_cap_lookup_irq_handler_type(struct glue_cspace *cspace,
		struct cptr c,
		struct irq_handler_container **irq_handler_container);
int glue_cap_lookup_lock_class_key_type(struct glue_cspace *cspace,
		struct cptr c,
		struct lock_class_key_container **lock_class_key_container);
int glue_cap_lookup_module_type(struct glue_cspace *cspace,
		struct cptr c,
		struct module_container **module_container);
int glue_cap_lookup_pci_bus_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_bus_container **pci_bus_container);
int glue_cap_lookup_pci_dev_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_dev_container **pci_dev_container);
int glue_cap_lookup_pci_driver_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_driver_container **pci_driver_container);
int glue_cap_lookup_resource_type(struct glue_cspace *cspace,
		struct cptr c,
		struct resource_container **resource_container);
int glue_cap_lookup_timer_list_type(struct glue_cspace *cspace,
		struct cptr c,
		struct timer_list_container **timer_list_container);

#define INIT_FIPC_MSG(m)		memset(m, 0x0, sizeof(*m))
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

#endif	/* __SHPCHP_GLUE_HELPER_H__ */
