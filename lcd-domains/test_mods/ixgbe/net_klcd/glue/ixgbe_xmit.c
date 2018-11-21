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

int prep_channel(struct trampoline_hidden_args *hidden_args);
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
	printk("%s, %s:%d lcdenter\n", __func__,
			current->comm, current->pid);

	/* step 1. create lcd env */
	lcd_enter();

	if (!PTS()) {
		printk("%s, %s:%d unable to create pts\n", __func__,
			current->comm, current->pid);
		return 1;
	}

	ptrs[smp_processor_id()] = PTS();

	/* set nonlcd ctx for future use */
	PTS()->nonlcd_ctx = true;

	PTS()->pid = current->pid;

	/*
	 * if it is ksoftirqd, let it use the channel that exist 
	 */
	if (!strncmp(current->comm, "ksoftirqd/",
				strlen("ksoftirqd/"))) {
		if (!sirq_channels[smp_processor_id()]) {
			printk("%s: sirqch empty for %d\n",
				__func__, smp_processor_id());
			PTS()->thc_chnl = xmit_irq_channel;
		}
		PTS()->thc_chnl =
			sirq_channels[smp_processor_id()];
		printk("[%d]%s[pid=%d] pts %p softirq channel %p\n",
				smp_processor_id(), current->comm,
				current->pid, PTS(),
				sirq_channels[smp_processor_id()]);

	} else if(!strncmp(current->comm, "iperf",
				strlen("iperf")) ||
		!strncmp(current->comm, "netperf",
				strlen("netperf"))) {

		printk("[%d]%s[pid=%d] calling prep_channel\n",
				smp_processor_id(), current->comm,
				current->pid);
		spin_lock(&prep_lock);
		prep_channel(hidden_args);
		printk("===================================\n");
		printk("===== Private Channel created =====\n");
		printk("===================================\n");

	} else {
		printk("===== app %s , giving xmit_chnl\n",
				current->comm);
		PTS()->thc_chnl = xmit_chnl;
		PTS()->dofin = true;
	}
	return 0;
}


/* sender side dispatch loop callback - called by thc_ipc_recv_req_resp */
int sender_dispatch(struct thc_channel *chnl, struct fipc_message *out, void *arg)
{
	/* we receive the skb pointer via arg, pass it to consume skb via reg 0 */
	fipc_set_reg0(out, (uint64_t) arg);
	return dispatch_async_loop(chnl, out, c_cspace, sync_ep); 
}

int __ndo_start_xmit_inner_async(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
#ifdef COPY
	struct skbuff_members *skb_lcd;
#endif
	unsigned int request_cookie;

	struct net_device_container *net_dev_container;
	struct sk_buff_container static_skbc;
	struct sk_buff_container *skb_c = &static_skbc;
	struct thc_channel *async_chnl = NULL;
	xmit_type_t xmit_type;

	xmit_type = check_skb_range(skb);

	if (xmit_type == VOLUNTEER_XMIT) {
		printk("%s, skb->proto %02X | len %d\n",
				__func__, ntohs(skb->protocol),
				skb->len);
		goto fail;
	}
	async_chnl = (struct thc_channel*) PTS()->thc_chnl;

	net_dev_container = container_of(dev,
			struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(
			async_chnl, &_request);

	if (unlikely(ret)) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(_request, NDO_START_XMIT);

	/* inform LCD that it is async */
	fipc_set_reg0(_request, 1);

	fipc_set_reg1(_request,
			net_dev_container->other_ref.cptr);

	/* we were sending skb ref cptr before. Now it is empty */
	fipc_set_reg2(_request,
			skb_c->my_ref.cptr);

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

	ret = thc_ipc_send_request(async_chnl, _request, &request_cookie);

	ret = thc_ipc_recv_response_inline(async_chnl, request_cookie,
				&_response);

	awe_mapper_remove_id(request_cookie);

	if (unlikely(ret)) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(
			async_chnl), _response);
fail:
fail_async:
fail_ipc:
	return ret;
}

int __ndo_start_xmit_bare_async(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	xmit_type_t xmit_type;
	_TS_DECL(xmit);
	u32 i;

	xmit_type = check_skb_range(skb);

	if (xmit_type == VOLUNTEER_XMIT) {
		printk("%s, skb->proto %02X | len %d\n",
				__func__, ntohs(skb->protocol),
				skb->len);
		goto free;
	}

	if (unlikely(!current->ptstate)) {
		if (setup_once(hidden_args))
			goto free;
	}

	/* 
	 * doesn't free the packet NUM_TRANSACTIONS times
	 * frees the packet only once
	 */
	_TS_START(xmit);
	for (i = 0; i < NUM_TRANSACTIONS; i++) {
		int j;
		DO_FINISH({
			for (j = 0; j < NUM_INNER_ASYNCS; j++) {
			ASYNC({
				__ndo_start_xmit_inner_async(skb, dev, hidden_args);
			}); //async
		}
		}); // dofinish
	}
	_TS_STOP(xmit);

	printk("%s, do_finish{async()}; %d transactions took %llu\n", __func__,
			NUM_TRANSACTIONS, _TS_DIFF(xmit)/NUM_TRANSACTIONS);
free:
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}

/*
 * This function measures the overhead of bare fipc in KLCD/LCD setting
 */
