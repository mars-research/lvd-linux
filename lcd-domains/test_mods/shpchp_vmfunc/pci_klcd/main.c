#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <thc.h>
#include "./shpchp_callee.h"
#include <asm/lcd_domains/libvmfunc.h>

#include <lcd_config/post_hook.h>

extern int vmfunc_init(void *stack_page, rpc_handler_t rpc_handler,
		rpc_handler_t sync_handler);
extern struct glue_cspace *c_cspace;
extern void *lcd_stack;

int shpchp_klcd_dispatch_async_loop(struct fipc_message *message);
int shpchp_klcd_syncipc_dispatch(struct fipc_message *message);

static int shpchp_klcd_init(void)
{
	struct fipc_message m;
	INIT_FIPC_MSG(&m);

	/* register with vmfunc */
	vmfunc_init(lcd_stack, shpchp_klcd_dispatch_async_loop, shpchp_klcd_syncipc_dispatch);

	/* call module_init for lcd */
	m.vmfunc_id = VMFUNC_RPC_CALL;
	m.rpc_id = MODULE_INIT;
	fipc_set_reg0(&m, this_cpu_read(cpu_info.loops_per_jiffy));
	vmfunc_klcd_wrapper(&m, OTHER_DOMAIN);

	return 0;
}

/*
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit shpchp_klcd_exit(void)
{
	struct fipc_message m;

	LIBLCD_MSG("%s: exiting", __func__);

	INIT_FIPC_MSG(&m);

	/* call module_init for lcd */
	m.vmfunc_id = VMFUNC_RPC_CALL;
	m.rpc_id = MODULE_EXIT;
	LIBLCD_MSG("Calling MODULE_EXIT of dummy_lcd");
	vmfunc_klcd_wrapper(&m, OTHER_DOMAIN);

	return;
}

module_init(shpchp_klcd_init);
module_exit(shpchp_klcd_exit);
MODULE_LICENSE("GPL");
