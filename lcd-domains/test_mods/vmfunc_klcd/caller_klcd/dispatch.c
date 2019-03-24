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

unsigned long noinline
bar(struct fipc_message *msg)
{
	printk("%s, bar called with reg0: %lx, reg1: %lx, reg2, %lx, reg3: %lx, reg4: %lx, reg5: %lx\n",
			__func__,
			fipc_get_reg0(msg),
			fipc_get_reg1(msg),
			fipc_get_reg2(msg),
			fipc_get_reg3(msg),
			fipc_get_reg4(msg),
			fipc_get_reg5(msg));
	fipc_set_reg1(msg, 0x100);
	return 0x12;
}

int handle_rpc_calls(struct fipc_message *msg)
{
	switch(msg->rpc_id) {
	case NULL_INVOCATION:
		null_invocation();
		break;
	case BAR:
		bar(msg);
		break;
	default:
		break;
	};
	return 0;
}
