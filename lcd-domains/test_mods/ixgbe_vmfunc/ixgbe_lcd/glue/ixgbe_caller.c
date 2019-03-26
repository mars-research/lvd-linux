#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include "../../ixgbe_common.h"
#include "../ixgbe_caller.h"
#include <asm/lcd_domains/liblcd.h>

#include <linux/hashtable.h>

#include <liblcd/netdev_helper.h>

#include "../../rdtsc_helper.h"
#include <lcd_config/post_hook.h>

//#define LCD_MEASUREMENT

struct cptr sync_ep;
static struct glue_cspace *c_cspace;
extern struct glue_cspace *ixgbe_cspace;
extern struct thc_channel *ixgbe_async;
extern cptr_t ixgbe_sync_endpoint;
extern cptr_t ixgbe_register_channel;
extern struct thc_channel_group ch_grp;

static struct net_device *g_net_device;

#ifdef IOMMU_ASSIGN
/* device for IOMMU assignment */
struct pcidev_info dev_assign = { 0x0000, 0x06, 0x00, 0x1 };
#endif

struct kmem_cache *skb_c_cache;
struct kmem_cache *skb_c_cache1;
struct kmem_cache *skb_cache;

/* XXX: How to determine this? */
#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(cptr_table, CPTR_HASH_BITS);

extern int init_default_flow_dissectors(void);

#define IXGBE_RX_HDR_SIZE	256
#define SKB_ALLOC_SIZE	(IXGBE_RX_HDR_SIZE + NET_SKB_PAD + NET_IP_ALIGN + SKB_DATA_ALIGN(sizeof(struct skb_shared_info)))

int glue_ixgbe_init(void)
{
	int ret;
	ret = glue_cap_init();
	if (ret) {
		LIBLCD_ERR("cap init");
		goto fail1;
	}
	ret = glue_cap_create(&c_cspace);
	if (ret) {
		LIBLCD_ERR("cap create");
		goto fail2;
	}
	ixgbe_cspace = c_cspace;
	hash_init(cptr_table);

        LIBLCD_MSG("dissector init ret %d",
                init_default_flow_dissectors());

	/* merge two datastructures into one for allocation */
	skb_c_cache = kmem_cache_create("skb_c_cache",
				sizeof(struct sk_buff_container)
				+ sizeof(struct sk_buff),
				0,
				SLAB_HWCACHE_ALIGN|SLAB_PANIC,
				NULL);
	if (!skb_c_cache) {
		LIBLCD_ERR("skb_container cache not created");
		goto fail2;
	}

	/* container only slab for rx */
	skb_c_cache1 = kmem_cache_create("skb_c_cache1",
				sizeof(struct sk_buff_container),
				0,
				SLAB_HWCACHE_ALIGN|SLAB_PANIC,
				NULL);
	if (!skb_c_cache1) {
		LIBLCD_ERR("skb_container cache not created");
		goto fail2;
	}

	/* skb->data cache */
	skb_cache = kmem_cache_create("skb+data",
				sizeof(struct sk_buff) +
				SKB_DATA_ALIGN(SKB_ALLOC_SIZE),
				0,
				SLAB_HWCACHE_ALIGN|SLAB_PANIC,
				NULL);
	if (!skb_cache) {
		LIBLCD_ERR("skb_container cache not created");
		goto fail2;
	}

	return 0;

fail2:
	glue_cap_exit();
fail1:
	return ret;

}

void glue_ixgbe_exit(void)
{
	glue_cap_destroy(c_cspace);
	glue_cap_exit();

	if (skb_c_cache)
		kmem_cache_destroy(skb_c_cache);

	if (skb_c_cache1)
		kmem_cache_destroy(skb_c_cache1);

	if (skb_cache)
		kmem_cache_destroy(skb_cache);
}

int glue_insert_skbuff(struct hlist_head *htable, struct sk_buff_container *skb_c)
{
	BUG_ON(!skb_c->skb);

	skb_c->my_ref = __cptr((unsigned long)skb_c->skb);

	hash_add(cptr_table, &skb_c->hentry,
			(unsigned long) skb_c->skb);
	return 0;
}

int glue_lookup_skbuff(struct hlist_head *htable, struct cptr c, struct sk_buff_container **skb_cout)
{
	struct sk_buff_container *skb_c;

	hash_for_each_possible(cptr_table, skb_c,
			hentry, (unsigned long) cptr_val(c)) {
		if (skb_c->skb == (struct sk_buff*) c.cptr)
			*skb_cout = skb_c;
	}
	return 0;
}

void glue_remove_skbuff(struct sk_buff_container *skb_c)
{
	hash_del(&skb_c->hentry);
}

static int setup_async_channel(cptr_t *buf1_cptr_out, cptr_t *buf2_cptr_out,
			struct thc_channel **chnl_out)
{
	int ret;
	cptr_t buf1_cptr, buf2_cptr;
	gva_t buf1_addr, buf2_addr;
	struct fipc_ring_channel *fchnl;
	struct thc_channel *chnl;
	unsigned int pg_order = ASYNC_RPC_BUFFER_ORDER - PAGE_SHIFT;
	LIBLCD_MSG("%s\n",__func__);
	/*
	 * Allocate buffers
	 *
	 * (We use the lower level alloc. If we used the heap, even though
	 * we may alloc only 1 - 2 pages, we would end up sharing around
	 * 4 MB chunks of memory, since the heap uses coarse microkernel
	 * allocations.)
	 */
	ret = _lcd_alloc_pages(GFP_KERNEL, pg_order, &buf1_cptr);
	if (ret) {
		LIBLCD_ERR("buf1 alloc");
		goto fail1;
	}
	ret = _lcd_alloc_pages(GFP_KERNEL, pg_order, &buf2_cptr);
	if (ret) {
		LIBLCD_ERR("buf2 alloc");
		goto fail2;
	}
	/*
	 * Map them somewhere
	 */
	ret = lcd_map_virt(buf1_cptr, pg_order, &buf1_addr);
	if (ret) {
		LIBLCD_ERR("error mapping buf1");
		goto fail3;
	}
	ret = lcd_map_virt(buf2_cptr, pg_order, &buf2_addr);
	if (ret) {
		LIBLCD_ERR("error mapping buf2");
		goto fail4;
	}
	/*
	 * Prep buffers for rpc
	 */
	ret = fipc_prep_buffers(ASYNC_RPC_BUFFER_ORDER,
				(void *)gva_val(buf1_addr),
				(void *)gva_val(buf2_addr));
	if (ret) {
		LIBLCD_ERR("prep buffers");
		goto fail5;
	}
	LIBLCD_MSG("==> Prep buffers");
	/*
	 * Alloc and init channel header
	 */
	fchnl = kmalloc(sizeof(*fchnl), GFP_KERNEL);
	if (!fchnl) {
		ret = -ENOMEM;
		LIBLCD_ERR("chnl alloc");
		goto fail6;
	}
	ret = fipc_ring_channel_init(fchnl, ASYNC_RPC_BUFFER_ORDER,
				(void *)gva_val(buf1_addr),
				(void *)gva_val(buf2_addr));
	if (ret) {
		LIBLCD_ERR("ring chnl init");
		goto fail7;
	}
	/*
	 * Install async channel in async dispatch loop
	 */
	chnl = kzalloc(sizeof(*chnl), GFP_KERNEL);
	if (!chnl) {
		ret = -ENOMEM;
		LIBLCD_ERR("alloc failed");
		goto fail8;
	}
	ret = thc_channel_init(chnl, fchnl);
	if (ret) {
		LIBLCD_ERR("error init'ing async channel group item");
		goto fail9;
	}

	*buf1_cptr_out = buf1_cptr;
	*buf2_cptr_out = buf2_cptr;
	*chnl_out = chnl;

	LIBLCD_MSG("Returning from %s", __func__);
	return 0;

fail9:
	kfree(chnl);
fail8:
fail7:
	kfree(fchnl);
fail6:
fail5:
	lcd_unmap_virt(buf1_addr, pg_order);
fail4:
	lcd_unmap_virt(buf1_addr, pg_order);
fail3:
	lcd_cap_delete(buf2_cptr);
fail2:
	lcd_cap_delete(buf1_cptr);
fail1:
	return ret;
}

