/*
 * lcd.c - code for isolated LCD in liblcd test
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <liblcd/liblcd.h>

#include <lcd_config/post_hook.h>

cptr_t ep;

static int do_recv(void)
{
	int ret;
	ret = lcd_sync_recv(ep);
	if (ret)
		return ret;
	else if (lcd_r0() != 12345)
		return -1;
	else
		return 0;
}

static void get_endpoint(void)
{
	ep = lcd_get_boot_info()->cptrs[0];
}

static int __noreturn __init lcd2_init(void) 
{
	int r;
	r = lcd_enter();
	if (r)
		goto out;

	get_endpoint();

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
