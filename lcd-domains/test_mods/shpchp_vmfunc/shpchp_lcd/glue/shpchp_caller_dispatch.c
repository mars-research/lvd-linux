#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../shpchp_caller.h"
#include <lcd_config/post_hook.h>
int dispatch_async_loop(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case IRQ_HANDLER:
			trace(IRQ_HANDLER);
			return irq_handler_callee(message);

		case IRQ_HANDLER_TNREAD:
			trace(IRQ_HANDLER_TNREAD);
			return irq_handler_tnread_callee(message);

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;

}

