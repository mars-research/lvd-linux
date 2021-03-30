#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../shpchp_callee.h"
#include <lcd_config/post_hook.h>
int dispatch_async_loop(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case ADD_TIMER:
			trace(ADD_TIMER);
			return add_timer_callee(message);

		case DEL_TIMER:
			trace(DEL_TIMER);
			return del_timer_callee(message);

		case INIT_TIMER_KEY:
			trace(INIT_TIMER_KEY);
			return init_timer_key_callee(message);

		case DEVICE_CREATE_FILE:
			trace(DEVICE_CREATE_FILE);
			return device_create_file_callee(message);

		case DEVICE_REMOVE_FILE:
			trace(DEVICE_REMOVE_FILE);
			return device_remove_file_callee(message);

		case IOREMAP_NOCACHE:
			trace(IOREMAP_NOCACHE);
			return ioremap_nocache_callee(message);

		case IOUNMAP:
			trace(IOUNMAP);
			return iounmap_callee(message);

		case PCI_BUS_ADD_DEVICES:
			trace(PCI_BUS_ADD_DEVICES);
			return pci_bus_add_devices_callee(message);

		case ACPI_GET_HP_HW_CONTROL_FROM_FIRMWARE:
			trace(ACPI_GET_HP_HW_CONTROL_FROM_FIRMWARE);
			return acpi_get_hp_hw_control_from_firmware_callee(message);

		case PCI_ASSIGN_UNASSIGNED_BRIDGE_RESOURCES:
			trace(PCI_ASSIGN_UNASSIGNED_BRIDGE_RESOURCES);
			return pci_assign_unassigned_bridge_resources_callee(message);

		case PCI_BUS_RESOURCE_N:
			trace(PCI_BUS_RESOURCE_N);
			return pci_bus_resource_n_callee(message);

		case PCI_DEV_GET:
			trace(PCI_DEV_GET);
			return pci_dev_get_callee(message);

		case PCI_DEV_PUT:
			trace(PCI_DEV_PUT);
			return pci_dev_put_callee(message);

		case PCIE_BUS_CONFIGURE_SETTINGS:
			trace(PCIE_BUS_CONFIGURE_SETTINGS);
			return pcie_bus_configure_settings_callee(message);

		case PCI_ENABLE_DEVICE:
			trace(PCI_ENABLE_DEVICE);
			return pci_enable_device_callee(message);

		case PCI_FIND_BUS:
			trace(PCI_FIND_BUS);
			return pci_find_bus_callee(message);

		case PCI_FIND_CAPABILITY:
			trace(PCI_FIND_CAPABILITY);
			return pci_find_capability_callee(message);

		case PCI_GET_SLOT:
			trace(PCI_GET_SLOT);
			return pci_get_slot_callee(message);

		case PCI_HP_ADD_BRIDGE:
			trace(PCI_HP_ADD_BRIDGE);
			return pci_hp_add_bridge_callee(message);

		case PCI_HP_CHANGE_SLOT_INFO:
			trace(PCI_HP_CHANGE_SLOT_INFO);
			return pci_hp_change_slot_info_callee(message);

		case PCI_HP_DEREGISTER:
			trace(PCI_HP_DEREGISTER);
			return pci_hp_deregister_callee(message);

		case __PCI_HP_REGISTER:
			trace(__PCI_HP_REGISTER);
			return __pci_hp_register_callee(message);

		case PCI_LOCK_RESCAN_REMOVE:
			trace(PCI_LOCK_RESCAN_REMOVE);
			return pci_lock_rescan_remove_callee(message);

		case __PCI_REGISTER_DRIVER:
			trace(__PCI_REGISTER_DRIVER);
			return __pci_register_driver_callee(message);

		case PCI_SCAN_SLOT:
			trace(PCI_SCAN_SLOT);
			return pci_scan_slot_callee(message);

		case PCI_STOP_AND_REMOVE_BUS_DEVICE:
			trace(PCI_STOP_AND_REMOVE_BUS_DEVICE);
			return pci_stop_and_remove_bus_device_callee(message);

		case PCI_UNLOCK_RESCAN_REMOVE:
			trace(PCI_UNLOCK_RESCAN_REMOVE);
			return pci_unlock_rescan_remove_callee(message);

		case PCI_UNREGISTER_DRIVER:
			trace(PCI_UNREGISTER_DRIVER);
			return pci_unregister_driver_callee(message);

		case __RELEASE_REGION:
			trace(__RELEASE_REGION);
			return __release_region_callee(message);

		case __REQUEST_REGION:
			trace(__REQUEST_REGION);
			return __request_region_callee(message);

		case REQUEST_THREADED_IRQ:
			trace(REQUEST_THREADED_IRQ);
			return request_threaded_irq_callee(message);

		case FREE_IRQ:
			trace(FREE_IRQ);
			return free_irq_callee(message);

		case SCHEDULE_TIMEOUT:
			trace(SCHEDULE_TIMEOUT);
			return schedule_timeout_callee(message);

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;

}

