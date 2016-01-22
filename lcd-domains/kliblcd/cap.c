/*
 * cap.c
 *
 * Copyright: University of Utah
 */

#include <libcap.h>
#include <lcd-domains/liblcd.h>

void lcd_cap_delete(cptr_t slot)
{
	cap_delete(current->lcd->cspace, slot);
}

int lcd_cap_revoke(cptr_t slot)
{
	return cap_revoke(current->lcd->cspace, slot);
}

int lcd_cptr_alloc(cptr_t *slot_out)
{
	return cptr_alloc(current->cptr_cache, slot_out);
}

void lcd_cptr_free(cptr_t slot)
{
	cptr_free(current->cptr_cache, slot);
}
