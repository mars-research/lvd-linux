
#include "server-idl.h"

/* Caller stubs */
int test_func1(unsigned long a, long b, char c) {

	msg->regs[0] = ;
	msg->regs[1] = 1;
	msg->regs[2] = 2;
	msg->regs[3] = 3;
	msg->valid_regs = 4;

	msg->cap_regs[0] = server_reply;
	msg->valid_cap_regs = 1;

	ret = ipc_call(server_interface, msg);
	if (ret) {
		printk(KERN_ERR "client failed to ivoke server:%d\n", ret);
		return ret;
	};


}

int test_func2(unsigned long a) {

}

/* Callee stubs */
int test_func1_srv(unsigned long a, long b, char c) {

}

int test_func2_srv(unsigned long a) {

}


