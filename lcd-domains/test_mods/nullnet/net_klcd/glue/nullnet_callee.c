#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/trampoline.h>
#include "../../glue_helper.h"
#include "../nullnet_callee.h"

#include <asm/cacheflush.h>

#include <lcd_config/post_hook.h>

struct thc_channel *net_async;
struct cptr sync_ep;

struct trampoline_hidden_args {
	void *struct_container;
	struct glue_cspace *cspace;
	struct lcd_trampoline_handle *t_handle;
	struct thc_channel *async_chnl;
	struct cptr sync_ep;
};

static struct glue_cspace *c_cspace;

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
	return 0;
fail2:
	glue_cap_exit();
fail1:
	return ret;

}

void glue_nullnet_exit()
{
	glue_cap_destroy(c_cspace);
	glue_cap_exit();

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

static int setup_async_fs_ring_channel(cptr_t tx, cptr_t rx, 
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

int ndo_init(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	int err;
	struct fipc_message *request;
	struct fipc_message *response;
	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	async_msg_set_fn_type(request, NDO_INIT);
/*	fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	fipc_set_reg2(request, dev->rtnl_link_ops->kind);*/
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
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

void ndo_uninit(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	int err;
	struct fipc_message *request;
	struct fipc_message *response;
	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	async_msg_set_fn_type(request, NDO_UNINIT);
/*	fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	fipc_set_reg2(request, dev->rtnl_link_ops->kind);*/
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return;
}

LCD_TRAMPOLINE_DATA(ndo_uninit_trampoline);
void LCD_TRAMPOLINE_LINKAGE(ndo_uninit_trampoline) ndo_uninit_trampoline(struct net_device *dev)
{
	void ( *volatile ndo_uninit_fp )(struct net_device *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_uninit_trampoline);
	ndo_uninit_fp = ndo_uninit;
	return ndo_uninit_fp(dev, hidden_args);

}

int ndo_start_xmit(struct sk_buff *skb, struct net_device *dev, struct trampoline_hidden_args *hidden_args)
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
	async_msg_set_fn_type(request, NDO_START_XMIT);
/*	fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	fipc_set_reg2(request, dev->rtnl_link_ops->kind);*/
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return ret;

}

LCD_TRAMPOLINE_DATA(ndo_start_xmit_trampoline);
int LCD_TRAMPOLINE_LINKAGE(ndo_start_xmit_trampoline)
 ndo_start_xmit_trampoline(struct sk_buff *skb, struct net_device *dev)
{
	int ( *volatile ndo_start_xmit_fp )(struct sk_buff *, struct net_device *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_start_xmit_trampoline);
	ndo_start_xmit_fp = ndo_start_xmit;
	return ndo_start_xmit_fp(skb, dev, hidden_args);

}

int ndo_validate_addr(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
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
	async_msg_set_fn_type(request, NDO_VALIDATE_ADDR);
/*	fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	fipc_set_reg2(request, dev->rtnl_link_ops->kind);*/
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
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

void ndo_set_rx_mode(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	int err;
	struct fipc_message *request;
	struct fipc_message *response;
	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	async_msg_set_fn_type(request, NDO_SET_RX_MODE);
	/*fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	fipc_set_reg2(request, dev->rtnl_link_ops->kind);*/
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
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

int ndo_set_mac_address(struct net_device *dev, void *addr, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	int sync_ret;
	unsigned 	long addr_mem_sz;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
	int err;
	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	async_msg_set_fn_type(request, NDO_SET_MAC_ADDRESS);
/*	fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	fipc_set_reg2(request, dev->rtnl_link_ops->kind);*/
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )addr), &addr_cptr, &addr_mem_sz, &addr_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	lcd_set_r0(addr_mem_sz);
	lcd_set_r1(addr_offset);
	lcd_set_cr0(addr_cptr);
	sync_ret = lcd_sync_send(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to send");
		lcd_exit(-1);
	}
	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
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

struct rtnl_link_stats64 *ndo_get_stats64(struct net_device *dev, struct rtnl_link_stats64 *storage, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	int err;
	struct fipc_message *request;
	struct fipc_message *response;
	struct rtnl_link_stats64 *ret1 = NULL;
	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	async_msg_set_fn_type(request, NDO_GET_STATS64);
	/*fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	fipc_set_reg2(request, dev->rtnl_link_ops->kind);*/
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	//ret1 = fipc_get_reg5(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return ret1;

}

LCD_TRAMPOLINE_DATA(ndo_get_stats64_trampoline);
struct rtnl_link_stats64 LCD_TRAMPOLINE_LINKAGE(ndo_get_stats64_trampoline) *ndo_get_stats64_trampoline(struct net_device *dev, struct rtnl_link_stats64 *storage)
{
	struct rtnl_link_stats64* ( *volatile ndo_get_stats64_fp )(struct net_device *, struct rtnl_link_stats64 *, struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args, ndo_get_stats64_trampoline);
	ndo_get_stats64_fp = ndo_get_stats64;
	return ndo_get_stats64_fp(dev, storage, hidden_args);

}

int ndo_change_carrier(struct net_device *dev, bool new_carrier, struct trampoline_hidden_args *hidden_args)
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
	async_msg_set_fn_type(request, NDO_CHANGE_CARRIER);
	/*fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	fipc_set_reg2(request, dev->rtnl_link_ops->kind);
	fipc_set_reg4(request, new_carrier);*/
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
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

void setup(struct net_device *dev, struct trampoline_hidden_args *hidden_args)
{
	int ret;
	int err;
	struct fipc_message *request;
	struct fipc_message *response;
	struct setup_container *setup_container;
	struct net_device_container *net_dev_container;

	net_dev_container = container_of(dev, struct net_device_container, net_device);
	ret = async_msg_blocking_send_start(hidden_args->async_chnl, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, SETUP);

	ret = glue_cap_insert_net_device_type(c_cspace, net_dev_container, &net_dev_container->my_ref);
	if (ret) {
		LIBLCD_ERR("insert");
		goto fail_insert;
	}

	/*fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);*/
	setup_container = (struct setup_container*)hidden_args->struct_container;

	LIBLCD_MSG("sending setup_container cptr: other_ref %lu", setup_container->other_ref.cptr);	
	fipc_set_reg1(request, net_dev_container->other_ref.cptr);
	fipc_set_reg2(request, setup_container->other_ref.cptr);
	fipc_set_reg3(request, net_dev_container->my_ref.cptr);

	err = thc_ipc_call(hidden_args->async_chnl, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(hidden_args->async_chnl), response);
fail_async:
fail_insert:
fail_ipc:
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

int register_netdevice_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct net_device_container *dev_container;
	struct net_device_ops_container *netdev_ops_container;
	struct rtnl_link_ops_container *rtnl_link_ops_container;
	struct fipc_message *response;
	int err;
	unsigned 	int request_cookie;
	struct trampoline_hidden_args *ndo_init_hidden_args;
	struct trampoline_hidden_args *ndo_uninit_hidden_args;
	struct trampoline_hidden_args *ndo_start_xmit_hidden_args;
	struct trampoline_hidden_args *ndo_validate_addr_hidden_args;
	struct trampoline_hidden_args *ndo_set_rx_mode_hidden_args;
	struct trampoline_hidden_args *ndo_set_mac_address_hidden_args;
	struct trampoline_hidden_args *ndo_get_stats64_hidden_args;
	struct trampoline_hidden_args *ndo_change_carrier_hidden_args;
	struct trampoline_hidden_args *setup_hidden_args;
	struct trampoline_hidden_args *validate_hidden_args;
	int ret;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
	dev_container = kzalloc(sizeof( struct net_device_container   ), GFP_KERNEL);
	if (!dev_container) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	err = glue_cap_insert_net_device_type(c_cspace, dev_container, &dev_container->my_ref);
	if (!err) {
		LIBLCD_ERR("lcd insert");
		lcd_exit(-1);
	}
	dev_container->other_ref.cptr = fipc_get_reg4(response);
	//dev_container->net_device.netdev_ops = &netdev_ops_container->net_device_ops;
	//dev_container->net_device.rtnl_link_ops = &rtnl_link_ops_container->rtnl_link_ops;
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
	//ndo_init_hidden_args->struct_container = netdev_ops_container;
	ndo_init_hidden_args->cspace = c_cspace;
	//netdev_ops_container->net_device_ops.ndo_init = LCD_HANDLE_TO_TRAMPOLINE(ndo_init_hidden_args->t_handle);
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
	//ndo_uninit_hidden_args->struct_container = netdev_ops_container;
	ndo_uninit_hidden_args->cspace = c_cspace;
	//netdev_ops_container->net_device_ops.ndo_uninit = LCD_HANDLE_TO_TRAMPOLINE(ndo_uninit_hidden_args->t_handle);
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
	//ndo_start_xmit_hidden_args->struct_container = netdev_ops_container;
	ndo_start_xmit_hidden_args->cspace = c_cspace;
	//netdev_ops_container->net_device_ops.ndo_start_xmit = LCD_HANDLE_TO_TRAMPOLINE(ndo_start_xmit_hidden_args->t_handle);
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
	//ndo_validate_addr_hidden_args->struct_container = netdev_ops_container;
	ndo_validate_addr_hidden_args->cspace = c_cspace;
	//netdev_ops_container->net_device_ops.ndo_validate_addr = LCD_HANDLE_TO_TRAMPOLINE(ndo_validate_addr_hidden_args->t_handle);
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
	//ndo_set_rx_mode_hidden_args->struct_container = netdev_ops_container;
	ndo_set_rx_mode_hidden_args->cspace = c_cspace;
	//netdev_ops_container->net_device_ops.ndo_set_rx_mode = LCD_HANDLE_TO_TRAMPOLINE(ndo_set_rx_mode_hidden_args->t_handle);
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
	//ndo_set_mac_address_hidden_args->struct_container = netdev_ops_container;
	ndo_set_mac_address_hidden_args->cspace = c_cspace;
	//netdev_ops_container->net_device_ops.ndo_set_mac_address = LCD_HANDLE_TO_TRAMPOLINE(ndo_set_mac_address_hidden_args->t_handle);
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
	//ndo_get_stats64_hidden_args->struct_container = netdev_ops_container;
	ndo_get_stats64_hidden_args->cspace = c_cspace;
	//netdev_ops_container->net_device_ops.ndo_get_stats64 = LCD_HANDLE_TO_TRAMPOLINE(ndo_get_stats64_hidden_args->t_handle);
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
	//ndo_change_carrier_hidden_args->struct_container = netdev_ops_container;
	ndo_change_carrier_hidden_args->cspace = c_cspace;
	//netdev_ops_container->net_device_ops.ndo_change_carrier = LCD_HANDLE_TO_TRAMPOLINE(ndo_change_carrier_hidden_args->t_handle);
	setup_hidden_args = kzalloc(sizeof( *setup_hidden_args ), GFP_KERNEL);
	if (!setup_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}
	setup_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(setup_trampoline);
	if (!setup_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		lcd_exit(-1);
	}
	setup_hidden_args->t_handle->hidden_args = setup_hidden_args;
	//setup_hidden_args->struct_container = rtnl_link_ops_container;
	setup_hidden_args->cspace = c_cspace;
	//rtnl_link_ops_container->rtnl_link_ops.setup = LCD_HANDLE_TO_TRAMPOLINE(setup_hidden_args->t_handle);
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
	//validate_hidden_args->struct_container = rtnl_link_ops_container;
	validate_hidden_args->cspace = c_cspace;
	//rtnl_link_ops_container->rtnl_link_ops.validate = LCD_HANDLE_TO_TRAMPOLINE(validate_hidden_args->t_handle);
	//dev->rtnl_link_ops->kind = fipc_get_reg2(request);
	ret = register_netdevice(( &dev_container->net_device ));
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg3(response, dev_container->other_ref.cptr);
	fipc_set_reg1(response, netdev_ops_container->other_ref.cptr);
	fipc_set_reg2(response, rtnl_link_ops_container->other_ref.cptr);
	fipc_set_reg4(response, ret);
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

int ether_setup_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int ret;
	struct net_device_container *dev;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);


	ret = glue_cap_lookup_net_device_type(c_cspace, __cptr(fipc_get_reg1(request)), &dev);

	LIBLCD_MSG("ndev other ref %lu", dev->other_ref.cptr);
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

int eth_mac_addr_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	//struct net_device *dev;
	int ret;
	//void *p;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	/*dev = kzalloc(*sizeof( dev ), GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	dev->netdev_ops = kzalloc(*sizeof( dev->netdev_ops ), GFP_KERNEL);
	if (!dev->netdev_ops) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	dev->rtnl_link_ops = kzalloc(*sizeof( dev->rtnl_link_ops ), GFP_KERNEL);
	if (!dev->rtnl_link_ops) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	dev->rtnl_link_ops->kind = kzalloc(sizeof( char   ), GFP_KERNEL);
	if (!dev->rtnl_link_ops->kind) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}*/
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	gva_t p_gva;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
	//dev->rtnl_link_ops->kind = fipc_get_reg2(request);
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
	sync_ret = lcd_map_virt(p_cptr, mem_order, &p_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *p");
		lcd_exit(-1);
	}
	//ret = eth_mac_addr(dev, p);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

int eth_validate_addr_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	//struct net_device *dev;
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
	//dev->rtnl_link_ops->kind = fipc_get_reg2(request);
	//ret = eth_validate_addr(dev);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

int free_netdev_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
//	struct net_device *dev;
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
	//free_netdev(dev);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

int netif_carrier_off_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
//	struct net_device *dev;
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
	//netif_carrier_off(dev);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

int netif_carrier_on_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	//struct net_device *dev;
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
	//netif_carrier_on(dev);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

//int __rtnl_link_register_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
int __rtnl_link_register_callee(void)
{
	struct rtnl_link_ops_container *ops_container;
	cptr_t tx, rx;
	struct thc_channel *chnl;
	cptr_t sync_endpoint;
	int ret;
	struct trampoline_hidden_args *setup_hidden_args;
	struct trampoline_hidden_args *validate_hidden_args;
	int err;
	struct fs_info *fs_info;
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

	/*
	 * Set up async ring channel
	 */
	ret = setup_async_fs_ring_channel(tx, rx, &chnl);
	if (ret) {
		LIBLCD_ERR("error setting up ring channel");
		goto fail6;
	}
	/*
	 * Add to dispatch loop
	 */
	fs_info = add_fs(chnl, c_cspace, sync_endpoint);
	if (!fs_info) {
		LIBLCD_ERR("error adding to dispatch loop");
		goto fail7;
	}
	ops_container->fs_info = fs_info;
	/*setup_hidden_args = kzalloc(sizeof( *setup_hidden_args ), GFP_KERNEL);
	if (!setup_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}
	setup_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(setup_trampoline);
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
*/
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
	ret = set_memory_x(((unsigned long)validate_hidden_args->t_handle) & PAGE_MASK,
			ALIGN(LCD_TRAMPOLINE_SIZE(validate_trampoline),
				PAGE_SIZE) >> PAGE_SHIFT);
	if (ret) {
		LIBLCD_ERR("set mem nx");
		goto fail3;
	}

	LIBLCD_MSG("Calling real function ");
ops_container->rtnl_link_ops.validate = LCD_HANDLE_TO_TRAMPOLINE(validate_hidden_args->t_handle);
	ops_container->rtnl_link_ops.kind = "dummy"; 
	ret = __rtnl_link_register(( &ops_container->rtnl_link_ops ));
	lcd_set_r1(ops_container->other_ref.cptr);
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

	lcd_set_r0(ret);

	if (lcd_sync_reply())
		LIBLCD_ERR("double fault?");
	return ret;
}

int __rtnl_link_unregister_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct rtnl_link_ops *ops;
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
	ops = kzalloc(sizeof( *ops ), GFP_KERNEL);
	if (!ops) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	//ops->kind = fipc_get_reg1(request);
	__rtnl_link_unregister(ops);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;
}

int rtnl_link_unregister_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct rtnl_link_ops_container *ops_container;
	int ret;
	int err;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	struct trampoline_hidden_args *setup_hidden_args;
	struct trampoline_hidden_args *validate_hidden_args;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
	err = glue_cap_lookup_rtnl_link_ops_type(c_cspace, __cptr(fipc_get_reg2(request)), &ops_container);
	if (err) {
		LIBLCD_ERR("lookup");
		lcd_exit(-1);
	}
	LIBLCD_MSG("Calling real function %s", __func__);
	rtnl_link_unregister(( &ops_container->rtnl_link_ops ));
	glue_cap_remove(c_cspace, ops_container->my_ref);
	/*setup_hidden_args = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(ops_container->rtnl_link_ops.setup);
	kfree(setup_hidden_args->t_handle);
	kfree(setup_hidden_args);*/
	validate_hidden_args = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(ops_container->rtnl_link_ops.validate);
	kfree(validate_hidden_args->t_handle);
	kfree(validate_hidden_args);
	kfree(ops_container);
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(channel, request_cookie, response);

	return ret;

}

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

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
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

	temp->other_ref.cptr = fipc_get_reg2(request);

	LIBLCD_MSG("received setup_container cptr: other_ref %lu", temp->other_ref.cptr);
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

	net_device = alloc_netdev_mqs_lcd(sizeof_priv, name, name_assign_type, (temp->setup ), txqs, rxqs, fipc_get_reg6(request));

	dev_container = container_of(net_device, struct net_device_container, net_device);
	
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

