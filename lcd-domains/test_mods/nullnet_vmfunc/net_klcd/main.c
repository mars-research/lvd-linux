#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <thc.h>

#include "./nullnet_callee.h"
#include "../rdtsc_helper.h"
#include "../perf_counter_helper.h"

/* COMPILER: This is always included after all includes. */
#include <lcd_config/post_hook.h>

/* INIT / EXIT ---------------------------------------- */
struct cspace *klcd_cspace;

static int net_klcd_init(void) 
{
	int ret;
	cptr_t net_chnl;
	/*
	 * Set up cptr cache, etc.
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter");
		goto fail1;
	}

	/*
	 * XXX: Hack: boot provided us with one cptr for the net chnl
	 */
	ret = lcd_cptr_alloc(&net_chnl);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto fail2;
	}
	LIBLCD_MSG("==========> got cptr %lu\n", net_chnl.cptr);
	/* save cspace for future use
	 * when userspace functions call function pointers,
	 * we need to get access to the sync_ep of this klcd
	 * to transfer pointers and data thro sync IPC to the lcd
	 */
	klcd_cspace = get_current_cspace(current);
	/*
	 * Init net glue
	 */
	ret = glue_nullnet_init();
	LIBLCD_MSG("-===== > glue nullnet init called\n");
	if (ret) {
		LIBLCD_ERR("net init");
		goto fail3;
	}

	return 0;

fail3:
fail2:
	lcd_exit(ret);
fail1:
	return ret;
}

/* 
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit net_klcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);
	/*
	 * Tear down net glue
	 */
	glue_nullnet_exit();

	lcd_exit(0);

	return;
}

module_init(net_klcd_init);
module_exit(net_klcd_exit);
MODULE_LICENSE("GPL");

