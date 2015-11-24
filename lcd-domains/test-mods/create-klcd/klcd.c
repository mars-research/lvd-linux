/*
 * Simple kLCD test
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <lcd-domains/kliblcd.h>
#include <linux/printk.h>

static int foo(void)
{
	printk(KERN_ERR "inside the klcd");
	return 10;
}

static int __init my_init(void) 
{
	/*
	 * Unlike other examples, the kLCD is already running in "LCD" mode,
	 * so we don't do lcd_enter / lcd_exit.
	 */
	foo();

	return 0;
}

/* 
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit my_exit(void)
{
	return;
}

module_init(my_init);
module_exit(my_exit);
