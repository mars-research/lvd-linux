/**
 * boot.c - non-isolated kernel module, does setup
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
				LCD_DIR("async"),
				"lcd_test_mod_async_lcd",
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
		LIBLCD_ERR("dump boot info");
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
	 * Hang out for a few seconds for lcd to exit
	 */
	msleep(3000);
	/*
	 * Tear everything down
	 */
	ret = 0;
	goto out;

out:
fail4:
fail3:
	/* 
	 * This call is necessary because we need to tear down the module
	 * on the host (otherwise, an lcd_cap_delete would be sufficient
	 */
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
