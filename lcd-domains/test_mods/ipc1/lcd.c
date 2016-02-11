/**
 * lcd.c - code for lcd in ipc test
 */

#include <lcd-domains/liblcd-config.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <lcd-domains/liblcd.h>

#include <lcd-domains/liblcd-hacks.h>

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

static int __noreturn __init ipc_lcd_init(void) 
{
	int r;
	r = lcd_enter();
	if (r)
		goto out;
	
	get_endpoint();

	r = do_recv();

	r = do_send(5678);

	goto out;

out:
	lcd_exit(r);
}

static void ipc_lcd_exit(void)
{
	return;
}

module_init(ipc_lcd_init);
module_exit(ipc_lcd_exit);
