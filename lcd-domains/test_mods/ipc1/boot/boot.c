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
	cptr_t endpoint;
	cptr_t dest;
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
	 * Create lcd
	 */
	ret = lcd_create_module_lcd(LCD_DIR("liblcd_test/lcd"),
				"lcd_test_mod_liblcd_test_lcd",
				&lcd, 
				&ctx);
	if (ret) {
		LIBLCD_ERR("failed to create lcd");
		goto fail3;
	}
	/*
	 * Allocate a cptr for the lcd to hold the endpoint
	 */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx), &dest);
	if (ret) {
		LIBLCD_ERR("failed to alloc dest slot");
		goto fail4;
	}
	/*
	 * Grant access to endpoint
	 */
	ret = lcd_cap_grant(lcd, endpoint, dest);
	if (ret) {
		LIBLCD_ERR("failed to grant endpoint to lcd");
		goto fail5;
	}
	/*
	 * Store cptr in boot area
	 */
	lcd_to_boot_info(ctx)->ctprs[0] = dest;
	/*
	 * Run lcd
	 */
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("failed to start lcd");
		goto fail6;
	}
	/*
	 * Do a send, followed by a receive
	 */
	lcd_set_r0(1234);
	ret = lcd_send(endpoint);
	if (ret) {
		LIBLCD_ERR("failed to do first send");
		goto fail7;
	}
	ret = lcd_recv(endpoint);
	if (ret) {
		LIBLCD_ERR("recv failed");
		goto fail8;
	}
	/*
	 * Check value in message reg r0
	 */
	if (lcd_r0() != 5678) {
		LIBLCD_ERR("unexpected value 0x%lx in reg r0",
			lcd_r0());
		goto fail9;
	}
	/*
	 * Hang out for a second
	 */
	msleep(1000);
	/*
	 * Tear everything down
	 */
	ret = 0;
	goto out;

	/*
	 * Everything torn down / freed during destroy / exit.
	 */
out:
fail9:
fail8:
fail7:
fail6:
fail5:
fail4:
	lcd_cap_delete(lcd);
	lcd_destroy_create_ctx(ctx); /* tears down LCD */
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