static void destroy_async_channel(struct thc_channel *chnl)
{
	unsigned int pg_order = ASYNC_RPC_BUFFER_ORDER - PAGE_SHIFT;
	gva_t tx_gva, rx_gva;
	cptr_t tx, rx;
	int ret;
	unsigned long unused1, unused2;
	/*
	 * Translate ring buffers to cptrs
	 */
	tx_gva = __gva((unsigned long)thc_channel_to_fipc(chnl)->tx.buffer);
	rx_gva = __gva((unsigned long)thc_channel_to_fipc(chnl)->rx.buffer);
	ret = lcd_virt_to_cptr(tx_gva, &tx, &unused1, &unused2);
	if (ret) {
		LIBLCD_ERR("failed to translate tx to cptr");
		goto fail1;
	}
	ret = lcd_virt_to_cptr(rx_gva, &rx, &unused1, &unused2);
	if (ret) {
		LIBLCD_ERR("failed to translate rx to cptr");
		goto fail2;
	}
	printk("%s, destroying channel %p", __func__, chnl); 
	/*
	 * Unmap and kill tx/rx
	 */
	lcd_unmap_virt(tx_gva, pg_order);
	lcd_cap_delete(tx);
	lcd_unmap_virt(rx_gva, pg_order);
	lcd_cap_delete(rx);
	/*
	 * Free chnl header
	 */
	kfree(thc_channel_to_fipc(chnl));
	/*
	 * Free the async channel
	 *
	 * XXX: This is ok to do because there is no dispatch loop
	 * polling on the channel when we free it.
	 */
	kfree(chnl);

	return;

fail2:
fail1:
	return;
}

struct thc_channel_group_item *ptrs[32] = {0};

int create_one_async_channel(struct thc_channel **chnl, cptr_t *tx, cptr_t *rx)
{
	int ret;
	static int idx = 0;
	struct thc_channel_group_item *xmit_ch_item;

	ret = setup_async_channel(tx, rx, chnl);

	if (ret) {
		LIBLCD_ERR("async xmit chnl setup failed");
		return -1;
	}

	xmit_ch_item = kzalloc(sizeof(*xmit_ch_item), GFP_KERNEL);

	thc_channel_group_item_init(xmit_ch_item, *chnl, NULL);

	xmit_ch_item->xmit_channel = true;

	thc_channel_group_item_add(&ch_grp, xmit_ch_item);

	printk("%s, assingning ptrs[%d] to %p", __func__, idx % 32, xmit_ch_item);
	ptrs[idx++%32] = xmit_ch_item;

	return 0;
}

int create_async_channel(void)
{
	int ret;
	cptr_t tx, rx;
	cptr_t tx_xmit, rx_xmit;

	struct thc_channel *chnl;
	struct thc_channel *xmit_chnl;

#ifdef EXTRA_CHANNELS
	cptr_t tx_xmit2, rx_xmit2;
	cptr_t txirq_xmit, rxirq_xmit;
	struct thc_channel *xmit_chnl2;
	struct thc_channel *xmit_irq_chnl;
#endif
	/*
	 * Set up async and sync channels
	 */
	ret = lcd_create_sync_endpoint(&ixgbe_sync_endpoint);
	if (ret) {
		LIBLCD_ERR("lcd_create_sync_endpoint");
		goto fail1;
	}

	if (create_one_async_channel(&chnl, &tx, &rx)) {
		LIBLCD_ERR("async channel creation failed");
		goto fail_ch;
	}

	if (create_one_async_channel(&xmit_chnl, &tx_xmit, &rx_xmit)) {
		LIBLCD_ERR("async channel creation failed");
		goto fail_ch;
	}

	ptrs[0] = ptrs[1] = NULL;

#ifdef EXTRA_CHANNELS
	if (create_one_async_channel(&xmit_chnl2, &tx_xmit2, &rx_xmit2)) {
		LIBLCD_ERR("async channel creation failed");
		goto fail_ch;
	}

	if (create_one_async_channel(&xmit_irq_chnl, &txirq_xmit, &rxirq_xmit)) {
		LIBLCD_ERR("async channel creation failed");
		goto fail_ch;
	}
#endif

	lcd_set_cr0(ixgbe_sync_endpoint);
        lcd_set_cr1(rx);
        lcd_set_cr2(tx);
        lcd_set_cr3(rx_xmit);
        lcd_set_cr4(tx_xmit);
#ifdef EXTRA_CHANNELS
        lcd_set_cr5(rxirq_xmit);
        lcd_set_cr6(txirq_xmit);
        lcd_set_cr7(rx_xmit2);
	lcd_set_cr8(tx_xmit2);
#endif
	LIBLCD_MSG("sync call %s", __func__);
	ret = lcd_sync_call(ixgbe_register_channel);

	LIBLCD_MSG("sync call returned%s", __func__);

        /*
         * Flush cap registers
         */
        lcd_set_cr0(CAP_CPTR_NULL);
        lcd_set_cr1(CAP_CPTR_NULL);
        lcd_set_cr2(CAP_CPTR_NULL);
        lcd_set_cr3(CAP_CPTR_NULL);
        lcd_set_cr4(CAP_CPTR_NULL);

#ifdef EXTRA_CHANNELS
        lcd_set_cr5(CAP_CPTR_NULL);
        lcd_set_cr6(CAP_CPTR_NULL);
#endif
        if (ret) {
                LIBLCD_ERR("lcd_call");
                goto fail3;
        }
	ixgbe_async = chnl;
	LIBLCD_MSG("%s returns", __func__);
	return ret;
fail3:
        destroy_async_channel(chnl);
fail_ch:
	lcd_cap_delete(ixgbe_sync_endpoint);
fail1:
	return ret;
}

void pci_disable_msix(struct pci_dev *dev)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_DISABLE_MSIX);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;
}

int pci_enable_msix_range(struct pci_dev *dev, struct msix_entry *entries, int minvec, int maxvec)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	int sync_ret;
	unsigned 	long p_mem_sz;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	unsigned 	int request_cookie;

	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(_request,
			PCI_ENABLE_MSIX_RANGE);

	fipc_set_reg0(_request, minvec);
	fipc_set_reg1(_request, maxvec);

	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long )entries),
		&p_cptr,
		&p_mem_sz,
		&p_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	ret = thc_ipc_send_request(ixgbe_async,
		_request,
		&request_cookie);
	if (ret) {
		LIBLCD_ERR("thc_ipc_send_request");
		goto fail_ipc;
	}
	lcd_set_r0(ilog2(( p_mem_sz ) >> ( PAGE_SHIFT )));
	lcd_set_r1(p_offset);
	lcd_set_cr0(p_cptr);
	sync_ret = lcd_sync_send(ixgbe_sync_endpoint);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to send");
		lcd_exit(-1);
	}

	ret = thc_ipc_recv_response(ixgbe_async,
		request_cookie,
		&_response);

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;


}

int __must_check __pci_register_driver(struct pci_driver *drv,
		struct module *owner,
		const char* name)
{
	struct pci_driver_container *drv_container;
	struct module_container *owner_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;

	drv_container = container_of(drv,
		struct pci_driver_container,
		pci_driver);
	ret = glue_cap_insert_pci_driver_type(c_cspace,
		drv_container,
		&drv_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	owner_container = container_of(owner,
		struct module_container,
		module);
	ret = glue_cap_insert_module_type(c_cspace,
		owner_container,
		&owner_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			__PCI_REGISTER_DRIVER);
	fipc_set_reg2(_request,
			drv_container->my_ref.cptr);
	fipc_set_reg3(_request,
			owner_container->my_ref.cptr);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	drv_container->other_ref.cptr = fipc_get_reg1(_response);
	owner_container->other_ref.cptr = fipc_get_reg2(_response);
	drv_container->pci_driver.driver.owner = &owner_container->module;
	func_ret = fipc_get_reg3(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);

	return func_ret;

fail_async:
fail_insert:
fail_ipc:
	return ret;
}

struct net_device *alloc_etherdev_mqs(int sizeof_priv,
		unsigned int txqs,
		unsigned int rxqs)
{
	struct net_device_container *func_ret_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	struct net_device *func_ret;
	struct net_device *dev;

	unsigned int alloc_size = sizeof(struct net_device_container);
	void *p;

	if (sizeof_priv) {
		/* ensure 32-byte alignment of private area */
		alloc_size = ALIGN(alloc_size, NETDEV_ALIGN);
		alloc_size += sizeof_priv;
	}
	/* ensure 32-byte alignment of whole construct */
	alloc_size += NETDEV_ALIGN - 1;

	p = kzalloc(alloc_size, GFP_KERNEL);
	if (!p) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret_container = PTR_ALIGN(p, NETDEV_ALIGN);
	dev = func_ret = &func_ret_container->net_device;

	/* assign global instance */
	g_net_device = func_ret;
	ret = glue_cap_insert_net_device_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			ALLOC_ETHERDEV_MQS);
	fipc_set_reg1(_request,
			sizeof_priv);
	fipc_set_reg2(_request,
			txqs);
	fipc_set_reg3(_request,
			rxqs);
	fipc_set_reg4(_request,
			func_ret_container->my_ref.cptr);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret_container->other_ref.cptr = fipc_get_reg1(_response);

	dev_addr_init(dev);

	dev_mc_init(dev);
	dev_uc_init(dev);

	dev->gso_max_size = GSO_MAX_SIZE;
	dev->gso_max_segs = GSO_MAX_SEGS;

	INIT_LIST_HEAD(&dev->napi_list);
	INIT_LIST_HEAD(&dev->unreg_list);
	INIT_LIST_HEAD(&dev->close_list);
	INIT_LIST_HEAD(&dev->link_watch_list);
	INIT_LIST_HEAD(&dev->adj_list.upper);
	INIT_LIST_HEAD(&dev->adj_list.lower);
	INIT_LIST_HEAD(&dev->all_adj_list.upper);
	INIT_LIST_HEAD(&dev->all_adj_list.lower);
	INIT_LIST_HEAD(&dev->ptype_all);
	INIT_LIST_HEAD(&dev->ptype_specific);
	dev->priv_flags = IFF_XMIT_DST_RELEASE | IFF_XMIT_DST_RELEASE_PERM;
	dev->num_tx_queues = txqs;
	dev->real_num_tx_queues = txqs;

	netif_alloc_netdev_queues(dev);

	if (!func_ret->dev_addr) {
		LIBLCD_ERR("dev_addr assignment failed");
	}

	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_insert:
fail_alloc:
fail_ipc:
	return NULL;
}

