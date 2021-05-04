#include <lcd_config/pre_hook.h>

#include <linux/skbuff.h>
#include <liblcd/skbuff.h>
#include <linux/etherdevice.h>

#include <lcd_config/post_hook.h>

#if 1
void skb_add_rx_frag(struct sk_buff *skb, int i, struct page *page, int off,
		     int size, unsigned int truesize)
{
	skb_fill_page_desc(skb, i, page, off, size);
	skb->len += size;
	skb->data_len += size;
	skb->truesize += truesize;
}
EXPORT_SYMBOL(skb_add_rx_frag);

/**
 * eth_get_headlen - determine the length of header for an ethernet frame
 * @data: pointer to start of frame
 * @len: total length of frame
 *
 * Make a best effort attempt to pull the length for all of the headers for
 * a given frame in a linear buffer.
 */
u32 eth_get_headlen(void *data, unsigned int len)
{
	const unsigned int flags = FLOW_DISSECTOR_F_PARSE_1ST_FRAG;
	const struct ethhdr *eth = (const struct ethhdr *)data;
	struct flow_keys keys;

	/* this should never happen, but better safe than sorry */
	if (unlikely(len < sizeof(*eth)))
		return len;

	/* parse any remaining L2/L3 headers, check for L4 */
	if (!skb_flow_dissect_flow_keys_buf(&keys, data, eth->h_proto,
					    sizeof(*eth), len, flags))
		return max_t(u32, keys.control.thoff, sizeof(*eth));

	/* parse for any L4 headers */
	return min_t(u32, __skb_get_poff(NULL, data, &keys, len), len);
}
EXPORT_SYMBOL(eth_get_headlen);

/**
 * eth_type_trans - determine the packet's protocol ID.
 * @skb: received socket data
 * @dev: receiving network device
 *
 * The rule here is that we
 * assume 802.3 if the type field is short enough to be a length.
 * This is normal practice and works for any 'now in use' protocol.
 */
__be16 eth_type_trans(struct sk_buff *skb, struct net_device *dev)
{
#ifdef IPX_PACKETS
	unsigned short _service_access_point;
	const unsigned short *sap;
#endif
	const struct ethhdr *eth;
	static int rate = 0;

	skb->dev = dev;
	skb_reset_mac_header(skb);

	eth = (struct ethhdr *)skb->data;

	if (skb->len < skb->data_len) {
		if (!(rate++ % 1000))
		LIBLCD_ERR("len %d | data_len %d | frags %d I'll drop this packet",
				skb->len, skb->data_len,
				skb_shinfo(skb)->nr_frags);
		return 0x0;
	}
	skb_pull_inline(skb, ETH_HLEN);

	if (unlikely(is_multicast_ether_addr_64bits(eth->h_dest))) {
		if (ether_addr_equal_64bits(eth->h_dest, dev->broadcast))
			skb->pkt_type = PACKET_BROADCAST;
		else
			skb->pkt_type = PACKET_MULTICAST;
	}
	else if (unlikely(!ether_addr_equal_64bits(eth->h_dest,
						   dev->dev_addr)))
		skb->pkt_type = PACKET_OTHERHOST;

	/*
	 * Some variants of DSA tagging don't have an ethertype field
	 * at all, so we check here whether one of those tagging
	 * variants has been configured on the receiving interface,
	 * and if so, set skb->protocol without looking at the packet.
	 */
	if (unlikely(netdev_uses_dsa(dev)))
		return htons(ETH_P_XDSA);

	if (likely(eth_proto_is_802_3(eth->h_proto)))
		return eth->h_proto;

/* XXX: This is not required for normal mode. Let's remove this to avoid
  copying skb_copy_bits function into the liblcd.
 */
#ifdef IPX_PACKETS
	/*
	 *      This is a magic hack to spot IPX packets. Older Novell breaks
	 *      the protocol design and runs IPX over 802.3 without an 802.2 LLC
	 *      layer. We look for FFFF which isn't a used 802.2 SSAP/DSAP. This
	 *      won't work for fault tolerant netware but does for the rest.
	 */
	sap = skb_header_pointer(skb, 0, sizeof(*sap), &_service_access_point);
	if (sap && *sap == 0xFFFF)
		return htons(ETH_P_802_3);
#endif
	/*
	 *      Real 802.2 LLC
	 */
	return htons(ETH_P_802_2);
}
EXPORT_SYMBOL(eth_type_trans);

struct kmem_cache *skbuff_head_cache;
/**
 *	__alloc_skb	-	allocate a network buffer
 *	@size: size to allocate
 *	@gfp_mask: allocation mask
 *	@flags: If SKB_ALLOC_FCLONE is set, allocate from fclone cache
 *		instead of head cache and allocate a cloned (child) skb.
 *		If SKB_ALLOC_RX is set, __GFP_MEMALLOC will be used for
 *		allocations in case the data is required for writeback
 *	@node: numa node to allocate memory on
 *
 *	Allocate a new &sk_buff. The returned buffer has no headroom and a
 *	tail room of at least size bytes. The object has a reference count
 *	of one. The return is the buffer. On a failure the return is %NULL.
 *
 *	Buffers may only be allocated from interrupts using a @gfp_mask of
 *	%GFP_ATOMIC.
 */
