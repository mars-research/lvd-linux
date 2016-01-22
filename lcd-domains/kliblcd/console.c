/*
 * console.c
 *
 * Copyright: University of Utah
 */

#include <linux/printk.h>
#include <lcd-domains/liblcd.h>

void lcd_printk(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
