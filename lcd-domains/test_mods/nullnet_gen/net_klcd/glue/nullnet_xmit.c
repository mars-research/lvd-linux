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

#define NUM_TRANSACTIONS	1000000
#define NUM_INNER_ASYNCS	2
#define NUM_CORES	32
#define NUM_THREADS	NUM_CORES

#if !defined(CONFIG_RUN_DUMMY_1) && !defined(CONFIG_RUN_DUMMY_2)
#error "Either RUN_DUMMY_1 or RUN_DUMMY_2 has to be defined"
#endif

#if defined(CONFIG_RUN_DUMMY_2)
#define HASHING
#endif

extern inline xmit_type_t check_skb_range(struct sk_buff *skb);
extern struct glue_cspace *c_cspace;

extern uint64_t *times_ndo_xmit[4];
extern u32 thread;
extern struct rtnl_link_stats64 g_stats;
extern priv_pool_t *pool;
extern struct kmem_cache *skb_c_cache;

/*
 * During ndo_start_xmit, we would insert the skb reference to this per-cpu
 * hash table.  When consume_skb is called, we lookp the skb_cptr on this
 * hashtable to retrieve the original skb pointer to be freed.
 */
DEFINE_PER_CPU(struct skb_hash_table, skb_hash);

int inline glue_insert_skb_hash(struct sk_buff_container_hash *skb_c)
{
	int cpu = smp_processor_id();
	struct skb_hash_table *this = &per_cpu(skb_hash, cpu);

	BUG_ON(!skb_c->skb);

	skb_c->my_ref = __cptr((unsigned long)skb_c->skb);

	hash_add(this->skb_table, &skb_c->hentry,
			(unsigned long) skb_c->skb);

	return 0;
}

int inline glue_lookup_skb_hash(struct cptr c, struct sk_buff_container_hash **skb_cout)
{
	int cpu = smp_processor_id();
	struct skb_hash_table *this = &per_cpu(skb_hash, cpu);
        struct sk_buff_container_hash *skb_c;

        hash_for_each_possible(this->skb_table, skb_c, hentry, (unsigned long) cptr_val(c)) {
		if (!skb_c) {
			WARN_ON(!skb_c);
			continue;
		}
		if (((unsigned long long) skb_c == 0xffffffffffffffd3ull) || (((unsigned long long)skb_c >> 48) != 0xffff)) {
			printk("%s, something is wrong here! cptr %lx | skb_table pcpu_ptr %p\n", __func__, c.cptr, this);
			goto skip;
		}

		if (skb_c->skb == (struct sk_buff*) c.cptr) {
	                *skb_cout = skb_c;
		}
        }
skip:
        return 0;
}

void inline glue_remove_skb_hash(struct sk_buff_container_hash *skb_c)
{
	hash_del(&skb_c->hentry);
}

unsigned long skb_containers[20];

#ifdef CONFIG_SKB_COPY
int __ndo_start_xmit_inner_async(struct sk_buff *skb,
		struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret = 0;
	struct ext_registers *ext_regs = get_register_page(smp_processor_id());
	u64 *regs = &ext_regs->regs[0];
	int i = 0;

#ifdef HASHING
	//struct sk_buff_container_hash static_skbc = {0};
	//struct sk_buff_container_hash *skb_c = &static_skbc;
	struct sk_buff_container_hash *skb_c;
#endif

#ifdef TIMESTAMP
	TS_DECL(mndo_xmit);
#endif

#if defined(TIMESTAMP) || defined(LCD_MEASUREMENT)
	static int iter = 0;
#endif
#ifdef TIMESTAMP
	TS_START(mndo_xmit);
#endif

	INIT_FIPC_MSG(&r);

	dev_container = container_of(dev,
			struct net_device_container,
			net_device);

#ifdef HASHING
#if 1
#ifdef SKBC_PRIVATE_POOL
	skb_c = priv_alloc(SKB_CONTAINER_POOL);
#else
	skb_c = kmem_cache_alloc(skb_c_cache, GFP_KERNEL);
#endif
	if (!skb_c) {
		LIBLCD_MSG("no memory");
		goto fail_alloc;
	}
	//memset(skb_c, 0x0, sizeof(*skb_c));
#endif
	skb_c->skb = skb;

	if (0)
		printk("%s, on cpu:%d skb:%p\n", __func__, smp_processor_id(), skb);
#endif
	/* pad to 17 bytes, don't care the ret val */
	skb_put_padto(skb, 17);
	async_msg_set_fn_type(_request, NDO_START_XMIT);

#ifdef HASHING
	glue_insert_skb_hash(skb_c);
	fipc_set_reg2(_request, skb_c->my_ref.cptr);
	//printk("%s, sending cptr %lx\n", __func__, skb_c->my_ref.cptr);
	//skb_containers[smp_processor_id()] = skb_c->my_ref.cptr;
#else
	fipc_set_reg0(_request, (unsigned long) skb);
#endif

	fipc_set_reg1(_request, dev_container->other_ref.cptr);

	fipc_set_reg3(_request, skb->len);
	fipc_set_reg4(_request, skb->end);
	fipc_set_reg5(_request, skb->protocol);

	SET_EREG(data_len);
	SET_EREG(queue_mapping);
	SET_EREG(xmit_more);
	SET_EREG(tail);
	SET_EREG(truesize);
	SET_EREG(ip_summed);
	SET_EREG(csum_start);
	SET_EREG(network_header);
	SET_EREG(csum_offset);
	SET_EREG(transport_header);
	regs[i++] = skb->data - skb->head;
	memcpy(&regs[i], skb->head, skb->len - skb->data_len + (skb->data -
				skb->head));
	if (0)
		printk("%s:%d %s skb->q %d\n", current->comm, current->pid, __func__, skb->queue_mapping);

	vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);