int __ndo_start_xmit_bare_fipc_nomarshal(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message *_request;
	struct fipc_message *_response;
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
		async_msg_blocking_send_start(
				async_chnl, &_request);

		async_msg_set_fn_type(_request, NDO_START_XMIT);

		thc_set_msg_type(_request, msg_type_request);

		fipc_send_msg_end(thc_channel_to_fipc(
				async_chnl), _request);

		/* guard nonlcd case with all macros */
		fipc_test_blocking_recv_start(
				async_chnl,
				&_response);

		fipc_recv_msg_end(thc_channel_to_fipc(
				async_chnl), _response);
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
	struct fipc_message *_request;
#ifdef SENDER_DISPATCH_LOOP
	struct fipc_message *_request1;
#endif
	struct fipc_message *_response;
	xmit_type_t xmit_type;
	struct thc_channel *async_chnl;
	struct net_device_container *net_dev_container;
	struct sk_buff_container static_skbc;
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

	/* get the async channel */
	async_chnl = PTS()->thc_chnl;


	async_msg_blocking_send_start(
			async_chnl, &_request);

	async_msg_set_fn_type(_request, NDO_START_XMIT);

	thc_set_msg_type(_request, msg_type_request);

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

	fipc_send_msg_end(thc_channel_to_fipc(
			async_chnl), _request);

#ifdef SENDER_DISPATCH_LOOP
	/* to receive consume_skb */
	fipc_test_blocking_recv_start(
			async_chnl,
			&_request1);

	/* TODO: replace this with a valid identifier mechanism
	 * e.g., bitmap, create a bitmap and store this skb pointer and send the bitnumber
	 * across the domain. during the consume_skb call, the same number is sent back and
	 * looked up here by the consume skb function to get the corresponding skb pointer.
	 * but this below mechanism is no way inferior to bitmap, except that the bitmap
	 * mechanism is more generalizable.
	 */
	fipc_set_reg0(_request1, (uint64_t) skb);

	/* call consume_skb */
	dispatch_async_loop(async_chnl, _request1, hidden_args->cspace,
				hidden_args->sync_ep);
#endif

	/* guard nonlcd case with all macros */
	fipc_test_blocking_recv_start(
			async_chnl,
			&_response);

	fipc_recv_msg_end(thc_channel_to_fipc(
			async_chnl), _response);

	ret = fipc_get_reg1(_response);
free:
#ifndef SENDER_DISPATCH_LOOP
	dev_kfree_skb(skb);
#endif
	return NETDEV_TX_OK;
}

/*
 * This function gets called when there is a chained skb in flight. For packet sizes > mtu, skbs are chained
 * at the IP layer if NETIF_CHAIN_SKB feature is enabled in the driver
 */
int ndo_start_xmit_async(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	struct net_device_container *net_dev_container;
	xmit_type_t xmit_type;
#ifdef COPY
	struct skbuff_members *skb_lcd;
#endif
	unsigned int request_cookie;
	struct sk_buff_container static_skbc;
	struct sk_buff_container *skb_c = &static_skbc;
	struct thc_channel *async_chnl = NULL;

	xmit_type = check_skb_range(skb);

	if (xmit_type == VOLUNTEER_XMIT) {
		printk("%s, skb->proto %02X | len %d\n",
				__func__, ntohs(skb->protocol),
				skb->len);
		goto free;
	}

	async_chnl = (struct thc_channel*) PTS()->thc_chnl;

	net_dev_container = container_of(dev,
			struct net_device_container, net_device);

	skb_c->skb = skb;

	ret = async_msg_blocking_send_start(
			async_chnl, &_request);
	if (unlikely(ret)) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(_request, NDO_START_XMIT);

	/* inform LCD that we are sending a chained skb */
	fipc_set_reg0(_request, skb->chain_skb);

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

	ret = thc_ipc_send_request(async_chnl, _request, &request_cookie);

#ifdef SENDER_DISPATCH_LOOP

	ret = thc_ipc_recv_req_resp(async_chnl, &_response, request_cookie, sender_dispatch, (void*)skb);

	fipc_recv_msg_end(thc_channel_to_fipc(async_chnl), _response);

#else
	ret = thc_ipc_recv_response_inline(async_chnl, request_cookie,
				&_response);

	awe_mapper_remove_id(request_cookie);
#endif
	if (unlikely(ret)) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	ret = fipc_get_reg1(_response);

	fipc_recv_msg_end(thc_channel_to_fipc(
			async_chnl), _response);

free:
#ifndef SENDER_DISPATCH_LOOP
	dev_kfree_skb(skb);
#endif
	g_stats.tx_packets += 1;
	g_stats.tx_bytes += skb->len;

fail_async:
fail_ipc:
	return ret;
}

int ndo_start_xmit_async_landing(struct sk_buff *first, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	struct sk_buff *skb = first;
	int rc = NETDEV_TX_OK;

	if (!skb->next)
		skb->chain_skb = false;

	if (!skb->chain_skb)
		return ndo_start_xmit_noasync(skb, dev, hidden_args);

	/* chain skb */
	if (unlikely(!current->ptstate)) {
		if (setup_once(hidden_args))
			goto free;
	}

	DO_FINISH_(dev_hard_xmit, {
		while (skb) {
			struct sk_buff *next = skb->next;	
			skb->next = NULL;
			ASYNC_({
				skb->chain_skb = true;
				rc = ndo_start_xmit_async(skb, dev, hidden_args);
				if (unlikely(!dev_xmit_complete(rc))) {
					skb->next = next;
					printk("%s, xmit failed\n", __func__);
					/* XXX: continue for now */
					continue;
				}	
			}, before_xmit); // ASYNC
			skb = next;
		}
	});
	return rc;
free:
	dev_kfree_skb(skb);
	return rc;
}

