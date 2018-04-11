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
//#define PERF_EVENTS
#define NUM_PACKETS	10000000
/* mechanism for unloading LCD gracefully */
static bool unload_lcd =0;
module_param_named(unload, unload_lcd, bool, S_IWUSR);

bool tdiff_valid = false;
u64 tdiff_disp = 0ull;
_TS_DECL(disp_loop);
/* LOOP ------------------------------------------------------------ */
struct net_info {
	struct thc_channel *chnl;
	struct glue_cspace *cspace;
	cptr_t sync_endpoint;
	struct list_head list;
};
static LIST_HEAD(net_infos);

extern int trigger_exit_to_lcd(struct thc_channel *_channel);

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

static int async_loop(struct net_info **fs_out, struct fipc_message **msg_out)
{
	struct net_info *cursor, *next;
	int ret;

	list_for_each_entry_safe(cursor, next, &net_infos, list) {

		ret = thc_ipc_poll_recv(cursor->chnl, msg_out);
		if (ret == -EPIPE) {
			/*
			 * fs channel is dead; free the channel,
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
			*fs_out = cursor;
			return 0;
		}

	}
	/*
	 * Nothing for us to recv right now
	 */
	return -EWOULDBLOCK;
}

/* LOOP -------------------------------------------------- */
static int do_one_register(cptr_t register_chnl)
{
	int ret;
	cptr_t sync_endpoint, tx, rx;
	cptr_t tx_xmit, rx_xmit;
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
	/*
	 * Dispatch to register handler
	 */
	ret = dispatch_sync_loop();
	if (ret)
		return ret; /* dispatch fn is responsible for cptr cleanup */

	return 0;

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
	lcd_cptr_free(rx_xmit);
fail1:
	return ret;
}
#define REGISTER_FREQ	50
extern uint64_t *perf1, *perf2, *perf3, *perf4;
static void loop(cptr_t register_chnl)
{
	unsigned long tics = jiffies + REGISTER_FREQ;
	struct fipc_message *msg;
	struct net_info *net;
	int stop = 0;
	int ret;
	u64 count = 0;

#ifdef PERF_EVENTS
	static int piter = 0;
	DECL_EVENT(e1); DECL_EVENT(e2);
	DECL_EVENT(e3); DECL_EVENT(e4);
	FILL_EVENT_OS(&e1, 0x26, 0x01);
	FILL_EVENT_OS(&e2, 0x26, 0x02);
	FILL_EVENT_OS(&e3, 0x26, 0x04);
	FILL_EVENT_OS(&e4, 0x26, 0x08);
#endif

	DO_FINISH(

		while (!stop) {
			//LIBLCD_MSG(" tics %lu\n", tics);
			if (jiffies >= tics) {
				/*
				 * Listen for a register call
				 */
				ret = do_one_register(register_chnl);
				if (ret) {
					LIBLCD_ERR("register error");
					break;
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
			if (unload_lcd) {
				unload_lcd = 0;
				if (__get_net(&net))
					trigger_exit_to_lcd(net->chnl);
			}
	//		_TS_START(disp_loop);
#ifdef PERF_EVENTS
	STOP_EVENT(EVENT_SEL3);
	STOP_EVENT(EVENT_SEL2);
	STOP_EVENT(EVENT_SEL1);
	STOP_EVENT(EVENT_SEL0);
	RESET_COUNTER(EVENT_SEL0);
	RESET_COUNTER(EVENT_SEL1);
	RESET_COUNTER(EVENT_SEL2);
	RESET_COUNTER(EVENT_SEL3);

	PROG_EVENT(&e1, EVENT_SEL0);
	PROG_EVENT(&e2, EVENT_SEL1);
	PROG_EVENT(&e3, EVENT_SEL2);
	PROG_EVENT(&e4, EVENT_SEL3);
#endif

			ret = async_loop(&net, &msg);
			if (!ret) {
			/*if (async_msg_get_fn_type(msg) ==
					CONSUME_SKB) {
				_TS_STOP(disp_loop);
				tdiff_disp = _TS_DIFF(disp_loop);
#ifdef PERF_EVENTS
	STOP_EVENT(EVENT_SEL3);
	STOP_EVENT(EVENT_SEL2);
	STOP_EVENT(EVENT_SEL1);
	STOP_EVENT(EVENT_SEL0);
	READ_PMC(&VAL(e1), EVENT_SEL0);
	READ_PMC(&VAL(e2), EVENT_SEL1);
	READ_PMC(&VAL(e3), EVENT_SEL2);
	READ_PMC(&VAL(e4), EVENT_SEL3);
	RESET_COUNTER(EVENT_SEL0);
	RESET_COUNTER(EVENT_SEL1);
	RESET_COUNTER(EVENT_SEL2);
	RESET_COUNTER(EVENT_SEL3);
	perf1[piter] = VAL(e1);
	perf2[piter] = VAL(e2);
	perf3[piter] = VAL(e3);
	perf4[piter] = VAL(e4);
	piter = (piter + 1) % NUM_PACKETS;
#endif
				tdiff_valid = true;
			}
*/
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
			cpu_relax();
			/*
			 * Play nice with the rest of the system
			 */
			if ((count++ % 65536) == 0)
				cond_resched();
#endif
		}

		LIBLCD_MSG("net layer exited loop");

		THCStopAllAwes();

		);

	LIBLCD_MSG("EXITED DUMMY DO_FINISH");
}

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

	/*
	 * Enter sync/async dispatch loop
	 */
	LIBLCD_MSG(">>>>> Looping .... \n");
	loop(net_chnl);
	/*
	 * Tear down net glue
	 */
	glue_nullnet_exit();

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

