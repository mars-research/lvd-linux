#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/trampoline.h>
#include <linux/aer.h>

#include "../../ixgbe_common.h"
#include "../ixgbe_callee.h"

#include <linux/hashtable.h>
#include <asm/cacheflush.h>

#include <lcd_config/post_hook.h>

struct glue_cspace *c_cspace = NULL;
struct thc_channel *ixgbe_async;
struct cptr sync_ep;
extern struct cspace *klcd_cspace;

struct timer_list service_timer;

/* This is the only device we strive for */
#define IXGBE_DEV_ID_82599_SFP_SF2       0x154D

/* XXX: There's no way to pass arrays across domains for now.
 * May not be in the future too! But agree that this is ugly
 * and move forward. - vik
 */
static const struct pci_device_id ixgbe_pci_tbl[] = {
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_SFP_SF2) },
	{ 0 } /* sentinel */
};

/* XXX: How to determine this? */
#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(cptr_table, CPTR_HASH_BITS);

struct pci_dev *g_pdev = NULL;
struct net_device *g_ndev = NULL;

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

}

int glue_insert_skbuff(struct hlist_head *htable, struct sk_buff_container *skb_c)
{
        BUG_ON(!skb_c->skb);

        skb_c->my_ref = __cptr((unsigned long)skb_c->skb);

        hash_add(cptr_table, &skb_c->hentry, (unsigned long) skb_c->skb);
        return 0;
}

int glue_lookup_skbuff(struct hlist_head *htable, struct cptr c, struct sk_buff_container **skb_cout)
{
        struct sk_buff_container *skb_c;

        hash_for_each_possible(cptr_table, skb_c, hentry, (unsigned long) cptr_val(c)) {
                printk("lookup skb %p\n", skb_c->skb);
		if (skb_c->skb == (struct sk_buff*) c.cptr) {
			printk("match %p <==> %lx", skb_c->skb, c.cptr);
	                *skb_cout = skb_c;
		}
        }
        return 0;
}

void glue_remove_skbuff(struct sk_buff_container *skb_c)
{
	hash_del(&skb_c->hentry);
}

void destroy_async_net_ring_channel(struct thc_channel *chnl)
{
	cptr_t tx, rx;
	gva_t tx_gva, rx_gva;
	unsigned long unused1, unused2;
	int ret;
	unsigned int pg_order = ASYNC_RPC_BUFFER_ORDER - PAGE_SHIFT;
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

int setup_async_net_ring_channel(cptr_t tx, cptr_t rx, 
				struct thc_channel **chnl_out)
{
	gva_t tx_gva, rx_gva;
	int ret;
	struct fipc_ring_channel *fchnl;
	struct thc_channel *chnl;
	unsigned int pg_order = ASYNC_RPC_BUFFER_ORDER - PAGE_SHIFT;
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
				ASYNC_RPC_BUFFER_ORDER,
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

int grant_sync_ep(cptr_t *sync_end, cptr_t ha_sync_ep)
{
	int ret;
	struct cspace *curr_cspace = get_current_cspace(current);
	lcd_cptr_alloc(sync_end);
	ret = cap_grant(klcd_cspace, ha_sync_ep, curr_cspace, *sync_end);
	return ret;
}

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

const char driver_name[] = "ixgbe_lcd";

int pci_disable_msix_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev = g_pdev;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	pci_disable_msix(dev);
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

int pci_enable_msix_range_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	gva_t p_gva;
	int minvec, maxvec;
	struct msix_entry *entries;

	dev = g_pdev;

	request_cookie = thc_get_request_cookie(_request);
	minvec = fipc_get_reg0(_request);
	maxvec = fipc_get_reg1(_request);

	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	sync_ret = lcd_cptr_alloc(&p_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}
	lcd_set_cr0(p_cptr);
	sync_ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to recv");
		lcd_exit(-1);
	}
	mem_order = lcd_r0();
	p_offset = lcd_r1();
	sync_ret = lcd_map_virt(p_cptr,
		mem_order,
		&p_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *p");
		lcd_exit(-1);
	}

	entries = (struct msix_entry*)(void*)(gva_val(p_gva) + p_offset);

	LIBLCD_MSG("%s, dev->msix_enabled %d", __func__, dev->msix_enabled);

	func_ret = pci_enable_msix_range(dev, entries, minvec, maxvec);

	LIBLCD_MSG("%s, returned %d", __func__, func_ret);

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
}

