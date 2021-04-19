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

#include <asm/lcd_domains/libvmfunc.h>

#include <linux/kthread.h>
#include "../common.h"

/* COMPILER: This is always included after all includes. */
#include <lcd_config/post_hook.h>

extern int vmfunc_init(void *stack_page, rpc_handler_t rpc_handler,
		rpc_handler_t sync_handler);

/* INIT / EXIT ---------------------------------------- */
struct cspace *klcd_cspace;
extern void *lcd_stack;

int blk_klcd_dispatch_loop(struct fipc_message *msg)
{
	const size_t id = msg->rpc_id;
	if (!try_dispatch(id, msg, get_register_page(smp_processor_id()))) {
		glue_user_panic("Couldn't dispatch on KLCD side");
	}

	return 0;
}

int blk_klcd_syncipc_dispatch(struct fipc_message *message)
{
	glue_user_panic("Shouldn't have done a synchronous call!\n");
	return -1;
}

static int _blk_klcd_init(void)
{
	int ret;
	struct fipc_message m = {0};

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
	glue_user_init();

	vmfunc_init(lcd_stack, blk_klcd_dispatch_loop, blk_klcd_syncipc_dispatch);

	/* call module_init for lcd */
	m.vmfunc_id = VMFUNC_RPC_CALL;
	m.rpc_id = MODULE_INIT;
	LIBLCD_MSG("vmfunc_init successfull! Calling MODULE_INIT of nullb_lcd");
	vmfunc_klcd_wrapper(&m, OTHER_DOMAIN);

	return 0;

#ifndef CONFIG_LVD
fail1:
#endif
	return ret;
}

/* 
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit _blk_klcd_exit(void)
{
	struct fipc_message m = {0};

	LIBLCD_MSG("%s: exiting", __func__);

	/* call module_init for lcd */
	m.vmfunc_id = VMFUNC_RPC_CALL;
	m.rpc_id = MODULE_EXIT;
	LIBLCD_MSG("Calling MODULE_EXIT of nullb_lcd");
	vmfunc_klcd_wrapper(&m, OTHER_DOMAIN);

	/*
	 * Tear down blk glue
	 */
	//glue_blk_exit();

#ifndef CONFIG_LVD
	lcd_exit(0);
#endif
	return;
}

module_init(_blk_klcd_init);
module_exit(_blk_klcd_exit);
MODULE_LICENSE("GPL");
