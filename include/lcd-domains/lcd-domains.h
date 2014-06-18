#ifndef LCD_DOMAINS_LCD_DOMAINS_H
#define LCD_DOMAINS_LCD_DOMAINS_H

#include <asm/lcd-domains-arch.h>
#include <lcd-domains/ipc.h>

struct lcd {
	/*
	 * Arch-dependent state of lcd
	 */
	struct lcd_arch *lcd_arch;
	/*
	 * IPC registers. Points to memory allocated by
	 * arch-dep code for IPC registers (inside struct lcd_arch).
	 */
	struct lcd_ipc_regs *ipc_regs;
};

#endif /* LCD_DOMAINS_LCD_DOMAINS_H */
