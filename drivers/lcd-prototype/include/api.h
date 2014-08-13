/*
 * api.h - wrappers for calls into api code, via ipc
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 * Implemented by liblcd. Sets up and performs calls
 * into api code, via ipc. See api code for more info.
 */

#ifndef LCD_PROTOTYPE_API_H
#define LCD_PROTOTYPE_API_H

#include <lcd-prototype/lcd.h>

int lcd_mk_sync_endpoint(cptr_t c);
int lcd_rm_sync_endpoint(cptr_t c);

#endif /* LCD_PROTOTYPE_API_H */
