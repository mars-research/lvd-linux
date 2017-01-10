#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include "../../glue_helper.h"
#include "../nullnet_caller.h"
static struct cptr c;
struct cptr sync_ep;
static struct glue_cspace *c_cspace;
struct thc_channel *net_async;
static struct lcd_sync_channel_group *nullnet_group;

int glue_nullnet_init(struct cptr c1, struct lcd_sync_channel_group *nullnet_group1)
{
	int ret;
	c = c1;
	nullnet_group = nullnet_group1;
	ret = glue_cap_init();
	if (!ret) {
		LIBLCD_ERR("cap init");
		goto fail1;
	}
	ret = glue_cap_create(&c_cspace);
	if (!ret) {
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

int __rtnl_link_register(struct rtnl_link_ops *ops)
{
	struct rtnl_link_ops_container *ops_container;
	int err;
	struct fipc_message *request;
	struct fipc_message *response;
	int ret;
	ops_container = container_of(ops, struct rtnl_link_ops_container, rtnl_link_ops);
	err = glue_cap_insert_rtnl_link_ops_type(c_cspace, ops_container, &ops_container->my_ref);
	if (err) {
		LIBLCD_ERR("lcd insert");
		lcd_exit(-1);
	}
	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	async_msg_set_fn_type(request, __RTNL_LINK_REGISTER);
	fipc_set_reg2(request, ops_container->my_ref.cptr);
//	fipc_set_reg1(request, ops->kind);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	ops_container->other_ref.cptr = fipc_get_reg2(response);
	ret = fipc_get_reg2(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return ret;

}

void __rtnl_link_unregister(struct rtnl_link_ops *ops)
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
	async_msg_set_fn_type(request, __RTNL_LINK_UNREGISTER);
//	fipc_set_reg1(request, ops->kind);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return;
}

int register_netdevice(struct net_device *dev)
{
	struct net_device_container *dev_container;
	struct net_device_ops_container *netdev_ops_container;
	struct rtnl_link_ops_container *rtnl_link_ops_container;
	int err;
	struct fipc_message *request;
	struct fipc_message *response;
	int ret;
	dev_container = container_of(dev, struct net_device_container, net_device);
	netdev_ops_container = container_of(dev->netdev_ops, struct net_device_ops_container, net_device_ops);
	err = glue_cap_insert_net_device_ops_type(c_cspace, netdev_ops_container, &netdev_ops_container->my_ref);
	if (err) {
		LIBLCD_ERR("lcd insert");
		lcd_exit(-1);
	}
	rtnl_link_ops_container = container_of(dev->rtnl_link_ops, struct rtnl_link_ops_container, rtnl_link_ops);
	err = glue_cap_insert_rtnl_link_ops_type(c_cspace, rtnl_link_ops_container, &rtnl_link_ops_container->my_ref);
	if (err) {
		LIBLCD_ERR("lcd insert");
		lcd_exit(-1);
	}
	err = glue_cap_insert_net_device_type(c_cspace, dev_container, &dev_container->my_ref);
	if (err) {
		LIBLCD_ERR("lcd insert");
		lcd_exit(-1);
	}
	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	async_msg_set_fn_type(request, REGISTER_NETDEVICE);
	fipc_set_reg4(request, dev_container->my_ref.cptr);
	fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
//	fipc_set_reg2(request, dev->rtnl_link_ops->kind);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	dev_container->other_ref.cptr = fipc_get_reg4(response);
	netdev_ops_container->other_ref.cptr = fipc_get_reg1(response);
	rtnl_link_ops_container->other_ref.cptr = fipc_get_reg3(response);
	ret = fipc_get_reg4(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return ret;

}

void ether_setup(struct net_device *dev)
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
	async_msg_set_fn_type(request, ETHER_SETUP);
	//fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	//fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	//fipc_set_reg2(request, dev->rtnl_link_ops->kind);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return;
}

int eth_mac_addr(struct net_device *dev, void *p)
{
	struct fipc_message *request;
	struct fipc_message *response;
	int sync_ret;
	unsigned 	long p_mem_sz;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	int ret;
	int err;
	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	async_msg_set_fn_type(request, ETH_MAC_ADDR);
	//fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	//fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	//fipc_set_reg2(request, dev->rtnl_link_ops->kind);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )p), &p_cptr, &p_mem_sz, &p_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	lcd_set_r0(p_mem_sz);
	lcd_set_r1(p_offset);
	lcd_set_cr0(p_cptr);
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

int eth_validate_addr(struct net_device *dev)
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
	async_msg_set_fn_type(request, ETH_VALIDATE_ADDR);
	//fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	//fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	//fipc_set_reg2(request, dev->rtnl_link_ops->kind);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return ret;

}

void free_netdev(struct net_device *dev)
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
	async_msg_set_fn_type(request, FREE_NETDEV);
	//fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	//fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	//fipc_set_reg2(request, dev->rtnl_link_ops->kind);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return;
}

void netif_carrier_off(struct net_device *dev)
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
	async_msg_set_fn_type(request, NETIF_CARRIER_OFF);
	//fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	//fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	//fipc_set_reg2(request, dev->rtnl_link_ops->kind);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return;
}

