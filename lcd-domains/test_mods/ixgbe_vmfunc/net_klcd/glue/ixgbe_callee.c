#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/trampoline.h>
#include <linux/aer.h>

#include "../../ixgbe_common.h"
#include "../ixgbe_callee.h"
#include "../../rdtsc_helper.h"

#include <linux/hashtable.h>
#include <asm/cacheflush.h>
#include <lcd_domains/microkernel.h>

#include <linux/priv_mempool.h>
#include <linux/sort.h>
#include <asm/lcd_domains/ept_lcd.h>
#include <linux/lcd_trace.h>

#include <lcd_config/post_hook.h>

/* This is the only device we strive for */
#define IXGBE_DEV_ID_82599_SFP_SF2       0x154D
#define IXGBE_DEV_ID_82599_SFP           0x10FB
/* XXX: How to determine this? */
#define CPTR_HASH_BITS      5
#define MAX_POOLS	40

u32 thread = 0;
struct glue_cspace *c_cspace = NULL;
extern struct cspace *klcd_cspace;
struct timer_list service_timer;

struct pci_dev *g_pdev = NULL;
struct net_device *g_ndev = NULL;
struct kmem_cache *skb_c_cache = NULL;
const char driver_name[] = "ixgbe_lcd";

#define NAPI_HASH_BITS	5
DEFINE_HASHTABLE(napi_hashtable, NAPI_HASH_BITS);
/* XXX: There's no way to pass arrays across domains for now.
 * May not be in the future too! But agree that this is ugly
 * and move forward. - vik
 */
static const struct pci_device_id ixgbe_pci_tbl[] = {
	/* {PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_SFP_SF2) }, */
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_SFP) },
	{ 0 } /* sentinel */
};

/*
 * During ndo_start_xmit, we would insert the skb reference to this per-cpu
 * hash table.  When consume_skb is called, we lookp the skb_cptr on this
 * hashtable to retrieve the original skb pointer to be freed.
 */
DEFINE_PER_CPU(struct skb_hash_table, skb_hash);

static unsigned long pool_pfn_start, pool_pfn_end;
priv_pool_t *pool;
void *pool_base = NULL;
size_t pool_size = 0;
#ifdef SKBC_PRIVATE_POOL
priv_pool_t *skbc_pool;
#endif

/* Pool allocation logic */
char *base_pools[MAX_POOLS];
int pool_order = 10;
int start_idx[MAX_POOLS/2] = {-1}, end_idx[MAX_POOLS/2] = {-1};
unsigned int best_diff = 0;
int best_idx = -1;
int pool_idx = 0;

struct {
	int start_idx;
	int end_idx;
	size_t size;
	bool valid;
} pools[MAX_POOLS] = { {0} };


int compare_addr(const void *a, const void *b)
{
	return *(unsigned int *)a - *(unsigned int *)b;
}

int pool_pick(void)
{
	int i;

	/* allocate series of pages */
	for (i = 0; i < MAX_POOLS; i++) {
		base_pools[i] = (char*) __get_free_pages(GFP_KERNEL | __GFP_ZERO,
	                            pool_order);
	}

	/* sort all of base addresses */
	sort(base_pools, MAX_POOLS, sizeof(char*), compare_addr, NULL);

	printk("%s, sorted order:\n", __func__);
	for (i = 0; i < MAX_POOLS; i++) {
		printk("%s, got pool %p\n", __func__, base_pools[i]);
	}

	pools[pool_idx].start_idx = 0;
	pools[pool_idx].end_idx = MAX_POOLS - 1;
	pools[pool_idx].valid = true;

	for (i = 0; i < MAX_POOLS - 1; i++) {
		printk("%s, comparing pool[%d]=%llx and pool[%d]=%llx\n", __func__,
					i+1, (uint64_t)base_pools[i+1], i, (uint64_t) base_pools[i]);
		if (((uint64_t) base_pools[i+1] - (uint64_t) base_pools[i]) != ((1 << pool_order) * PAGE_SIZE)) {
			printk("%s, found discontinuity @ i %d\n", __func__, i);
			pools[pool_idx].valid = true;
			pools[pool_idx++].end_idx = i;
			pools[pool_idx].start_idx = i + 1;
		}
	}
	/* if there is no discontinuity, then we will have a huge chunk until the end */
	pools[pool_idx].valid = true;
	pools[pool_idx].end_idx = MAX_POOLS - 1;

	for (i = 0; i < pool_idx + 1; i++) {
		printk("%s, pool %d: start idx = %d | end idx = %d\n",
				__func__, i, pools[i].start_idx, pools[i].end_idx);
		if (!pools[i].valid)
			continue;
		if ((pools[i].end_idx - pools[i].start_idx + 1) > best_diff) {
			best_idx = i;
			best_diff = pools[i].end_idx - pools[i].start_idx + 1;
		}
	}
	printk("%s, best diff %u | best idx %d | start = %d | end = %d\n",
			__func__, best_diff, best_idx, pools[best_idx].start_idx, pools[best_idx].end_idx);
       	return best_idx;
}

void skb_data_pool_init(void)
{
	printk("%s, init pool for skbdata | size %zu | %lx\n", __func__,
			SKB_DATA_SIZE, SKB_DATA_SIZE);
	// XXX: round it to 2KiB
	pool_base = base_pools[pools[pool_pick()].start_idx];
	pool_size = best_diff * ((1 << pool_order) * PAGE_SIZE);
	pool = priv_pool_init(SKB_DATA_POOL, (void*) pool_base, pool_size, 2048);
#ifdef SKBC_PRIVATE_POOL
	skbc_pool = priv_pool_init(SKB_CONTAINER_POOL, 0x20,
				SKB_CONTAINER_SIZE * 2);
#endif
}

void skb_data_pool_free(void)
{
	priv_pool_destroy(pool);
#ifdef SKBC_PRIVATE_POOL
	priv_pool_destroy(skbc_pool);
#endif
}

xmit_type_t check_skb_range(struct sk_buff *skb)
{
	unsigned long pfn;
	pfn = ((unsigned long)skb->data) >> PAGE_SHIFT;
	if (pfn >= pool_pfn_start && pfn <= pool_pfn_end) {
		WARN_ON(!skb->private);
		return SHARED_DATA_XMIT;
	} else
		return VOLUNTEER_XMIT;
}

