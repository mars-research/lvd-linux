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
extern int ixgbe_service_event_sched(void);
extern int trigger_exit_to_lcd(enum dispatch_t);
extern struct timer_list service_timer;
extern struct glue_cspace *c_cspace;
extern void *lcd_stack;

/* mechanism for unloading LCD gracefully */
static bool unload_lcd =0;
static bool clean_up = false;
module_param_named(unload, unload_lcd, bool, S_IWUSR);

/* to dump ixgbe registers */
static bool ixgbe_dump =0;
module_param_named(dump_regs, ixgbe_dump, bool, S_IWUSR);
module_param_named(clean, clean_up, bool, S_IWUSR);


int net_klcd_dispatch_async_loop(struct fipc_message *message);
int net_klcd_syncipc_dispatch(struct fipc_message *message);

#ifdef HOST_IRQ
extern struct napi_struct *napi_q0;

irqreturn_t msix_clean_rings_host(int irq, void *data)
{
	napi_schedule_irqoff(napi_q0);
	return IRQ_HANDLED;
}
#endif

void ixgbe_service_timer(unsigned long data)
{
	unsigned long next_event_offset;

	next_event_offset = HZ * 2;

	/* Reset the timer */
	mod_timer(&service_timer, next_event_offset + jiffies);

	ixgbe_service_event_sched();
}

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
	/*
	 * there is no separate LCD for KLCD module, we operate in the context
	 * of boot thread
	 */
#if 0
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
	vmfunc_klcd_test_wrapper(&m, OTHER_DOMAIN, VMFUNC_TEST_RPC_CALL);

	return 0;

fail2:
	lcd_exit(ret);
	return ret;
}

static int __net_klcd_init(void)
{
	LIBLCD_MSG("%s: entering", __func__);
	return net_klcd_init();
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
	glue_ixgbe_exit();

	lcd_exit(0);

	return;
}

module_init(__net_klcd_init);
module_exit(net_klcd_exit);
MODULE_LICENSE("GPL");

