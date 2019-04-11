/*
 * main.c - module init/exit for vfs klcd
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <thc.h>

#include "nullb_callee.h"

#include <lcd_config/post_hook.h>

#include <linux/kthread.h>

extern int vmfunc_init(void *stack_page, rpc_handler_t rpc_handler,
		rpc_handler_t sync_handler);
/* INIT / EXIT ---------------------------------------- */
struct cspace *klcd_cspace;
extern void *lcd_stack;

int blk_klcd_dispatch_loop(struct fipc_message *msg);
int blk_klcd_syncipc_dispatch(struct fipc_message *message);

static int blk_klcd_init(void) 
{
	int ret;
	struct fipc_message m;

	/*
	 * Set up cptr cache, etc.
	 */
#ifndef CONFIG_LVD
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter");
		goto fail1;
	}
#endif
	klcd_cspace = get_current_cspace(current);
	/*
	 * Init blk glue
	 */
	ret = glue_blk_init();
	if (ret) {
		LIBLCD_ERR("vfs init");
		goto fail2;
	}

	vmfunc_init(lcd_stack, blk_klcd_dispatch_loop, blk_klcd_syncipc_dispatch);

	/* call module_init for lcd */
	m.vmfunc_id = VMFUNC_RPC_CALL;
	m.rpc_id = MODULE_INIT;
	LIBLCD_MSG("vmfunc_init successfull! Calling MODULE_INIT of nullb_lcd");
	vmfunc_klcd_wrapper(&m, OTHER_DOMAIN);

	return 0;

fail2:
	lcd_exit(ret);

#ifndef CONFIG_LVD
fail1:
#endif
	return ret;
}

static int _blk_klcd_init(void)
{
	return blk_klcd_init();
}

/* 
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit _blk_klcd_exit(void)
{
	struct fipc_message m;

	LIBLCD_MSG("%s: exiting", __func__);

	/* call module_init for lcd */
	m.vmfunc_id = VMFUNC_RPC_CALL;
	m.rpc_id = MODULE_EXIT;
	LIBLCD_MSG("Calling MODULE_EXIT of nullb_lcd");
	vmfunc_klcd_wrapper(&m, OTHER_DOMAIN);

	/*
	 * Tear down blk glue
	 */
	glue_blk_exit();

	lcd_exit(0);

	return;
}

module_init(_blk_klcd_init);
module_exit(_blk_klcd_exit);
MODULE_LICENSE("GPL");
