#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../coretemp_hwmon_callee.h"

#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("ctemp_hwmon_klcd got " #x " msg")

int hwmon_klcd_dispatch_loop(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case X86_MATCH_CPU:
			trace(X86_MATCH_CPU);
			return x86_match_cpu_callee(message);

		case __PLATFORM_DRIVER_REGISTER:
			trace(__PLATFORM_DRIVER_REGISTER);
			return __platform_driver_register_callee(message);

		case PLATFORM_DEVICE_UNREGISTER:
			trace(PLATFORM_DEVICE_UNREGISTER);
			return platform_device_unregister_callee(message);


		case CPU_MAPS_UPDATE_BEGIN:
			trace(CPU_MAPS_UPDATE_BEGIN);
			return cpu_maps_update_begin_callee(message);

		case CPU_MAPS_UPDATE_DONE:
			trace(CPU_MAPS_UPDATE_DONE);
			return cpu_maps_update_done_callee(message);

		case __REGISTER_CPU_NOTIFIER:
			trace(__REGISTER_CPU_NOTIFIER);
			return __register_cpu_notifier_callee(message);

		case PLATFORM_DRIVER_UNREGISTER:
			trace(PLATFORM_DRIVER_UNREGISTER);
			return platform_driver_unregister_callee(message);

		case __UNREGISTER_CPU_NOTIFIER:
			trace(__UNREGISTER_CPU_NOTIFIER);
			return __unregister_cpu_notifier_callee(message);

		case PLATFORM_DEVICE_ADD:
			trace(PLATFORM_DEVICE_ADD);
			return platform_device_add_callee(message);

		case PLATFORM_DEVICE_ALLOC:
			trace(PLATFORM_DEVICE_ALLOC);
			return platform_device_alloc_callee(message);

		case PLATFORM_DEVICE_PUT:
			trace(PLATFORM_DEVICE_PUT);
			return platform_device_put_callee(message);

		case PCI_GET_DOMAIN_BUS_AND_SLOT:
			trace(PCI_GET_DOMAIN_BUS_AND_SLOT);
			return pci_get_domain_bus_and_slot_callee(message);

		case DEVM_HWMON_DEVICE_REGISTER_WITH_GROUPS:
			trace(DEVM_HWMON_DEVICE_REGISTER_WITH_GROUPS);
			return devm_hwmon_device_register_with_groups_callee(message);

		case RDMSR_SAFE_ON_CPU:
			trace(RDMSR_SAFE_ON_CPU);
			return rdmsr_safe_on_cpu_callee(message);

		case RDMSR_ON_CPU:
			trace(RDMSR_ON_CPU);
			return rdmsr_on_cpu_callee(message);

		case SYSFS_CREATE_GROUP:
			trace(SYSFS_CREATE_GROUP);
			return sysfs_create_group_callee(message);

		case SYSFS_REMOVE_GROUP:
			trace(SYSFS_REMOVE_GROUP);
			return sysfs_remove_group_callee(message);

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;
}

int hwmon_klcd_syncipc_dispatch(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch(fn_type) {
		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;
	}
}
