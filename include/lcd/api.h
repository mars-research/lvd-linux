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
#include <lcd/boot.h>
#include <lcd/sched.h>

struct lcd_api {
	bool 	     	 exit_flag;
	capability_t 	 ep_cap;
	struct sync_ipc  *ep; 
};

static inline capability_t lcd_api_cap() {
	return current->utcb->boot_info.boot_caps[LCD_BOOT_API_CAP];
};

static inline capability_t lcd_api_reply_cap() {
	return current->utcb->boot_info.boot_caps[LCD_BOOT_REPLY_CAP];
};

int lcd_api_init(struct lcd_api *api);
int lcd_api_exit(struct lcd_api *api); 
#endif