int __pci_register_driver_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_driver_container *drv_container;
	struct module_container *owner_container;
	char *name;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	struct trampoline_hidden_args *drv_probe_hidden_args;
	struct trampoline_hidden_args *drv_remove_hidden_args;
	int func_ret;
	int ret;

	request_cookie = thc_get_request_cookie(_request);
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
	drv_container->other_ref.cptr = fipc_get_reg2(_request);
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
	owner_container->other_ref.cptr = fipc_get_reg3(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

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
	drv_probe_hidden_args->async_chnl = _channel;
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
	drv_remove_hidden_args->async_chnl = _channel;
	drv_container->pci_driver.remove = LCD_HANDLE_TO_TRAMPOLINE(drv_remove_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_remove_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(remove_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	drv_container->pci_driver.name = driver_name;
	drv_container->pci_driver.id_table = ixgbe_pci_tbl;
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

	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("retor getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			drv_container->my_ref.cptr);
	fipc_set_reg2(_response,
			owner_container->my_ref.cptr);
	fipc_set_reg3(_response,
			func_ret);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
	return ret;
fail_alloc:
fail_alloc1:
fail_alloc2:
fail_dup1:
fail_dup2:
fail_insert:
	return ret;
}

int alloc_etherdev_mqs_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	int sizeof_priv;
	unsigned 	int txqs;
	unsigned 	int rxqs;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	struct net_device_container *func_ret_container;
	struct net_device *func_ret;
	cptr_t netdev_ref;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	sizeof_priv = fipc_get_reg1(_request);
	txqs = fipc_get_reg2(_request);
	rxqs = fipc_get_reg3(_request);
	netdev_ref.cptr = fipc_get_reg4(_request);

	func_ret = alloc_etherdev_mqs(sizeof_priv,
		txqs,
		rxqs);
	g_ndev = func_ret;
	func_ret_container = container_of(func_ret,
		struct net_device_container, net_device);
	func_ret_container->other_ref = netdev_ref;
	ret = glue_cap_insert_net_device_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret_container->my_ref.cptr);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
fail_insert:
	return ret;

}

int probe_user(struct pci_dev *dev,
		struct pci_device_id *id,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	int ret;

	/* XXX: we need cptr tree too. This lcd context will be destroyed
	 * rendering any volunteered resource void after this function is
	 * returned. Not the right way to do it. Use lcd_enter instead.
	 */
	thc_init();

	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PROBE);
	DO_FINISH_(probe_user,{
		ASYNC_({
			ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
		}, probe_user
		);
	}
	);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	lcd_exit(0);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

extern void ixgbe_service_timer(unsigned long data);

int probe(struct pci_dev *dev,
		struct pci_device_id *id,
		struct trampoline_hidden_args *hidden_args)
{
	struct pci_dev_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
#ifdef PCI_REGIONS
	uint32_t request_cookie;
	cptr_t res0_cptr;
	unsigned int res0_len;
#endif
	/* assign pdev to a global instance */
	g_pdev = dev;

	LIBLCD_MSG("%s, irq # %d | msix_enabled %d", __func__, dev->irq, dev->msix_enabled);

	if (!current->ptstate) {
		dump_stack();
		LIBLCD_MSG("Calling from a non-LCD context! creating thc runtime!");
		LCD_MAIN({
			ret = probe_user(dev,
		id,
		hidden_args);
		}
		);
		return ret;
	}

	dump_stack();
	dev_container = kzalloc(sizeof( struct pci_dev_container   ),
		GFP_KERNEL);
	if (!dev_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_pci_dev_type(hidden_args->cspace,
		dev_container,
		&dev_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			PROBE);
	fipc_set_reg1(_request,
			dev_container->my_ref.cptr);
	fipc_set_reg2(_request,
			*dev->dev.dma_mask);

#ifdef PCI_REGIONS
	ret = thc_ipc_send_request(hidden_args->async_chnl,
			_request,
			&request_cookie);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	/*
	 * ixgbe driver just needs res[0]
	 */
	res0_len = pci_resource_len(dev, 0);
	ret = lcd_volunteer_dev_mem(__gpa(pci_resource_start(dev, 0)),
			get_order(res0_len),
			&res0_cptr);
	if (ret) {
		LIBLCD_ERR("volunteer devmem");
		goto fail_vol;
	}

	lcd_set_cr0(res0_cptr);
	lcd_set_r0(res0_len);

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

#else
	ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
#endif
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);

	setup_timer(&service_timer, &ixgbe_service_timer, (unsigned long) NULL);
	return func_ret;
fail_async:
fail_sync:
fail_ipc:
fail_vol:
fail_insert:
fail_alloc:
fail_ipc_rx:
	return ret;
}

int pci_unregister_driver_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_driver_container *drv_container;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int ret;
	struct trampoline_hidden_args *drv_probe_hidden_args;

	request_cookie = thc_get_request_cookie(_request);

	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	ret = glue_cap_lookup_pci_driver_type(cspace,
		__cptr(fipc_get_reg2(_request)),
		&drv_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	LIBLCD_MSG("Calling pci_unregister_driver");

	pci_unregister_driver(( &drv_container->pci_driver ));

	LIBLCD_MSG("Called pci_unregister_driver");
	glue_cap_remove(c_cspace,
			drv_container->my_ref);

	/* XXX: Do not do anything like this! read the comments
	 * under pci_unregister_driver
	 * atomic_dec_if_positive(&drv_container->pci_driver.driver.owner->refcnt);
	 */
	drv_probe_hidden_args = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(drv_container->pci_driver.probe);
	kfree(drv_probe_hidden_args->t_handle);
	kfree(drv_probe_hidden_args);
	kfree(drv_container);
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("retor getting response msg");
		return -EIO;
	}
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
	return ret;
fail_lookup:
	return ret;
}

