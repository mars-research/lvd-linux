/*
 * dstore.h - routines for creating and manipulating a data store
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 * Implemented by liblcd.
 */

#ifndef LCD_PROTOTYPE_DSTORE_H
#define LCD_PROTOTYPE_DSTORE_H

#include <lcd-prototype/lcd.h>

typedef u64 dsptr_t;

enum dstore_rights {
	DSTORE_READ_RIGHTS  = 0x01,
	DSTORE_WRITE_RIGHTS = 0x02,
};
#define DSTORE_ALL_RIGHTS (DSTORE_READ_RIGHTS | DSTORE_WRITE_RIGHTS)

struct dsnode {
	int rights;
	void *object;
};
#define DSTORE_CAN_READ(dsnode) ((dsnode)->rights & DSTORE_READ_RIGHTS)
#define DSTORE_CAN_WRITE(dsnode) ((dsnode)->rights & DSTORE_WRITE_RIGHTS)

/**
 * Initializes the lcd's data store. Must call before
 * other routines. 
 */
int lcd_ds_init(void);
/**
 * Tears down lcd's data store. Should call before
 * lcd dies.
 */
int lcd_ds_exit(void);
/**
 * Stores object in lcd's data store for the remote lcd identified
 * by badge, and returns an identifying dsptr. Object can be
 * retrieved in the future via lcd_ds_lookup with dsptr and badge.
 */
int lcd_ds_store(void *object, dsptr_t *dsptr, u64 badge, int rights);
/**
 * Looks up dsnode containing object referred to by (badge, dsptr) combo.
 * Returns NULL if no such combo is mapped. No permission checks.
 */
struct dsnode *__lcd_ds_lookup(u64 badge, dsptr_t dsptr);
/**
 * Unmaps (badge, dsptr) combo from data store. Returns pointer to mapped 
 * object (so that e.g. caller can free it). If check is non-zero,
 * confirms dnode has all rights.
 */
void * __lcd_ds_drop(u64 badge, dsptr_t dsptr, int check);
static inline void * lcd_ds_drop(u64 badge, dsptr_t dsptr)
{
	return __lcd_ds_drop(badge, dsptr, 1);
}
static inline void * lcd_ds_drop_no_check(u64 badge, dsptr_t dsptr)
{
	return __lcd_ds_drop(badge, dsptr, 0);
}
/**
 * Looks up and returns object mapped by (badge, dsptr) combo. Confirms
 * read right to object; returns NULL if no read right. Caller is
 * responsible for not accidentally writing to the object.
 */
static inline void * lcd_ds_read(u64 badge, dsptr_t dsptr)
{
	struct dsnode *dsnode;
	dsnode = __lcd_ds_lookup(badge, dsptr);
	if (!dsnode)
		return NULL;
	else if (!DSTORE_CAN_READ(dsnode))
		return NULL;
	else
		return dsnode->object;
}
/**
 * Looks up and returns object mapped by (badge, dsptr) combo. Confirms
 * write right to object; returns NULL if no write right. Caller is
 * responsible for not accidentally reading the object.
 */
static inline void * lcd_ds_write(u64 badge, dsptr_t dsptr)
{
	struct dsnode *dsnode;
	dsnode = __lcd_ds_lookup(badge, dsptr);
	if (!dsnode)
		return NULL;
	else if (!DSTORE_CAN_WRITE(dsnode))
		return NULL;
	else
		return dsnode->object;
}

#endif /* LCD_PROTOTYPE_DSTORE_H */
