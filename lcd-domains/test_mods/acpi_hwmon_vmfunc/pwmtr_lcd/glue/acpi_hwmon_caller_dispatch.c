#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../acpi_hwmon_caller.h"

#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("pwmtr got " #x " msg")

int handle_rpc_calls(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case ACPI_OP_ADD:
			trace(ACPI_OP_ADD);
			return acpi_op_add_callee(message);

		case ACPI_OP_REMOVE:
			trace(ACPI_OP_REMOVE);
			return acpi_op_remove_callee(message);

		case ACPI_OP_NOTIFY:
			trace(ACPI_OP_NOTIFY);
			return acpi_op_notify_callee(message);

		case ATTR_SHOW:
			trace(ATTR_SHOW);
			return attr_show_callee(message);

		case ATTR_STORE:
			trace(ATTR_STORE);
			return attr_store_callee(message);

		case DMI_CALLBACK:
			trace(DMI_CALLBACK);
			return dmi_callback_callee(message);

		case MODULE_INIT:
			trace(MODULE_INIT);
			return __pwmtr_lcd_init();

		case MODULE_EXIT:
			trace(MODULE_EXIT);
			__pwmtr_lcd_exit();
			break;

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;
}