void remove_user(struct pci_dev *dev,
		struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	thc_init();

	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			REMOVE);
	DO_FINISH_(remove_user,{
		ASYNC_({
			ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
		}, remove_user
		);
	}
	);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	lcd_exit(0);
	return;
fail_async:
fail_ipc:
	return;
}

void remove(struct pci_dev *dev,
		struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	if (!current->ptstate) {
		LIBLCD_MSG("Calling from a non-LCD context! creating thc runtime!");
		LCD_MAIN({
			remove_user(dev,
					hidden_args);
		}
		);
		return;
	}

	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			REMOVE);

	ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	del_timer_sync(&service_timer);
	return;
fail_async:
fail_ipc:
	return;
}

int ndo_open_user(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	thc_init();
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_OPEN);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	DO_FINISH({
		ASYNC({
			ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
		}
		);
	}
	);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	printk("%s, returned %d\n", __func__, func_ret);
	mod_timer(&service_timer, jiffies + msecs_to_jiffies(2000));
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	lcd_exit(0);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

int ndo_open(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	if (!current->ptstate) {
		LIBLCD_MSG("Calling from a non-LCD context! creating thc runtime!");
		LCD_MAIN({
			ret = ndo_open_user(dev,
		hidden_args);
		}
		);
		return ret;
	}
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_OPEN);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

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

int ndo_stop_user(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	thc_init();
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_STOP);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	DO_FINISH({
		ASYNC_({
			ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
		}, ndo_stop
		);
	}
	);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	lcd_exit(0);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

int ndo_stop(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	if (!current->ptstate) {
		LIBLCD_MSG("Calling from a non-LCD context! creating thc runtime!");
		LCD_MAIN({
			ret = ndo_stop_user(dev,
		hidden_args);
		}
		);
		return ret;
	}
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_STOP);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

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

int ndo_start_xmit_user(struct sk_buff *skb,
		struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	unsigned int request_cookie;

	cptr_t sync_end;
	unsigned long skb_ord, skb_off;
	unsigned long skbd_ord, skbd_off;
	cptr_t skb_cptr, skbd_cptr;

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);

	/* enter LCD mode to have cspace tree */
	lcd_enter();

	ret = grant_sync_ep(&sync_end, hidden_args->sync_ep);

	ret = sync_setup_memory(skb, sizeof(struct sk_buff), &skb_ord, &skb_cptr, &skb_off);

	ret = sync_setup_memory(skb->head, skb_end_offset(skb) + sizeof(struct skb_shared_info), &skbd_ord, &skbd_cptr, &skbd_off);

	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_START_XMIT);

	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);

	ret = thc_ipc_send_request(hidden_args->async_chnl, _request, &request_cookie);

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	//sync half
	lcd_set_cr0(skb_cptr);
	lcd_set_cr1(skbd_cptr);
	lcd_set_r0(skb_ord);
	lcd_set_r1(skb_off);
	lcd_set_r2(skbd_ord);
	lcd_set_r3(skbd_off);
	lcd_set_r4(skb->data - skb->head);

	LIBLCD_MSG("skb->data %p | nr_frags %d",
		__pa(skb->data),
		skb_shinfo(skb)->nr_frags);

	ret = lcd_sync_send(sync_end);
	lcd_set_cr0(CAP_CPTR_NULL);
	lcd_set_cr1(CAP_CPTR_NULL);
	if (ret) {
		LIBLCD_ERR("failed to send");
		goto fail_sync;
	}

	lcd_cap_delete(sync_end);

	DO_FINISH({
		ASYNC_({
			ret = thc_ipc_recv_response(hidden_args->async_chnl, request_cookie, &_response);
		}, ndo_xmit
		);
	}
	);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);

fail_sync:
fail_async:
fail_ipc:
	lcd_exit(0);
	return func_ret;

}

int ndo_start_xmit(struct sk_buff *skb,
		struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	if (!current->ptstate) {
		LIBLCD_MSG("Calling %s from a non-LCD context! creating thc runtime!",
			__func__);
		LCD_MAIN({
			ret = ndo_start_xmit_user(skb,
		dev,
		hidden_args);
		}
		);
		LIBLCD_MSG("%s returns %d", __func__, ret);
		return ret;
	}
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_START_XMIT);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

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

void ndo_set_rx_mode_user(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	thc_init();
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_SET_RX_MODE);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	DO_FINISH({
		ASYNC_({
			ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
		}, ndo_set_rx_mode
		);
	}
	);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	lcd_exit(0);
	return;
fail_async:
fail_ipc:
	return;

}

void ndo_set_rx_mode(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	if (!current->ptstate) {
		LIBLCD_MSG("Calling from a non-LCD context! creating thc runtime!");
		LCD_MAIN({
			ndo_set_rx_mode_user(dev,
					hidden_args);
		}
		);
		return;
	}
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_SET_RX_MODE);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	return;
fail_async:
fail_ipc:
	return;

}

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

