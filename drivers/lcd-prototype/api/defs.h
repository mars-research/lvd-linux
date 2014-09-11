/*
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

#ifndef LCD_PROTOTYPE_API_DEFS_H
#define LCD_PROTOTYPE_API_DEFS_H

#include <linux/mutex.h>
#include <linux/sched.h>
#include <lcd-prototype/lcd.h>
#include "../include/api-internal.h"

/* CAPABILITIES -------------------------------------------------- */

/*
 * A cspace is a simple static array of cnodes. Each cnode can have 0 or more
 * derived children, in a linked list. This simple prototype uses a global 
 * mutex for the entire capability module. All routines marked as unsafe
 * should be executed with the lock held. 
 *
 * Unlike seL4, objects are not deallocated automatically when no one has a 
 * capability to it. Here is a typical sequence that exhibits the current 
 * features:
 *
 *   // Create cspaces for two lcds.
 *   lcd_mk_cspace(&cspace1);
 *   lcd_mk_cspace(&cspace2);
 *
 *   // Create a rendezvous point.
 *   rvp = mk_rvp();
 *
 *   // Put rvp in cspace1, give lcd1 all rights (lcd1 is the owner). The 
 *   // corresponding cnode will be parentless.
 *   lcd_cnode_alloc(cspace1, &cptr1);
 *   lcd_cnode_insert(cspace1, cptr1, rvp, LCD_CAP_RIGHT_ALL);
 *
 *   // lcd1 grants write rights to lcd2. Internally, lcd1's cnode
 *   // will be the parent of lcd2's cnode.
 *   lcd_cnode_grant(cspace1, cptr1, cspace2, cptr2, rvp, 
 *                 LCD_CAP_RIGHT_WRITE);
 *
 *   // lcd2 tries to send on rvp
 *   lcd_cap_lock();
 *   cnode2 = __lcd_cnode_lookup(cspace2, cptr2);
 *   if (__lcd_cnode_can_write(cnode2) && __lcd_cnode_sync_ep(cnode2))
 *      ... do send ...
 *   else
 *      ... fail ...
 *   lcd_cap_unlock();
 *
 *   // lcd1 revokes write rights to all derived children (lcd2)
 *   lcd_cnode_revoke(cspace1, cptr1, LCD_CAP_RIGHT_WRITE);
 *
 *   // lcd1 destroys the rvp. This revokes all rights given to lcd1,
 *   // and transitively, all rights to lcd2 (if they hadn't been
 *   // revoked already). Since the hypervisor created the object, it
 *   // will free the allocated memory for the rvp.
 *   lcd_cap_lock();
 *   ret = __lcd_cnode_lookup(cspace1, cptr1, &cnode1);
 *   if (ret)
 *       ... look up error ...
 *   if (__lcd_cnode_is_owner(cnode1) && __lcd_cnode_sync_ep(cnode1))
 *       rvp = cnode->object;
 *       free_rvp(rvp);
 *       __lcd_cnode_free(cspace1, cnode1);
 *   else
 *       ... fail ...
 *   lcd_cap_unlock();
 *   
 *   // lcd1 and lcd2 are torn down
 *   lcd_cap_lock();
 *   __lcd_rm_cspace(cspace1);
 *   __lcd_rm_cspace(cspace2);
 *   lcd_cap_unlock();
 *
 * All routines must exec in process context (no interrupt context,
 * since processes will sleep when trying to acquire the mutex).
 */

enum lcd_cap_type {
	LCD_CAP_TYPE_FREE     = 0,
	LCD_CAP_TYPE_UNFORMED = 1,
	LCD_CAP_TYPE_SYNC_EP  = 2,
	LCD_CAP_TYPE_DSTORE   = 3,
};

enum lcd_cap_right {
	LCD_CAP_RIGHT_READ    = (1 << 0),
	LCD_CAP_RIGHT_WRITE   = (1 << 1),
	LCD_CAP_RIGHT_GRANT   = (1 << 2),
	LCD_CAP_RIGHT_OWNER   = (1 << 3),
};
#define LCD_CAP_RIGHT_ALL ((1 << 4) - 1)
#define LCD_CAP_RIGHT_NONE 0

struct cspace;

struct cnode {
	enum lcd_cap_type type;
	int rights;
	/* Pointer to containing cspace */
	struct cspace *cspace;
	/* Linked list of free cnodes */
	struct list_head free_list;
	/* Pointer to linked list of my children */
	struct list_head children;
	/* Linked list of siblings */
	struct list_head child_list;
	/* The contained object */
	void *object;
};

#define LCD_NUM_CAPS 32
struct cspace {
	struct cnode cnodes[LCD_NUM_CAPS];
	struct list_head free_list;
};

extern struct mutex __lcd_cap_lock;

/**
 * Initializes capability module.
 */
int lcd_cap_init(void);
/**
 * Lock entire capability module (all cspaces)
 */
static inline int lcd_cap_lock(void)
{
	return mutex_lock_interruptible(&__lcd_cap_lock);
}
/**
 * Unlock entire capability module (all cspaces)
 */
