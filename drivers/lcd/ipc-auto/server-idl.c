
#include "server-idl.h"

int terminate_loop; 

void terminate_execution_loop() {
	terminate_loop = 1;
};

/* Caller stubs */
int test_func1_caller(capability_t server, unsigned long a, long b, char c) {
	int ret; 

	msg->regs[0] = server_interface_test_func1;
	msg->regs[1] = (uint64_t) a;
	msg->regs[2] = (uint64_t) b;
	msg->regs[3] = (uint64_t) c;
	msg->valid_regs = 4;

	msg->cap_regs[0] = server;
	msg->valid_cap_regs = 1;

	ret = ipc_call(server, msg);
	if (ret) {
		printk(KERN_ERR "client failed to ivoke server:%d\n", ret);
		/* XXX: BU: make up some result, which? */
		return ret;
	};

	return (int) msg->regs[0];
}

int test_func2_caller(unsigned long a) {
	int ret; 

	msg->regs[0] = server_interface_test_func2;
	msg->regs[1] = (uint64_t) a;
	msg->valid_regs = 2;

	msg->cap_regs[0] = server;
	msg->valid_cap_regs = 1;

	ret = ipc_call(server, msg);
	if (ret) {
		printk(KERN_ERR "client failed to ivoke server:%d\n", ret);
		/* XXX: BU: make up some result, which? */
		return ret;
	};

	return (int) msg->regs[0];

}

/* Callee stubs */
int test_func1_callee(struct message_info *msg, capability_t reply_cap) {
	int ret; 
	unsigned long a = (unsigned long) msg->regs[1]; 
	long b = (long) msg->regs[2];
	char c = (char) msg->regs[3];
	
	ret = test_func1(a, b, c); 
	
	msg->regs[0] = ret;
	msg->valid_regs = 1;
	msg->valid_cap_regs = 0;
	
	ret = ipc_reply(reply_cap, msg);
	if(ret) {
		printk(KERN_ERR "IPC reply failed: %d\n", ret);
		return ret;
	};

	return ret;
}

int test_func2_callee(struct message_info *msg, capability_t reply_cap) {
	int ret; 
	unsigned long a = (unsigned long) msg->regs[1]; 
	
	ret = test_func2(a); 
	
	msg->regs[0] = ret;
	msg->valid_regs = 1;
	msg->valid_cap_regs = 0; 
	
	ret = ipc_reply(reply_cap, msg);
	if(ret) {
		printk(KERN_ERR "IPC reply failed: %d\n", ret);
		return ret;
	};

	return ret;

}

/* Main execution loop */
int execution_loop(void) {

	int ret;
	enum client_interface call; 
	//capability_t client = current->utcb->boot_info.boot_caps[LCD_BOOT_FREE_CAP0];
  	capability_t server_interface; 
	struct message_info *msg = &current->utcb->msg_info;

	while(!terminate_loop) {
		msg->valid_regs = 3;

		ret = ipc_recv(server_interface, msg);
		if (ret) {
			printk(KERN_ERR "server failed ipc_recv:%d\n", ret);
			return ret;
		};

		call = msg->regs[0];

		switch (call) {
		case server_interface_test_func1: 
			ret = test_func1_callee(msg, server_interface); 
			break; 
		case server_interface_test_func2: 
			ret = test_func2_callee(msg, server_interface); 
			break; 

		default: 
			printk(KERN_ERR "lcd_api: invalid call number:%d\n", call);
			msg->valid_regs = 0; 
			msg->valid_cap_regs = 0; 
			ipc_reply(server_interface, msg); 
		};
	};

	return 0;
};


int execution_loop_thread(void *p) {
	int ret;

	ret = lcd_init_current();
	if (ret) {
		printk(KERN_ERR "Failed to init current thread with LCD:%d\n", ret);
		return -EINVAL;
	}

	/* Introduction code */
	ret = lcd_accept_client_introduction();
	if (ret) {
		printk(KERN_ERR "client failed to initialize client-server environtment\n");
		return -EINVAL;
	}

	return execution_loop();
};

int execution_loop_thread_init(void) {

	struct task_struct *t;

	t = kthread_create(execution_loop_thread, NULL, "cs-client");
	if (!t) {
		printk(KERN_ERR "Failed to create module thread\n");
		return -EINVAL;
	};

	wake_up_process(t);
	return 0;
}