int ndo_validate_addr_user(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	thc_init();
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_VALIDATE_ADDR);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	DO_FINISH({
		ASYNC_({
			ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
		}, ndo_validate_addr
		);
	}
	);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	lcd_exit(0);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

int ndo_validate_addr(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	if (!current->ptstate) {
		LIBLCD_MSG("Calling from a non-LCD context! creating thc runtime!");
		LCD_MAIN({
			ret = ndo_validate_addr_user(dev,
		hidden_args);
		}
		);
		return ret;
	}
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_VALIDATE_ADDR);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

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

int ndo_set_mac_address_user(struct net_device *dev,
		void *addr,
		struct trampoline_hidden_args *hidden_args)
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
	thc_init();
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_SET_MAC_ADDRESS);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )addr),
		&addr_cptr,
		&addr_mem_sz,
		&addr_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	ret = thc_ipc_send_request(hidden_args->async_chnl,
		_request,
		&request_cookie);

	if (ret) {
		LIBLCD_ERR("thc_ipc_send_request");
		goto fail_ipc;
	}
	lcd_set_r0(ilog2(( addr_mem_sz ) >> ( PAGE_SHIFT )));
	lcd_set_r1(addr_offset);
	lcd_set_cr0(addr_cptr);
	sync_ret = lcd_sync_send(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to send");
		lcd_exit(-1);
	}
	DO_FINISH({
		ASYNC_({
		ret = thc_ipc_recv_response(hidden_args->async_chnl,
		request_cookie,
		&_response);
		}, ndo_set_mac_addr
		);
	});
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	lcd_exit(0);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

int ndo_set_mac_address(struct net_device *dev,
		void *addr,
		struct trampoline_hidden_args *hidden_args)
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
	if (!current->ptstate) {
		LIBLCD_MSG("Calling from a non-LCD context! creating thc runtime!");
		LCD_MAIN({
			ret = ndo_set_mac_address_user(dev,
		addr,
		hidden_args);
		}
		);
		return ret;
	}
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_SET_MAC_ADDRESS);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )addr),
		&addr_cptr,
		&addr_mem_sz,
		&addr_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	ret = thc_ipc_send_request(hidden_args->async_chnl,
		_request,
		&request_cookie);
	if (ret) {
		LIBLCD_ERR("thc_ipc_send_request");
		goto fail_ipc;
	}
	lcd_set_r0(ilog2(( addr_mem_sz ) >> ( PAGE_SHIFT )));
	lcd_set_r1(addr_offset);
	lcd_set_cr0(addr_cptr);
	sync_ret = lcd_sync_send(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to send");
		lcd_exit(-1);
	}
	ret = thc_ipc_recv_response(hidden_args->async_chnl,
		request_cookie,
		&_response);
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

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

int ndo_change_mtu_user(struct net_device *dev,
		int new_mtu,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	thc_init();
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_CHANGE_MTU);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg3(_request,
			new_mtu);
	DO_FINISH({
		ASYNC_({
			ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
		}, ndo_change_mtu
		);
	}
	);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	lcd_exit(0);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

int ndo_change_mtu(struct net_device *dev,
		int new_mtu,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	if (!current->ptstate) {
		LIBLCD_MSG("Calling from a non-LCD context! creating thc runtime!");
		LCD_MAIN({
			ret = ndo_change_mtu_user(dev,
		new_mtu,
		hidden_args);
		}
		);
		return ret;
	}
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_CHANGE_MTU);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg3(_request,
			new_mtu);
	ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

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

void ndo_tx_timeout_user(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	thc_init();
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_TX_TIMEOUT);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	DO_FINISH({
		ASYNC_({
			ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
		}, ndo_tx_timeout
		);
	}
	);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	lcd_exit(0);

fail_async:
fail_ipc:
	return;

}

void ndo_tx_timeout(struct net_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	if (!current->ptstate) {
		LIBLCD_MSG("Calling from a non-LCD context! creating thc runtime!");
		LCD_MAIN({
			ndo_tx_timeout_user(dev,
		hidden_args);
		}
		);
		return;
	}
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_TX_TIMEOUT);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
fail_async:
fail_ipc:
	return;

}

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

int ndo_set_tx_maxrate_user(struct net_device *dev,
		int queue_index,
		unsigned int maxrate,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	thc_init();
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_SET_TX_MAXRATE);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg3(_request,
			queue_index);
	fipc_set_reg4(_request,
			maxrate);
	DO_FINISH({
		ASYNC_({
			ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
		}, ndo_set_tx_maxrate
		);
	}
	);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	lcd_exit(0);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

int ndo_set_tx_maxrate(struct net_device *dev,
		int queue_index,
		unsigned int maxrate,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	if (!current->ptstate) {
		LIBLCD_MSG("Calling from a non-LCD context! creating thc runtime!");
		LCD_MAIN({
			ret = ndo_set_tx_maxrate_user(dev,
		queue_index,
		maxrate,
		hidden_args);
		}
		);
		return ret;
	}
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_SET_TX_MAXRATE);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	fipc_set_reg3(_request,
			queue_index);
	fipc_set_reg4(_request,
			maxrate);
	ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;

}

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

