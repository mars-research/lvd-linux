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
MODULE_DESCRIPTION("LCD ping-pong test receiver");


extern int lcd_ping_pong_init_receiver(void);

int module_execution_loop(void) {

	return 0;
};

int module_thread(void *p) {
	int ret;

	ret = lcd_ping_pong_init_receiver();
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

static int __init receiver_init(void)
{
	printk(KERN_ERR "IPC ping-pong, sender module loaded\n");
	return module_thread_init();
}

static void __exit receiver_exit(void)
{
}

module_init(receiver_init);
module_exit(receiver_exit);
