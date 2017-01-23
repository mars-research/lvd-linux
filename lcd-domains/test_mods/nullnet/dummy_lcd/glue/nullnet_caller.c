#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include "../../glue_helper.h"
#include "../nullnet_caller.h"

#include <lcd_config/post_hook.h>

struct cptr sync_ep;
static struct glue_cspace *c_cspace;
extern struct thc_channel *net_async;

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

static int setup_async_channel(cptr_t *buf1_cptr_out, cptr_t *buf2_cptr_out,
			struct thc_channel **chnl_out)
{
	int ret;
	cptr_t buf1_cptr, buf2_cptr;
	gva_t buf1_addr, buf2_addr;
	struct fipc_ring_channel *fchnl;
	struct thc_channel *chnl;
	unsigned int pg_order = PMFS_ASYNC_RPC_BUFFER_ORDER - PAGE_SHIFT;
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
	ret = fipc_prep_buffers(PMFS_ASYNC_RPC_BUFFER_ORDER,
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
	ret = fipc_ring_channel_init(fchnl, PMFS_ASYNC_RPC_BUFFER_ORDER,
				(void *)gva_val(buf1_addr),
				(void *)gva_val(buf2_addr));
	if (ret) {
		LIBLCD_ERR("ring chnl init");
		goto fail7;
	}
	if (0) {
		size_t sz = sizeof(*chnl);
		int idx = kmalloc_index(sz);
		int i = 0;
		for (i = 0; i < sizeof(kmalloc_caches)/sizeof(kmalloc_caches[0]); i++) {
			printk("--> idx %d | cache addr %p", i, kmalloc_caches[i]);
		}
		LIBLCD_MSG("==> Prep buffers 2, size %zu | idx %d  | ptr %p | KMALOC_MIN %d, SHIFT_LOW %d",
			sz, idx, kmalloc_caches[idx], KMALLOC_MIN_SIZE, KMALLOC_SHIFT_LOW);
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
	unsigned int pg_order = PMFS_ASYNC_RPC_BUFFER_ORDER - PAGE_SHIFT;
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

extern cptr_t nullnet_sync_endpoint;
extern cptr_t nullnet_register_channel;

int __rtnl_link_register(struct rtnl_link_ops *ops)
{
	struct rtnl_link_ops_container *ops_container;
	int err;
	cptr_t tx, rx;
	struct thc_channel *chnl;
	int ret;

	/*
	 * Set up async and sync channels
	 */
	ret = lcd_create_sync_endpoint(&nullnet_sync_endpoint);
	if (ret) {
		LIBLCD_ERR("lcd_create_sync_endpoint");
		goto fail1;
	}
	ret = setup_async_channel(&tx, &rx, &chnl);
	if (ret) {
		LIBLCD_ERR("async chnl setup failed");
		goto fail2;
	}

	ops_container = container_of(ops, struct rtnl_link_ops_container, rtnl_link_ops);
	LIBLCD_MSG("Retrieve container ops %p\n", ops_container);
	err = glue_cap_insert_rtnl_link_ops_type(c_cspace, ops_container, &ops_container->my_ref);
	if (err) {
		LIBLCD_ERR("lcd insert");
		goto fail3;
	}
        lcd_set_r0(__RTNL_LINK_REGISTER);
        lcd_set_r1(cptr_val(ops_container->my_ref));
        lcd_set_cr0(nullnet_sync_endpoint);
        lcd_set_cr1(rx);
        lcd_set_cr2(tx);

        ret = lcd_sync_call(nullnet_register_channel);

        /*
         * Flush cap registers
         */
        lcd_set_cr0(CAP_CPTR_NULL);
        lcd_set_cr1(CAP_CPTR_NULL);
        lcd_set_cr2(CAP_CPTR_NULL);
        if (ret) {
                LIBLCD_ERR("lcd_call");
                goto fail4;
        }

        ret = lcd_r0();
	printk("%s: Got %d\n", __func__, ret);
        if (ret < 0) {
                LIBLCD_ERR("remote register fs failed");
                goto fail5;
        }

	ops_container->other_ref.cptr = lcd_r1();
	net_async = chnl;
	return ret;

fail5:
fail4:
        //glue_cap_remove(c_cspace, ops_container->my_ref);
fail3:
        //destroy_async_channel(chnl);
fail2:
	lcd_cap_delete(nullnet_sync_endpoint);
fail1:
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
	struct net_device_container *netdev_container;

	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, ETHER_SETUP);

	netdev_container = container_of(dev, struct net_device_container, net_device);
	fipc_set_reg1(request, netdev_container->other_ref.cptr);
	LIBLCD_MSG("ndev other ref %lu\n", netdev_container->other_ref.cptr);
	//fipc_set_reg3(request, rtnl_link_ops_container->my_ref.cptr);
	//fipc_set_reg2(request, dev->rtnl_link_ops->kind);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
fail_ipc:
fail_async:
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
		goto fail1;
	}
	async_msg_set_fn_type(request, RTNL_LINK_UNREGISTER);
	fipc_set_reg2(request, ops_container->my_ref.cptr);
	//fipc_set_reg1(request, ops->kind);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail2;
	}
	glue_cap_remove(c_cspace, ops_container->my_ref);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);

	lcd_cap_delete(nullnet_sync_endpoint);
	destroy_async_channel(net_async);
fail2:
fail1:
	return;
}

struct net_device *alloc_netdev_mqs(int sizeof_priv, const char *name, unsigned char name_assign_type, void (*setup)(struct net_device* dev), unsigned int txqs, unsigned int rxqs)
{
	struct setup_container *setup_container;
	int ret;
	int err;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *ret1;
	struct rtnl_link_ops *rtnl_ops;
	struct rtnl_link_ops_container *rtnl_ops_cnt;

	ret1 = kzalloc(sizeof( struct net_device_container   ), GFP_KERNEL);
	if (!ret1) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	setup_container = kzalloc(sizeof(*setup_container), GFP_KERNEL);
	if (!setup_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	setup_container->setup = setup;

	ret = glue_cap_insert_setup_type(c_cspace, setup_container, &setup_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	ret = glue_cap_insert_net_device_type(c_cspace, ret1, &ret1->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(request, ALLOC_NETDEV_MQS);
	fipc_set_reg1(request, sizeof_priv);
	fipc_set_reg2(request, setup_container->my_ref.cptr);
	fipc_set_reg3(request, name_assign_type);
	fipc_set_reg4(request, txqs);
	fipc_set_reg5(request, rxqs);
	fipc_set_reg6(request, ret1->my_ref.cptr);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	//ret1->other_ref.cptr = fipc_get_reg5(response);

	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);

fail_ipc:
fail_async:
fail_alloc:
fail_insert:
	return &ret1->net_device;
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

int ndo_init_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
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

int ndo_uninit_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
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

int ndo_start_xmit_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
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

int ndo_validate_addr_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
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

int ndo_set_rx_mode_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
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

int ndo_set_mac_address_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
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

int ndo_get_stats64_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
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

int ndo_change_carrier_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
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

int setup_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	struct setup_container *setup_container;
	struct net_device_container *net_dev_container;

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = glue_cap_lookup_setup_type(c_cspace, __cptr(fipc_get_reg2(request)), &setup_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	ret = glue_cap_lookup_net_device_type(c_cspace, __cptr(fipc_get_reg1(request)), &net_dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	// save other ref cptr
	net_dev_container->other_ref = __cptr(fipc_get_reg3(request));
	setup_container->setup(&net_dev_container->net_device);

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(channel, request_cookie, response);

fail_lookup:
	return ret;
}

int validate_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
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