struct rtnl_link_stats64 *ndo_get_stats64_user(struct net_device *dev,
		struct rtnl_link_stats64 *stats,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	thc_init();
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_GET_STATS64);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	DO_FINISH({
		ASYNC_({
			ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
		}, ndo_get_stats
		);
	}
	);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	lcd_exit(0);
	return stats;
fail_async:
fail_ipc:
	return stats;
}

struct rtnl_link_stats64 *ndo_get_stats64(struct net_device *dev,
		struct rtnl_link_stats64 *stats,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	struct rtnl_link_stats64 *func_ret = stats;
	if (!current->ptstate) {
		LIBLCD_MSG("Calling from a non-LCD context! creating thc runtime!");
		LCD_MAIN({
			func_ret = ndo_get_stats64_user(dev,
		stats,
		hidden_args);
		}
		);
		return func_ret;
	}
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			NDO_GET_STATS64);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);

	LIBLCD_MSG("netdev lcd_ref %lu", dev_container->other_ref.cptr);

	ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	stats->rx_packets = fipc_get_reg1(_response);
	stats->rx_bytes = fipc_get_reg2(_response);
	stats->tx_packets = fipc_get_reg3(_response);
	stats->tx_bytes = fipc_get_reg4(_response);

	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return func_ret;
}

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


void setup_netdev_ops(struct net_device_container *dev_c,
	struct net_device_ops_container *netdev_ops_container, struct thc_channel *_channel,
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
	dev_netdev_ops_ndo_open_hidden_args->async_chnl = _channel;
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
	dev_netdev_ops_ndo_stop_hidden_args->async_chnl = _channel;
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
	dev_netdev_ops_ndo_start_xmit_hidden_args->async_chnl = _channel;
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
	dev_netdev_ops_ndo_set_rx_mode_hidden_args->async_chnl = _channel;
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
	dev_netdev_ops_ndo_validate_addr_hidden_args->async_chnl = _channel;
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
	dev_netdev_ops_ndo_set_mac_address_hidden_args->async_chnl = _channel;
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
	dev_netdev_ops_ndo_change_mtu_hidden_args->async_chnl = _channel;
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
	dev_netdev_ops_ndo_tx_timeout_hidden_args->async_chnl = _channel;
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
	dev_netdev_ops_ndo_set_tx_maxrate_hidden_args->async_chnl = _channel;
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
	dev_netdev_ops_ndo_get_stats64_hidden_args->async_chnl = _channel;
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

int register_netdev_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	struct net_device_ops_container *netdev_ops_container;
	struct net_device *dev;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	//a0:36:9f:08:1c:3e
	u8 mac_addr[] = {0xa0, 0x36, 0x9f, 0x08, 0x1c, 0x3e};

	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
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
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = &dev_container->net_device;
	dev->flags = fipc_get_reg1(_request);
	dev->priv_flags = fipc_get_reg2(_request);
	dev->features = fipc_get_reg3(_request);
	dev->hw_features = fipc_get_reg4(_request);
	dev->hw_enc_features = fipc_get_reg5(_request);
	dev->mpls_features = fipc_get_reg6(_request);

	memcpy(dev->dev_addr, mac_addr, ETH_ALEN);
	/* setup netdev_ops */
	setup_netdev_ops(dev_container, netdev_ops_container, _channel, sync_ep);

	func_ret = register_netdev(( &dev_container->net_device ));
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret);
	fipc_set_reg2(_response,
			dev->reg_state);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);

fail_lookup:
fail_insert:
fail_alloc:
	return ret;
}

int ether_setup_callee(struct fipc_message *_request,
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
	ether_setup(( &dev_container->net_device ));
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

int eth_mac_addr_callee(struct fipc_message *_request,
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
	unsigned 	long p_offset;
	cptr_t p_cptr;
	gva_t p_gva;

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
	sync_ret = lcd_cptr_alloc(&p_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}
	lcd_set_cr0(p_cptr);
	sync_ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to recv");
		lcd_exit(-1);
	}
	mem_order = lcd_r0();
	p_offset = lcd_r1();
	sync_ret = lcd_map_virt(p_cptr,
		mem_order,
		&p_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *p");
		lcd_exit(-1);
	}
	func_ret = eth_mac_addr(( &dev_container->net_device ),
		( void  * )( ( gva_val(p_gva) ) + ( p_offset ) ));
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

int eth_validate_addr_callee(struct fipc_message *_request,
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
	func_ret = eth_validate_addr(( &dev_container->net_device ));
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

int free_netdev_callee(struct fipc_message *_request,
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
	free_netdev(( &dev_container->net_device ));
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

int netif_carrier_off_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int ret = 0;
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
	netif_carrier_off(( &dev_container->net_device ));
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response, dev_container->net_device.state);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
fail_lookup:
	return ret;

}

int netif_carrier_on_callee(struct fipc_message *_request,
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
	netif_carrier_on(( &dev_container->net_device ));
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response, dev_container->net_device.state);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
fail_lookup:
	return ret;

}

