/* 
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 */
#include <linux/slab.h>
#include "defs.h"
#include "../include/common.h"

struct cspace * lcd_mk_cspace(void)
{
	struct cspace *cspace;
	int i;
	/*
	 * Allocate cspace
	 */
	cspace = kmalloc(sizeof(*cspace), GFP_KERNEL);
	if (!cspace)
		return NULL;
	memset(cspace, 0, sizeof(*cspace)); /* sets cnode types to invalid */
	/*
	 * Initialize free list
	 */
	for (i = 0; i < LCD_NUM_CAPS; i++)
		list_add(&cspace->free_list, &cspace->cnodes[i].free_list);
	/*
	 * Initialize lock
	 */
	mutex_init(&cspace->lock);

	return cspace;
}

int lcd_cap_insert(struct cspace *cspace, cptr_t cptr, void *object,
		enum lcd_cap_type type)
{
	struct cnode *cnode;

	cnode = lcd_cnode_lookup(cspace, cap);
	if(cnode == 0) {
		printk(KERN_ERR "Failed to create capability\n");
		return -ENOMEM;
	};

	cnode->type = type;
	cnode->object = object; 

	lcd_cnode_release(cnode);
	return 0; 
}

struct sync_ipc * lcd_cap_make_sync_endpoint(struct cspace *cspace, 
		struct cap_cache *cache, capability_t *cap) {
	struct sync_ipc *rvp;

	rvp = alloc_sync_ipc();
	if(!rvp) {
		printk(KERN_ERR "Failed to allocate memory\n");
		return NULL;
	};

	ret = lcd_alloc_cap(cache, cap); 
	if(ret) {
		printk(KERN_ERR "Failed to allocate free capability\n");
		kfree(rvp);
		return NULL;
	};

        ret = lcd_cap_insert_object(cspace, *cap, rvp, LCD_TYPE_SYNC_EP); 
	if(ret) {
		printk(KERN_ERR "Failed to insert endpoint into the cspace:%d\n", ret);
		kfree(rvp);
		return NULL;
	};

	return rvp;
};

struct cnode *lcd_cnode_lookup(struct cspace *cspace, capability_t cap) {

	unsigned long flags;
	
	if(cap >= LCD_MAX_CAPS) 
		return NULL;

	spin_lock_irqsave(&cspace->lock, flags);
	lcd_cnode_lock(&cspace->cnode[cap]);
	spin_unlock_irqrestore(&cspace->lock, flags);		

	return &cspace->cnode[cap];
};
EXPORT_SYMBOL(lcd_cnode_lookup);

void lcd_cap_drop(struct cspace *cspace, capability_t cap) {
	struct cnode *cnode; 

	cnode = lcd_cnode_lookup(cspace, cap); 
	if(!cnode)
		return; 
	cnode->type = LCD_TYPE_FREE; 
	cnode->object = NULL; 
	lcd_cnode_release(cnode); 
	return; 
};

