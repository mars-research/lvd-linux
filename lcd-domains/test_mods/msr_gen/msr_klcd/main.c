#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <thc.h>

//#include "../glue_helper.h"
#include <asm/lcd_domains/libvmfunc.h>

#include "../common.h"

#include <lcd_config/post_hook.h>

#define INIT_FIPC_MSG(msg)              memset(msg, 0x0, sizeof(*msg))

extern int vmfunc_init(void *stack_page, rpc_handler_t rpc_handler,
		rpc_handler_t sync_handler);

/* INIT / EXIT ---------------------------------------- */
struct cspace *klcd_cspace;
extern void *lcd_stack;

int msr_klcd_dispatch_loop(struct fipc_message *msg)
{
	const size_t id = msg->rpc_id;
	if (!try_dispatch(id, msg, get_register_page(smp_processor_id()))) {
		glue_user_panic("Couldn't dispatch on KLCD side");
	}

	return 0;
}

int msr_klcd_syncipc_dispatch(struct fipc_message *message)
{
	glue_user_panic("Shouldn't have done a synchronous call!\n");
	return -1;
}

static int msr_klcd_init(void)
{
	int ret = 0;
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

	INIT_FIPC_MSG(&m);

	/* save cspace for future use
	 * when userspace functions call function pointers,
	 * we need to get access to the sync_ep of this klcd
	 * to transfer pointers and data thro sync IPC to the lcd
	 */
	klcd_cspace = get_current_cspace(current);

	/*
	 * Init net glue
	 */
	glue_user_init();
	LIBLCD_MSG("-===== > glue msr init called\n");

	if (ret) {
		LIBLCD_ERR("msr init");
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

#ifndef CONFIG_LVD
	/*
	 * In case of LVD, boot module calls lcd_exit
	 */
	lcd_exit(0);
#endif

	return;
}

module_init(msr_klcd_init);
module_exit(msr_klcd_exit);
MODULE_LICENSE("GPL");
