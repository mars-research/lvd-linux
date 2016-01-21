/*
 * cap.h
 *
 * Operations an LCD can invoke on its own cspace.
 *
 * Why do we have this header if we have libcap? Because an LCD
 * cannot do e.g. cap_insert on its own cspace. It does operations
 * on its own cspace via hypercalls out into the microkernel. Similar
 * for kLCDs. That's what these functions are for.
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_CAP_H
#define LCD_DOMAINS_CAP_H

#include <libcap.h>

/**
 * lcd_cap_delete -- Delete the capability in slot from caller's cspace
 * @slot: the cptr to the capability to delete
 *
 * This may change the state of the caller. (For example, if the caller is
 * a regular lcd, and if the capability is to a page, the page will be unmapped
 * from the caller's guest physical address space.)
 *
 * If this is the last capability to the object, object-specific cleanup
 * code will run.
 *
 * If @slot points to an empty slot in the caller's cspace, silently does
 * nothing.
 */
void lcd_cap_delete(cptr_t slot);

/**
 * lcd_cap_revoke -- Revoke all derived capabilities
 * @slot: the cptr to the slot that contains the parent capability
 *
 * Does not delete the caller's capability, but deletes all derived
 * capabilities in other cspaces.
 *
 * This may change the state of the LCD's whose capabilities are revoked (see
 * comment lcd_cap_delete).
 *
 * Returns non-zero if revocation failed, in whole or in part.
 */
int lcd_cap_revoke(cptr_t slot);

#endif /* LCD_DOMAINS_CAP_H */
