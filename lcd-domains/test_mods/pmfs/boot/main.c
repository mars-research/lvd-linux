/**
 * main.c - non-isolated kernel module, does setup
 *
 */

#include <lcd_config/pre_hook.h>

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/dispatch_loop.h>

#include <lcd_config/post_hook.h>

static int boot_main(void)
{
	int ret;
	struct lcd_create_ctx *ctx;
	cptr_t lcd;
	cptr_t vfs;
	cptr_t vfs_chnl, bdi_chnl;
	cptr_t pmfs_dest1, pmfs_dest2;
	cptr_t vfs_dest1, vfs_dest2;
	/*
	 * Enter lcd mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("enter");
		goto out;
	}
	/*
	 * Create VFS channel
	 */
	ret = lcd_create_sync_endpoint(&vfs_chnl);
	if (ret) {
		LIBLCD_ERR("create vfs endpoint");
		goto lcd_exit;
	}
	/*
	 * Create bdi channel
	 */
	ret = lcd_create_sync_endpoint(&bdi_chnl);
	if (ret) {
		LIBLCD_ERR("create bdi endpoint");
		goto lcd_exit;
	}
	
	/* CREATE LCDS -------------------------------------------------- */

	/*
	 * Create vfs klcd
	 */
	ret = lcd_create_module_klcd(LCD_DIR("pmfs/vfs_klcd"),
				"lcd_test_mod_pmfs_vfs",
				&vfs);
	if (ret) {
		LIBLCD_ERR("create vfs klcd");
		goto lcd_exit;
	}
	/*
	 * Create pmfs lcd
	 */
	ret = lcd_create_module_lcd(LCD_DIR("pmfs/pmfs_lcd"),
				"lcd_test_mod_pmfs_lcd",
				&lcd,
				&ctx);
	if (ret) {
		LIBLCD_ERR("create module lcd");
		goto destroy_vfs;
	}

	/* GRANT ENDPOINTS TO PMFS ------------------------------ */

	/*
	 * Alloc dest slot
	 */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx), &pmfs_dest1);
	if (ret) {
		LIBLCD_ERR("failed to alloc cptr");
		goto destroy_both;
	}
	/*
	 * Grant
	 */
	ret = lcd_cap_grant(lcd, vfs_chnl, pmfs_dest1);
	if (ret) {
		LIBLCD_ERR("failed to grant vfs endpoint to pmfs");
		goto destroy_both;
	}
	/*
	 * Alloc dest slot
	 */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx), &pmfs_dest2);
	if (ret) {
		LIBLCD_ERR("failed to alloc cptr");
		goto destroy_both;
	}
	/*
	 * Grant
	 */
	ret = lcd_cap_grant(lcd, bdi_chnl, pmfs_dest2);
	if (ret) {
		LIBLCD_ERR("failed to grant bdi endpoint to pmfs");
		goto destroy_both;
	}

	/* GRANT ENDPOINTS TO VFS ------------------------------ */

	/* Hack for now */

	vfs_dest1 = __cptr(3);
	ret = lcd_cap_grant(vfs, vfs_chnl, vfs_dest1);
	if (ret) {
		LIBLCD_ERR("failed to grant vfs endpoint to vfs");
		goto destroy_both;
	}
	vfs_dest2 = __cptr((1UL << 8)); /* slot = 0, fanout = 0, level = 1 */
	ret = lcd_cap_grant(vfs, bdi_chnl, vfs_dest2);
	if (ret) {
		LIBLCD_ERR("failed to grant bdi endpoint to vfs");
		goto destroy_both;
	}

	/* DUMP BOOT INFO FOR PMFS ------------------------------ */

	/*
	 * Set up boot info for pmfs lcd
	 */
	lcd_to_boot_info(ctx)->cptrs[0] = pmfs_dest1;
	lcd_to_boot_info(ctx)->cptrs[1] = pmfs_dest2;

	/* RUN -------------------------------------------------- */

	/*
	 * Run vfs
	 */
	ret = lcd_run(vfs);
	if (ret) {
		LIBLCD_ERR("run vfs");
		goto destroy_both;
	}
	/*
	 * Run pmfs
	 */
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("run pmfs");
		goto destroy_both;
	}
	/*
	 * Wait for 2 seconds
	 */
	msleep(2000);
	/*
	 * Tear everything down
	 */
	ret = 0;
	goto destroy_both;

destroy_both:
	lcd_cap_delete(lcd);
	lcd_destroy_create_ctx(ctx);
destroy_vfs:
	lcd_destroy_module_klcd(vfs, "lcd_test_mod_pmfs_vfs");
lcd_exit:
	/* frees endpoint */
	lcd_exit(0);
out:
	return ret;
}

static void boot_exit(void)
{
	/* nothing to do */
}

module_init(boot_main);
module_exit(boot_exit);
