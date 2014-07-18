
#include "server-idl.h"

struct server_interface {
	int (*test_func1) (unsigned long a, long b, char c);
	int (*test_func2) (unsigned long a);
};


/* Caller stubs */
int register_server(struct server_interface *server) {


}

/* Callee stubs */
int register_server_callee(struct server_interface *server) {


}

/* Main execution loop */
int execution_loop(void) {

	int ret; 
	capability_t api_cap = current->utcb->boot_info.boot_caps[LCD_BOOT_API_CAP];
	capability_t reply_cap = current->utcb->boot_info.boot_caps[LCD_BOOT_REPLY_CAP];
	capability_t server = current->utcb->boot_info.boot_caps[LCD_BOOT_FREE_CAP0];

	capability_t server_interface;
	capability_t server_reply; 
	
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
		case LCD_CREATE_SYNC_ENDPOINT: 
			ret = lcd_api_create_sync_endpoint(lcd_api, reply_cap); 
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
			&current->utcb->boot_info.boot_caps[LCD_BOOT_FREE_CAP0]);

};
EXPORT_SYMBOL(accept_client_introduction);

int execution_loop_thread(void *p) {
	int ret;

	ret = lcd_init_current();
	if (ret) {
		printk(KERN_ERR "Failed to init current thread with LCD:%d\n", ret);
		return -EINVAL;
	}

	ret = lcd_prepare_introduction(lcd_api_cap(), 
			lcd_api_reply_cap(), 
			&client_server_rvp, 
			&current->utcb->boot_info.boot_caps[LCD_BOOT_FREE_CAP0]);
	if (ret) {
		printk(KERN_ERR "client failed to initialize client-server environtment\n");
		return -EINVAL;
	}

	/* Introduction code */
	ret = lcd_prepare_introduction(&client_server_rvp);
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


