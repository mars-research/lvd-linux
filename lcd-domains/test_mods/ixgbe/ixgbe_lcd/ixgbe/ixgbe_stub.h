#ifndef IXGBE_STUB_H
#define IXGBE_STUB_H

void napi_disable(struct napi_struct *n) {

}

void napi_complete_done(struct napi_struct *n, int work_done)
{
}

void pci_disable_msix(struct pci_dev *dev) { }

int pci_enable_msi_range(struct pci_dev *dev, int minvec,
				       int maxvec)
{ return -ENOSYS; }

void pci_disable_msi(struct pci_dev *dev) { }

int __must_check
request_threaded_irq(unsigned int irq, irq_handler_t handler,
		     irq_handler_t thread_fn,
		     unsigned long flags, const char *name, void *dev)
{
	return 0;
}

int call_netdevice_notifiers(unsigned long val, struct net_device *dev)
{
	return 0;
}
bool cancel_work_sync(struct work_struct *work)
{
	return true;
}
int rtnl_is_locked(void)
{
	return 0;
}
void usleep_range(unsigned long min, unsigned long max)
{
}

void synchronize_irq(unsigned int irq) {}

int mdio45_probe(struct mdio_if_info *mdio, int prtad) { return 0; }

void napi_gro_flush(struct napi_struct *napi, bool flush_old) {}
void free_irq(unsigned int a, void *p) {}

unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
	return (char*) skb;
}

int skb_pad(struct sk_buff *skb, int pad)
{
	return 0;
}

int skb_copy_bits(const struct sk_buff *skb, int offset, void *to, int len)
{
	return 0;
}

int skb_checksum_help(struct sk_buff *skb)
{
	return 0;
}

void skb_tstamp_tx(struct sk_buff *orig_skb,
		   struct skb_shared_hwtstamps *hwtstamps)
{}

int pskb_expand_head(struct sk_buff *skb, int nhead, int ntail, gfp_t gfp_mask) { return 0; }

unsigned char *__pskb_pull_tail(struct sk_buff *skb, int delta) { return (char*)skb; }

void skb_add_rx_frag(struct sk_buff *skb, int i, struct page *page, int off,
		     int size, unsigned int truesize) { }

void netdev_rss_key_fill(void *buffer, size_t len) { }

int del_timer(struct timer_list * timer) { return 0; }
int mod_timer(struct timer_list *timer, unsigned long expires) { return 0; }

struct net_device *netdev_all_upper_get_next_dev_rcu(struct net_device *dev,
						     struct list_head **iter) { return NULL; }

struct sk_buff *__napi_alloc_skb(struct napi_struct *napi,
				 unsigned int length, gfp_t gfp_mask) { return NULL; }

unsigned long msleep_interruptible(unsigned int msecs) { return 0ul; }

struct rtnl_link_stats64 *dev_get_stats(struct net_device *dev,
					struct rtnl_link_stats64 *storage) { return storage; }

void __dev_kfree_skb_any(struct sk_buff *skb, enum skb_free_reason reason) { };

u32 ethtool_op_get_link(struct net_device *dev) { return 0; }
int ethtool_op_get_ts_info(struct net_device *dev, struct ethtool_ts_info *eti) { return 0; }

__be16 eth_type_trans(struct sk_buff *skb, struct net_device *dev) { return 0; }


int __hw_addr_sync_dev(struct netdev_hw_addr_list *list,
		       struct net_device *dev,
		       int (*sync)(struct net_device *, const unsigned char *),
		       int (*unsync)(struct net_device *,
				     const unsigned char *)) { return 0; }
void __hw_addr_unsync_dev(struct netdev_hw_addr_list *list,
			  struct net_device *dev,
			  int (*unsync)(struct net_device *,
					const unsigned char *)) { }

struct sk_buff *__alloc_skb(unsigned int size, gfp_t priority, int flags,
			    int node) { return NULL; }

int ipv6_find_hdr(const struct sk_buff *skb, unsigned int *offset, int target,
		  unsigned short *fragoff, int *fragflg) { return 0; }

void __local_bh_enable_ip(unsigned long ip, unsigned int cnt) { } 

void kfree_skb(struct sk_buff *skb) { }

unsigned long volatile __jiffy_data jiffies;

int dma_supported(struct device *dev, u64 mask) { return 0; }

struct dma_map_ops *dma_ops;

bool arch_dma_alloc_attrs(struct device **dev, gfp_t *gfp) { return true; }

struct pglist_data contig_page_data;

/* TODO: Move this to a common header, say arch/x86/include/asm/udelay.h
 * As of now, lpj is passed by boot module to LCD. Devise a way to retrieve
 * this during vtx container creation so that arch module can handle this
 * seamlessly
 */
extern unsigned long loops_per_jiffy;

static void delay_loop(unsigned long loops)
{
	asm volatile(
		"	test %0,%0	\n"
		"	jz 3f		\n"
		"	jmp 1f		\n"

		".align 16		\n"
		"1:	jmp 2f		\n"

		".align 16		\n"
		"2:	dec %0		\n"
		"	jnz 2b		\n"
		"3:	dec %0		\n"

		: /* we don't need output */
		:"a" (loops)
	);
}

void __const_udelay(unsigned long xloops)
{
	int d0;

	xloops *= 4;
	asm("mull %%edx"
		:"=d" (xloops), "=&a" (d0)
		:"1" (xloops), "0" (loops_per_jiffy * (HZ/4)));

	delay_loop(++xloops);
}

/* XXX: Huh? Yeah, a tight loop for msleep is *ugly*. There is
 * no way to enable timers inside LCD. Replace this with a real
 * timer when such a support is added to LCDs.
 */
void msleep(unsigned int msecs)
{
	udelay(msecs * 1000);
}

void __udelay(unsigned long usecs)
{
	__const_udelay(usecs * 0x000010c7); /* 2**32 / 1000000 (rounded up) */
}

#endif /* IXGBE_STUB_H */
