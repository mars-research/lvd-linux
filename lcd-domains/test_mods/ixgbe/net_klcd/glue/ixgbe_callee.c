#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/trampoline.h>
#include <linux/aer.h>

#include "../../ixgbe_common.h"
#include "../ixgbe_callee.h"

#include <asm/cacheflush.h>

#include <lcd_config/post_hook.h>

struct glue_cspace *c_cspace = NULL;
struct thc_channel *ixgbe_async;
struct cptr sync_ep;
extern struct cspace *klcd_cspace;


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

struct pci_dev *g_pdev = NULL;

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
extern struct pci_driver_container *pci_container;

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
	/* Assign it to the global instance
	 * This is to safely deregister while unloading
	 */
	pci_container = drv_container;
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
	return;
fail_async:
fail_ipc:
	return;
}

int register_netdev_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	struct net_device *dev;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	ret = glue_cap_lookup_net_device_type(cspace,
		__cptr(fipc_get_reg0(_request)),
		&dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = &dev_container->net_device;
	dev->flags = fipc_get_reg1(_request);
	dev->priv_flags = fipc_get_reg2(_request);
	dev->features = fipc_get_reg3(_request);
	dev->hw_features = fipc_get_reg4(_request);
	dev->hw_enc_features = fipc_get_reg5(_request);
	dev->mpls_features = fipc_get_reg6(_request);
	func_ret = register_netdev(( &dev_container->net_device ));
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
	struct net_device *dev;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	netif_device_attach(dev);
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

int netif_device_detach_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device *dev;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	netif_device_detach(dev);
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

int netif_set_real_num_rx_queues_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device *dev;
	unsigned 	int rxq;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	rxq = fipc_get_reg1(_request);
	func_ret = netif_set_real_num_rx_queues(dev,
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
fail_alloc:
	return ret;

}

int netif_set_real_num_tx_queues_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device *dev;
	unsigned 	int txq;
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	txq = fipc_get_reg1(_request);
	func_ret = netif_set_real_num_tx_queues(dev,
		txq);
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
fail_alloc:
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

int dev_addr_del_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct net_device *dev;
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
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

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
	addr_type = fipc_get_reg1(_request);
	func_ret = dev_addr_del(dev,
		( void  * )( ( gva_val(addr_gva) ) + ( addr_offset ) ),
		addr_type);
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
fail_alloc:
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
