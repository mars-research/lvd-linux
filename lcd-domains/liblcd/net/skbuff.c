#include <lcd_config/pre_hook.h>

#include <linux/skbuff.h>
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
	unsigned short _service_access_point;
	const unsigned short *sap;
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

	/*
	 *      This is a magic hack to spot IPX packets. Older Novell breaks
	 *      the protocol design and runs IPX over 802.3 without an 802.2 LLC
	 *      layer. We look for FFFF which isn't a used 802.2 SSAP/DSAP. This
	 *      won't work for fault tolerant netware but does for the rest.
	 */
	sap = skb_header_pointer(skb, 0, sizeof(*sap), &_service_access_point);
	if (sap && *sap == 0xFFFF)
		return htons(ETH_P_802_3);

	/*
	 *      Real 802.2 LLC
	 */
	return htons(ETH_P_802_2);
}
EXPORT_SYMBOL(eth_type_trans);
#endif