/* FIXME: Handle this without extern */
extern struct pci_driver_container ixgbe_driver_container;
u64 dma_mask = 0;
void *data_pool;
#if 0
void lcd_register_chardev(const char* name, struct file_operations *fops)
{
	int ret = 0;
	struct fipc_message *request;
	struct fipc_message *response;

	/* we dont use the fops now, neither the name! we can handle it 
	 * later if needed */
	ret = async_msg_blocking_send_start(ixgbe_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	/* KLCD will setup a char device for us */
	async_msg_set_fn_type(request, REGISTER_CHARDEV);

	ret = thc_ipc_call(ixgbe_async, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async), response);

fail_async:
fail_ipc:
	return;
}

/* Give the user process a dedicated channel pair */
int ixgbe_user_open(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	struct fipc_message *response;
	unsigned int request_cookie;
	int ret = 0;
	unsigned int func_ret = 0;
	cptr_t tx, rx;
	struct thc_channel *chnl;

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	if (create_one_async_channel(&chnl, &tx, &rx)) {
		LIBLCD_ERR("async channel creation failed");
	}

        lcd_set_cr0(rx);
        lcd_set_cr1(tx);

	ret = lcd_sync_send(ixgbe_sync_endpoint);

        lcd_set_cr0(CAP_CPTR_NULL);
        lcd_set_cr1(CAP_CPTR_NULL);

	if (ret) {
		LIBLCD_ERR("sync send failed");
		func_ret = -1;
		goto fail_sync;
	}

fail_sync:
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		ret = -EIO;
	}
	fipc_set_reg1(response, func_ret);
	thc_ipc_reply(channel, request_cookie, response);

	return ret;
}

/* destroy the async channel created in open here */
int ixgbe_user_close(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep)
{
	return 0;
}

/* dummy */
static int ixgbeu_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int ixgbeu_close (struct inode *inode, struct file *filp)
{
	return 0;
}

struct file_operations ixgbe_user_fops = {
        .open   = ixgbeu_open,
      	.release = ixgbeu_close,
};
#endif
int probe_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev_container *dev_container;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	int ret = 0;
	cptr_t other_ref;
#ifdef IOMMU_ASSIGN
	unsigned int devfn;
#endif

#ifdef PCI_REGIONS
	cptr_t res0_cptr;
	gpa_t gpa_addr;
	unsigned int res0_len;
	void *dev_resource_0;
	cptr_t pool_cptr;
	gva_t pool_addr;
	unsigned int pool_ord;
#endif


	LIBLCD_MSG("%s called", __func__);
	request_cookie = thc_get_request_cookie(_request);
	dma_mask = fipc_get_reg2(_request);
	other_ref.cptr = fipc_get_reg1(_request);

	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	dev_container = kzalloc(sizeof( struct pci_dev_container   ),
		GFP_KERNEL);
	if (!dev_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ret = glue_cap_insert_pci_dev_type(c_cspace,
		dev_container,
		&dev_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	dev_container->pci_dev.dev.kobj.name = "ixgbe_lcd";
	dev_container->pci_dev.vendor = 0x8086;
	dev_container->pci_dev.device = 0x10fd;
	dev_container->other_ref = other_ref;
	dev_container->pci_dev.dev.dma_mask = &dma_mask;
#ifdef PCI_REGIONS
	ret = lcd_cptr_alloc(&res0_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_cptr;
	}

	ret = lcd_cptr_alloc(&pool_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_cptr;
	}

	lcd_set_cr0(res0_cptr);
	lcd_set_cr1(pool_cptr);
	ret = lcd_sync_recv(ixgbe_sync_endpoint);
	lcd_set_cr0(CAP_CPTR_NULL);
	lcd_set_cr1(CAP_CPTR_NULL);

	/* resource len */
	res0_len = lcd_r0();
	if (ret) {
		LIBLCD_ERR("failed to recv sync");
		goto fail_sync;
	}
	pool_ord = lcd_r1();

	ret = lcd_ioremap_phys(res0_cptr, res0_len, &gpa_addr);
	if (ret) {
		LIBLCD_ERR("failed to ioremap phys");
		goto fail_ioremap;
	}

	dev_resource_0 = lcd_ioremap(gpa_val(gpa_addr), res0_len);
	if (!dev_resource_0) {
		LIBLCD_ERR("failed to ioremap virt");
		goto fail_ioremap2;
	}
	dev_container->pci_dev.resource[0].start = (resource_size_t) dev_resource_0;
	dev_container->pci_dev.resource[0].end = (resource_size_t)((char*)dev_resource_0 + res0_len - 1);
	LIBLCD_MSG("%s: status reg 0x%X\n", __func__, *(unsigned int *)((char*)dev_resource_0 + 0x8));

	ret = lcd_map_virt(pool_cptr, pool_ord, &pool_addr);
	if (ret) {
		LIBLCD_ERR("failed to map pool");
		goto fail_pool;
	}

	LIBLCD_MSG("%s, mapping private pool %p | ord %d", __func__,
			gva_val(pool_addr), pool_ord);

	data_pool = (void*)gva_val(pool_addr);
#endif

#ifdef IOMMU_ASSIGN
	devfn = PCI_DEVFN(dev_assign.slot, dev_assign.fn);

	/* assign the device after we map the memory so that
	 * all the rammapped pages are automatically keyed
	 * into the iommu hardware
	 */
	ret = lcd_syscall_assign_device(dev_assign.domain,
				dev_assign.bus,
				devfn);
	if (ret)
		LIBLCD_ERR("Could not assign pci device to LCD: ret %d",
				ret);
#endif

	/* XXX: Pass null for now. struct pci_dev is passed from the
	 * kernel and needs analysis for wrapping it around a container
	 */
	func_ret = ixgbe_driver_container.pci_driver.probe(&dev_container->pci_dev, NULL);

//	lcd_register_chardev("ixgbe_user", &ixgbe_user_fops);

	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
#ifdef PCI_REGIONS
fail_ioremap:
fail_ioremap2:
fail_alloc:
fail_sync:
fail_insert:
fail_cptr:
fail_pool:
#endif
	return ret;
}

void pci_unregister_driver(struct pci_driver *drv)
{
	struct pci_driver_container *drv_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	drv_container = container_of(drv,
		struct pci_driver_container,
		pci_driver);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_UNREGISTER_DRIVER);
	fipc_set_reg2(_request,
			drv_container->other_ref.cptr);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	glue_cap_remove(c_cspace,
			drv_container->my_ref);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);

	return;
fail_async:
fail_ipc:
        destroy_async_channel(ixgbe_async);
	return;
}

int remove_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	/* async loop should get 0 */
	int ret = 0;
	struct fipc_message *_response;
	unsigned int __maybe_unused devfn;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

