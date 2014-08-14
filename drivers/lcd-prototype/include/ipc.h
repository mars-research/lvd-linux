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

struct lcd_handler {
	cptr_t c;
	int (*handler)(void);
};

static inline int lcd_send(cptr_t c)
{
	return __lcd_send(current, c);
}
static inline int lcd_reply(void)
{
	return lcd_send(current, lcd_reply_cap());
}
static inline int lcd_call(cptr_t c)
{
	return __lcd_call(current, c);
}
static inline int lcd_select(struct lcd_handlers *hs, int hs_count)
{
	cptr_t cs[hs_count];
	int recv_idx;
	int i;
	int ret;
	for (i = 0; i < hs_count; i++)
		cs[i] = hs[i].c;
	ret = __lcd_select(current, cs, hs_count, &idx);
	if (ret)
		return ret;
	else
		return hs[idx].handler();
}

#endif /* LCD_PROTOTYPE_IPC_H */
