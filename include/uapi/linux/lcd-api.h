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
	LCD_CREATE_LCD = 1,
};

struct utcb *utcb(void);
capability_t lcd_api_cap(void);
capability_t lcd_api_reply_cap(void);

static inline int __lcd_api_create_sync_endpoint(capability_t cap, capability_t reply_cap, capability_t ep_cap) {

	struct message_info *msg = &utcb()->msg_info;

	msg->regs[0] = LCD_CREATE_SYNC_ENDPOINT; 
	msg->valid_regs = 1; 

	msg->cap_regs[0] = ep_cap; 
	msg->cap_regs[1] = reply_cap; 
	msg->valid_cap_regs = 2; 

	return ipc_call(cap, msg); 

};

static inline int lcd_api_create_sync_endpoint(capability_t ep_cap) {
	return __lcd_api_create_sync_endpoint(lcd_api_cap(), lcd_api_reply_cap(), ep_cap);
};

static inline int __lcd_api_create_lcd(capability_t cap, 
		capability_t reply_cap, 
		char *module_name,
		capability_t *boot_caps,
		unsigned long boot_caps_size, 
		capability_t lcd_cap) 
{
	unsigned short str_regs;
	unsigned short cap_regs;
	struct message_info *msg = &utcb()->msg_info;

	msg->regs[0] = LCD_CREATE_LCD; 
	str_regs = lcd_cap_marshal_string(&msg->regs[1], module_name, LCD_MAX_REGS - 1);
	msg->valid_regs = 1 + str_regs; 

	cap_regs = lcd_cap_marshal_cap_array(&msg->cap_regs[0], boot_caps, boot_caps_size, LCD_MAX_CAP_REGS - 2);
	msg->cap_regs[cap_regs] = lcd_cap; 
	msg->cap_regs[cap_regs + 1] = reply_cap; 
	
	msg->valid_cap_regs = cap_regs + 2; 

	return ipc_call(cap, msg); 

};

static inline int lcd_api_create_sync_endpoint(char *module_name,
		capability_t *boot_caps, unsigned long boot_caps_size,
		capability_t lcd_cap) 
{
	return __lcd_api_create_sync_endpoint(lcd_api_cap(), 
			lcd_api_reply_cap(), 
			module_name, boot_caps, 
			boot_caps_size,
			lcd_cap);
};

#endif
