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
	cptr_t klcd;

	/*
	 * Enter lcd mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("enter");
		goto fail1;
	}
	/*
	 * Create a new klcd
	 */
	ret = klcd_create_module_klcd(&klcd,
				LCD_DIR("create_klcd"),
				"lcd_test_mod_create_klcd_klcd");
	if (ret) {
		LIBLCD_ERR("create module klcd");
		goto fail2;
	}
	/*
	 * Run it
	 */
	ret = lcd_run(klcd);
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
	klcd_destroy_module_klcd(klcd, "lcd_test_mod_create_klcd_klcd");
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