#ifdef IOMMU_ASSIGN
	devfn = PCI_DEVFN(dev_assign.slot, dev_assign.fn);

	ret = lcd_syscall_deassign_device(dev_assign.domain,
				dev_assign.bus,
				devfn);
	if (ret)
		LIBLCD_ERR("Could not deassign pci device to LCD: ret %d",
				ret);
#endif

	/* XXX: refer the comments under probe_callee */
	ixgbe_driver_container.pci_driver.remove(NULL);

	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
	return ret;
}

int register_netdev(struct net_device *dev)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			REGISTER_NETDEV);
	fipc_set_reg0(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg1(_request,
			dev->flags);
	fipc_set_reg2(_request,
			dev->priv_flags);
	fipc_set_reg3(_request,
			dev->features);
	fipc_set_reg4(_request,
			dev->hw_features);
	fipc_set_reg5(_request,
			dev->hw_enc_features);
	fipc_set_reg6(_request,
			dev->mpls_features);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	dev->reg_state = fipc_get_reg2(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

void ether_setup(struct net_device *dev)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			ETHER_SETUP);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;

}

int eth_mac_addr(struct net_device *dev,
		void *p)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int sync_ret;
	unsigned 	long p_mem_sz;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	unsigned 	int request_cookie;
	int func_ret;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			ETH_MAC_ADDR);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )p),
		&p_cptr,
		&p_mem_sz,
		&p_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	ret = thc_ipc_send_request(ixgbe_async,
		_request,
		&request_cookie);
	if (ret) {
		LIBLCD_ERR("thc_ipc_send_request");
		goto fail_ipc;
	}
	lcd_set_r0(ilog2(( p_mem_sz ) >> ( PAGE_SHIFT )));
	lcd_set_r1(p_offset);
	lcd_set_cr0(p_cptr);
	sync_ret = lcd_sync_send(ixgbe_sync_endpoint);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to send");
		lcd_exit(-1);
	}
	ret = thc_ipc_recv_response(ixgbe_async,
		request_cookie,
		&_response);
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

int eth_validate_addr(struct net_device *dev)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			ETH_VALIDATE_ADDR);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

void free_netdev(struct net_device *dev)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			FREE_NETDEV);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;
}

void netif_carrier_off(struct net_device *dev)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NETIF_CARRIER_OFF);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	dev->state = fipc_get_reg1(_response);

	return;
fail_async:
fail_ipc:
	return;

}

void netif_carrier_on(struct net_device *dev)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NETIF_CARRIER_ON);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	dev->state = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;

}

void netif_device_attach(struct net_device *dev)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NETIF_DEVICE_ATTACH);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	dev->state = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;
}

void netif_device_detach(struct net_device *dev)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NETIF_DEVICE_DETACH);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	dev->state = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;
}

int netif_set_real_num_rx_queues(struct net_device *dev,
		unsigned int rxq)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NETIF_SET_REAL_NUM_RX_QUEUES);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg3(_request,
			rxq);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

int netif_set_real_num_tx_queues(struct net_device *dev,
		unsigned int txq)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NETIF_SET_REAL_NUM_TX_QUEUES);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg3(_request,
			txq);
	LIBLCD_MSG("%s, setting real_num_tx_queues to %d", __func__, txq);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

void napi_consume_skb(struct sk_buff *skb, int budget)
{
	int ret;
	struct fipc_message *_request;
	struct sk_buff_container *skb_c = NULL;
#ifndef NAPI_CONSUME_SEND_ONLY
	struct fipc_message *_response;
#endif
	struct thc_channel *channel;

	glue_lookup_skbuff(cptr_table,
		__cptr((unsigned long)skb), &skb_c);

#ifdef SENDER_DISPATCH_LOOP
	channel = skb_c->channel;
#else
	channel = ixgbe_async;
#endif

	ret = async_msg_blocking_send_start(channel,
		&_request);

	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NAPI_CONSUME_SKB);

	fipc_set_reg0(_request, skb_c->other_ref.cptr);
	fipc_set_reg1(_request, budget);

#ifdef NAPI_CONSUME_SEND_ONLY
	thc_set_msg_type(_request, msg_type_request);
	fipc_send_msg_end(thc_channel_to_fipc(channel),
					_request);
#else
	ret = thc_ipc_call(channel,
		_request,
		&_response);

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	fipc_recv_msg_end(thc_channel_to_fipc(channel),
			_response);
#endif

	glue_remove_skbuff(skb_c);
	kmem_cache_free(skb_c_cache, skb_c);

fail_async:
#ifndef NAPI_CONSUME_SEND_ONLY
fail_ipc:
#endif
	return;
}

void consume_skb(struct sk_buff *skb)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	unsigned long skb_sz, skb_off, skbh_sz, skbh_off;
	cptr_t skb_cptr, skbh_cptr;
	struct sk_buff_container *skb_c = NULL;

	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);

	glue_lookup_skbuff(cptr_table,
		__cptr((unsigned long)skb), &skb_c);

	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			CONSUME_SKB);

	fipc_set_reg0(_request, skb_c->other_ref.cptr);

	if (!skb->private) {
		ret = lcd_virt_to_cptr(__gva((unsigned long)skb),
			&skb_cptr,
			&skb_sz,
			&skb_off);
		if (ret) {
			LIBLCD_ERR("lcd_virt_to_cptr");
			goto fail_virt;
		}

		ret = lcd_virt_to_cptr(__gva((unsigned long)skb->head),
			&skbh_cptr,
			&skbh_sz,
			&skbh_off);
		if (ret) {
			LIBLCD_ERR("lcd_virt_to_cptr");
			goto fail_virt;
		}

#ifdef IOMMU_ASSIGN
		LIBLCD_MSG("%s, iommu unmap page", __func__);
		ret = lcd_syscall_iommu_unmap_page(lcd_gva2gpa(__gva((unsigned long) skb->head)),
					get_order(skbh_sz));
		if (ret)
			LIBLCD_ERR("unMapping failed for packet %p",
					__pa(skb->data));
#endif

		lcd_unmap_virt(__gva((unsigned long)skb->head), get_order(skb_sz));
		lcd_unmap_virt(__gva((unsigned long)skb), get_order(skbh_sz));

		lcd_cap_delete(skb_c->skb_cptr);
		lcd_cap_delete(skb_c->skbh_cptr);
	} else {
		kfree(skb);
	}

	glue_remove_skbuff(skb_c);
	kfree(skb_c);

	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
fail_virt:
	return;

}

void unregister_netdev(struct net_device *dev)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	struct net_device_container *dev_container;

	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			UNREGISTER_NETDEV);
	fipc_set_reg0(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;

}

int eth_platform_get_mac_address(struct device *dev,
		u8 *mac_addr)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	int i;
	union mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };

	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			ETH_PLATFORM_GET_MAC_ADDRESS);

	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);

	if (!func_ret) {
		m.mac_addr_l = fipc_get_reg2(_response);
		memcpy(mac_addr, m.mac_addr, ETH_ALEN);
		for (i = 0; i < ETH_ALEN; i++) {
			printk("%02X:", m.mac_addr[i]);
		}
		LIBLCD_MSG("\n%s, got 0x%06lX", __func__, m.mac_addr_l);
	}

	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

int dev_addr_add(struct net_device *dev,
		const unsigned char *addr,
		unsigned char addr_type)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int sync_ret;
	unsigned 	long addr_mem_sz;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
	unsigned 	int request_cookie;
	int func_ret;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			DEV_ADDR_ADD);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )addr),
		&addr_cptr,
		&addr_mem_sz,
		&addr_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg3(_request,
			addr_type);
	ret = thc_ipc_send_request(ixgbe_async,
		_request,
		&request_cookie);
	if (ret) {
		LIBLCD_ERR("thc_ipc_send_request");
		goto fail_ipc;
	}
	lcd_set_r0(ilog2(( addr_mem_sz ) >> ( PAGE_SHIFT )));
	lcd_set_r1(addr_offset);
	lcd_set_cr0(addr_cptr);
	sync_ret = lcd_sync_send(ixgbe_sync_endpoint);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to send");
		lcd_exit(-1);
	}
	ret = thc_ipc_recv_response(ixgbe_async,
		request_cookie,
		&_response);
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);

	/* call local function */
	liblcd_dev_addr_add(dev, addr, addr_type);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