int netif_device_attach_callee(struct fipc_message *_request,
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
	netif_device_attach(( &dev_container->net_device ));
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response, dev_container->net_device.state);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
	return ret;
fail_lookup:
	return ret;
}

int netif_device_detach_callee(struct fipc_message *_request,
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
	netif_device_detach(( &dev_container->net_device ));
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response, dev_container->net_device.state);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
	return ret;
fail_lookup:
	return ret;
}

int netif_set_real_num_rx_queues_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	unsigned 	int rxq;
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
	rxq = fipc_get_reg3(_request);
	func_ret = netif_set_real_num_rx_queues(( &dev_container->net_device ),
		rxq);
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

int netif_set_real_num_tx_queues_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	unsigned 	int txq;
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
	txq = fipc_get_reg3(_request);

	LIBLCD_MSG("%s, txq %d | num_tx_queues %d", __func__, txq, dev_container->net_device.num_tx_queues);

	func_ret = netif_set_real_num_tx_queues(( &dev_container->net_device ),
		txq);
	LIBLCD_MSG("netif_set_real_num_tx_queues returns %d", func_ret);
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

int consume_skb_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct sk_buff *skb;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	skb = kzalloc(sizeof( *skb ),
		GFP_KERNEL);
	if (!skb) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	consume_skb(skb);
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
fail_alloc:
	return ret;

}

int unregister_netdev_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg0(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	unregister_netdev(&dev_container->net_device);
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

int eth_platform_get_mac_address_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct device *dev;
	int ret = 0;
	struct fipc_message *_response;
	unsigned int request_cookie;
	int func_ret;
	union mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };
	u8 mac_addr[ETH_ALEN];

	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = &g_pdev->dev;

	func_ret = eth_platform_get_mac_address(dev, mac_addr);

	LIBLCD_MSG("%s returned %d", __func__, func_ret);

	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}

	/* pass on mac addr only if the function gets a
	 * valid address
	 */
	if (!func_ret) {
		memcpy(m.mac_addr, mac_addr, ETH_ALEN);
		fipc_set_reg2(_response,
			m.mac_addr_l);
	}

	fipc_set_reg1(_response,
			func_ret);

	thc_ipc_reply(_channel,
			request_cookie,
			_response);
	return ret;
}

int dev_addr_add_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device *dev;
	struct net_device_container *dev_container;
	unsigned 	char addr_type;
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
	dev = &dev_container->net_device;
	addr_type = fipc_get_reg3(_request);
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
	rtnl_lock();
	func_ret = dev_addr_add(dev,
		( void  * )( ( gva_val(addr_gva) ) + ( addr_offset ) ),
		addr_type);
	rtnl_unlock();
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

int dev_addr_del_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device *dev;
	struct net_device_container *dev_container;
	unsigned 	char addr_type;
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
	dev = &dev_container->net_device;
	addr_type = fipc_get_reg3(_request);
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
	rtnl_lock();
	func_ret = dev_addr_del(dev,
		( void  * )( ( gva_val(addr_gva) ) + ( addr_offset ) ),
		addr_type);
	rtnl_unlock();
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

int device_set_wakeup_enable_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct device *dev;
	bool enable;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = &g_pdev->dev;
	enable = fipc_get_reg1(_request);
	func_ret = device_set_wakeup_enable(dev,
		enable);
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
}

int eth_get_headlen_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long data_offset;
	cptr_t data_cptr;
	gva_t data_gva;
	unsigned 	int len;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	unsigned 	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	sync_ret = lcd_cptr_alloc(&data_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}
	lcd_set_cr0(data_cptr);
	sync_ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to recv");
		lcd_exit(-1);
	}
	mem_order = lcd_r0();
	data_offset = lcd_r1();
	sync_ret = lcd_map_virt(data_cptr,
		mem_order,
		&data_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *data");
		lcd_exit(-1);
	}
	len = fipc_get_reg1(_request);
	func_ret = eth_get_headlen(( void  * )( ( gva_val(data_gva) ) + ( data_offset ) ),
		len);
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

}

int netif_tx_stop_all_queues_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *ndev_container;
	struct net_device *dev;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg0(_request)),
		&ndev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	dev = &ndev_container->net_device;
	netif_tx_stop_all_queues(dev);
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

int _netif_tx_wake_all_queues_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_queue_container;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	struct net_device *dev;
	int num_qs;

	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev_queue_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	num_qs = fipc_get_reg2(_request);

	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	dev = &dev_queue_container->net_device;
	dev->num_tx_queues = num_qs;

	netif_tx_wake_all_queues(dev);

	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
	return ret;
fail_lookup:
	return ret;
}

int pci_disable_pcie_error_reporting_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = g_pdev;
	func_ret = pci_disable_pcie_error_reporting(dev);
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
}

int pci_bus_read_config_word_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_bus *bus = g_pdev->bus;
	unsigned 	int devfn;
	int where;
	unsigned short val;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	devfn = fipc_get_reg1(_request);
	where = fipc_get_reg2(_request);
	func_ret = pci_bus_read_config_word(bus,
		devfn,
		where,
		&val);
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			val);
	fipc_set_reg2(_response,
			func_ret);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
	return ret;
}

