#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/trampoline.h>
#include <linux/hashtable.h>
#include <asm/cacheflush.h>
#include <linux/vmalloc.h>

#include "../../glue_helper.h"
#include "../nullnet_callee.h"
#include "../../rdtsc_helper.h"
#include "../../perf_counter_helper.h"
#include "../../ipc_helper.h"

#include <lcd_config/post_hook.h>

struct thc_channel *sirq_channels[4];

#define NUM_TRANSACTIONS	1000000
#define NUM_INNER_ASYNCS	2
#define NUM_CORES	32
#define NUM_THREADS	NUM_CORES

extern inline xmit_type_t check_skb_range(struct sk_buff *skb);
extern struct glue_cspace *c_cspace;
extern struct cptr sync_ep;

extern uint64_t *times_ndo_xmit[4];
extern u32 thread;
extern struct ptstate_t *ptrs[NUM_THREADS];
extern struct rtnl_link_stats64 g_stats;
extern struct thc_channel *xmit_chnl;
extern priv_pool_t *pool;

/*
 * setup a new channel for the first time when an application thread
 * wishes to send a packet through this interface
 */
int setup_once(struct trampoline_hidden_args *hidden_args)
{
	if(!strncmp(current->comm, "iperf",
				strlen("iperf")) ||
		!strncmp(current->comm, "netperf",
				strlen("netperf"))) {

		printk("===================================\n");
		printk("===== Private Channel created =====\n");
		printk("===================================\n");
		if (thread < 4) {
			current->ptstate->times_ndo_xmit = times_ndo_xmit[thread++];
			printk("Assigning %p to pid %d\n", times_ndo_xmit[thread-1], current->pid);
		}
#ifndef NO_HASHING
#ifdef DOUBLE_HASHING
		spin_lock_init(&current->ptstate->hash_lock);
		hash_init(current->ptstate->cptr_table);

		// Add it once per thread
		glue_insert_tid(tid_table, current->ptstate);
		printk("Adding pts: %p and pid: %llu to the tid_table\n", current->ptstate, current->ptstate->pid);
#endif
#endif /* NO_HASHING */
	}
	return 0;
}

int __ndo_start_xmit_inner_async(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret = 0;
	struct fipc_message r;
	struct fipc_message *_request = &r;
#ifdef COPY
	struct skbuff_members *skb_lcd;
#endif

	struct net_device_container *net_dev_container;
	xmit_type_t xmit_type;

	xmit_type = check_skb_range(skb);

	if (xmit_type == VOLUNTEER_XMIT) {
		printk("%s, skb->proto %02X | len %d\n",
				__func__, ntohs(skb->protocol),
				skb->len);
		goto fail;
	}

	net_dev_container = container_of(dev,
			struct net_device_container, net_device);

	async_msg_set_fn_type(_request, NDO_START_XMIT);

	/* inform LCD that it is async */
	fipc_set_reg0(_request, 1);

	fipc_set_reg1(_request,
			net_dev_container->other_ref.cptr);

	fipc_set_reg3(_request,
			(unsigned long)
			((void*)skb->head - pool->pool));

	fipc_set_reg4(_request, skb->end);
	fipc_set_reg5(_request, skb->protocol);
	fipc_set_reg6(_request, skb->len);

#ifdef COPY
	skb_lcd = SKB_LCD_MEMBERS(skb);
	C(len);
	C(data_len);
	C(queue_mapping);
	C(xmit_more);
	C(tail);
	C(truesize);
	C(ip_summed);
	C(csum_start);
	C(network_header);
	C(csum_offset);
	C(transport_header);
	skb_lcd->head_data_off = skb->data - skb->head;
#endif

	ret = vmfunc_wrapper(_request);
	ret = fipc_get_reg1(_request);

fail:
	return ret;
}

/*
 * This function measures the overhead of bare fipc in KLCD/LCD setting
 */
int __ndo_start_xmit_bare_fipc_nomarshal(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	xmit_type_t xmit_type;
	struct thc_channel *async_chnl;
#ifdef TIMESTAMP
	_TS_START(xmit);
#endif

	xmit_type = check_skb_range(skb);

	if (unlikely(xmit_type == VOLUNTEER_XMIT)) {
		printk("%s, skb->proto %02X | len %d\n",
				__func__, ntohs(skb->protocol),
				skb->len);
		goto free;
	}

	if (unlikely(!current->ptstate)) {
		if (setup_once(hidden_args))
			goto free;
	}

	async_chnl = PTS()->thc_chnl;

#ifdef TIMESTAMP
	_TS_START(xmit);
	for (i = 0; i < NUM_TRANSACTIONS; i++) {
#endif
	
		async_msg_set_fn_type(_request, NDO_START_XMIT);
		vmfunc_wrapper(_request);

#ifdef TIMESTAMP
	}
	_TS_STOP(xmit);
	
	if (PTS()->iter++ == NUM_PACKETS) {
		PTS()->iter = 0;
		printk("-------------------------------------------------\n");
		fipc_test_stat_print_info(PTS()->times_ndo_xmit,
				NUM_PACKETS);
		memset(PTS()->times_ndo_xmit, 0x00, NUM_PACKETS);	
		printk("-------------------------------------------------\n");
	}
	PTS()->times_ndo_xmit[PTS()->iter]
			= _TS_DIFF(xmit) - CORRECTION_VALUE;
#endif
free:
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}

/* This function does exactly what the native xmit does
 * this serves as our base. If we use this function for isolated xmit,
 * we should ideally get the same bandwidth as the native dummy driver
 */
int __ndo_start_xmit_dummy(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}

/* This function should be called when there is no async involved in the send. This means, we call this
 * for packet sizes that are mtu and less.
 * If we use this function, we should ideally see a overhead of a bare_fipc_ping_pong, as we do a
 * blocking send and blocking receive.
 */
int ndo_start_xmit_noasync(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	xmit_type_t xmit_type;
	struct net_device_container *net_dev_container;
	struct sk_buff_container static_skbc = {0};
	struct sk_buff_container *skb_c = &static_skbc;
	int ret;

	net_dev_container = container_of(dev,
			struct net_device_container, net_device);

	skb_c->skb = skb;

	xmit_type = check_skb_range(skb);

	if (xmit_type == VOLUNTEER_XMIT) {
		printk("%s, skb->proto %02X | len %d\n",
				__func__, ntohs(skb->protocol),
				skb->len);
		goto free;
	}

	/* setup once for this thread */
	if (unlikely(!current->ptstate)) {
		if (setup_once(hidden_args))
			goto free;
	}

	async_msg_set_fn_type(_request, NDO_START_XMIT);

	/* chain skb or not */
	fipc_set_reg0(_request, false);

	fipc_set_reg1(_request,
			net_dev_container->other_ref.cptr);
	fipc_set_reg2(_request,
			skb_c->my_ref.cptr);

	fipc_set_reg3(_request,
			(unsigned long)
			((void*)skb->head - pool->pool));

	fipc_set_reg4(_request, skb->end);
	fipc_set_reg5(_request, skb->protocol);
	fipc_set_reg6(_request, skb->len);

	ret = vmfunc_wrapper(_request);

	ret = fipc_get_reg1(_request);
free:
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}

int ndo_start_xmit_async_landing(struct sk_buff *first, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	struct sk_buff *skb = first;
	int rc = NETDEV_TX_OK;

	if (!skb->next)
		skb->chain_skb = false;

	if (!skb->chain_skb)
		return ndo_start_xmit_noasync(skb, dev, hidden_args);

	return rc;
}
