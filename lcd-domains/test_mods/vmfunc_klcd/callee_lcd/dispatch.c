#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../rdtsc_helper.h"
#include "../rpc.h"
#include <linux/module.h>

#include <lcd_config/post_hook.h>

unsigned long noinline
null_invocation(struct fipc_message *msg)
{
	fipc_set_reg1(msg, 0xbad);
	return 0;
}

int handle_rpc_calls(struct fipc_message *msg)
{
	switch(msg->rpc_id) {
	case NULL_INVOCATION:
		null_invocation(msg);
		break;
	case CALLEE:
		callee(msg);
		break;
	default:
		break;
	};
	return 0;
}