#ifdef LCD_MEASUREMENT
	times_lcd[iter] = fipc_get_reg2(_request);
	iter = (iter + 1) % NUM_PACKETS;
#endif

#ifdef TIMESTAMP
	TS_STOP(mndo_xmit);
	times_ndo_xmit[iter] = TS_DIFF(mndo_xmit);
	iter = (iter + 1) % NUM_PACKETS;
#endif
#ifdef HASHING
fail_alloc:
#endif
#ifdef CONFIG_RUN_DUMMY_1
	dev_kfree_skb(skb);
#endif
	if (func_ret == NETDEV_TX_BUSY) {
		dev_kfree_skb(skb);
	}
	return func_ret;
}
#else		/* CONFIG_SKB_COPY */
int __ndo_start_xmit_inner_async(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret = 0;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct sk_buff_container_hash static_skbc = {0};
	struct sk_buff_container_hash *skb_c = &static_skbc;
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

	skb_c->skb = skb;

#ifdef HASHING
	glue_insert_skb_hash(skb_c);
#endif

	/* inform LCD that it is async */
	fipc_set_reg0(_request, 1);

	fipc_set_reg1(_request,
			net_dev_container->other_ref.cptr);
#ifdef HASHING
	fipc_set_reg2(_request,
			skb_c->my_ref.cptr);
#else
	fipc_set_reg2(_request, (unsigned long) skb);
#endif
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

	if (0)
		printk("%s, sending packet\n", __func__);

	vmfunc_klcd_wrapper(_request, 1);

	ret = fipc_get_reg1(_request);

#ifdef CONFIG_RUN_DUMMY_1
	dev_kfree_skb(skb);
#endif
fail:
	return ret;
}
#endif		/* CONFIG_SKB_COPY */

/*
 * This function measures the overhead of bare fipc in KLCD/LCD setting
 */
int __ndo_start_xmit_bare_fipc_nomarshal(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	xmit_type_t xmit_type;
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

#ifdef TIMESTAMP
	_TS_START(xmit);
	for (i = 0; i < NUM_TRANSACTIONS; i++) {
#endif
	
		async_msg_set_fn_type(_request, NDO_START_XMIT);
		vmfunc_klcd_wrapper(_request, 1);

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
	struct sk_buff_container_hash static_skbc = {0};
	struct sk_buff_container_hash *skb_c = &static_skbc;
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

	ret = vmfunc_klcd_wrapper(_request, 1);

	ret = fipc_get_reg1(_request);
free:
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}

int consume_skb_callee(struct fipc_message *request)
{
	int ret = 0;
	struct sk_buff *skb;
#ifdef HASHING
	struct sk_buff_container_hash *skb_c = NULL;
	//printk("%s, my_ref %lx\n", __func__,
	//			fipc_get_reg0(request));
	/*if (skb_containers[smp_processor_id()] !=
			fipc_get_reg0(request)) {
		printk("%s, xmit cptr %lx, received cptr %lx\n",
			__func__, skb_containers[smp_processor_id()], fipc_get_reg0(request));
		goto skip;
	}*/
	glue_lookup_skb_hash(__cptr(fipc_get_reg0(request)),
			&skb_c);
	if (!((unsigned long long)skb_c & (0xffffull << 48))) {
		printk("%s, invalid cptr %p | reg0 %lx\n", __func__, skb_c, fipc_get_reg0(request));
		goto skip;
	}

	if (!skb_c) {
		printk("%s, skb_c null\n", __func__);
		goto skip;
	} else {
		skb = skb_c->skb;
	}

	WARN_ON(!skb);

	consume_skb(skb_c->skb);

	glue_remove_skb_hash(skb_c);

	kmem_cache_free(skb_c_cache, skb_c);
skip:
#else
	skb = (struct sk_buff*) fipc_get_reg0(request);
	consume_skb(skb);
#endif
	return ret;
}
