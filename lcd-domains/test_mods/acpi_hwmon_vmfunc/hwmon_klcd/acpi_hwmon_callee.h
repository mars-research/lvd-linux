#ifndef __ACPI_HWMON_CALLEE_H__
#define __ACPI_HWMON_CALLEE_H__

#include "../acpi_hwmon_common.h"

int acpi_bus_generate_netlink_event_callee(struct fipc_message *_request);
int acpi_bus_get_device_callee(struct fipc_message *_request);
int acpi_bus_register_driver_callee(struct fipc_message *_request);
int acpi_bus_unregister_driver_callee(struct fipc_message *_request);
int acpi_evaluate_integer_callee(struct fipc_message *_request);
int acpi_extract_package_callee(struct fipc_message *_request);
int acpi_evaluate_object_callee(struct fipc_message *_request);
int acpi_exception_callee(struct fipc_message *_request);
int device_create_file_callee(struct fipc_message *_request);
int device_remove_file_callee(struct fipc_message *_request);
int get_device_callee(struct fipc_message *_request);
int put_device_callee(struct fipc_message *_request);
int dmi_check_system_callee(struct fipc_message *_request);
int hwmon_device_register_callee(struct fipc_message *_request);
int hwmon_device_unregister_callee(struct fipc_message *_request);
int kobject_create_and_add_callee(struct fipc_message *_request);
int kobject_put_callee(struct fipc_message *_request);
int sysfs_create_link_callee(struct fipc_message *_request);
int sysfs_remove_link_callee(struct fipc_message *_request);
int sysfs_notify_callee(struct fipc_message *_request);
int dispatch_sync_loop(struct fipc_message *message);
int dispatch_async_loop(struct fipc_message *message);
int glue_acpi_hwmon_init(void);
void glue_acpi_hwmon_exit(void);

#endif	/* __ACPI_HWMON_CALLEE_H__ */
