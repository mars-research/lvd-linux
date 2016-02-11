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
	cptr_t dest1, dest2;
	struct lcd_info *mi1, *mi2;
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
	ret = lcd_create_module_lcd(&lcd1, 
				LCD_DIR("ipc2"),
				"lcd_test_mod_ipc2_lcd1",
				LCD_CPTR_NULL, &mi1);
	if (ret) {
		LIBLCD_ERR("failed to create lcd1");
		goto fail3;
	}
	/*
	 * Create lcd 2
	 */
	ret = lcd_create_module_lcd(&lcd2, 
				LCD_DIR("ipc2"),
				"lcd_test_mod_ipc2_lcd2",
				LCD_CPTR_NULL, &mi2);
	if (ret) {
		LIBLCD_ERR("failed to create lcd2");
		goto fail4;
	}
	/*
	 * Alloc dest slots
	 */
	ret = __lcd_alloc_cptr(mi1->cache, &dest1);
	if (ret) {
		LIBLCD_ERR("failed to alloc cptr");
		goto fail5;
	}
	ret = __lcd_alloc_cptr(mi2->cache, &dest2);
	if (ret) {
		LIBLCD_ERR("failed to alloc cptr");
		goto fail6;
	}
	/*
	 * Grant access to endpoint for both lcd's
	 */
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
	/*
	 * Set up boot info
	 */
	ret = lcd_dump_boot_info(mi1);
	if (ret) {
		LIBLCD_ERR("dump boot info 1");
		goto fail9;
	}
	ret = lcd_dump_boot_info(mi2);
	if (ret) {
		LIBLCD_ERR("dump boot info 2");
		goto fail9;
	}
	/*
	 * Store dest cptrs for endpoint
	 */
	to_boot_info(mi1)->cptrs[0] = dest1;
	to_boot_info(mi2)->cptrs[0] = dest2;
	/*
	 * Run lcd's
	 */
	ret = lcd_run(lcd1);
	if (ret) {
		LIBLCD_ERR("failed to start lcd1");
		goto fail10;
	}
	ret = lcd_run(lcd2);
	if (ret) {
		LIBLCD_ERR("failed to start lcd2");
		goto fail11;
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
fail11:
fail10:
fail9:
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
