#ifndef __SHPCHP_CALLEE_H__
#define __SHPCHP_CALLEE_H__

#include "../shpchp_glue_helper.h"

int add_timer_callee(struct fipc_message *_request);
int del_timer_callee(struct fipc_message *_request);
int init_timer_key_callee(struct fipc_message *_request);
int device_create_file_callee(struct fipc_message *_request);
int device_remove_file_callee(struct fipc_message *_request);
int ioremap_nocache_callee(struct fipc_message *_request);
int iounmap_callee(struct fipc_message *_request);
int pci_bus_add_devices_callee(struct fipc_message *_request);
int acpi_get_hp_hw_control_from_firmware_callee(struct fipc_message *_request);
int pci_assign_unassigned_bridge_resources_callee(struct fipc_message *_request);
int pci_bus_resource_n_callee(struct fipc_message *_request);
int pci_dev_get_callee(struct fipc_message *_request);
int pci_dev_put_callee(struct fipc_message *_request);
int pcie_bus_configure_settings_callee(struct fipc_message *_request);
int pci_enable_device_callee(struct fipc_message *_request);
int pci_find_bus_callee(struct fipc_message *_request);
int pci_find_capability_callee(struct fipc_message *_request);
int pci_get_slot_callee(struct fipc_message *_request);
int pci_hp_add_bridge_callee(struct fipc_message *_request);
int pci_hp_change_slot_info_callee(struct fipc_message *_request);
int pci_hp_deregister_callee(struct fipc_message *_request);
int __pci_hp_register_callee(struct fipc_message *_request);
int pci_lock_rescan_remove_callee(struct fipc_message *_request);
int __pci_register_driver_callee(struct fipc_message *_request);
int pci_scan_slot_callee(struct fipc_message *_request);
int pci_stop_and_remove_bus_device_callee(struct fipc_message *_request);
int pci_unlock_rescan_remove_callee(struct fipc_message *_request);
int pci_unregister_driver_callee(struct fipc_message *_request);
int __release_region_callee(struct fipc_message *_request);
int __request_region_callee(struct fipc_message *_request);
int request_threaded_irq_callee(struct fipc_message *_request);
int free_irq_callee(struct fipc_message *_request);
int schedule_timeout_callee(struct fipc_message *_request);
int dispatch_sync_loop(struct fipc_message *message);
int dispatch_async_loop(struct fipc_message *message);

#endif	/* __SHPCHP_CALLEE_H__ */
