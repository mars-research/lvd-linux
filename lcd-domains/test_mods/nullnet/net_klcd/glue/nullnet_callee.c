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

struct trampoline_hidden_args {
	void *struct_container;
	struct glue_cspace *cspace;
	struct lcd_trampoline_handle *t_handle;
	struct thc_channel *async_chnl;
	struct cptr sync_ep;
};

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

/* XXX: How to determine this? */
#define CPTR_HASH_BITS      8
static DEFINE_HASHTABLE(cptr_table, CPTR_HASH_BITS);

#define TID_HASH_BITS      8
static DEFINE_HASHTABLE(tid_table, TID_HASH_BITS);

struct rtnl_link_stats64 pkt_stats;
static struct glue_cspace *c_cspace;
struct thc_channel *net_async;
struct cptr sync_ep;
extern struct cspace *klcd_cspace;
struct rtnl_link_ops_container *g_ops_container;
DEFINE_MUTEX(hash_lock);
DEFINE_SPINLOCK(hash_slock);
static unsigned long pool_pfn_start, pool_pfn_end;
priv_pool_t *pool;
priv_pool_t *skbc_pool;
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
struct thc_channel *xmit_chnl;
#define NUM_CORES	32
#define NUM_THREADS	NUM_CORES

uint64_t *times_ndo_xmit[4] = {0};
u32 thread = 0;
struct ptstate_t *ptrs[NUM_THREADS] = {0};
struct rtnl_link_stats64 g_stats;

int ndo_start_xmit(struct sk_buff *skb, struct net_device *dev,
			struct trampoline_hidden_args *hidden_args);

int ndo_start_xmit_clean(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args);

int ndo_start_xmit_dummy(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args);

int ndo_start_xmit_single_nohashing(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args);

int ndo_start_xmit_bare(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args);

int ndo_start_xmit_bare2(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args);

int ndo_start_xmit_bare_async(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args);

void skb_data_pool_init(void)
{
	pool = priv_pool_init(SKB_DATA_POOL, 10, SKB_DATA_SIZE);
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

//	hash_init(cptr_table);
	hash_init(tid_table);

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

int inline glue_insert_skbuff(struct hlist_head *htable,
			struct sk_buff_container *skb_c)
{
	BUG_ON(!skb_c->skb);

	skb_c->my_ref = __cptr((unsigned long)skb_c->skb);
#if 0
	mutex_lock_interruptible(&hash_lock);
	hash_add_rcu(cptr_table, &skb_c->hentry,
			(unsigned long) skb_c->skb);
	mutex_unlock(&hash_lock);
#endif
	spin_lock(&hash_slock);
	hash_add(cptr_table, &skb_c->hentry,
			(unsigned long) skb_c->skb);
	spin_unlock(&hash_slock);
	return 0;
}

int inline glue_lookup_skbuff(struct hlist_head *htable, struct cptr c, struct sk_buff_container **skb_cout)
{
        struct sk_buff_container *skb_c;

	spin_lock(&hash_slock);
        //hash_for_each_possible_rcu(cptr_table, skb_c, hentry, (unsigned long) cptr_val(c)) {
        hash_for_each_possible(cptr_table, skb_c, hentry, (unsigned long) cptr_val(c)) {
		if (!skb_c) {
			WARN_ON(!skb_c);
			continue;
		}
		if (skb_c->skb == (struct sk_buff*) c.cptr) {
	                *skb_cout = skb_c;
		}
        }
	spin_unlock(&hash_slock);
        return 0;
}

void inline glue_remove_skbuff(struct sk_buff_container *skb_c)
{
#if 0
	mutex_lock_interruptible(&hash_lock);
	hash_del_rcu(&skb_c->hentry);
	mutex_unlock(&hash_lock);
#endif
	spin_lock(&hash_slock);
	hash_del(&skb_c->hentry);
	spin_unlock(&hash_slock);
}

int inline glue_insert_skbuff_2(struct ptstate_t *pts,
			struct sk_buff_container *skb_c)
{
	BUG_ON(!skb_c->skb);

	skb_c->my_ref = __cptr((unsigned long)skb_c->skb);

	spin_lock(&pts->hash_lock);
	hash_add(pts->cptr_table, &skb_c->hentry,
			(unsigned long) skb_c->skb);
	spin_unlock(&pts->hash_lock);
	return 0;
}

int inline glue_lookup_skbuff_2(struct ptstate_t *pts,
			struct cptr c,
			struct sk_buff_container **skb_cout)
{
        struct sk_buff_container *skb_c;

	spin_lock(&pts->hash_lock);
        //hash_for_each_possible_rcu(cptr_table, skb_c, hentry, (unsigned long) cptr_val(c)) {
        hash_for_each_possible(pts->cptr_table, skb_c, hentry, (unsigned long) cptr_val(c)) {
		if (!skb_c) {
			WARN_ON(!skb_c);
			continue;
		}
		if (skb_c->skb == (struct sk_buff*) c.cptr) {
	                *skb_cout = skb_c;
		}
        }
	spin_unlock(&pts->hash_lock);
        return 0;
}

void inline glue_remove_skbuff_2(struct ptstate_t *pts,
		struct sk_buff_container *skb_c)
{
	spin_lock(&pts->hash_lock);
	hash_del(&skb_c->hentry);
	spin_unlock(&pts->hash_lock);
}


int inline glue_insert_tid(struct hlist_head *htable,
			struct ptstate_t *pts)
{
//	spin_lock(&hash_tid_lock);
	hash_add_rcu(tid_table, &pts->hentry,
			(unsigned long) pts->pid);
//	spin_unlock(&hash_tid_lock);
	return 0;
}

int inline glue_lookup_tid(struct hlist_head *htable, unsigned long pid, struct ptstate_t **pts_out)
{
        struct ptstate_t *ptstate;

//	spin_lock(&hash_tid_lock);
        hash_for_each_possible_rcu(tid_table, ptstate, hentry, pid) {
		if (!ptstate) {
			WARN_ON(!ptstate);
			continue;
		}
		if (ptstate->pid == pid) {
	                *pts_out = ptstate;
		}
        }
//`	spin_unlock(&hash_tid_lock);
        return 0;
}

void inline glue_remove_tid(struct ptstate_t *pts)
{
//	spin_lock(&hash_tid_lock);
	hash_del_rcu(&pts->hentry);
//	spin_unlock(&hash_tid_lock);
}


int grant_sync_ep(cptr_t *sync_end, cptr_t ha_sync_ep)
{
	int ret;
	struct cspace *curr_cspace = get_current_cspace(current);
	lcd_cptr_alloc(sync_end);
	ret = cap_grant(klcd_cspace, ha_sync_ep,
			curr_cspace, *sync_end);
	PTS()->syncep_present = true;
	PTS()->sync_ep = sync_end->cptr;
	return ret;
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

static void destroy_async_fs_ring_channel(struct thc_channel *chnl)
{
	cptr_t tx, rx;
	gva_t tx_gva, rx_gva;
	unsigned long unused1, unused2;
	int ret;
	unsigned int pg_order = PMFS_ASYNC_RPC_BUFFER_ORDER - PAGE_SHIFT;
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
	 * Free the thc channel
	 *
	 * XXX: We are assuming this is called *from the dispatch loop*
	 * (i.e., as part of handling a callee function), so no one
	 * else (no other awe) is going to try to use the channel
	 * after we kill it. (For the PMFS LCD, this is not possible,
	 * because the unregister happens from a *caller context*.)
	 */
	kfree(chnl);

	return;

fail2:
fail1:
	return;
}

static int setup_async_net_ring_channel(cptr_t tx, cptr_t rx, 
				struct thc_channel **chnl_out)
{
	gva_t tx_gva, rx_gva;
	int ret;
	struct fipc_ring_channel *fchnl;
	struct thc_channel *chnl;
	unsigned int pg_order = PMFS_ASYNC_RPC_BUFFER_ORDER - PAGE_SHIFT;
	/*
	 * Map tx and rx buffers (caller has already prep'd buffers)
	 */
	ret = lcd_map_virt(tx, pg_order, &tx_gva);
	if (ret) {
		LIBLCD_ERR("failed to map tx");
		goto fail1;
	}
	ret = lcd_map_virt(rx, pg_order, &rx_gva);
	if (ret) {
		LIBLCD_ERR("failed to map rx");
		goto fail2;
	}
	/*
	 * Alloc and init channel header
	 */
	fchnl = kmalloc(sizeof(*fchnl), GFP_KERNEL);
	if (!fchnl) {
		ret = -ENOMEM;
		LIBLCD_ERR("malloc failed");
		goto fail3;
	}
	ret = fipc_ring_channel_init(fchnl,
				PMFS_ASYNC_RPC_BUFFER_ORDER,
				/* (note: gva == hva for non-isolated) */
				(void *)gva_val(tx_gva),
				(void *)gva_val(rx_gva));
	if (ret) {
		LIBLCD_ERR("channel init failed");
		goto fail4;
	}
	/*
	 * Add to async channel group
	 */
	chnl = kzalloc(sizeof(*chnl), GFP_KERNEL);
	if (!chnl) {
		ret = -ENOMEM;
		LIBLCD_ERR("malloc failed");
		goto fail5;
	}
	ret = thc_channel_init(chnl, fchnl);
	if (ret) {
		LIBLCD_ERR("async group item init failed");
		goto fail6;
	}

	*chnl_out = chnl;
	return 0;

fail6:
	kfree(chnl);
fail5:
fail4:
	kfree(fchnl);
fail3:
	lcd_unmap_virt(rx_gva, pg_order);
fail2:
	lcd_unmap_virt(tx_gva, pg_order);
fail1:
	return ret;
}

int ndo_init_user(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *net_dev_container;

	thc_init();

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(request, NDO_INIT);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	DO_FINISH_(ndo_init, {
		ASYNC_( {
		ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
		}, ndo_init);
	});

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);

fail_ipc:
fail_async:
	lcd_exit(0);
	return ret;
}


int ndo_init(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *net_dev_container;

	if (!get_current()->ptstate) {
		LIBLCD_MSG("%s:Called from userland - can't process", __func__);
		LCD_MAIN({
			ret = ndo_init_user(dev, hidden_args);
		});
		return ret;
	}

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(request, NDO_INIT);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);
fail_ipc:
fail_async:
	return ret;
}


LCD_TRAMPOLINE_DATA(ndo_init_trampoline);
int LCD_TRAMPOLINE_LINKAGE(ndo_init_trampoline) ndo_init_trampoline(struct net_device *dev)
{
	int ( *volatile ndo_init_fp )(struct net_device *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_init_trampoline);
	ndo_init_fp = ndo_init;
	return ndo_init_fp(dev, hidden_args);

}

void ndo_uninit_user(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *net_dev_container;

	thc_init();

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, NDO_UNINIT);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	DO_FINISH_(ndo_uninit, {
		ASYNC_( {
		ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
		}, ndo_uninit);
	});

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);
fail_ipc:
fail_async:
	lcd_exit(0);
	return;
}

void ndo_uninit(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *net_dev_container;

	if (!get_current()->ptstate) {
		LIBLCD_MSG("%s:Called from userland - can't process", __func__);
		LCD_MAIN({
			ndo_uninit_user(dev, hidden_args);
		});
		return;	
	}

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, NDO_UNINIT);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);
fail_ipc:
fail_async:
	return;
}

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

