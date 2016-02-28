/*
 * console.c
 *
 * Printk to the host kernel logs.
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <linux/kernel.h> /* for vsnprintf */
#include <asm/lcd_domains/liblcd.h>

#include <lcd_config/post_hook.h>

void _lcd_printk(const char *fmt, va_list args)
{
	unsigned char buf[512]; /* this is probably a bit big ... */
	unsigned char *p;
	/*
	 * Convert fmt string to chars
	 */
	vsnprintf(buf, 512, fmt, args);
	/*
	 * Write char by char
	 */
	for (p = buf; *p; p++)
		lcd_syscall_putchar(*p);
	/*
	 * Write null char
	 */
	lcd_syscall_putchar(0);
}

void lcd_printk(const char *fmt, ...)
{
	va_list args;
	/*
	 * Convert fmt string to chars
	 */
	va_start(args, fmt);
	_lcd_printk(fmt, args);
	va_end(args);
}
