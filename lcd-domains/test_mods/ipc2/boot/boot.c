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
	cptr_t endpoint;
	cptr_t lcd1, lcd2;
	cptr_t dest1, dest2;
	struct lcd_create_ctx *ctx1, *ctx2;
	/*
	 * Enter LCD mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter failed");
		goto fail1;
	}
	/*
	 * Create an endpoint
	 */
	ret = lcd_create_sync_endpoint(&endpoint);
	if (ret) {
		LIBLCD_ERR("failed to create endpoint");
		goto fail2;
	}
	/*
	 * Create lcds
	 */
	ret = lcd_create_module_lcd(LCD_DIR("ipc12/lcd1"),
				"lcd_test_mod_ipc2_lcd1",
				&lcd1, 
				&ctx1);
	if (ret) {
		LIBLCD_ERR("failed to create lcd1");
		goto fail3;
	}
	ret = lcd_create_module_lcd(LCD_DIR("ipc12/lcd2"),
				"lcd_test_mod_ipc2_lcd2",
				&lcd2, 
				&ctx2);
	if (ret) {
		LIBLCD_ERR("failed to create lcd2");
		goto fail4;
	}
	/*
	 * --------------------------------------------------
	 * Grant cap to endpoint
	 *
	 * Allocate cptrs
	 */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx1), &dest1);
	if (ret) {
		LIBLCD_ERR("failed to alloc dest slot");
		goto fail5;
	}
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx2), &dest2);
	if (ret) {
		LIBLCD_ERR("failed to alloc dest slot");
		goto fail6;
	}
	ret = lcd_cap_grant(lcd1, endpoint, dest1);
	if (ret) {
		LIBLCD_ERR("failed to grant endpoint to lcd1");
		goto fail7;
	}
	ret = lcd_cap_grant(lcd2, endpoint, dest2);
	if (ret) {
		LIBLCD_ERR("failed to grant endpoint to lcd2");
		goto fail8;
	}

	lcd_to_boot_info(ctx1)->cptrs[0] = dest1;
	lcd_to_boot_info(ctx2)->cptrs[0] = dest2;
	/*
	 * --------------------------------------------------
	 * Run lcd's
	 *
	 */
	ret = lcd_run(lcd1);
	if (ret) {
		LIBLCD_ERR("failed to start lcd1");
		goto fail9;
	}
	ret = lcd_run(lcd2);
	if (ret) {
		LIBLCD_ERR("failed to start lcd2");
		goto fail10;
	}
	/*
	 * Hang out for a few seconds
	 */
	msleep(3000);
	/*
	 * Tear everything down
	 */
	ret = 0;
	goto out;

	/*
	 * Everything torn down / freed during destroy / exit.
	 */
out:
fail10:
fail9:
fail8:
fail7:
fail6:
fail5:
	lcd_cap_delete(lcd2);
	lcd_destroy_create_ctx(ctx2); /* tears down LCD 2 */
fail4:
	lcd_cap_delete(lcd1);
	lcd_destroy_create_ctx(ctx1); /* tears down LCD 1 */
fail3:
fail2:
	lcd_exit(0); /* tears down everything else */
fail1:
	return ret;
}

static void boot_exit(void)
{
	/* nothing to do */
}

module_init(boot_main);
module_exit(boot_exit);
