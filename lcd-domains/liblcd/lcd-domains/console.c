/*
 * console.c
 *
 * Printk to the host kernel logs.
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <linux/kernel.h> /* for vsnprintf */

#include <lcd_config/post_hook.h>

static lcd_sys_put_char(char c)
{
	
	lcd_set_debug_reg(c);
	return LCD_DO_SYSCALL(LCD_SYSCALL_PUTCHAR);
}

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
		lcd_put_char(*p);
	/*
	 * Write null char
	 */
	lcd_put_char(0);
}
