#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../msr_caller.h"

#include <lcd_config/post_hook.h>

#define trace(x)	LIBLCD_MSG("got msg %s", #x)
int handle_rpc_calls(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		/*
		case NOTIFIER_FN_T:
			trace(NOTIFIER_FN_T);
			return notifier_fn_t_callee(message);

		case MODULE_INIT:
			trace(MODULE_INIT);
			return __coretemp_lcd_init();

		case MODULE_EXIT:
			trace(MODULE_EXIT);
			__coretemp_lcd_exit();
			break;

		case PROBE:
			trace(PROBE);
			return probe_callee(message);

		case REMOVE:
			trace(REMOVE);
			return remove_callee(message);

		case ATTR_SHOW:
			trace(ATTR_SHOW);
			return attr_show_callee(message);
		*/
		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;

}

