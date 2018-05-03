#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <thc.h>


#include "./ixgbe_callee.h"

/* COMPILER: This is always included after all includes. */
#include <lcd_config/post_hook.h>
/* LOOP ------------------------------------------------------------ */
struct net_info {
	struct thc_channel *chnl;
	struct glue_cspace *cspace;
	cptr_t sync_endpoint;
	struct list_head list;
};
static LIST_HEAD(net_infos);
struct thc_channel *xmit_chnl;
struct thc_channel *xmit_chnl2;
struct thc_channel *xmit_irq_chnl;
struct thc_channel *klcd_chnl;

extern int setup_async_net_ring_channel(cptr_t tx, cptr_t rx, 
				struct thc_channel **chnl_out);
extern void destroy_async_net_ring_channel(struct thc_channel *chnl);
extern int ixgbe_trigger_dump(struct thc_channel *_channel);
extern int ixgbe_service_event_sched(struct thc_channel *_channel);
extern int trigger_exit_to_lcd(struct thc_channel *_channel, enum dispatch_t);
extern struct timer_list service_timer;
extern struct glue_cspace *c_cspace;

/* mechanism for unloading LCD gracefully */
static bool unload_lcd =0;
static bool clean_up = false;
module_param_named(unload, unload_lcd, bool, S_IWUSR);

/* to dump ixgbe registers */
static bool ixgbe_dump =0;
module_param_named(dump_regs, ixgbe_dump, bool, S_IWUSR);
module_param_named(clean, clean_up, bool, S_IWUSR);

struct net_info *
add_net(struct thc_channel *chnl, struct glue_cspace *cspace,
	cptr_t sync_endpoint)
{
	struct net_info *net_info;

	net_info = kmalloc(sizeof(*net_info), GFP_KERNEL);
	if (!net_info)
		goto fail1;
	net_info->chnl = chnl;
	net_info->cspace = cspace;
	net_info->sync_endpoint = sync_endpoint;
	INIT_LIST_HEAD(&net_info->list);
	list_add(&net_info->list, &net_infos);

	return net_info;

fail1:
	return NULL;
}

void remove_net(struct net_info *net)
{
	list_del_init(&net->list);
	kfree(net);
}

static int __get_net(struct net_info **net_out)
{
	struct net_info *first;
	first = list_first_entry_or_null(&net_infos, struct net_info, list);
	if (first)
		*net_out = first;
	return first ? 1 : 0;
}

static int async_loop(struct net_info **net_out, struct fipc_message **msg_out)
{
	struct net_info *cursor, *next;
	int ret;

	list_for_each_entry_safe(cursor, next, &net_infos, list) {

		ret = thc_ipc_poll_recv(cursor->chnl, msg_out);
		if (ret == -EPIPE) {
			/*
			 * net channel is dead; free the channel,
			 * and remove from list
			 */
			kfree(cursor->chnl);
			remove_net(cursor);
		} else if (ret == -EWOULDBLOCK) {
			/*
			 * Skip over empty channels
			 */
			continue;
		} else if (ret) {
			/*
			 * Unexpected error
			 */
			LIBLCD_ERR("error ret = %d on async channel");
			return ret;
		} else {
			/*
			 * Got a msg
			 */
			*net_out = cursor;
			return 0;
		}

	}
	/*
	 * Nothing for us to recv right now
	 */
	return -EWOULDBLOCK;
}

#ifdef HOST_IRQ
extern struct napi_struct *napi_q0;

irqreturn_t msix_clean_rings_host(int irq, void *data)
{
	struct net_info *net;
	if (__get_net(&net)) {
		napi_schedule_irqoff(napi_q0);
	}
	return IRQ_HANDLED;
}
#endif

void ixgbe_service_timer(unsigned long data)
{
	unsigned long next_event_offset;
	struct net_info *net;

	next_event_offset = HZ * 2;

	/* Reset the timer */
	mod_timer(&service_timer, next_event_offset + jiffies);

	if (__get_net(&net)) {
		ixgbe_service_event_sched(net->chnl);
	}
}


/* LOOP -------------------------------------------------- */
static int do_one_register(cptr_t register_chnl)
{
	int ret;
	cptr_t sync_endpoint, tx, rx;
	cptr_t tx_xmit, rx_xmit;

	struct thc_channel *chnl;
	struct net_info *net_info;

	/*
	 * Set up cptrs
	 */
	ret = lcd_cptr_alloc(&sync_endpoint);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	ret = lcd_cptr_alloc(&tx);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail2;
	}
	ret = lcd_cptr_alloc(&rx);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail3;
	}
	ret = lcd_cptr_alloc(&tx_xmit);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail2;
	}
	ret = lcd_cptr_alloc(&rx_xmit);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail3;
	}
	
	/*
	 * Set up regs and poll
	 */
	lcd_set_cr0(sync_endpoint);
	lcd_set_cr1(tx);
	lcd_set_cr2(rx);
	lcd_set_cr3(tx_xmit);
	lcd_set_cr4(rx_xmit);

	ret = lcd_sync_poll_recv(register_chnl);
	if (ret) {
		if (ret == -EWOULDBLOCK)
			ret = 0;
		goto free_cptrs;
	}
#ifdef SYNC_TX
	/*
	 * Dispatch to register handler
	 */
	ret = dispatch_sync_loop();
	if (ret)
		return ret; /* dispatch fn is responsible for cptr cleanup */