int dev_addr_del(struct net_device *dev,
		const unsigned char *addr,
		unsigned char addr_type)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int sync_ret;
	unsigned 	long addr_mem_sz;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
	unsigned 	int request_cookie;
	int func_ret;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			DEV_ADDR_DEL);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )addr),
		&addr_cptr,
		&addr_mem_sz,
		&addr_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg3(_request,
			addr_type);
	ret = thc_ipc_send_request(ixgbe_async,
		_request,
		&request_cookie);
	if (ret) {
		LIBLCD_ERR("thc_ipc_send_request");
		goto fail_ipc;
	}
	lcd_set_r0(ilog2(( addr_mem_sz ) >> ( PAGE_SHIFT )));
	lcd_set_r1(addr_offset);
	lcd_set_cr0(addr_cptr);
	sync_ret = lcd_sync_send(ixgbe_sync_endpoint);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to send");
		lcd_exit(-1);
	}
	ret = thc_ipc_recv_response(ixgbe_async,
		request_cookie,
		&_response);
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);

	/* call local function */
	liblcd_dev_addr_del(dev, addr, addr_type);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

int device_set_wakeup_enable(struct device *dev,
		bool enable)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			DEVICE_SET_WAKEUP_ENABLE);
	fipc_set_reg1(_request,
			enable);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

void netif_tx_stop_all_queues(struct net_device *dev)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	struct net_device_container *dev_container;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	async_msg_set_fn_type(_request,
			NETIF_TX_STOP_ALL_QUEUES);

	fipc_set_reg0(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;
}

void _netif_tx_wake_all_queues(struct net_device *dev)
{
	struct net_device_container *dev_queue_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	dev_queue_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NETIF_TX_WAKE_ALL_QUEUES);
	fipc_set_reg1(_request,
			dev_queue_container->other_ref.cptr);
	fipc_set_reg2(_request,
			dev->num_tx_queues);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;
}

int pci_disable_pcie_error_reporting(struct pci_dev *dev)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_DISABLE_PCIE_ERROR_REPORTING);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

int pci_bus_read_config_word(struct pci_bus *bus,
		unsigned int devfn,
		int where,
		unsigned short *val)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_BUS_READ_CONFIG_WORD);
	fipc_set_reg1(_request,
			devfn);
	fipc_set_reg2(_request,
			where);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	*val = fipc_get_reg1(_response);
	func_ret = fipc_get_reg2(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

int pci_bus_write_config_word(struct pci_bus *bus,
		unsigned int devfn,
		int where,
		unsigned short val)
{
	struct pci_bus_container *bus_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	bus_container = container_of(bus,
		struct pci_bus_container,
		pci_bus);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_BUS_WRITE_CONFIG_WORD);
	fipc_set_reg1(_request,
			bus_container->other_ref.cptr);
	fipc_set_reg3(_request,
			devfn);
	fipc_set_reg4(_request,
			where);
	fipc_set_reg5(_request,
			val);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

int pci_cleanup_aer_uncorrect_error_status(struct pci_dev *dev)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_CLEANUP_AER_UNCORRECT_ERROR_STATUS);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

void pci_disable_device(struct pci_dev *dev)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_DISABLE_DEVICE);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;
}

int pci_enable_pcie_error_reporting(struct pci_dev *dev)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_ENABLE_PCIE_ERROR_REPORTING);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

int pcie_capability_read_word(struct pci_dev *dev,
		int pos,
		unsigned short *val)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCIE_CAPABILITY_READ_WORD);
	fipc_set_reg1(_request,
			pos);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

int pcie_get_minimum_link(struct pci_dev *dev,
		enum pci_bus_speed *speed,
		enum pcie_link_width *width)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCIE_GET_MINIMUM_LINK);
	fipc_set_reg1(_request,
			*speed);
	fipc_set_reg2(_request,
			*width);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

int pci_enable_device_mem(struct pci_dev *dev)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret = 0;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_ENABLE_DEVICE_MEM);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return func_ret;
}

int pci_request_selected_regions(struct pci_dev *dev,
		int type,
		const char *reg)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_REQUEST_SELECTED_REGIONS);
	fipc_set_reg1(_request,
			type);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

int pci_request_selected_regions_exclusive(struct pci_dev *dev,
		int type,
		const char *reg)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_REQUEST_SELECTED_REGIONS_EXCLUSIVE);
	fipc_set_reg1(_request,
			type);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

void pci_set_master(struct pci_dev *dev)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_SET_MASTER);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;

}

int pci_save_state(struct pci_dev *dev)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_SAVE_STATE);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

void pci_release_selected_regions(struct pci_dev *dev,
		int r)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_RELEASE_SELECTED_REGIONS);
	fipc_set_reg1(_request,
			r);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;

}

int pci_select_bars(struct pci_dev *dev,
		unsigned long flags)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret = 0;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_SELECT_BARS);
	fipc_set_reg1(_request,
			flags);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return func_ret;

}

int pci_wake_from_d3(struct pci_dev *dev,
		bool enable)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret = 0;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PCI_WAKE_FROM_D3);
	fipc_set_reg1(_request,
			enable);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return func_ret;
}

int ndo_open_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret = 0;
	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	func_ret = dev_container->net_device.netdev_ops->ndo_open(( &dev_container->net_device ));

	LIBLCD_MSG("%s, returns %d", __func__, func_ret);
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
fail_lookup:
	return ret;
}

int ndo_stop_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	func_ret = dev_container->net_device.netdev_ops->ndo_stop(( &dev_container->net_device ));
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
fail_lookup:
	return ret;

}

extern netdev_tx_t ixgbe_xmit_frame(struct sk_buff *skb,
				struct net_device *dev);
extern struct net_device *g_netdev;

int prep_channel_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	cptr_t tx, rx;
#ifdef SOFTIRQ_CHANNELS
	cptr_t tx_sirq, rx_sirq;
	struct thc_channel *xmit_sirq;
#endif
	struct thc_channel *xmit;
	unsigned 	int request_cookie;
	int ret;
	struct fipc_message *_response;

	request_cookie = thc_get_request_cookie(_request);

	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	if (create_one_async_channel(&xmit, &tx, &rx))
		LIBLCD_ERR("async channel creation failed\n");

	lcd_set_cr0(tx);
	lcd_set_cr1(rx);

#ifdef SOFTIRQ_CHANNELS
	printk("Creating one for softirq\n");

	if (create_one_async_channel(&xmit_sirq, &tx_sirq, &rx_sirq))
		LIBLCD_ERR("async channel creation failed\n");
	lcd_set_cr2(tx_sirq);
	lcd_set_cr3(rx_sirq);
#endif

	LIBLCD_MSG("%s: Preparing sync send", __func__);
	ret = lcd_sync_send(ixgbe_sync_endpoint);

	lcd_set_cr0(CAP_CPTR_NULL);
	lcd_set_cr1(CAP_CPTR_NULL);

#ifdef SOFTIRQ_CHANNELS
	lcd_set_cr2(CAP_CPTR_NULL);
	lcd_set_cr3(CAP_CPTR_NULL);
#endif

	if (ret) {
		LIBLCD_ERR("failed to send");
		lcd_exit(-1);
	}

	if (async_msg_blocking_send_start(_channel, &_response)) {
		LIBLCD_ERR("error getting response msg");
		ret = -EIO;
	}

	thc_ipc_reply(_channel, request_cookie, _response);

	return ret;
}

/* This function is used for testing bare fipc, non-async mtu sized packets */
int ndo_start_xmit_bare_callee(struct fipc_message *_request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct fipc_message *response;
	fipc_recv_msg_end(thc_channel_to_fipc(channel), _request);

	if (likely(async_msg_blocking_send_start(channel, &response))) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_send_msg_end(thc_channel_to_fipc(channel), response);

	return 0;
}

int ndo_start_xmit_clean_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct sk_buff *skb;
	struct sk_buff_container *skb_c;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	cptr_t skb_ref;
	xmit_type_t xmit_type;
	unsigned long skbh_offset, skb_end;
	struct skbuff_members *skb_lcd;
	__be16 proto;
	void *mem;
	unsigned int seq, ack;
#ifdef LCD_MEASUREMENT
	TS_DECL(xmit);
