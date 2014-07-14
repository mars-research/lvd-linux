/**
 * sender.c - sender for the IPC ping-pong test
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
#include <lcd/cap.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD ping-pong IPC test (sender)");

struct sync_ipc *rvp; 

int lcd_ping_pong_init_sender(void) {
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

int lcd_ping_pong_init_receiver(void) 
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
EXPORT_SYMBOL(lcd_ping_pong_init_receiver); 

int module_execution_loop(void) {

	int ret;
        capability_t rvp_cap = current->utcb->boot_info.boot_rvp;
	struct message_info *msg = &current->utcb->msg_info;

	msg->regs[0] = 1;
	msg->regs[1] = 2;
	msg->regs[2] = 3;
	msg->regs[3] = 4;
	msg->regs[4] = 5;
	msg->regs[5] = 6;

	msg->valid_regs = 6;

	ret = ipc_send(rvp_cap, msg);
	if (ret) {
		printk(KERN_ERR "sender failed:%d\n", ret);
		return ret;
	};

	msg->valid_regs = 8;

	ret = ipc_recv(rvp_cap, msg);
	if (ret) {
		printk(KERN_ERR "sender failed to recv:%d\n", ret);
		return ret;
	};

	printk(KERN_INFO "Sender got: %lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld\n", 
			 msg->regs[0], msg->regs[1], msg->regs[2], msg->regs[3], 
			 msg->regs[4], msg->regs[5], msg->regs[6], msg->regs[7]);



	return 0;
};

int module_thread(void *p) {
	int ret;

	ret = lcd_ping_pong_init_sender();
	if (ret) {
		printk(KERN_ERR "sender failed to initialize ping-pong environtment\n");
		return -EINVAL;
	}

	return module_execution_loop();
};

int module_thread_init(void) {

	struct task_struct *t;

	t = kthread_create(module_thread, NULL, "pp-sender");
	if (!t) {
		printk(KERN_ERR "Failed to create module thread\n");
		return -EINVAL;
	};

	wake_up_process(t);
	return 0;
}

static int __init sender_init(void)
{
	printk(KERN_ERR "IPC ping-pong, sender module loaded\n");
	return module_thread_init();
}

static void __exit sender_exit(void)
{
}

module_init(sender_init);
module_exit(sender_exit);
