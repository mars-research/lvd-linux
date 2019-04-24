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
extern cptr_t ixgbe_sync_endpoint;
extern cptr_t ixgbe_register_channel;
extern struct thc_channel_group ch_grp;

static struct net_device *g_net_device;

#ifdef IOMMU_ASSIGN
/* device for IOMMU assignment */
struct pcidev_info dev_assign = { 0x0000, 0x06, 0x00, 0x1 };
#endif

struct kmem_cache *skb_c_cache;
struct kmem_cache *skb2_cache;
struct kmem_cache *skb_c_cache1;
struct kmem_cache *skb_cache;

#ifdef NAPI_STRUCT_ARRAY
struct napi_struct *napi_struct_array[32];
static int napi_reg = 0;
#endif
/* XXX: How to determine this? */
#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(cptr_table, CPTR_HASH_BITS);

#define NAPI_HASH_BITS	5
DEFINE_HASHTABLE(napi_hashtable, NAPI_HASH_BITS);

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

	hash_init(cptr_table);

        LIBLCD_MSG("dissector init ret %d",
                init_default_flow_dissectors());

	skb2_cache = kmem_cache_create("skb2_cache",
				sizeof(struct sk_buff_container_2),
				0,
				SLAB_HWCACHE_ALIGN|SLAB_PANIC,
				NULL);
	if (!skb2_cache) {
		LIBLCD_ERR("skb_container cache not created");
		goto fail2;
	}

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

int inline glue_insert_napi_hash(struct napi_struct_container *napi_c)
{
	/* assign address of napi_struct to napi */
	napi_c->napi = &napi_c->napi_struct;

	BUG_ON(!napi_c->napi);

	napi_c->my_ref = __cptr((unsigned long)napi_c->napi);

	hash_add(napi_hashtable, &napi_c->hentry, (unsigned long) napi_c->napi);

	return 0;
}

int inline glue_lookup_napi_hash(struct cptr c, struct napi_struct_container **napi_cout)
{
        struct napi_struct_container *napi_c;

        hash_for_each_possible(napi_hashtable, napi_c,
				hentry, (unsigned long) cptr_val(c)) {
		if (!napi_c) {
			WARN_ON(!napi_c);
			continue;
		}
		if (napi_c->napi == (struct napi_struct*) c.cptr) {
	                *napi_cout = napi_c;
		}
        }
        return 0;
}

void inline glue_remove_napi_hash(struct napi_struct_container *napi_c)
{
	hash_del(&napi_c->hentry);
}

void pci_disable_msix(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container;
	INIT_IPC_MSG(&r);

	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_DISABLE_MSIX);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	vmfunc_wrapper(_request);

	return;
}

int pci_enable_msix_range(struct pci_dev *dev, struct msix_entry *entries, int minvec, int maxvec)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	int sync_ret;
	unsigned 	long p_mem_sz;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	struct pci_dev_container *device_container;
	INIT_IPC_MSG(&r);

	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_ENABLE_MSIX_RANGE);

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

	fipc_set_reg2(_request, ilog2((p_mem_sz) >> (PAGE_SHIFT)));
	fipc_set_reg3(_request, p_offset);
	fipc_set_reg4(_request, cptr_val(p_cptr));
	fipc_set_reg5(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int __must_check __pci_register_driver(struct pci_driver *drv,
		struct module *owner,
		const char* name)
{
	struct pci_driver_container *drv_container;
	struct module_container *owner_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	INIT_IPC_MSG(&r);

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

	async_msg_set_fn_type(_request,
			__PCI_REGISTER_DRIVER);
	fipc_set_reg0(_request, drv_container->my_ref.cptr);
	fipc_set_reg1(_request, owner_container->my_ref.cptr);

	vmfunc_wrapper(_request);

	drv_container->other_ref.cptr = fipc_get_reg1(_request);
	owner_container->other_ref.cptr = fipc_get_reg2(_request);
	drv_container->pci_driver.driver.owner = &owner_container->module;
	func_ret = fipc_get_reg3(_request);

	LIBLCD_MSG("%s returned %d", __func__, func_ret);

	return func_ret;

fail_insert:
	return ret;
}

struct net_device *alloc_etherdev_mqs(int sizeof_priv,
		unsigned int txqs,
		unsigned int rxqs)
{
	struct net_device_container *func_ret_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	struct net_device *func_ret;
	struct net_device *dev;
	unsigned int alloc_size = sizeof(struct net_device_container);
	void *p;
	INIT_IPC_MSG(&r);

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

	vmfunc_wrapper(_request);

	func_ret_container->other_ref.cptr = fipc_get_reg1(_request);

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


	return func_ret;

fail_insert:
fail_alloc:
	return NULL;
}

/* FIXME: Handle this without extern */
extern struct pci_driver_container ixgbe_driver_container;
u64 dma_mask = 0;
void *data_pool;

struct pci_dev *g_pdev;

int probe_callee(struct fipc_message *_request)
{
	struct pci_dev_container *dev_container;

	int func_ret;
	int ret = 0;
	cptr_t other_ref;
	unsigned int devfn;

	cptr_t res0_cptr;
	gpa_t gpa_addr;
	unsigned int res0_len;
	volatile void *dev_resource_0;
	cptr_t pool_cptr;
	gva_t pool_addr;
	unsigned int pool_ord;

	LIBLCD_MSG("%s called", __func__);
	other_ref.cptr = fipc_get_reg1(_request);
	dma_mask = fipc_get_reg2(_request);

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

	g_pdev = &dev_container->pci_dev;

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

	fipc_set_reg0(_request, cptr_val(res0_cptr));
	fipc_set_reg1(_request, cptr_val(pool_cptr));
	fipc_set_reg2(_request, dev_container->other_ref.cptr);

	vmfunc_sync_call(_request, SYNC_PROBE);

	res0_len = fipc_get_reg0(_request);
	pool_ord = fipc_get_reg1(_request);

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
	LIBLCD_MSG("%s: status reg 0x%X", __func__, *(unsigned int *)((char*)dev_resource_0 + 0x8));

	ret = lcd_map_virt(pool_cptr, pool_ord, &pool_addr);
	if (ret) {
		LIBLCD_ERR("failed to map pool");
		goto fail_pool;
	}

	LIBLCD_MSG("%s, mapping private pool %p | ord %d", __func__,
			gva_val(pool_addr), pool_ord);

	data_pool = (void*)gva_val(pool_addr);

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

	/* XXX: Pass null for now. struct pci_dev is passed from the
	 * kernel and needs analysis for wrapping it around a container
	 */
	func_ret = ixgbe_driver_container.pci_driver.probe(&dev_container->pci_dev, NULL);

	fipc_set_reg0(_request, func_ret);

fail_ioremap:
fail_ioremap2:
fail_alloc:
fail_insert:
fail_cptr:
fail_pool:
	return ret;
}

void pci_unregister_driver(struct pci_driver *drv)
{
	struct pci_driver_container *drv_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	INIT_IPC_MSG(&r);

	drv_container = container_of(drv,
		struct pci_driver_container,
		pci_driver);

	async_msg_set_fn_type(_request, PCI_UNREGISTER_DRIVER);
	fipc_set_reg0(_request, drv_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	glue_cap_remove(c_cspace, drv_container->my_ref);

	return;
}

int remove_callee(struct fipc_message *_request)
{
	/* async loop should get 0 */
	int ret = 0;

	unsigned int __maybe_unused devfn;


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
	ixgbe_driver_container.pci_driver.remove(g_pdev);

	return ret;
}

int register_netdev(struct net_device *dev)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;

	INIT_IPC_MSG(&r);
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

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

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg1(_request);
	dev->reg_state = fipc_get_reg2(_request);

	return func_ret;
}

void ether_setup(struct net_device *dev)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	INIT_IPC_MSG(&r);

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			ETHER_SETUP);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	vmfunc_wrapper(_request);

	return;
}

