/**
 * boot.c - non-isolated kernel module, does setup
 *
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <lcd_config/post_hook.h>

static int boot_main(void)
{
	int ret;
	struct lcd_create_ctx *ctx;
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
	ret = lcd_create_module_lcd(LCD_DIR("bug/lcd"),
				"lcd_test_mod_bug_lcd",
				&lcd,
				&ctx, 0);
	if (ret) {
		LIBLCD_ERR("create module lcd");
		goto fail2;
	}
	/*
	 * Run it
	 */
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("run lcd");
		goto fail3;
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
fail3:
	lcd_cap_delete(lcd);
	lcd_destroy_create_ctx(ctx);
fail2:
	lcd_exit(0);
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
