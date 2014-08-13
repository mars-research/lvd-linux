/*
 * api-internal.h - direct access to hypervisor routines
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 * Use the higher-level calls in ipc.h. In the future, 
 * these won't be directly callable.
 */

#ifndef LCD_PROTOTYPE_API_INTERNAL_H
#define LCD_PROTOTYPE_API_INTERNAL_H

#include <lcd-prototype/lcd.h>

struct lcd_handler {
	cptr_t c;
	int (*handler)(void);
};

int __lcd_send(cptr_t c);
int __lcd_call(cptr_t c);
int __lcd_select(struct lcd_handlers *hs, int hs_count);

#endif /* LCD_PROTOTYPE_API_INTERNAL_H */
