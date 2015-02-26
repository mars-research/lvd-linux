/**
 * lcd2.c - code for lcd2 in ipc test
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/lcd-domains/liblcd.h>

#include "../../../../arch/x86/lcd-domains/liblcd.c"

static int do_recv(void)
{
	int ret;
	ret = lcd_recv(__cptr(0x10d));
	if (ret)
		return ret;
	else if (lcd_r0() != 12345)
		return -1;
	else
		return 0;
}

static int __noreturn __init lcd2_init(void) 
{
	int r;
	r = lcd_enter();
	if (r)
		goto out;

	r = do_recv();

	goto out;


out:
	lcd_exit(r);
}

static void lcd2_exit(void)
{
	return;
}

module_init(lcd2_init);
module_exit(lcd2_exit);
