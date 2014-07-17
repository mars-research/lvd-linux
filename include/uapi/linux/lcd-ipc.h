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
	u8		err; 
};

struct boot_info {
	capability_t 	boot_caps[8];
};

struct utcb {
	struct message_info msg_info;
	struct boot_info    boot_info;
};

int ipc_send(capability_t cap, struct message_info *msg);
int ipc_recv(capability_t rvp_cap, struct message_info *msg);
int ipc_call(capability_t cap, struct message_info *msg);
int ipc_reply(capability_t rvp_cap, struct message_info *msg);

#endif
