/*
 * console.c -- Primitive putchar for LCDs
 *
 * Copyright: University of Utah
 */

#include <linux/printk.h>
#include "internal.h"

int __lcd_put_char(struct lcd *lcd, char c)
{
	/*
	 * Put char in lcd's console buff
	 */
	lcd->console_buff[lcd->console_cursor] = c;
	/*
	 * Bump cursor and check
	 */
	lcd->console_cursor++;
	if (c == 0) {
		/*
		 * End of string; printk to host and reset buff
		 */
		printk(KERN_INFO "message from lcd %p: %s\n",
			lcd, lcd->console_buff);
		lcd->console_cursor = 0;
		return 0;
	}
	if (lcd->console_cursor >= LCD_CONSOLE_BUFF_SIZE - 1) {
		/*
		 * Filled buffer; empty it.
		 */
		lcd->console_buff[LCD_CONSOLE_BUFF_SIZE - 1] = 0;
		printk(KERN_INFO "(incomplete) message from lcd %p: %s\n",
			lcd, lcd->console_buff);
		lcd->console_cursor = 0;
		return 0;
	}
	return 0;
}

/* INIT/EXIT ------------------------------------------------------------ */

int __lcd_console_init(void)
{
	/*
	 * Nothing for now
	 */
	return 0;
}

void __lcd_console_exit(void)
{
	/*
	 * Nothing for now
	 */
	return;
}