int glue_ixgbe_init(void)
{
	int ret;
	int cpu;

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

	/* hash_init our percpu skb_hash_table */
	for_each_online_cpu(cpu) {
		struct skb_hash_table *this = &per_cpu(skb_hash, smp_processor_id());
		hash_init(this->skb_table);
	}

	/* initialize our private pool */
	skb_data_pool_init();

	skb_c_cache = kmem_cache_create("skb_c_cache",
				sizeof(struct sk_buff_container),
				0,
				SLAB_HWCACHE_ALIGN|SLAB_PANIC,
				NULL);
	if (!skb_c_cache) {
		LIBLCD_ERR("Could not create skb container cache");
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
}

int inline glue_insert_skb_hash(struct sk_buff_container *skb_c)
{
	int cpu = smp_processor_id();
	struct skb_hash_table *this = &per_cpu(skb_hash, cpu);

	BUG_ON(!skb_c->skb);

	skb_c->my_ref = __cptr((unsigned long)skb_c->skb);

	hash_add(this->skb_table, &skb_c->hentry,
			(unsigned long) skb_c->skb);

	return 0;
}

int inline glue_lookup_skb_hash(struct cptr c, struct sk_buff_container **skb_cout)
{
	int cpu = smp_processor_id();
	struct skb_hash_table *this = &per_cpu(skb_hash, cpu);
        struct sk_buff_container *skb_c;

        hash_for_each_possible(this->skb_table, skb_c, hentry, (unsigned long) cptr_val(c)) {
		if (!skb_c) {
			WARN_ON(!skb_c);
			continue;
		}
		if (skb_c->skb == (struct sk_buff*) c.cptr) {
	                *skb_cout = skb_c;
		}
        }
        return 0;
}

void inline glue_remove_skb_hash(struct sk_buff_container *skb_c)
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

int sync_setup_memory(void *data, size_t sz, unsigned long *order, cptr_t *data_cptr, unsigned long *data_offset)
{
        int ret;
        struct page *p;
        unsigned long data_len;
        unsigned long mem_len;
        /*
         * Determine page that contains (start of) data
         */
        p = virt_to_head_page(data);
        if (!p) {
                LIBLCD_ERR("failed to translate to page");
                ret = -EINVAL;
                goto fail1;
        }
        data_len = sz;
        mem_len = ALIGN(data + data_len - page_address(p), PAGE_SIZE);
        *order = ilog2(roundup_pow_of_two(mem_len >> PAGE_SHIFT));
        /*
         * Volunteer memory
         */
        *data_offset = data - page_address(p);
        ret = lcd_volunteer_pages(p, *order, data_cptr);
        if (ret) {
                LIBLCD_ERR("failed to volunteer memory");
                goto fail2;
        }
        /*
         * Done
         */
        return 0;
fail2:
fail1:
        return ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(probe_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(probe_trampoline)
probe_trampoline(struct pci_dev *dev,
		struct pci_device_id *id)
{
	int ( *volatile probe_fp )(struct pci_dev *,
		struct pci_device_id *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			probe_trampoline);
	probe_fp = probe;
	return probe_fp(dev,
		id,
		hidden_args);

}
#endif

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(remove_trampoline);
void  LCD_TRAMPOLINE_LINKAGE(remove_trampoline)
remove_trampoline(struct pci_dev *dev)
{
	void ( *volatile remove_fp )(struct pci_dev *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			remove_trampoline);
	remove_fp = remove;
	return remove_fp(dev,
		hidden_args);

}
#endif

int pci_disable_msix_callee(struct fipc_message *_request)
{
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;
	int ret;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	pci_disable_msix(pdev);
fail_lookup:
	return ret;
}

int pci_enable_msix_range_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long p_offset;
	cptr_t p_cptr, lcd_cptr;
	gva_t p_gva;
	int minvec, maxvec;
	struct msix_entry *entries;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg5(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	minvec = fipc_get_reg0(_request);
	maxvec = fipc_get_reg1(_request);

	sync_ret = lcd_cptr_alloc(&p_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}

	mem_order = fipc_get_reg2(_request);
	p_offset = fipc_get_reg3(_request);
	lcd_cptr = __cptr(fipc_get_reg4(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_cptr,
			p_cptr);

	sync_ret = lcd_map_virt(p_cptr, mem_order, &p_gva);

	if (sync_ret) {
		LIBLCD_ERR("failed to map void *p");
		lcd_exit(-1);
	}

	entries = (struct msix_entry*)(void*)(gva_val(p_gva) + p_offset);

	LIBLCD_MSG("%s, dev->msix_enabled %d | minvec %d | maxvec %d",
			__func__, pdev->msix_enabled, minvec, maxvec);

	func_ret = pci_enable_msix_range(pdev, entries, minvec, maxvec);

	LIBLCD_MSG("%s, returned %d", __func__, func_ret);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}


int __pci_register_driver_callee(struct fipc_message *_request)
{
	struct pci_driver_container *drv_container;
	struct module_container *owner_container;
	char *name;

#ifndef CONFIG_LVD
	struct trampoline_hidden_args *drv_probe_hidden_args;
	struct trampoline_hidden_args *drv_remove_hidden_args;
#endif
	int func_ret = 0;
	int ret = 0;

	drv_container = kzalloc(sizeof( struct pci_driver_container   ),
		GFP_KERNEL);
	if (!drv_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_pci_driver_type(c_cspace,
		drv_container,
		&drv_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	drv_container->other_ref.cptr = fipc_get_reg0(_request);
	owner_container = kzalloc(sizeof( struct module_container   ),
		GFP_KERNEL);
	if (!owner_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ret = glue_cap_insert_module_type(c_cspace,
		owner_container,
		&owner_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	owner_container->other_ref.cptr = fipc_get_reg1(_request);

#ifndef CONFIG_LVD
	drv_probe_hidden_args = kzalloc(sizeof( *drv_probe_hidden_args ),
		GFP_KERNEL);
	if (!drv_probe_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc1;
	}
	drv_probe_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(probe_trampoline);
	if (!drv_probe_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	drv_probe_hidden_args->t_handle->hidden_args = drv_probe_hidden_args;
	drv_probe_hidden_args->struct_container = drv_container;
	drv_probe_hidden_args->cspace = c_cspace;
	drv_probe_hidden_args->sync_ep = sync_ep;
	drv_container->pci_driver.probe = LCD_HANDLE_TO_TRAMPOLINE(drv_probe_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_probe_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(probe_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	drv_remove_hidden_args = kzalloc(sizeof( *drv_remove_hidden_args ),
		GFP_KERNEL);
	if (!drv_remove_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc2;
	}
	drv_remove_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(remove_trampoline);
	if (!drv_remove_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup2;
	}
	drv_remove_hidden_args->t_handle->hidden_args = drv_remove_hidden_args;
	drv_remove_hidden_args->struct_container = drv_container;
	drv_remove_hidden_args->cspace = c_cspace;
	drv_remove_hidden_args->sync_ep = sync_ep;
	drv_container->pci_driver.remove = LCD_HANDLE_TO_TRAMPOLINE(drv_remove_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_remove_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(remove_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
#endif
	drv_container->pci_driver.name = driver_name;
	drv_container->pci_driver.id_table = ixgbe_pci_tbl;
	drv_container->pci_driver.probe = probe;
	drv_container->pci_driver.remove = remove;
	name = "ixgbe_lcd";

	/* XXX: We should rather call __pci_register_driver
	 * (at least according to the RPC semantics).
	 * However, kobject subsys is not happy with us on mangling
	 * the module name. If we call pci_register_driver instead,
	 * module pointer is taken from THIS_MODULE and kobject is
	 * happy. So, do _not_ do such crap! kobject is unhappy
	owner_container->module.state = MODULE_STATE_LIVE;
	strcpy(owner_container->module.name, "ixgbe_lcd");
	atomic_inc(&owner_container->module.refcnt);
	*/

	func_ret = pci_register_driver(&drv_container->pci_driver);

	LIBLCD_MSG("%s returned %d", __func__, func_ret);

	fipc_set_reg1(_request, drv_container->my_ref.cptr);
	fipc_set_reg2(_request, owner_container->my_ref.cptr);
	fipc_set_reg3(_request, func_ret);

	return ret;
fail_alloc:
#ifndef CONFIG_LVD
fail_alloc1:
fail_alloc2:
fail_dup1:
fail_dup2:
#endif
fail_insert:
	return ret;
}

int alloc_etherdev_mqs_callee(struct fipc_message *_request)
{
	int sizeof_priv;
	unsigned 	int txqs;
	unsigned 	int rxqs;
	int ret;

	struct net_device_container *func_ret_container;
	struct net_device *func_ret;
	cptr_t netdev_ref;

	sizeof_priv = fipc_get_reg1(_request);
	txqs = fipc_get_reg2(_request);
	rxqs = fipc_get_reg3(_request);
	netdev_ref.cptr = fipc_get_reg4(_request);

	func_ret = alloc_etherdev_mqs(sizeof_priv, txqs, rxqs);

	g_ndev = func_ret;
	func_ret_container = container_of(func_ret, struct
			net_device_container, net_device);
	func_ret_container->other_ref = netdev_ref;

	ret = glue_cap_insert_net_device_type(c_cspace, func_ret_container,
			&func_ret_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	fipc_set_reg1(_request, func_ret_container->my_ref.cptr);

fail_insert:
	return ret;
}

int sync_probe_callee(struct fipc_message *_request)
{
	int ret;
	cptr_t res0_cptr;
	unsigned int res0_len;
	struct page *p;
	unsigned int pool_ord;
	cptr_t pool_cptr;
	cptr_t lcd_pool_cptr;
	cptr_t lcd_res0_cptr;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	LIBLCD_MSG("%s, called");

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg2(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	/*
	 * ixgbe driver just needs res[0]
	 */

	/* get LCD's pool cptr */
	lcd_res0_cptr = __cptr(fipc_get_reg0(_request));
	lcd_pool_cptr = __cptr(fipc_get_reg1(_request));

	res0_len = pci_resource_len(pdev, 0);

	LIBLCD_MSG("res0 start: %lx, len: %d",
			pci_resource_start(pdev, 0),
			get_order(res0_len));

	ret = lcd_volunteer_dev_mem(__gpa(pci_resource_start(pdev, 0)),
			get_order(res0_len),
			&res0_cptr);
	if (ret) {
		LIBLCD_ERR("volunteer devmem");
		goto fail_vol;
	}

        p = virt_to_head_page(pool->pool);

	pool_ord = ilog2(roundup_pow_of_two((1 << pool_order) * best_diff));
        ret = lcd_volunteer_pages(p, pool_ord, &pool_cptr);

	if (ret) {
		LIBLCD_ERR("volunteer shared region");
		goto fail_vol;
	}

	pool_pfn_start = (unsigned long)pool->pool >> PAGE_SHIFT;
	pool_pfn_end = pool_pfn_start + ((1 << pool_order) * best_diff);

	copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, pool_cptr, lcd_pool_cptr);
	copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, res0_cptr, lcd_res0_cptr);

	fipc_set_reg0(_request, res0_len);
	fipc_set_reg1(_request, pool_ord);

fail_lookup:
fail_vol:
	return 0;
}

int ixgbe_service_event_sched(void)
{
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;
	struct net_device_container *dev_container;

	async_msg_set_fn_type(_request, SERVICE_EVENT_SCHED);

	dev_container = container_of(g_ndev,
		struct net_device_container, net_device);

	fipc_set_reg0(_request,
			dev_container->other_ref.cptr);

	vmfunc_klcd_wrapper(_request, 1);

	return 0;
}

void ixgbe_service_timer(unsigned long data)
{
	unsigned long next_event_offset;

	next_event_offset = HZ * 5;

	/* Reset the timer */
	mod_timer(&service_timer, next_event_offset + jiffies);

	ixgbe_service_event_sched();
}

#ifdef CONFIG_LVD
int probe(struct pci_dev *dev,
		const struct pci_device_id *id)
#else
int probe(struct pci_dev *dev,
		const struct pci_device_id *id,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct pci_dev_container *dev_container;
	int ret = 0;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;

	int func_ret;
	/* assign pdev to a global instance */
	g_pdev = dev;

	LIBLCD_MSG("%s, irq # %d | msix_enabled %d", __func__, dev->irq, dev->msix_enabled);

	dev_container = kzalloc(sizeof( struct pci_dev_container   ),
		GFP_KERNEL);
	if (!dev_container) {
		LIBLCD_ERR("kzalloc dev_container");
		goto fail_alloc;
	}

	dev_container->pdev = dev;

	/*
	 * Kernel does not give us pci_dev with a container wrapped in.
	 * So, let's have a pointer!
	 */
	ret = glue_cap_insert_pci_dev_type(c_cspace, dev_container,
			&dev_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}


	async_msg_set_fn_type(_request, PROBE);

	fipc_set_reg1(_request, dev_container->my_ref.cptr);
	fipc_set_reg2(_request, *dev->dev.dma_mask);

	vmfunc_klcd_wrapper(_request, 1);

	printk("%s, send request done\n", __func__);

	func_ret = fipc_get_reg0(_request);

	setup_timer(&service_timer, &ixgbe_service_timer, (unsigned long) NULL);

	return func_ret;

fail_insert:
fail_alloc:
	return ret;
}

int pci_unregister_driver_callee(struct fipc_message *_request)
{
	struct pci_driver_container *drv_container;
	int ret;
#ifndef CONFIG_LVD
	struct trampoline_hidden_args *drv_probe_hidden_args;
#endif
	ret = glue_cap_lookup_pci_driver_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&drv_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	LIBLCD_MSG("Calling pci_unregister_driver");

	pci_unregister_driver(( &drv_container->pci_driver ));

	/* destroy our skb->data pool */
	skb_data_pool_free();

	LIBLCD_MSG("Called pci_unregister_driver");
	glue_cap_remove(c_cspace, drv_container->my_ref);

	/* XXX: Do not do anything like this! read the comments
	 * under pci_unregister_driver
	 * atomic_dec_if_positive(&drv_container->pci_driver.driver.owner->refcnt);
	 */
#ifndef CONFIG_LVD
	drv_probe_hidden_args = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(drv_container->pci_driver.probe);
	kfree(drv_probe_hidden_args->t_handle);
	kfree(drv_probe_hidden_args);
#endif
	kfree(drv_container);

fail_lookup:
	return ret;
}

#ifdef CONFIG_LVD
void remove(struct pci_dev *dev)
#else
void remove(struct pci_dev *dev,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;

	async_msg_set_fn_type(_request, REMOVE);

	vmfunc_klcd_wrapper(_request, 1);

	del_timer_sync(&service_timer);

	return;
}

#ifdef CONFIG_LVD
int ndo_open(struct net_device *dev)
#else
int ndo_open(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;

	int func_ret;

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request, NDO_OPEN);
	fipc_set_reg0(_request, dev_container->other_ref.cptr);

	vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);
	/* TODO: this should be done synchronously */
	mod_timer(&service_timer, jiffies + msecs_to_jiffies(5000));

	/* TODO: enable napi from within the LCD */
	/* napi_enable(napi_q0); */

	return func_ret;

}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_open_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(ndo_open_trampoline)
ndo_open_trampoline(struct net_device *dev)
{
	int ( *volatile ndo_open_fp )(struct net_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			ndo_open_trampoline);
	ndo_open_fp = ndo_open;
	return ndo_open_fp(dev,
		hidden_args);

}
#endif

#ifdef CONFIG_LVD
int ndo_stop(struct net_device *dev)
#else
int ndo_stop(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;
	int func_ret;

	dev_container = container_of(dev,
			struct net_device_container,
			net_device);

	async_msg_set_fn_type(_request, NDO_STOP);
	fipc_set_reg0(_request, dev_container->other_ref.cptr);

	vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_stop_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(ndo_stop_trampoline)
ndo_stop_trampoline(struct net_device *dev)
{
	int ( *volatile ndo_stop_fp )(struct net_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			ndo_stop_trampoline);
	ndo_stop_fp = ndo_stop;
	return ndo_stop_fp(dev,
		hidden_args);

}
#endif

#ifdef CONFIG_LVD
int ndo_start_xmit(struct sk_buff *skb,
		struct net_device *dev)
#else
int ndo_start_xmit(struct sk_buff *skb,
		struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;
	int func_ret = 0;
	struct sk_buff_container *skb_c;
	struct skbuff_members *skb_lcd;
#ifdef TIMESTAMP
	TS_DECL(mndo_xmit);
#endif

#if defined(TIMESTAMP) || defined(LCD_MEASUREMENT)
	static int iter = 0;
#endif
#ifdef TIMESTAMP
	TS_START(mndo_xmit);
#endif
	xmit_type_t xmit_type = check_skb_range(skb);

	if (xmit_type == VOLUNTEER_XMIT) {
		printk("%s, comm %s | pid %d | skblen %d " "| skb->proto %02X\n",
				__func__, current->comm, current->pid,
				skb->len, ntohs(skb->protocol));
		return NETDEV_TX_OK;
	}

	dev_container = container_of(dev,
			struct net_device_container,
			net_device);

#ifdef SKBC_PRIVATE_POOL
	skb_c = priv_alloc(SKB_CONTAINER_POOL);
#else
	skb_c = kmem_cache_alloc(skb_c_cache, GFP_KERNEL);
#endif
	if (!skb_c) {
		LIBLCD_MSG("no memory");
		goto fail_alloc;
	}

	skb_c->skb = skb;

	if (0)
		printk("%s, on cpu:%d skb:%p\n", __func__, smp_processor_id(), skb);

	glue_insert_skb_hash(skb_c);

	/* pad to 17 bytes, don't care the ret val */
	skb_put_padto(skb, 17);
	async_msg_set_fn_type(_request, NDO_START_XMIT);

	fipc_set_reg0(_request, xmit_type);

	fipc_set_reg1(_request, dev_container->other_ref.cptr);

	fipc_set_reg2(_request, skb_c->my_ref.cptr);

	fipc_set_reg3(_request,
			(unsigned long)
			((void*)skb->head - pool->pool));

	fipc_set_reg4(_request, skb->end);

	fipc_set_reg5(_request, skb->protocol);

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

	if (0)
		printk("%s:%d %s skb->q %d\n", current->comm, current->pid, __func__, skb->queue_mapping);

	add_trace_entry(EVENT_XMIT, async_msg_get_fn_type(_request));
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
fail_alloc:
	if (func_ret != NETDEV_TX_OK)
		printk("%s, got %d\n", __func__, func_ret);
	return func_ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_start_xmit_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(ndo_start_xmit_trampoline)
ndo_start_xmit_trampoline(struct sk_buff *skb,
		struct net_device *dev)
{
	int ( *volatile ndo_start_xmit_fp )(struct sk_buff *,
		struct net_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			ndo_start_xmit_trampoline);
	ndo_start_xmit_fp = ndo_start_xmit;
	return ndo_start_xmit_fp(skb,
		dev,
		hidden_args);

}
#endif

#ifdef CONFIG_LVD
void ndo_set_rx_mode(struct net_device *dev)
#else
void ndo_set_rx_mode(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;

	dev_container = container_of(dev,
			struct net_device_container,
			net_device);

	async_msg_set_fn_type(_request, NDO_SET_RX_MODE);
	fipc_set_reg0(_request, dev_container->other_ref.cptr);

	vmfunc_klcd_wrapper(_request, 1);

	return;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_set_rx_mode_trampoline);
void  LCD_TRAMPOLINE_LINKAGE(ndo_set_rx_mode_trampoline)
ndo_set_rx_mode_trampoline(struct net_device *dev)
{
	void ( *volatile ndo_set_rx_mode_fp )(struct net_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			ndo_set_rx_mode_trampoline);
	ndo_set_rx_mode_fp = ndo_set_rx_mode;
	return ndo_set_rx_mode_fp(dev,
		hidden_args);

}
#endif

#ifdef CONFIG_LVD
int ndo_validate_addr(struct net_device *dev)
#else
int ndo_validate_addr(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;
	int func_ret;

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			NDO_VALIDATE_ADDR);
	fipc_set_reg0(_request,
			dev_container->other_ref.cptr);
	vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_validate_addr_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(ndo_validate_addr_trampoline)
ndo_validate_addr_trampoline(struct net_device *dev)
{
	int ( *volatile ndo_validate_addr_fp )(struct net_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			ndo_validate_addr_trampoline);
	ndo_validate_addr_fp = ndo_validate_addr;
	return ndo_validate_addr_fp(dev,
		hidden_args);

}
#endif

void *mac_addr;

int sync_ndo_set_mac_address_callee(struct fipc_message *message)
{
	int sync_ret;
	unsigned 	long addr_mem_sz;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
	cptr_t lcd_addr_cptr;

	sync_ret = lcd_virt_to_cptr(__gva((unsigned long)mac_addr),
			&addr_cptr, &addr_mem_sz, &addr_offset);

	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}

	lcd_addr_cptr = __cptr(fipc_get_reg0(message));

	copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, addr_cptr, lcd_addr_cptr);

	fipc_set_reg0(message, ilog2(( addr_mem_sz ) >> ( PAGE_SHIFT )));
	fipc_set_reg1(message, addr_offset);

	return 0;
}

#ifdef CONFIG_LVD
int ndo_set_mac_address(struct net_device *dev,
		void *addr)
#else
int ndo_set_mac_address(struct net_device *dev,
		void *addr,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;


	int func_ret;

	mac_addr = addr;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request, NDO_SET_MAC_ADDRESS);
	fipc_set_reg0(_request, dev_container->other_ref.cptr);

	vmfunc_klcd_wrapper(_request, 1);
	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_set_mac_address_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(ndo_set_mac_address_trampoline)
ndo_set_mac_address_trampoline(struct net_device *dev,
		void *addr)
{
	int ( *volatile ndo_set_mac_address_fp )(struct net_device *,
		void *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			ndo_set_mac_address_trampoline);
	ndo_set_mac_address_fp = ndo_set_mac_address;
	return ndo_set_mac_address_fp(dev,
		addr,
		hidden_args);

}
#endif

#ifdef CONFIG_LVD
int ndo_change_mtu(struct net_device *dev,
		int new_mtu)
#else
int ndo_change_mtu(struct net_device *dev,
		int new_mtu,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;
	int func_ret;

	dev_container = container_of(dev,
			struct net_device_container,
			net_device);

	async_msg_set_fn_type(_request, NDO_CHANGE_MTU);
	fipc_set_reg0(_request, dev_container->other_ref.cptr);
	fipc_set_reg1(_request, new_mtu);

	vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_change_mtu_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(ndo_change_mtu_trampoline)
ndo_change_mtu_trampoline(struct net_device *dev,
		int new_mtu)
{
	int ( *volatile ndo_change_mtu_fp )(struct net_device *,
		int ,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			ndo_change_mtu_trampoline);
	ndo_change_mtu_fp = ndo_change_mtu;
	return ndo_change_mtu_fp(dev,
		new_mtu,
		hidden_args);

}
#endif

#ifdef CONFIG_LVD
void ndo_tx_timeout(struct net_device *dev)
#else
void ndo_tx_timeout(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request, NDO_TX_TIMEOUT);
	fipc_set_reg0(_request, dev_container->other_ref.cptr);
	vmfunc_klcd_wrapper(_request, 1);
	return;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_tx_timeout_trampoline);
void LCD_TRAMPOLINE_LINKAGE(ndo_tx_timeout_trampoline)
ndo_tx_timeout_trampoline(struct net_device *dev)
{
	void ( *volatile ndo_tx_timeout_fp )(struct net_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			ndo_tx_timeout_trampoline);
	ndo_tx_timeout_fp = ndo_tx_timeout;
	ndo_tx_timeout_fp(dev,
		hidden_args);
	return;
}
#endif

#ifdef CONFIG_LVD
int ndo_set_tx_maxrate(struct net_device *dev,
		int queue_index,
		unsigned int maxrate)
#else
int ndo_set_tx_maxrate(struct net_device *dev,
		int queue_index,
		unsigned int maxrate,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;
	int func_ret;

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			NDO_SET_TX_MAXRATE);
	fipc_set_reg0(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg1(_request,
			queue_index);
	fipc_set_reg2(_request,
			maxrate);
	vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_set_tx_maxrate_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(ndo_set_tx_maxrate_trampoline)
ndo_set_tx_maxrate_trampoline(struct net_device *dev,
		int queue_index,
		unsigned int maxrate)
{
	int ( *volatile ndo_set_tx_maxrate_fp )(struct net_device *,
		int ,
		unsigned int ,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			ndo_set_tx_maxrate_trampoline);
	ndo_set_tx_maxrate_fp = ndo_set_tx_maxrate;
	return ndo_set_tx_maxrate_fp(dev,
		queue_index,
		maxrate,
		hidden_args);

}
#endif

#ifdef CONFIG_LVD
struct rtnl_link_stats64 *ndo_get_stats64(struct net_device *dev,
		struct rtnl_link_stats64 *stats)
#else
struct rtnl_link_stats64 *ndo_get_stats64(struct net_device *dev,
		struct rtnl_link_stats64 *stats,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;

	struct rtnl_link_stats64 *func_ret = stats;

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request, NDO_GET_STATS64);
	fipc_set_reg0(_request, dev_container->other_ref.cptr);

	vmfunc_klcd_wrapper(_request, 1);
	stats->rx_packets = fipc_get_reg0(_request);
	stats->rx_bytes = fipc_get_reg1(_request);
	stats->tx_packets = fipc_get_reg2(_request);
	stats->tx_bytes = fipc_get_reg3(_request);

	return func_ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_get_stats64_trampoline);
struct rtnl_link_stats64  LCD_TRAMPOLINE_LINKAGE(ndo_get_stats64_trampoline)
*ndo_get_stats64_trampoline(struct net_device *dev,
		struct rtnl_link_stats64 *stats)
{
	struct rtnl_link_stats64* ( *volatile ndo_get_stats64_fp )(struct net_device *,
		struct rtnl_link_stats64 *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			ndo_get_stats64_trampoline);
	ndo_get_stats64_fp = ndo_get_stats64;
	return ndo_get_stats64_fp(dev,
		stats,
		hidden_args);

}
#endif

#ifndef CONFIG_LVD
void setup_netdev_ops(struct net_device_container *dev_c,
	struct net_device_ops_container *netdev_ops_container,
	struct cptr sync_ep)
{
	struct trampoline_hidden_args *dev_netdev_ops_ndo_open_hidden_args;
	struct trampoline_hidden_args *dev_netdev_ops_ndo_stop_hidden_args;
	struct trampoline_hidden_args *dev_netdev_ops_ndo_start_xmit_hidden_args;
	struct trampoline_hidden_args *dev_netdev_ops_ndo_set_rx_mode_hidden_args;
	struct trampoline_hidden_args *dev_netdev_ops_ndo_validate_addr_hidden_args;
	struct trampoline_hidden_args *dev_netdev_ops_ndo_set_mac_address_hidden_args;
	struct trampoline_hidden_args *dev_netdev_ops_ndo_change_mtu_hidden_args;
	struct trampoline_hidden_args *dev_netdev_ops_ndo_tx_timeout_hidden_args;
	struct trampoline_hidden_args *dev_netdev_ops_ndo_set_tx_maxrate_hidden_args;
	struct trampoline_hidden_args *dev_netdev_ops_ndo_get_stats64_hidden_args;
	int ret;

	dev_netdev_ops_ndo_open_hidden_args = kzalloc(sizeof( struct trampoline_hidden_args ),
		GFP_KERNEL);
	if (!dev_netdev_ops_ndo_open_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc1;
	}
	dev_netdev_ops_ndo_open_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_open_trampoline);
	if (!dev_netdev_ops_ndo_open_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	dev_netdev_ops_ndo_open_hidden_args->t_handle->hidden_args = dev_netdev_ops_ndo_open_hidden_args;
	dev_netdev_ops_ndo_open_hidden_args->struct_container = netdev_ops_container;
	dev_netdev_ops_ndo_open_hidden_args->cspace = c_cspace;
	dev_netdev_ops_ndo_open_hidden_args->sync_ep = sync_ep;
	netdev_ops_container->net_device_ops.ndo_open = LCD_HANDLE_TO_TRAMPOLINE(dev_netdev_ops_ndo_open_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )dev_netdev_ops_ndo_open_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(ndo_open_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	dev_netdev_ops_ndo_stop_hidden_args = kzalloc(sizeof( struct trampoline_hidden_args ),
		GFP_KERNEL);
	if (!dev_netdev_ops_ndo_stop_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc2;
	}
	dev_netdev_ops_ndo_stop_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_stop_trampoline);
	if (!dev_netdev_ops_ndo_stop_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup2;
	}
	dev_netdev_ops_ndo_stop_hidden_args->t_handle->hidden_args = dev_netdev_ops_ndo_stop_hidden_args;
	dev_netdev_ops_ndo_stop_hidden_args->struct_container = netdev_ops_container;
	dev_netdev_ops_ndo_stop_hidden_args->cspace = c_cspace;
	dev_netdev_ops_ndo_stop_hidden_args->sync_ep = sync_ep;
	netdev_ops_container->net_device_ops.ndo_stop = LCD_HANDLE_TO_TRAMPOLINE(dev_netdev_ops_ndo_stop_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )dev_netdev_ops_ndo_stop_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(ndo_stop_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	dev_netdev_ops_ndo_start_xmit_hidden_args = kzalloc(sizeof( struct trampoline_hidden_args ),
		GFP_KERNEL);
	if (!dev_netdev_ops_ndo_start_xmit_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc3;
	}
	dev_netdev_ops_ndo_start_xmit_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_start_xmit_trampoline);
	if (!dev_netdev_ops_ndo_start_xmit_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup3;
	}
	dev_netdev_ops_ndo_start_xmit_hidden_args->t_handle->hidden_args = dev_netdev_ops_ndo_start_xmit_hidden_args;
	dev_netdev_ops_ndo_start_xmit_hidden_args->struct_container = netdev_ops_container;
	dev_netdev_ops_ndo_start_xmit_hidden_args->cspace = c_cspace;
	dev_netdev_ops_ndo_start_xmit_hidden_args->sync_ep = sync_ep;
	netdev_ops_container->net_device_ops.ndo_start_xmit = LCD_HANDLE_TO_TRAMPOLINE(dev_netdev_ops_ndo_start_xmit_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )dev_netdev_ops_ndo_start_xmit_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(ndo_start_xmit_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	dev_netdev_ops_ndo_set_rx_mode_hidden_args = kzalloc(sizeof( struct trampoline_hidden_args ),
		GFP_KERNEL);
	if (!dev_netdev_ops_ndo_set_rx_mode_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc5;
	}
	dev_netdev_ops_ndo_set_rx_mode_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_set_rx_mode_trampoline);
	if (!dev_netdev_ops_ndo_set_rx_mode_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup5;
	}
	dev_netdev_ops_ndo_set_rx_mode_hidden_args->t_handle->hidden_args = dev_netdev_ops_ndo_set_rx_mode_hidden_args;
	dev_netdev_ops_ndo_set_rx_mode_hidden_args->struct_container = netdev_ops_container;
	dev_netdev_ops_ndo_set_rx_mode_hidden_args->cspace = c_cspace;
	dev_netdev_ops_ndo_set_rx_mode_hidden_args->sync_ep = sync_ep;
	netdev_ops_container->net_device_ops.ndo_set_rx_mode = LCD_HANDLE_TO_TRAMPOLINE(dev_netdev_ops_ndo_set_rx_mode_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )dev_netdev_ops_ndo_set_rx_mode_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(ndo_set_rx_mode_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	dev_netdev_ops_ndo_validate_addr_hidden_args = kzalloc(sizeof( struct trampoline_hidden_args ),
		GFP_KERNEL);
	if (!dev_netdev_ops_ndo_validate_addr_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc6;
	}
	dev_netdev_ops_ndo_validate_addr_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_validate_addr_trampoline);
	if (!dev_netdev_ops_ndo_validate_addr_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup6;
	}
	dev_netdev_ops_ndo_validate_addr_hidden_args->t_handle->hidden_args = dev_netdev_ops_ndo_validate_addr_hidden_args;
	dev_netdev_ops_ndo_validate_addr_hidden_args->struct_container = netdev_ops_container;
	dev_netdev_ops_ndo_validate_addr_hidden_args->cspace = c_cspace;
	dev_netdev_ops_ndo_validate_addr_hidden_args->sync_ep = sync_ep;
	netdev_ops_container->net_device_ops.ndo_validate_addr = LCD_HANDLE_TO_TRAMPOLINE(dev_netdev_ops_ndo_validate_addr_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )dev_netdev_ops_ndo_validate_addr_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(ndo_validate_addr_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	dev_netdev_ops_ndo_set_mac_address_hidden_args = kzalloc(sizeof( struct trampoline_hidden_args ),
		GFP_KERNEL);
	if (!dev_netdev_ops_ndo_set_mac_address_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc7;
	}
	dev_netdev_ops_ndo_set_mac_address_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_set_mac_address_trampoline);
	if (!dev_netdev_ops_ndo_set_mac_address_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup7;
	}
	dev_netdev_ops_ndo_set_mac_address_hidden_args->t_handle->hidden_args = dev_netdev_ops_ndo_set_mac_address_hidden_args;
	dev_netdev_ops_ndo_set_mac_address_hidden_args->struct_container = netdev_ops_container;
	dev_netdev_ops_ndo_set_mac_address_hidden_args->cspace = c_cspace;
	dev_netdev_ops_ndo_set_mac_address_hidden_args->sync_ep = sync_ep;
	netdev_ops_container->net_device_ops.ndo_set_mac_address = LCD_HANDLE_TO_TRAMPOLINE(dev_netdev_ops_ndo_set_mac_address_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )dev_netdev_ops_ndo_set_mac_address_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(ndo_set_mac_address_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	dev_netdev_ops_ndo_change_mtu_hidden_args = kzalloc(sizeof( struct trampoline_hidden_args ),
		GFP_KERNEL);
	if (!dev_netdev_ops_ndo_change_mtu_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc8;
	}
	dev_netdev_ops_ndo_change_mtu_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_change_mtu_trampoline);
	if (!dev_netdev_ops_ndo_change_mtu_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup8;
	}
	dev_netdev_ops_ndo_change_mtu_hidden_args->t_handle->hidden_args = dev_netdev_ops_ndo_change_mtu_hidden_args;
	dev_netdev_ops_ndo_change_mtu_hidden_args->struct_container = netdev_ops_container;
	dev_netdev_ops_ndo_change_mtu_hidden_args->cspace = c_cspace;
	dev_netdev_ops_ndo_change_mtu_hidden_args->sync_ep = sync_ep;
	netdev_ops_container->net_device_ops.ndo_change_mtu = LCD_HANDLE_TO_TRAMPOLINE(dev_netdev_ops_ndo_change_mtu_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )dev_netdev_ops_ndo_change_mtu_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(ndo_change_mtu_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	dev_netdev_ops_ndo_tx_timeout_hidden_args = kzalloc(sizeof( struct trampoline_hidden_args),
		GFP_KERNEL);
	if (!dev_netdev_ops_ndo_tx_timeout_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc9;
	}
	dev_netdev_ops_ndo_tx_timeout_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_tx_timeout_trampoline);
	if (!dev_netdev_ops_ndo_tx_timeout_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup9;
	}
	dev_netdev_ops_ndo_tx_timeout_hidden_args->t_handle->hidden_args = dev_netdev_ops_ndo_tx_timeout_hidden_args;
	dev_netdev_ops_ndo_tx_timeout_hidden_args->struct_container = netdev_ops_container;
	dev_netdev_ops_ndo_tx_timeout_hidden_args->cspace = c_cspace;
	dev_netdev_ops_ndo_tx_timeout_hidden_args->sync_ep = sync_ep;
	netdev_ops_container->net_device_ops.ndo_tx_timeout = LCD_HANDLE_TO_TRAMPOLINE(dev_netdev_ops_ndo_tx_timeout_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )dev_netdev_ops_ndo_tx_timeout_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(ndo_tx_timeout_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	dev_netdev_ops_ndo_set_tx_maxrate_hidden_args = kzalloc(sizeof( struct trampoline_hidden_args ),
		GFP_KERNEL);
	if (!dev_netdev_ops_ndo_set_tx_maxrate_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc10;
	}
	dev_netdev_ops_ndo_set_tx_maxrate_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_set_tx_maxrate_trampoline);
	if (!dev_netdev_ops_ndo_set_tx_maxrate_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup10;
	}
	dev_netdev_ops_ndo_set_tx_maxrate_hidden_args->t_handle->hidden_args = dev_netdev_ops_ndo_set_tx_maxrate_hidden_args;
	dev_netdev_ops_ndo_set_tx_maxrate_hidden_args->struct_container = netdev_ops_container;
	dev_netdev_ops_ndo_set_tx_maxrate_hidden_args->cspace = c_cspace;
	dev_netdev_ops_ndo_set_tx_maxrate_hidden_args->sync_ep = sync_ep;
	netdev_ops_container->net_device_ops.ndo_set_tx_maxrate = LCD_HANDLE_TO_TRAMPOLINE(dev_netdev_ops_ndo_set_tx_maxrate_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )dev_netdev_ops_ndo_set_tx_maxrate_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(ndo_set_tx_maxrate_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	dev_netdev_ops_ndo_get_stats64_hidden_args = kzalloc(sizeof( struct trampoline_hidden_args ),
		GFP_KERNEL);
	if (!dev_netdev_ops_ndo_get_stats64_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc11;
	}
	dev_netdev_ops_ndo_get_stats64_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_get_stats64_trampoline);
	if (!dev_netdev_ops_ndo_get_stats64_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup11;
	}
	dev_netdev_ops_ndo_get_stats64_hidden_args->t_handle->hidden_args = dev_netdev_ops_ndo_get_stats64_hidden_args;
	dev_netdev_ops_ndo_get_stats64_hidden_args->struct_container = netdev_ops_container;
	dev_netdev_ops_ndo_get_stats64_hidden_args->cspace = c_cspace;
	dev_netdev_ops_ndo_get_stats64_hidden_args->sync_ep = sync_ep;
	netdev_ops_container->net_device_ops.ndo_get_stats64 = LCD_HANDLE_TO_TRAMPOLINE(dev_netdev_ops_ndo_get_stats64_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )dev_netdev_ops_ndo_get_stats64_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(ndo_get_stats64_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
fail_alloc1:
fail_dup1:
fail_alloc2:
fail_dup2:
fail_alloc3:
fail_dup3:
fail_alloc5:
fail_dup5:
fail_alloc6:
fail_dup6:
fail_alloc7:
fail_dup7:
fail_alloc8:
fail_dup8:
fail_alloc9:
fail_dup9:
fail_alloc10:
fail_dup10:
fail_alloc11:
fail_dup11:
	return;
}
#endif

int register_netdev_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	struct net_device_ops_container *netdev_ops_container;
	struct net_device *dev;
	int ret;

	int func_ret;
	//node-0 90:e2:ba:b5:15:69
	u8 mac_addr[] = {0x90, 0xe2, 0xba, 0xb5, 0x15, 0x69};

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	netdev_ops_container = kzalloc(sizeof( struct net_device_ops_container   ),
		GFP_KERNEL);
	if (!netdev_ops_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ret = glue_cap_insert_net_device_ops_type(c_cspace,
		netdev_ops_container,
		&netdev_ops_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	dev_container->net_device.netdev_ops = &netdev_ops_container->net_device_ops;

	dev = &dev_container->net_device;
	dev->flags = fipc_get_reg1(_request);
	dev->priv_flags = fipc_get_reg2(_request);
	dev->features = fipc_get_reg3(_request);
	dev->hw_features = fipc_get_reg4(_request);
	dev->hw_enc_features = fipc_get_reg5(_request);
	dev->mpls_features = fipc_get_reg6(_request);

	memcpy(dev->dev_addr, mac_addr, ETH_ALEN);

#ifdef CONFIG_LVD
	netdev_ops_container->net_device_ops.ndo_open = ndo_open;
	netdev_ops_container->net_device_ops.ndo_stop = ndo_stop;
	netdev_ops_container->net_device_ops.ndo_start_xmit = ndo_start_xmit;
	netdev_ops_container->net_device_ops.ndo_set_rx_mode = ndo_set_rx_mode;
	netdev_ops_container->net_device_ops.ndo_validate_addr = ndo_validate_addr;
	netdev_ops_container->net_device_ops.ndo_set_mac_address = ndo_set_mac_address;
	netdev_ops_container->net_device_ops.ndo_change_mtu = ndo_change_mtu;
	netdev_ops_container->net_device_ops.ndo_tx_timeout = ndo_tx_timeout;
	netdev_ops_container->net_device_ops.ndo_set_tx_maxrate = ndo_set_tx_maxrate;
	netdev_ops_container->net_device_ops.ndo_get_stats64 = ndo_get_stats64;
#else
	/* setup netdev_ops */
	setup_netdev_ops(dev_container, netdev_ops_container, sync_ep);
#endif
	func_ret = register_netdev(( &dev_container->net_device ));

	fipc_set_reg1(_request,
			func_ret);
	fipc_set_reg2(_request,
			dev->reg_state);


fail_lookup:
fail_insert:
fail_alloc:
	return ret;
}

int ether_setup_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	ether_setup(( &dev_container->net_device ));

fail_lookup:
	return ret;

}

int eth_mac_addr_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;
	int func_ret;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	cptr_t lcd_p_cptr;
	gva_t p_gva;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	sync_ret = lcd_cptr_alloc(&p_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}

	lcd_p_cptr = __cptr(fipc_get_reg1(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_p_cptr,
			p_cptr);

	mem_order = fipc_get_reg2(_request);
	p_offset = fipc_get_reg3(_request);

	sync_ret = lcd_map_virt(p_cptr, mem_order, &p_gva);

	if (sync_ret) {
		LIBLCD_ERR("failed to map void *p");
		lcd_exit(-1);
	}

	func_ret = eth_mac_addr(( &dev_container->net_device ),
		( void  * )( ( gva_val(p_gva) ) + ( p_offset ) ));

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;

}

int eth_validate_addr_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;
	int func_ret;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	func_ret = eth_validate_addr(( &dev_container->net_device ));

	fipc_set_reg1(_request,
			func_ret);
fail_lookup:
	return ret;

}

int free_netdev_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	free_netdev(( &dev_container->net_device ));

fail_lookup:
	return ret;
}

int netif_carrier_off_callee(struct fipc_message *_request)
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

	netif_carrier_off(( &dev_container->net_device ));

	fipc_set_reg1(_request, dev_container->net_device.state);

fail_lookup:
	return ret;
}

int netif_carrier_on_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	netif_carrier_on(( &dev_container->net_device ));

	fipc_set_reg1(_request, dev_container->net_device.state);

fail_lookup:
	return ret;
}

int netif_device_attach_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	netif_device_attach(( &dev_container->net_device ));

	fipc_set_reg1(_request, dev_container->net_device.state);

fail_lookup:
	return ret;
}

int netif_device_detach_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	int ret;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	netif_device_detach(( &dev_container->net_device ));

	fipc_set_reg1(_request, dev_container->net_device.state);

fail_lookup:
	return ret;
}

int netif_set_real_num_rx_queues_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	unsigned 	int rxq;
	int ret;

	int func_ret;
	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	rxq = fipc_get_reg3(_request);
	func_ret = netif_set_real_num_rx_queues(( &dev_container->net_device ),
		rxq);

	fipc_set_reg1(_request,
			func_ret);
fail_lookup:
	return ret;

}

int netif_set_real_num_tx_queues_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	unsigned 	int txq;
	int ret;
	int func_ret;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	txq = fipc_get_reg3(_request);

	LIBLCD_MSG("%s, txq %d | num_tx_queues %d", __func__, txq, dev_container->net_device.num_tx_queues);

	func_ret = netif_set_real_num_tx_queues(( &dev_container->net_device ),
		txq);
	LIBLCD_MSG("netif_set_real_num_tx_queues returns %d", func_ret);

	fipc_set_reg1(_request,
			func_ret);

fail_lookup:
	return ret;

}

int napi_consume_skb_callee(struct fipc_message *_request)
{
	struct sk_buff *skb;
	struct sk_buff_container *skb_c = NULL;
	int ret = 0;
	int budget;

	glue_lookup_skb_hash(__cptr(fipc_get_reg0(_request)), &skb_c);
	budget = fipc_get_reg1(_request);

	skb = skb_c->skb;

	if (check_skb_range(skb) == VOLUNTEER_XMIT)
		printk("%s, skb possibly corrupted %p\n", __func__, skb);
	
	if (0)
		printk("%s, on cpu:%d skb:%p\n", __func__, smp_processor_id(), skb);

	napi_consume_skb(skb, budget);

	glue_remove_skb_hash(skb_c);

#ifdef SKBC_PRIVATE_POOL
	WARN_ON(!skb_c);
	if(skb_c)
		priv_free(skb_c, SKB_CONTAINER_POOL);
#else
	kmem_cache_free(skb_c_cache, skb_c);
#endif

	return ret;
}

int consume_skb_callee(struct fipc_message *_request)
{
	struct sk_buff *skb;
	struct sk_buff_container *skb_c = NULL;
	int ret = 0;

	glue_lookup_skb_hash(__cptr(fipc_get_reg0(_request)), &skb_c);

	skb = skb_c->skb;

	consume_skb(skb);

	glue_remove_skb_hash(skb_c);

	kfree(skb_c);

	return ret;
}

int unregister_netdev_callee(struct fipc_message *_request)
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
	unregister_netdev(&dev_container->net_device);

fail_lookup:
	return ret;
}

int eth_platform_get_mac_address_callee(struct fipc_message *_request)
{
	struct device *dev;
	int ret = 0;

	int func_ret;
	union mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };

	u8 mac_addr[ETH_ALEN] = {0x90, 0xe2, 0xba, 0xb5, 0x15, 0x69};


	dev = &g_pdev->dev;

	func_ret = eth_platform_get_mac_address(dev, mac_addr);

	LIBLCD_MSG("%s returned %d", __func__, func_ret);


	/* pass on mac addr only if the function gets a
	 * valid address
	 */
	if (func_ret < 0) {
		memcpy(m.mac_addr, mac_addr, ETH_ALEN);
		fipc_set_reg2(_request, m.mac_addr_l);
	}

	fipc_set_reg1(_request,
			func_ret);

	return ret;
}

int dev_addr_add_callee(struct fipc_message *_request)
{
	struct net_device *dev;
	struct net_device_container *dev_container;
	unsigned 	char addr_type;
	int ret;
	int func_ret;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long addr_offset;
	cptr_t addr_cptr, lcd_cptr;
	gva_t addr_gva;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	dev = &dev_container->net_device;
	addr_type = fipc_get_reg1(_request);

	sync_ret = lcd_cptr_alloc(&addr_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}

	mem_order = fipc_get_reg2(_request);
	addr_offset = fipc_get_reg3(_request);
	lcd_cptr = __cptr(fipc_get_reg4(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_cptr,
			addr_cptr);

	sync_ret = lcd_map_virt(addr_cptr, mem_order, &addr_gva);

	if (sync_ret) {
		LIBLCD_ERR("failed to map void *addr");
		lcd_exit(-1);
	}

	rtnl_lock();
	func_ret = dev_addr_add(dev,
		( void  * )( ( gva_val(addr_gva) ) + ( addr_offset ) ),
		addr_type);
	rtnl_unlock();

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int dev_addr_del_callee(struct fipc_message *_request)
{
	struct net_device *dev;
	struct net_device_container *dev_container;
	unsigned 	char addr_type;
	int ret;
	int func_ret;
	void *addr;
#ifdef PASS_DEV_ADDR_IN_REG
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };
#else
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long addr_offset;
	cptr_t addr_cptr, lcd_cptr;
	gva_t addr_gva;
#endif

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	dev = &dev_container->net_device;
	addr_type = fipc_get_reg1(_request);

#ifdef PASS_DEV_ADDR_IN_REG
	m.mac_addr_l = fipc_get_reg2(_request);
	addr = (void *)m.mac_addr;
#else
	sync_ret = lcd_cptr_alloc(&addr_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_cptr;
	}

	mem_order = fipc_get_reg2(_request);
	addr_offset = fipc_get_reg3(_request);
	lcd_cptr = __cptr(fipc_get_reg4(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_cptr,
			addr_cptr);

	sync_ret = lcd_map_virt(addr_cptr, mem_order, &addr_gva);

	if (sync_ret) {
		LIBLCD_ERR("failed to map void *addr");
		lcd_exit(-1);
	}
	addr = (void *)(gva_val(addr_gva) + addr_offset);
#endif
	rtnl_lock();
	func_ret = dev_addr_del(dev, addr, addr_type);
	rtnl_unlock();

	fipc_set_reg0(_request, func_ret);
#ifndef PASS_DEV_ADDR_IN_REG
fail_cptr:
#endif
fail_lookup:
	return ret;
}

int device_set_wakeup_enable_callee(struct fipc_message *_request)
{
	struct device *dev;
	bool enable;
	int ret = 0;

	int func_ret;

	dev = &g_pdev->dev;
	enable = fipc_get_reg1(_request);
	func_ret = device_set_wakeup_enable(dev,
		enable);

	fipc_set_reg1(_request,
			func_ret);

	return ret;
}

int netif_tx_stop_all_queues_callee(struct fipc_message *_request)
{
	struct net_device_container *ndev_container;
	struct net_device *dev;
	int ret;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg0(_request)),
		&ndev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	dev = &ndev_container->net_device;
	netif_tx_stop_all_queues(dev);


fail_lookup:
	return ret;
}

int _netif_tx_wake_all_queues_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_queue_container;
	int ret;

	struct net_device *dev;
	int num_qs;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_queue_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	num_qs = fipc_get_reg2(_request);



	dev = &dev_queue_container->net_device;
	dev->num_tx_queues = num_qs;

	netif_tx_wake_all_queues(dev);

fail_lookup:
	return ret;
}

int pci_disable_pcie_error_reporting_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	func_ret = pci_disable_pcie_error_reporting(pdev);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_bus_read_config_word_callee(struct fipc_message *_request)
{
	struct pci_dev *pdev;
	struct pci_bus *bus;
	unsigned int devfn;
	int where;
	unsigned short val;
	int func_ret;

	pdev = g_pdev;
	bus = pdev->bus;

	devfn = fipc_get_reg0(_request);
	where = fipc_get_reg1(_request);

	func_ret = pci_bus_read_config_word(bus, devfn, where, &val);

	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, val);

	return 0;
}

int pci_bus_write_config_word_callee(struct fipc_message *_request)
{
	struct pci_dev *pdev;
	struct pci_bus *bus;
	unsigned int devfn;
	int where;
	unsigned short val;
	int func_ret;

	pdev = g_pdev;
	bus = pdev->bus;

	devfn = fipc_get_reg0(_request);
	where = fipc_get_reg1(_request);
	val = fipc_get_reg2(_request);

	func_ret = pci_bus_write_config_word(bus, devfn, where, val);

	fipc_set_reg0(_request, func_ret);

	return 0;
}

int pci_cleanup_aer_uncorrect_error_status_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	func_ret = pci_cleanup_aer_uncorrect_error_status(pdev);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_disable_device_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	pci_disable_device(pdev);

fail_lookup:
	return ret;
}