#if 0
int ndo_start_xmit_user(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args, xmit_type_t xmit_type)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	struct net_device_container *net_dev_container;
	unsigned int request_cookie;
	cptr_t sync_end;
	unsigned long skb_ord, skb_off;
	unsigned long skbd_ord, skbd_off;
	struct sk_buff_container *skb_c;
	struct skbuff_members *skb_lcd;
	cptr_t skb_cptr, skbd_cptr;

	net_dev_container = container_of(dev,
			struct net_device_container, net_device);

	skb_c = kzalloc(sizeof(*skb_c), GFP_KERNEL);

	if (!skb_c)
		LIBLCD_MSG("no memory");
	skb_c->skb = skb;
	glue_insert_skbuff(cptr_table, skb_c);

	/* save original head, data */
	skb_c->head = skb->head;
	skb_c->data = skb->data;
	skb_c->skb_ord = skb_ord;

	/* enter LCD mode to have cspace tree */
	lcd_enter();

	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(_request,
			NDO_START_XMIT);

	fipc_set_reg0(_request, xmit_type);

	fipc_set_reg1(_request,
			net_dev_container->other_ref.cptr);

	fipc_set_reg2(_request,
			skb_c->my_ref.cptr);

	switch (xmit_type) {
	case VOLUNTEER_XMIT:

		ret = thc_ipc_send_request(hidden_args->async_chnl,
			_request, &request_cookie);

		if (ret) {
			LIBLCD_ERR("thc_ipc_call");
			goto fail_ipc;
		}

		ret = grant_sync_ep(&sync_end, hidden_args->sync_ep);

		ret = sync_setup_memory(skb, sizeof(struct sk_buff),
				&skb_ord, &skb_cptr, &skb_off);

		ret = sync_setup_memory(skb->head,
			skb_end_offset(skb)
			+ sizeof(struct skb_shared_info),
			&skbd_ord, &skbd_cptr, &skbd_off);

		skb_c->skb_cptr = skb_cptr;
		skb_c->skbh_cptr = skbd_cptr;

		//sync half
		lcd_set_cr0(skb_cptr);
		lcd_set_cr1(skbd_cptr);
		lcd_set_r0(skb_ord);
		lcd_set_r1(skb_off);
		lcd_set_r2(skbd_ord);
		lcd_set_r3(skbd_off);
		lcd_set_r4(skb->data - skb->head);

		ret = lcd_sync_send(sync_end);
		lcd_set_cr0(CAP_CPTR_NULL);
		lcd_set_cr1(CAP_CPTR_NULL);
		if (ret) {
			LIBLCD_ERR("failed to send");
			goto fail_sync;
		}

		lcd_cap_delete(sync_end);

		break;

	case SHARED_DATA_XMIT:
		WARN_ON(PTS()->user_ctx);
		PTS()->user_ctx = true;
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


		ret = thc_ipc_send_request(hidden_args->async_chnl,
			_request, &request_cookie);

		if (ret) {
			LIBLCD_ERR("thc_ipc_call");
			goto fail_ipc;
		}
		break;

	default:
		LIBLCD_ERR("%s, Unknown xmit_type requested",
			__func__);
		break;
	}


	DO_FINISH_(ndo_start_xmit, {
	  ASYNC_({
	    ret = thc_ipc_recv_response(hidden_args->async_chnl,
			request_cookie, &_response);
	  }, ndo_start_xmit);
	});

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), _response);

fail_async:
fail_sync:
fail_ipc:
	if (!PTS()->user_ctx)
		lcd_exit(0);
	return ret;
}
#endif

#define fipc_test_pause()    asm volatile ( "pause\n": : :"memory" );

/**
 * This function will block until a message is received and stored in out.
 */
static inline
int fipc_test_blocking_recv_start_cnt ( struct thc_channel *chnl, struct fipc_message** out, u64 *count )
{
	int ret;

	while ( 1 )
	{
		(*count)++;
		// Poll until we get a message or error
		ret = fipc_recv_msg_start( thc_channel_to_fipc(chnl),
						out );

		if ( !ret || ret != -EWOULDBLOCK )
		{
			return ret;
		}
		
		fipc_test_pause();
		asm volatile("xchg %rax, %rax");
//		asm volatile("xchg %rax, %rax");
	}

	return 0;
}

static inline
int fipc_test_blocking_recv_start ( struct thc_channel *chnl, struct fipc_message** out)
{
	int ret;

	while ( 1 )
	{
#ifdef ONE_SLOT
		// Poll until we get a message or error
		ret = fipc_recv_msg_start_0( thc_channel_to_fipc(chnl),
						out );
#else
		// Poll until we get a message or error
		ret = fipc_recv_msg_start( thc_channel_to_fipc(chnl),
						out );
#endif
		if ( !ret || ret != -EWOULDBLOCK )
		{
			return ret;
		}
		
		fipc_test_pause();
	}

	return 0;
}


/**
 * This function will block until a message slot is available and stored in out.
 */
static inline
int fipc_test_blocking_send_start ( struct thc_channel * chnl, struct fipc_message** out )
{
	int ret;

	while ( 1 )
	{
		// Poll until we get a free slot or error
		ret = fipc_send_msg_start( thc_channel_to_fipc(chnl), out );

		if ( !ret || ret != -EWOULDBLOCK )
		{
			return ret;
		}

		fipc_test_pause();
	}

	return 0;
}

struct thc_channel *sirq_channels[4];

int prep_channel(struct trampoline_hidden_args *hidden_args)
{
	cptr_t tx, rx;
//	cptr_t tx_softirq, rx_softirq;
	cptr_t sync_end;
	struct thc_channel *chnl;
//	struct thc_channel *chnl_softirq;
	struct fipc_message *_request;
	struct fipc_message *_response;
	unsigned int request_cookie;
	int ret;

	ret = lcd_cptr_alloc(&tx);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail_cptr;
	}
	ret = lcd_cptr_alloc(&rx);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail_cptr;
	}
#if 0
	ret = lcd_cptr_alloc(&tx_softirq);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail_cptr;
	}
	ret = lcd_cptr_alloc(&rx_softirq);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail_cptr;
	}
#endif


	/* grant sync_ep */
	if (grant_sync_ep(&sync_end, hidden_args->sync_ep)) {
		LIBLCD_ERR("%s, grant_syncep failed %d\n",
				__func__, ret);
		goto fail_ep;
	}

	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(_request,
			PREP_CHANNEL);
/*
	printk("%s:%d awe_map used slots\n&pts:%p, &awe:%p | aweget:%p"
			"&usedslots:%p val: %u\n", current->comm,
			current->pid, current->ptstate,
			&current->ptstate->awe_map,
			get_awe_map(),
			&current->ptstate->awe_map->used_slots,
			current->ptstate->awe_map->used_slots);
*/
//	tracing_on();
//	testhrarr_init();
//	tracing_off();
//	testhrarr_exit();
	//bpevent = install_hwbp((void*) &current->ptstate->awe_map, memwrite_handler, HW_BREAKPOINT_LEN_8, HW_BREAKPOINT_RW);

	//*ptr = 0xdead;
	/* No need to wait for a response here */
	ret = thc_ipc_send_request(hidden_args->async_chnl,
			_request,
			&request_cookie);

	//if (!IS_ERR(bpevent))
	//	uninstall_hwbp(bpevent);

	if (ret) {
		LIBLCD_ERR("thc_ipc send");
		goto fail_ipc;
	}

	lcd_set_cr0(rx);
	lcd_set_cr1(tx);
//	lcd_set_cr2(rx_softirq);
//	lcd_set_cr3(tx_softirq);

	ret = lcd_sync_recv(sync_end);
	if (ret) {
		if (ret == -EWOULDBLOCK)
			ret = 0;
		goto fail_ep;
	}
	/*
	 * Set up async ring channel
	 */
	ret = setup_async_net_ring_channel(tx, rx, &chnl);
	if (ret) {
		LIBLCD_ERR("error setting up ring channel");
		goto fail_ep;
	}

#if 0
	printk("[%d]%s[pid: %d]: Creating a pair for softirq\n", smp_processor_id(),
					current->comm,
					current->pid);
	/*
	 * Set up async ring channel for softirq
	 */
	ret = setup_async_net_ring_channel(tx_softirq, rx_softirq,
				&chnl_softirq);
	if (ret) {
		LIBLCD_ERR("error setting up ring channel");
		goto fail_ep2;
	}
#endif
	lcd_set_cr0(CAP_CPTR_NULL);
	lcd_set_cr1(CAP_CPTR_NULL);
//	lcd_set_cr2(CAP_CPTR_NULL);
//	lcd_set_cr3(CAP_CPTR_NULL);


	PTS()->thc_chnl = chnl;
//	sirq_channels[smp_processor_id()] = chnl_softirq;

//fail_ep2:
	/* technically, we do not need to receive the response */
	DO_FINISH_(prep_channel, {
	  ASYNC_({
	    ret = thc_ipc_recv_response(hidden_args->async_chnl,
			request_cookie, &_response);
	  }, prep_channel);
	});

	printk("%s, ipc recv resp %d | pts %p | reqc 0x%x\n",
				__func__, ret, PTS(),
				request_cookie);

	if (ret) {
		LIBLCD_ERR("thc_ipc_recv_response");
		goto fail_ipc;
	}

	fipc_recv_msg_end(thc_channel_to_fipc(
			hidden_args->async_chnl),_response);

	return 0;
fail_async:
fail_ep:
	lcd_cptr_free(rx);
	lcd_cptr_free(tx);
fail_ipc:
fail_cptr:
	return -1;
}

int __cptr_alloc(void)
{
	struct cptr_cache *cache;
	int ret;
	cptr_t unused;
	/*
	 * Set up our cptr cache
	 */
	ret = cptr_cache_alloc(&cache);
	if (ret) {
		LIBLCD_ERR("cptr cache alloc");
		goto fail2;
	}
	current->cptr_cache = cache;
	ret = cptr_cache_init(cache);
	if (ret) {
		LIBLCD_ERR("cptr cache init");
		goto fail3;
	}
	/*
	 * Reserve first two slots for call/reply caps (just alloc them)
	 */
	ret = cptr_alloc(cache, &unused);
	if (ret) {
		LIBLCD_ERR("cptr cache alloc1");
		goto fail4;
	}
	ret = cptr_alloc(cache, &unused);
	if (ret) {
		LIBLCD_ERR("cptr cache alloc2");
		goto fail5;
	}
fail2:
fail3:
fail4:
fail5:
	return ret;
}

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
			//PTS()->thc_chnl = xmit_chnl2;
		}
		PTS()->thc_chnl =
			sirq_channels[smp_processor_id()];
	} else if(!strncmp(current->comm, "iperf",
				strlen("iperf")) ||
		!strncmp(current->comm, "netperf",
				strlen("netperf"))) {

		prep_channel(hidden_args);
		printk("===================================\n");
		printk("===== Private Channel created =====\n");
		printk("===================================\n");
		if (thread < 4) {
			current->ptstate->times_ndo_xmit = times_ndo_xmit[thread++];
			printk("Assigning %p to pid %d\n", times_ndo_xmit[thread-1], current->pid);
		}
#ifndef NO_HASHING
#ifdef DOUBLE_HASHING
		spin_lock_init(&current->ptstate->hash_lock);
		hash_init(current->ptstate->cptr_table);

		// Add it once per thread
		glue_insert_tid(tid_table, current->ptstate);
		printk("Adding pts: %p and pid: %llu to the tid_table\n", current->ptstate, current->ptstate->pid);
#endif
#endif /* NO_HASHING */
	} else {
		printk("===== app %s , giving xmit_chnl\n",
				current->comm);
		PTS()->thc_chnl = xmit_chnl;
		PTS()->dofin = true;
	}
	return 0;
}

