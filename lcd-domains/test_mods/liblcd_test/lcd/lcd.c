/*
 * lcd.c - code for isolated LCD in liblcd test
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>

#include <lcd_config/post_hook.h>

static int __noreturn __init liblcd_test_lcd_init(void) 
{
	int r = 0;
	r = lcd_enter();

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
