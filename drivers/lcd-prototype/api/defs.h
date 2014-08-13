/*
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 * This simple prototype uses a global lock on an lcd's cspace.
 */

#ifndef LCD_PROTOTYPE_API_DEFS_H
#define LCD_PROTOTYPE_API_DEFS_H

#include <linux/spinlock.h>
#include <lcd-prototype/lcd.h>

/* CAPABILITIES -------------------------------------------------- */

enum lcd_cap_type {
	LCD_CAP_TYPE_INVALID = 0,
	LCD_CAP_TYPE_SYNC_EP = 1,
};

enum lcd_cap_right {
	LCD_CAP_RIGHT_READ  = 0x01,
	LCD_CAP_RIGHT_WRITE = 0x02,
	LCD_CAP_RIGHT_GRANT = 0x04,
};
#define LCD_CAP_RIGHT_ALL = (LCD_CAP_RIGHT_READ | LCD_CAP_RIGHT_WRITE | LCD_CAP_RIGHT_GRANT)

struct cnode {
	enum lcd_cap_type type;
	int rights;
	void *object;
	struct listhead free_list;
};

#define LCD_NUM_CAPS 32
struct cspace {
	struct mutex lock;
	struct cnode cnodes[LCD_NUM_CAPS];
	struct listhead free_list;
};

/**
 * Return current's cspace
 */
static inline struct cspace * lcd_cspace(void)
{
	return current->lcd->cspace;
}
/**
 * Set current's cspace
 */
static inline void lcd_store_cspace(struct cspace *cspace)
{
	current->lcd->cspace = cspace;
}
/**
 * Allocates memory and initializes the contents of a cspace.
 */
struct cspace * lcd_mk_cspace(void);
/**
 * Acquires/releases global cspace lock. Must wrap calls to unsafe
 * insert, lookup, etc. with lock/unlock.
 */
void lcd_lock_cspace(struct cspace *cspace);
void lcd_unlock_cspace(struct cspace *cspace);
/**
 * (UNSAFE) Insert object into cspace at cptr with type and rights.
 */
int __lcd_cap_insert(struct cspace *cspace, cptr_t cptr, void *object,
		enum lcd_cap_type type, int rights);
/**
 * (SAFE) Insert object into cspace.
 */
static inline int lcd_cap_insert(struct cspace *cspace, cptr_t cptr, 
				void *object, enum lcd_cap_type type, 
				int rights)
{
	lcd_lock_cspace(cspace);
	__lcd_cap_insert(cspace, cptr, object, type, rights);
	lcd_unlock_cspace(cspace);
}
/**
 * (UNSAFE) Look up cnode in cspace at cptr.
 */
struct cnode *__lcd_cnode_lookup(struct cspace *cspace, cptr_t cptr);
/**
 * (UNSAFE) Free cnode at cptr in cspace.
 */
void __lcd_cap_drop(struct cspace *cspace, cptr_t cptr);
/**
 * (SAFE) Free cnode at cptr in cspace.
 */
static inline void lcd_cap_drop(struct cspace *cspace, cptr_t cptr)
{
	lcd_lock_cspace(cspace);
	__lcd_cap_drop(cspace, cptr);
	lcd_unlock_cspace(cspace);
}
/**
 * (SAFE) Get capability type.
 */
static inline enum lcd_cap_type lcd_cap_type(struct cspace *cspace, cptr_t cptr)
{
	struct cnode *cnode;
	enum lcd_cap_type type;
	lcd_lock_cspace(cspace);
	cnode = __lcd_lookup_cnode(cspace, cptr);
	type = cnode->type;
	lcd_unlock_cspace(cspace);
	return type;
}
/**
 * (SAFE) Get capability rights.
 */
static inline int lcd_cap_rights(struct cspace *cspace, cptr_t cptr)
{
	struct cnode *cnode;
	int rights;
	lcd_lock_cspace(cspace);
	cnode = __lcd_lookup_cnode(cspace, cptr);
	rights = cnode->rights;
	lcd_unlock_cspace(cspace);
	return rights;
}
static inline int lcd_cap_can_read()
{
	return rights & LCD_CAP_RIGHT_READ;
}
static inline int lcd_cap_can_write(int rights)
{
	return rights & LCD_CAP_RIGHT_WRITE;
}
static inline int lcd_cap_can_grant(int rights)
{
	return rights & LCD_CAP_RIGHT_GRANT;
}
/**
 * Deallocates cspace. Caller should ensure no one is trying to
 * use the cspace (e.g., is waiting on the lock).
 */
void lcd_rm_cspace(struct cspace *cspace);

#endif /* LCD_PROTOTYPE_API_DEFS_H */
