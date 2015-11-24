/**
 * cap.c
 *
 * Authors:
 *   Charlie Jacobsen  <charlesj@cs.utah.edu>
 */

#include <lcd-domains/kliblcd.h>
#include <lcd-domains/utcb.h>
#include <lcd-domains/types.h>
#include "../microkernel/internal.h"

int klcd_cap_grant(cptr_t lcd, cptr_t src, cptr_t dest)
{
	return __lcd_cap_grant_cheat(current->lcd, lcd, src, dest);
}

int klcd_cap_page_grant_map(cptr_t lcd, cptr_t page, cptr_t dest, gpa_t gpa)
{
	return __lcd_cap_page_grant_map_cheat(current->lcd, lcd, page, dest, 
					gpa);
}

void klcd_cap_delete(cptr_t slot)
{
	/*
	 * Delete capability from cspace
	 */
	__lcd_cap_delete(&current->lcd->cspace, slot);
	/*
	 * Return cptr
	 */
	lcd_free_cptr(slot);
}

int klcd_cap_revoke(cptr_t slot)
{
	/*
	 * Revoke child capabilities
	 *
	 * XXX: How do the lcd's know these slots are now free? The microkernel
	 * won't tell them.
	 */
	return __lcd_cap_revoke(&current->lcd->cspace, slot);
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(klcd_cap_grant);
EXPORT_SYMBOL(klcd_cap_page_grant_map);
EXPORT_SYMBOL(klcd_cap_delete);
EXPORT_SYMBOL(klcd_cap_revoke);
