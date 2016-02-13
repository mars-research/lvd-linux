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

void lcd_printk(char *fmt, ...)
{
	va_list args;
	char buf[512]; /* this is probably a bit big ... */
	char *p;
	/*
	 * Convert fmt string to chars
	 */
	va_start(args, fmt);
	vsnprintf(buf, 512, fmt, args);
	va_end(args);
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
