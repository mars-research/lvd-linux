/* 
 * main.c
 *
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Muktesh Khole <muktesh.khole@utah.edu>
 * Copyright: University of Utah
 */

#include <linux/init.h>
#include <linux/module.h>

int __init api_init(void)
{
	printk(KERN_ERR "lcd async entering module");
	return 0;
}

void __exit api_exit(void)
{
	printk(KERN_ERR "lcd async exiting module");
	return;
}

module_init(api_init);
module_exit(api_exit);

