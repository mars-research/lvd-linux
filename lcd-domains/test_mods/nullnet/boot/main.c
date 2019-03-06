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
struct lcd_create_ctx **dummy_ctxs;
cptr_t *dummy_lcds;
cptr_t net_chnl;
cptr_t net_chnl_domain_cptr, dummy_chnl_domain_cptr;
int num_lcds = 2;
cptr_t *dummy_chnl_domain_cptrs;

static int boot_main(void)
{
	int ret;
	int i;
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

	dummy_ctxs = kzalloc(sizeof(struct lcd_create_ctx*) * num_lcds, GFP_KERNEL);

	if (!dummy_ctxs) {
		LIBLCD_ERR("failed to alloc memory for dummy_ctxs");
		goto fail4;
	}
	printk("%s, dummy_ctxs %p\n", __func__, dummy_ctxs);

	dummy_lcds = kzalloc(sizeof(cptr_t*) * num_lcds, GFP_KERNEL);

	if (!dummy_lcds) {
		LIBLCD_ERR("failed to alloc memory for dummy_lcds");
		goto fail4;
	}

	printk("%s, dummy_lcds %p\n", __func__, dummy_lcds);

	dummy_chnl_domain_cptrs = kzalloc(sizeof(cptr_t*) * num_lcds, GFP_KERNEL);

	if (!dummy_chnl_domain_cptrs) {
		LIBLCD_ERR("failed to alloc memory for dummy_chnl_domain_cptrs");
		goto fail4;
	}

	ret = lcd_create_module_lcds(LCD_DIR("nullnet/dummy_lcd"),
				"lcd_test_mod_nullnet_dummy_lcd",
				dummy_lcds,
				dummy_ctxs, num_lcds);
	if (ret) {
		LIBLCD_ERR("failed to create dummy lcd");
		goto fail4;
	}

	LIBLCD_MSG("Created parent and child LCDS");

	for (i = 0; i < num_lcds; i++) {
		LIBLCD_MSG("LCD %d, cptr %lu | ctx %p\n", i,
				dummy_lcds[i].cptr,
				dummy_ctxs[i]);
	}

	for (i = 0; i < num_lcds; i++) {

		LIBLCD_MSG("granting domain_chnl_ptr to LCD %d\n", i);
		ret = cptr_alloc(lcd_to_boot_cptr_cache(dummy_ctxs[i]),
				&dummy_chnl_domain_cptrs[i]);
		printk("%s, cptr alloc returned %lu\n", __func__,
				dummy_chnl_domain_cptrs[i].cptr);
		if (ret) {
			LIBLCD_ERR("alloc cptr");
			goto fail5;
		}
		printk("%s, granting %lu to LCD %lu\n", __func__,
				net_chnl.cptr, dummy_lcds[i].cptr);
		ret = lcd_cap_grant(dummy_lcds[i], net_chnl, dummy_chnl_domain_cptrs[i]);
		printk("%s, grant returned %d\n", __func__, ret);

		if (ret) {
			LIBLCD_ERR("grant");
			goto fail6;
		}
		lcd_to_boot_info(dummy_ctxs[i])->cptrs[0] = dummy_chnl_domain_cptrs[i];
		printk("%s, done with LCD %d\n", __func__, i);
	}
	
	/* ---------- Set up boot info ---------- */
	// HACK: But WTF is this?
	net_chnl_domain_cptr = __cptr(3);
	printk("%s, granting %lu to KLCD %lu\n", __func__,
				net_chnl.cptr, net_klcd.cptr);
	ret = lcd_cap_grant(net_klcd, net_chnl, net_chnl_domain_cptr);
	if (ret) {
		LIBLCD_ERR("grant");
		goto fail7;
	}

	/* ---------- RUN! ---------- */
	printk("%s starting network\n", __func__);
	LIBLCD_MSG("starting network...");
	ret = lcd_run(net_klcd);
	if (ret) {
		LIBLCD_ERR("failed to start vfs lcd");
		goto fail8;
	}
	
	LIBLCD_MSG("starting dummy ethernet...\n");
	for (i = 0; i < num_lcds; i++) {
		LIBLCD_MSG("Starting LCD %d ", i);
		ret = lcd_run(dummy_lcds[i]);
		if (ret) {
			LIBLCD_ERR("failed to start dummy lcd");
			goto fail9;
		}
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
	lcd_cap_delete(dummy_lcds[0]);
	lcd_destroy_create_ctx(dummy_ctxs[0]);
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
	int i;
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
	if (current->lcd) {
		for (i = 0; i < num_lcds; i++) {
			lcd_cap_delete(dummy_lcds[i]);
		}
		kfree(dummy_lcds);
	}
	if (dummy_ctxs) {
		for (i = 0; i < num_lcds; i++) {
			lcd_destroy_create_ctx(dummy_ctxs[i]);
		}
		kfree(dummy_ctxs);
	}
	kfree(dummy_chnl_domain_cptrs);

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
MODULE_LICENSE("GPL");
