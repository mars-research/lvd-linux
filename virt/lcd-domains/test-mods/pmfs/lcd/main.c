/*
 * main.c - runs when pmfs lcd boots
 */

#include <lcd-domains/liblcd-config.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <lcd-domains/liblcd.h>

#include <lcd-domains/liblcd-hacks.h>

int init_pmfs_fs(void);
void exit_pmfs_fs(void);

static int __noreturn __init pmfs_lcd_init(void) 
{
	int r;
	r = lcd_enter();
	if (r)
		goto fail1;

	r = init_pmfs_fs();
	if (r)
		goto fail1;

	exit_pmfs_fs();


fail1:
	lcd_exit(r);
}

static void __exit pmfs_lcd_exit(void)
{
	return;
}

module_init(pmfs_lcd_init);
module_exit(pmfs_lcd_exit);
