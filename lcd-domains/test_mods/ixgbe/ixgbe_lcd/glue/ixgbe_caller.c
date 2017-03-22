#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include "../../ixgbe_common.h"
#include "../ixgbe_caller.h"
#include <asm/lcd_domains/liblcd.h>
#include <lcd_config/post_hook.h>

struct cptr sync_ep;
static struct glue_cspace *c_cspace;
extern struct thc_channel *ixgbe_async;
extern cptr_t ixgbe_sync_endpoint;
extern cptr_t ixgbe_register_channel;

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

int create_async_channel(void)
{
	int ret;
	cptr_t tx, rx;
	struct thc_channel *chnl;
	/*
	 * Set up async and sync channels
	 */
	ret = lcd_create_sync_endpoint(&ixgbe_sync_endpoint);
	if (ret) {
		LIBLCD_ERR("lcd_create_sync_endpoint");
		goto fail1;
	}
	ret = setup_async_channel(&tx, &rx, &chnl);
	if (ret) {
		LIBLCD_ERR("async chnl setup failed");
		goto fail2;
	}
        lcd_set_cr0(ixgbe_sync_endpoint);
        lcd_set_cr1(rx);
        lcd_set_cr2(tx);

	LIBLCD_MSG("sync call %s", __func__);
	ret = lcd_sync_call(ixgbe_register_channel);

	LIBLCD_MSG("sync call returned%s", __func__);

        /*
         * Flush cap registers
         */
        lcd_set_cr0(CAP_CPTR_NULL);
        lcd_set_cr1(CAP_CPTR_NULL);
        lcd_set_cr2(CAP_CPTR_NULL);
        if (ret) {
                LIBLCD_ERR("lcd_call");
                goto fail3;
        }
	ixgbe_async = chnl;
	LIBLCD_MSG("%s returns", __func__);
	return ret;
fail3:
        destroy_async_channel(chnl);
fail2:
	lcd_cap_delete(ixgbe_sync_endpoint);
fail1:
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
	drv_container->other_ref.cptr = fipc_get_reg2(_response);
	owner_container->other_ref.cptr = fipc_get_reg3(_response);
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
	func_ret_container = kzalloc(sizeof( struct net_device_container   ),
		GFP_KERNEL);
	if (!func_ret_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
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

int probe_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	int func_ret;
	int ret = 0;

	LIBLCD_MSG("%s called", __func__);
	request_cookie = thc_get_request_cookie(_request);

	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

	/* XXX: Pass null for now. struct pci_dev is passed from the
	 * kernel and needs analysis for wrapping it around a container
	 */
	func_ret = ixgbe_driver_container.pci_driver.probe(NULL, NULL);

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
	int ret;
	struct fipc_message *_response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(_request);
	fipc_recv_msg_end(thc_channel_to_fipc(_channel),
			_request);

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

