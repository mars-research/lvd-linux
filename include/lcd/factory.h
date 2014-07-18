/*
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 *
 */

#ifndef _LCD_FACTORY_H_
#define _LCD_FACTORY_H_

#include <lcd/cap.h>
#include <lcd/ipc.h>

int lcd_prepare_introduction(capability_t api_cap, capabiity_t reply_cap, sync_ipc *rvp, capability_t *rvp_cap);
int lcd_accept_introduction(struct sync_ipc *rvp, capability_t *rvp_cap);

#endif 
