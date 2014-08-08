/**
 * client-server-idl.c - Module that launches client and server 
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
#include <lcd/api.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD client-server test launcher");

static int __init cslaunch_init(void)
{
 	capability_t boot_caps[3];
	capability_t shared_rvp = 2; 
	capability_t client_lcd_cap = 3; 
	capability_t server_lcd_cap = 4; 

	boot_caps[0] = lcd_api_cap(); 
	boot_caps[1] = lcd_api_reply_cap(); 
	boot_caps[2] = shared_rvp; 


	ret = lcd_init_current();
	if (ret) {
		printk(KERN_ERR "Failed to init current thread with LCD:%d\n", ret);
		return -EINVAL;
	}

	ret = lcd_api_create_sync_endpoint(shared_rvp); 
	if(ret) {
		printk(KERN_ERR "Failed to create sync endpoint\n");
		return -ENOSPC;
	};

	ret = lcd_api_create_lcd("cs-client", &boot_caps, 3, client_lcd_cap); 
	if(ret) {
		printk(KERN_ERR "Failed to create client LCD\n");
		return -ENOSPC;
	};

	ret = lcd_api_create_lcd("cs-server", &boot_caps, 3, server_lcd_cap); 
	if(ret) {
		printk(KERN_ERR "Failed to create server LCD\n");
		return -ENOSPC;
	};

	return;
}

static void __exit cslaunch_exit(void)
{
}

module_init(cslaunch_init);
module_exit(cslaunch_exit);
