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
static unsigned int bind_cpu = 2;

module_param(bind_cpu, uint, 0644);
MODULE_PARM_DESC(bind_cpu, "Bind kthread to this cpu");

static unsigned int domain;
static unsigned int bus;
static unsigned int slot;
static unsigned int fn;

module_param(domain, uint, 0);
module_param(bus, uint, 0);
module_param(slot, uint, 0);
module_param(fn, uint, 0);

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

	m = lvd_create_module_klcd_no_thread(LCD_DIR("ixgbe_vmfunc/net_klcd"),
				"lcd_test_mod_ixgbe_vmfunc_net_klcd",
				&net_klcd);

	if (!m) {
		ret = -1;
		LIBLCD_ERR("failed to create net klcd");
		goto fail2;
	}

	/* ---------- Create LCD ---------- */
	ret = lvd_create_module_lvd(LCD_DIR("ixgbe_vmfunc/ixgbe_lcd"),
				"lcd_test_mod_ixgbe_vmfunc_ixgbe_lcd",
				&ixgbe_lcd,
				&ixgbe_ctx, 1);
	if (ret) {
		LIBLCD_ERR("failed to create ixgbe lcd");
		goto fail3;
	}

	/* for udelay calculation we need lpj inside LCD */
	lcd_to_boot_info(ixgbe_ctx)->cptrs[1].cptr =
			this_cpu_read(cpu_info.loops_per_jiffy);

	lcd_to_boot_info(ixgbe_ctx)->cptrs[2].cptr = domain;
	lcd_to_boot_info(ixgbe_ctx)->cptrs[3].cptr = bus;
	lcd_to_boot_info(ixgbe_ctx)->cptrs[4].cptr = slot;
	lcd_to_boot_info(ixgbe_ctx)->cptrs[5].cptr = fn;

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
			ret = boot_main();
		}
		once = 1;
		wait_event_interruptible(wq, shutdown != 0);
	}
	LIBLCD_MSG("Exiting thread");

	if (!ret) {
		/* trigger exit module */
		lcd_stop(net_klcd);

		lcd_destroy_module_klcd(net_klcd,
				"lcd_test_mod_ixgbe_vmfunc_net_klcd");

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

	{
		struct task_struct *proc_list;
		for_each_process(proc_list) {
			proc_list->mapped_cr3 = 0UL;
		}
	}
}

module_init(boot_init);
module_exit(boot_exit);

MODULE_LICENSE("GPL");
