/**
 * lcd.c - code for lcd in liblcd test
 */

#include <lcd-domains/liblcd-config.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <lcd-domains/liblcd.h>

#include <lcd-domains/liblcd-hacks.h>

static int __noreturn __init liblcd_test_lcd_init(void) 
{
	int r = 0;
	r = lcd_enter();
	if (r)
		goto out;

	r = lcd_run_tests();

	goto out;

out:
	lcd_exit(r);
}

static void liblcd_test_lcd_exit(void)
{
	return;
}

module_init(liblcd_test_lcd_init);
module_exit(liblcd_test_lcd_exit);
