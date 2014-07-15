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
	struct cap_cache cap_cache;  
	int 	     	 api_thread_exit;
	capability_t 	 rvp;
};


int lcd_api_init(struct lcd_api *api);
int lcd_api_exit(struct lcd_api *api); 
#endif
