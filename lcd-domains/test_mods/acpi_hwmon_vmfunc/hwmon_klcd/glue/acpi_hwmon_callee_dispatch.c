#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../acpi_hwmon_callee.h"

#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

int hwmon_klcd_dispatch_loop(struct fipc_message *message)
{
	int fn_type;

	fn_type = async_msg_get_fn_type(message);

	switch (fn_type) {
		case ACPI_BUS_GENERATE_NETLINK_EVENT:
			trace(ACPI_BUS_GENERATE_NETLINK_EVENT);
			return acpi_bus_generate_netlink_event_callee(message);

		case ACPI_BUS_GET_DEVICE:
			trace(ACPI_BUS_GET_DEVICE);
			return acpi_bus_get_device_callee(message);

		case ACPI_BUS_REGISTER_DRIVER:
			trace(ACPI_BUS_REGISTER_DRIVER);
			return acpi_bus_register_driver_callee(message);

		case ACPI_BUS_UNREGISTER_DRIVER:
			trace(ACPI_BUS_UNREGISTER_DRIVER);
			return acpi_bus_unregister_driver_callee(message);

		case ACPI_EVALUATE_INTEGER:
			trace(ACPI_EVALUATE_INTEGER);
			return acpi_evaluate_integer_callee(message);

		case ACPI_EXTRACT_PACKAGE:
			trace(ACPI_EXTRACT_PACKAGE);
			return acpi_extract_package_callee(message);

		case ACPI_EVALUATE_OBJECT:
			trace(ACPI_EVALUATE_OBJECT);
			return acpi_evaluate_object_callee(message);

		case DEVICE_CREATE_FILE:
			trace(DEVICE_CREATE_FILE);
			return device_create_file_callee(message);

		case DEVICE_REMOVE_FILE:
			trace(DEVICE_REMOVE_FILE);
			return device_remove_file_callee(message);

		case GET_DEVICE:
			trace(GET_DEVICE);
			return get_device_callee(message);

		case PUT_DEVICE:
			trace(PUT_DEVICE);
			return put_device_callee(message);

		case DMI_CHECK_SYSTEM:
			trace(DMI_CHECK_SYSTEM);
			return dmi_check_system_callee(message);

		case HWMON_DEVICE_REGISTER:
			trace(HWMON_DEVICE_REGISTER);
			return hwmon_device_register_callee(message);

		case HWMON_DEVICE_UNREGISTER:
			trace(HWMON_DEVICE_UNREGISTER);
			return hwmon_device_unregister_callee(message);

		case KOBJECT_CREATE_AND_ADD:
			trace(KOBJECT_CREATE_AND_ADD);
			return kobject_create_and_add_callee(message);

		case KOBJECT_PUT:
			trace(KOBJECT_PUT);
			return kobject_put_callee(message);

		case SYSFS_CREATE_LINK:
			trace(SYSFS_CREATE_LINK);
			return sysfs_create_link_callee(message);

		case SYSFS_REMOVE_LINK:
			trace(SYSFS_REMOVE_LINK);
			return sysfs_remove_link_callee(message);

		case SYSFS_NOTIFY:
			trace(SYSFS_NOTIFY);
			return sysfs_notify_callee(message);

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

	switch (fn_type) {
	default:
		LIBLCD_ERR("%s, unexpected function label %d", __func__, fn_type);
		return -EINVAL;
	}
	return 0;
}
