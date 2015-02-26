/**
 * lcd1.c - code for lcd1 in ipc test
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/lcd-domains/liblcd.h>

#include "../../../../arch/x86/lcd-domains/liblcd.c"

static int do_send(void)
{
	lcd_set_r0(12345);
	return lcd_send(__cptr(0x10d));
}

static int __noreturn __init lcd1_init(void) 
{
	int r;
	r = lcd_enter();
	if (r)
		goto out;

	r = do_send();

	goto out;


out:
	lcd_exit(r);
}

static void lcd1_exit(void)
{
	return;
}

module_init(lcd1_init);
module_exit(lcd1_exit);
