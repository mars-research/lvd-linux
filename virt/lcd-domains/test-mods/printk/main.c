/*
 * Simple test for lcd_printk.
 *
 * IMPORTANT: This code *does not* expect to be
 * booted to use kmalloc, page alloc, etc., so if you
 * try to add kmalloc in here, make sure whoever boots
 * this code sets up things properly!
 */

#include <lcd-domains/liblcd-config.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <lcd-domains/liblcd.h>

#include <lcd-domains/liblcd-hacks.h>

static int __noreturn __init test_init(void) 
{
	int r = 0;
	r = lcd_enter();
	if (r)
		goto out;

	lcd_printk("message 1");
	lcd_printk("message 2 with int %d", 5);
	lcd_printk("message 3 with hex 0x%x", 255);
	lcd_printk("message 4 with str %s", "hello");

out:
	lcd_exit(r);
}

static void __exit test_exit(void)
{
	return;
}

module_init(test_init);
module_exit(test_exit);
