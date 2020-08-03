#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <thc.h>

#include "./msr_callee.h"

#include <asm/lcd_domains/libvmfunc.h>

#include <lcd_config/post_hook.h>

extern int vmfunc_init(void *stack_page, rpc_handler_t rpc_handler,
		rpc_handler_t sync_handler);

/* INIT / EXIT ---------------------------------------- */
struct cspace *klcd_cspace;
extern void *lcd_stack;

int msr_klcd_dispatch_loop(struct fipc_message *msg);
int msr_klcd_syncipc_dispatch(struct fipc_message *message);

static int msr_klcd_init(void)
{
	int ret;
	struct fipc_message m;
	/*
	 * Set up cptr cache, etc.
	 */
	INIT_FIPC_MSG(&m);

	/*
	 * Init net glue
	 */
	ret = glue_msr_init();
	LIBLCD_MSG("-===== > glue msr init called\n");

	if (ret) {
		LIBLCD_ERR("net init");
		goto fail2;
	}

	vmfunc_init(lcd_stack, msr_klcd_dispatch_loop, msr_klcd_syncipc_dispatch);

	/* call module_init for lcd */
	m.vmfunc_id = VMFUNC_RPC_CALL;
	m.rpc_id = MODULE_INIT;
	LIBLCD_MSG("vmfunc_init successfull! Calling MODULE_INIT of dummy_lcd");
	vmfunc_klcd_wrapper(&m, OTHER_DOMAIN);

	return 0;

fail2:
	lcd_exit(ret);
#ifndef CONFIG_LVD
fail1:
#endif
	return ret;
}

/*
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit msr_klcd_exit(void)
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
	glue_msr_exit();

	return;
}

module_init(msr_klcd_init);
module_exit(msr_klcd_exit);
MODULE_LICENSE("GPL");
