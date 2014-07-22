/**
 * server.c - server for the LCD client server example
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




int test_func1(unsigned long a, long b, char c) {
	printk(KERN_INFO "test_func1: a:%llu, b:%li, c:%c\n", a, b, c);
	return a + b; 
};

int test_func2(unsigned long a) {
	printk(KERN_INFO "test_func1: a:%llu\n", a);
	return a * 10; 
};

struct server_interface server = { 
	.test_func1 = test_func1,
	.test_func2 = test_func2,
};

static int __init server_init(void)
{
	execution_loop_thread_init();
	register_server(&server);
}

static void __exit server_exit(void)
{
	terminate_execution_loop();
	return;
}

module_init(server_init);
module_exit(server_exit);
