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
	struct lcd_info *mi;
	cptr_t lcd;

	/*
	 * Enter lcd mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("enter");
		goto fail1;
	}
	/*
	 * Create a new lcd
	 */
	ret = lcd_create_module_lcd(&lcd,
				LCD_DIR("load"),
				"lcd_test_mod_load_lcd",
				LCD_CPTR_NULL, &mi);
	if (ret) {
		LIBLCD_ERR("create module lcd");
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
	 * Run it
	 */
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("run lcd");
		goto fail4;
	}
	/*
	 * Wait for 4 seconds
	 */
	msleep(4000);
	/*
	 * Tear everything down
	 */
	ret = 0;
	goto out;

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
