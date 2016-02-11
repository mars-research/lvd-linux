/**
 * boot.c - non-isolated kernel module
 *
 */

#include <lcd-domains/kliblcd.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>


static int boot_main(void)
{
	int ret;
	cptr_t lcd;
	struct lcd_info *mi;
	/*
	 * Enter lcd mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter failed");
		goto fail1;
	}
	/*
	 * Create lcd
	 */
	ret = lcd_create_module_lcd(&lcd, 
				LCD_DIR("liblcd_tests"),
				"lcd_test_mod_liblcd_tests_lcd",
				LCD_CPTR_NULL, &mi);
	if (ret) {
		LIBLCD_ERR("failed to create lcd");
		goto fail2;
	}
	/*
	 * Set up boot info
	 */
	ret = lcd_dump_boot_info(mi);
	if (ret) {
		LIBLCD_ERR("failed to set up boot params");
		goto fail3;
	}
	/*
	 * Run lcd
	 */
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("failed to start lcd");
		goto fail4;
	}
	/*
	 * Hang out for four seconds
	 */
	msleep(4000);
	/*
	 * Tear everything down
	 */
	ret = 0;
	goto out;

	/*
	 * Everything torn down / freed during destroy / exit.
	 */
out:
fail4:
fail3:
	lcd_destroy_module_lcd(lcd, mi, LCD_CPTR_NULL);
fail2:
	lcd_exit(0);
fail1:
	return ret;
}

static void boot_exit(void)
{
	/* nothing to do */
}

module_init(boot_main);
module_exit(boot_exit);