#endif
	request_cookie = thc_get_request_cookie(_request);
	xmit_type = fipc_get_reg0(_request);

	skb_ref = __cptr(fipc_get_reg2(_request));

	skbh_offset = fipc_get_reg3(_request);

	skb_end = fipc_get_reg4(_request);
	proto = fipc_get_reg5(_request);

	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	//if (_channel == ixgbe_async)
	//	printk("%s, Got msg - reqc 0x%x | xmitty %d",
	//			__func__, request_cookie,
	//			xmit_type);

	skb_c = mem = kmem_cache_alloc(skb_c_cache,
				GFP_KERNEL);
	skb = (struct sk_buff*)((char*)mem +
			sizeof(struct sk_buff_container));

	if (!skb) {
		LIBLCD_MSG("out of mmeory");
		goto fail_alloc;
	}

	skb->head = (char*)data_pool + skbh_offset;
	skb->end = skb_end;

	skb_lcd = SKB_LCD_MEMBERS(skb);
	skb->private = true;
	skb->protocol = proto;

	P(len);
	P(data_len);
	P(queue_mapping);
	P(xmit_more);
	P(tail);
	P(truesize);
	P(ip_summed);
	P(csum_start);
	P(network_header);
	P(csum_offset);
	P(transport_header);

	if (0)
	LIBLCD_MSG("lcd-> l: %d | dlen %d | qm %d"
		   " | xm %d | t %lu |ts %u",
		skb->len, skb->data_len, skb->queue_mapping,
		skb->xmit_more, skb->tail, skb->truesize);

	skb->data = skb->head + skb_lcd->head_data_off;

	skb_c->skb = skb;
	glue_insert_skbuff(cptr_table, skb_c);
	skb_c->other_ref = skb_ref;

	skb_c->channel = _channel;

#ifdef LCD_MEASUREMENT
	TS_START_LCD(xmit);
#endif

	/* if TCP */
	/* 38-41 - Seq no
	 * 42-45 - Ack no
	 */
	if (skb->data[23] == 0x6) {
		unsigned char flags = (skb->data[46] & 0x0F) | skb->data[47];
		seq = (skb->data[38] << 24) | (skb->data[39] << 16) | (skb->data[40] << 8) | skb->data[41];
		ack = (skb->data[42] << 24) | (skb->data[43] << 16) | (skb->data[44] << 8) | skb->data[45];

		printk("got pkt | cookie %d | proto %x | IP proto %x | TCP.seq %u | TCP.ack %u | TCP Flags [%s%s%s%s%s]\n",
				request_cookie, htons(skb->protocol),
				skb->data[23], seq, ack,
				(flags & 0x1) ? " FIN " : "",
				(flags & 0x2) ? " SYN " : "",
				(flags & 0x4) ? " RST " : "",
				(flags & 0x8) ? " PSH " : "",
				(flags & 0x10) ? " ACK " : "");

	}

	func_ret = ixgbe_xmit_frame(skb, g_netdev);

	if (func_ret)
		LIBLCD_MSG("ixgbe xmit failed %p", skb);

#ifdef LCD_MEASUREMENT
	TS_STOP_LCD(xmit);
#endif
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret);
#ifdef LCD_MEASUREMENT
	fipc_set_reg2(_response,
			TS_DIFF(xmit));
#endif
	thc_ipc_reply(_channel,
			request_cookie,
			_response);

//	printk("%s, posting response for pkt | cookie %d | seq %u | ack %u", __func__,
//				request_cookie, seq, ack);

//	if (_channel == ixgbe_async)
//		printk("%s, Sending reply for reqc 0x%x", __func__,
//				request_cookie);
fail_alloc:
	return ret;

}

int ndo_start_xmit_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct sk_buff *skb = NULL;
	struct sk_buff_container *skb_c = NULL;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	cptr_t skb_cptr, skbd_cptr;
	unsigned long skb_ord, skb_off;
	unsigned long skbd_ord, skbd_off;
	gva_t skb_gva, skbd_gva;
	unsigned int data_off;
	cptr_t skb_ref;
	xmit_type_t xmit_type;
	unsigned long skbh_offset, skb_end;
	struct skbuff_members *skb_lcd;
	__be16 proto;
	void *mem;
#ifdef LCD_MEASUREMENT
	TS_DECL(xmit);
#endif
	request_cookie = thc_get_request_cookie(_request);
	xmit_type = fipc_get_reg0(_request);

	skb_ref = __cptr(fipc_get_reg2(_request));

	skbh_offset = fipc_get_reg3(_request);

	skb_end = fipc_get_reg4(_request);
	proto = fipc_get_reg5(_request);

	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

//	if (_channel == ixgbe_async)
//		printk("%s, Got msg - reqc 0x%x | xmitty %d",
//				__func__, request_cookie,
//				xmit_type);

	switch (xmit_type) {
	case VOLUNTEER_XMIT:
		ret = lcd_cptr_alloc(&skb_cptr);

		if (ret) {
			LIBLCD_ERR("failed to get cptr");
			goto fail_sync;
		}
		ret = lcd_cptr_alloc(&skbd_cptr);
		if (ret) {
			LIBLCD_ERR("failed to get cptr");
			goto fail_sync;
		}

		lcd_set_cr6(skb_cptr);
		lcd_set_cr1(skbd_cptr);
		ret = lcd_sync_recv(sync_ep);
		lcd_set_cr0(CAP_CPTR_NULL);
		lcd_set_cr1(CAP_CPTR_NULL);
		if (ret) {
			LIBLCD_ERR("failed to recv");
			goto fail_sync;
		}
		skb_ord = lcd_r0();
		skb_off = lcd_r1();
		skbd_ord = lcd_r2();
		skbd_off = lcd_r3();
		data_off = lcd_r4();

		ret = lcd_map_virt(skb_cptr, skb_ord, &skb_gva);
		if (ret) {
			LIBLCD_ERR("failed to map void *addr");
			goto fail_sync;
		}

		ret = lcd_map_virt(skbd_cptr, skbd_ord, &skbd_gva);
		if (ret) {
			LIBLCD_ERR("failed to map void *addr");
			goto fail_sync;
		}

		skb = (void*)(gva_val(skb_gva) + skb_off);
		skb->head = (void*)(gva_val(skbd_gva) + skbd_off);
		skb->data = skb->head + data_off;
		break;

	case SHARED_DATA_XMIT:
		skb_c = mem = kmem_cache_alloc(skb_c_cache,
					GFP_KERNEL);
		skb = (struct sk_buff*)((char*)mem +
				sizeof(struct sk_buff_container));

		if (!skb) {
			LIBLCD_MSG("out of mmeory");
			goto fail_alloc;
		}

		skb->head = (char*)data_pool + skbh_offset;
		skb->end = skb_end;

		skb_lcd = SKB_LCD_MEMBERS(skb);
		skb->private = true;
		skb->protocol = proto;

		P(len);
		P(data_len);
		P(queue_mapping);
		P(xmit_more);
		P(tail);
		P(truesize);
		P(ip_summed);
		P(csum_start);
		P(network_header);
		P(csum_offset);
		P(transport_header);

		if (0)
		LIBLCD_MSG("lcd-> l: %d | dlen %d | qm %d"
			   " | xm %d | t %lu |ts %u",
			skb->len, skb->data_len, skb->queue_mapping,
			skb->xmit_more, skb->tail, skb->truesize);

		skb->data = skb->head + skb_lcd->head_data_off;

		//printk("%s, len %d", __func__, skb->len);

		break;

	default:
		LIBLCD_ERR("%s, unknown xmit type", __func__);
		break;
	}

	skb_c->skb = skb;
	glue_insert_skbuff(cptr_table, skb_c);
	skb_c->other_ref = skb_ref;

	if (xmit_type == VOLUNTEER_XMIT) {
		skb_c->skbd_ord = skbd_ord;
		skb_c->skb_cptr = skb_cptr;
		skb_c->skbh_cptr = skbd_cptr;

#ifdef IOMMU_ASSIGN
		LIBLCD_MSG("%s, iommu map page", __func__);
		ret = lcd_syscall_iommu_map_page(
				lcd_gva2gpa(skbd_gva),
				skbd_ord, true);

		if (ret)
			LIBLCD_ERR("Mapping failed for packet %p",
					__pa(skb->data));
#endif
	} /* if */

#ifdef LCD_MEASUREMENT
	TS_START_LCD(xmit);
#endif
	func_ret = ixgbe_xmit_frame(skb, g_netdev);

#ifdef LCD_MEASUREMENT
	TS_STOP_LCD(xmit);
#endif
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret);
#ifdef LCD_MEASUREMENT
	fipc_set_reg2(_response,
			TS_DIFF(xmit));
#endif
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
//	if (_channel == ixgbe_async)
//		printk("%s, Sending reply for reqc 0x%x", __func__,
//				request_cookie);
fail_alloc:
fail_sync:
	return ret;

}

int ndo_set_rx_mode_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev_container->net_device.netdev_ops->ndo_set_rx_mode(( &dev_container->net_device ));
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
fail_lookup:
	return ret;

}