int sender_dispatch(struct thc_channel *chnl, struct fipc_message *out, void *arg)
{
	/* we receive the skb pointer via arg, pass it to consume skb via reg 0 */
	fipc_set_reg0(out, (uint64_t) arg);
//	printk("%s, called\n", __func__);
	return dispatch_async_loop(chnl, out, c_cspace, sync_ep); 
}

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

//	printk("%s, sending ipc \n", __func__);
	ret = async_msg_blocking_send_start(
			async_chnl, &_request);
	if (unlikely(ret)) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(_request, NDO_START_XMIT);

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

	//printk("%s, ipc call\n", __func__);
	ret = thc_ipc_send_request(async_chnl, _request, &request_cookie);

//	ret = thc_ipc_recv_response(async_chnl, request_cookie, &_response);

#ifdef SENDER_DISPATCH_LOOP

//	printk("%s, server dispatch %p\n", __func__, skb);
	ret = thc_ipc_recv_req_resp(async_chnl, &_response, request_cookie, sender_dispatch, (void*)skb);

	fipc_recv_msg_end(thc_channel_to_fipc(async_chnl), _response);

//	printk("%s, got response\n", __func__);
#else
	ret = thc_ipc_recv_response_inline(async_chnl, request_cookie,
				&_response);

	awe_mapper_remove_id(request_cookie);
#endif
	//printk("%s, ipc call returned %d\n", __func__, ret);
	if (unlikely(ret)) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	ret = fipc_get_reg1(_response);

	fipc_recv_msg_end(thc_channel_to_fipc(
			async_chnl), _response);

free:
#ifdef NO_HASHING
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
//	u64 s1, s2, s3;
//	static u32 iter = 0;

	if (!skb->next) {
		skb->chain_skb = false;
	}

	if (!skb->chain_skb) {
		return ndo_start_xmit_bare_async(skb, dev, hidden_args);
	} else {
		/* chain skb */
	    if (!current->ptstate) {
		/* step 1. create lcd env */
		/* LCD environment already created */
		/* thc_init won't get executed, the rest will */
		printk("%s, %s:%d lcdenter\n", __func__, current->comm, current->pid);

		if (!strncmp(current->comm, "klcd", strlen("klcd")))
			return NETDEV_TX_OK;

		lcd_enter();

		ptrs[smp_processor_id()] = PTS();

		/* set nonlcd ctx for future use */
		PTS()->nonlcd_ctx = true;

		//PTS()->times_ndo_xmit = vzalloc(NUM_PACKETS * sizeof(uint64_t));
		PTS()->pid = current->pid;

		PTS()->inited = true;
		/*
		 * if it is ksoftirqd, let it use the channel that exist 
		 */
		if (!strncmp(current->comm, "ksoftirqd/",
					strlen("ksoftirqd/"))) {
			if (!sirq_channels[smp_processor_id()]) {
				printk("%s: sirqch empty for %d\n",
					__func__, smp_processor_id());
				//PTS()->thc_chnl = xmit_chnl2;
			}
			PTS()->thc_chnl =
				sirq_channels[smp_processor_id()];
		} else if(!strncmp(current->comm, "iperf",
					strlen("iperf")) ||
			!strncmp(current->comm, "netperf",
					strlen("netperf"))) {

			if (!PTS()->awe_map) {
				printk("%s:%d Awemap not initialized %p\n",
					current->comm, current->pid,
					current->ptstate->awe_map);
			}

			prep_channel(hidden_args);
			printk("===================================\n");
			printk("===== Private Channel created =====\n");
			printk("===================================\n");
			if (thread < 4) {
				current->ptstate->times_ndo_xmit = times_ndo_xmit[thread++];
				printk("Assigning %p to pid %d\n", times_ndo_xmit[thread-1], current->pid);
			}
#ifndef NO_HASHING
			spin_lock_init(&current->ptstate->hash_lock);
			hash_init(current->ptstate->cptr_table);

			// Add it once per thread
			glue_insert_tid(tid_table, current->ptstate);
			printk("Adding pts: %p and pid: %llu to the tid_table\n", current->ptstate, current->ptstate->pid);
#endif /* NO_HASHING */
		} else {
			printk("===== app %s , giving xmit_chnl\n",
					current->comm);
			PTS()->thc_chnl = xmit_chnl;
			PTS()->dofin = true;
		}

	    }
	}
	//LCD_MAIN( {
		DO_FINISH_(dev_hard_xmit, {
		//	s1 = rdtsc();
			while (skb) {
				struct sk_buff *next = skb->next;
			
				//printk("%s, before async next: %p\n", __func__,
				//				next);
				skb->next = NULL;
				ASYNC_( {
				//	printk("%s, sending skb %p\n", __func__, skb);
					skb->chain_skb = true;
					rc = ndo_start_xmit_async(skb, dev, hidden_args);
					if (unlikely(!dev_xmit_complete(rc))) {
						skb->next = next;
						printk("%s, xmit failed\n", __func__);
						// continue for now
						continue;
					}
					
				}, before_xmit); // ASYNC

				//printk("%s, after async next: %p\n", __func__,
				//				next);
				skb = next;
/*				if (netif_xmit_stopped(txq) && skb) {
					rc = NETDEV_TX_BUSY;
					break;
				}
*/
			}
			//s2 = rdtsc();
		}
		); // DO_FIN
//	} );
#if 0
	s3 = rdtsc();

	times_dofin[iter] = s2 - s1;
	times_total[iter] = s3 - s2;

	if (iter++ == NUM_PACKETS) {
		printk("---> times dofin <-----\n");
		fipc_test_stat_print_info(times_dofin, NUM_PACKETS);

		printk("---> times total <-----\n");
		fipc_test_stat_print_info(times_total, NUM_PACKETS);
		iter = 0;
		memset(times_dofin, 0, sizeof(times_dofin));
		memset(times_total, 0, sizeof(times_total));
	}
#endif
//	printk("%s, dofin complete\n", __func__);
	return rc;
}

#define NUM_TRANSACTIONS	1000000

int ndo_start_xmit_async2(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	struct thc_channel *async_chnl;
#ifdef COPY
	struct skbuff_members *skb_lcd;
#endif
	unsigned int request_cookie;
#ifdef MARSHAL
	struct net_device_container *net_dev_container;
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
#endif
	async_chnl = (struct thc_channel*) PTS()->thc_chnl;

#ifdef MARSHAL
	net_dev_container = container_of(dev,
			struct net_device_container, net_device);
#endif
//	printk("%s, sending ipc \n", __func__);
	ret = async_msg_blocking_send_start(
			async_chnl, &_request);
	if (unlikely(ret)) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(_request, NDO_START_XMIT);
#ifndef NO_ASYNC
	fipc_set_reg0(_request, 1);
#endif
#ifdef MARSHAL
	fipc_set_reg1(_request,
			net_dev_container->other_ref.cptr);
	fipc_set_reg2(_request, 0x0);

	fipc_set_reg3(_request,
			(unsigned long)
			((void*)skb->head - pool->pool));

	fipc_set_reg4(_request, skb->end);
	fipc_set_reg5(_request, skb->protocol);
	fipc_set_reg6(_request, skb->len);
#endif
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

#ifndef NO_ASYNC
	ret = thc_ipc_send_request(async_chnl, _request, &request_cookie);

	#ifdef OLD_RESPONSE
	  ret = thc_ipc_recv_response(async_chnl, request_cookie,
						&_response);
	#elif defined(SENDER_DISPATCH_LOOP)
	ret = thc_ipc_recv_req_resp(async_chnl, &_response, request_cookie, sender_dispatch, (void*)skb);

	  fipc_recv_msg_end(thc_channel_to_fipc(async_chnl), _response);
	#else
	  ret = thc_ipc_recv_response_inline(async_chnl, request_cookie,
				&_response);

	awe_mapper_remove_id(request_cookie);
	#endif /* OLD_RESPONSE */

#else /* NO_ASYNC */
	thc_set_msg_type(_request, msg_type_request);
	fipc_send_msg_end(thc_channel_to_fipc(
			async_chnl), _request);

	/* guard nonlcd case with all macros */
	fipc_test_blocking_recv_start(
			async_chnl,
			&_response);

	fipc_recv_msg_end(thc_channel_to_fipc(
			async_chnl), _response);

#endif
	//printk("%s, ipc call returned %d\n", __func__, ret);
	if (unlikely(ret)) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
#ifdef MARSHAL
	ret = fipc_get_reg1(_response);
#endif
	fipc_recv_msg_end(thc_channel_to_fipc(
			async_chnl), _response);

fail_async:
fail_ipc:
	return ret;
}

int NUM_INNER_ASYNCS = 1;

//#define TS
int ndo_start_xmit_bare_async(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
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

	_TS_START(xmit);
	for (i = 0; i < NUM_TRANSACTIONS; i++) {
		int j;
		DO_FINISH({
			for (j = 0; j < NUM_INNER_ASYNCS; j++) {
			ASYNC({
				ndo_start_xmit_async2(skb, dev, hidden_args);
			}); //async
		}
		}); // dofinish
	}
	_TS_STOP(xmit);

	printk("%s, %d ch3 transactions took %llu\n", __func__,
			NUM_TRANSACTIONS, _TS_DIFF(xmit)/NUM_TRANSACTIONS);

	if (0)
	{
	unsigned long num = 0, j;
	unsigned int ids[NUM_INNER_ASYNCS];
	_TS_START(xmit);
	for (i = 0; i < NUM_TRANSACTIONS; i++) {
	     DO_FINISH({
             for (j = 0; j < NUM_INNER_ASYNCS/2; j++) {
                 ASYNC({
                     awe_mapper_create_id(&ids[j]);
                     assert(ids[j]);
                     THCYieldAndSave(ids[j]);
                     awe_mapper_remove_id(ids[j]);
                     num++;
                 });
             }
             for (j = 0; j < NUM_INNER_ASYNCS/2; j++) {
                 ASYNC({
                     THCYieldToId(ids[j]);
                 });
             };
         });
	}
	_TS_STOP(xmit);

	printk("%s, %d ch3 1 yield and 1 yields back (via awe mapper) transactions took %llu\n", __func__,
			NUM_TRANSACTIONS, _TS_DIFF(xmit)/NUM_TRANSACTIONS);
	}
#ifdef CH2
	PTS()->thc_chnl = xmit_chnl;
	_TS_START(xmit);
	for (i = 0; i < NUM_TRANSACTIONS; i++) {
		int j;
		DO_FINISH({
		for (j = 0; j < num_inner_asyncs; j++) {
			ASYNC({
				ndo_start_xmit_async2(skb, dev, hidden_args);
			}); //async
		}
		}); // dofinish
	}
	_TS_STOP(xmit);

	printk("%s, %d ch2 transactions took %llu\n", __func__,
			NUM_TRANSACTIONS, _TS_DIFF(xmit)/NUM_TRANSACTIONS);
#endif
free:
#ifdef CONSUME_SKB_NO_HASHING
	dev_kfree_skb(skb);
#endif
#ifdef NO_HASHING
//	dev_kfree_skb(skb);
#endif
	return NETDEV_TX_OK;
}

