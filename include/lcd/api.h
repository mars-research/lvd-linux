/* 
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 */
#ifndef _LCD_API_H_
#define _LCD_API_H_

#include <uapi/linux/lcd-api.h>
#include <lcd/cap.h>
#include <lcd/ipc.h>
#include <lcd/cap-cache.h>

struct lcd_api {
	bool 	     	 exit_flag;
	capability_t 	 ep_cap;
	struct sync_ipc  *ep; 
};


int lcd_api_init(struct lcd_api *api);
int lcd_api_exit(struct lcd_api *api); 
#endif
