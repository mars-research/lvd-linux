/**
 * main.c - non-isolated kernel module, does setup
 *
 */

#include <lcd-domains/kliblcd.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int boot_main(void)
{
	int ret;
	struct lcd_info *mi;
	cptr_t lcd;
	cptr_t vfs;
	cptr_t endpoint;
	cptr_t dest1, dest2;

	/*
	 * Enter lcd mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("enter");
		goto fail1;
	}
	/*
	 * Create VFS channel
	 */
	ret = lcd_create_sync_endpoint(&endpoint);
	if (ret) {
		LIBLCD_ERR("create endpoint");
		goto fail2;
	}

	/* CREATE LCDS -------------------------------------------------- */

	/*
	 * Create vfs klcd
	 */
	ret = klcd_create_module_klcd(&vfs, "lcd_test_mod_pmfs_vfs");
	if (ret) {
		LIBLCD_ERR("create vfs klcd");
		goto fail2;
	}
	/*
	 * Create pmfs lcd
	 */
	ret = lcd_create_module_lcd(&lcd, "lcd_test_mod_pmfs_lcd",
				LCD_CPTR_NULL, &mi);
	if (ret) {
		LIBLCD_ERR("create module lcd");
		goto fail3;
	}

	/* GRANT ENDPOINT TO PMFS ------------------------------ */

	/*
	 * Alloc dest slot
	 */
	ret = __lcd_alloc_cptr(mi->cache, &dest1);
	if (ret) {
		LIBLCD_ERR("failed to alloc cptr");
		goto fail4;
	}
	/*
	 * Grant
	 */
	ret = lcd_cap_grant(lcd, endpoint, dest1);
	if (ret) {
		LIBLCD_ERR("failed to grant endpoint to lcd1");
		goto fail5;
	}


	/* GRANT ENDPOINT TO VFS ------------------------------ */

	dest2 = __cptr(3);
	ret = lcd_cap_grant(vfs, endpoint, dest2);
	if (ret) {
		LIBLCD_ERR("failed to grant endpoint to vfs");
		goto fail6;
	}

	/* DUMP BOOT INFO FOR PMFS ------------------------------ */

	/*
	 * Set up boot info
	 */
	ret = lcd_dump_boot_info(mi);
	if (ret) {
		LIBLCD_ERR("dump boot info");
		goto fail7;
	}
	to_boot_info(mi)->cptrs[0] = dest1;

	/* RUN -------------------------------------------------- */

	/*
	 * Run vfs
	 */
	ret = lcd_run(vfs);
	if (ret) {
		LIBLCD_ERR("run vfs");
		goto fail8;
	}
	/*
	 * Run pmfs
	 */
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("run pmfs");
		goto fail9;
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
fail9:
fail8:
fail7:
fail6:
fail5:
fail4:
	lcd_destroy_module_lcd(lcd, mi, LCD_CPTR_NULL);
fail3:
	klcd_destroy_module_klcd(vfs, "lcd_test_mod_pmfs_vfs");
fail2:
	/* frees endpoint */
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
