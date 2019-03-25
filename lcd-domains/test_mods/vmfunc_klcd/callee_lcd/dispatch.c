#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../rdtsc_helper.h"
#include "../rpc.h"
#include <linux/module.h>
#include <asm/lcd_domains/libvmfunc.h>

#include <lcd_config/post_hook.h>

extern int callee_main(void);
unsigned long noinline
null_invocation(struct fipc_message *msg)
{
	fipc_set_reg1(msg, 0xbad);
	printk("%s, called\n", __func__);
	return 0;
}

unsigned long noinline
foo(struct fipc_message *msg)
{
	struct fipc_message kmsg = {0};
	int ret = 0;
	kmsg.vmfunc_id = VMFUNC_RPC_CALL;
	kmsg.rpc_id = BAR;

	fipc_set_reg1(&kmsg, 0xaa);
	fipc_set_reg2(&kmsg, 0xbb);
	fipc_set_reg3(&kmsg, 0xcc);
	fipc_set_reg4(&kmsg, 0xdd);
	fipc_set_reg5(&kmsg, 0xee);
	fipc_set_reg6(&kmsg, 0xff);

	vmfunc_wrapper(&kmsg);

	ret = fipc_get_reg1(&kmsg);
	fipc_set_reg2(&kmsg, 0xbb + ret);
	fipc_set_reg3(&kmsg, 0xcc + ret);
	fipc_set_reg4(&kmsg, 0xdd + ret);
	fipc_set_reg5(&kmsg, 0xee + ret);
	fipc_set_reg6(&kmsg, 0xff + ret);

	return 0;
}

int handle_rpc_calls(struct fipc_message *msg)
{
	switch(msg->rpc_id) {
	case MODULE_INIT:
		callee_main();
		break;
	case NULL_INVOCATION:
		null_invocation(msg);
		break;
	case FOO:
		foo(msg);
		break;
	case CALLEE:
		callee(msg);
		break;
	default:
		break;
	};
	return 0;
}
