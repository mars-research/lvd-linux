/*
 * dstore.h - lcd-local data store
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

#ifndef LCD_PROTOTYPE_DSTORE_H
#define LCD_PROTOTYPE_DSTORE_H

#include <linux/kernel.h>
#include <lcd-prototype/lcd.h>
#include "utcb.h"
#include "api-internal.h"
#include "../api/defs.h"

typedef cptr_t dsptr_t;

/**
 * Put an object in the current lcd's data store.
 */
int lcd_ds_store(void *object, u64 badge, dsptr_t *out);
/**
 * Remove an object (but doesn't free it) from the current
 * lcd's data store. Returns the stored object.
 */
void * lcd_ds_drop(u64 badge, dsptr_t ptr);
/**
 * Get the object in the data store.
 */
void * lcd_ds_read(u64 badge, dsptr_t out);

#endif /* LCD_PROTOTYPE_DSTORE_H */