//#define TS
int ndo_start_xmit_bare(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message *_request;
	struct fipc_message *_response;
	xmit_type_t xmit_type;
	struct thc_channel *async_chnl;
#ifdef TS
	u32 i;
	static bool prog = false;

	//_TS_DECL(xmit);
	u64 a,b;

	if (!prog) {
		for ( i = 0; i < ev_num; ++i )
			FILL_EVENT_OS(&ev[i], ev_idx[i], ev_msk[i]);
		prog = true;
	}
#endif

#ifdef MARSHAL
	struct net_device_container *net_dev_container;
	struct sk_buff_container static_skbc;
	struct sk_buff_container *skb_c = &static_skbc;

	net_dev_container = container_of(dev,
			struct net_device_container, net_device);

	skb_c->skb = skb;
#endif

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
		async_chnl = PTS()->thc_chnl;
	} else {
		async_chnl = PTS()->thc_chnl;
	}

#ifdef TS
	// Start counting
	for ( i = 0; i < ev_num; ++i )
		PROG_EVENT(&ev[i], i);

	//_TS_START(xmit);
	a = rdtsc();
#endif
	//for (i = 0; i < NUM_TRANSACTIONS; i++) {

	async_msg_blocking_send_start(
			async_chnl, &_request);

	async_msg_set_fn_type(_request, NDO_START_XMIT);

#ifdef MARSHAL
	fipc_set_reg0(_request, xmit_type);
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
#endif
	thc_set_msg_type(_request, msg_type_request);
	fipc_send_msg_end(thc_channel_to_fipc(
			async_chnl), _request);

	/* guard nonlcd case with all macros */
	fipc_test_blocking_recv_start(
			async_chnl,
			&_response);

	fipc_recv_msg_end(thc_channel_to_fipc(
			async_chnl), _response);

	//}
#ifdef TS
	//_TS_STOP(xmit);
	b = rdtsc();
	// Stop counting
	for ( i = 0; i < ev_num; ++i )
		STOP_EVENT(i);
	
	if (PTS()->iter++ == NUM_PACKETS) {
		PTS()->iter = 0;
		printk("-------------------------------------------------\n");
		fipc_test_stat_print_info(PTS()->times_ndo_xmit,
				NUM_PACKETS);
		memset(PTS()->times_ndo_xmit, 0x00, NUM_PACKETS);
	// Read count
	for ( i = 0; i < ev_num; ++i )
		READ_PMC(&ev_val[i], i);


	for ( i = 0; i < ev_num; ++i )
		printk("Event id:%2x   mask:%2x   count: %llu\n", ev_idx[i], ev_msk[i], ev_val[i]);
	
	printk("-------------------------------------------------\n");

	// Reset count
	for ( i = 0; i < ev_num; ++i ) {
		RESET_COUNTER(i);
		ev_val[i] = 0;
	}

	}
	PTS()->times_ndo_xmit[PTS()->iter]
			//= _TS_DIFF(xmit) - CORRECTION_VALUE;
			= b - a - CORRECTION_VALUE;
#endif
free:
#ifdef NO_HASHING
	dev_kfree_skb(skb);
#endif
	return NETDEV_TX_OK;
}
#if 0
int dispatch_request(struct thc_channel *channel, struct fipc_message *req)
{


}
#endif
int ndo_start_xmit_bare2(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message *_request;
	struct fipc_message *_request1;
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

	if (unlikely(!current->ptstate)) {
		if (setup_once(hidden_args))
			goto free;
	}

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

	/* guard nonlcd case with all macros */
	fipc_test_blocking_recv_start(
			async_chnl,
			&_response);

	fipc_recv_msg_end(thc_channel_to_fipc(
			async_chnl), _response);

	ret = fipc_get_reg1(_response);
free:
#ifdef NO_HASHING
	dev_kfree_skb(skb);
#endif
	return NETDEV_TX_OK;
}

/* This function does exactly what the native xmit does
 * this serves as our base. If we use this function for isolated xmit,
 * we should ideally get the same bandwidth as the native dummy driver
 */
int ndo_start_xmit_dummy(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}

int ndo_start_xmit_single_nohashing(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	xmit_type_t xmit_type;
	struct net_device_container *net_dev_container;

	struct sk_buff_container static_skbc;
	struct sk_buff_container *skb_c = &static_skbc;
	bool nonlcd_ctx;
	struct thc_channel *async_chnl = NULL;
	static bool ts = false;
	//struct ptstate_t *pts = NULL;

#ifdef NDO_XMIT_TS
	xmit_type_t xmit_type_2;
	static int iter = 0;
	_TS_DECL(xmit);
#endif
	xmit_type = check_skb_range(skb);

	if (unlikely((xmit_type == VOLUNTEER_XMIT))) {
		printk("%s, skb->proto %02X | len %d\n",
				__func__, ntohs(skb->protocol),
				skb->len);
		return NETDEV_TX_OK;
	}
	if (unlikely(!current->ptstate)) {
		if (setup_once(hidden_args))
			goto free;
		nonlcd_ctx = PTS()->nonlcd_ctx;
		ts = true;
	} else {
//		pts = PTS();
		nonlcd_ctx = PTS()->nonlcd_ctx;
	}
#ifdef NDO_XMIT_TS
	if (ts)
		_TS_START(xmit);

	for (iter = 0; iter < NUM_TRANSACTIONS; iter++)
	{
	xmit_type_2 = check_skb_range(skb);
	if (unlikely((xmit_type_2 == VOLUNTEER_XMIT))) {
		printk("%s, skb->proto %02X | len %d\n",
				__func__, ntohs(skb->protocol),
				skb->len);
		return NETDEV_TX_OK;
	}
	if (unlikely(!current->ptstate)) {
//		nonlcd_ctx = PTS()->nonlcd_ctx;
		ts = true;
	}
// else {
//		nonlcd_ctx = PTS()->nonlcd_ctx;
//	}

#endif
#if 0
	if (likely(nonlcd_ctx)) {
		async_chnl = (struct thc_channel*) PTS()->thc_chnl;
		if (unlikely(!async_chnl)) {
			printk("%s: async channel is null\n", current->comm);
			return NETDEV_TX_OK;
		}
	} else {
		async_chnl = xmit_chnl;
	}
#endif
	async_chnl = PTS()->thc_chnl;
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

#ifndef NO_MARSHAL
	fipc_set_reg0(_request, xmit_type);
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
#endif

	thc_set_msg_type(_request, msg_type_request);
	fipc_send_msg_end(thc_channel_to_fipc(
			async_chnl), _request);

	fipc_test_blocking_recv_start(
			async_chnl,
			&_response);

	ret = fipc_get_reg1(_response);

	fipc_recv_msg_end(thc_channel_to_fipc(
			async_chnl), _response);

#ifdef NDO_XMIT_TS
	}
	if (ts) {
		_TS_STOP(xmit);

	printk("%s, %d transactions took %llu\n", __func__,
			NUM_TRANSACTIONS, _TS_DIFF(xmit)/NUM_TRANSACTIONS); 
	}
#endif

#if 0
	if (ts) {
		_TS_STOP(xmit);
	if (PTS()->iter++ == NUM_PACKETS) {
		PTS()->iter = 0;
		fipc_test_stat_print_info(PTS()->times_ndo_xmit,
				NUM_PACKETS);
		memset(PTS()->times_ndo_xmit, 0x00, NUM_PACKETS);
	}
	PTS()->times_ndo_xmit[PTS()->iter]
			= _TS_DIFF(xmit) - CORRECTION_VALUE;
	}
#endif

free:
	dev_kfree_skb(skb);

fail_async:
	return ret;
}


int ndo_start_xmit_clean(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	struct net_device_container *net_dev_container;
	xmit_type_t xmit_type;
#ifdef COPY
	struct skbuff_members *skb_lcd;
#endif
	struct sk_buff_container static_skbc;
	struct sk_buff_container *skb_c = &static_skbc;
	bool nonlcd_ctx;
	struct thc_channel *async_chnl = NULL;
	static bool ts = false;

#ifdef NDO_XMIT_TS
	_TS_DECL(xmit);

#endif
	//int iter = 0;
	xmit_type = check_skb_range(skb);

	if (unlikely((xmit_type == VOLUNTEER_XMIT))) {
		printk("%s, skb->proto %02X | len %d\n",
				__func__, ntohs(skb->protocol),
				skb->len);
		return NETDEV_TX_OK;
	}

	if (unlikely(!current->ptstate)) {
		if (setup_once(hidden_args))
			return NETDEV_TX_OK;
		nonlcd_ctx = PTS()->nonlcd_ctx;
		ts = true;
	} else {
		nonlcd_ctx = PTS()->nonlcd_ctx;
	}

#ifdef NDO_XMIT_TS
	if (ts)
		_TS_START(xmit);
#endif
	if (likely(nonlcd_ctx)) {
		async_chnl = (struct thc_channel*) PTS()->thc_chnl;
		if (unlikely(!async_chnl)) {
			printk("%s: async channel is null\n", current->comm);
			return NETDEV_TX_OK;
		}
	} else {
		async_chnl = xmit_chnl;
	}

	net_dev_container = container_of(dev,
			struct net_device_container, net_device);

	//printk("%s:%d xmitting\n", current->comm, current->pid);

#ifdef SKBC_PRIVATE_POOL
//	skb_c = priv_alloc(SKB_CONTAINER_POOL);
#else
//	skb_c = kmem_cache_alloc(skb_c_cache, GFP_KERNEL);
#endif
/*
	if (!skb_c) {
		LIBLCD_MSG("no memory");
		goto fail_alloc;
	}
*/
	skb_c->skb = skb;

#if !defined(NO_HASHING)
	/* add it hash table only when we are releasing the skb from klcd context
	 * NO_HASHING means that the skb will be released in the calling context
	 */
	/* hash insert */
	glue_insert_skbuff(cptr_table, skb_c);
#endif

#ifdef NDO_XMIT_TS
//	_TS_START(xmit);
#endif
//	for (iter = 0; iter < NUM_TRANSACTIONS; iter++) {
	ret = async_msg_blocking_send_start(
			async_chnl, &_request);
	if (unlikely(ret)) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(_request, NDO_START_XMIT);

#ifndef NO_MARSHAL
	fipc_set_reg0(_request, xmit_type);
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
#endif

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

	thc_set_msg_type(_request, msg_type_request);
	fipc_send_msg_end(thc_channel_to_fipc(
			async_chnl), _request);

	fipc_test_blocking_recv_start(
			async_chnl,
			&_response);

	ret = fipc_get_reg1(_response);

	fipc_recv_msg_end(thc_channel_to_fipc(
			async_chnl), _response);

	//}

//	printk("%s, %d transactions took %llu\n", __func__,
//			NUM_TRANSACTIONS, _TS_DIFF(xmit)/NUM_TRANSACTIONS); 
	//printk("%s, got response\n", __func__);
#ifdef NO_HASHING
	dev_kfree_skb(skb);
#endif

#ifdef NDO_XMIT_TS
	if (ts)
	_TS_STOP(xmit);
#endif
#ifdef NDO_XMIT_TS
	if (ts) {
	if (PTS()->iter++ == NUM_PACKETS) {
		PTS()->iter = 0;
		fipc_test_stat_print_info(current->ptstate->times_ndo_xmit,
				NUM_PACKETS);
		memset(current->ptstate->times_ndo_xmit, 0x00, NUM_PACKETS);
	}
	current->ptstate->times_ndo_xmit[PTS()->iter]
			= _TS_DIFF(xmit) - CORRECTION_VALUE;
	}
#endif

fail_async:
//fail_alloc:
	return ret;
}