static inline void lcd_cap_unlock(void)
{
	return mutex_unlock(&__lcd_cap_lock);
}
/**
 * Return current's cspace
 */
static inline struct cspace * lcd_cspace(void)
{
	return current->lcd->cspace;
}
/**
 * [SAFE]
 * Allocates memory and initializes the contents of a cspace.
 */
int lcd_mk_cspace(struct cspace **cspace_ptr);
/**
 * [SAFE, USES LOCK]
 * Allocates an empty cnode slot, and returns a cptr to it. Returns
 * non-zero if no free slots available, or couldn't get lock.
 *
 * (It is necessary to separate allocation and insertion since an lcd
 * can specify where it wants incoming capabilities inserted, via ipc.)
 */
int lcd_cnode_alloc(struct cspace *cspace, cptr_t *cptr);
/**
 * [UNSAFE]
 * Look up cnode in cspace at cptr. Returns non-zero if cptr is out of range. 
 */
int __lcd_cnode_lookup(struct cspace *cspace, cptr_t cptr, struct cnode **out);
/**
 * [SAFE, USES LOCK]
 * Insert object into cspace with type and rights. Fails if the type of
 * the cnode is not unformed (cnode is free or occupied), or if it fails to 
 * get the lock (interrupted).
 *
 * (Use lcd_cnode_grant to transfer a capability from one cspace to another,
 * and update the parent's children.)
 */
int lcd_cnode_insert(struct cspace *cspace, cptr_t cptr, 
		void *object, enum lcd_cap_type type, int rights);
/**
 * [SAFE, USES LOCK]
 * Transfer capability from one cspace to another. Fails if we can't get
 * the lock; if source is not occupied, or dest is not unformed; if source 
 * doesn't have grant rights; or if the source and destination cspaces are the 
 * same.
 */
int lcd_cnode_grant(struct cspace *src_cspace, struct cspace *dest_cspace,
		cptr_t src_cptr, cptr_t dest_cptr, int rights);
/**
 * [SAFE, USES LOCK]
 * Revoke rights from all derived cnodes. Fails if we can't lock. A cnode
 * is not freed if its rights drop to nothing.
 */
int lcd_cnode_revoke(struct cspace *cspace, cptr_t cptr, int rights);
/**
 * [UNSAFE]
 * Frees the slot containing cnode in its containing cspace, and recursively 
 * frees derived children in other cspaces (so this also acts as a total 
 * revoke).
 */
void __lcd_cnode_free(struct cnode *cnode);
/**
 * [UNSAFE]
 * Deallocates cspace. Caller should ensure no one is trying to
 * use the cspace (e.g., via grant, insert, etc.). This will revoke all
 * cnodes derived from this cspace.
 */
void __lcd_rm_cspace(struct cspace **cspace_ptr);
/**
 * [UNSAFE]
 * Quick inlines for determining rights and type. These should be wrapped
 * in a lookup/release.
 */
static inline int __lcd_cnode_rights(struct cnode *cnode)
{
	return cnode->rights;
}
static inline void __lcd_cnode_set_rights(struct cnode *cnode, int rights)
{
	cnode->rights = rights;
}
static inline int __lcd_cnode_type(struct cnode *cnode)
{
	return cnode->type;
}
static inline void __lcd_cnode_set_type(struct cnode *cnode, 
				enum lcd_cap_type t)
{
	cnode->type = t;
}
static inline void * __lcd_cnode_object(struct cnode *cnode)
{
	return cnode->object;
}
static inline int __lcd_cnode_can_read(struct cnode *cnode)
{
	return cnode->rights & LCD_CAP_RIGHT_READ;
}
static inline int __lcd_cnode_can_write(struct cnode *cnode)
{
	return cnode->rights & LCD_CAP_RIGHT_WRITE;
}
static inline int __lcd_cnode_can_grant(struct cnode *cnode)
{
	return cnode->rights & LCD_CAP_RIGHT_GRANT;
}
static inline int __lcd_cnode_is_owner(struct cnode *cnode)
{
	return cnode->rights & LCD_CAP_RIGHT_OWNER;
}
static inline int __lcd_cnode_is_free(struct cnode *cnode)
{
	return cnode->type == LCD_CAP_TYPE_FREE;
}
static inline int __lcd_cnode_is_unformed(struct cnode *cnode)
{
	return cnode->type == LCD_CAP_TYPE_UNFORMED;
}
static inline int __lcd_cnode_is_occupied(struct cnode *cnode)
{
	return !(__lcd_cnode_is_unformed(cnode) || __lcd_cnode_is_free(cnode));
}
static inline int __lcd_cnode_is_sync_ep(struct cnode *cnode)
{
	return cnode->type == LCD_CAP_TYPE_SYNC_EP;
}

/* IPC -------------------------------------------------- */

/* 
 * There is no low-level interface to the api code, so we need
 * to make the ipc routines directly callable for now, in
 * include/api-internal.h. All other api code is directly or
 * indirectly accessed via ipc.
 */

struct sync_endpoint {
	struct list_head senders;
        struct list_head receivers;
        struct mutex lock;
};

#endif /* LCD_PROTOTYPE_API_DEFS_H */
