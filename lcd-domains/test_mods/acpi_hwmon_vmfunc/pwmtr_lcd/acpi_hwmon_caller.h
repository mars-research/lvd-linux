#ifndef __ACPI_HWMON_CALLER_H__
#define __ACPI_HWMON_CALLER_H__

#include "../acpi_hwmon_common.h"

int dispatch_sync_loop(struct fipc_message *message);
int handle_rpc_calls(struct fipc_message *message);
int glue_acpi_hwmon_init(void);
void glue_acpi_hwmon_exit(void);

int __pwmtr_lcd_init(void);
void __pwmtr_lcd_exit(void);

int acpi_op_add_callee(struct fipc_message *_request);
int acpi_op_remove_callee(struct fipc_message *_request);
int acpi_op_notify_callee(struct fipc_message *_request);
int attr_show_callee(struct fipc_message *_request);
int attr_store_callee(struct fipc_message *_request);
int dmi_callback_callee(struct fipc_message *_request);

acpi_status
_acpi_evaluate_object(struct acpi_device *acpi_device,
		     acpi_string pathname,
		     struct acpi_object_list *external_params,
		     struct acpi_buffer *return_buffer);

struct kobject *_kobject_create_and_add(const char *name, struct device *dev);

#endif	/* __ACPI_HWMON_CALLER_H__ */