int ndo_start_xmit(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	struct net_device_container *net_dev_container;
	xmit_type_t xmit_type;
#ifdef COPY
	struct skbuff_members *skb_lcd;
#endif
	struct sk_buff_container *skb_c;
	unsigned long skb_ord, skb_off;
	unsigned long skbd_ord, skbd_off;
	unsigned int request_cookie;
	cptr_t skb_cptr, skbd_cptr;
	cptr_t sync_end;
	bool nonlcd_ctx;
	struct thc_channel *async_chnl = NULL;
	//unsigned long flags;
#ifdef INDIVIDUAL_MARKERS
	_TS_DECL(ipc_send);
	_TS_DECL(ipc_recv);
#endif
#ifdef LCD_MEASUREMENT
	static int iter = 0;
#endif
#ifdef TIMESTAMP
	_TS_DECL(ndo_xmit);
#ifdef DUMMY_RDTSC
	_TS_DECL(dummy);
#endif
	static int iter = 0;
	bool ts = false;
#endif
#ifdef NDO_XMIT_TS
	//_TS_DECL(ndo_xmit);
	u64 a = rdtsc();
	//_TS_START(ndo_xmit);
#endif
//#ifdef PERF_EVENTS
#if 0
	static int piter = 0;
	DECL_EVENT(e1); DECL_EVENT(e2);
	DECL_EVENT(e3); DECL_EVENT(e4);
	FILL_EVENT_OS(&e1, 0x26, 0x01);
	FILL_EVENT_OS(&e2, 0x26, 0x02);
	FILL_EVENT_OS(&e3, 0x26, 0x04);
	FILL_EVENT_OS(&e4, 0x26, 0x08);
#endif

	xmit_type = check_skb_range(skb);

	if (!current->ptstate) {

		printk("%s, %s:%d lcdenter\n", __func__,
				current->comm, current->pid);

		/* step 1. create lcd env */
		lcd_enter();

		
		if (!PTS()) {
			printk("%s, %s:%d unable to create pts\n", __func__,
				current->comm, current->pid);
			return NETDEV_TX_OK;
		}
			
		ptrs[smp_processor_id()] = PTS();

		/* set nonlcd ctx for future use */
		PTS()->nonlcd_ctx = nonlcd_ctx = true;

		//PTS()->times_ndo_xmit = vzalloc(NUM_PACKETS * sizeof(uint64_t));
		PTS()->pid = current->pid;

		/* step 2. grant sync_ep if needed */
		/* FIXME: always grant sync_ep? */
//		if (grant_sync_ep(&sync_end, hidden_args->sync_ep))
//			printk("%s, grant_syncep failed %d\n",
//					__func__, ret);
		/*
		 * if it is ksoftirqd, let it use the channel that exist 
		 */
		if (!strncmp(current->comm, "ksoftirqd/",
					strlen("ksoftirqd/"))) {
			if (!sirq_channels[smp_processor_id()]) {
				printk("%s: sirqch empty for %d\n",
					__func__, smp_processor_id());
				//PTS()->thc_chnl = xmit_chnl2;
			}
			PTS()->thc_chnl =
				sirq_channels[smp_processor_id()];
		} else if(!strncmp(current->comm, "iperf",
					strlen("iperf")) ||
			!strncmp(current->comm, "netperf",
					strlen("netperf"))) {
	
			prep_channel(hidden_args);
			printk("===================================\n");
			printk("===== Private Channel created =====\n");
			printk("===================================\n");
			if (thread < 4) {
				current->ptstate->times_ndo_xmit = times_ndo_xmit[thread++];
				printk("Assigning %p to pid %d\n", times_ndo_xmit[thread-1], current->pid);
			}
#ifndef NO_HASHING
			spin_lock_init(&current->ptstate->hash_lock);
			hash_init(current->ptstate->cptr_table);

			// Add it once per thread
			glue_insert_tid(tid_table, current->ptstate);
			printk("Adding pts: %p and pid: %llu to the tid_table\n", current->ptstate, current->ptstate->pid);
#endif /* NO_HASHING */
		} else {
			printk("===== app %s , giving xmit_chnl\n",
					current->comm);
			PTS()->thc_chnl = xmit_chnl;
			PTS()->dofin = true;
		}

	} else {
		nonlcd_ctx = PTS()->nonlcd_ctx;
	}

	if (xmit_type == VOLUNTEER_XMIT) {
		printk("%s, skb->proto %02X | len %d\n",
				__func__, ntohs(skb->protocol),
				skb->len);
		return NETDEV_TX_OK;
	}

	if (PTS()->nonlcd_ctx) {
		async_chnl = (struct thc_channel*) PTS()->thc_chnl;
		if (!async_chnl) {
			printk("%s: async channel is null\n", current->comm);
			return NETDEV_TX_OK;
		}
#if defined(TIMESTAMP)
		ts = true;
#endif 
	} else {
		async_chnl = xmit_chnl;
	}
#ifdef TIMESTAMP
	/* start time stamping */
	if (ts) {
	//	preempt_disable();
	//	raw_local_irq_save(flags);
	//	_TS_START(ndo_xmit);
#ifdef TS_SANITY_CHECK
		_TS_STOP(ndo_xmit);
		times_ndo_xmit[iter] = _TS_DIFF(ndo_xmit);
		iter = (iter + 1) % NUM_PACKETS;
#endif
	}
#endif

#if 0
//#ifdef PERF_EVENTS
	PROG_EVENT(&e1, EVENT_SEL0);
/*	PROG_EVENT(&e2, EVENT_SEL1);
	PROG_EVENT(&e3, EVENT_SEL2);
	PROG_EVENT(&e4, EVENT_SEL3);*/
#endif

	net_dev_container = container_of(dev,
			struct net_device_container, net_device);

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

#ifndef NO_HASHING
#ifdef DOUBLE_HASHING
	//_TS_START(ndo_xmit);
	glue_insert_skbuff_2(current->ptstate, skb_c);
	//_TS_STOP(ndo_xmit);
#else
	/* hash insert */
	glue_insert_skbuff(cptr_table, skb_c);
#endif
#endif /* NO_HASHING */

#ifdef INDIVIDUAL_MARKERS
	if (likely(ts)) {
		_TS_STOP(ndo_xmit);
		_TS_START(ipc_send);
	}
#endif

#ifdef ONE_SLOT
	ret = async_msg_blocking_send_start_0(
			async_chnl, &_request);
#else	
	ret = async_msg_blocking_send_start(
			async_chnl, &_request);
#endif
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(_request, NDO_START_XMIT);

	fipc_set_reg0(_request, xmit_type);
	fipc_set_reg1(_request,
			net_dev_container->other_ref.cptr);
	fipc_set_reg2(_request,
			skb_c->my_ref.cptr);

	switch (xmit_type) {
	case VOLUNTEER_XMIT:
		printk("$##\n");
		ret = thc_ipc_send_request(hidden_args->async_chnl,
				_request, &request_cookie);

		if (ret) {
			LIBLCD_ERR("thc_ipc_call");
			goto fail_ipc;
		}

		ret = sync_setup_memory(skb, sizeof(struct sk_buff),
				&skb_ord, &skb_cptr, &skb_off);

		ret = sync_setup_memory(skb->head,
			skb_end_offset(skb)
			+ sizeof(struct skb_shared_info),
				&skbd_ord, &skbd_cptr, &skbd_off);

		skb_c->skb_cptr = skb_cptr;
		skb_c->skbh_cptr = skbd_cptr;

		/* sync half */
		lcd_set_cr0(skb_cptr);
		lcd_set_cr1(skbd_cptr);
		lcd_set_r0(skb_ord);
		lcd_set_r1(skb_off);
		lcd_set_r2(skbd_ord);
		lcd_set_r3(skbd_off);
		lcd_set_r4(skb->data - skb->head);

		/* handle nonlcd case with a granted sync ep */
		if (PTS()->nonlcd_ctx && PTS()->syncep_present) {
			sync_end.cptr = PTS()->sync_ep;
			ret = lcd_sync_send(sync_end);
		} else
			ret = lcd_sync_send(hidden_args->sync_ep);

		lcd_set_cr0(CAP_CPTR_NULL);
		lcd_set_cr1(CAP_CPTR_NULL);

		if (ret) {
			LIBLCD_ERR("failed to send");
			goto fail_sync;
		}

		break;

	case SHARED_DATA_XMIT:
		fipc_set_reg3(_request,
				(unsigned long)
				((void*)skb->head - pool->pool));

		fipc_set_reg4(_request, skb->end);
		fipc_set_reg5(_request, skb->protocol);
#ifdef DOUBLE_HASHING
		fipc_set_reg6(_request,
			current->ptstate->pid << 32 | skb->len);
#else
		fipc_set_reg6(_request, skb->len);
#endif

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

#ifdef NO_AWE
		thc_set_msg_type(_request, msg_type_request);
		fipc_send_msg_end(thc_channel_to_fipc(
				async_chnl), _request);
//	raw_local_irq_restore(flags);
//	preempt_enable();

#else
		ret = thc_ipc_send_request(async_chnl,
			_request, &request_cookie);

		if (ret) {
			LIBLCD_ERR("thc_ipc_call");
			goto fail_ipc;
		}
#endif
#ifdef INDIVIDUAL_MARKERS
		_TS_STOP(ipc_send);
#endif
#ifdef DUMMY_RDTSC
		_TS_START(dummy);
		_TS_STOP(dummy);
#endif
#ifdef INDIVIDUAL_MARKERS
		_TS_START(ipc_recv);
#endif
		break;
	default:
		LIBLCD_ERR("%s, Unknown xmit_type requested",
			__func__);
		break;
	}

	/* guard nonlcd case with all macros */
	if (nonlcd_ctx) {
#ifdef NO_AWE
		fipc_test_blocking_recv_start(
				async_chnl,
				&_response);
#else
//		printk("dofinish\n");
		LCD_MAIN({
		DO_FINISH({
		ASYNC_({
		ret = thc_ipc_recv_response(
				async_chnl,
				request_cookie,
				&_response);
		}, ndo_xmit);
		});
		});
#endif
	} else {
		printk("Here?\n");
		ret = thc_ipc_recv_response(
				async_chnl,
				request_cookie,
				&_response);
	}

#ifndef NO_AWE
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
#endif
	ret = fipc_get_reg1(_response);

//	current->ptstate->times_ndo_xmit[PTS()->iter++%10000] = fipc_get_reg2(_response);

#ifdef LCD_MEASUREMENT
//	if (tdiff_valid)
//		times_con_skb[iter] = tdiff_disp;
	times_con_skb[iter] = fipc_get_reg2(_response);
//	times_ipc_send[iter] = fipc_get_reg3(_response);
//	times_ndo_xmit[iter] = fipc_get_reg4(_response);
	if (times_con_skb[iter])
		iter = (iter + 1) % NUM_PACKETS;
#endif
	fipc_recv_msg_end(thc_channel_to_fipc(
			async_chnl), _response);
#if 0
//#ifdef PERF_EVENTS
#if 1
/*	STOP_EVENT(EVENT_SEL3);
	STOP_EVENT(EVENT_SEL2);
	STOP_EVENT(EVENT_SEL1);*/
	STOP_EVENT(EVENT_SEL0);
#endif
	READ_PMC(&VAL(e1), EVENT_SEL0);
/*	READ_PMC(&VAL(e2), EVENT_SEL1);
	READ_PMC(&VAL(e3), EVENT_SEL2);
	READ_PMC(&VAL(e4), EVENT_SEL3);*/
	RESET_COUNTER(EVENT_SEL0);
/*	RESET_COUNTER(EVENT_SEL1);
	RESET_COUNTER(EVENT_SEL2);
	RESET_COUNTER(EVENT_SEL3);*/
	perf1[piter] = VAL(e1);
/*	perf2[piter] = VAL(e2);
	perf3[piter] = VAL(e3);
	perf4[piter] = VAL(e4);*/
	piter = (piter + 1) % NUM_PACKETS;
#endif

#ifndef TS_SANITY_CHECK
#ifdef TIMESTAMP
	if (likely(ts)) {
#ifndef INDIVIDUAL_MARKERS
		//_TS_STOP(ndo_xmit);
#else
		_TS_STOP(ipc_recv);
#endif
	}

	if (ts) {
#ifdef INDIVIDUAL_MARKERS
		times_ipc_recv[iter] = _TS_DIFF(ipc_recv);
		times_ipc_send[iter] = _TS_DIFF(ipc_send);
#endif
		times_ndo_xmit[iter] = _TS_DIFF(ndo_xmit);
		iter = (iter + 1) % NUM_PACKETS;
	}
#endif /* TIMESTAMP */
#endif /* TS_SANITY_CHECK */

#ifdef NO_HASHING
	dev_kfree_skb(skb);
#endif

#ifdef NDO_XMIT_TS
//	_TS_STOP(ndo_xmit);
	if (PTS()->iter++ == NUM_PACKETS) {
		PTS()->iter = 0;
		fipc_test_stat_print_info(current->ptstate->times_ndo_xmit,
				NUM_PACKETS);
		memset(current->ptstate->times_ndo_xmit, 0x00, NUM_PACKETS);
	}
	current->ptstate->times_ndo_xmit[PTS()->iter]
			= rdtsc() - a;
//			= _TS_DIFF(ndo_xmit);
#endif

fail_async:
fail_alloc:
fail_sync:
fail_ipc:
	return ret;
}

