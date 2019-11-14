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
#include <linux/mutex.h>

#include <lcd_config/post_hook.h>

static DEFINE_SPINLOCK(print_lock);

void _lcd_printk(const char *fmt, va_list args)
{
	unsigned char buf[512]; /* this is probably a bit big ... */
	unsigned char *p;
	unsigned long flags;

	/*
	 * FIXME: Ideally one should implement ring buffers. But I wanted to
	 * quickly spin a version to have ungarbled printks for debugging.
	 */
	spin_lock_irqsave(&print_lock, flags);
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

	spin_unlock_irqrestore(&print_lock, flags);
}

void
LIBLCD_FUNC_ATTR
lcd_printk(const char *fmt, ...)
{
	va_list args;
	/*
	 * Convert fmt string to chars
	 */
	va_start(args, fmt);
	_lcd_printk(fmt, args);
	va_end(args);
}
