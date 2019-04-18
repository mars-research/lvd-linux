#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../nullb_caller.h"
#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("nullb got " #x " msg")

int __nullb_lcd_init(void);
void __nullb_lcd_exit(void);

int handle_rpc_calls(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case QUEUE_RQ_FN:
			//trace(QUEUE_RQ_FN);
			return queue_rq_fn_callee(message);

		case MAP_QUEUE_FN:
			trace(MAP_QUEUE_FN);
			return map_queue_fn_callee(message);

		case INIT_HCTX_FN:
			trace(INIT_HCTX_FN);
			return init_hctx_fn_callee(message);

		case SOFTIRQ_DONE_FN:
			trace(SOFTIRQ_DONE_FN);
			return softirq_done_fn_callee(message);

		case OPEN:
			trace(OPEN);
			return open_callee(message);

		case RELEASE:
			trace(RELEASE);
			return release_callee(message);

		case MODULE_INIT:
			trace(MODULE_INIT);
			return __nullb_lcd_init();

		case MODULE_EXIT:
			trace(MODULE_EXIT);
			__nullb_lcd_exit();
			break;
		default:
			LIBLCD_ERR("unexpected function label: %d", fn_type);
			return -EINVAL;
	}
	return 0;
}

