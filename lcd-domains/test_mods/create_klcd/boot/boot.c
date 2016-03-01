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
	cptr_t klcd;
	/*
	 * Enter LCD mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter failed");
		goto fail1;
	}
	/*
	 * Create klcd
	 */
	ret = lcd_create_module_klcd(LCD_DIR("create_klcd/lcd"),
				"lcd_test_mod_create_klcd_klcd",
				&klcd);
	if (ret) {
		LIBLCD_ERR("failed to create lcd");
		goto fail2;
	}
	/*
	 * Run klcd
	 */
	ret = lcd_run(klcd);
	if (ret) {
		LIBLCD_ERR("failed to start lcd");
		goto fail3;
	}
	/*
	 * Hang out for a couple seconds
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
fail3:
	lcd_destroy_module_klcd(klcd, "lcd_test_mod_create_klcd_klcd");
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
