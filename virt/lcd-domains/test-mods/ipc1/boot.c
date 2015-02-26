/**
 * boot.c - non-isolated kernel module, does setup and
 *          send/recv
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
	cptr_t lcd;
	struct lcd_module_info *mi;
	/*
	 * Enter lcd mode
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
	ret = lcd_create_module_lcd(&lcd, "lcd_test_mod_ipc1_lcd",
				LCD_CPTR_NULL, &mi);
	if (ret) {
		LIBLCD_ERR("failed to create lcd");
		goto fail3;
	}
	/*
	 * Grant access to endpoint
	 */
	ret = lcd_cap_grant(lcd, endpoint, __cptr(0x10d));
	if (ret) {
		LIBLCD_ERR("failed to grant endpoint to lcd");
		goto fail4;
	}
	/*
	 * Run lcd
	 */
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("failed to start lcd");
		goto fail5;
	}
	/*
	 * Do a send, followed by a receive
	 */
	lcd_set_r0(1234);
	ret = lcd_send(endpoint);
	if (ret) {
		LIBLCD_ERR("failed to do first send");
		goto fail6;
	}
	ret = lcd_recv(endpoint);
	if (ret) {
		LIBLCD_ERR("recv failed");
		goto fail7;
	}
	/*
	 * Check value in message reg r0
	 */
	if (lcd_r0() != 5678) {
		LIBLCD_ERR("unexpected value 0x%lx in reg r0",
			lcd_r0());
		goto fail8;
	}
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
	/* 
	 * No need to "ungrant" - everything is taken care of during tear
	 * down
	 */
fail4:
	/* 
	 * This call is necessary because we need to tear down the module
	 * on the host (otherwise, an lcd_cap_delete would be sufficient
	 */
	lcd_destroy_module_lcd(lcd, mi, LCD_CPTR_NULL);
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
