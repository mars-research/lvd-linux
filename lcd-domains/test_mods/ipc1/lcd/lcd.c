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

static int do_send(u64 val)
{
	lcd_set_r0(val);
	return lcd_send(ep);
}

static int do_recv(void)
{
	int ret;
	ret = lcd_recv(ep);
	if (ret)
		return ret;
	if (lcd_r0() != 1234)
		return 1234;
	return 0;
}

static void get_endpoint(void)
{
	ep = lcd_get_boot_info()->cptrs[0];
}

static int __noreturn __init ipc1_lcd_init(void) 
{
	int ret = 0;
	ret = lcd_enter();

	get_endpoint();

	r = do_recv();

	r = do_send(5678);
	
	goto out;

out:
	lcd_exit(ret);
}

static void ipc1_lcd_exit(void)
{
	return;
}

module_init(ipc1_lcd_init);
module_exit(ipc1_lcd_exit);
