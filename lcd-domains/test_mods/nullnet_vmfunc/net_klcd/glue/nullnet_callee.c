#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/trampoline.h>
#include "../../glue_helper.h"
#include "../nullnet_callee.h"

#include <linux/hashtable.h>
#include <asm/cacheflush.h>

#include "../../rdtsc_helper.h"
#include "../../perf_counter_helper.h"
#include <linux/vmalloc.h>

#include "../../ipc_helper.h"
#include <linux/sort.h>
#include <linux/priv_mempool.h>
#include <lcd_domains/microkernel.h>
#include <lcd_config/post_hook.h>

#define NUM_PACKETS	(1000000)
#define VMALLOC_SZ	(NUM_PACKETS * sizeof(uint64_t))

#define CORRECTION_VALUE	44
//#define NDO_XMIT_TS

//#define INDIVIDUAL_MARKERS
//#define SKBC_PRIVATE_POOL
//#define TIMESTAMP
//#define DUMMY_RDTSC
//#define LCD_MEASUREMENT
//#define TS_SANITY_CHECK
//#define PERF_EVENTS
#ifdef TS_SANITY_CHECK
#define TIMESTAMP
#endif

//#define ONE_SLOT

#define LOOP_THRESHOLD		10000

#if defined(TIMESTAMP) && defined(LCD_MEASUREMENT)
#error "Cannot do both TIMESTAMP and LCD_MEASUREMENT"
#endif

extern bool tdiff_valid;
extern u64 tdiff_disp;

#ifdef TS
// Events
static evt_sel_t ev[8];
static uint64_t  ev_val[8] = { 0 };

static uint32_t ev_num    = 4;
static uint8_t  ev_idx[8] = { 0x24, 0x24, 0x24, 0x24 };
static uint8_t  ev_msk[8] = { 0x1, 0x2, 0x4, 0x8 };

module_param_array( ev_idx, byte, &ev_num, 0 );
module_param_array( ev_msk, byte, NULL,    0 );
#endif

struct rtnl_link_stats64 pkt_stats;
struct glue_cspace *c_cspace;
struct thc_channel *net_async;
struct cptr sync_ep;
extern struct cspace *klcd_cspace;
struct rtnl_link_ops_container *g_ops_container;

static unsigned long pool_pfn_start, pool_pfn_end;

#define NUM_CORES	32
#define NUM_THREADS	NUM_CORES

uint64_t *times_ndo_xmit[4] = {0};
u32 thread = 0;
struct ptstate_t *ptrs[NUM_THREADS] = {0};
struct rtnl_link_stats64 g_stats;
struct thc_channel *xmit_chnl;
priv_pool_t *pool;
priv_pool_t *skbc_pool;
void *pool_base = NULL;
size_t pool_size = 0;

DECLARE_PER_CPU(struct skb_hash_table, skb_hash);

#ifdef TIMESTAMP
uint64_t *times_ndo_xmit = NULL;
uint64_t *times_ipc_send = NULL;
uint64_t *times_ipc_recv = NULL;
#endif
#ifdef LCD_MEASUREMENT
uint64_t *times_con_skb = NULL;
#endif
#ifdef PERF_EVENTS
uint64_t *perf1 = NULL;
uint64_t *perf2 = NULL;
uint64_t *perf3 = NULL;
uint64_t *perf4 = NULL;
#endif
struct kmem_cache *skb_c_cache = NULL;

#ifdef CONFIG_LVD
int __ndo_start_xmit_inner_async(struct sk_buff *skb, struct net_device *dev);
#else
int __ndo_start_xmit_inner_async(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args);
#endif

#define MAX_POOLS	20

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
	for (i = 0; i < MAX_POOLS; i++) {
		base_pools[i] = (char*) __get_free_pages(GFP_KERNEL | __GFP_ZERO,
	                            pool_order);
	}

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

	for (i = 0; i < MAX_POOLS; i++) {
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
	pool_base = base_pools[pools[pool_pick()].start_idx];
	pool_size = best_diff * ((1 << pool_order) * PAGE_SIZE);
	pool = priv_pool_init(SKB_DATA_POOL, (void*) pool_base, pool_size, 2048);

#ifdef SKBC_PRIVATE_POOL
	skbc_pool = priv_pool_init(SKB_CONTAINER_POOL, 10,
				SKB_CONTAINER_SIZE * 2);
#endif
}

void skb_data_pool_free(void)
{
	if (pool)
		priv_pool_destroy(pool);
#ifdef SKBC_PRIVATE_POOL
	if (skbc_pool)
		priv_pool_destroy(skbc_pool);
#endif
}

inline xmit_type_t check_skb_range(struct sk_buff *skb)
{
	unsigned long pfn;
	pfn = ((unsigned long)skb->data) >> PAGE_SHIFT;
	if (pfn >= pool_pfn_start && pfn <= pool_pfn_end) {
		WARN_ON(!skb->private);
		return SHARED_DATA_XMIT;
	} else
		return VOLUNTEER_XMIT;
}

int glue_nullnet_init(void)
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

#ifdef PERF_EVENTS
	perf1 = vzalloc(NUM_PACKETS * sizeof(uint64_t));
	perf2 = vzalloc(NUM_PACKETS * sizeof(uint64_t));
	perf3 = vzalloc(NUM_PACKETS * sizeof(uint64_t));
	perf4 = vzalloc(NUM_PACKETS * sizeof(uint64_t));

	if(!perf1 || !perf2 || !perf3 || !perf4)
		LIBLCD_ERR("%s, error allocating perf memory",
						__func__);
