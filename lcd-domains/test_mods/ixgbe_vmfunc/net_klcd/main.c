#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <thc.h>
#include "./ixgbe_callee.h"
#include <asm/lcd_domains/libvmfunc.h>

#include <lcd_config/post_hook.h>

extern int vmfunc_init(void *stack_page, rpc_handler_t rpc_handler,
		rpc_handler_t sync_handler);
extern int ixgbe_trigger_dump(void);
extern struct glue_cspace *c_cspace;
extern void *lcd_stack;

/* to dump ixgbe registers */
static bool ixgbe_dump = 0;
module_param_named(dump_regs, ixgbe_dump, bool, S_IWUSR);

int net_klcd_dispatch_async_loop(struct fipc_message *message);
int net_klcd_syncipc_dispatch(struct fipc_message *message);

/* INIT / EXIT ---------------------------------------- */
struct cspace *klcd_cspace;
struct task_struct *task_klcd;

static int net_klcd_init(void)
{
	int ret;
	struct fipc_message m;
	/*
	 * Set up cptr cache, etc.
	 */
#ifndef CONFIG_LVD
	/*
	 * there is no separate LCD for KLCD module, we operate in the context
	 * of boot thread
	 */

	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter");
		goto fail1;
	}
#endif
	INIT_FIPC_MSG(&m);
	/*
	 * Init net glue
	 */
	ret = glue_ixgbe_init();
	LIBLCD_MSG("-===== > glue ixgbe init called\n");
	if (ret) {
		LIBLCD_ERR("net init");
		goto fail2;
	}
	/* register with vmfunc */
	vmfunc_init(lcd_stack, net_klcd_dispatch_async_loop, net_klcd_syncipc_dispatch);

	/* call module_init for lcd */
	m.vmfunc_id = VMFUNC_RPC_CALL;
	m.rpc_id = MODULE_INIT;
	vmfunc_klcd_wrapper(&m, OTHER_DOMAIN);

	return 0;

fail2:
#ifndef CONFIG_LVD
fail1:
#endif
	lcd_exit(ret);
	return ret;
}

/*
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit net_klcd_exit(void)
{
	struct fipc_message m;

	LIBLCD_MSG("%s: exiting", __func__);

	INIT_FIPC_MSG(&m);

	/* call module_init for lcd */
	m.vmfunc_id = VMFUNC_RPC_CALL;
	m.rpc_id = MODULE_EXIT;
	LIBLCD_MSG("Calling MODULE_EXIT of dummy_lcd");
	vmfunc_klcd_wrapper(&m, OTHER_DOMAIN);

	/*
	 * Tear down net glue
	 */
	glue_ixgbe_exit();

#ifndef CONFIG_LVD
	/*
	 * in case of lvd, boot module calls lcd_exit
	 */
	lcd_exit(0);
#endif

	return;
}

module_init(net_klcd_init);
module_exit(net_klcd_exit);
MODULE_LICENSE("GPL");
