#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../rdtsc_helper.h"
#include "../rpc.h"
#include <linux/module.h>
#include <asm/lcd_domains/libvmfunc.h>

#include <lcd_config/post_hook.h>

unsigned long noinline
null_invocation(struct fipc_message *msg)
{
	fipc_set_reg1(msg, 0xbad);
	return 0;
}

unsigned long noinline
foo(struct fipc_message *msg)
{
	struct fipc_message kmsg;
	kmsg.vmfunc_id = VMFUNC_RPC_CALL;
	kmsg.rpc_id = BAR;

	fipc_set_reg1(&kmsg, 0xaa);
	fipc_set_reg2(&kmsg, 0xbb);
	fipc_set_reg3(&kmsg, 0xcc);
	fipc_set_reg4(&kmsg, 0xdd);
	fipc_set_reg5(&kmsg, 0xee);
	fipc_set_reg6(&kmsg, 0xff);

	vmfunc_wrapper(&kmsg);

	return 0;
}

int handle_rpc_calls(struct fipc_message *msg)
{
	switch(msg->rpc_id) {
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