#endif

	times_ndo_xmit[0] = vzalloc(NUM_PACKETS * sizeof(uint64_t));
	times_ndo_xmit[1] = vzalloc(NUM_PACKETS * sizeof(uint64_t));
	times_ndo_xmit[2] = vzalloc(NUM_PACKETS * sizeof(uint64_t));
	times_ndo_xmit[3] = vzalloc(NUM_PACKETS * sizeof(uint64_t));

#ifdef TIMESTAMP
	times_ndo_xmit = vzalloc(NUM_PACKETS * sizeof(uint64_t));
	times_ipc_send = vzalloc(NUM_PACKETS * sizeof(uint64_t));
	times_ipc_recv = vzalloc(NUM_PACKETS * sizeof(uint64_t));

	if (!times_ndo_xmit || !times_ipc_send
			|| !times_ipc_recv)
		printk("%s, cannot alloc times var\n", __func__);
#endif

#ifdef LCD_MEASUREMENT
	times_con_skb = vzalloc(NUM_PACKETS * sizeof(uint64_t));
	if (!times_con_skb)
		printk("%s, cannot alloc times var\n", __func__);
#endif

	skb_c_cache = kmem_cache_create("skb_c_cache",
				sizeof(struct sk_buff_container),
				0,
				SLAB_HWCACHE_ALIGN|SLAB_PANIC,
				NULL);
	if (!skb_c_cache)
		printk("WARN: skb_container cache not created\n");

	return 0;
fail2:
	glue_cap_exit();
fail1:
	return ret;
}

void glue_nullnet_exit()
{
	int i;
	glue_cap_destroy(c_cspace);
	glue_cap_exit();
	skb_data_pool_free();

	for (i = 0; i < 4; i++) {
		if (times_ndo_xmit[i])
			vfree(times_ndo_xmit[i]);
	}

#ifdef TIMESTAMP
	if (times_ndo_xmit)
		vfree(times_ndo_xmit);

	if (times_ipc_send)
		vfree(times_ipc_send);

	if (times_ipc_recv)
		vfree(times_ipc_recv);
#endif

#ifdef LCD_MEASUREMENT
	if (times_con_skb)
		vfree(times_con_skb);
#endif

#ifdef PERF_EVENTS
	if (perf1)
		vfree(perf1);

	if (perf2)
		vfree(perf2);
	if (perf3)
		vfree(perf3);
	if (perf4)
		vfree(perf4);
#endif
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

#ifdef CONFIG_LVD
int ndo_init(struct net_device *dev)
#else
int ndo_init(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
#endif
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct net_device_container *net_dev_container;

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	async_msg_set_fn_type(request, NDO_INIT);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	ret = vmfunc_klcd_wrapper(request, 1);

	ret = fipc_get_reg1(request);
	return ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_init_trampoline);
int LCD_TRAMPOLINE_LINKAGE(ndo_init_trampoline) ndo_init_trampoline(struct net_device *dev)
{
	int ( *volatile ndo_init_fp )(struct net_device *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_init_trampoline);
	ndo_init_fp = ndo_init;
	return ndo_init_fp(dev, hidden_args);

}
#endif

#ifdef CONFIG_LVD
void ndo_uninit(struct net_device *dev)
#else
void ndo_uninit(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
#endif
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct net_device_container *net_dev_container;
	
	net_dev_container = container_of(dev, struct net_device_container, net_device);

	async_msg_set_fn_type(request, NDO_UNINIT);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	ret = vmfunc_klcd_wrapper(request, 1);

	return;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_uninit_trampoline);
void LCD_TRAMPOLINE_LINKAGE(ndo_uninit_trampoline)
ndo_uninit_trampoline(struct net_device *dev)
{
	void ( *volatile ndo_uninit_fp )(struct net_device *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_uninit_trampoline);
	ndo_uninit_fp = ndo_uninit;
	return ndo_uninit_fp(dev, hidden_args);

}
#endif

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_start_xmit_trampoline);
int LCD_TRAMPOLINE_LINKAGE(ndo_start_xmit_trampoline)
 ndo_start_xmit_trampoline(struct sk_buff *skb, struct net_device *dev)
{
	int ( *volatile ndo_start_xmit_fp )(struct sk_buff *, struct net_device *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_start_xmit_trampoline);
	ndo_start_xmit_fp = ndo_start_xmit_async_landing;
	return ndo_start_xmit_fp(skb, dev, hidden_args);

}
#endif

#ifdef CONFIG_LVD
int ndo_validate_addr(struct net_device *dev)
#else
int ndo_validate_addr(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
#endif
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct net_device_container *net_dev_container;

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	async_msg_set_fn_type(request, NDO_VALIDATE_ADDR);

	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	ret = vmfunc_klcd_wrapper(request, 1);

	ret = fipc_get_reg1(request);

	return ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_validate_addr_trampoline);
int LCD_TRAMPOLINE_LINKAGE(ndo_validate_addr_trampoline) ndo_validate_addr_trampoline(struct net_device *dev)
{
	int ( *volatile ndo_validate_addr_fp )(struct net_device *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_validate_addr_trampoline);
	ndo_validate_addr_fp = ndo_validate_addr;
	return ndo_validate_addr_fp(dev, hidden_args);

}
#endif

#ifdef CONFIG_LVD
void ndo_set_rx_mode(struct net_device *dev)
#else
void ndo_set_rx_mode(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
#endif
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct net_device_container *net_dev_container;

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	async_msg_set_fn_type(request, NDO_SET_RX_MODE);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	ret = vmfunc_klcd_wrapper(request, 1);

	return;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_set_rx_mode_trampoline);
