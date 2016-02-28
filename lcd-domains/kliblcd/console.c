/*
 * console.c
 *
 * Copyright: University of Utah
 */

#include <linux/printk.h>
#include <liblcd/liblcd.h>

void _lcd_printk(const char *fmt, va_list args)
{
	printk(KERN_ERR "message from klcd %p: ", current->lcd);
	vprintk(fmt, args);
	printk("\n"); /* flush */
}

void lcd_printk(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	_lcd_printk(fmt, args);
	va_end(args);
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_printk);
EXPORT_SYMBOL(_lcd_printk);
