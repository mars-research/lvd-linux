/*
 * ipc.h - communicate with other lcd's
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 * These are the only routines that call directly into the
 * api code. In the future, their bodies would be replaced
 * by some kind of low-level hypercall code.
 */

#ifndef LCD_PROTOTYPE_IPC_H
#define LCD_PROTOTYPE_IPC_H

#include <lcd-prototype/lcd.h>
#include "utcb.h"
#include "api-internal.h"

static inline int lcd_send(cptr_t c)
{
	return __lcd_send(c);
}
static inline int lcd_reply(void)
{
	return lcd_send(lcd_reply_cap());
}
static inline int lcd_call(cptr_t c)
{
	return __lcd_call(c);
}
static inline int lcd_select(struct lcd_handlers *hs, int hs_count)
{
	return __lcd_select(hs, hs_count);
}

#endif /* LCD_PROTOTYPE_IPC_H */
