#ifndef LCD_DOMAINS_LCD_DOMAINS_H
#define LCD_DOMAINS_LCD_DOMAINS_H

#include <asm/lcd-domains-arch.h>
#include <lcd-domains/utcb.h>

struct lcd {
	/*
	 * Arch-dependent state of lcd
	 */
	struct lcd_arch *lcd_arch;
	/*
	 * User thread control block. This is the data accessible
	 * inside the lcd (ipc registers, ...). Points to memory allocated by
	 * arch-dep code (inside struct lcd_arch).
	 */
	struct lcd_utcb *utcb;
};

#endif /* LCD_DOMAINS_LCD_DOMAINS_H */
