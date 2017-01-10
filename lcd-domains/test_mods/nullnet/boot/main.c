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
	cptr_t net_chnl;
	cptr_t net_chnl_domain_cptr, dummy_chnl_domain_cptr;
	cptr_t net_lcd, dummy_lcd;
	struct lcd_create_ctx *dummy_ctx;
	/*
	 * Enter lcd mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter failed");
		goto fail1;
	}

	/* ---------- Create vfs channel ---------- */

	ret = lcd_create_sync_endpoint(&net_chnl);
	if (ret) {
		LIBLCD_ERR("lcd create sync endpoint");
		goto fail2;
	}

	/* ---------- Create LCDs ---------- */

	ret = lcd_create_module_klcd(LCD_DIR("nullnet/net_klcd"),
				"lcd_test_mod_nullnet_net_klcd",
				&net_lcd);

	if (ret) {
		LIBLCD_ERR("failed to create net klcd");
		goto fail3;
	}
	ret = lcd_create_module_lcd(LCD_DIR("nullnet/dummy_lcd"),
				"lcd_test_mod_nullnet_dummy_lcd",
				&dummy_lcd,
				&dummy_ctx);
	if (ret) {
		LIBLCD_ERR("failed to create dummy lcd");
		goto fail4;
	}

	/* ---------- Grant caps ---------- */
	/*
	ret = cptr_alloc(lcd_to_boot_cptr_cache(net_ctx), 
			&net_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto fail5;
	}
	ret = lcd_cap_grant(net_lcd, net_chnl, net_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("grant");
		goto fail6;
	} */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(dummy_ctx), 
			&dummy_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto fail7;
	}
	ret = lcd_cap_grant(dummy_lcd, net_chnl, dummy_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("grant");
		goto fail8;
	}
	
	/* ---------- Set up boot info ---------- */

//	lcd_to_boot_info(net_ctx)->cptrs[0] = net_chnl_domain_cptr;
	net_chnl_domain_cptr = __cptr(3);
	ret = lcd_cap_grant(net_lcd, net_chnl, net_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("grant");
		goto fail6;
	}

	lcd_to_boot_info(dummy_ctx)->cptrs[0] = dummy_chnl_domain_cptr;


	/* ---------- RUN! ---------- */

	LIBLCD_MSG("starting network...");
	ret = lcd_run(net_lcd);
	if (ret) {
		LIBLCD_ERR("failed to start vfs lcd");
		goto fail9;
	}
	/* Wait a few moments so vfs lcd has a chance to enter its
	 * dispatch loop. This is obviously a hack. You could use some
	 * kind of protocol to wait for the vfs to signal it is ready. */
	//msleep(2000);

	LIBLCD_MSG("starting dummy ethernet...");
	ret = lcd_run(dummy_lcd);
	if (ret) {
		LIBLCD_ERR("failed to start dummy lcd");
		goto fail10;
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
fail10:
fail9:
fail8:
fail7:
fail6:
	lcd_cap_delete(dummy_lcd);
	lcd_destroy_create_ctx(dummy_ctx);
fail4:
	lcd_cap_delete(net_lcd);
fail3:
fail2:
	lcd_exit(0); /* will free endpoints */
fail1:
	return ret;
}

static int boot_init(void)
{
	int ret;
	
	LCD_MAIN({

			ret = boot_main();

		});

	return ret;
}

static void boot_exit(void)
{
	/* nothing to do */
}

module_init(boot_init);
module_exit(boot_exit);