int pci_bus_write_config_word_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_bus *bus;
	unsigned 	int devfn;
	int where;
	unsigned 	short val;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);

	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	bus = g_pdev->bus;
	devfn = fipc_get_reg3(_request);
	where = fipc_get_reg4(_request);
	val = fipc_get_reg5(_request);
	func_ret = pci_bus_write_config_word(bus,
		devfn,
		where,
		val);
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
}

int pci_cleanup_aer_uncorrect_error_status_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = g_pdev;
	func_ret = pci_cleanup_aer_uncorrect_error_status(dev);
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
}

int pci_disable_device_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = g_pdev;
	pci_disable_device(dev);
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

int pci_enable_pcie_error_reporting_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev = g_pdev;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	func_ret = pci_enable_pcie_error_reporting(dev);
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
}

int pcie_capability_read_word_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev;
	int pos;
	unsigned short val;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = g_pdev;
	pos = fipc_get_reg1(_request);
	val = fipc_get_reg2(_request);
	func_ret = pcie_capability_read_word(dev,
		pos,
		&val);
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(_response,
			func_ret);
	fipc_set_reg2(_response,
			val);
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
	return ret;
}

int pcie_get_minimum_link_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev;
	enum pci_bus_speed speed;
	enum pcie_link_width width;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = g_pdev;
	speed = fipc_get_reg1(_request);
	width = fipc_get_reg2(_request);

	func_ret = pcie_get_minimum_link(dev,
		&speed,
		&width);
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
}

int pci_enable_device_mem_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev = g_pdev;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	func_ret = pci_enable_device_mem(dev);
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
}

int pci_request_selected_regions_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	int type;
	int ret = 0;
	struct fipc_message *_response;
	unsigned int request_cookie;
	int func_ret;
	struct pci_dev *dev = g_pdev;

	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	type = fipc_get_reg1(_request);
	func_ret = pci_request_selected_regions(dev,
		type,
		driver_name);
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
}

int pci_request_selected_regions_exclusive_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev;
	int type;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = g_pdev;
	type = fipc_get_reg1(_request);
	func_ret = pci_request_selected_regions_exclusive(dev,
		type,
		driver_name);
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
}

int pci_set_master_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev = g_pdev;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	pci_set_master(dev);
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

int pci_save_state_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev = g_pdev;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	func_ret = pci_save_state(dev);
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
}

int pci_release_selected_regions_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev;
	int r;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = g_pdev;
	r = fipc_get_reg1(_request);
	pci_release_selected_regions(dev,
			r);
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

int pci_select_bars_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev;
	unsigned 	long flags;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = g_pdev;
	flags = fipc_get_reg1(_request);
	func_ret = pci_select_bars(dev,
		flags);
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
}

int pci_wake_from_d3_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct pci_dev *dev;
	bool enable;
	int ret = 0;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = g_pdev;
	enable = fipc_get_reg1(_request);
	func_ret = pci_wake_from_d3(dev,
		enable);
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
}

int trigger_exit_to_lcd(struct thc_channel *_channel)
{
	struct fipc_message *_request;
	int ret;
	unsigned int request_cookie;

	dump_stack();
	ret = async_msg_blocking_send_start(_channel,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			TRIGGER_EXIT);

	/* No need to wait for a response here */
	ret = thc_ipc_send_request(_channel,
			_request,
			&request_cookie);

	if (ret) {
		LIBLCD_ERR("thc_ipc send");
		goto fail_ipc;
	}

fail_async:
fail_ipc:
	return ret;
}

int ixgbe_trigger_dump(struct thc_channel *_channel)
{
	struct fipc_message *_request;
	unsigned int request_cookie;
	int ret;
	struct net_device_container *dev_container;
	bool cleanup = false;

	if (!PTS()) {
		cleanup = true;
		thc_init();
	}

	ret = async_msg_blocking_send_start(_channel,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			TRIGGER_DUMP);
	dev_container = container_of(g_ndev,
		struct net_device_container, net_device);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);

	/* No need to wait for a response here */
	ret = thc_ipc_send_request(_channel,
			_request,
			&request_cookie);
	if (ret) {
		LIBLCD_ERR("thc_ipc send");
		goto fail_ipc;
	}

fail_async:
fail_ipc:
	if (cleanup)
		lcd_exit(0);
	return ret;
}

int ixgbe_service_event_sched(struct thc_channel *_channel)
{
	struct fipc_message *_request;
	unsigned int request_cookie;
	int ret;
	struct net_device_container *dev_container;
	bool cleanup = false;

	if (!PTS()) {
		cleanup = true;
		thc_init();
	}

	ret = async_msg_blocking_send_start(_channel,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			SERVICE_EVENT_SCHED);
	dev_container = container_of(g_ndev,
		struct net_device_container, net_device);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);

	/* No need to wait for a response here */
	ret = thc_ipc_send_request(_channel,
			_request,
			&request_cookie);
	if (ret) {
		LIBLCD_ERR("thc_ipc send");
		goto fail_ipc;
	}

