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
	ret = klcd_create_module_klcd(&vfs, 
				LCD_DIR("pmfs/vfs-klcd"),
				"lcd_test_mod_pmfs_vfs");
	if (ret) {
		LIBLCD_ERR("create vfs klcd");
		goto lcd_exit;
	}
	/*
	 * Create pmfs lcd
	 */
	ret = lcd_create_module_lcd(&lcd, 
				LCD_DIR("pmfs/pmfs-lcd"),
				"lcd_test_mod_pmfs_lcd",
				LCD_CPTR_NULL, &mi);
	if (ret) {
		LIBLCD_ERR("create module lcd");
		goto destroy_vfs;
	}

	/* GRANT ENDPOINTS TO PMFS ------------------------------ */

	/*
	 * Alloc dest slot
	 */
	ret = __lcd_alloc_cptr(mi->cache, &pmfs_dest1);
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
	ret = __lcd_alloc_cptr(mi->cache, &pmfs_dest2);
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

	vfs_dest1 = __cptr(3);
	ret = lcd_cap_grant(vfs, vfs_chnl, vfs_dest1);
	if (ret) {
		LIBLCD_ERR("failed to grant vfs endpoint to vfs");
		goto destroy_both;
	}
	vfs_dest2 = lcd_cptr_set_lvl(__cptr(0), 1);
	vfs_dest2 = lcd_cptr_set_fanout(vfs_dest2, 0, 0);
	vfs_dest2 = lcd_cptr_set_slot(vfs_dest2, 0);
	ret = lcd_cap_grant(vfs, bdi_chnl, vfs_dest2);
	if (ret) {
		LIBLCD_ERR("failed to grant bdi endpoint to vfs");
		goto destroy_both;
	}
	/* DUMP BOOT INFO FOR PMFS ------------------------------ */

	/*
	 * Set up boot info
	 */
	ret = lcd_dump_boot_info(mi);
	if (ret) {
		LIBLCD_ERR("dump boot info");
		goto destroy_both;
	}
	to_boot_info(mi)->cptrs[0] = pmfs_dest1;
	to_boot_info(mi)->cptrs[1] = pmfs_dest2;

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
	lcd_destroy_module_lcd(lcd, mi, LCD_CPTR_NULL);
destroy_vfs:
	klcd_destroy_module_klcd(vfs, "lcd_test_mod_pmfs_vfs");
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
