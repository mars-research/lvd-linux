/* 
 * main.c
 *
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Muktesh Khole <muktesh.khole@utah.edu>
 * Copyright: University of Utah
 */

#include <linux/init.h>
#include <linux/module.h>
#include "test.h"

int __init api_init(void)
{
	bool res = true;
	printk(KERN_ERR "lcd cspace entering module");
	res = test_insert();
	if (res == false) {
		printk(KERN_ERR "lcd_cap_insert failed");
	} else {
		printk(KERN_ERR "lcd_cap_insert succeeded");
		test_delete();
	}

	return 0;
}

void __exit api_exit(void)
{
	printk(KERN_ERR "lcd cspace exiting module");
	return;
}

module_init(api_init);
module_exit(api_exit);

