/**
 * lcd1.c - code for lcd1 in ipc test
 */

#include <lcd-domains/liblcd-config.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <lcd-domains/liblcd.h>

#include <lcd-domains/liblcd-hacks.h>

cptr_t ep;

static int do_send(void)
{
	lcd_set_r0(12345);
	return lcd_send(ep);
}

static void get_endpoint(void)
{
	ep = lcd_get_boot_info()->cptrs[0];
}

static int __noreturn __init lcd1_init(void) 
{
	int r;
	r = lcd_enter();
	if (r)
		goto out;

	get_endpoint();

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