int eth_mac_addr(struct net_device *dev,
		void *p)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int sync_ret;
	unsigned 	long p_mem_sz;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	int func_ret;

	INIT_IPC_MSG(&r);
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			ETH_MAC_ADDR);
	fipc_set_reg0(_request,
			dev_container->other_ref.cptr);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )p), &p_cptr,
			&p_mem_sz, &p_offset);

	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}

	fipc_set_reg1(_request, cptr_val(p_cptr));
	fipc_set_reg2(_request, ilog2(( p_mem_sz ) >> ( PAGE_SHIFT )));
	fipc_set_reg3(_request, p_offset);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int eth_validate_addr(struct net_device *dev)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;

	INIT_IPC_MSG(&r);

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			ETH_VALIDATE_ADDR);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	vmfunc_wrapper(_request);
	func_ret = fipc_get_reg1(_request);

	return func_ret;
}

void free_netdev(struct net_device *dev)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	INIT_IPC_MSG(&r);

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			FREE_NETDEV);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	vmfunc_wrapper(_request);

	return;
}

void netif_carrier_off(struct net_device *dev)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	INIT_IPC_MSG(&r);

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			NETIF_CARRIER_OFF);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	vmfunc_wrapper(_request);

	dev->state = fipc_get_reg1(_request);

	return;
}

void netif_carrier_on(struct net_device *dev)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	INIT_IPC_MSG(&r);

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			NETIF_CARRIER_ON);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	vmfunc_wrapper(_request);
	dev->state = fipc_get_reg1(_request);

	return;
}

void netif_device_attach(struct net_device *dev)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	INIT_IPC_MSG(&r);

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			NETIF_DEVICE_ATTACH);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	vmfunc_wrapper(_request);
	dev->state = fipc_get_reg1(_request);

	return;
}

void netif_device_detach(struct net_device *dev)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	INIT_IPC_MSG(&r);

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			NETIF_DEVICE_DETACH);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	vmfunc_wrapper(_request);
	dev->state = fipc_get_reg1(_request);

	return;
}

int netif_set_real_num_rx_queues(struct net_device *dev,
		unsigned int rxq)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;

	INIT_IPC_MSG(&r);
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			NETIF_SET_REAL_NUM_RX_QUEUES);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg3(_request,
			rxq);
	vmfunc_wrapper(_request);
	func_ret = fipc_get_reg1(_request);

	return func_ret;
}

int netif_set_real_num_tx_queues(struct net_device *dev,
		unsigned int txq)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;

	INIT_IPC_MSG(&r);
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			NETIF_SET_REAL_NUM_TX_QUEUES);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg3(_request,
			txq);
	LIBLCD_MSG("%s, setting real_num_tx_queues to %d", __func__, txq);
	vmfunc_wrapper(_request);
	func_ret = fipc_get_reg1(_request);

	return func_ret;
}

