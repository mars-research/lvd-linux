#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../skx_edac_caller.h"
#include <lcd_config/post_hook.h>
int dispatch_async_loop(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case GET:
			trace(GET);
			return get_callee(message);

		case SET:
			trace(SET);
			return set_callee(message);

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;

}