LCD_TRAMPOLINE_DATA(ndo_start_xmit_trampoline);
int LCD_TRAMPOLINE_LINKAGE(ndo_start_xmit_trampoline)
 ndo_start_xmit_trampoline(struct sk_buff *skb, struct net_device *dev)
{
	int ( *volatile ndo_start_xmit_fp )(struct sk_buff *, struct net_device *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_start_xmit_trampoline);
	//ndo_start_xmit_fp = ndo_start_xmit;
	ndo_start_xmit_fp = ndo_start_xmit_async_landing;
	return ndo_start_xmit_fp(skb, dev, hidden_args);

}

int ndo_validate_addr_user(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *net_dev_container;

	thc_init();

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, NDO_VALIDATE_ADDR);

	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	LIBLCD_MSG("%s, cptr klcd %lu", __func__, net_dev_container->other_ref.cptr);

	DO_FINISH_(ndo_validate_addr, {
	  ASYNC_({
	    ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
	  }, ndo_validate_addr);
	});

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);
fail_ipc:
fail_async:
	lcd_exit(0);
	return ret;
}

int ndo_validate_addr(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *net_dev_container;

	if (!get_current()->ptstate) {
		LIBLCD_MSG("%s:Called from userland %p", __func__, dev);
		LCD_MAIN({
			ret = ndo_validate_addr_user(dev, hidden_args);
		});
		printk("%s, returns to userspace \n", __func__);
		return ret;
	}

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, NDO_VALIDATE_ADDR);

	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);
fail_ipc:
fail_async:
	return ret;
}

LCD_TRAMPOLINE_DATA(ndo_validate_addr_trampoline);
int LCD_TRAMPOLINE_LINKAGE(ndo_validate_addr_trampoline) ndo_validate_addr_trampoline(struct net_device *dev)
{
	int ( *volatile ndo_validate_addr_fp )(struct net_device *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_validate_addr_trampoline);
	ndo_validate_addr_fp = ndo_validate_addr;
	return ndo_validate_addr_fp(dev, hidden_args);

}

void ndo_set_rx_mode_user(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *net_dev_container;

	thc_init();

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, NDO_SET_RX_MODE);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	printk("%s, before async \n", __func__);
	DO_FINISH_(ndo_set_rx_mode, {
	  ASYNC_({
	    ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
	  printk("%s, after async \n", __func__);
	  }, ndo_set_rx_mode);
	  printk("%s, after async2 \n", __func__);
	});

	 printk("%s, after dofin \n", __func__);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);

fail_ipc:
fail_async:
	lcd_exit(0);
	return;
}

void ndo_set_rx_mode(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *net_dev_container;

	if (!get_current()->ptstate) {
		LIBLCD_MSG("%s:Called from userland - can't process", __func__);
		LCD_MAIN({
			ndo_set_rx_mode_user(dev, hidden_args);
		});
		printk("%s, returns to userspace \n", __func__);
		return;
	}

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, NDO_SET_RX_MODE);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	printk("%s, ipc call \n", __func__);
	ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
	printk("%s, ipc call done\n", __func__);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);

fail_ipc:
fail_async:
	return;
}

LCD_TRAMPOLINE_DATA(ndo_set_rx_mode_trampoline);
void LCD_TRAMPOLINE_LINKAGE(ndo_set_rx_mode_trampoline) ndo_set_rx_mode_trampoline(struct net_device *dev)
{
	void ( *volatile ndo_set_rx_mode_fp )(struct net_device *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_set_rx_mode_trampoline);
	ndo_set_rx_mode_fp = ndo_set_rx_mode;
	return ndo_set_rx_mode_fp(dev, hidden_args);

}



//DONE
int ndo_set_mac_address_user(struct net_device *dev, void *addr, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	int sync_ret;
	unsigned 	long addr_mem_sz;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
	unsigned 	int request_cookie;
	struct net_device_container *net_dev_container;
	cptr_t sync_end;
	net_dev_container = container_of(dev, struct net_device_container, net_device);

	/* creates lcd and cspace */
	lcd_enter();
	ret = grant_sync_ep(&sync_end, hidden_args->sync_ep);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, NDO_SET_MAC_ADDRESS);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	ret = sync_setup_memory(addr, sizeof(struct sockaddr), &addr_mem_sz, &addr_cptr, &addr_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}

	ret = thc_ipc_send_request(hidden_args->async_chnl, request, &request_cookie);

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
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

	lcd_cap_delete(sync_end);
	DO_FINISH_(ndo_set_mac_address, {
	  ASYNC_({
	ret = thc_ipc_recv_response(hidden_args->async_chnl,
				request_cookie,
				&response);
	 }, ndo_set_mac_address);
	});

	if (ret) {
		LIBLCD_ERR("async recv failed");
		goto fail_ipcresp;
	}

	ret = fipc_get_reg1(response);

	LIBLCD_MSG("%s: returned %d", __func__, ret);

	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);

fail_ipcresp:
fail_virt:
fail_sync:
fail_ipc:
fail_async:
	lcd_exit(0);
	return ret;
}

//DONE
int ndo_set_mac_address(struct net_device *dev, void *addr, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	int sync_ret;
	unsigned 	long addr_mem_sz;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
	struct net_device_container *net_dev_container;

	if (!get_current()->ptstate) {
		LIBLCD_MSG("%s:Called from userland - can't process", __func__);
		for (ret = 0; ret < 14; ret++) {
			printk("%02X:",((char*)addr)[ret]);
		}
		if (1) {
		LCD_MAIN({
			ret = ndo_set_mac_address_user(dev, addr, hidden_args); 
		});
		}
		return ret;//eth_mac_addr(dev, addr);
	}

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, NDO_SET_MAC_ADDRESS);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )addr), &addr_cptr, &addr_mem_sz, &addr_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}
	ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	lcd_set_r0(addr_mem_sz);
	lcd_set_r1(addr_offset);
	lcd_set_cr0(addr_cptr);
	sync_ret = lcd_sync_send(hidden_args->sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to send");
		goto fail_sync;
	}
	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);
	return ret;
fail_virt:
fail_sync:
fail_ipc:
fail_async:
	return ret;
}

LCD_TRAMPOLINE_DATA(ndo_set_mac_address_trampoline);
int LCD_TRAMPOLINE_LINKAGE(ndo_set_mac_address_trampoline) ndo_set_mac_address_trampoline(struct net_device *dev, void *addr)
{
	int ( *volatile ndo_set_mac_address_fp )(struct net_device *, void *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_set_mac_address_trampoline);
	ndo_set_mac_address_fp = ndo_set_mac_address;
	return ndo_set_mac_address_fp(dev, addr, hidden_args);

}

struct rtnl_link_stats64 *ndo_get_stats64_user(struct net_device *dev,
		struct rtnl_link_stats64 *stats,
		struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *net_dev_container;

	thc_init();

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_MSG("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(request, NDO_GET_STATS64);

	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	LIBLCD_MSG("%s, klcd netdev other ref %p | %lu",__func__, net_dev_container, net_dev_container->other_ref.cptr);

	DO_FINISH_(ndo_get_stats64, {
	  ASYNC_({
	    ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
	  }, ndo_get_stats64);
	});

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	stats->tx_packets += fipc_get_reg1(response);
	stats->tx_bytes += fipc_get_reg2(response);

	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);

fail_ipc:
fail_async:
	lcd_exit(0);
	return stats;
}