void napi_consume_skb(struct sk_buff *skb, int budget)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct sk_buff_container_2 *skb_c = NULL;
	INIT_IPC_MSG(&r);

	skb_c = container_of(skb, struct sk_buff_container_2,
				skb);

	/* TODO: handle lookup failure */
	async_msg_set_fn_type(_request, NAPI_CONSUME_SKB);

	fipc_set_reg0(_request, skb_c->other_ref.cptr);
	fipc_set_reg1(_request, budget);

	vmfunc_wrapper(_request);

	kmem_cache_free(skb2_cache, skb_c);
	return;
}

void consume_skb(struct sk_buff *skb)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct sk_buff_container_2 *skb_c = NULL;
	INIT_IPC_MSG(&r);

	async_msg_set_fn_type(_request, CONSUME_SKB);

	skb_c = container_of(skb, struct sk_buff_container_2,
				skb);

	fipc_set_reg0(_request, skb_c->other_ref.cptr);

	vmfunc_wrapper(_request);

	kfree(skb);

	kfree(skb_c);

	return;
}

void unregister_netdev(struct net_device *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct net_device_container *dev_container;
	INIT_IPC_MSG(&r);

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			UNREGISTER_NETDEV);
	fipc_set_reg0(_request,
			dev_container->other_ref.cptr);
	vmfunc_wrapper(_request);

	return;
}

int eth_platform_get_mac_address(struct device *dev,
		u8 *mac_addr)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	int func_ret;
	int i;
	union mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };

	INIT_IPC_MSG(&r);

	async_msg_set_fn_type(_request,
			ETH_PLATFORM_GET_MAC_ADDRESS);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg1(_request);

	if (func_ret < 0) {
		m.mac_addr_l = fipc_get_reg2(_request);
		memcpy(mac_addr, m.mac_addr, ETH_ALEN);
		for (i = 0; i < ETH_ALEN; i++) {
			printk("%02X:", m.mac_addr[i]);
		}
		LIBLCD_MSG("\n%s, got 0x%06lX", __func__, m.mac_addr_l);
	}
	return func_ret;
}

int dev_addr_add(struct net_device *dev,
		const unsigned char *addr,
		unsigned char addr_type)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
#ifdef PASS_DEV_ADDR_IN_REG
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };
#else
	int sync_ret;
	unsigned 	long addr_mem_sz;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
#endif
	int func_ret;

	INIT_IPC_MSG(&r);

	dev_container = container_of(dev, struct net_device_container,
			net_device);

	async_msg_set_fn_type(_request, DEV_ADDR_ADD);

#ifndef PASS_DEV_ADDR_IN_REG
	sync_ret = lcd_virt_to_cptr(__gva((unsigned long)addr),
			&addr_cptr, &addr_mem_sz, &addr_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
#endif

	fipc_set_reg0(_request, dev_container->other_ref.cptr);
	fipc_set_reg1(_request, addr_type);
#ifdef PASS_DEV_ADDR_IN_REG
	memcpy(m.mac_addr, addr, dev->addr_len);
	fipc_set_reg2(_request, m.mac_addr_l);
#else
	fipc_set_reg2(_request, ilog2(( addr_mem_sz ) >> ( PAGE_SHIFT )));
	fipc_set_reg3(_request, addr_offset);
	fipc_set_reg4(_request, cptr_val(addr_cptr));
#endif
	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	/* call local function */
	liblcd_dev_addr_add(dev, addr, addr_type);
	return func_ret;
}

int dev_addr_del(struct net_device *dev,
		const unsigned char *addr,
		unsigned char addr_type)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
#ifdef PASS_DEV_ADDR_IN_REG
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };
#else
	int sync_ret;
	unsigned 	long addr_mem_sz;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
#endif
	int func_ret;
	INIT_IPC_MSG(&r);

	dev_container = container_of(dev, struct net_device_container,
			net_device);
#ifndef PASS_DEV_ADDR_IN_REG
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )addr),
			&addr_cptr, &addr_mem_sz, &addr_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
#endif
	async_msg_set_fn_type(_request, DEV_ADDR_DEL);
	fipc_set_reg0(_request, dev_container->other_ref.cptr);
	fipc_set_reg1(_request, addr_type);

#ifdef PASS_DEV_ADDR_IN_REG
	memcpy(m.mac_addr, addr, dev->addr_len);
	fipc_set_reg2(_request, m.mac_addr_l);
#else
	fipc_set_reg2(_request, ilog2(( addr_mem_sz ) >> ( PAGE_SHIFT )));
	fipc_set_reg3(_request, addr_offset);
	fipc_set_reg4(_request, cptr_val(addr_cptr));
#endif
	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	/* call local function */
	liblcd_dev_addr_del(dev, addr, addr_type);
	return func_ret;
}

int device_set_wakeup_enable(struct device *dev,
		bool enable)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;

	INIT_IPC_MSG(&r);

	async_msg_set_fn_type(_request,
			DEVICE_SET_WAKEUP_ENABLE);
	fipc_set_reg1(_request,
			enable);
	vmfunc_wrapper(_request);
	func_ret = fipc_get_reg1(_request);

	return func_ret;
}

void netif_tx_stop_all_queues(struct net_device *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct net_device_container *dev_container;

	INIT_IPC_MSG(&r);


	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	async_msg_set_fn_type(_request,
			NETIF_TX_STOP_ALL_QUEUES);

	fipc_set_reg0(_request,
			dev_container->other_ref.cptr);
	vmfunc_wrapper(_request);

	return;
}

