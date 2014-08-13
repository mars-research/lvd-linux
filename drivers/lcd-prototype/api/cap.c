/* 
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 */
#include <linux/slab.h>
#include "defs.h"
#include "../include/common.h"

int lcd_mk_cspace(struct cspace **cspace_ptr)
{
	struct cspace *cspace;
	int i;
	/*
	 * Allocate cspace
	 */
	cspace = kmalloc(sizeof(*cspace), GFP_KERNEL);
	if (!cspace)
		return -ENOMEM;
	/* 
	 * Sets cnode types to unformed, and free_slot to 0
	 */
	memset(cspace, 0, sizeof(*cspace));
	/*
	 * Initialize lock
	 */
	mutex_init(&cspace->lock);
	/*
	 * Done
	 */
	*cspace_ptr = cspace;
	return 0;
}

int lcd_cap_insert(struct cspace *cspace, cptr_t cptr, void *object,
		enum lcd_cap_type type)
{
	struct cnode *cnode = NULL;
	int ret;
	/*
	 * Lookup cnode
	 */
	ret = lcd_lock_cspace(cspace);
	if (!ret) {
		cnode = __lcd_cnode_lookup(cspace, cptr);
		lcd_unlock_cspace(cspace);
	} else {
		return ret;
	}
	/*
	 * Set vals
	 */
	if (cnode && cnode->type == LCD_CAP_TYPE_UNFORMED) {
		cnode->object = object;
		cnode->type = type;
		return 0;
	} else {
		return -EINVAL;
	}
}

int lcd_cap_grant(struct cspace *src_cspace, struct cspace *dest_cspace,
		cptr_t src_cptr, cptr_t dest_cptr)
{
	struct cnode *src_cnode;
	int ret;
	/*
	 * Get source capability data
	 */
	ret = lcd_cnode_lookup(src_cspace, src_cptr, &src_cnode);
	if (ret)
		return ret;
	if (!src_cnode)
		return -EINVAL;
	if (!__lcd_cap_can_grant(src_cnode))
		return -EINVAL;
	/*
	 * Insert into destination
	 */
	return lcd_cap_insert(dest_cspace, dest_cptr, src_cnode->object,
			src_cnode->type);
}

void lcd_rm_cspace(struct cspace *cspace)
{
	kfree(cspace);
}