fail_async:
fail_ipc:
	if (cleanup)
		lcd_exit(0);
	return ret;
}

int sync_user(struct net_device *dev,
		unsigned char *mac,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };


	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	thc_init();
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			SYNC);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	if (mac) {
		memcpy(m.mac_addr, mac, ETH_ALEN);
		MAC_ADDR_DUMP(m.mac_addr);
		fipc_set_reg3(_request, m.mac_addr_l);
	}

	DO_FINISH_(sync_user, {
		ASYNC_({
			ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
		}, sync_user
		);
	}
	);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	lcd_exit(0);
	return func_ret;
fail_async:
fail_ipc:
	lcd_exit(0);
	return ret;
}

int sync(struct net_device *dev,
		unsigned char *mac,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };

	if (!current->ptstate) {
		LIBLCD_MSG("Calling from a non-LCD context! creating thc runtime!");
		LCD_MAIN({
			ret = sync_user(dev,
		mac,
		hidden_args);
		}
		);
		return ret;
	}
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	if (mac) {
		memcpy(m.mac_addr, mac, ETH_ALEN);
		MAC_ADDR_DUMP(m.mac_addr);
		fipc_set_reg3(_request, m.mac_addr_l);
	}
	async_msg_set_fn_type(_request,
			SYNC);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

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

int unsync_user(struct net_device *dev,
		unsigned char *mac,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };

	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	thc_init();
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			UNSYNC);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	if (mac) {
		memcpy(m.mac_addr, mac, ETH_ALEN);
		MAC_ADDR_DUMP(m.mac_addr);
		fipc_set_reg3(_request, m.mac_addr_l);
	}

	DO_FINISH_(unsync_user, {
		ASYNC_({
			ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
		}, unsync_user
		);
	}
	);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	lcd_exit(0);
	return func_ret;
fail_async:
fail_ipc:
	lcd_exit(0);
	return ret;
}


int unsync(struct net_device *dev,
		unsigned char *mac,
		struct trampoline_hidden_args *hidden_args)
{
	struct net_device_container *dev_container;
	int ret;
	struct fipc_message *_request;
	struct fipc_message *_response;
	int func_ret;
	union __mac {
		u8 mac_addr[ETH_ALEN];
		unsigned long mac_addr_l;
	} m = { {0} };

	if (!current->ptstate) {
		LIBLCD_MSG("Calling from a non-LCD context! creating thc runtime!");
		LCD_MAIN({
			ret = unsync_user(dev,
		mac,
		hidden_args);
		}
		);
		return ret;
	}
	dev_container = container_of(dev,
		struct net_device_container,
		net_device);
	ret = async_msg_blocking_send_start(hidden_args->async_chnl,
		&_request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(_request,
			UNSYNC);
	fipc_set_reg1(_request,
			dev_container->other_ref.cptr);
	if (mac) {
		memcpy(m.mac_addr, mac, ETH_ALEN);
		MAC_ADDR_DUMP(m.mac_addr);
		fipc_set_reg3(_request, m.mac_addr_l);
	}

	ret = thc_ipc_call(hidden_args->async_chnl,
		_request,
		&_response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	func_ret = fipc_get_reg1(_response);
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl),
			_response);
	return func_ret;
fail_async:
fail_ipc:
	return ret;
}

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

struct trampoline_hidden_args *unsync_hidden_args;
struct unsync_container *unsync_container;

int __hw_addr_sync_dev_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev1_container;
	struct sync_container *sync_container;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	addr_list _type;
	struct trampoline_hidden_args *sync_hidden_args;

	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev1_container);
	_type = fipc_get_reg2(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
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
	sync_hidden_args->async_chnl = _channel;

	sync_container->sync = LCD_HANDLE_TO_TRAMPOLINE(sync_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )sync_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(sync_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));


	unsync_container = kzalloc(sizeof( struct unsync_container   ),
		GFP_KERNEL);
	if (!unsync_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
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
	unsync_hidden_args->async_chnl = _channel;

	unsync_container->unsync = LCD_HANDLE_TO_TRAMPOLINE(unsync_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )unsync_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(unsync_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	func_ret = __hw_addr_sync_dev(
		_type == UC_LIST ? &dev1_container->net_device.uc :
			&dev1_container->net_device.mc,
		( &dev1_container->net_device ),
		( sync_container->sync ),
		( unsync_container->unsync ));
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
fail_alloc:
fail_alloc1:
fail_dup1:
fail_alloc2:
fail_dup2:
	return ret;
}

int __hw_addr_unsync_dev_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev1_container;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	addr_list _type;
	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg1(_request)),
		&dev1_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	_type = fipc_get_reg2(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	__hw_addr_unsync_dev(
		_type == UC_LIST ? &dev1_container->net_device.uc :
			&dev1_container->net_device.mc,
		( &dev1_container->net_device ),
		( unsync_container->unsync ));
	if (async_msg_blocking_send_start(_channel,
		&_response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(_channel,
			request_cookie,
			_response);
	return ret;
fail_lookup:
	return ret;
}
