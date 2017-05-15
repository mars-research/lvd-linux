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
#include <linux/kmod.h>

#include "../glue_helper.h"

#include <lcd_config/post_hook.h>

cptr_t net_klcd, dummy_lcd;
struct lcd_create_ctx *dummy_ctx;
cptr_t net_chnl;
cptr_t net_chnl_domain_cptr, dummy_chnl_domain_cptr;

static int boot_main(void)
{
	int ret;
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
				&net_klcd);

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

	ret = cptr_alloc(lcd_to_boot_cptr_cache(dummy_ctx), 
			&dummy_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto fail5;
	}
	ret = lcd_cap_grant(dummy_lcd, net_chnl, dummy_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("grant");
		goto fail6;
	}
	
	/* ---------- Set up boot info ---------- */
	// HACK: But WTF is this?
	net_chnl_domain_cptr = __cptr(3);
	ret = lcd_cap_grant(net_klcd, net_chnl, net_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("grant");
		goto fail7;
	}

	lcd_to_boot_info(dummy_ctx)->cptrs[0] = dummy_chnl_domain_cptr;


	/* ---------- RUN! ---------- */

	LIBLCD_MSG("starting network...");
	ret = lcd_run(net_klcd);
	if (ret) {
		LIBLCD_ERR("failed to start vfs lcd");
		goto fail8;
	}
	
	LIBLCD_MSG("starting dummy ethernet...");
	ret = lcd_run(dummy_lcd);
	if (ret) {
		LIBLCD_ERR("failed to start dummy lcd");
		goto fail9;
	}
	/*
	 * Wait for 4 seconds
	 */
	//msleep(100000);
	/*
	 * Tear everything down
	 */
	ret = 0;
	// return
	goto fail1;


	/* The destroy's will free up everything ... */
fail9:
fail8:
fail7:
	lcd_cap_delete(dummy_lcd);
	lcd_destroy_create_ctx(dummy_ctx);
fail6:
fail5:
fail4:
	//lcd_cap_delete(net_klcd);
	lcd_destroy_module_klcd(net_klcd, "lcd_test_mod_nullnet_net_klcd");
fail3:
fail2:
	lcd_exit(0); /* will free endpoints */
fail1:
	return ret;
}

static DECLARE_WAIT_QUEUE_HEAD(wq);
static int shutdown = 0;

int boot_lcd_thread(void *data)
{
	static unsigned once = 0;
	int ret;
	while (!kthread_should_stop()) {
		if (!once) {
			LCD_MAIN({
				ret = boot_main();
			});
		}
		once = 1;
		wait_event_interruptible(wq, shutdown != 0);
	}
	msleep(2000);
	LIBLCD_MSG("Exiting thread");
	lcd_destroy_module_klcd(net_klcd,
			"lcd_test_mod_nullnet_net_klcd");
	if (current->lcd)
		lcd_cap_delete(dummy_lcd);
	if (dummy_ctx)
		lcd_destroy_create_ctx(dummy_ctx);

	lcd_exit(0);
	return 0;
}

struct task_struct *boot_task;

static int boot_init(void)
{
	LIBLCD_MSG("%s: entering", __func__);

	boot_task = kthread_create(boot_lcd_thread, NULL, "boot_lcd_thread");

	if (!IS_ERR(boot_task))
		wake_up_process(boot_task);
	return 0;
}

static void boot_exit(void)
{
	/* nothing to do */
	if (!IS_ERR(boot_task)) {
		LIBLCD_MSG("%s: exiting", __func__);
		shutdown = 1;
		wake_up_interruptible(&wq);
		kthread_stop(boot_task);
	}
}
module_init(boot_init);
module_exit(boot_exit);