int pci_enable_pcie_error_reporting_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	func_ret = pci_enable_pcie_error_reporting(pdev);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pcie_capability_read_word_callee(struct fipc_message *_request)
{
	int pos;
	unsigned short val;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	pos = fipc_get_reg1(_request);
	val = fipc_get_reg2(_request);

	func_ret = pcie_capability_read_word(pdev, pos, &val);

	fipc_set_reg0(_request, func_ret);

	fipc_set_reg2(_request, val);

fail_lookup:
	return ret;
}

int pcie_get_minimum_link_callee(struct fipc_message *_request)
{
	enum pci_bus_speed speed;
	enum pcie_link_width width;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	speed = fipc_get_reg1(_request);
	width = fipc_get_reg2(_request);

	func_ret = pcie_get_minimum_link(pdev, &speed, &width);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_enable_device_mem_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	func_ret = pci_enable_device_mem(pdev);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_request_selected_regions_callee(struct fipc_message *_request)
{
	int type;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	type = fipc_get_reg1(_request);

	func_ret = pci_request_selected_regions(pdev,
				type,
				driver_name);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_request_selected_regions_exclusive_callee(struct fipc_message *_request)
{
	int type;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	type = fipc_get_reg1(_request);

	func_ret = pci_request_selected_regions_exclusive(pdev,
				type,
				driver_name);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_set_master_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	pci_set_master(pdev);

fail_lookup:
	return ret;
}

int pci_save_state_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	func_ret = pci_save_state(pdev);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_release_selected_regions_callee(struct fipc_message *_request)
{
	int r;
	int ret = 0;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	r = fipc_get_reg1(_request);
	pci_release_selected_regions(pdev, r);

fail_lookup:
	return ret;
}

int pci_select_bars_callee(struct fipc_message *_request)
{
	unsigned long flags;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	flags = fipc_get_reg1(_request);

	func_ret = pci_select_bars(pdev, flags);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_wake_from_d3_callee(struct fipc_message *_request)
{
	bool enable;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container;
	struct pci_dev *pdev;

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;
	enable = fipc_get_reg1(_request);

	func_ret = pci_wake_from_d3(pdev, enable);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int trigger_exit_to_lcd(struct thc_channel *_channel, enum dispatch_t disp)
{
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;

	async_msg_set_fn_type(_request,
			disp);

	/* No need to wait for a response here */
	vmfunc_klcd_wrapper(_request, 1);

	return 0;
}

int ixgbe_trigger_dump(void)
{
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;
	struct net_device_container *dev_container;

	async_msg_set_fn_type(_request, TRIGGER_DUMP);
	dev_container = container_of(g_ndev,
		struct net_device_container, net_device);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);

	vmfunc_klcd_wrapper(_request, 1);

	return 0;
}

int sync_user(struct net_device *dev,
		unsigned char *mac,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;

	int func_ret;

	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };


	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			SYNC);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	if (mac) {
		memcpy(m.mac_addr, mac, ETH_ALEN);
		MAC_ADDR_DUMP(m.mac_addr);
		fipc_set_reg3(_request, m.mac_addr_l);
	}

	vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg1(_request);

	return 0;
}

int sync(struct net_device *dev,
		unsigned char *mac,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;

	int func_ret;
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	if (mac) {
		memcpy(m.mac_addr, mac, ETH_ALEN);
		MAC_ADDR_DUMP(m.mac_addr);
		fipc_set_reg3(_request, m.mac_addr_l);
	}
	async_msg_set_fn_type(_request,
			SYNC);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	vmfunc_klcd_wrapper(_request, 1);
	func_ret = fipc_get_reg1(_request);

	return 0;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(sync_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(sync_trampoline)
sync_trampoline(struct net_device *dev,
		unsigned char *mac)
{
	int ( *volatile sync_fp )(struct net_device *,
		unsigned char *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			sync_trampoline);
	sync_fp = sync;
	return sync_fp(dev,
		mac,
		hidden_args);

}
#endif

int unsync_user(struct net_device *dev,
		unsigned char *mac,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;

	int func_ret;
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			UNSYNC);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	if (mac) {
		memcpy(m.mac_addr, mac, ETH_ALEN);
		MAC_ADDR_DUMP(m.mac_addr);
		fipc_set_reg3(_request, m.mac_addr_l);
	}

	vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg1(_request);

	return 0;
}

int unsync(struct net_device *dev,
		unsigned char *mac,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;

	int func_ret;
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	async_msg_set_fn_type(_request,
			UNSYNC);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	if (mac) {
		memcpy(m.mac_addr, mac, ETH_ALEN);
		MAC_ADDR_DUMP(m.mac_addr);
		fipc_set_reg3(_request, m.mac_addr_l);
	}

	vmfunc_klcd_wrapper(_request, 1);
	func_ret = fipc_get_reg1(_request);

	return 0;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(unsync_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(unsync_trampoline)
unsync_trampoline(struct net_device *dev,
		unsigned char *mac)
{
	int ( *volatile unsync_fp )(struct net_device *,
		unsigned char *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			unsync_trampoline);
	unsync_fp = unsync;
	return unsync_fp(dev,
		mac,
		hidden_args);

}
#endif

struct trampoline_hidden_args *unsync_hidden_args;
struct unsync_container *unsync_container;

int __hw_addr_sync_dev_callee(struct fipc_message *_request)
{
	struct net_device_container *dev1_container;
	struct sync_container *sync_container;
	int ret;

	int func_ret;
	addr_list _type;
#ifndef CONFIG_LVD
	struct trampoline_hidden_args *sync_hidden_args;
#endif

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev1_container);
	_type = fipc_get_reg2(_request);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	sync_container = kzalloc(sizeof( struct sync_container   ),
		GFP_KERNEL);
	if (!sync_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

#ifndef CONFIG_LVD
	sync_hidden_args = kzalloc(sizeof( *sync_hidden_args ),
		GFP_KERNEL);
	if (!sync_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc1;
	}
	sync_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(sync_trampoline);
	if (!sync_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	sync_hidden_args->t_handle->hidden_args = sync_hidden_args;
	sync_hidden_args->struct_container = sync_container;
	sync_hidden_args->cspace = c_cspace;
	sync_hidden_args->sync_ep = sync_ep;

	sync_container->sync = LCD_HANDLE_TO_TRAMPOLINE(sync_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )sync_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(sync_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
#endif

	unsync_container = kzalloc(sizeof( struct unsync_container   ),
		GFP_KERNEL);
	if (!unsync_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

#ifndef CONFIG_LVD
	unsync_hidden_args = kzalloc(sizeof( *unsync_hidden_args ),
		GFP_KERNEL);
	if (!unsync_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc2;
	}
	unsync_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(unsync_trampoline);
	if (!unsync_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup2;
	}
	unsync_hidden_args->t_handle->hidden_args = unsync_hidden_args;
	unsync_hidden_args->struct_container = unsync_container;
	unsync_hidden_args->cspace = c_cspace;
	unsync_hidden_args->sync_ep = sync_ep;

	unsync_container->unsync = LCD_HANDLE_TO_TRAMPOLINE(unsync_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )unsync_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(unsync_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
#endif
	func_ret = __hw_addr_sync_dev(
		_type == UC_LIST ? &dev1_container->net_device.uc :
			&dev1_container->net_device.mc,
		( &dev1_container->net_device ),
		( sync_container->sync ),
		( unsync_container->unsync ));

	fipc_set_reg1(_request,
			func_ret);

	return ret;
fail_lookup:
fail_alloc:
#ifndef CONFIG_LVD
fail_alloc1:
fail_dup1:
fail_alloc2:
fail_dup2:
#endif
	return ret;
}

int __hw_addr_unsync_dev_callee(struct fipc_message *_request)
{
	struct net_device_container *dev1_container;
	int ret;

	addr_list _type;
	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev1_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	_type = fipc_get_reg2(_request);

	__hw_addr_unsync_dev(
		_type == UC_LIST ? &dev1_container->net_device.uc :
			&dev1_container->net_device.mc,
		( &dev1_container->net_device ),
		( unsync_container->unsync ));


	return ret;
fail_lookup:
	return ret;
}

irqreturn_t msix_vector_handler(int irq, void *data)
{
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;
	struct irqhandler_t_container *irqhandler_container;
	irqreturn_t irqret;

	WARN_ONCE(!irqs_disabled(),"irqs enabled in %s\n", __func__);

	irqhandler_container = (struct irqhandler_t_container*) data;

	async_msg_set_fn_type(_request, MSIX_IRQ_HANDLER);
	fipc_set_reg0(_request, irq);
	/* pass this irqhandler's other ref cptr */
	fipc_set_reg1(_request, irqhandler_container->other_ref.cptr);

	add_trace_entry(EVENT_MSIX_HANDLER, async_msg_get_fn_type(_request));

	vmfunc_klcd_wrapper(_request, 1);

	irqret = fipc_get_reg0(_request);

	return irqret;
}

struct irq_handler_data_map {
	int irq;
	struct irqhandler_t_container *irqhandler_data;
}irq_map[2];

int reg_irqs;

int request_threaded_irq_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret = 0;
	int irq;
	unsigned long flags;
	struct irqhandler_t_container *irqhandler_container;
	unsigned char *vector_name;

	irqhandler_container = kzalloc(sizeof(struct irqhandler_t_container),
					GFP_KERNEL);
	if (!irqhandler_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	vector_name = kzalloc(IFNAMSIZ + 9, GFP_KERNEL);

	if (!vector_name) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	irq = fipc_get_reg0(_request);
	irqhandler_container->other_ref.cptr = fipc_get_reg1(_request);
	flags = fipc_get_reg2(_request);

	snprintf(vector_name, IFNAMSIZ + 9, "TxRx%d", irq);

	LIBLCD_MSG("%s, request_threaded_irq for %d | name: %s",
			__func__, irq, vector_name);

	func_ret = request_threaded_irq(irq, msix_vector_handler,
				NULL, flags,
				vector_name, (void*) irqhandler_container);

	irq_map[reg_irqs].irq = irq;
	irq_map[reg_irqs].irqhandler_data = irqhandler_container;

	fipc_set_reg0(_request, func_ret);

fail_alloc:
	return ret;
}

int free_irq_callee(struct fipc_message *_request)
{
	unsigned 	int irq;
	struct irqhandler_t_container *irqhandler_container;
	int ret = 0;
	int i;

	irq = fipc_get_reg0(_request);

	LIBLCD_MSG("%s, freeing irq %d", __func__, irq);

	for (i = 0; i < 2; i++) {
		if (irq_map[i].irq = irq) {
			irqhandler_container = irq_map[i].irqhandler_data;
			break;
		}
	}

	free_irq(irq, irqhandler_container);

	return ret;
}

#ifdef CONFIG_LVD
int poll(struct napi_struct *napi,
		int budget)
#else
int poll(struct napi_struct *napi,
		int budget,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	int ret;
	struct fipc_message r;
	INIT_IPC_MSG(&r);
	struct fipc_message *_request = &r;
	struct napi_struct_container *napi_c;

	napi_c = container_of(napi, struct napi_struct_container, napi_struct);

	async_msg_set_fn_type(_request, POLL);
	fipc_set_reg0(_request, budget);
	fipc_set_reg1(_request, napi_c->other_ref.cptr);
	fipc_set_reg2(_request, napi->state);

	if (0)
		printk("%s, poll - budget %d | napi_c->other_ref %lx\n", __func__,
			budget, napi_c->other_ref.cptr);

	add_trace_entry(EVENT_SOFTIRQ_POLL, async_msg_get_fn_type(_request));
	vmfunc_klcd_wrapper(_request, 1);

	ret = fipc_get_reg0(_request);
#ifdef CONFIG_LCD_NAPI_STATE_SYNC
	napi->state = fipc_get_reg1(_request);
#endif
	pr_debug("%s, napi %p, napi->state %lx\n", __func__, napi, napi->state);
	pr_debug("%s, returned %d\n", __func__, ret);

	return ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(poll_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(poll_trampoline)
poll_trampoline(struct napi_struct *napi,
		int budget)
{
	int ( *volatile poll_fp )(struct napi_struct *,
		int ,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			poll_trampoline);
	poll_fp = poll;
	return poll_fp(napi,
		budget,
		hidden_args);

}
#endif

int netif_napi_add_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	struct poll_container *poll_container;
	struct napi_struct_container *napi_struct_container;
	int weight;
	int ret;
	struct napi_struct *napi;

#ifndef CONFIG_LVD
	struct trampoline_hidden_args *poll_hidden_args;
#endif

	ret = glue_cap_lookup_net_device_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	poll_container = kzalloc(sizeof(struct poll_container),
			GFP_KERNEL);
	if (!poll_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	napi_struct_container = kzalloc(sizeof(*napi_struct_container), GFP_KERNEL);

	if (!napi_struct_container) {
		LIBLCD_ERR("kzalloc napi_struct_container");
		goto fail_alloc;
	}

	napi_struct_container->other_ref = __cptr(fipc_get_reg1(_request));

	LIBLCD_MSG("napi_struct_other_ref %lx", napi_struct_container->other_ref.cptr);

	glue_insert_napi_hash(napi_struct_container);

	napi = napi_struct_container->napi;

	weight = fipc_get_reg2(_request);

#ifndef CONFIG_LVD
	poll_hidden_args = kzalloc(sizeof( *poll_hidden_args ),
		GFP_KERNEL);
	if (!poll_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc1;
	}
	poll_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(poll_trampoline);
	if (!poll_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	poll_hidden_args->t_handle->hidden_args = poll_hidden_args;
	poll_hidden_args->struct_container = poll_container;
	poll_hidden_args->cspace = c_cspace;
	poll_hidden_args->sync_ep = sync_ep;

	poll_container->poll = LCD_HANDLE_TO_TRAMPOLINE(poll_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )poll_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(poll_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
#endif
	poll_container->poll = poll;

	netif_napi_add((&dev_container->net_device),
				napi,
				(poll_container->poll),
				weight);

	fipc_set_reg0(_request, napi_struct_container->my_ref.cptr);
	fipc_set_reg1(_request, napi->state);

	LIBLCD_MSG("%s, napi %p | napi->dev %p", __func__, napi, napi->dev);

fail_lookup:
fail_alloc:
#ifndef CONFIG_LVD
fail_alloc1:
fail_dup1:
#endif
	return ret;
}

int netif_napi_del_callee(struct fipc_message *_request)
{
	struct napi_struct_container *napi_container = NULL;
	struct napi_struct *napi;

	glue_lookup_napi_hash(__cptr(fipc_get_reg0(_request)),
						&napi_container);
	napi = napi_container->napi;
#ifdef CONFIG_LCD_NAPI_STATE_SYNC
	napi->state = fipc_get_reg1(_request);
#endif

	netif_napi_del(napi);

	fipc_set_reg0(_request, napi->state);
	return 0;
}

int netif_wake_subqueue_callee(struct fipc_message *_request)
{
	struct net_device_container *dev_container;
	unsigned 	short queue_index;
	int ret;

	ret = glue_cap_lookup_net_device_type(c_cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	queue_index = fipc_get_reg3(_request);
	netif_wake_subqueue(( &dev_container->net_device ),
			queue_index);

fail_lookup:
	return ret;
}

int netif_receive_skb_callee(struct fipc_message *_request)
{
	struct sk_buff *skb;
	struct sk_buff_container *skb_c = NULL;
	int ret = 0;

	int func_ret;
	cptr_t skb_ref;

	skb_ref = __cptr(fipc_get_reg0(_request));

	glue_lookup_skb_hash(skb_ref, &skb_c);

	skb = skb_c->skb;
	skb->head = skb_c->head;
	skb->data = skb_c->data;

	LIBLCD_MSG("%s, skb->dev %p", __func__, skb->dev);
	func_ret = netif_receive_skb(skb);
	LIBLCD_MSG("%s ret %d", __func__, func_ret);

	glue_remove_skb_hash(skb_c);

	kfree(skb_c);

	fipc_set_reg1(_request, func_ret);

	return ret;
}

extern struct lcd *iommu_lcd;

#ifndef LOCAL_SKB
int napi_gro_receive_callee(struct fipc_message *_request)
{
	struct napi_struct *napi;
	struct sk_buff *skb;
	struct sk_buff_container *skb_c;
	struct napi_struct_container *napi_container = NULL;
	int ret = 0;
	int func_ret;
	cptr_t skb_ref;
	cptr_t skb_cptr, skbh_cptr;
	unsigned 	long page = 0ul;
	struct lcd *lcd_struct;
	hva_t hva_out;
	struct skb_shared_info *shinfo;
	struct page *p = NULL;
	unsigned int old_pcount;

	skb_ref = __cptr(fipc_get_reg0(_request));

	glue_lookup_napi_hash(__cptr(fipc_get_reg1(_request)),
						&napi_container);
	napi = napi_container->napi;

	page = fipc_get_reg2(_request);

	napi = &napi_container->napi_struct;

	glue_lookup_skbuff(cptr_table, skb_ref, &skb_c);

	skb = skb_c->skb;
	skb->head = skb_c->head;
	skb->data = skb_c->data;
	skb_cptr = skb_c->skb_cptr;
	skbh_cptr = skb_c->skbh_cptr;

	skb->dev = napi->dev;
	shinfo = skb_shinfo(skb);

	if (shinfo->nr_frags) {
		skb_frag_t *frag = &shinfo->frags[0];

		lcd_struct = iommu_lcd;

		ret = lcd_arch_ept_gpa_to_hva_cpu(lcd_struct->lcd_arch,
			__gpa(page), &hva_out, smp_processor_id());
		if (ret) {
			LIBLCD_WARN("getting gpa:hpa mapping %p:%llx",
				(void*)page, hva_val(hva_out));
			ret = 0;
			goto skip;
		}

		/* set frag page */
		p = frag->page.p = virt_to_page(hva_val(hva_out));
		old_pcount = page_count(skb_frag_page(frag));

		set_page_count(skb_frag_page(frag), 2);

		if (0)
		printk("%s, Frag #%d | page %p | refc %d\n", __func__,
				shinfo->nr_frags,
				frag->page.p,
				page_count(frag->page.p));
	}
skip:
	post_recv = true;

	//skb_pull_inline(skb, ETH_HLEN);
	skb->data += ETH_HLEN;

	func_ret = napi_gro_receive(napi, skb);

	if (p)
		set_page_count(p, 1);

	if (skb_c->tsk == current) {
		lcd_cap_revoke(skb_cptr);
		lcd_cap_revoke(skbh_cptr);
		lcd_unvolunteer_pages(skb_cptr);
		lcd_unvolunteer_pages(skbh_cptr);
	}

	glue_remove_skbuff(skb_c);
	kfree(skb_c);

	fipc_set_reg1(_request, func_ret);

fail_lookup:
	return ret;
}

#else
void ixgbe_pull_tail(struct sk_buff *skb)
{
	struct skb_frag_struct *frag = &skb_shinfo(skb)->frags[0];
	unsigned char *va;
	unsigned int pull_len;

	/*
	 * it is valid to use page_address instead of kmap since we are
	 * working with pages allocated out of the lomem pool per
	 * alloc_page(GFP_ATOMIC)
	 */
	va = skb_frag_address(frag);

	/*
	 * we need the header to contain the greater of either ETH_HLEN or
	 * 60 bytes if the skb->len is less than 60 for skb_pad.
	 */

#define IXGBE_RX_HDR_SIZE	256

	pull_len = eth_get_headlen(va, IXGBE_RX_HDR_SIZE);

	/* align pull length to size of long to optimize memcpy performance */
	skb_copy_to_linear_data(skb, va, ALIGN(pull_len, sizeof(long)));

	//printk("%s, pull len %d\n", __func__, pull_len);
	/* update all of the pointers */
	skb_frag_size_sub(frag, pull_len);
	frag->page_offset += pull_len;
	skb->data_len -= pull_len;
	skb->tail += pull_len;
}

int napi_gro_receive_callee(struct fipc_message *_request)
{
	struct napi_struct *napi;
	struct napi_struct_container *napi_container = NULL;
	struct sk_buff *skb;
	int ret = 0;
	int func_ret = 0;
	unsigned long page = 0ul;
	struct lcd *lcd_struct;
	hva_t hva_out = {0};
	struct skb_shared_info *shinfo;
	struct page *p = NULL;
	unsigned int old_pcount;
	__be16 prot;
	u64 off_sz,
		truesize,
		csum_ipsum,
		hash_l4sw,
		nr_frags_tail_cptr;
	unsigned int pull_len;
	unsigned char nr_frags;
	cptr_t napi_struct_ref;

	u32 frag_off = 0, frag_size = 0;
	nr_frags_tail_cptr = fipc_get_reg0(_request);

	nr_frags = nr_frags_tail_cptr & 0xff;
	/* this is the amount of data copied to the skb->data
	 * by copy_to_linear_data. We have to do it again with
	 * the skb allocated here
	 */
	pull_len = nr_frags_tail_cptr >> 8;
	napi_struct_ref = __cptr(nr_frags_tail_cptr >> 16);

	if (nr_frags) {
		page = fipc_get_reg1(_request);
		off_sz = fipc_get_reg3(_request);
		frag_size = off_sz >> 32;
		frag_off = off_sz;
		/* reverse the effects of pull_tail done at LCD end */
		frag_size += pull_len;
		frag_off -= pull_len;
	}

	prot = fipc_get_reg2(_request);
	truesize = fipc_get_reg5(_request);
	hash_l4sw = fipc_get_reg4(_request);
	csum_ipsum = fipc_get_reg6(_request);

	glue_lookup_napi_hash(napi_struct_ref, &napi_container);

	napi = napi_container->napi;

	WARN_ON(nr_frags > 1);

#define IXGBE_HDR_SIZE	256

	skb = napi_alloc_skb(napi, IXGBE_HDR_SIZE);

	skb->dev = napi->dev;

	shinfo = skb_shinfo(skb);

	if (nr_frags) {
		skb_frag_t *frag;

		lcd_struct = iommu_lcd;

		ret = lcd_arch_ept_gpa_to_hva_cpu(lcd_struct->lcd_arch,
			__gpa(page), &hva_out, smp_processor_id());
		if (ret) {
			LIBLCD_WARN("getting gpa:hpa mapping %p:%llx",
				(void*)page, hva_val(hva_out));
			ret = 0;
			goto skip;
		}

		p = virt_to_page(hva_val(hva_out));

		/* add frag */
		skb_add_rx_frag(skb, shinfo->nr_frags, p, frag_off,
					frag_size,
					truesize); 

		frag = &shinfo->frags[0];

		old_pcount = page_count(skb_frag_page(frag));

		set_page_count(skb_frag_page(frag), 2);
	}

	if (skb_is_nonlinear(skb))
		ixgbe_pull_tail(skb);

	skb->protocol = prot;

	eth_skb_pad(skb);

	skb->queue_mapping = csum_ipsum & 0xffff;
	skb->csum_level = (csum_ipsum >> 16)& 0x3;
	skb->ip_summed = (csum_ipsum >> 18) & 0x3;
	skb->queue_mapping = (csum_ipsum >> 2) & 0x3;

	skb->napi_id = napi->napi_id;
	skb->hash = hash_l4sw;
	skb->l4_hash = (hash_l4sw >> 32) & 0x1;
	skb->sw_hash = (hash_l4sw >> 33) & 0x1;

	skb_pull_inline(skb, ETH_HLEN);

	func_ret = napi_gro_receive(napi, skb);

	if (p)
		set_page_count(p, old_pcount);
skip:
	printk("%s, returned %d\n", __func__, ret);
	fipc_set_reg1(_request, func_ret);
	return ret;
}
#endif

int __napi_alloc_skb_callee(struct fipc_message *_request)
{
	struct napi_struct *napi;
	unsigned 	int len;
	int ret = 0;
	gfp_t gfp_mask;
	struct sk_buff *skb;
	struct sk_buff_container *skb_c;
	struct napi_struct_container *napi_container = NULL;

	unsigned long skb_ord, skbd_ord;
	unsigned long skb_off, skbd_off;
	cptr_t skb_cptr, skbd_cptr;
	cptr_t lcd_skb_cptr, lcd_skbd_cptr;


	len = fipc_get_reg1(_request);
	gfp_mask = fipc_get_reg2(_request);
	lcd_skb_cptr = __cptr(fipc_get_reg3(_request));
	lcd_skbd_cptr = __cptr(fipc_get_reg4(_request));

	glue_lookup_napi_hash(__cptr(fipc_get_reg5(_request)),
				&napi_container);

	napi = napi_container->napi;

	skb = __napi_alloc_skb(napi, len, gfp_mask);

	skb_c = kzalloc(sizeof(*skb_c), GFP_KERNEL);

	if (!skb_c) {
		LIBLCD_MSG("skb_c allocation failed");
		goto fail_alloc;
	}

	skb_c->tsk = current;
	skb_c->skb = skb;
	skb_c->head = skb->head;
	skb_c->data = skb->data;

	glue_insert_skb_hash(skb_c);

	ret = sync_setup_memory(skb, sizeof(struct sk_buff),
			&skb_ord, &skb_cptr, &skb_off);

	ret = sync_setup_memory(skb->head,
		skb_end_offset(skb) + sizeof(struct skb_shared_info),
			&skbd_ord, &skbd_cptr, &skbd_off);

	skb_c->skb_ord = skb_ord;
	skb_c->skbd_ord = skbd_ord;
	skb_c->skb_cptr = skb_cptr;
	skb_c->skbh_cptr = skbd_cptr;

	copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, skb_cptr, lcd_skb_cptr);
	copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, skbd_cptr, lcd_skbd_cptr);

	fipc_set_reg0(_request, skb_ord);
	fipc_set_reg1(_request, skb_off);
	fipc_set_reg2(_request, skbd_ord);
	fipc_set_reg3(_request, skbd_off);
	fipc_set_reg4(_request, skb->data - skb->head);
	fipc_set_reg5(_request, skb_c->my_ref.cptr);

fail_alloc:
	return ret;
}

int ___napi_schedule_irqoff_callee(struct fipc_message *_request)
{
	struct napi_struct_container *napi_container = NULL;
	struct napi_struct *napi;

	glue_lookup_napi_hash(__cptr(fipc_get_reg0(_request)),
						&napi_container);
	napi = napi_container->napi;

#ifdef CONFIG_LCD_NAPI_STATE_SYNC
	napi->state = fipc_get_reg1(_request);
#endif

	napi_schedule_irqoff(napi);

	fipc_set_reg0(_request, napi->state);

	return 0;
}

int __napi_schedule_irqoff_callee(struct fipc_message *_request)
{
	struct napi_struct_container *napi_container = NULL;
	struct napi_struct *napi;

	glue_lookup_napi_hash(__cptr(fipc_get_reg0(_request)),
						&napi_container);
	napi = napi_container->napi;

#ifdef CONFIG_LCD_NAPI_STATE_SYNC
	napi->state = fipc_get_reg1(_request);
#endif
	__napi_schedule_irqoff(napi);

	fipc_set_reg0(_request, napi->state);

	return 0;
}

int __napi_enable_callee(struct fipc_message *_request)
{
	struct napi_struct_container *napi_container = NULL;
	struct napi_struct *napi;

	glue_lookup_napi_hash(__cptr(fipc_get_reg0(_request)),
						&napi_container);
	napi = napi_container->napi;

#ifdef CONFIG_LCD_NAPI_STATE_SYNC
	napi->state = fipc_get_reg1(_request);
#endif

	napi_enable(napi);

	fipc_set_reg0(_request, napi->state);

	return 0;
}

int napi_disable_callee(struct fipc_message *_request)
{
	struct napi_struct_container *napi_container = NULL;
	struct napi_struct *napi;

	glue_lookup_napi_hash(__cptr(fipc_get_reg0(_request)),
						&napi_container);
	napi = napi_container->napi;

#ifdef CONFIG_LCD_NAPI_STATE_SYNC
	napi->state = fipc_get_reg1(_request);
#endif

	napi_disable(napi);

	fipc_set_reg0(_request, napi->state);

	return 0;
}

int napi_complete_done_callee(struct fipc_message *_request)
{
	struct napi_struct_container *napi_container = NULL;
	struct napi_struct *napi;
	int work_done;

	glue_lookup_napi_hash(__cptr(fipc_get_reg0(_request)),
						&napi_container);
	napi = napi_container->napi;
	work_done = fipc_get_reg1(_request);
#ifdef CONFIG_LCD_NAPI_STATE_SYNC
	napi->state = fipc_get_reg2(_request);
#endif

	add_trace_entry(EVENT_NAPI_COMPLETE_DONE, async_msg_get_fn_type(_request));
	napi_complete_done(napi, work_done);

	fipc_set_reg0(_request, napi->state);
	return 0;
}

int synchronize_irq_callee(struct fipc_message *_request)
{
	unsigned int irq;

	irq = fipc_get_reg0(_request);

	synchronize_irq(irq);

	return 0;
}

int __netif_tx_disable_callee(struct fipc_message *_request)
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

	netif_tx_disable(( &dev_container->net_device ));

fail_lookup:
	return ret;
}