int ndo_validate_addr_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	func_ret = dev_container->net_device.netdev_ops->ndo_validate_addr(( &dev_container->net_device));
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
fail_lookup:
	return ret;

}

int ndo_set_mac_address_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
	gva_t addr_gva;

	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	sync_ret = lcd_cptr_alloc(&addr_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}
	lcd_set_cr0(addr_cptr);
	sync_ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to recv");
		lcd_exit(-1);
	}
	mem_order = lcd_r0();
	addr_offset = lcd_r1();
	sync_ret = lcd_map_virt(addr_cptr,
		mem_order,
		&addr_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *addr");
		lcd_exit(-1);
	}
	func_ret = dev_container->net_device.netdev_ops->ndo_set_mac_address(( &dev_container->net_device ),
		( void  * )( ( gva_val(addr_gva) ) + ( addr_offset ) ));
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
fail_lookup:
	return ret;

}

int ndo_change_mtu_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int new_mtu;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	new_mtu = fipc_get_reg3(_request);
	func_ret = dev_container->net_device.netdev_ops->ndo_change_mtu(( &dev_container->net_device ),
		new_mtu);
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
fail_lookup:
	return ret;

}

int ndo_tx_timeout_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;

	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev_container->net_device.netdev_ops->ndo_tx_timeout(( &dev_container->net_device ));

	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}

	thc_ipc_reply(_channel,
			request_cookie,
			_response);
fail_lookup:
	return ret;

}

int ndo_set_tx_maxrate_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int queue_index;
	unsigned 	int maxrate;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	queue_index = fipc_get_reg3(_request);
	maxrate = fipc_get_reg4(_request);
	func_ret = dev_container->net_device.netdev_ops->ndo_set_tx_maxrate(( &dev_container->net_device ),
		queue_index,
		maxrate);
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
fail_lookup:
	return ret;

}

int ndo_get_stats64_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	struct rtnl_link_stats64 stats;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;

	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	dev_container->net_device.netdev_ops->ndo_get_stats64(( &dev_container->net_device ),
		&stats);

	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			stats.rx_packets);
	fipc_set_reg2(_response,
			stats.rx_bytes);
	fipc_set_reg3(_response,
			stats.tx_packets);
	fipc_set_reg4(_response,
			stats.tx_bytes);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
fail_lookup:
	return ret;
}

extern void __ixgbe_service_event_schedule(struct net_device *netdev);

int ixgbe_service_event_schedule_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int ret = 0;
	unsigned int request_cookie;
	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	__ixgbe_service_event_schedule(&dev_container->net_device);

fail_lookup:
	return ret;
}

extern void __ixgbe_dump(struct net_device *);

int trigger_dump_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int ret = 0;
	unsigned int request_cookie;
	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	__ixgbe_dump(&dev_container->net_device);

fail_lookup:
	return ret;
}

struct sync_container *sync_container;
struct unsync_container *unsync_container;

int __hw_addr_sync_dev(
		struct netdev_hw_addr_list *list,
		struct net_device *dev1,
		int ( *sync )(struct net_device *, const unsigned char*),
		int ( *unsync )(struct net_device *, const unsigned char*))
{
	struct net_device_container *dev1_container;
	int ret = 0;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	dev1_container = container_of(dev1,
		struct net_device_container,
		net_device);
	sync_container = kzalloc(sizeof( struct sync_container   ),
		GFP_KERNEL);
	if (!sync_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	sync_container->sync = sync;

	unsync_container = kzalloc(sizeof( struct unsync_container   ),
		GFP_KERNEL);
	if (!unsync_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	unsync_container->unsync = unsync;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			__HW_ADDR_SYNC_DEV);
	fipc_set_reg1(_request,
			dev1_container->other_ref.cptr);
	fipc_set_reg2(_request, list == &dev1->mc ? MC_LIST : UC_LIST);

	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	liblcd__hw_addr_sync_dev(list, dev1, sync, unsync);
	return func_ret;
fail_async:
fail_ipc:
fail_alloc:
	return ret;
}

void __hw_addr_unsync_dev(
		struct netdev_hw_addr_list *list,
		struct net_device *dev1,
		int ( *unsync )(struct net_device *, const unsigned char*))
{
	struct net_device_container *dev1_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	dev1_container = container_of(dev1,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			__HW_ADDR_UNSYNC_DEV);
	fipc_set_reg1(_request,
			dev1_container->other_ref.cptr);
	fipc_set_reg2(_request, list == &dev1->mc ? MC_LIST : UC_LIST);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);

	liblcd__hw_addr_unsync_dev(list, dev1, unsync);
	return;
fail_async:
fail_ipc:
	return;

}

int sync_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };

	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	m.mac_addr_l = fipc_get_reg3(_request);

	func_ret = sync_container->sync(( &dev_container->net_device ),
		m.mac_addr);

	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
	return ret;
fail_lookup:
	return ret;
}

int unsync_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };

	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	m.mac_addr_l = fipc_get_reg3(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	func_ret = unsync_container->unsync(( &dev_container->net_device ),
		m.mac_addr);
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
	return ret;

fail_lookup:
	return ret;
}

#ifdef HOST_IRQ
irq_handler_t msix_other_irqh;

int _request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags, const char *name, void *dev)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	printk("%s, irq # %d", __func__, irq);

	async_msg_set_fn_type(_request,
			REQUEST_IRQ);
	fipc_set_reg1(_request, irq);
	fipc_set_reg2(_request, flags);
	msix_other_irqh = handler;
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

extern int __ixgbe_poll(void);

void _free_irq(unsigned int irq,
		int dev_id)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			_FREE_IRQ);
	fipc_set_reg1(_request,
			irq);

	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;
}
#endif /* HOST_IRQ */

void netif_napi_add(struct net_device *dev,
		struct napi_struct *napi,
		int ( *poll )(struct napi_struct*, int),
		int weight)
{
	struct net_device_container *dev_container;
	struct poll_container *poll_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	poll_container = kzalloc(sizeof( struct poll_container   ),
		GFP_KERNEL);
	if (!poll_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	poll_container->poll = poll;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NETIF_NAPI_ADD);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg3(_request,
			weight);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
fail_alloc:
	return;
}

extern bool poll_start;

int poll_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	unsigned 	int request_cookie;
	bool poll;

	request_cookie = thc_get_request_cookie(_request);

	poll = fipc_get_reg0(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	poll_start = poll;
	LIBLCD_MSG("%s, setting poll_start to %d",
		__func__, poll_start);

	return 0;
}

void netif_napi_del(struct napi_struct *napi)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NETIF_NAPI_DEL);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;
}

void netif_wake_subqueue(struct net_device *dev,
		unsigned short queue_index)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NETIF_WAKE_SUBQUEUE);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg3(_request,
			queue_index);
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return;
fail_async:
fail_ipc:
	return;
}

int netif_receive_skb(struct sk_buff *skb)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	struct sk_buff_container *skb_c = NULL;
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	glue_lookup_skbuff(cptr_table,
		__cptr((unsigned long)skb), &skb_c);

	async_msg_set_fn_type(_request,
			NETIF_RECEIVE_SKB);

	fipc_set_reg0(_request, skb_c->other_ref.cptr);

	glue_remove_skbuff(skb_c);
	kfree(skb_c);

	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

#ifndef LOCAL_SKB
gro_result_t napi_gro_receive(struct napi_struct *napi,
		struct sk_buff *skb)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	struct sk_buff_container *skb_c;
	unsigned long skb_sz, skb_off, skbh_sz, skbh_off;
	cptr_t skb_cptr, skbh_cptr;
	struct skb_shared_info *shinfo;
	struct page *p = NULL;

	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	glue_lookup_skbuff(cptr_table,
		__cptr((unsigned long)skb), &skb_c);

	shinfo = skb_shinfo(skb);

	async_msg_set_fn_type(_request,
			NAPI_GRO_RECEIVE);

	fipc_set_reg0(_request, skb_c->other_ref.cptr);

	if (shinfo->nr_frags) {
		skb_frag_t *frag = &shinfo->frags[0];

		fipc_set_reg1(_request,	gpa_val(lcd_gva2gpa(
			__gva(
			(unsigned long)lcd_page_address(
				skb_frag_page(frag))))));
		p = skb_frag_page(frag);
	}

	ret = lcd_virt_to_cptr(__gva((unsigned long)skb),
		&skb_cptr,
		&skb_sz,
		&skb_off);
	if (ret) {
		LIBLCD_ERR("lcd_virt_to_cptr");
		goto fail_virt;
	}

	ret = lcd_virt_to_cptr(__gva((unsigned long)skb->head),
		&skbh_cptr,
		&skbh_sz,
		&skbh_off);
	if (ret) {
		LIBLCD_ERR("lcd_virt_to_cptr");
		goto fail_virt;
	}

	lcd_unmap_virt(__gva((unsigned long)skb->head),
			get_order(skbh_sz));
	lcd_unmap_virt(__gva((unsigned long)skb),
			get_order(skb_sz));

	lcd_cap_delete(skb_c->skb_cptr);
	lcd_cap_delete(skb_c->skbh_cptr);


	glue_remove_skbuff(skb_c);
	kmem_cache_free(skb_c_cache1, skb_c);

	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);

	/* simulate the effect of put_page called by kfree_skb
	 * a page_ref_inc is done by the driver to make sure that
	 * this page is not freed and reused again
	 */
	if (p)
		page_ref_dec(p);

	return func_ret;
