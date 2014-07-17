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


struct server_interface *server; 

int register_server(struct server_interface *s) {
	server = s;
};
EXPORT_SYMBOL(register_server);

int server_test_1(struct server_interface *server) {
	ret = server->test_func1(1, 2, 3);
	printk(KERN_INFO "Server test1 returns: %d\n");
	return ret; 	
};

int server_test_2(struct server_interface *server) {
	ret = server->test_func2(4);
	printk(KERN_INFO "Server test1 returns: %d\n");
	return ret; 	
};

int client_main(void) {
	server_test_1(server);
	server_test_2(server);
}


static int __init client_init(void)
{
	lcd_thread_init();
	return;
}

static void __exit client_exit(void)
{
}

module_init(client_init);
module_exit(client_exit);
