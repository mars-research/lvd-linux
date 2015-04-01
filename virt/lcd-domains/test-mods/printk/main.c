#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/lcd-domains/liblcd.h>

#include "../../../../arch/x86/lcd-domains/liblcd.c"

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
