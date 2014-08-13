/*
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 * This simple prototype uses a global lock on an lcd's cspace. No
 * rights checking. The lcd need only have a valid cnode mapped by
 * the cptr. Capability grants just copy cnode data from one cspace
 * to another. Capabilities cannot be revoked, so if a cnode is valid,
 * it can be used freely. Cnodes are allocated in a linear order, so
 * no need to do fancy free lists, etc.
 */

#ifndef LCD_PROTOTYPE_API_DEFS_H
#define LCD_PROTOTYPE_API_DEFS_H

#include <linux/mutex.h>
#include <linux/sched.h>
#include <lcd-prototype/lcd.h>

/* CAPABILITIES -------------------------------------------------- */

enum lcd_cap_type {
	LCD_CAP_TYPE_UNFORMED = 0,
	LCD_CAP_TYPE_SYNC_EP  = 1,
};

struct cnode {
	enum lcd_cap_type type;
	void *object;
};

#define LCD_NUM_CAPS 32
struct cspace {
	struct mutex lock;
	struct cnode cnodes[LCD_NUM_CAPS];
	int free_slot;
};

/**
 * Return current's cspace
 */
static inline struct cspace * lcd_cspace(void)
{
	return current->lcd->cspace;
}
/**
 * Allocates memory and initializes the contents of a cspace.
 */
int lcd_mk_cspace(struct cspace **cspace_ptr);
/**
 * Acquires/releases global cspace lock. Must wrap calls to unsafe
 * insert, lookup, etc. with lock/unlock. Lock returns zero if
 * lock successfully taken (no signals).
 */
static inline int lcd_lock_cspace(struct cspace *cspace)
{
	return mutex_lock_interruptible(&cspace->lock);
}
static inline void lcd_unlock_cspace(struct cspace *cspace)
{
	mutex_unlock(&cspace->lock);
}
/**
 * (SAFE) Allocates an empty cnode slot, and returns a cptr to it. Returns
 * non-zero if no free slots available.
 */
static inline int lcd_cap_alloc(struct cspace *cspace, cptr_t *cptr)
{
	int ret = lcd_lock_cspace(cspace);
	if (!ret) {
		if (cspace->free_slot < LCD_NUM_CAPS)
			*cptr = ++cspace->free_slot;
		else
			ret = -ENOMEM;
		lcd_unlock_cspace(cspace);
	}
	return ret;
}
/**
 * (SAFE) Insert object into cspace.
 */
int lcd_cap_insert(struct cspace *cspace, cptr_t cptr, 
		void *object, enum lcd_cap_type type);
/**
 * (UNSAFE) Look up cnode in cspace at cptr.
 */
static inline struct cnode *__lcd_cnode_lookup(struct cspace *cspace, 
					cptr_t cptr)
{
	BUG_ON(cspace->free_slot > LCD_NUM_CAPS);
	if (cptr < cspace->free_slot)
		return &cspace->cnodes[cptr];
	else
		return NULL;
}
/**
 * (SAFE) Look up cnode in cspace at cptr. Returns non-zero if lock
 * failed. out set to null if cnode is marked as unformed.
 */
static inline int lcd_cnode_lookup(struct cspace *cspace, cptr_t cptr,
				struct cnode **out)
{
	struct cnode *c = NULL;
	int ret = lcd_lock_cspace(cspace);
	if (!ret) {
		c = __lcd_cnode_lookup(cspace, cptr);
		if (c && c->type == LCD_CAP_TYPE_UNFORMED)
			c = NULL;
		lcd_unlock_cspace(cspace);
	}
	*out = c;
	return ret;
}
/**
 * (SAFE) Transfer capability from one cspace to another.
 */
int lcd_cap_grant(struct cspace *src_cspace, struct cspace *dest_cspace,
		cptr_t src_cptr, cptr_t dest_cptr);
/**
 * (SAFE) Get capability type.
 */
static inline int lcd_cap_type(struct cspace *cspace, cptr_t cptr, int *out)
{
	struct cnode *cnode;
	int ret = lcd_cnode_lookup(cspace, cptr, &cnode);
	if (ret)
		return ret;
	if (!cnode)
		return -EINVAL;
	else {
		*out = cnode->type;
		return 0;
	}
}
static inline int __lcd_cap_can_read(struct cnode *cnode)
{
	return 1;
}
static inline int __lcd_cap_can_write(struct cnode *cnode)
{
	return 1;
}
static inline int __lcd_cap_can_grant(struct cnode *cnode)
{
	return 1;
}
static inline int lcd_cap_can_read(struct cspace *cspace, cptr_t cptr)
{
	return 1;
}
static inline int lcd_cap_can_write(struct cspace *cspace, cptr_t cptr)
{
	return 1;
}
static inline int lcd_cap_can_grant(struct cspace *cspace, cptr_t cptr)
{
	return 1;
}
/**
 * Deallocates cspace. Caller should ensure no one is trying to
 * use the cspace (e.g., is waiting on the lock).
 */
void lcd_rm_cspace(struct cspace *cspace);

#endif /* LCD_PROTOTYPE_API_DEFS_H */