//DONE
struct rtnl_link_stats64 *ndo_get_stats64(struct net_device *dev, struct rtnl_link_stats64 *stats, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	//int i;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *net_dev_container;

	if (!get_current()->ptstate) {
#ifdef TIMESTAMP
#ifndef INDIVIDUAL_MARKERS
		printk("----> stat ndo_xmit <------\n");
#else
		printk("----> stat p1.kzalloc + hash <------\n");
#endif
		if (times_ndo_xmit)
			fipc_test_stat_print_info(times_ndo_xmit,
						NUM_PACKETS);

#endif

#if 0
		for (i = 0; i < NUM_CORES; i++) {
			if (ptrs[i]) {
				printk("----> stat ndo_xmit %llu <----\n", ptrs[i]->pid);
				fipc_test_stat_print_info(ptrs[i]->times_ndo_xmit,
						NUM_PACKETS);
			}
		}
#endif

#ifdef LCD_MEASUREMENT
		printk("----> stat LCD con_skb <------\n");

		if (times_con_skb)
			fipc_test_stat_print_info(times_con_skb,
						NUM_PACKETS);
#endif
#ifdef INDIVIDUAL_MARKERS
		printk("----> stat lcd ipc_send <------\n");
		if (times_ipc_send)
			fipc_test_stat_print_info(times_ipc_send,
						NUM_PACKETS);

		printk("----> stat p2.ipc_recv+msG_end <------\n");
		if (times_ipc_recv)
			fipc_test_stat_print_info(times_ipc_recv,
						NUM_PACKETS);
#endif
#ifdef PERF_EVENTS
		printk("----> perf event 1 <------\n");
		if (perf1)
			fipc_test_stat_print_info(perf1, NUM_PACKETS);

		printk("----> perf event 2 <------\n");
		if (perf2)
			fipc_test_stat_print_info(perf2, NUM_PACKETS);

		printk("----> perf event 3 <------\n");
		if (perf3)
			fipc_test_stat_print_info(perf3, NUM_PACKETS);

		printk("----> perf event 4 <------\n");
		if (perf4)
			fipc_test_stat_print_info(perf4, NUM_PACKETS);
#endif
/*		LCD_MAIN( {
			ndo_get_stats64_user(dev, stats, hidden_args);
		});*/

		stats->tx_packets = g_stats.tx_packets;
		stats->tx_bytes = g_stats.tx_bytes;

		return stats;
	}

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(request, NDO_GET_STATS64);

	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	stats->tx_packets = fipc_get_reg1(response);
	stats->tx_bytes = fipc_get_reg2(response);

	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);

fail_ipc:
fail_async:
	return stats;
}

LCD_TRAMPOLINE_DATA(ndo_get_stats64_trampoline);
struct rtnl_link_stats64 LCD_TRAMPOLINE_LINKAGE(ndo_get_stats64_trampoline) *ndo_get_stats64_trampoline(struct net_device *dev, struct rtnl_link_stats64 *stats)
{
	struct rtnl_link_stats64* ( *volatile ndo_get_stats64_fp )(struct net_device *, struct rtnl_link_stats64 *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_get_stats64_trampoline);
	ndo_get_stats64_fp = ndo_get_stats64;
	return ndo_get_stats64_fp(dev, stats, hidden_args);

}

//DONE
int ndo_change_carrier_user(struct net_device *dev, bool new_carrier, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *net_dev_container;

	thc_init();

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, NDO_CHANGE_CARRIER);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);
	fipc_set_reg2(request, new_carrier);

	DO_FINISH_(ndo_change_carrier, {
	  ASYNC_({
		ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
	  }, ndo_change_carrier);
	});

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);

fail_async:
fail_ipc:
	lcd_exit(0);
	return ret;
}

//DONE
int ndo_change_carrier(struct net_device *dev, bool new_carrier, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *net_dev_container;

	if (!get_current()->ptstate) {
		LIBLCD_ERR("%s:Called from userland - can't process", __func__);
		LCD_MAIN( {
			ret = ndo_change_carrier_user(dev, new_carrier, hidden_args);
		});
		return ret;
	}

	net_dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, NDO_CHANGE_CARRIER);
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);

	ret = thc_ipc_call(hidden_args->async_chnl, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);

fail_async:
fail_ipc:
	return ret;
}

LCD_TRAMPOLINE_DATA(ndo_change_carrier_trampoline);
int LCD_TRAMPOLINE_LINKAGE(ndo_change_carrier_trampoline) ndo_change_carrier_trampoline(struct net_device *dev, bool new_carrier)
{
	int ( *volatile ndo_change_carrier_fp )(struct net_device *, bool , struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_change_carrier_trampoline);
	ndo_change_carrier_fp = ndo_change_carrier;
	return ndo_change_carrier_fp(dev, new_carrier, hidden_args);

}

//TODO:
int validate(struct nlattr **tb, struct nlattr **data, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	int err;
	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	async_msg_set_fn_type(request, VALIDATE);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return ret;

}

LCD_TRAMPOLINE_DATA(validate_trampoline);
int LCD_TRAMPOLINE_LINKAGE(validate_trampoline) validate_trampoline(struct nlattr **tb, struct nlattr **data)
{
	int ( *volatile validate_fp )(struct nlattr **, struct nlattr **, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, validate_trampoline);
	validate_fp = validate;
	return validate_fp(tb, data, hidden_args);

}

//DONE
void setup_device_ops_trampolines(struct net_device_ops_container *netdev_ops_container, struct trampoline_hidden_args *hargs)
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

//DONE
void setup(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	struct setup_container *setup_container;
	struct net_device_container *net_dev_container;
	struct net_device_ops_container *netdev_ops_container;
	struct page *p;
	unsigned int pool_ord;
	cptr_t pool_cptr;
	uint32_t request_cookie;

	net_dev_container = container_of(dev,
			struct net_device_container,
			net_device);

	ret = async_msg_blocking_send_start(
			hidden_args->async_chnl,
			&_request);

	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

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

	/*fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);*/
	setup_container = (struct setup_container*)hidden_args->struct_container;

	LIBLCD_MSG("%s sending setup_container cptr: other_ref %lu", __func__, setup_container->other_ref.cptr);
	printk("%s sending netdev_cptr: %p my_ref %lu", __func__, dev, net_dev_container->my_ref.cptr);
	printk(" | other_ref %lu\n", net_dev_container->other_ref.cptr);
	fipc_set_reg1(_request, net_dev_container->other_ref.cptr);
	fipc_set_reg2(_request, setup_container->other_ref.cptr);
	fipc_set_reg3(_request, net_dev_container->my_ref.cptr);
	fipc_set_reg4(_request, netdev_ops_container->my_ref.cptr);

	ret = thc_ipc_send_request(hidden_args->async_chnl,
			_request,
			&request_cookie);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

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

	lcd_set_cr0(pool_cptr);
	lcd_set_r0(pool_ord);

	printk("%s, calling sync send", __func__);
	ret = lcd_sync_send(hidden_args->sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);

	if (ret) {
		LIBLCD_ERR("sync send");
		goto fail_sync;
	}

	ret = thc_ipc_recv_response(hidden_args->async_chnl,
			request_cookie,
			&_response);

	if (ret) {
		LIBLCD_ERR("failed to recv ipc");
		goto fail_ipc_rx;
	}
	printk("%s:%d, done with msg %d\n", __func__, __LINE__, async_msg_get_fn_type(_request));

	net_dev_container->net_device.flags = fipc_get_reg1(_response);
	net_dev_container->net_device.priv_flags = fipc_get_reg2(_response);
	net_dev_container->net_device.features = fipc_get_reg3(_response);
	net_dev_container->net_device.hw_features = fipc_get_reg4(_response);
	net_dev_container->net_device.hw_enc_features = fipc_get_reg5(_response);

	netdev_ops_container->other_ref.cptr = fipc_get_reg6(_response);

	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), _response);

	net_dev_container->net_device.netdev_ops = &netdev_ops_container->net_device_ops;

	setup_device_ops_trampolines(netdev_ops_container, hidden_args);
	net_dev_container->net_device.rtnl_link_ops
		= &g_ops_container->rtnl_link_ops;
fail_alloc:
fail_async:
fail_sync:
fail_vol:
fail_insert:
fail_ipc:
fail_ipc_rx:
	return;
}

LCD_TRAMPOLINE_DATA(setup_trampoline);
void LCD_TRAMPOLINE_LINKAGE(setup_trampoline) setup_trampoline(struct net_device *dev)
{
	void ( *volatile setup_fp )(struct net_device *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, setup_trampoline);
	setup_fp = setup;
	return setup_fp(dev, hidden_args);

}

//DONE
int register_netdevice_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	int ret;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = glue_cap_lookup_net_device_type(c_cspace, __cptr(fipc_get_reg1(request)), &dev_container);

	if (ret) {
		LIBLCD_MSG("lookup failed");
		goto fail_lookup;
	}

	dev_container->net_device.dev_addr = kmalloc(MAX_ADDR_LEN, GFP_KERNEL);

	rtnl_lock();
	ret = register_netdevice(( &dev_container->net_device ));
	rtnl_unlock();
	LIBLCD_MSG("unlocked ");

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}

	fipc_set_reg4(response, ret);
	thc_ipc_reply(channel, request_cookie, response);
fail_lookup:
	return ret;
}

//DONE
int ether_setup_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int ret;
	struct net_device_container *dev;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	cptr_t netdev_ref = __cptr(fipc_get_reg1(request));
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = glue_cap_lookup_net_device_type(c_cspace, netdev_ref, &dev);
	if (ret) {
		LIBLCD_MSG("lookup failed");
		goto fail_lookup;
	}

	LIBLCD_MSG("Calling ether_setup");
	ether_setup(&dev->net_device);
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(channel, request_cookie, response);
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

	lcd_set_cr0(*data_cptr);

	ret = lcd_sync_recv(sync_ep);

	lcd_set_cr0(CAP_CPTR_NULL);

	if (ret) {
		LIBLCD_ERR("failed to recv");
		goto fail2;
	}

	*order = lcd_r0();
	*off = lcd_r1();

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
int eth_mac_addr_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
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
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

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

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	thc_ipc_reply(channel, request_cookie, response);
fail_lookup:
	return ret;
}

int eth_validate_addr_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	struct net_device_container *net_dev_container;
	cptr_t netdev_ref = __cptr(fipc_get_reg1(request));

	request_cookie = thc_get_request_cookie(request);

	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	LIBLCD_MSG("cptr %lu", netdev_ref.cptr);

	ret = glue_cap_lookup_net_device_type(c_cspace, netdev_ref, &net_dev_container);
	if (ret) {
		LIBLCD_MSG("lookup failed");
		goto fail_lookup;
	}

	ret = eth_validate_addr(&net_dev_container->net_device);

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	printk("%s, returns %d\n", __func__, ret);
	thc_ipc_reply(channel, request_cookie, response);
	printk("%s, reply done %d\n", __func__, ret);

fail_lookup:
	return ret;
}

//DONE
int free_netdev_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	struct net_device_container *dev_container;
	struct net_device *dev;

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = glue_cap_lookup_net_device_type(c_cspace, __cptr(fipc_get_reg1(request)), &dev_container);

	if (ret) {
		LIBLCD_MSG("lookup failed");
		goto fail_lookup;
	}

	dev = &dev_container->net_device;

	free_netdev(dev);
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(channel, request_cookie, response);
fail_lookup:
	return ret;
}

//DONE
int netif_carrier_off_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	struct net_device_container *net_dev_container;
	cptr_t netdev_ref = __cptr(fipc_get_reg1(request));

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = glue_cap_lookup_net_device_type(c_cspace, netdev_ref, &net_dev_container);
	if (ret) {
		LIBLCD_MSG("lookup failed");
		goto fail_lookup;
	}

	netif_carrier_off(&net_dev_container->net_device);
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(channel, request_cookie, response);
fail_lookup:
	return ret;
}