struct sk_buff *__alloc_skb(unsigned int size, gfp_t gfp_mask,
			    int flags, int node)
{
	struct kmem_cache *cache;
	struct skb_shared_info *shinfo;
	struct sk_buff *skb = NULL;
	struct sk_buff_container *skb_c;
	u8 *data;

	cache = skbuff_head_cache;

	/* Get the HEAD */
	skb_c = kmem_cache_alloc_node(cache, gfp_mask & ~__GFP_DMA, node);

	if (!skb_c)
		goto out;
	memset(skb_c, 0x0, sizeof(*skb_c));
	prefetchw(skb_c);

	skb = &skb_c->skb;
	/* We do our best to align skb_shared_info on a separate cache
	 * line. It usually works because kmalloc(X > SMP_CACHE_BYTES) gives
	 * aligned memory blocks, unless SLUB/SLAB debug is enabled.
	 * Both skb->head and skb_shared_info are cache line aligned.
	 */

	size = SKB_DATA_ALIGN(size);
	size += SKB_DATA_ALIGN(sizeof(struct skb_shared_info));

	data = kzalloc(size, gfp_mask);

	if (!data)
		goto nodata;

	/* kmalloc(size) might give us more room than requested.
	 * Put skb_shared_info exactly at the end of allocated zone,
	 * to allow max possible filling before reallocation.
	 */
	size = SKB_WITH_OVERHEAD(ksize(data));
	prefetchw(data + size);


	/*
	 * Only clear those fields we need to clear, not those that we will
	 * actually initialise below. Hence, don't put any more fields after
	 * the tail pointer in struct sk_buff!
	 */
	memset(skb, 0, offsetof(struct sk_buff, tail));
	/* Account for allocated memory : skb + skb->head */
	skb->truesize = SKB_TRUESIZE(size);
	atomic_set(&skb->users, 1);
	skb->head = data;
	skb->data = data;
	skb_reset_tail_pointer(skb);
	skb->end = skb->tail + size;
	skb->mac_header = (typeof(skb->mac_header))~0U;
	skb->transport_header = (typeof(skb->transport_header))~0U;

	/* make sure we initialize shinfo sequentially */
	shinfo = skb_shinfo(skb);
	memset(shinfo, 0, offsetof(struct skb_shared_info, dataref));
	atomic_set(&shinfo->dataref, 1);

out:
	return skb;
nodata:
	kmem_cache_free(cache, skb);
	skb = NULL;
	goto out;
}
EXPORT_SYMBOL(__alloc_skb);

void skb_init(void)
{
	skbuff_head_cache = kmem_cache_create("skbuff_head_cache",
					      sizeof(struct sk_buff),
					      0,
					      SLAB_HWCACHE_ALIGN|SLAB_PANIC,
					      NULL);
}

static void skb_free_head(struct sk_buff *skb)
{
	unsigned char *head = skb->head;

	if (skb->head_frag)
		skb_free_frag(head);
	else {
		kfree(head);
	}
}

static void skb_release_data(struct sk_buff *skb)
{
	struct skb_shared_info *shinfo = skb_shinfo(skb);
	int i;

	for (i = 0; i < shinfo->nr_frags; i++)
		__skb_frag_unref(&shinfo->frags[i]);

	if (shinfo->frag_list)
		kfree_skb_list(shinfo->frag_list);

	skb_free_head(skb);
}

/*
 *	Free an skbuff by memory without cleaning the state.
 */
static void kfree_skbmem(struct sk_buff *skb)
{
	kmem_cache_free(skbuff_head_cache, skb);
	return;
}

/* Free everything but the sk_buff shell. */
static void skb_release_all(struct sk_buff *skb)
{
	if (likely(skb->head))
		skb_release_data(skb);
}

/**
 *	__kfree_skb - private function
 *	@skb: buffer
 *
 *	Free an sk_buff. Release anything attached to the buffer.
 *	Clean the state. This is an internal helper function. Users should
 *	always call kfree_skb
 */

void __lcd_kfree_skb(struct sk_buff *skb)
{
	skb_release_all(skb);
	kfree_skbmem(skb);
}
EXPORT_SYMBOL(__lcd_kfree_skb);

/**
 *	kfree_skb - free an sk_buff
 *	@skb: buffer to free
 *
 *	Drop a reference to the buffer and free it if the usage count has
 *	hit zero.
 */
void lcd_kfree_skb(struct sk_buff *skb)
{
	if (unlikely(!skb))
		return;
	if (likely(atomic_read(&skb->users) == 1))
		smp_rmb();
	else if (likely(!atomic_dec_and_test(&skb->users)))
		return;
	__lcd_kfree_skb(skb);
}
EXPORT_SYMBOL(lcd_kfree_skb);


/**
 *	consume_skb - free an skbuff
 *	@skb: buffer to free
 *
 *	Drop a ref to the buffer and free it if the usage count has hit zero
 *	Functions identically to kfree_skb, but kfree_skb assumes that the frame
 *	is being dropped after a failure and notes that
 */
void lcd_consume_skb(struct sk_buff *skb)
{
	if (unlikely(!skb))
		return;
	if (likely(atomic_read(&skb->users) == 1))
		smp_rmb();
	else if (likely(!atomic_dec_and_test(&skb->users)))
		return;
	__lcd_kfree_skb(skb);
}
EXPORT_SYMBOL(lcd_consume_skb);
#endif
