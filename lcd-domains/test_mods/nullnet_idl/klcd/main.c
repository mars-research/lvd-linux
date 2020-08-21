#include <lcd_config/pre_hook.h>

#include "../common.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#include "../rdtsc_helper.h"
#include "../perf_counter_helper.h"
#include <asm/lcd_domains/libvmfunc.h>
/* COMPILER: This is always included after all includes. */
#include <lcd_config/post_hook.h>

extern int vmfunc_init(void *stack_page, rpc_handler_t rpc_handler,
		rpc_handler_t sync_handler);

/* INIT / EXIT ---------------------------------------- */
struct cspace *klcd_cspace;
extern void *lcd_stack;

int dispatch(struct fipc_message *msg);

int sync_stub(struct fipc_message *message)
{
	LIBLCD_MSG("sync stub was called");
	return 0;
}

static int net_klcd_init(void) 
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
	INIT_FIPC_MSG(&m);

	/* save cspace for future use
	 * when userspace functions call function pointers,
	 * we need to get access to the sync_ep of this klcd
	 * to transfer pointers and data thro sync IPC to the lcd
	 */
	klcd_cspace = get_current_cspace(current);
	/*
	 * Init hashmaps
	 */
	hash_init(locals);
	hash_init(remotes);
	LIBLCD_MSG("-===== > initialized glue hashmaps\n");

	vmfunc_init(lcd_stack, dispatch, sync_stub);

	/* call module_init for lcd */
	m.vmfunc_id = VMFUNC_RPC_CALL;
	m.rpc_id = RPC_LCD_INIT;
	LIBLCD_MSG("vmfunc_init successfull! Calling LCD_INIT of dummy_lcd");
	vmfunc_klcd_wrapper(&m, OTHER_DOMAIN);

	return 0;

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
static void __exit net_klcd_exit(void)
{
	struct fipc_message m;

	LIBLCD_MSG("%s: exiting", __func__);

	INIT_FIPC_MSG(&m);

	/* call module_init for lcd */
	m.vmfunc_id = VMFUNC_RPC_CALL;
	m.rpc_id = RPC_LCD_EXIT;
	LIBLCD_MSG("Calling LCD_EXIT of dummy_lcd");
	vmfunc_klcd_wrapper(&m, OTHER_DOMAIN);

	/*
	 * IDL-SPECIFIC: hashmap teardown?
	 */

#ifndef CONFIG_LVD
	/*
	 * In case of LVD, boot module calls lcd_exit
	 */
	lcd_exit(0);
#endif
	return;
}

module_init(net_klcd_init);
module_exit(net_klcd_exit);
MODULE_LICENSE("GPL");

