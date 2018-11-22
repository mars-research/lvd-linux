/*
 * boot.c - non-isolated boot module
 */

#include <lcd_config/pre_hook.h>

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#include <lcd_config/post_hook.h>

cptr_t endpoint;
cptr_t lcd1, lcd2;
cptr_t dest1, dest2;
struct lcd_create_ctx *ctx1, *ctx2;

static int boot_main(void)
{
	int ret;

	/*
	 * Enter LCD mode
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
	 * Create lcds
	 */
	ret = lcd_create_module_lcd(LCD_DIR("vmfunc/caller_lcd"),
				"lcd_test_mod_vmfunc_caller_lcd",
				&lcd1, 
				&ctx1);
	if (ret) {
		LIBLCD_ERR("failed to create lcd1");
		goto fail3;
	}
#ifdef TEST_VMFUNC_TWO_LCDS
	ret = lcd_create_module_lcd(LCD_DIR("vmfunc/callee_lcd"),
				"lcd_test_mod_vmfunc_callee_lcd",
				&lcd2, 
				&ctx2);
	if (ret) {
		LIBLCD_ERR("failed to create lcd2");
		goto fail4;
	}
#endif
	/*
	 * --------------------------------------------------
	 * Grant cap to endpoint
	 *
	 * Allocate cptrs
	 */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx1), &dest1);
	if (ret) {
		LIBLCD_ERR("failed to alloc dest slot");
		goto fail5;
	}

#ifdef TEST_VMFUNC_TWO_LCDS
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx2), &dest2);
	if (ret) {
		LIBLCD_ERR("failed to alloc dest slot");
		goto fail6;
	}
#endif
	ret = lcd_cap_grant(lcd1, endpoint, dest1);
	if (ret) {
		LIBLCD_ERR("failed to grant endpoint to lcd1");
		goto fail7;
	}
	lcd_to_boot_info(ctx1)->cptrs[0] = dest1;

#ifdef TEST_VMFUNC_TWO_LCDS
	ret = lcd_cap_grant(lcd2, endpoint, dest2);
	if (ret) {
		LIBLCD_ERR("failed to grant endpoint to lcd2");
		goto fail8;
	}
	lcd_to_boot_info(ctx2)->cptrs[0] = dest2;
#endif

	/*
	 * --------------------------------------------------
	 * Run lcd's
	 *
	 */
	ret = lcd_run(lcd1);
	if (ret) {
		LIBLCD_ERR("failed to start lcd1");
		goto fail9;
	}
#ifdef TEST_VMFUNC_TWO_LCDS
	ret = lcd_run(lcd2);
	if (ret) {
		LIBLCD_ERR("failed to start lcd2");
		goto fail10;
	}
#endif
	ret = 0;
	goto fail1;

	/*
	 * Everything torn down / freed during destroy / exit.
	 */
#ifdef TEST_VMFUNC_TWO_LCDS
fail10:
fail8:
fail6:
#endif
fail9:
fail7:
fail5:

#ifdef TEST_VMFUNC_TWO_LCDS
	lcd_cap_delete(lcd2);
	lcd_destroy_create_ctx(ctx2); /* tears down LCD 2 */
fail4:
#endif
	lcd_cap_delete(lcd1);
	lcd_destroy_create_ctx(ctx1); /* tears down LCD 1 */
fail3:
fail2:
	lcd_exit(0); /* tears down everything else */
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

	if (current->lcd) {
#ifdef TEST_VMFUNC_TWO_LCDS
		lcd_cap_delete(lcd2);
#endif
		lcd_cap_delete(lcd1);
	}

#ifdef TEST_VMFUNC_TWO_LCDS
	if (ctx2)
		lcd_destroy_create_ctx(ctx2); /* tears down LCD 2 */
#endif
	if (ctx1)
		lcd_destroy_create_ctx(ctx1); /* tears down LCD 1 */

	lcd_exit(0);
	return 0;
}
struct task_struct *boot_task;

static int boot_init(void)
{

	boot_task = kthread_create(boot_lcd_thread, NULL, "boot_lcd_thread");

	if (!IS_ERR(boot_task))
		wake_up_process(boot_task);
	return 0;
}

static void boot_exit(void)
{
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