#endif
	LIBLCD_MSG("settingup net_ring channel");
	/*
	 * Set up async ring channel
	 */
	ret = setup_async_net_ring_channel(tx, rx, &chnl);
	if (ret) {
		LIBLCD_ERR("error setting up ring channel");
		goto fail6;
	}

	klcd_chnl = chnl;

	LIBLCD_MSG("settingup xmit channel");
	/*
	 * Set up async ring channel
	 */
	ret = setup_async_net_ring_channel(tx_xmit, rx_xmit,
					&xmit_chnl);
	if (ret) {
		LIBLCD_ERR("error setting up ring channel");
		goto fail6;
	}

	/*
	 * Add to dispatch loop
	 */
	LIBLCD_MSG("Adding to fsinfo cspace %lu | chnl %p | sync_ep %lu", c_cspace, chnl, sync_endpoint);
 
	net_info = add_net(chnl, c_cspace, sync_endpoint);
	if (!net_info) {
		LIBLCD_ERR("error adding to dispatch loop");
		goto fail7;
	}

	LIBLCD_MSG("Returning from %s", __func__);
	lcd_set_cr0(CAP_CPTR_NULL);
	lcd_set_cr1(CAP_CPTR_NULL);
	lcd_set_cr2(CAP_CPTR_NULL);
	lcd_set_cr3(CAP_CPTR_NULL);
	lcd_set_cr4(CAP_CPTR_NULL);

	if (lcd_sync_reply())
		LIBLCD_ERR("Error reply");
	return 0;

fail6:
	destroy_async_net_ring_channel(chnl);
fail7:
free_cptrs:
	lcd_set_cr0(CAP_CPTR_NULL);
	lcd_set_cr1(CAP_CPTR_NULL);
	lcd_set_cr2(CAP_CPTR_NULL);
	lcd_set_cr3(CAP_CPTR_NULL);
	lcd_set_cr4(CAP_CPTR_NULL);
	lcd_cptr_free(sync_endpoint);
fail3:
	lcd_cptr_free(tx);
	lcd_cptr_free(tx_xmit);
fail2:
	lcd_cptr_free(rx);
fail1:
	return ret;
}
#define REGISTER_FREQ	50

static void loop(cptr_t register_chnl)
{
	unsigned long tics = jiffies + REGISTER_FREQ;
	struct fipc_message *msg;
	struct net_info *net;
	int stop = 0;
	int ret;
	bool reg_done = false;

	DO_FINISH(
	while (!stop) {
		if (jiffies >= tics && !reg_done) {
			/*
			 * Listen for a register call
			 */
			ret = do_one_register(register_chnl);
			if (ret) {
				LIBLCD_ERR("register error");
				break;
			} else {
				reg_done = true;
			}
			tics = jiffies + REGISTER_FREQ;
			continue;
		}
		if (stop)
			break;

		/*
		 * will be updated by a write into sysfs
		 * from userspace.
		 */
		if (unload_lcd || clean_up) {
			if (__get_net(&net)) {
				if (unload_lcd) {
					trigger_exit_to_lcd(net->chnl, TRIGGER_EXIT);
					unload_lcd ^= unload_lcd;
				}
				if (clean_up) {
					LIBLCD_MSG("cleanup triggered"); 
					trigger_exit_to_lcd(net->chnl, TRIGGER_CLEAN);
					clean_up ^= clean_up;
				}
			}
		}

		if (ixgbe_dump) {
			ixgbe_dump = 0;
			if (__get_net(&net))
				ixgbe_trigger_dump(net->chnl);
		}
		ret = async_loop(&net, &msg);
		if (!ret) {
			ASYNC(
				ret = dispatch_async_loop(
					net->chnl,
					msg,
					net->cspace,
					net->sync_endpoint);
				if (ret) {
					LIBLCD_ERR("net dispatch err");
					/* (break won't work here) */
					stop = 1;
				}
				);
		} else if (ret != -EWOULDBLOCK) {
			LIBLCD_ERR("async loop failed");
			stop = 1;
			break;
		}

		if (kthread_should_stop()) {
			LIBLCD_MSG("kthread should stop");
			stop = 1;
			break;
		}

#ifndef CONFIG_PREEMPT
		/*
		 * Play nice with the rest of the system
		 */
		cond_resched();
#endif
	}

		LIBLCD_MSG("net layer exited loop");

		//THCStopAllAwes();

	);

	LIBLCD_MSG("EXITED net_klcd DO_FINISH");
}

/* INIT / EXIT ---------------------------------------- */
struct cspace *klcd_cspace;
struct task_struct *task_klcd;

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
	task_klcd = current;
	/*
	 * Init net glue
	 */
	ret = glue_ixgbe_init();
	LIBLCD_MSG("-===== > glue ixgbe init called\n");
	if (ret) {
		LIBLCD_ERR("net init");
		goto fail3;
	}
	/*
	 * Enter sync/async dispatch loop
	 */
	LIBLCD_MSG(">>>>> Looping .... \n");
	loop(net_chnl);
	/*
	 * Tear down net glue
	 */
	glue_ixgbe_exit();

	lcd_exit(0);

	return 0;

fail3:
fail2:
	lcd_exit(ret);
fail1:
	return ret;
}

static int __net_klcd_init(void)
{
	int ret;
	LIBLCD_MSG("%s: entering", __func__);
	LCD_MAIN({

			ret = net_klcd_init();

		});
	return ret;
}

/*
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit net_klcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);
	return;
}

module_init(__net_klcd_init);
module_exit(net_klcd_exit);
MODULE_LICENSE("GPL");