int consume_skb_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int ret;
	int err;
	struct sk_buff_container *skb_container;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
	err = glue_cap_lookup_sk_buff_type(c_cspace, __cptr(fipc_get_reg1(request)), &skb_container);
	if (err) {
		LIBLCD_ERR("lookup");
		lcd_exit(-1);
	}
	consume_skb(( &skb_container->sk_buff ));
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}




void dispatch_loop()
{
	int ret = 0;
	for (;;) {
//		ret = lcd_recv(manufacturer_interface_cap);
		if (ret) {
			goto out;
		}
		switch (ret) {
			case REGISTER_NETDEVICE:
				LIBLCD_MSG("Calling function register_netdevice");
				//ret = register_netdevice_callee();
				break;
			case ETHER_SETUP:
				LIBLCD_MSG("Calling function ether_setup");
				//ret = ether_setup_callee();
				break;
			case ETH_MAC_ADDR:
				LIBLCD_MSG("Calling function eth_mac_addr");
				//ret = eth_mac_addr_callee();
				break;
			case ETH_VALIDATE_ADDR:
				LIBLCD_MSG("Calling function eth_validate_addr");
				//ret = eth_validate_addr_callee();
				break;
			case FREE_NETDEV:
				LIBLCD_MSG("Calling function free_netdev");
				//ret = free_netdev_callee();
				break;
			case NETIF_CARRIER_OFF:
				LIBLCD_MSG("Calling function netif_carrier_off");
				//ret = netif_carrier_off_callee();
				break;
			case NETIF_CARRIER_ON:
				LIBLCD_MSG("Calling function netif_carrier_on");
				//ret = netif_carrier_on_callee();
				break;
			case __RTNL_LINK_REGISTER:
				LIBLCD_MSG("Calling function __rtnl_link_register");
				//ret = __rtnl_link_register_callee();
				break;
			case __RTNL_LINK_UNREGISTER:
				LIBLCD_MSG("Calling function __rtnl_link_unregister");
				//ret = __rtnl_link_unregister_callee();
				break;
			case RTNL_LINK_UNREGISTER:
				LIBLCD_MSG("Calling function rtnl_link_unregister");
				//ret = rtnl_link_unregister_callee();
				break;
			case ALLOC_NETDEV_MQS:
				LIBLCD_MSG("Calling function alloc_netdev_mqs");
				//ret = alloc_netdev_mqs_callee();
				break;
			case CONSUME_SKB:
				LIBLCD_MSG("Calling function consume_skb");
				//ret = consume_skb_callee();
				break;
			case NDO_INIT:
				LIBLCD_MSG("Calling function ndo_init");
				//ret = ndo_init_callee();
				break;
			case NDO_UNINIT:
				LIBLCD_MSG("Calling function ndo_uninit");
				//ret = ndo_uninit_callee();
				break;
			case NDO_START_XMIT:
				LIBLCD_MSG("Calling function ndo_start_xmit");
				//ret = ndo_start_xmit_callee();
				break;
			case NDO_VALIDATE_ADDR:
				LIBLCD_MSG("Calling function ndo_validate_addr");
				//ret = ndo_validate_addr_callee();
				break;
			case NDO_SET_RX_MODE:
				LIBLCD_MSG("Calling function ndo_set_rx_mode");
				//ret = ndo_set_rx_mode_callee();
				break;
			case NDO_SET_MAC_ADDRESS:
				LIBLCD_MSG("Calling function ndo_set_mac_address");
				//ret = ndo_set_mac_address_callee();
				break;
			case NDO_GET_STATS64:
				LIBLCD_MSG("Calling function ndo_get_stats64");
				//ret = ndo_get_stats64_callee();
				break;
			case NDO_CHANGE_CARRIER:
				LIBLCD_MSG("Calling function ndo_change_carrier");
				//ret = ndo_change_carrier_callee();
				break;
			default:
				break;
		}
		if (ret) {
			goto out;
		}
	}
out:
	return;
}