void netif_carrier_on(struct net_device *dev)
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
	async_msg_set_fn_type(request, NETIF_CARRIER_ON);
	//fipc_set_reg1(request, netdev_ops_container->my_ref.cptr);
	//fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	//fipc_set_reg2(request, dev->rtnl_link_ops->kind);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return;
}


void rtnl_link_unregister(struct rtnl_link_ops *ops)
{
	struct rtnl_link_ops_container *ops_container;
	int err;
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	ops_container = container_of(ops, struct rtnl_link_ops_container, rtnl_link_ops);
	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	async_msg_set_fn_type(request, RTNL_LINK_UNREGISTER);
	fipc_set_reg2(request, ops_container->my_ref.cptr);
	//fipc_set_reg1(request, ops->kind);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	glue_cap_remove(c_cspace, ops_container->my_ref);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return;
}

struct net_device *alloc_netdev_mqs(int sizeof_priv, const char *name, unsigned char name_assign_type, void (*setup)(struct net_device*), unsigned int txqs, unsigned int rxqs)
{
	struct setup_container *temp;
	int ret;
	int err;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device *ret1;
	temp = kzalloc(sizeof( struct setup_container   ), GFP_KERNEL);
	if (!temp) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	async_msg_set_fn_type(request, ALLOC_NETDEV_MQS);
	fipc_set_reg1(request, sizeof_priv);
//	fipc_set_reg2(request, name);
	fipc_set_reg3(request, name_assign_type);
	fipc_set_reg4(request, txqs);
	fipc_set_reg5(request, rxqs);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	ret = fipc_get_reg5(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return ret1;

}

void consume_skb(struct sk_buff *skb)
{
	struct sk_buff_container *skb_container;
	int ret;
	int err;
	struct fipc_message *request;
	struct fipc_message *response;
	skb_container = kzalloc(sizeof( struct sk_buff_container   ), GFP_KERNEL);
	if (!skb_container) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	err = glue_cap_insert_sk_buff_type(c_cspace, skb_container, &skb_container->my_ref);
	if (!err) {
		LIBLCD_ERR("lcd insert");
		lcd_exit(-1);
	}
	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	async_msg_set_fn_type(request, CONSUME_SKB);
	fipc_set_reg1(request, skb_container->my_ref.cptr);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return;
}

int ndo_init_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep)
{
	struct net_device *dev;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	int ret;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
	dev = kzalloc(sizeof( *dev ), GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	dev->netdev_ops = kzalloc(sizeof( *dev->netdev_ops ), GFP_KERNEL);
	if (!dev->netdev_ops) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	dev->rtnl_link_ops = kzalloc(sizeof( *dev->rtnl_link_ops ), GFP_KERNEL);
	if (!dev->rtnl_link_ops) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
/*	dev->rtnl_link_ops->kind = kzalloc(sizeof( char   ), GFP_KERNEL);
	if (!dev->rtnl_link_ops->kind) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	dev->rtnl_link_ops->kind = fipc_get_reg2(request);*/
	//ret = ndo_init(dev);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

int ndo_uninit_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep)
{
	//struct net_device *dev;
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
//	ndo_uninit(dev);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

int ndo_start_xmit_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep)
{
//	struct sk_buff *skb;
//	struct net_device *dev;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	int ret;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
//	ret = ndo_start_xmit(skb, dev);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

int ndo_validate_addr_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep)
{
	//struct net_device *dev;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	int ret;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
//	ret = ndo_validate_addr(dev);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

int ndo_set_rx_mode_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep)
{
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
//	ndo_set_rx_mode(dev);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

int ndo_set_mac_address_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep)
{
//	void *addr;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	int ret;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
	gva_t addr_gva;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
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
	sync_ret = lcd_map_virt(addr_cptr, mem_order, &addr_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *addr");
		lcd_exit(-1);
	}
	//ret = ndo_set_mac_address(dev, addr);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

int ndo_get_stats64_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep)
{
	//struct rtnl_link_stats64 *storage;
	struct fipc_message *response;
	unsigned 	int request_cookie;
//	struct rtnl_link_stats64 *ret1;
	int ret;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
	//ret = ndo_get_stats64(dev, storage);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

int ndo_change_carrier_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep)
{
//	struct net_device *dev;
	//bool new_carrier;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	int ret;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
//	ret = ndo_change_carrier(dev, new_carrier);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}

int setup_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep)
{
	//struct net_device *dev;
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);

	//setup(dev);
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;
}

int validate_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep)
{
	struct nlattr **tb;
	struct nlattr **data;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	int ret;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), request);
	tb = kzalloc(sizeof( void  * ), GFP_KERNEL);
	if (!tb) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	( *tb ) = kzalloc(sizeof( ( **tb ) ), GFP_KERNEL);
	if (!( *tb )) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	data = kzalloc(sizeof( void  * ), GFP_KERNEL);
	if (!data) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	( *data ) = kzalloc(sizeof( ( *data ) ), GFP_KERNEL);
	if (!( *data )) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
//	ret = validate(( *tb ), ( *data ));
	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	thc_ipc_reply(net_async, request_cookie, response);
	return ret;

}


