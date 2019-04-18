#ifndef IXGBE_STUB_H
#define IXGBE_STUB_H

#ifndef CONFIG_LVD
int pci_enable_msi_range(struct pci_dev *dev, int minvec, int maxvec) {
	LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__);
	return -ENOSYS;
}

void pci_disable_msi(struct pci_dev *dev) {
	LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__);
}

int __must_check
request_threaded_irq(unsigned int irq, irq_handler_t handler,
		     irq_handler_t thread_fn,
		     unsigned long flags, const char *name, void *dev) {
	LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__);
	return 0;
}

void free_irq(unsigned int a, void *p) {
	LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__);
}
void napi_disable(struct napi_struct *n) {
	LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__);
}
void napi_complete_done(struct napi_struct *n, int work_done) {
	LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__);
}
#endif

int call_netdevice_notifiers(unsigned long val, struct net_device *dev)
{ LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 0; }

bool cancel_work_sync(struct work_struct *work) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return true; }

/* ASSERT_RTNL macro checks if rtnl_lock is held by the caller
 * during certain API calls. Inside LCDs we don't hold this lock,
 * just make the assert macro happy by faking that we did.
 */
int rtnl_is_locked(void) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 1; }

void synchronize_irq(unsigned int irq) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); }


void napi_gro_flush(struct napi_struct *napi, bool flush_old) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); }

unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
	LIBLCD_MSG("=====> Dummy %s called skb->len %d | pad %d", __func__, skb->len, len); return (char*) skb;
}

int skb_pad(struct sk_buff *skb, int pad) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 0; }

int skb_copy_bits(const struct sk_buff *skb, int offset, void *to, int len) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); dump_stack(); return 0; }

int skb_checksum_help(struct sk_buff *skb) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 0; }

void skb_tstamp_tx(struct sk_buff *orig_skb,
		   struct skb_shared_hwtstamps *hwtstamps) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); }

int pskb_expand_head(struct sk_buff *skb, int nhead, int ntail, gfp_t gfp_mask) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 0; }

unsigned char *__pskb_pull_tail(struct sk_buff *skb, int delta) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return (char*)skb; }

void netdev_rss_key_fill(void *buffer, size_t len) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); }

unsigned long dev_trans_start(struct net_device *dev)
{
	LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__);
	return 0ul;
}

void init_timer_key(struct timer_list *timer, unsigned int flags,
		    const char *name, struct lock_class_key *key)
{
	LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__);
	return;
}

int del_timer(struct timer_list * timer) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 0; }
int mod_timer(struct timer_list *timer, unsigned long expires) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 0; }

struct net_device *netdev_all_upper_get_next_dev_rcu(struct net_device *dev,
						     struct list_head **iter) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return NULL; }

unsigned long msleep_interruptible(unsigned int msecs) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 0ul; }

struct rtnl_link_stats64 *dev_get_stats(struct net_device *dev,
					struct rtnl_link_stats64 *storage) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return storage; }

void __dev_kfree_skb_any(struct sk_buff *skb, enum skb_free_reason reason) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); };

u32 ethtool_op_get_link(struct net_device *dev) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 0; }
int ethtool_op_get_ts_info(struct net_device *dev, struct ethtool_ts_info *eti) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 0; }

struct sk_buff *__alloc_skb(unsigned int size, gfp_t priority, int flags,
			    int node) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return NULL; }

int ipv6_find_hdr(const struct sk_buff *skb, unsigned int *offset, int target,
		  unsigned short *fragoff, int *fragflg) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 0; }

void __local_bh_enable_ip(unsigned long ip, unsigned int cnt) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); }

void kfree_skb(struct sk_buff *skb) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); }

unsigned long volatile __jiffy_data jiffies;

struct pglist_data contig_page_data;

void pci_restore_state(struct pci_dev *dev) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); }

#ifndef CONFIG_LVD
void __napi_schedule_irqoff(struct napi_struct *n) {
	LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__);
}
#endif

int mdio_mii_ioctl(const struct mdio_if_info *mdio,
			struct mii_ioctl_data *mii_data, int cmd)
{ LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 0; }

int ndo_dflt_bridge_getlink(struct sk_buff *skb, u32 pid, u32 seq,
				   struct net_device *dev, u16 mode,
				   u32 flags, u32 mask, int nlflags,
				   u32 filter_mask,
				   int (*vlan_fill)(struct sk_buff *skb,
						    struct net_device *dev,
						    u32 filter_mask))
{ LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 0; }

int ndo_dflt_fdb_add(struct ndmsg *ndm,
			    struct nlattr *tb[],
			    struct net_device *dev,
			    const unsigned char *addr,
			    u16 vid,
			    u16 flags)
{ LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return 0; }

struct nlattr *nla_find(const struct nlattr *head, int len, int attrtype)
{ LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return NULL; }

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

void usleep_range(unsigned long min, unsigned long max)
{
	udelay((max + min) >> 1);
}

bool napi_hash_del(struct napi_struct *napi) { LIBLCD_MSG("================>$$$$$$ Dummy %s called", __func__); return true; }

#endif /* IXGBE_STUB_H */
