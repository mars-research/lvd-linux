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

#include <lcd/lcd.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD ping-pong IPC test (sender)");

struct sync_ipc *rvp; 

int lcd_ping_pong_init_sender(void) {
	int ret; 
	capability_t rvp_cap;
	
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

        rvp_cap = lcd_cap_create_capability(&current->cspace, (void*)rvp, 0);
	if(rvp_cap == 0) {
		printk(KERN_ERR "Failed to create capability\n");
		kfree(rvp);
		return -ENOMEM;
	};

	current->utcb->boot_info.boot_rvp = rvp_cap;
	return 0;
};

int lcd_ping_pong_init_receiver(void) 
{
	int ret; 
	struct sync_ipc *rvp; 
	capability_t rvp_cap;
	
	ret = lcd_enter();
	if (ret) {
		printk(KERN_ERR "Failed to enter LCD environment\n");
		return ret;
	};

	BUG_ON(rvp == NULL); 

        rvp_cap = lcd_cap_create_capability(&current->cspace, (void*)rvp, 0);
	if(rvp_cap == 0) {
		printk(KERN_ERR "Failed to create capability\n");
		kfree(rvp);
		return -ENOMEM;
	};

	current->utcb->boot_info.boot_rvp = rvp_cap;
	return 0;
};
EXPORT_SYMBOL(lcd_ping_pong_init_receiver); 

int module_execution_loop(void) {

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
