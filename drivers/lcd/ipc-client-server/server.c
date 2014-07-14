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
MODULE_DESCRIPTION("LCD client-server test server");

extern int lcd_client_server_init_server(void);

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

	ret = lcd_client_server_init_server();
	if (ret) {
		printk(KERN_ERR "sender failed to initialize ping-pong environtment\n");
		return -EINVAL;
	}

	return module_execution_loop();
};

int module_thread_init(void) {

	struct task_struct *t;

	t = kthread_create(module_thread, NULL, "cs-server");
	if (!t) {
		printk(KERN_ERR "Failed to create module thread\n");
		return -EINVAL;
	};

	wake_up_process(t);
	return 0;
}

static int __init server_init(void)
{
	printk(KERN_ERR "IPC ping-pong, sender module loaded\n");
	return module_thread_init();
}

static void __exit server_exit(void)
{
}

module_init(server_init);
module_exit(server_exit);