void _netif_tx_wake_all_queues(struct net_device *dev)
{
	struct net_device_container *dev_queue_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;

	INIT_IPC_MSG(&r);

	dev_queue_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			NETIF_TX_WAKE_ALL_QUEUES);
	fipc_set_reg1(_request,
			dev_queue_container->other_ref.cptr);
	fipc_set_reg2(_request,
			dev->num_tx_queues);
	vmfunc_wrapper(_request);

	return;
}

int pci_disable_pcie_error_reporting(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);

	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_DISABLE_PCIE_ERROR_REPORTING);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int pci_bus_read_config_word(struct pci_bus *bus,
		unsigned int devfn,
		int where,
		unsigned short *val)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;

	INIT_IPC_MSG(&r);
	async_msg_set_fn_type(_request, PCI_BUS_READ_CONFIG_WORD);
	fipc_set_reg0(_request, devfn);
	fipc_set_reg1(_request, where);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	*val = fipc_get_reg1(_request);

	return func_ret;
}

int pci_bus_write_config_word(struct pci_bus *bus,
		unsigned int devfn,
		int where,
		unsigned short val)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;

	INIT_IPC_MSG(&r);
	async_msg_set_fn_type(_request, PCI_BUS_WRITE_CONFIG_WORD);
	fipc_set_reg0(_request, devfn);
	fipc_set_reg1(_request, where);
	fipc_set_reg2(_request, val);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int pci_cleanup_aer_uncorrect_error_status(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);
	async_msg_set_fn_type(_request,
			PCI_CLEANUP_AER_UNCORRECT_ERROR_STATUS);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

void pci_disable_device(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_DISABLE_DEVICE);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	return;
}

int pci_enable_pcie_error_reporting(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);
	async_msg_set_fn_type(_request, PCI_ENABLE_PCIE_ERROR_REPORTING);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int pcie_capability_read_word(struct pci_dev *dev,
		int pos,
		unsigned short *val)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCIE_CAPABILITY_READ_WORD);

	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, pos);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int pcie_get_minimum_link(struct pci_dev *dev,
		enum pci_bus_speed *speed,
		enum pcie_link_width *width)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);
	async_msg_set_fn_type(_request,
			PCIE_GET_MINIMUM_LINK);

	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, *speed);
	fipc_set_reg2(_request, *width);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int pci_enable_device_mem(struct pci_dev *dev)
{
	int func_ret = 0;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_ENABLE_DEVICE_MEM);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int pci_request_selected_regions(struct pci_dev *dev,
		int type,
		const char *reg)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_REQUEST_SELECTED_REGIONS);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, type);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int pci_request_selected_regions_exclusive(struct pci_dev *dev,
		int type,
		const char *reg)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);
	async_msg_set_fn_type(_request,
			PCI_REQUEST_SELECTED_REGIONS_EXCLUSIVE);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, type);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

void pci_set_master(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);
	async_msg_set_fn_type(_request, PCI_SET_MASTER);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	return;
}

int pci_save_state(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);
	async_msg_set_fn_type(_request, PCI_SAVE_STATE);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

void pci_release_selected_regions(struct pci_dev *dev, int r)
{
	struct fipc_message req;
	struct fipc_message *_request = &req;
	struct pci_dev_container *device_container;

	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_RELEASE_SELECTED_REGIONS);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, r);

	vmfunc_wrapper(_request);

	return;
}

int pci_select_bars(struct pci_dev *dev, unsigned long flags)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret = 0;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_SELECT_BARS);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, flags);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

int pci_wake_from_d3(struct pci_dev *dev, bool enable)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret = 0;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_WAKE_FROM_D3);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, enable);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int ndo_open_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;
	int func_ret = 0;
	ret = glue_cap_lookup_net_device_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	func_ret = dev_container->net_device.netdev_ops->ndo_open(( &dev_container->net_device ));

	LIBLCD_MSG("%s, returns %d", __func__, func_ret);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int ndo_stop_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;

	int func_ret;
	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	func_ret = dev_container->net_device.netdev_ops->ndo_stop(( &dev_container->net_device ));

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;

}

extern netdev_tx_t ixgbe_xmit_frame(struct sk_buff *skb,
				struct net_device *dev);
extern struct net_device *g_netdev;

int ndo_start_xmit_callee(struct fipc_message *_request)
{
	struct sk_buff *skb;
#ifdef SKB_CONTAINER_HAS_SKBP
	struct sk_buff_container *skb_c = NULL;
#else
	struct sk_buff_container_2 *skb_c = NULL;
#endif
	int ret = 0;
	int func_ret;
	cptr_t skb_ref;
	xmit_type_t xmit_type;
	unsigned long skbh_offset, skb_end;
	struct skbuff_members *skb_lcd;
	__be16 proto;
#ifdef SKB_CONTAINER_HAS_SKBP
	void *mem;
#endif

#ifdef LCD_MEASUREMENT
	TS_DECL(xmit);
#endif
	xmit_type = fipc_get_reg0(_request);
	skb_ref = __cptr(fipc_get_reg2(_request));
	skbh_offset = fipc_get_reg3(_request);
	skb_end = fipc_get_reg4(_request);
	proto = fipc_get_reg5(_request);

#ifdef SKB_CONTAINER_HAS_SKBP
	skb_c = kmem_cache_alloc(skb_c_cache, GFP_KERNEL);
#else
	skb_c = kmem_cache_alloc(skb2_cache, GFP_KERNEL);
#endif

	if (!skb_c) {
		LIBLCD_MSG("out of mmeory");
		goto fail_alloc;
	}
#ifndef SKB_CONTAINER_HAS_SKBP
	skb = &skb_c->skb;
#endif

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

#ifdef SKB_CONTAINER_HAS_SKBP
	skb_c->skb = skb;
#endif

#ifdef SKB_CONTAINER_HAS_SKBP
	if (0)
		glue_insert_skbuff(cptr_table, skb_c);
#endif

	skb_c->other_ref = skb_ref;

#ifdef LCD_MEASUREMENT
	TS_START_LCD(xmit);
#endif
	func_ret = ixgbe_xmit_frame(skb, g_netdev);

	/* packet transmission failed - free skb_c right here */
	if (func_ret) {
		kmem_cache_free(skb2_cache, skb_c);
	}

#ifdef LCD_MEASUREMENT
	TS_STOP_LCD(xmit);
#endif

	async_msg_set_fn_type(_request, 0xdeadbeef);
	fipc_set_reg0(_request, func_ret);

#ifdef LCD_MEASUREMENT
	fipc_set_reg2(_request,
			TS_DIFF(xmit));
#endif

fail_alloc:
	return ret;
}