fail_async:
fail_ipc:
fail_virt:
	return ret;
}

struct sk_buff *__napi_alloc_skb(struct napi_struct *napi,
		unsigned int len,
		gfp_t gfp_mask)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	cptr_t skb_cptr, skbd_cptr;
	unsigned int skb_ord, skbd_ord;
	unsigned int skb_off, skbd_off, data_off;
	unsigned int request_cookie;
	cptr_t skb_ref;
	gva_t skb_gva, skbd_gva;
	struct sk_buff *skb;
	struct sk_buff_container *skb_c;

	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			__NAPI_ALLOC_SKB);
	fipc_set_reg1(_request,
			len);
	fipc_set_reg2(_request,
			gfp_mask);

	ret = thc_ipc_send_request(ixgbe_async,
			_request,
			&request_cookie);

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	/* sync half */
	ret = lcd_cptr_alloc(&skb_cptr);

	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_sync;
	}
	ret = lcd_cptr_alloc(&skbd_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_sync;
	}

	lcd_set_cr0(skb_cptr);
	lcd_set_cr1(skbd_cptr);
	ret = lcd_sync_recv(ixgbe_sync_endpoint);
	lcd_set_cr0(CAP_CPTR_NULL);
	lcd_set_cr1(CAP_CPTR_NULL);
	if (ret) {
		LIBLCD_ERR("failed to recv");
		goto fail_sync;
	}
	skb_ord = lcd_r0();
	skb_off = lcd_r1();
	skbd_ord = lcd_r2();
	skbd_off = lcd_r3();
	data_off = lcd_r4();
	skb_ref.cptr = lcd_r5();

	ret = lcd_map_virt(skb_cptr, skb_ord, &skb_gva);
	if (ret) {
		LIBLCD_ERR("failed to map void *addr");
		goto fail_sync;
	}

	ret = lcd_map_virt(skbd_cptr, skbd_ord, &skbd_gva);
	if (ret) {
		LIBLCD_ERR("failed to map void *addr");
		goto fail_sync;
	}

	skb = (void*)(gva_val(skb_gva) + skb_off);
	skb->head = (void*)(gva_val(skbd_gva) + skbd_off);
	skb->data = skb->head + data_off;
	skb_c = kmem_cache_alloc(skb_c_cache1, GFP_KERNEL);

	if (!skb_c)
		LIBLCD_MSG("no memory");
	skb_c->skb = skb;
	skb_c->skbd_ord = skbd_ord;
	glue_insert_skbuff(cptr_table, skb_c);

	skb_c->other_ref = skb_ref;
	skb_c->skb_cptr = skb_cptr;
	skb_c->skbh_cptr = skbd_cptr;

	ret = thc_ipc_recv_response(ixgbe_async,
		request_cookie,
		&_response);

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return skb;
fail_async:
fail_sync:
fail_ipc:
	return NULL;
}

#else

gro_result_t napi_gro_receive(struct napi_struct *napi,
		struct sk_buff *skb)
{
	int ret;
	struct fipc_message *_request;
#ifndef NAPI_RX_SEND_ONLY
	struct fipc_message *_response;
#endif
	int func_ret = 0;
	struct skb_shared_info *shinfo;
	struct page *p = NULL;
	u64 hash;

	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	shinfo = skb_shinfo(skb);

	async_msg_set_fn_type(_request,
			NAPI_GRO_RECEIVE);

	fipc_set_reg0(_request, shinfo->nr_frags |
				(skb->tail << 8));

	if (shinfo->nr_frags) {
		skb_frag_t *frag = &shinfo->frags[0];
		u64 frag_sz = frag->size;
		fipc_set_reg1(_request,	gpa_val(lcd_gva2gpa(
			__gva(
			(unsigned long)lcd_page_address(
				skb_frag_page(frag))))));
		p = skb_frag_page(frag);
		fipc_set_reg3(_request, frag->page_offset |
				(frag_sz << 32));
	}
	fipc_set_reg2(_request, skb->protocol);
	hash = skb->l4_hash | skb->sw_hash << 1;
	fipc_set_reg4(_request, skb->hash |
			(hash << 32));
	fipc_set_reg5(_request, skb->truesize);
	fipc_set_reg6(_request, skb->queue_mapping |
			(skb->csum_level << 16) |
			(skb->ip_summed << 18));

	//printk("%s, skb->tail %d", __func__, skb->tail);

	if (0)
	print_hex_dump(KERN_DEBUG, "Frame contents: ",
			       DUMP_PREFIX_OFFSET, 16, 1,
			       skb->data, skb->len, false);

#ifdef NAPI_RX_SEND_ONLY
	thc_set_msg_type(_request, msg_type_request);
	fipc_send_msg_end(thc_channel_to_fipc(ixgbe_async),
					_request);
#else
	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	func_ret = fipc_get_reg1(_response);

	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
#endif
	kmem_cache_free(skb_cache, skb);

	/* simulate the effect of put_page called by kfree_skb
	 * a page_ref_inc is done by the driver to make sure that
	 * this page is not freed and reused again
	 */
	if (p)
		page_ref_dec(p);

	return func_ret;
fail_async:
#ifndef NAPI_RX_SEND_ONLY
fail_ipc:
#endif
	return ret;
}

struct sk_buff *__napi_alloc_skb(struct napi_struct *napi,
			unsigned int len,
			gfp_t gfp)
{
	struct sk_buff *skb;
	unsigned int size = SKB_ALLOC_SIZE;
	void *data;
	struct skb_shared_info *shinfo;

	skb = kmem_cache_alloc(skb_cache, gfp);	

	memset(skb, 0x0, SKB_ALLOC_SIZE);

	if (!skb) {
		LIBLCD_ERR("%s: alloc failed", __func__);
		goto out;
	}
	data = ((char*)skb + sizeof(struct sk_buff));

	size -= SKB_DATA_ALIGN(sizeof(struct skb_shared_info));

	//memset(skb, 0, offsetof(struct sk_buff, tail));
	skb->truesize = SKB_TRUESIZE(size);
	skb->head = data;
	skb->data = data;
	skb_reset_tail_pointer(skb);
	skb->end = skb->tail + size;
	skb->mac_header = (typeof(skb->mac_header))~0U;
	skb->transport_header = (typeof(skb->transport_header))~0U;

	/* make sure we initialize shinfo sequentially */
	shinfo = skb_shinfo(skb);
	memset(shinfo, 0, offsetof(struct skb_shared_info, dataref));
//	printk("%s, alloc local skb", __func__);
out:
	return skb;
}
#endif

#if 0
__be16 eth_type_trans(struct sk_buff *skb,
		struct net_device *dev)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	__be16 func_ret;
	struct sk_buff_container *skb_c;

	glue_lookup_skbuff(cptr_table,
		__cptr((unsigned long)skb), &skb_c);
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(ixgbe_async,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			ETH_TYPE_TRANS);

	/* save pointers */
	skb_c->head = skb->head;
	skb_c->data = skb->data;

	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);

	fipc_set_reg2(_request, skb_c->other_ref.cptr);

	ret = thc_ipc_call(ixgbe_async,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);

	/* restore pointers */
	skb->head = skb_c->head;
	skb->data = skb_c->data;

	fipc_recv_msg_end(thc_channel_to_fipc(ixgbe_async),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}
#endif

int cleanup_channel_group(struct fipc_message *request, struct thc_channel *channel)
{
	int i;
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	for (i = 0; i < 32; i++) {
		if (ptrs[i]) {
			thc_channel_group_item_remove(&ch_grp, ptrs[i]);
			destroy_async_channel(ptrs[i]->channel);
			kfree(ptrs[i]);
			ptrs[i] = NULL;
		} //if
	} //for

	return 0;
}
