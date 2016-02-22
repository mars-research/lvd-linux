/*
 * console.c
 *
 * Copyright: University of Utah
 */

#include <linux/printk.h>
#include <liblcd/liblcd.h>

void lcd_printk(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
	printk("\n"); /* flush */
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_printk);
