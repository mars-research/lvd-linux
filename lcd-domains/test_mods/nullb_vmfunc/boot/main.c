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

cptr_t blk_klcd, nullb_lcd;
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
	/* setup memory for LCD, assign in current->lcd 
	 * setup cspace, utcb, endpoint - hardcoded at cptr(1),
	 * comment says it is for CALL EP (not clear though!)
	 * thc_init */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter failed");
		goto fail1;
	}

	/* ---------- Create LCDs ---------- */
	/* Until now the boot module is current. Inside this function,
	 * a new lcd struct is created, a kthread is associated with the newly
	 * allocated lcd struct and the cptr for this lcd is inserted into the
	 * boot's cspace and the cptr is returned outside as blk_klcd */
	m = lvd_create_module_klcd_no_thread(LCD_DIR("nullb_vmfunc/blk_klcd"),
				"lcd_test_mod_nullb_vmfunc_blk_klcd",
				&blk_klcd);

	if (!m) {
		LIBLCD_ERR("failed to create net klcd");
		ret = -1;
		goto fail3;
	}

	ret = lvd_create_module_lvd(LCD_DIR("nullb_vmfunc/nullb_lcd"),
				"lcd_test_mod_nullb_vmfunc_nullb_lcd",
				&nullb_lcd,
				&dummy_ctx, 1);
	if (ret) {
		LIBLCD_ERR("failed to create dummy lcd");
		goto fail4;
	}

	/* ---------- RUN! ---------- */

	LIBLCD_MSG("starting blk klcd...");
	/* The capability pointer of the kthread that was created in the create_klcd
	 * call will now be woken up here! This will wake up a common kernel thread and
	 * the thread will call klcd_main() which my guess is the module init of klcd
	 * module! */
	ret = lcd_run(blk_klcd);
	if (ret) {
		LIBLCD_ERR("failed to start blk klcd");
		goto fail8;
	}
	
	LIBLCD_MSG("starting nullb lcd...");
	ret = lcd_run(nullb_lcd);
	if (ret) {
		LIBLCD_ERR("failed to start nullb lcd");
		goto fail9;
	}

	ret = 0;
	goto fail1;

	/* The destroy's will free up everything ... */
fail9:
fail8:
	lcd_cap_delete(nullb_lcd);
	lcd_destroy_create_ctx(dummy_ctx);
fail4:
	//lcd_cap_delete(blk_klcd);
	lcd_destroy_module_klcd(blk_klcd, "lcd_test_mod_nullb_vmfunc_blk_klcd");
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
			ret = boot_main();
		}
		once = 1;
		wait_event_interruptible(wq, shutdown != 0);
	}
	LIBLCD_MSG("Exiting thread");

	if (!ret) {
		/* trigger exit module */
		lcd_stop(blk_klcd);

		lcd_destroy_module_klcd(blk_klcd, "lcd_test_mod_nullb_vmfunc_blk_klcd");
		if (current->lcd)
			lcd_cap_delete(nullb_lcd);
		if (dummy_ctx)
			lcd_destroy_create_ctx(dummy_ctx);
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
