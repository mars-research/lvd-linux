/*
 * boot.c - non-isolated kernel module
 *          Launches ixgbe lcd and net klcd modules and waits
 *	    until they are shutdown.
 *
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kmod.h>

#include "../ixgbe_common.h"

#include <lcd_config/post_hook.h>

cptr_t net_klcd, ixgbe_lcd;
struct lcd_create_ctx *ixgbe_ctx;
cptr_t net_chnl;

static int boot_main(void)
{
	int ret;
	struct module *m;
	/*
	 * Enter lcd mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter failed");
		goto fail1;
	}

	/* ---------- Create KLCD ---------- */

	m = lvd_create_module_klcd_no_thread(LCD_DIR("ixgbe/net_klcd"),
				"lcd_test_mod_ixgbe_vmfunc_net_klcd",
				&net_klcd);

	if (!m) {
		ret = -1;
		LIBLCD_ERR("failed to create net klcd");
		goto fail2;
	}

	/* ---------- Create LCD ---------- */
	ret = lvd_create_module_lvd(LCD_DIR("ixgbe/ixgbe_lcd"),
				"lcd_test_mod_ixgbe_vmfunc_ixgbe_lcd",
				&ixgbe_lcd,
				&ixgbe_ctx, 1);
	if (ret) {
		LIBLCD_ERR("failed to create ixgbe lcd");
		goto fail3;
	}

	/* ---------- RUN! ---------- */
	LIBLCD_MSG("starting network...");
	ret = lcd_run(net_klcd);
	if (ret) {
		LIBLCD_ERR("failed to start vfs lcd");
		goto fail4;
	}

	LIBLCD_MSG("starting ixgbe ethernet...");
	ret = lcd_run(ixgbe_lcd);
	if (ret) {
		LIBLCD_ERR("failed to start ixgbe lcd");
		goto fail5;
	}

	goto fail1;

	/* The destroy's will free up everything ... */
fail5:
fail4:
	lcd_cap_delete(ixgbe_lcd);
	lcd_destroy_create_ctx(ixgbe_ctx);
fail3:
	//lcd_cap_delete(net_klcd);
	lcd_destroy_module_klcd(net_klcd, "lcd_test_mod_ixgbe_vmfunc_net_klcd");
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
	int ret = 0;
	while (!kthread_should_stop()) {
		if (!once) {
			LCD_MAIN({
				ret = boot_main();
			});
		}
		once = 1;
		wait_event_interruptible(wq, shutdown != 0);
	}
	LIBLCD_MSG("Exiting thread");

	if (!ret) {
		lcd_destroy_module_klcd(net_klcd, "lcd_test_mod_ixgbe_vmfunc_net_klcd");
		if (current->lcd)
			lcd_cap_delete(ixgbe_lcd);
		if (ixgbe_ctx)
			lcd_destroy_create_ctx(ixgbe_ctx);
		lcd_exit(0);
	}
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

MODULE_LICENSE("GPL");
