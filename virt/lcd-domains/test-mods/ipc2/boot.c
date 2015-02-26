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
	cptr_t endpoint;
	cptr_t lcd1, lcd2;
	struct lcd_module_info *mi1, *mi2;
	/*
	 * Enter lcd mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("klcd enter failed");
		goto fail1;
	}
	/*
	 * Create an endpoint for two lcd's
	 */
	ret = lcd_create_sync_endpoint(&endpoint);
	if (ret) {
		LIBLCD_ERR("failed to create endpoint");
		goto fail2;
	}
	/*
	 * Create lcd 1
	 */
	ret = lcd_create_module_lcd(&lcd1, "lcd_test_mod_ipc2_lcd1",
				LCD_CPTR_NULL, &mi1);
	if (ret) {
		LIBLCD_ERR("failed to create lcd1");
		goto fail3;
	}
	/*
	 * Create lcd 2
	 */
	ret = lcd_create_module_lcd(&lcd2, "lcd_test_mod_ipc2_lcd2",
				LCD_CPTR_NULL, &mi2);
	if (ret) {
		LIBLCD_ERR("failed to create lcd2");
		goto fail4;
	}
	/*
	 * Grant access to endpoint for both lcd's
	 */
	ret = lcd_cap_grant(lcd1, endpoint, __cptr(0x10d));
	if (ret) {
		LIBLCD_ERR("failed to grant endpoint to lcd1");
		goto fail5;
	}
	ret = lcd_cap_grant(lcd2, endpoint, __cptr(0x10d));
	if (ret) {
		LIBLCD_ERR("failed to grant endpoint to lcd2");
		goto fail6;
	}
	/*
	 * Run lcd's
	 */
	ret = lcd_run(lcd1);
	if (ret) {
		LIBLCD_ERR("failed to start lcd1");
		goto fail7;
	}
	ret = lcd_run(lcd2);
	if (ret) {
		LIBLCD_ERR("failed to start lcd2");
		goto fail8;
	}
	/*
	 * Wait for 2 seconds
	 */
	msleep(2000);
	/*
	 * Tear everything down
	 */
	ret = 0;
	goto out;

out:
fail8:
fail7:
fail6:
fail5:
	lcd_destroy_module_lcd(lcd2, mi2, LCD_CPTR_NULL);
fail4:
	lcd_destroy_module_lcd(lcd1, mi1, LCD_CPTR_NULL);
fail3:
fail2:
	lcd_exit(0); /* will free endpoint */
fail1:
	return ret;
}

static void boot_exit(void)
{
	/* nothing to do */
}

module_init(boot_main);
module_exit(boot_exit);
