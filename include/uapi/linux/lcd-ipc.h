/*
 * lcd.h - interface to LCD domains
 *
 * Author: Anton Burtsev 
 * Copyright: University of Utah
 */

#ifndef __LINUX_PUBLIC_LCD_IPC_H__
#define __LINUX_PUBLIC_LCD_IPC_H__

#include <linux/lcd-cap.h>

struct message_info {
	u64 		regs[8];
	capability_t 	cap_regs[8];
	u8		valid_regs;
	u8		valid_cap_regs;
};

struct boot_info {
	capability_t 	boot_rvp; 
};

struct utcb {
	struct message_info msg_info;
	struct boot_info    boot_info;
};

#endif
