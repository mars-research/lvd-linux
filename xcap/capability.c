
#undef __KERNEL__
#define __KERNEL__

#undef MODULE
#define MODULE

#include "capability.h"

int init_module(void)
{
	printk(KERN_INFO "Initializing LCD Module %d\n", PAGE_SIZE);
	return 0;    // Non-zero return means that the module couldn't be loaded.
}

void cleanup_module(void)
{
	printk(KERN_INFO "Cleaning up LCD Module\n");
}


