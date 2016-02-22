/*
 * boot.c - non-isolated boot module
 */

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>

static int boot_main(void)
{
	int ret;
	cptr_t lcd;
	struct lcd_create_ctx *ctx;
	/*
	 * Enter LCD mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter failed");
		goto fail1;
	}
	/*
	 * Create lcd
	 */
	ret = lcd_create_module_lcd(LCD_DIR("liblcd_test/lcd"),
				"lcd_test_mod_liblcd_test_lcd",
				&lcd, 
				&ctx);
	if (ret) {
		LIBLCD_ERR("failed to create lcd");
		goto fail2;
	}
	/*
	 * Run lcd
	 */
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("failed to start lcd");
		goto fail3;
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
fail3:
	lcd_cap_delete(lcd);
	lcd_destroy_create_ctx(ctx);
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