//DONE
int netif_carrier_on_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int ret;
	struct fipc_message *response;
	unsigned int request_cookie;
	struct net_device_container *net_dev_container;
	cptr_t netdev_ref = __cptr(fipc_get_reg1(request));

	request_cookie = thc_get_request_cookie(request);

	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = glue_cap_lookup_net_device_type(c_cspace, netdev_ref, &net_dev_container);
	if (ret) {
		LIBLCD_MSG("lookup failed");
		goto fail_lookup;
	}
	netif_carrier_on(&net_dev_container->net_device);
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(channel, request_cookie, response);
fail_lookup:
	return ret;
}


//DONE
int __rtnl_link_register_callee(void)
{
	struct rtnl_link_ops_container *ops_container;
	cptr_t tx, rx;
	cptr_t tx_xmit, rx_xmit;
	struct thc_channel *chnl;
	cptr_t sync_endpoint;
	int ret;
	struct trampoline_hidden_args *validate_hidden_args;
	struct trampoline_hidden_args *setup_hidden_args;
	int err;
	struct net_info *net_info;
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
	ops_container->other_ref.cptr = lcd_r1();
	sync_endpoint = lcd_cr0();
	tx = lcd_cr1(); rx = lcd_cr2();
	tx_xmit = lcd_cr3(); rx_xmit = lcd_cr4();
	printk("%s, tx_xmit %lx | rx_xmit %lx\n", __func__,
			cptr_val(tx_xmit), cptr_val(rx_xmit));
	/*
	 * Set up async ring channel
	 */
	ret = setup_async_net_ring_channel(tx, rx, &chnl);
	if (ret) {
		LIBLCD_ERR("error setting up ring channel");
		goto fail6;
	}
	/*
	 * Add to dispatch loop
	 */
	net_info = add_net(chnl, c_cspace, sync_endpoint);
	if (!net_info) {
		LIBLCD_ERR("error adding to dispatch loop");
		goto fail7;
	}

	LIBLCD_MSG("%s, setting up xmit_channel", __func__);
	/*
	 * Set up async xmit_ring channel
	 */
	ret = setup_async_net_ring_channel(tx_xmit, rx_xmit, &xmit_chnl);
	if (ret) {
		LIBLCD_ERR("error setting up ring channel");
		goto fail6;
	}
	/*
	 * Add to dispatch loop
	 */
/*	net_info = add_net(xmit_chnl, c_cspace, sync_endpoint);
	if (!net_info) {
		LIBLCD_ERR("error adding to dispatch loop");
		goto fail7;
	}
*/
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
	setup_hidden_args->sync_ep = sync_endpoint;
	setup_hidden_args->async_chnl = chnl;
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
	validate_hidden_args->sync_ep = sync_endpoint;
	validate_hidden_args->async_chnl = chnl;
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

	ops_container->rtnl_link_ops.validate =
		LCD_HANDLE_TO_TRAMPOLINE(
			validate_hidden_args->t_handle);
	ops_container->rtnl_link_ops.kind = "dummy"; 
	ret = __rtnl_link_register(( &ops_container->rtnl_link_ops ));
	lcd_set_r1(ops_container->my_ref.cptr);

	g_ops_container = ops_container;

	goto out;
fail7:
fail6:
fail3:
	kfree(chnl);
	destroy_async_fs_ring_channel(chnl);
	return ret;
out:
	/*
	 * Flush capability registers
	 */
	lcd_set_cr0(CAP_CPTR_NULL);
	lcd_set_cr1(CAP_CPTR_NULL);
	lcd_set_cr2(CAP_CPTR_NULL);
	lcd_set_cr3(CAP_CPTR_NULL);
	lcd_set_cr4(CAP_CPTR_NULL);

	lcd_set_r0(ret);

	if (lcd_sync_reply())
		LIBLCD_ERR("double fault?");
	return ret;
}

//DONE
int __rtnl_link_unregister_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct rtnl_link_ops *ops;
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
	ops = kzalloc(sizeof( *ops ), GFP_KERNEL);
	if (!ops) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	//ops->kind = fipc_get_reg1(request);
	__rtnl_link_unregister(ops);
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(channel, request_cookie, response);
	return ret;
}

//DONE
int rtnl_link_unregister_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct rtnl_link_ops_container *ops_container;
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	struct trampoline_hidden_args *validate_hidden_args;

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = glue_cap_lookup_rtnl_link_ops_type(c_cspace, __cptr(fipc_get_reg2(request)), &ops_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	rtnl_link_unregister(( &ops_container->rtnl_link_ops ));

	glue_cap_remove(c_cspace, ops_container->my_ref);

	validate_hidden_args = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(
			ops_container->rtnl_link_ops.validate);
	kfree(validate_hidden_args->t_handle);
	kfree(validate_hidden_args);
	kfree(ops_container);

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}

	thc_ipc_reply(channel, request_cookie, response);
fail_lookup:
	return ret;
}

//DONE
int alloc_netdev_mqs_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int sizeof_priv;
	int ret;
	char *name;
	char name_assign_type;
	struct setup_container *temp;
	int txqs;
	int rxqs;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	struct net_device_container *dev_container;
	struct net_device *net_device;
	struct trampoline_hidden_args *setup_hidden_args;
	cptr_t netdev_otherref;

	request_cookie = thc_get_request_cookie(request);

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

	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	printk("received setup_container cptr: other_ref %lu", temp->other_ref.cptr);
	printk(" | netdev other ref cptr: %lu\n", netdev_otherref.cptr);

	ret = glue_cap_insert_setup_type(c_cspace, temp, &temp->my_ref);
	if (ret) {
		LIBLCD_ERR("insert");
		goto fail_insert;
	}


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
	setup_hidden_args->async_chnl = channel;
	temp->setup = LCD_HANDLE_TO_TRAMPOLINE(setup_hidden_args->t_handle);
	ret = set_memory_x(((unsigned long)setup_hidden_args->t_handle) & PAGE_MASK,
			ALIGN(LCD_TRAMPOLINE_SIZE(setup_trampoline),
				PAGE_SIZE) >> PAGE_SHIFT);
	if (ret) {
		LIBLCD_ERR("set mem nx");
		goto fail3;
	}

	net_device = alloc_netdev_mqs_lcd(sizeof_priv, name,
			name_assign_type,
			g_ops_container->rtnl_link_ops.setup,
			txqs, rxqs, netdev_otherref.cptr);

	dev_container = container_of(net_device, struct net_device_container, net_device);
	
//	dev_container->other_ref.cptr = netdev_otherref.cptr;

	/*ret = glue_cap_insert_net_device_type(c_cspace, dev_container , &dev_container->my_ref);

	if (!ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}*/

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
//	fipc_set_reg5(response, dev_container->my_ref.cptr);
	thc_ipc_reply(channel, request_cookie, response);
	return ret;
fail_insert:
fail3:
fail_alloc:
	return ret;
}

#ifdef CONSUME_SKB_NO_HASHING
int consume_skb_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int ret = 0;
	struct sk_buff *skb;

	skb = (struct sk_buff *) fipc_get_reg0(request);

	fipc_recv_msg_end(thc_channel_to_fipc(channel),
			request);

//	printk("%s, freeing %p\n", __func__, skb);
//	consume_skb(skb);

	return ret;
}

#else

//TODO:
int consume_skb_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int ret = 0;
	struct sk_buff_container *skb_c;
	struct sk_buff *skb;
#ifndef CONSUME_SKB_SEND_ONLY
	struct fipc_message *response;
	unsigned 	int request_cookie;
#endif
#ifdef DOUBLE_HASHING
	struct ptstate_t *pts_out;
	int pid;
#endif
	cptr_t skb_cptr, skbh_cptr;
	bool revoke = false;

#ifndef CONSUME_SKB_SEND_ONLY
	request_cookie = thc_get_request_cookie(request);
#endif

#ifdef DOUBLE_HASHING
	pid = fipc_get_reg1(request);

//	printk("Looking up for pid %d\n", pid);

	glue_lookup_tid(tid_table,
			pid,
			&pts_out);

//	printk("Got pts %p for pid %d\n", pts_out, pid);

	glue_lookup_skbuff_2(pts_out,
			__cptr(fipc_get_reg0(request)),
			&skb_c);

//	printk("Got skb %p for pts %p\n", skb_c, pts_out);
#else
	glue_lookup_skbuff(cptr_table,
			__cptr(fipc_get_reg0(request)),
			&skb_c);
#endif
	fipc_recv_msg_end(thc_channel_to_fipc(channel),
			request);

	if (!skb_c) {
		printk("%s, skb_c null\n", __func__);
		goto skip;
	} else {
		skb = skb_c->skb;
	}

	WARN_ON(!skb);

	if (!skb)
		goto skip;

	if (!skb->private) {
		/* restore */
		skb->head = skb_c->head;
		skb->data = skb_c->data;

		skb_cptr = skb_c->skb_cptr;
		skbh_cptr = skb_c->skbh_cptr;
		revoke = true;
	}
	//printk("%s, skb %p | skb_c %p\n", __func__, skb, skb_c);
	consume_skb(skb_c->skb);

	if (skb_c->tsk == current && revoke) {
		lcd_cap_revoke(skb_cptr);
		lcd_cap_revoke(skbh_cptr);
		lcd_unvolunteer_pages(skb_cptr);
		lcd_unvolunteer_pages(skbh_cptr);
	}

#ifdef DOUBLE_HASHING
	glue_remove_skbuff_2(pts_out, skb_c);
#else
	glue_remove_skbuff(skb_c);
#endif
#ifdef SKBC_PRIVATE_POOL
	WARN_ON(!skb_c);
	if(skb_c)
	priv_free(skb_c, SKB_CONTAINER_POOL);
//	printk("free : %p\n", skb_c);
#else
//	printk("%s, skb %p\n", __func__, skb_c);
	kmem_cache_free(skb_c_cache, skb_c);
#endif
skip:
#ifndef CONSUME_SKB_SEND_ONLY
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		goto fail_async;
	}
	thc_ipc_reply(channel, request_cookie, response);
fail_async:
#endif
	return ret;
}
#endif /* CONSUME_SKB_NO_HASHING */

int trigger_exit_to_lcd(struct thc_channel *_channel, enum dispatch_t disp)
{
	struct fipc_message *_request;
	int ret;
	int i;
	unsigned int request_cookie;

	ret = async_msg_blocking_send_start(_channel,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			disp);

	/* No need to wait for a response here */
	ret = thc_ipc_send_request(_channel,
			_request,
			&request_cookie);


	// free the pointers
	// there is a race if get_stats try to operate on this data at the same time
	// but quite unlikely.
	if (disp == TRIGGER_CLEAN) {
	thread = 0;
	for (i = 0; i < NUM_CORES; i++) {
		if (ptrs[i]) {
			if (ptrs[i]->exited) {
				kfree(ptrs[i]);
				ptrs[i] = NULL;
				continue;
			}
		}
	}
	}

	if (ret) {
		LIBLCD_ERR("thc_ipc send");
		goto fail_ipc;
	}
	awe_mapper_remove_id(request_cookie);
fail_async:
fail_ipc:
	return ret;
}

