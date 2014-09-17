/* 
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 */
#include <linux/slab.h>
#include "defs.h"
#include "../include/common.h"

struct mutex __lcd_cap_lock;
EXPORT_SYMBOL(__lcd_cap_lock);

int lcd_cap_init(void)
{
	mutex_init(&__lcd_cap_lock);
	return 0;
}
EXPORT_SYMBOL(lcd_cap_init);

static void init_cspace_cnodes(struct cspace *cspace)
{
	int i;
	struct cnode *cnode;

	INIT_LIST_HEAD(&cspace->free_list);

	for (i = 0; i < LCD_NUM_CAPS; i++) {
		cnode = &cspace->cnodes[i];

		INIT_LIST_HEAD(&cnode->free_list);
		INIT_LIST_HEAD(&cnode->children);
		INIT_LIST_HEAD(&cnode->child_list);

		list_add(&cnode->free_list, &cspace->free_list);

		cnode->cspace = cspace;
	}

	return;
}

int lcd_mk_cspace(struct cspace **cspace_ptr)
{
	struct cspace *cspace;
	/*
	 * Allocate cspace
	 */
	cspace = kmalloc(sizeof(*cspace), GFP_KERNEL);
	if (!cspace)
		return -ENOMEM;
	/* 
	 * Zero out all data (including cnodes). Sets cnode types to free with
	 * no rights. Initialize free list and cnodes.
	 */
	memset(cspace, 0, sizeof(*cspace));
	init_cspace_cnodes(cspace);
	/*
	 * Done
	 */
	*cspace_ptr = cspace;
	return 0;
}
EXPORT_SYMBOL(lcd_mk_cspace);

static int __lcd_cnode_alloc(struct cspace *cspace, cptr_t *cptr)
{
	struct cnode *cnode;
	if (list_empty(&cspace->free_list)) {
		return -ENOMEM;
	} else {
		/*
		 * Get first available cnode, and remove it from the list.
		 */
		cnode = list_first_entry(&cspace->free_list, struct cnode,
					free_list);
		/*
		 * Remove from free list
		 */
		list_del_init(&cnode->free_list);
		/*
		 * Mark it as unformed
		 */
		__lcd_cnode_set_type(cnode, LCD_CAP_TYPE_UNFORMED);
		/*
		 * Calc index
		 */
		*cptr = cnode - (cspace->cnodes);
		return 0;
	}
}

int lcd_cnode_alloc(struct cspace *cspace, cptr_t *cptr)
{
	int ret;
	/*
	 * LOCK cap
	 */
	ret = lcd_cap_lock();
	if (ret)
		return ret;
	ret = __lcd_cnode_alloc(cspace, cptr);
	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();
	return ret;
}
EXPORT_SYMBOL(lcd_cnode_alloc);

int __lcd_cnode_lookup(struct cspace *cspace, cptr_t cptr, struct cnode **out)
{
	struct cnode *c;
	if (cptr >= LCD_NUM_CAPS)
		return -EINVAL;
	c = &cspace->cnodes[cptr];
	*out = c;
	return 0;
}
EXPORT_SYMBOL(__lcd_cnode_lookup);

static int __lcd_cnode_insert(struct cnode *cnode, void *object, 
			enum lcd_cap_type type, int rights)
{
	if (!__lcd_cnode_is_unformed(cnode))
		return -EINVAL;
	else {
		cnode->object = object;
		cnode->type   = type;
		cnode->rights = rights;
		return 0;
	}
}

int lcd_cnode_insert(struct cspace *cspace, cptr_t cptr, 
		void *object, enum lcd_cap_type type, int rights)
{
	struct cnode *cnode;
	int ret;
	/*
	 * LOCK cap
	 */
	ret = lcd_cap_lock();
	if (ret)
		return ret;
	ret = __lcd_cnode_lookup(cspace, cptr, &cnode);
	if (ret)
		goto out;
	ret = __lcd_cnode_insert(cnode, object, type, rights);
out:
	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();
	return ret;
}
EXPORT_SYMBOL(lcd_cnode_insert);

static int __lcd_cnode_insert_for_grant(struct cnode *src_cnode, 
					struct cnode *dest_cnode, int rights)
{
	int ret;

	ret = __lcd_cnode_insert(dest_cnode, src_cnode->object,
				src_cnode->type, rights);
	if (ret)
		return ret;
	/*
	 * Add dest_cnode to src_cnode's children.
	 */
	list_add(&dest_cnode->child_list, &src_cnode->children);
	return 0;
}

