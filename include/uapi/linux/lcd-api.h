/* 
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 */
#ifndef _LINUX_PUBLIC_LCD_API_H_
#define _LINUX_PUBLIC_LCD_API_H_

#include <linux/lcd-cap.h>
#include <linux/lcd-ipc.h>

enum lcd_api_calls {
	LCD_CREATE_SYNC_ENDPOINT = 1,
};

struct utcb *utcb(void);

static inline int lcd_api_create_sync_endpoint(capability_t cap, capability_t reply_cap, capability_t ep_cap) {

	struct message_info *msg = &utcb()->msg_info;

	msg->regs[0] = LCD_CREATE_SYNC_ENDPOINT; 
	msg->valid_regs = 1; 

	msg->cap_regs[0] = ep_cap; 
	msg->cap_regs[1] = reply_cap; 
	msg->valid_cap_regs = 2; 

	return ipc_call(cap, msg); 

};

#endif
