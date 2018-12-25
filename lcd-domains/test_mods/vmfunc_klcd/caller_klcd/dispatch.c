#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../rdtsc_helper.h"
#include "../rpc.h"
#include <linux/module.h>

#include <lcd_config/post_hook.h>

unsigned long noinline
null_invocation(void)
{
       return 10;
}

int handle_rpc_calls(struct fipc_message *msg)
{
	switch(msg->rpc_id) {
	case NULL_INVOCATION:
		null_invocation();
		break;
	default:
		break;
	};
	return 0;
}
