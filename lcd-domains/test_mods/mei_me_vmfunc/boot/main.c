/*
 * boot.c - non-isolated kernel module
 *          Launches nvme lcd and blk klcd modules and waits
 *	    until they are shutdown.
 *
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kmod.h>

#include "../mei_glue_helper.h"

#include <lcd_config/post_hook.h>

cptr_t mei_klcd, mei_me_lcd;
struct lcd_create_ctx *mei_me_ctx;
static unsigned int bind_cpu = 2;

module_param(bind_cpu, uint, 0644);
MODULE_PARM_DESC(bind_cpu, "Bind kthread to this cpu");

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

	m = lvd_create_module_klcd_no_thread(LCD_DIR("mei_me_vmfunc/mei_klcd"),
				"lcd_test_mod_mei_me_vmfunc_mei_klcd",
				&mei_klcd);

	if (!m) {
		ret = -1;
		LIBLCD_ERR("failed to create mei klcd");
		goto fail2;
	}

	/* ---------- Create LCD ---------- */
	ret = lvd_create_module_lvd(LCD_DIR("mei_me_vmfunc/mei_me_lcd"),
				"lcd_test_mod_mei_me_vmfunc_mei_me_lcd",
                
				&mei_me_lcd,
				&mei_me_ctx, 1);
	if (ret) {
		LIBLCD_ERR("failed to create nvme lcd");
		goto fail3;
	}

	/* for udelay calculation we need lpj inside LCD */
	lcd_to_boot_info(mei_me_ctx)->cptrs[1].cptr =
			this_cpu_read(cpu_info.loops_per_jiffy);

	/* ---------- RUN! ---------- */
	LIBLCD_MSG("starting blk...");
	ret = lcd_run(mei_klcd);
	if (ret) {
		LIBLCD_ERR("failed to start blk lcd");
		goto fail4;
	}

	LIBLCD_MSG("starting nvme...");
	ret = lcd_run(mei_me_lcd);
	if (ret) {
		LIBLCD_ERR("failed to start nvme lcd");
		goto fail5;
	}

	goto fail1;

	/* The destroy's will free up everything ... */
fail5:
fail4:
	lcd_cap_delete(mei_me_lcd);
	lcd_destroy_create_ctx(mei_me_ctx);
fail3:
	//lcd_cap_delete(mei_klcd);
	lcd_destroy_module_klcd(mei_klcd, "lcd_test_mod_mei_me_vmfunc_mei_klcd");
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
			ret = boot_main();
		}
		once = 1;
		wait_event_interruptible(wq, shutdown != 0);
	}
	LIBLCD_MSG("Exiting thread");

	if (!ret) {
		/* trigger exit module */
		lcd_stop(mei_klcd);

		lcd_destroy_module_klcd(mei_klcd,
				"lcd_test_mod_mei_me_vmfunc_mei_klcd");

		if (current->lcd)
			lcd_cap_delete(mei_me_lcd);
		if (mei_me_ctx)
			lcd_destroy_create_ctx(mei_me_ctx);
		lcd_exit(0);
	}
	return 0;
}

struct task_struct *boot_task;

static int boot_init(void)
{
	LIBLCD_MSG("%s: entering", __func__);

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
