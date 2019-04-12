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

cptr_t klcd, lcd;
cptr_t dest_lcd, dest_klcd;
struct lcd_create_ctx *lcd_ctx;
static unsigned int bind_cpu = 2;

module_param(bind_cpu, uint, 0644);
MODULE_PARM_DESC(bind_cpu, "Bind kthread to this cpu");

static int boot_main(void)
{
	int ret;
	struct module *m = NULL;

	/*
	 * Enter LCD mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter failed");
		goto fail1;
	}

	/*
	 * Create lcds
	 */
	m = lvd_create_module_klcd_no_thread(LCD_DIR("vmfunc_klcd/caller_klcd"),
				"lcd_test_mod_vmfunc_klcd_caller_klcd",
				&klcd);
	if (!m) {
		LIBLCD_ERR("failed to create lcd1");
		ret = -1;
		goto fail3;
	}

	ret = lvd_create_module_lvd(LCD_DIR("vmfunc_klcd/callee_lcd"),
				"lcd_test_mod_vmfunc_klcd_callee_lcd",
				&lcd,
				&lcd_ctx, 1);
	if (ret) {
		LIBLCD_ERR("failed to create lcd2");
		goto fail4;
	}
	/*
	 * --------------------------------------------------
	 * Grant cap to endpoint
	 *
	 * Allocate cptrs
	 */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(lcd_ctx), &dest_lcd);
	if (ret) {
		LIBLCD_ERR("failed to alloc dest slot");
		goto fail5;
	}

	/*
	 * --------------------------------------------------
	 * Run lcd's
	 *
	 */
	LIBLCD_MSG("Starting KLCD...");

	ret = lcd_run(klcd);
	if (ret) {
		LIBLCD_ERR("failed to start lcd1");
		goto fail9;
	}

	LIBLCD_MSG("Starting LCD...");
	ret = lcd_run(lcd);
	if (ret) {
		LIBLCD_ERR("failed to start lcd2");
		goto fail10;
	}

	ret = 0;
	LIBLCD_MSG("Done booting vmfunc_klcd");
	goto fail1;

	/*
	 * Everything torn down / freed during destroy / exit.
	 */
fail10:
fail9:
fail5:
	lcd_cap_delete(lcd);
	lcd_destroy_create_ctx(lcd_ctx); /* tears down LCD 2 */
fail4:
	lcd_destroy_module_klcd(klcd, "lcd_test_mod_vmfunc_klcd_caller_klcd");
fail3:
	lcd_exit(0); /* tears down everything else */
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
	msleep(2000);
	LIBLCD_MSG("Exiting thread");
	/* tear down everything only if the load was successful */
	if (!ret) {
		lcd_destroy_module_klcd(klcd, "lcd_test_mod_vmfunc_klcd_caller_klcd");

		if (current->lcd) {
			lcd_cap_delete(lcd);
		}

		if (lcd_ctx)
			lcd_destroy_create_ctx(lcd_ctx); /* tears down LCD 2 */

		lcd_exit(0);
	}
	return 0;
}
struct task_struct *boot_task;

static int boot_init(void)
{
	boot_task = kthread_create(boot_lcd_thread, NULL, "boot_lcd_thread");

	kthread_bind(boot_task, bind_cpu);
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
