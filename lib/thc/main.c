/* 
 * main.c
 *
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Muktesh Khole <muktesh.khole@utah.edu>
 * Copyright: University of Utah
 */

#include <linux/init.h>
#include <linux/module.h>
#include <lcd-domains/thc.h>
#include <lcd-domains/thcinternal.h>
#include <lcd-domains/thcsync.h>
void foo1(void);
void foo2(void);
bool testFlag = false;

void foo1(void) {
	printk(KERN_ERR "lcd async entering foo1");
	while (testFlag == false) {
		printk(KERN_ERR "lcd async yielding to foo2");
		THCYield();
	}
	printk(KERN_ERR "lcd async foo1 complete");
}

void foo2(void) {
	printk(KERN_ERR "lcd async entering foo2");
	testFlag = true;
}

static int __init api_init(void)
{
	DO_FINISH(ASYNC(foo1();foo2();););
	printk(KERN_ERR "lcd async entering module");
	return 0;
}

static void __exit api_exit(void)
{
	printk(KERN_ERR "lcd async exiting module");
	return;
}

module_init(api_init);
module_exit(api_exit);

