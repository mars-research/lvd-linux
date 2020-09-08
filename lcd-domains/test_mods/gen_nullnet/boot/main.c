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
static unsigned int bind_cpu = 2;

module_param(bind_cpu, uint, 0644);
MODULE_PARM_DESC(bind_cpu, "Bind kthread to this cpu");

static int boot_main(void)
{
	int ret;
	struct module *m = NULL;

	/*
	 * Enter lcd mode
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter failed");
		goto fail1;
	}

	/* ---------- Create LCDs ---------- */

	m = lvd_create_module_klcd_no_thread(LCD_DIR("gen_nullnet/klcd"),
				"lcd_test_mod_gen_nullnet_klcd",
				&net_klcd);

	if (!m) {
		LIBLCD_ERR("failed to create klcd");
		ret = -1;
		goto fail3;
	}
	ret = lvd_create_module_lvd(LCD_DIR("gen_nullnet/lcd"),
				"lcd_test_mod_gen_nullnet_lcd",
				&dummy_lcd,
				&dummy_ctx, 1);
	if (ret) {
		LIBLCD_ERR("failed to create lcd");
		goto fail4;
	}

	/* ---------- RUN! ---------- */

	LIBLCD_MSG("starting network...");

	/* run KLCD init */
	ret = lcd_run(net_klcd);
	if (ret) {
		LIBLCD_ERR("failed to start klcd");
		goto fail8;
	}

	LIBLCD_MSG("starting dummy ethernet...");

	ret = lcd_run(dummy_lcd);
	if (ret) {
		LIBLCD_ERR("failed to start lcd");
		goto fail9;
	}

	return 0;

	/* The destroy's will free up everything ... */
fail9:
fail8:
	lcd_cap_delete(dummy_lcd);
	lcd_destroy_create_ctx(dummy_ctx);
fail4:
	//lcd_cap_delete(net_klcd);
	lcd_destroy_module_klcd(net_klcd, "lcd_test_mod_gen_nullnet_klcd");
fail3:
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
	msleep(2000);
	LIBLCD_MSG("Exiting thread");
	if (!ret) {
		/* trigger exit module */
		lcd_stop(net_klcd);

		lcd_destroy_module_klcd(net_klcd,
				"lcd_test_mod_gen_nullnet_klcd");
		if (current->lcd)
			lcd_cap_delete(dummy_lcd);
		if (dummy_ctx)
			lcd_destroy_create_ctx(dummy_ctx);

		lcd_exit(0);
	}
	return 0;
}

struct task_struct *boot_task;

static int boot_init(void)
{
	LIBLCD_MSG("%s: entering on cpu: %d", __func__, bind_cpu);

	boot_task = kthread_create(boot_lcd_thread, NULL, "boot_lcd_thread");

	kthread_bind(boot_task, bind_cpu);
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
