/*
 * boot.c - non-isolated kernel module, does setup
 *          when fake minix and vfs are to be launched
 *          in isolated containers
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
	cptr_t vfs_chnl;
	cptr_t vfs_chnl_domain_cptr, minix_chnl_domain_cptr;
	cptr_t vfs_lcd, minix_lcd;
	struct lcd_create_ctx *vfs_ctx, *minix_ctx;
	/*
	 * Enter lcd mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter failed");
		goto fail1;
	}

	/* ---------- Create vfs channel ---------- */

	ret = lcd_create_sync_endpoint(&vfs_chnl);
	if (ret) {
		LIBLCD_ERR("lcd create sync endpoint");
		goto fail2;
	}

	/* ---------- Create LCDs ---------- */

	ret = lcd_create_module_lcd(LCD_DIR("glue_example/vfs"),
				"lcd_test_mod_glue_example_vfs",
				&vfs_lcd,
				&vfs_ctx);
	if (ret) {
		LIBLCD_ERR("failed to create vfs lcd");
		goto fail3;
	}
	ret = lcd_create_module_lcd(LCD_DIR("glue_example/minix"),
				"lcd_test_mod_glue_example_minix",
				&minix_lcd,
				&minix_ctx);
	if (ret) {
		LIBLCD_ERR("failed to create minix lcd");
		goto fail4;
	}

	/* ---------- Grant caps ---------- */

	ret = cptr_alloc(vfs_ctx->cache, &vfs_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto fail5;
	}
	ret = lcd_cap_grant(vfs_lcd, vfs_chnl, vfs_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("grant");
		goto fail6;
	}
	ret = cptr_alloc(minix_ctx->cache, &minix_chnl_domain_cptr);
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

	lcd_to_boot_info(vfs_ctx)->cptrs[0] = vfs_chnl_domain_cptr;

	lcd_to_boot_info(minix_mi)->cptrs[0] = minix_chnl_domain_cptr;


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
	lcd_cap_delete(minix_lcd);
	lcd_destroy_create_ctx(minix_ctx);
fail4:
	lcd_cap_delete(vfs_lcd);
	lcd_destroy_create_ctx(vfs_ctx);
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
