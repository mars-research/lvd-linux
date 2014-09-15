/*
 * dstore.c - lcd-local data store
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

#include "../include/dstore.h"

int lcd_ds_store(void *object, u64 badge, dsptr_t *out)
{
	struct cspace *cspace;
	int ret;

	cspace = current_lcd()->cspace;

	/* Ignore the badge for now */
	ret = lcd_cnode_alloc(cspace, out);
	if (ret)
		goto fail1;

	/* Insert object */
	ret = lcd_cnode_insert(cspace, *out, object, LCD_CAP_TYPE_DSTORE, 
			LCD_CAP_RIGHT_ALL);
	if (ret)
		goto fail2;

fail2:
	/* XXX: no way to de-alloc cnodes */
fail1:
	return ret;
}
EXPORT_SYMBOL(lcd_ds_store);

static void * __lcd_ds_drop(u64 badge, dsptr_t ptr)
{
	struct cnode *cnode;
	struct cspace *cspace;
	void *obj;
	int ret;

	cspace = current_lcd()->cspace;

	ret = __lcd_cnode_lookup(cspace, ptr, &cnode);
	if (ret)
		return NULL;

	if (__lcd_cnode_type(cnode) != LCD_CAP_TYPE_DSTORE)
		return NULL;

	obj = cnode->object;

	__lcd_cnode_free(cnode);

	return obj;	
}

void * lcd_ds_drop(u64 badge, dsptr_t ptr)
{
	int ret;
	void *obj;

	ret = lcd_cap_lock();
	if (ret)
		return NULL;

	obj = __lcd_ds_drop(badge, ptr);

	lcd_cap_unlock();

	return obj;
}
EXPORT_SYMBOL(lcd_ds_drop);

static void * __lcd_ds_read(u64 badge, dsptr_t ptr)
{
	struct cnode *cnode;
	struct cspace *cspace;
	int ret;

	cspace = current_lcd()->cspace;

	ret = __lcd_cnode_lookup(cspace, ptr, &cnode);
	if (ret)
		return NULL;

	if (__lcd_cnode_type(cnode) != LCD_CAP_TYPE_DSTORE)
		return NULL;

	return cnode->object;
}

void * lcd_ds_read(u64 badge, dsptr_t ptr)
{
	int ret;
	void *obj;

	ret = lcd_cap_lock();
	if (ret)
		return NULL;

	obj = __lcd_ds_read(badge, ptr);

	lcd_cap_unlock();

	return obj;
}
EXPORT_SYMBOL(lcd_ds_read);
