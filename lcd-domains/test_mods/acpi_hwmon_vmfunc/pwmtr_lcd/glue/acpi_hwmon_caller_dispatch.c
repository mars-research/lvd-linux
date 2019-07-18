#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../acpi_hwmon_caller.h"

#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

int dispatch_async_loop(struct fipc_message *message)
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

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;
}