void LCD_TRAMPOLINE_LINKAGE(ndo_set_rx_mode_trampoline) ndo_set_rx_mode_trampoline(struct net_device *dev)
{
	void ( *volatile ndo_set_rx_mode_fp )(struct net_device *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_set_rx_mode_trampoline);
	ndo_set_rx_mode_fp = ndo_set_rx_mode;
	return ndo_set_rx_mode_fp(dev, hidden_args);

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

//DONE
#ifdef CONFIG_LVD
int ndo_set_mac_address(struct net_device *dev, void *addr)
#else
int ndo_set_mac_address(struct net_device *dev, void *addr, struct trampoline_hidden_args *hidden_args)
#endif
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;
	unsigned 	long addr_mem_sz;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
	struct net_device_container *net_dev_container;
#ifndef CONFIG_LVD
	cptr_t sync_end;
	int sync_ret;
#endif
	net_dev_container = container_of(dev, struct net_device_container, net_device);


	async_msg_set_fn_type(request, NDO_SET_MAC_ADDRESS);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	ret = sync_setup_memory(addr, sizeof(struct sockaddr), &addr_mem_sz, &addr_cptr, &addr_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}
#ifndef CONFIG_LVD
	lcd_set_cr0(addr_cptr);
	lcd_set_r0(addr_mem_sz);
	lcd_set_r1(addr_offset);

	LIBLCD_MSG("%s: cptr %lu | order %lu | offset %lu",
		__func__, addr_cptr.cptr, addr_mem_sz, addr_offset);

	sync_ret = lcd_sync_send(sync_end);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to send");
		goto fail_sync;
	}
#endif
	ret = fipc_get_reg1(request);

	LIBLCD_MSG("%s: returned %d", __func__, ret);

fail_virt:
//fail_sync:
	return ret;
}


#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_set_mac_address_trampoline);
int LCD_TRAMPOLINE_LINKAGE(ndo_set_mac_address_trampoline) ndo_set_mac_address_trampoline(struct net_device *dev, void *addr)
{
	int ( *volatile ndo_set_mac_address_fp )(struct net_device *, void *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_set_mac_address_trampoline);
	ndo_set_mac_address_fp = ndo_set_mac_address;
	return ndo_set_mac_address_fp(dev, addr, hidden_args);

}
#endif

#ifdef CONFIG_LVD
struct rtnl_link_stats64 *ndo_get_stats64(struct net_device *dev, struct rtnl_link_stats64 *stats)
#else
struct rtnl_link_stats64 *ndo_get_stats64(struct net_device *dev, struct rtnl_link_stats64 *stats, struct trampoline_hidden_args *hidden_args)
#endif
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct net_device_container *net_dev_container;

	net_dev_container = container_of(dev, struct net_device_container, net_device);


	async_msg_set_fn_type(request, NDO_GET_STATS64);

	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

#if 1
	ret = vmfunc_klcd_wrapper(request, 1);
	stats->tx_packets = fipc_get_reg1(request);
	stats->tx_bytes = fipc_get_reg2(request);
#else
	stats->tx_packets = g_stats.tx_packets;
	stats->tx_bytes = g_stats.tx_bytes;
#endif
	return stats;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_get_stats64_trampoline);
struct rtnl_link_stats64 LCD_TRAMPOLINE_LINKAGE(ndo_get_stats64_trampoline) *ndo_get_stats64_trampoline(struct net_device *dev, struct rtnl_link_stats64 *stats)
{
	struct rtnl_link_stats64* ( *volatile ndo_get_stats64_fp )(struct net_device *, struct rtnl_link_stats64 *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_get_stats64_trampoline);
	ndo_get_stats64_fp = ndo_get_stats64;
	return ndo_get_stats64_fp(dev, stats, hidden_args);

}
#endif

#ifdef CONFIG_LVD
int ndo_change_carrier(struct net_device *dev, bool new_carrier)
#else
int ndo_change_carrier(struct net_device *dev, bool new_carrier, struct trampoline_hidden_args *hidden_args)
#endif
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct net_device_container *net_dev_container;

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	async_msg_set_fn_type(request, NDO_CHANGE_CARRIER);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	ret = vmfunc_klcd_wrapper(request, 1);
	ret = fipc_get_reg1(request);

	return ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(ndo_change_carrier_trampoline);
int LCD_TRAMPOLINE_LINKAGE(ndo_change_carrier_trampoline) ndo_change_carrier_trampoline(struct net_device *dev, bool new_carrier)
{
	int ( *volatile ndo_change_carrier_fp )(struct net_device *, bool , struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_change_carrier_trampoline);
	ndo_change_carrier_fp = ndo_change_carrier;
	return ndo_change_carrier_fp(dev, new_carrier, hidden_args);

}
#endif

#ifdef CONFIG_LVD
int validate(struct nlattr **tb, struct nlattr **data)
#else
int validate(struct nlattr **tb, struct nlattr **data, struct trampoline_hidden_args *hidden_args)
#endif
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;

	async_msg_set_fn_type(request, VALIDATE);
	ret = vmfunc_klcd_wrapper(request, 1);
	ret = fipc_get_reg1(request);
	return ret;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(validate_trampoline);
int LCD_TRAMPOLINE_LINKAGE(validate_trampoline) validate_trampoline(struct nlattr **tb, struct nlattr **data)
{
	int ( *volatile validate_fp )(struct nlattr **, struct nlattr **, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, validate_trampoline);
	validate_fp = validate;
	return validate_fp(tb, data, hidden_args);

}
#endif