int ndo_set_rx_mode_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	dev_container->net_device.netdev_ops->ndo_set_rx_mode(( &dev_container->net_device ));

fail_lookup:
	return ret;

}

int ndo_validate_addr_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;

	int func_ret;
	ret = glue_cap_lookup_net_device_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	func_ret = dev_container->net_device.netdev_ops->ndo_validate_addr(( &dev_container->net_device));

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int ndo_set_mac_address_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;

	int func_ret;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
	gva_t addr_gva;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	sync_ret = lcd_cptr_alloc(&addr_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}

	fipc_set_reg0(_request, cptr_val(addr_cptr));

	vmfunc_sync_call(_request, SYNC_NDO_SET_MAC_ADDRESS);

	mem_order = fipc_get_reg0(_request);
	addr_offset = fipc_get_reg1(_request);

	sync_ret = lcd_map_virt(addr_cptr, mem_order, &addr_gva);

	if (sync_ret) {
		LIBLCD_ERR("failed to map void *addr");
		lcd_exit(-1);
	}
	func_ret = dev_container->net_device.netdev_ops->ndo_set_mac_address(( &dev_container->net_device ),
		( void  * )( ( gva_val(addr_gva) ) + ( addr_offset ) ));

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;

}

int ndo_change_mtu_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int new_mtu;
	int ret;

	int func_ret;
	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	new_mtu = fipc_get_reg1(_request);
	func_ret = dev_container->net_device.netdev_ops->ndo_change_mtu(( &dev_container->net_device ),
		new_mtu);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;

}

int ndo_tx_timeout_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;


	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	dev_container->net_device.netdev_ops->ndo_tx_timeout(( &dev_container->net_device ));

fail_lookup:
	return ret;

}

int ndo_set_tx_maxrate_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int queue_index;
	unsigned 	int maxrate;
	int ret;

	int func_ret;
	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	queue_index = fipc_get_reg1(_request);
	maxrate = fipc_get_reg2(_request);
	func_ret = dev_container->net_device.netdev_ops->ndo_set_tx_maxrate(( &dev_container->net_device ),
		queue_index,
		maxrate);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;

}

int ndo_get_stats64_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	struct rtnl_link_stats64 stats;
	int ret;


	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}


	dev_container->net_device.netdev_ops->ndo_get_stats64(( &dev_container->net_device ),
		&stats);


	fipc_set_reg0(_request, stats.rx_packets);
	fipc_set_reg1(_request, stats.rx_bytes);
	fipc_set_reg2(_request, stats.tx_packets);
	fipc_set_reg3(_request, stats.tx_bytes);

fail_lookup:
	return ret;
}

extern void __ixgbe_service_event_schedule(struct net_device *netdev);

int ixgbe_service_event_schedule_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;
	ret = glue_cap_lookup_net_device_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	if (0)
		printk("%s, napi %p, napi->state %lx", __func__, napi_struct_array[0],
					napi_struct_array[0]->state);

	__ixgbe_service_event_schedule(&dev_container->net_device);

fail_lookup:
	return ret;
}

extern void __ixgbe_dump(struct net_device *);

int trigger_dump_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret = 0;
	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

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
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;

	INIT_IPC_MSG(&r);
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

	async_msg_set_fn_type(_request,
			__HW_ADDR_SYNC_DEV);
	fipc_set_reg1(_request,
			dev1_container->other_ref.cptr);
	fipc_set_reg2(_request, list == &dev1->mc ? MC_LIST : UC_LIST);

	vmfunc_wrapper(_request);
	func_ret = fipc_get_reg1(_request);

	liblcd__hw_addr_sync_dev(list, dev1, sync, unsync);
	return func_ret;

fail_alloc:
	return ret;
}

void __hw_addr_unsync_dev(
		struct netdev_hw_addr_list *list,
		struct net_device *dev1,
		int ( *unsync )(struct net_device *, const unsigned char*))
{
	struct net_device_container *dev1_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;

	INIT_IPC_MSG(&r);
	dev1_container = container_of(dev1,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			__HW_ADDR_UNSYNC_DEV);
	fipc_set_reg1(_request,
			dev1_container->other_ref.cptr);
	fipc_set_reg2(_request, list == &dev1->mc ? MC_LIST : UC_LIST);
	vmfunc_wrapper(_request);

	liblcd__hw_addr_unsync_dev(list, dev1, unsync);
	return;
}

