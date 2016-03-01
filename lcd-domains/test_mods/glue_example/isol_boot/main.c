/*
 * main.c - non-isolated kernel module, does setup
 *          when fake minix and vfs are to be launched
 *          in isolated containers
 */

#include <lcd-domains/kliblcd.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int boot_main(void)
{
	int ret;
	cptr_t vfs_chnl;
	cptr_t vfs_chnl_domain_cptr, minix_chnl_domain_cptr;
	cptr_t vfs_lcd, minix_lcd;
	struct lcd_info *vfs_mi, *minix_mi;
	/*
	 * Enter lcd mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("klcd enter failed");
		goto fail1;
	}

	/* ---------- Create vfs channel ---------- */

	ret = lcd_create_sync_endpoint(&vfs_chnl);
	if (ret) {
		LIBLCD_ERR("lcd create sync endpoint");
		goto fail2;
	}

	/* ---------- Create LCDs ---------- */

	ret = lcd_create_module_lcd(&vfs_lcd, 
				LCD_DIR("glue_example/vfs"),
				"lcd_test_mod_glue_example_vfs",
				LCD_CPTR_NULL, &vfs_mi);
	if (ret) {
		LIBLCD_ERR("failed to create vfs lcd");
		goto fail3;
	}
	ret = lcd_create_module_lcd(&minix_lcd, 
				LCD_DIR("glue_example/minix"),
				"lcd_test_mod_glue_example_minix",
				LCD_CPTR_NULL, &minix_mi);
	if (ret) {
		LIBLCD_ERR("failed to create minix lcd");
		goto fail4;
	}

	/* ---------- Grant caps ---------- */

	ret = __lcd_alloc_cptr(vfs_mi->cache, &vfs_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto fail5;
	}
	ret = lcd_cap_grant(vfs_lcd, vfs_chnl, vfs_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("grant");
		goto fail6;
	}
	ret = __lcd_alloc_cptr(minix_mi->cache, &minix_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto fail7;
	}
	ret = lcd_cap_grant(minix_lcd, vfs_chnl, minix_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("grant");
		goto fail7;
	}
	
	/* ---------- Set up boot info ---------- */

	ret = lcd_dump_boot_info(vfs_mi);
	if (ret) {
		LIBLCD_ERR("dump vfs boot info");
		goto fail8;
	}
	to_boot_info(vfs_mi)->cptrs[0] = vfs_chnl_domain_cptr;

	ret = lcd_dump_boot_info(minix_mi);
	if (ret) {
		LIBLCD_ERR("dump minix boot info");
		goto fail9;
	}
	to_boot_info(minix_mi)->cptrs[0] = minix_chnl_domain_cptr;


	/* ---------- RUN! ---------- */

	LIBLCD_MSG("starting vfs...");
	ret = lcd_run(vfs_lcd);
	if (ret) {
		LIBLCD_ERR("failed to start vfs lcd");
		goto fail10;
	}
	/* Wait a few moments so vfs lcd has a chance to enter its
	 * dispatch loop. This is obviously a hack. You could use some
	 * kind of protocol to wait for the vfs to signal it is ready. */
	msleep(2000);

	LIBLCD_MSG("starting minix...");
	ret = lcd_run(minix_lcd);
	if (ret) {
		LIBLCD_ERR("failed to start minix lcd");
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


	/* The destroy's will free up everything ... */
out:
fail11:
fail10:
fail9:
fail8:
fail7:
fail6:
fail5:
	lcd_destroy_module_lcd(minix_lcd, minix_mi, LCD_CPTR_NULL);
fail4:
	lcd_destroy_module_lcd(vfs_lcd, vfs_mi, LCD_CPTR_NULL);
fail3:
fail2:
	lcd_exit(0); /* will free endpoints */
fail1:
	return ret;
}

static void boot_exit(void)
{
	/* nothing to do */
}

module_init(boot_main);
module_exit(boot_exit);