#ifndef CONFIG_LVD
#ifdef CONFIG_LVD
void setup_device_ops_trampolines(struct net_device_ops_container *netdev_ops_container)
#else
void setup_device_ops_trampolines(struct net_device_ops_container *netdev_ops_container, struct trampoline_hidden_args *hargs)
#endif
{
	struct thc_channel *chnl = hargs->async_chnl;
	cptr_t sync_ep = hargs->sync_ep;

	struct trampoline_hidden_args *ndo_init_hidden_args;
	struct trampoline_hidden_args *ndo_uninit_hidden_args;
	struct trampoline_hidden_args *ndo_start_xmit_hidden_args;
	struct trampoline_hidden_args *ndo_validate_addr_hidden_args;
	struct trampoline_hidden_args *ndo_set_rx_mode_hidden_args;
	struct trampoline_hidden_args *ndo_set_mac_address_hidden_args;
	struct trampoline_hidden_args *ndo_get_stats64_hidden_args;
	struct trampoline_hidden_args *ndo_change_carrier_hidden_args;
	int ret;

	ndo_init_hidden_args = kzalloc(sizeof( *ndo_init_hidden_args ), GFP_KERNEL);
	if (!ndo_init_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}
	ndo_init_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_init_trampoline);
	if (!ndo_init_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		lcd_exit(-1);
	}
	ndo_init_hidden_args->t_handle->hidden_args = ndo_init_hidden_args;
	ndo_init_hidden_args->struct_container = netdev_ops_container;
	ndo_init_hidden_args->cspace = c_cspace;	
	netdev_ops_container->net_device_ops.ndo_init = LCD_HANDLE_TO_TRAMPOLINE(ndo_init_hidden_args->t_handle);

	ndo_uninit_hidden_args = kzalloc(sizeof( *ndo_uninit_hidden_args ), GFP_KERNEL);
	if (!ndo_uninit_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}
	ndo_uninit_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_uninit_trampoline);
	if (!ndo_uninit_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		lcd_exit(-1);
	}
	ndo_uninit_hidden_args->t_handle->hidden_args = ndo_uninit_hidden_args;
	ndo_uninit_hidden_args->struct_container = netdev_ops_container;
	ndo_uninit_hidden_args->cspace = c_cspace;
	netdev_ops_container->net_device_ops.ndo_uninit = LCD_HANDLE_TO_TRAMPOLINE(ndo_uninit_hidden_args->t_handle);
	ndo_start_xmit_hidden_args = kzalloc(sizeof( *ndo_start_xmit_hidden_args ), GFP_KERNEL);
	if (!ndo_start_xmit_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}
	ndo_start_xmit_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_start_xmit_trampoline);
	if (!ndo_start_xmit_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		lcd_exit(-1);
	}
	ndo_start_xmit_hidden_args->t_handle->hidden_args = ndo_start_xmit_hidden_args;
	ndo_start_xmit_hidden_args->struct_container = netdev_ops_container;
	ndo_start_xmit_hidden_args->cspace = c_cspace;
	netdev_ops_container->net_device_ops.ndo_start_xmit = LCD_HANDLE_TO_TRAMPOLINE(ndo_start_xmit_hidden_args->t_handle);
	ndo_validate_addr_hidden_args = kzalloc(sizeof( *ndo_validate_addr_hidden_args ), GFP_KERNEL);
	if (!ndo_validate_addr_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}
	ndo_validate_addr_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_validate_addr_trampoline);
	if (!ndo_validate_addr_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		lcd_exit(-1);
	}
	ndo_validate_addr_hidden_args->t_handle->hidden_args = ndo_validate_addr_hidden_args;
	ndo_validate_addr_hidden_args->struct_container = netdev_ops_container;
	ndo_validate_addr_hidden_args->cspace = c_cspace;
	netdev_ops_container->net_device_ops.ndo_validate_addr = LCD_HANDLE_TO_TRAMPOLINE(ndo_validate_addr_hidden_args->t_handle);
	ndo_set_rx_mode_hidden_args = kzalloc(sizeof( *ndo_set_rx_mode_hidden_args ), GFP_KERNEL);
	if (!ndo_set_rx_mode_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}
	ndo_set_rx_mode_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_set_rx_mode_trampoline);
	if (!ndo_set_rx_mode_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		lcd_exit(-1);
	}
	ndo_set_rx_mode_hidden_args->t_handle->hidden_args = ndo_set_rx_mode_hidden_args;
	ndo_set_rx_mode_hidden_args->struct_container = netdev_ops_container;
	ndo_set_rx_mode_hidden_args->cspace = c_cspace;
	netdev_ops_container->net_device_ops.ndo_set_rx_mode = LCD_HANDLE_TO_TRAMPOLINE(ndo_set_rx_mode_hidden_args->t_handle);
	ndo_set_mac_address_hidden_args = kzalloc(sizeof( *ndo_set_mac_address_hidden_args ), GFP_KERNEL);
	if (!ndo_set_mac_address_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}
	ndo_set_mac_address_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_set_mac_address_trampoline);
	if (!ndo_set_mac_address_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		lcd_exit(-1);
	}
	ndo_set_mac_address_hidden_args->t_handle->hidden_args = ndo_set_mac_address_hidden_args;
	ndo_set_mac_address_hidden_args->struct_container = netdev_ops_container;
	ndo_set_mac_address_hidden_args->cspace = c_cspace;
	netdev_ops_container->net_device_ops.ndo_set_mac_address = LCD_HANDLE_TO_TRAMPOLINE(ndo_set_mac_address_hidden_args->t_handle);
	ndo_get_stats64_hidden_args = kzalloc(sizeof( *ndo_get_stats64_hidden_args ), GFP_KERNEL);
	if (!ndo_get_stats64_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}
	ndo_get_stats64_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_get_stats64_trampoline);
	if (!ndo_get_stats64_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		lcd_exit(-1);
	}
	ndo_get_stats64_hidden_args->t_handle->hidden_args = ndo_get_stats64_hidden_args;
	ndo_get_stats64_hidden_args->struct_container = netdev_ops_container;
	ndo_get_stats64_hidden_args->cspace = c_cspace;
	netdev_ops_container->net_device_ops.ndo_get_stats64 = LCD_HANDLE_TO_TRAMPOLINE(ndo_get_stats64_hidden_args->t_handle);
	ndo_change_carrier_hidden_args = kzalloc(sizeof( *ndo_change_carrier_hidden_args ), GFP_KERNEL);
	if (!ndo_change_carrier_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}
	ndo_change_carrier_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(ndo_change_carrier_trampoline);
	if (!ndo_change_carrier_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		lcd_exit(-1);
	}
	ndo_change_carrier_hidden_args->t_handle->hidden_args = ndo_change_carrier_hidden_args;
	ndo_change_carrier_hidden_args->struct_container = netdev_ops_container;
	ndo_change_carrier_hidden_args->cspace = c_cspace;
	netdev_ops_container->net_device_ops.ndo_change_carrier = LCD_HANDLE_TO_TRAMPOLINE(ndo_change_carrier_hidden_args->t_handle);

        ret = set_memory_x(((unsigned long)ndo_init_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(ndo_init_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem ndo_init");
                goto fail1;
        }

        ret = set_memory_x(((unsigned long)ndo_uninit_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(ndo_uninit_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem ndo_uninit");
                goto fail2;
        }

        ret = set_memory_x(((unsigned long)ndo_start_xmit_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(ndo_start_xmit_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem ndo_start_xmit");
                goto fail3;
        }

        ret = set_memory_x(((unsigned long)ndo_validate_addr_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(ndo_validate_addr_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem ndo_validate_addr");
                goto fail4;
        }

        ret = set_memory_x(((unsigned long)ndo_set_rx_mode_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(ndo_set_rx_mode_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem ndo_set_rx_mode");
                goto fail5;
        }

        ret = set_memory_x(((unsigned long)ndo_set_mac_address_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(ndo_set_mac_address_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem ndo_set_mac_address");
                goto fail6;
        }

        ret = set_memory_x(((unsigned long)ndo_get_stats64_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(ndo_get_stats64_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem ndo_get_stats64");
                goto fail7;
        }

        ret = set_memory_x(((unsigned long)ndo_change_carrier_hidden_args->t_handle) & PAGE_MASK,
                        ALIGN(LCD_TRAMPOLINE_SIZE(ndo_change_carrier_trampoline),
                                PAGE_SIZE) >> PAGE_SHIFT);
        if (ret) {
                LIBLCD_ERR("set mem ndo_change_carrier");
                goto fail8;
        }
        ndo_init_hidden_args->sync_ep = sync_ep;
        ndo_init_hidden_args->async_chnl = chnl;
        

        ndo_uninit_hidden_args->sync_ep = sync_ep;
        ndo_uninit_hidden_args->async_chnl = chnl;
        

        ndo_start_xmit_hidden_args->sync_ep = sync_ep;
        ndo_start_xmit_hidden_args->async_chnl = xmit_chnl;
        

        ndo_validate_addr_hidden_args->sync_ep = sync_ep;
        ndo_validate_addr_hidden_args->async_chnl = chnl;
        

        ndo_set_rx_mode_hidden_args->sync_ep = sync_ep;
        ndo_set_rx_mode_hidden_args->async_chnl = chnl;
        

        ndo_set_mac_address_hidden_args->sync_ep = sync_ep;
        ndo_set_mac_address_hidden_args->async_chnl = chnl;
        

        ndo_get_stats64_hidden_args->sync_ep = sync_ep;
        ndo_get_stats64_hidden_args->async_chnl = chnl;
        

        ndo_change_carrier_hidden_args->sync_ep = sync_ep;
        ndo_change_carrier_hidden_args->async_chnl = chnl;

fail1:
fail2:
fail3:
fail4:
fail5:
fail6:
fail7:
fail8:
	return;
}
#endif

void setup_sync_callee(struct fipc_message *_request)
{
	int ret;
	struct page *p;
	unsigned int pool_ord;
	cptr_t pool_cptr;
	cptr_t lcd_pool_cptr;

	/* get LCD's pool cptr */
	lcd_pool_cptr = __cptr(fipc_get_reg0(_request));

	p = virt_to_head_page(pool->pool);

        pool_ord = ilog2(roundup_pow_of_two((
				pool->total_pages * PAGE_SIZE)
				>> PAGE_SHIFT));

        ret = lcd_volunteer_pages(p, pool_ord, &pool_cptr);

	if (ret) {
		LIBLCD_ERR("volunteer shared data pool");
		goto fail_vol;
	}

	pool_pfn_start = (unsigned long)pool->pool >> PAGE_SHIFT;
	pool_pfn_end = pool_pfn_start + pool->total_pages;

	printk("%s, pool pfn start %lu | end %lu\n", __func__,
			pool_pfn_start, pool_pfn_end);

	copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, pool_cptr, lcd_pool_cptr);

	fipc_set_reg0(_request, pool_ord);

fail_vol:
	return;
}

//DONE
#ifdef CONFIG_LVD
void setup(struct net_device *dev)
#else
void setup(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
#endif
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
#ifndef CONFIG_LVD
	struct setup_container *setup_container;
#endif
	struct net_device_container *net_dev_container;
	struct net_device_ops_container *netdev_ops_container;


	net_dev_container = container_of(dev,
			struct net_device_container,
			net_device);

	async_msg_set_fn_type(_request, SETUP);

	netdev_ops_container = kzalloc(sizeof(*netdev_ops_container), GFP_KERNEL);
	if (!netdev_ops_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_net_device_ops_type(c_cspace, netdev_ops_container, &netdev_ops_container->my_ref);
	if (ret) {
		LIBLCD_ERR("insert");
		goto fail_insert;
	}

	ret = glue_cap_insert_net_device_type(c_cspace, net_dev_container, &net_dev_container->my_ref);
	if (ret) {
		LIBLCD_ERR("insert");
		goto fail_insert;
	}

#ifndef CONFIG_LVD
	/*fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);*/
	setup_container = (struct setup_container*)hidden_args->struct_container;
	LIBLCD_MSG("%s sending setup_container cptr: other_ref %lu", __func__, setup_container->other_ref.cptr);
	fipc_set_reg2(_request, setup_container->other_ref.cptr);
#endif
	printk("%s sending netdev_cptr: %p my_ref %lu", __func__, dev, net_dev_container->my_ref.cptr);
	printk(" | other_ref %lu\n", net_dev_container->other_ref.cptr);
	fipc_set_reg1(_request, net_dev_container->other_ref.cptr);
	fipc_set_reg3(_request, net_dev_container->my_ref.cptr);
	fipc_set_reg4(_request, netdev_ops_container->my_ref.cptr);

	ret = vmfunc_klcd_wrapper(_request, 1);

	printk("%s:%d, done with msg %d\n", __func__, __LINE__, async_msg_get_fn_type(_request));

	net_dev_container->net_device.flags = fipc_get_reg1(_request);
	net_dev_container->net_device.priv_flags = fipc_get_reg2(_request);
	net_dev_container->net_device.features = fipc_get_reg3(_request);
	net_dev_container->net_device.hw_features = fipc_get_reg4(_request);
	net_dev_container->net_device.hw_enc_features = fipc_get_reg5(_request);

	netdev_ops_container->other_ref.cptr = fipc_get_reg6(_request);


	net_dev_container->net_device.netdev_ops = &netdev_ops_container->net_device_ops;

#ifdef CONFIG_LVD
	netdev_ops_container->net_device_ops.ndo_init = ndo_init;
	netdev_ops_container->net_device_ops.ndo_uninit = ndo_uninit;
	netdev_ops_container->net_device_ops.ndo_start_xmit = __ndo_start_xmit_inner_async;
	netdev_ops_container->net_device_ops.ndo_validate_addr = ndo_validate_addr;
	netdev_ops_container->net_device_ops.ndo_set_rx_mode = ndo_set_rx_mode;
	netdev_ops_container->net_device_ops.ndo_set_mac_address = ndo_set_mac_address;
	netdev_ops_container->net_device_ops.ndo_get_stats64 = ndo_get_stats64;
	netdev_ops_container->net_device_ops.ndo_change_carrier = ndo_change_carrier;
#else
	setup_device_ops_trampolines(netdev_ops_container, hidden_args);
#endif
	net_dev_container->net_device.rtnl_link_ops =
		&g_ops_container->rtnl_link_ops;
fail_alloc:
fail_insert:
	printk("%s: returned\n", __func__);
	return;
}

#ifndef CONFIG_LVD
LCD_TRAMPOLINE_DATA(setup_trampoline);
void LCD_TRAMPOLINE_LINKAGE(setup_trampoline) setup_trampoline(struct net_device *dev)
{
	void ( *volatile setup_fp )(struct net_device *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, setup_trampoline);
	setup_fp = setup;
	return setup_fp(dev, hidden_args);

}
#endif

//DONE
int register_netdevice_callee(struct fipc_message *request)
{
	struct net_device_container *dev_container;
	int ret;

	ret = glue_cap_lookup_net_device_type(c_cspace, __cptr(fipc_get_reg1(request)), &dev_container);

	if (ret) {
		LIBLCD_MSG("lookup failed");
		goto fail_lookup;
	}

	dev_container->net_device.dev_addr = kmalloc(MAX_ADDR_LEN, GFP_KERNEL);

	/* caller must hold rtnl mutex */
	rtnl_lock();
	ret = register_netdevice(( &dev_container->net_device ));
	rtnl_unlock();

	fipc_set_reg0(request, ret);
fail_lookup:
	return ret;
}

//DONE
int ether_setup_callee(struct fipc_message *request)
{
	int ret;
	struct net_device_container *dev;

	cptr_t netdev_ref = __cptr(fipc_get_reg1(request));

	ret = glue_cap_lookup_net_device_type(c_cspace, netdev_ref, &dev);
	if (ret) {
		LIBLCD_MSG("lookup failed");
		goto fail_lookup;
	}

	LIBLCD_MSG("Calling ether_setup");
	ether_setup(&dev->net_device);
	LIBLCD_MSG("%s returned", __func__);
fail_lookup:
	return ret;
}

int sync_recv_data(unsigned long *order, unsigned long *off, cptr_t *data_cptr, gva_t *data_gva, cptr_t sync_ep)
{
    int ret;
	ret = lcd_cptr_alloc(data_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail1;
	}

#ifndef CONFIG_LVD
	lcd_set_cr0(*data_cptr);

	ret = lcd_sync_recv(sync_ep);

	lcd_set_cr0(CAP_CPTR_NULL);
#endif
	if (ret) {
		LIBLCD_ERR("failed to recv");
		goto fail2;
	}

#ifndef CONFIG_LVD
	*order = lcd_r0();
	*off = lcd_r1();
#endif
	ret = lcd_map_virt(*data_cptr, *order, data_gva);
	if (ret) {
		LIBLCD_ERR("failed to map void *addr");
		goto fail3;
	}
	return 0;

fail3:
    lcd_cap_delete(*data_cptr);
fail2:
    lcd_cptr_free(*data_cptr);
fail1:
	return ret;
}


//DONE
int eth_mac_addr_callee(struct fipc_message *request)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *response;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	gva_t p_gva;
unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(request);

	ret = glue_cap_lookup_net_device_type(c_cspace, __cptr(fipc_get_reg1(request)), &dev_container);

	if (ret) {
		LIBLCD_MSG("lookup failed");
		goto fail_lookup;
	}
	sync_ret = sync_recv_data(&mem_order, &p_offset, &p_cptr, &p_gva, sync_ep);
	if (sync_ret) {
		LIBLCD_ERR("failed to recv data");
		lcd_exit(-1);
	}

	ret = eth_mac_addr(&dev_container->net_device, (void*)(gva_val(p_gva) + p_offset));

	LIBLCD_MSG("eth_mac_addr returned %d", ret);

	fipc_set_reg1(response, ret);
fail_lookup:
	return ret;
}

int eth_validate_addr_callee(struct fipc_message *request)
{
	int ret;
	struct net_device_container *net_dev_container;
	cptr_t netdev_ref = __cptr(fipc_get_reg1(request));

	LIBLCD_MSG("cptr %lu", netdev_ref.cptr);

	ret = glue_cap_lookup_net_device_type(c_cspace, netdev_ref, &net_dev_container);
	if (ret) {
		LIBLCD_MSG("lookup failed");
		goto fail_lookup;
	}

	ret = eth_validate_addr(&net_dev_container->net_device);

	fipc_set_reg1(request, ret);

fail_lookup:
	return ret;
}

int free_netdev_callee(struct fipc_message *request)
{
	int ret;
	struct net_device_container *dev_container;
	struct net_device *dev;

	ret = glue_cap_lookup_net_device_type(c_cspace, __cptr(fipc_get_reg1(request)), &dev_container);

	if (ret) {
		LIBLCD_MSG("lookup failed");
		goto fail_lookup;
	}

	dev = &dev_container->net_device;

	free_netdev(dev);
fail_lookup:
	return ret;
}

int netif_carrier_off_callee(struct fipc_message *request)
{
	int ret;
	struct net_device_container *net_dev_container;
	cptr_t netdev_ref = __cptr(fipc_get_reg1(request));

	ret = glue_cap_lookup_net_device_type(c_cspace, netdev_ref, &net_dev_container);
	if (ret) {
		LIBLCD_MSG("lookup failed");
		goto fail_lookup;
	}

	netif_carrier_off(&net_dev_container->net_device);
fail_lookup:
	return ret;
}

int netif_carrier_on_callee(struct fipc_message *request)
{
	int ret;
	struct net_device_container *net_dev_container;
	cptr_t netdev_ref = __cptr(fipc_get_reg1(request));

	ret = glue_cap_lookup_net_device_type(c_cspace, netdev_ref, &net_dev_container);
	if (ret) {
		LIBLCD_MSG("lookup failed");
		goto fail_lookup;
	}
	netif_carrier_on(&net_dev_container->net_device);
fail_lookup:
	return ret;
}

int __rtnl_link_register_callee(struct fipc_message *request)
{
	struct rtnl_link_ops_container *ops_container;
	int ret = 0;
#ifndef CONFIG_LVD
	struct trampoline_hidden_args *validate_hidden_args;
	struct trampoline_hidden_args *setup_hidden_args;
#endif
	int err;

	ops_container = kzalloc(sizeof( struct rtnl_link_ops_container   ), GFP_KERNEL);
	if (!ops_container) {
		LIBLCD_ERR("kzalloc");
		goto fail7;
	}
	err = glue_cap_insert_rtnl_link_ops_type(c_cspace, ops_container, &ops_container->my_ref);
	if (err) {
		LIBLCD_ERR("lcd insert");
		goto fail7;
	}

	ops_container->other_ref.cptr = fipc_get_reg0(request);


#ifndef CONFIG_LVD
	setup_hidden_args = kzalloc(sizeof(*setup_hidden_args),
				GFP_KERNEL);

	if (!setup_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}

	setup_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(
				setup_trampoline);
	if (!setup_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		lcd_exit(-1);
	}
	setup_hidden_args->t_handle->hidden_args = setup_hidden_args;
	setup_hidden_args->struct_container = ops_container;
	setup_hidden_args->cspace = c_cspace;

	ops_container->rtnl_link_ops.setup = LCD_HANDLE_TO_TRAMPOLINE(setup_hidden_args->t_handle);
	ret = set_memory_x(((unsigned long)setup_hidden_args->t_handle) & PAGE_MASK,
			ALIGN(LCD_TRAMPOLINE_SIZE(setup_trampoline),
				PAGE_SIZE) >> PAGE_SHIFT);
	if (ret) {
		LIBLCD_ERR("set mem nx");
		goto fail3;
	}

	validate_hidden_args = kzalloc(sizeof( *validate_hidden_args ), GFP_KERNEL);
	if (!validate_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}
	validate_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(validate_trampoline);
	if (!validate_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		lcd_exit(-1);
	}
	validate_hidden_args->t_handle->hidden_args = validate_hidden_args;
	validate_hidden_args->struct_container = ops_container;
	validate_hidden_args->cspace = c_cspace;

	ret = set_memory_x((
		(unsigned long)validate_hidden_args->t_handle)
			& PAGE_MASK,
			ALIGN(LCD_TRAMPOLINE_SIZE(
				validate_trampoline),
				PAGE_SIZE) >> PAGE_SHIFT);
	if (ret) {
		LIBLCD_ERR("set mem nx");
		goto fail3;
	}

	ops_container->rtnl_link_ops.validate = LCD_HANDLE_TO_TRAMPOLINE(
			validate_hidden_args->t_handle);
#else
	ops_container->rtnl_link_ops.setup = setup;
	ops_container->rtnl_link_ops.validate = validate;
#endif
	ops_container->rtnl_link_ops.kind = "dummy"; 

	printk("%s, acquiring rtnl_lock\n", __func__);

	rtnl_lock();

	printk("%s, acquired rtnl_lock, calling original func\n", __func__);

	ret = __rtnl_link_register(( &ops_container->rtnl_link_ops ));

	printk("%s, original func returned ret %d\n", __func__, ret);

	rtnl_unlock();

	fipc_set_reg0(request, ret);

	fipc_set_reg1(request, ops_container->my_ref.cptr);

	g_ops_container = ops_container;

fail7:
#ifndef CONFIG_LVD
fail3:
#endif
	return ret;
}

//DONE
int __rtnl_link_unregister_callee(struct fipc_message *request)
{
	struct rtnl_link_ops *ops;
	int ret = 0;

	ops = kzalloc(sizeof( *ops ), GFP_KERNEL);
	if (!ops) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	__rtnl_link_unregister(ops);
	return ret;
}

//DONE
int rtnl_link_unregister_callee(struct fipc_message *request)
{
	struct rtnl_link_ops_container *ops_container;
	int ret;

#ifndef CONFIG_LVD
	struct trampoline_hidden_args *validate_hidden_args;
#endif

	ret = glue_cap_lookup_rtnl_link_ops_type(c_cspace, __cptr(fipc_get_reg2(request)), &ops_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	rtnl_link_unregister(( &ops_container->rtnl_link_ops ));

	glue_cap_remove(c_cspace, ops_container->my_ref);

#ifndef CONFIG_LVD
	validate_hidden_args = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(
			ops_container->rtnl_link_ops.validate);
	kfree(validate_hidden_args->t_handle);
	kfree(validate_hidden_args);
#endif
	kfree(ops_container);

fail_lookup:
	return ret;
}

//DONE
int alloc_netdev_mqs_callee(struct fipc_message *request)
{
	int sizeof_priv;
	int ret = 0;
	char *name;
	char name_assign_type;
	struct setup_container *temp;
	int txqs;
	int rxqs;
	struct net_device_container *dev_container;
	struct net_device *net_device;
#ifndef CONFIG_LVD
	struct trampoline_hidden_args *setup_hidden_args;
#endif
	cptr_t netdev_otherref;

	temp = kzalloc(sizeof( struct setup_container   ), GFP_KERNEL);
	if (!temp) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	sizeof_priv = fipc_get_reg1(request);
	name = "dummy%d";
	name_assign_type = fipc_get_reg3(request);
	txqs = fipc_get_reg4(request);
	rxqs = fipc_get_reg5(request);
	netdev_otherref.cptr = fipc_get_reg6(request);
	temp->other_ref.cptr = fipc_get_reg2(request);

	printk("received setup_container cptr: other_ref %lu", temp->other_ref.cptr);
	printk(" | netdev other ref cptr: %lu\n", netdev_otherref.cptr);

	ret = glue_cap_insert_setup_type(c_cspace, temp, &temp->my_ref);
	if (ret) {
		LIBLCD_ERR("insert");
		goto fail_insert;
	}

#ifndef CONFIG_LVD
	setup_hidden_args = kzalloc(sizeof( *setup_hidden_args ), GFP_KERNEL);
	if (!setup_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc;
	}
	setup_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(setup_trampoline);
	if (!setup_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_alloc;
	}
	setup_hidden_args->t_handle->hidden_args = setup_hidden_args;
	setup_hidden_args->struct_container = temp;
	setup_hidden_args->cspace = c_cspace;
	setup_hidden_args->sync_ep = sync_ep;

	temp->setup = LCD_HANDLE_TO_TRAMPOLINE(setup_hidden_args->t_handle);
	ret = set_memory_x(((unsigned long)setup_hidden_args->t_handle) & PAGE_MASK,
			ALIGN(LCD_TRAMPOLINE_SIZE(setup_trampoline),
				PAGE_SIZE) >> PAGE_SHIFT);
	if (ret) {
		LIBLCD_ERR("set mem nx");
		goto fail3;
	}
#endif
	net_device = alloc_netdev_mqs_lcd(sizeof_priv, name,
			name_assign_type,
			g_ops_container->rtnl_link_ops.setup,
			txqs, rxqs, netdev_otherref.cptr);

	dev_container = container_of(net_device, struct net_device_container, net_device);
	
	printk("%s: returned\n", __func__);
//	dev_container->other_ref.cptr = netdev_otherref.cptr;

	/*ret = glue_cap_insert_net_device_type(c_cspace, dev_container , &dev_container->my_ref);

	if (!ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}*/

//	fipc_set_reg5(response, dev_container->my_ref.cptr);
	return ret;
fail_insert:
#ifndef CONFIG_LVD
fail3:
#endif
fail_alloc:
	return ret;
}