static int __lcd_cnode_grant(struct cspace *src_cspace, 
			struct cspace *dest_cspace, cptr_t src_cptr, 
			cptr_t dest_cptr, int rights)

{
	struct cnode *src_cnode;
	struct cnode *dest_cnode;
	int dest_rights;
	int ret;

	if (src_cspace == dest_cspace)
		return -EINVAL;

	ret = __lcd_cnode_lookup(src_cspace, src_cptr, &src_cnode);
	if (ret)
		return ret;
	if (!__lcd_cnode_can_grant(src_cnode))
		return -EINVAL;
	ret = __lcd_cnode_lookup(dest_cspace, dest_cptr, &dest_cnode);
	if (ret)
		return ret;

	if (!__lcd_cnode_is_occupied(src_cnode) ||
		!__lcd_cnode_is_unformed(dest_cnode))
		return -EINVAL;

	dest_rights = __lcd_cnode_rights(src_cnode) & rights;

	ret = __lcd_cnode_insert_for_grant(src_cnode, dest_cnode, dest_rights);

	return ret;
}

int lcd_cnode_grant(struct cspace *src_cspace, struct cspace *dest_cspace,
		cptr_t src_cptr, cptr_t dest_cptr, int rights)
{
	int ret;
	/*
	 * LOCK cap
	 */
	ret = lcd_cap_lock();
	if (ret)
		return ret;
	ret = __lcd_cnode_grant(src_cspace, dest_cspace, src_cptr, dest_cptr,
				rights);
	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();
	return ret;
}
EXPORT_SYMBOL(lcd_cnode_grant);

static void __lcd_cnode_do_revoke(struct cnode *parent, int rights)
{
	struct cnode *child;
	struct list_head *ptr;

	list_for_each(ptr, &parent->children) {
		child = list_entry(ptr, struct cnode, child_list);
		/*
		 * Revoke child's rights
		 */
		child->rights &= ~rights;
		/*
		 * Recur on its children
		 *
		 * XXX: Be aware of stack overflow
		 */
		__lcd_cnode_do_revoke(child, rights);
	}
	return;
}

static int __lcd_cnode_revoke(struct cspace *cspace, cptr_t cptr, int rights)
{
	int ret;
	struct cnode *parent;

	ret = __lcd_cnode_lookup(cspace, cptr, &parent);
	if (ret)
		return ret;
	__lcd_cnode_do_revoke(parent, rights);
	return 0;
}

int lcd_cnode_revoke(struct cspace *cspace, cptr_t cptr, int rights)
{
	int ret;
	/*
	 * LOCK cap
	 */
	ret = lcd_cap_lock();
	if (ret)
		return ret;
	ret = __lcd_cnode_revoke(cspace, cptr, rights);
	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();
	return ret;
}
EXPORT_SYMBOL(lcd_cnode_revoke);

static void __lcd_cnode_do_free(struct cnode *parent)
{
	struct cnode *child;
	struct list_head *ptr;
	struct list_head *n;

	list_for_each_safe(ptr, n, &parent->children) {
		child = list_entry(ptr, struct cnode, child_list);
		/*
		 * Recur on child's children
		 *
		 * XXX: Be aware of stack overflow
		 */
		__lcd_cnode_do_free(child);
	}
	/*
	 * Mark parent as free, and remove all rights
	 */
	__lcd_cnode_set_type(parent, LCD_CAP_TYPE_FREE);
	__lcd_cnode_set_rights(parent, LCD_CAP_RIGHT_NONE);
	/*
	 * Remove from parent
	 */
	list_del_init(&parent->child_list);
	/*
	 * Add to containing cspace free list
	 */
	list_add(&parent->free_list, &parent->cspace->free_list);
	return;
}

void __lcd_cnode_free(struct cnode *cnode)
{
	__lcd_cnode_do_free(cnode);
	return;
}
EXPORT_SYMBOL(__lcd_cnode_free);

void __lcd_rm_cnode(struct cnode *cnode)
{
	if (__lcd_cnode_is_occupied(cnode))
		__lcd_cnode_do_free(cnode);
	return;
}
EXPORT_SYMBOL(__lcd_rm_cnode);

void __lcd_rm_cspace(struct cspace **cspace_ptr)
{
	int i;
	struct cspace *cspace;
	cspace = *cspace_ptr;
	for (i = 0; i < LCD_NUM_CAPS; i++)
		__lcd_rm_cnode(&cspace->cnodes[i]);
	kfree(cspace);
	*cspace_ptr = NULL;
	return;
}
EXPORT_SYMBOL(__lcd_rm_cspace);
