/**
 * client.c - client for the client-server IPC test 
 *
 *
 * Authors: Anton Burtsev   <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 */

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/compat.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/kthread.h>
#include <linux/slab.h>

#include <lcd/lcd.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD client-server test client");

struct sync_ipc *rvp; 

int lcd_client_server_init_client(void) {
	int ret; 
	capability_t rvp_cap = 0;
	struct cnode *cnode;
	
	ret = lcd_enter();
	if (ret) {
		printk(KERN_ERR "Failed to enter LCD environment\n");
		return ret;
	};

	rvp = alloc_sync_ipc();
	if(!rvp) {
		printk(KERN_ERR "Failed to allocate memory\n");
		return -ENOMEM;
	};

        cnode = lcd_cnode_lookup(&current->cspace, rvp_cap);
	if(cnode == 0) {
		printk(KERN_ERR "Failed to create capability\n");
		kfree(rvp);
		return -ENOMEM;
	};

	cnode->type = LCD_TYPE_SYNC_EP;
	cnode->object = rvp; 

	lcd_cnode_release(cnode);

	current->utcb->boot_info.boot_rvp = rvp_cap;
	return 0;
};

int lcd_client_server_init_server(void) 
{
	int ret; 
	capability_t rvp_cap = 0;
	struct cnode *cnode;
	
	ret = lcd_enter();
	if (ret) {
		printk(KERN_ERR "Failed to enter LCD environment\n");
		return ret;
	};

	BUG_ON(rvp == NULL); 

        cnode = lcd_cnode_lookup(&current->cspace, rvp_cap);
	if(cnode == 0) {
		printk(KERN_ERR "Failed to create capability\n");
		kfree(rvp);
		return -ENOMEM;
	};

	cnode->type = LCD_TYPE_SYNC_EP;
	cnode->object = rvp;

	lcd_cnode_release(cnode);

	current->utcb->boot_info.boot_rvp = rvp_cap;
	return 0;
};
EXPORT_SYMBOL(lcd_client_server_init_server); 

int module_execution_loop(void) {

	int ret; 
	capability_t boot_cap = current->utcb->boot_info.boot_rvp;
	capability_t server_cap; 
	struct message_info *msg = &current->utcb->msg_info;

	server_cap = boot_cap + 1;

	msg->cap_regs[0] = server_cap;
	msg->valid_cap_regs = 1;

	ret = ipc_recv(boot_cap, msg);
	if (ret) {
		printk(KERN_ERR "client failed to receive from the server:%d\n", ret);
		return ret;
	};

	msg->regs[0] = 0 /* FUNC_TYPE_1*/;
	msg->regs[1] = 1;
	msg->regs[2] = 2;
	msg->regs[3] = 3;

	msg->valid_regs = 4;

	ret = ipc_send(server_cap, msg);
	if (ret) {
		printk(KERN_ERR "client failed to ivoke server:%d\n", ret);
		return ret;
	};

	return 0;
};

int module_thread(void *p) {
	int ret;

	ret = lcd_client_server_init_client();
	if (ret) {
		printk(KERN_ERR "client failed to initialize client-server environtment\n");
		return -EINVAL;
	}

	return module_execution_loop();
};

int module_thread_init(void) {

	struct task_struct *t;

	t = kthread_create(module_thread, NULL, "cs-client");
	if (!t) {
		printk(KERN_ERR "Failed to create module thread\n");
		return -EINVAL;
	};

	wake_up_process(t);
	return 0;
}

static int __init client_init(void)
{
	return module_thread_init();
}

static void __exit client_exit(void)
{
}

module_init(client_init);
module_exit(client_exit);
