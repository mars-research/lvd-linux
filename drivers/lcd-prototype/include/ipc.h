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
#include "../api/defs.h"

struct lcd_handler {
	cptr_t c;
	int (*handler)(void);
};

static inline int lcd_send(cptr_t c)
{
	return __lcd_send(current_lcd(), c);
}
static inline int lcd_recv(cptr_t c)
{
	return __lcd_recv(current_lcd(), c);
}
static inline int lcd_reply(void)
{
	return lcd_send(lcd_reply_cap());
}
static inline int lcd_call(cptr_t c)
{
	return __lcd_call(current_lcd(), c);
}
static inline int lcd_mk_sync_endpoint(cptr_t *c)
{
	int ret;

	ret = lcd_cnode_alloc(current_lcd()->cspace, c);
	if (ret)
		return ret;

	return __lcd_mk_sync_endpoint(current_lcd(), *c);
}

static inline int lcd_rm_sync_endpoint(cptr_t c)
{
	return __lcd_rm_sync_endpoint(current_lcd(), c);
}

#if 0
static inline int lcd_select(struct lcd_handlers *hs, int hs_count)
{
	return -ENOSYS;
}
#endif

#endif /* LCD_PROTOTYPE_IPC_H */