int sync_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;
	int func_ret;
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}


	m.mac_addr_l = fipc_get_reg3(_request);

	func_ret = sync_container->sync(( &dev_container->net_device ),
		m.mac_addr);


	fipc_set_reg1(_request,
			func_ret);

fail_lookup:
	return ret;
}

int unsync_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;
	int func_ret;
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	m.mac_addr_l = fipc_get_reg3(_request);


	func_ret = unsync_container->unsync(( &dev_container->net_device ),
		m.mac_addr);

	fipc_set_reg1(_request, func_ret);

	return ret;

fail_lookup:
	return ret;
}

int msix_vector_handler_callee(struct fipc_message *_request)
{
	struct irqhandler_t_container *irqhandler_container;
	irqreturn_t irqret;
	int irq;
	int ret = 0;

	irq = fipc_get_reg0(_request);
	irqhandler_container = (struct irqhandler_t_container *) fipc_get_reg1(_request); 

	/* call real irq handler */
	irqret = irqhandler_container->irqhandler(irq, irqhandler_container->data); 

	/* printk("%s, irq:%d irqret %d\n", __func__, irq, irqret); */
	fipc_set_reg0(_request, irqret);
	return ret;
}

/* typedef irqreturn_t (*irq_handler_t)(int, void *); */
int request_threaded_irq(unsigned int irq, irq_handler_t handler,
				irq_handler_t thread_fn,
				unsigned long flags, const char *name, void *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct irqhandler_t_container *irqhandler_container;
	int func_ret;

	INIT_IPC_MSG(&r);
	printk("%s, irq # %d", __func__, irq);

	irqhandler_container = kzalloc(sizeof(struct irqhandler_t_container), GFP_KERNEL);

	if (!irqhandler_container) {
		LIBLCD_ERR("kzalloc");
		func_ret = -ENOMEM;
		goto fail_alloc;
	}

	irqhandler_container->irqhandler = handler;
	/*
	 * XXX: in irq context, we don't want to be doing a lookup.
	 * so save the handler_container pointer as myref
	 */
	irqhandler_container->my_ref.cptr = (unsigned long) irqhandler_container;
	irqhandler_container->data = dev;

	async_msg_set_fn_type(_request, REQUEST_THREADED_IRQ);
	fipc_set_reg0(_request, irq);
	fipc_set_reg1(_request, irqhandler_container->my_ref.cptr);
	fipc_set_reg2(_request, flags);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

fail_alloc:
	return func_ret;
}

void free_irq(unsigned int irq, void *dev_id)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	INIT_IPC_MSG(&r);
	async_msg_set_fn_type(_request, FREE_IRQ);

	fipc_set_reg0(_request, irq);

	vmfunc_wrapper(_request);

	return;
}

void netif_napi_add(struct net_device *dev,
		struct napi_struct *napi,
		int (*poll)(struct napi_struct*, int),
		int weight)
{
	struct net_device_container *dev_container;
	struct poll_container *poll_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct napi_struct_container *napi_container;

	INIT_IPC_MSG(&r);
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	napi_container = container_of(napi, struct napi_struct_container, napi_struct);

	poll_container = kzalloc(sizeof(struct poll_container), GFP_KERNEL);

	if (!poll_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	glue_insert_napi_hash(napi_container);

	LIBLCD_MSG("napi_struct_my_ref %lx", napi_container->my_ref.cptr);

	poll_container->poll = poll;

	async_msg_set_fn_type(_request, NETIF_NAPI_ADD);
	fipc_set_reg0(_request, dev_container->other_ref.cptr);
	fipc_set_reg2(_request, weight);

#ifdef NAPI_STRUCT_ARRAY
	/* XXX: napi_hashtable creates some trouble */
	napi_struct_array[napi_reg] = napi;
	printk("%s, adding napi: %p to napi_struct_array idx: %d",
					__func__, napi, napi_reg);
	/* pass the idx into napi_struct array */
	fipc_set_reg1(_request, napi_reg);
	napi_reg++;
#else
	fipc_set_reg1(_request, napi_container->my_ref.cptr);
#endif
	vmfunc_wrapper(_request);

	napi_container->other_ref = __cptr(fipc_get_reg0(_request));


	napi->state = fipc_get_reg1(_request);

fail_alloc:
	return;
}

extern int ixgbe_poll(struct napi_struct *napi, int budget);

int poll_callee(struct fipc_message *_request)
{
	int budget;
#ifdef NAPI_STRUCT_ARRAY
	int napi_idx;
#else
	struct napi_struct_container *napi_c = NULL;
#endif
	struct napi_struct *napi;
	static int count = 0;
	int ret;

	budget = fipc_get_reg0(_request);

#ifdef NAPI_STRUCT_ARRAY
	napi_idx = fipc_get_reg1(_request);
	if (napi_idx > 32) {
		printk("%s, napi_idx 32! BUG", __func__);
	}
	napi = napi_struct_array[napi_idx];
#else
	glue_lookup_napi_hash(__cptr(fipc_get_reg1(_request)), &napi_c);
	BUG_ON(!napi_c->napi);
	napi = napi_c->napi;
#endif

	if (!count) {
		//printk("%s, napi_c %p | napi_c->napi %p", __func__, napi_c,
		//		napi_c ? napi_c->napi : NULL);
		//printk("%s, napi %p", __func__, napi);
		printk("%s, napi->state 0x%lx budget:%d", __func__,
						napi->state, budget);
		count = 1;
	}

	napi->state = fipc_get_reg2(_request);

	ret = ixgbe_poll(napi, budget);

	fipc_set_reg0(_request, ret);
	//fipc_set_reg1(_request, napi->state);

	return 0;
}

void netif_napi_del(struct napi_struct *napi)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct napi_struct_container *napi_container;

	INIT_IPC_MSG(&r);
	napi_container = container_of(napi, struct napi_struct_container, napi_struct);

	async_msg_set_fn_type(_request, NETIF_NAPI_DEL);

	fipc_set_reg0(_request, napi_container->other_ref.cptr);
	//fipc_set_reg1(_request, napi->state);

	vmfunc_wrapper(_request);

	napi->state = fipc_get_reg0(_request);

	return;
}

