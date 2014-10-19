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
#include <linux/spinlock.h>
#include <lcd/cap.h>
#include "test.h"

struct cdt_cache_t {
	spinlock_t lock;
	struct kmem_cache *cdt_root_cache;
};

struct cdt_cache_t cdt_cache;

int __init api_init(void)
{
	bool res = true;
	printk(KERN_ERR "lcd_cap entering module");
	spin_lock_init(&(cdt_cache.lock));
	cdt_cache.cdt_root_cache = KMEM_CACHE(cdt_root_node, 0);
	if(!cdt_cache.cdt_root_cache){
		printk(KERN_ERR "lcd_cap Failed to initialize cdt_root_node allocator\n");
		return -ENOMEM;
	};

	res = begintests();
	if (res == false) {
		printk(KERN_ERR "tests failed");
	} else {
		printk(KERN_ERR "tests succeeded");
	}
	return 0;
}

void __exit api_exit(void)
{
	printk(KERN_ERR "lcd cspace exiting module");
	return;
}

struct cdt_root_node * get_cdt_root(void) {
	unsigned long flags;
	struct cdt_root_node *cdt_node;
	spin_lock_irqsave(&cdt_cache.lock, flags);
	cdt_node = kmem_cache_alloc(cdt_cache.cdt_root_cache, 0);
	cdt_node->state = ALLOCATION_VALID;
	spin_lock_init(&(cdt_node->lock));
	cdt_node->node_count = 0;
	spin_unlock_irqrestore(&cdt_cache.lock, flags);	
	return cdt_node;
}

void free_cdt_root(struct cdt_root_node *cdt_node) {
	unsigned long flags;
	spin_lock_irqsave(&cdt_cache.lock, flags);
	cdt_node->state = ALLOCATION_REMOVED;
	cdt_node->node_count = 0;
	kmem_cache_free(cdt_cache.cdt_root_cache, cdt_node);
	spin_unlock_irqrestore(&cdt_cache.lock, flags);	
}

module_init(api_init);
module_exit(api_exit);

