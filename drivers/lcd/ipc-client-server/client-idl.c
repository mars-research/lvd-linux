
#include "server-idl.h"


/* Caller stubs */
/* Generate a global capability variable */
client_t client; 

struct cap_space module_cspace; 

int __register_server_caller(capability_t client, struct server_interface *server) {
	int ret; 

	msg->regs[0] = client_interface_register_server;
	msg->valid_regs = 1;

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

int register_server_caller(struct server_interface *server) {
	return __register_server_caller(client, server);
}

/* Callee stubs */
int register_server_callee(struct message_info *msg, capability_t reply_cap) {
	int ret; 
	struct server_interface *server; 

	server = (struct server_interface *) malloc(sizeof(struct server_interface));
	if (!server) {
		printk(KERN_ERR "memory allocation failed\n");
		msg->valid_regs = 0; 
		msg->valid_cap_regs = 0; 
		msg->err = -ENOMEM;
		ipc_reply(reply_cap, msg);
		return 0;
	};


	


	ret = register_server(server); 
	
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
	capability_t server = current->utcb->boot_info.boot_caps[LCD_BOOT_FREE_CAP0];
	capability_t server_interface;
	
	struct message_info *msg = &current->utcb->msg_info;

	ret = lcd_alloc_cap(&current->cap_cache, &server_interface); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		return -ENOSPC;
	};

	msg->cap_regs[0] = server_interface;
	msg->valid_cap_regs = 1;

	ret = ipc_recv(server, msg);
	if (ret) {
		printk(KERN_ERR "client failed to receive from the server:%d\n", ret);
		return ret;
	};


	call = msg->regs[0];

	switch (call) {
		case client_interface_register_server: 
			ret = register(lcd_api, reply_cap); 
			break; 
		default: 
			printk(KERN_ERR "lcd_api: invalid call number:%d\n", call);
			msg->valid_regs = 0; 
			msg->valid_cap_regs = 0; 
			ipc_reply(reply_cap, msg); 
			lcd_cap_drop(&current->cspace, reply_cap);
		};


	return 0;
};

struct sync_ipc *client_server_rvp;
int accept_client_introduction(void) {
	return lcd_accept_introduction(&client_server_rvp, 
			&client);

};
EXPORT_SYMBOL(accept_client_introduction);

int execution_loop_thread(void *p) {
	int ret;

	ret = lcd_init_current();
	if (ret) {
		printk(KERN_ERR "Failed to init current thread with LCD:%d\n", ret);
		return -EINVAL;
	}

	ret = lcd_init_cspace(&module_cspace); 
	if (ret) {
		printk(KERN_ERR "Failed to init global cspace for this module:%d\n", ret);
		return ret;
	};

	/* Introduction code */
	ret = lcd_prepare_introduction(lcd_api_cap(), 
			lcd_api_reply_cap(), 
			&client_server_rvp, 
			&current->utcb->boot_info.boot_caps[LCD_BOOT_FREE_CAP0]);
	if (ret) {
		printk(KERN_ERR "client failed to initialize client-server environtment\n");
		return -EINVAL;
	}

	/* Record the clients session */
	lcd_cap_make_cspace(&module_cspace, &module_cap_cache, * 

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


