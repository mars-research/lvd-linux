/* 
 * main.c
 *
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Muktesh Khole <muktesh.khole@utah.edu>
 * Copyright: University of Utah
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <lcd-domains/thc.h>
#include <lcd-domains/thcinternal.h>
#include <lcd-domains/thcsync.h>
void foo1(void);
void foo2(void);

noinline void foo1(void) {
	printk(KERN_ERR "lcd async entering foo1\n");
	printk(KERN_ERR "lcd async yielding to foo2\n");
	int count = 0;
	while (count < 2) {
	 printk(KERN_ERR "lcd async Yielding\n");
	 THCYield();	
	 count++;
	}
	printk(KERN_ERR "lcd async foo1 complete\n");
}

noinline void foo2(void) {
	printk(KERN_ERR "lcd async entering foo2\n");
	printk(KERN_ERR "lcd async foo2 Complete\n");
}

static int __init api_init(void)
{
	current->ptstate = kzalloc(sizeof(struct ptstate_t), GFP_KERNEL);
	thc_latch_init(&(current->ptstate->latch));
	thc_init();
    //assert((PTS() == NULL) && "PTS already initialized");
	printk(KERN_ERR "lcd async entering module ptstate allocated");
	DO_FINISH(ASYNC(foo1();); printk(KERN_ERR "lcd async apit_init coming back\n"); ASYNC(foo2();););
    printk(KERN_ERR "lcd async end of DO_FINISH");
	return 0;
}

static void __exit api_exit(void)
{
	printk(KERN_ERR "lcd async exiting module and deleting ptstate");
	thc_done();
	kfree(current->ptstate);
	return;
}

module_init(api_init);
module_exit(api_exit);

