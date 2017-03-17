#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../ixgbe_caller.h"
#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

int dispatch_async_loop(struct thc_channel *_channel,
		struct fipc_message *message,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case PROBE:
			trace(PROBE);
			return probe_callee(message,
		_channel,
		cspace,
		sync_ep);

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;

}