void netif_wake_subqueue(struct net_device *dev,
		unsigned short queue_index)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;

	INIT_IPC_MSG(&r);
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request, NETIF_WAKE_SUBQUEUE);
	fipc_set_reg1(_request, dev_container->other_ref.cptr);
	fipc_set_reg3(_request, queue_index);

	vmfunc_wrapper(_request);

	return;
}

int netif_receive_skb(struct sk_buff *skb)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	int func_ret;
	struct sk_buff_container *skb_c = NULL;


	INIT_IPC_MSG(&r);
	glue_lookup_skbuff(cptr_table,
		__cptr((unsigned long)skb), &skb_c);

	async_msg_set_fn_type(_request,
			NETIF_RECEIVE_SKB);

	fipc_set_reg0(_request, skb_c->other_ref.cptr);

	glue_remove_skbuff(skb_c);
	kfree(skb_c);

	vmfunc_wrapper(_request);
	func_ret = fipc_get_reg1(_request);

	return func_ret;
}

#ifndef LOCAL_SKB
gro_result_t napi_gro_receive(struct napi_struct *napi,
		struct sk_buff *skb)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	int func_ret;
	struct sk_buff_container *skb_c;
	unsigned long skb_sz, skb_off, skbh_sz, skbh_off;
	cptr_t skb_cptr, skbh_cptr;
	struct skb_shared_info *shinfo;
	struct page *p = NULL;
	struct napi_struct_container *napi_container;

	INIT_IPC_MSG(&r);
	napi_container = container_of(napi, struct napi_struct_container, napi_struct);

	glue_lookup_skbuff(cptr_table,
		__cptr((unsigned long)skb), &skb_c);

	shinfo = skb_shinfo(skb);

	async_msg_set_fn_type(_request, NAPI_GRO_RECEIVE);

	fipc_set_reg0(_request, skb_c->other_ref.cptr);
	fipc_set_reg1(_request, napi_container->other_ref.cptr);

	if (shinfo->nr_frags) {
		skb_frag_t *frag = &shinfo->frags[0];

		fipc_set_reg2(_request,	gpa_val(lcd_gva2gpa(
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

	vmfunc_wrapper(_request);
	func_ret = fipc_get_reg1(_request);


	/* simulate the effect of put_page called by kfree_skb
	 * a page_ref_inc is done by the driver to make sure that
	 * this page is not freed and reused again
	 */
	if (p)
		page_ref_dec(p);

	return func_ret;


fail_virt:
	return ret;
}

struct sk_buff *__napi_alloc_skb(struct napi_struct *napi,
		unsigned int len,
		gfp_t gfp_mask)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	cptr_t skb_cptr, skbd_cptr;
	unsigned int skb_ord, skbd_ord;
	unsigned int skb_off, skbd_off, data_off;
	cptr_t skb_ref;
	gva_t skb_gva, skbd_gva;
	struct sk_buff *skb;
	struct sk_buff_container *skb_c;

	INIT_IPC_MSG(&r);
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


	async_msg_set_fn_type(_request, __NAPI_ALLOC_SKB);
	fipc_set_reg1(_request, len);
	fipc_set_reg2(_request, gfp_mask);
	fipc_set_reg3(_request, cptr_val(skb_cptr));
	fipc_set_reg4(_request, cptr_val(skbd_cptr));

	vmfunc_wrapper(_request);

	skb_ord = fipc_get_reg0(_request);
	skb_off = fipc_get_reg1(_request);
	skbd_ord = fipc_get_reg2(_request);
	skbd_off = fipc_get_reg3(_request);
	data_off = fipc_get_reg4(_request);
	skb_ref.cptr = fipc_get_reg5(_request);

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

	return skb;
}

#else

gro_result_t napi_gro_receive(struct napi_struct *napi,
		struct sk_buff *skb)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret = 0;
	struct skb_shared_info *shinfo;
	struct page *p = NULL;
	u64 hash;
	struct napi_struct_container *napi_container;

	INIT_IPC_MSG(&r);
	napi_container = container_of(napi, struct napi_struct_container, napi_struct);

	shinfo = skb_shinfo(skb);

	async_msg_set_fn_type(_request, NAPI_GRO_RECEIVE);

	fipc_set_reg0(_request, shinfo->nr_frags | (skb->tail << 8));

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
	fipc_set_reg2(_request, skb->protocol |
			(napi_container->other_ref.cptr << 16));
	hash = skb->l4_hash | skb->sw_hash << 1;
	fipc_set_reg4(_request, skb->hash |
			(hash << 32));
	fipc_set_reg5(_request, skb->truesize);
	fipc_set_reg6(_request, skb->queue_mapping |
			(skb->csum_level << 16) |
			(skb->ip_summed << 18));

	//printk("%s, skb->tail %d | napi_c %lx", __func__, skb->tail, napi_container->other_ref.cptr);

	if (0)
	print_hex_dump(KERN_DEBUG, "Frame contents: ",
			       DUMP_PREFIX_OFFSET, 16, 1,
			       skb->data, skb->len, false);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg1(_request);

	kmem_cache_free(skb_cache, skb);

	/* simulate the effect of put_page called by kfree_skb
	 * a page_ref_inc is done by the driver to make sure that
	 * this page is not freed and reused again
	 */
	if (p)
		page_ref_dec(p);

	return func_ret;
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
	//printk("%s, alloc local skb for rx", __func__);
out:
	return skb;
}
#endif

#if 0
__be16 eth_type_trans(struct sk_buff *skb,
		struct net_device *dev)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;

	__be16 func_ret;
	struct sk_buff_container *skb_c;

	glue_lookup_skbuff(cptr_table,
		__cptr((unsigned long)skb), &skb_c);
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			ETH_TYPE_TRANS);

	/* save pointers */
	skb_c->head = skb->head;
	skb_c->data = skb->data;

	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);

	fipc_set_reg2(_request, skb_c->other_ref.cptr);

	vmfunc_wrapper(_request);
	func_ret = fipc_get_reg1(_request);

	/* restore pointers */
	skb->head = skb_c->head;
	skb->data = skb_c->data;


	return func_ret;

	return ret;
}
#endif

