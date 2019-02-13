/*
 * boot.c - non-isolated boot module
 *
 * NOTE: This not built for SMACK. This is only used for the
 * real environment.
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
	ret = lcd_create_module_lcd(LCD_DIR("llvm_example/lcd"),
				"lcd_test_mod_llvm_example_lcd",
				&lcd, 
				&ctx, 0);
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
	lcd_to_boot_info(ctx)->cptrs[0] = dest;
	/*
	 * Run lcd
	 */
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("failed to start lcd");
		goto fail6;
	}

	/* IPC -------------------------------------------------- */

	/*
	 * Do an RPC for FUNC1
	 */
	lcd_set_r0(1);
	lcd_set_r1(2);
	lcd_set_r2(3);
	ret = lcd_sync_call(endpoint);
	if (ret) {
		LIBLCD_ERR("failed to call FUNC1");
		goto fail7;
	}
	
	if (lcd_r0() != 17) {
		LIBLCD_ERR("FUNC1 rpc failed");
		goto fail8;
	}

	/*
	 * Do an RPC for FUNC2
	 */
	lcd_set_r0(2);
	ret = lcd_sync_call(endpoint);
	if (ret) {
		LIBLCD_ERR("failed to call FUNC2");
		goto fail9;
	}
	
	if (lcd_r0() != 29) {
		LIBLCD_ERR("FUNC2 rpc failed");
		goto fail10;
	}

	/*
	 * Hang out for a two seconds for lcd to exit
	 */
	msleep(2000);

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
fail4:
	lcd_cap_delete(lcd);
	lcd_destroy_create_ctx(ctx); /* tears down LCD */
fail3:
fail2:
	lcd_exit(0); /* tears down everything else */
fail1:
	return ret;
}

static int boot_init(void)
{
	int ret;
	
	LCD_MAIN({

			ret = boot_main();

		});

	return ret;
}

static void boot_exit(void)
{
	/* nothing to do */
}

module_init(boot_init);
module_exit(boot_exit);