void ___napi_schedule_irqoff(struct napi_struct *napi)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct napi_struct_container *napi_container;

	INIT_IPC_MSG(&r);
	napi_container = container_of(napi, struct napi_struct_container, napi_struct);

	async_msg_set_fn_type(_request, ___NAPI_SCHEDULE_IRQOFF);

	fipc_set_reg0(_request, napi_container->other_ref.cptr);
	//fipc_set_reg1(_request, napi->state);

	vmfunc_wrapper(_request);

	napi->state = fipc_get_reg0(_request);

	return;
}


void __napi_schedule_irqoff(struct napi_struct *napi)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct napi_struct_container *napi_container;

	INIT_IPC_MSG(&r);
	napi_container = container_of(napi, struct napi_struct_container, napi_struct);

	async_msg_set_fn_type(_request, __NAPI_SCHEDULE_IRQOFF);

	fipc_set_reg0(_request, napi_container->other_ref.cptr);
	//fipc_set_reg1(_request, napi->state);

	vmfunc_wrapper(_request);

	napi->state = fipc_get_reg0(_request);

	return;
}

void __napi_enable(struct napi_struct *napi)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct napi_struct_container *napi_container;

	INIT_IPC_MSG(&r);
	napi_container = container_of(napi, struct napi_struct_container, napi_struct);

	async_msg_set_fn_type(_request, __NAPI_ENABLE);

	fipc_set_reg0(_request, napi_container->other_ref.cptr);
	//fipc_set_reg1(_request, napi->state);

	vmfunc_wrapper(_request);

	napi->state = fipc_get_reg0(_request);

	return;
}

bool napi_hash_del(struct napi_struct *napi)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct napi_struct_container *napi_container;
	bool sync_needed;

	INIT_IPC_MSG(&r);
	napi_container = container_of(napi, struct napi_struct_container, napi_struct);

	async_msg_set_fn_type(_request, NAPI_HASH_DEL);

	fipc_set_reg0(_request, napi_container->other_ref.cptr);
	//fipc_set_reg1(_request, napi->state);

	vmfunc_wrapper(_request);

	sync_needed = fipc_get_reg0(_request);
	napi->state = fipc_get_reg1(_request);

	return sync_needed;
}

void napi_disable(struct napi_struct *napi)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct napi_struct_container *napi_container;

	INIT_IPC_MSG(&r);
	napi_container = container_of(napi, struct napi_struct_container, napi_struct);

	async_msg_set_fn_type(_request, NAPI_DISABLE);

	fipc_set_reg0(_request, napi_container->other_ref.cptr);
	//fipc_set_reg1(_request, napi->state);

	vmfunc_wrapper(_request);

	napi->state = fipc_get_reg0(_request);

	return;
}

void napi_complete_done(struct napi_struct *napi, int work_done)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct napi_struct_container *napi_container;

	INIT_IPC_MSG(&r);
	napi_container = container_of(napi, struct napi_struct_container, napi_struct);

	async_msg_set_fn_type(_request, NAPI_COMPLETE_DONE);

	fipc_set_reg0(_request, napi_container->other_ref.cptr);
	fipc_set_reg1(_request, work_done);
	//fipc_set_reg2(_request, napi->state);

	vmfunc_wrapper(_request);

	napi->state = fipc_get_reg0(_request);
	return;
}

void synchronize_irq(unsigned int irq)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	INIT_IPC_MSG(&r);
	async_msg_set_fn_type(_request, SYNCHRONIZE_IRQ);

	fipc_set_reg0(_request, irq);

	vmfunc_wrapper(_request);

	return;
}

void __netif_tx_disable(struct net_device *dev)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;

	INIT_IPC_MSG(&r);
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request, __NETIF_TX_DISABLE);
	fipc_set_reg1(_request, dev_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	/*
	 * TODO: All the queue states are modified. we should synchronize all of
	 * them. PITA!
	 */

	return;
}
