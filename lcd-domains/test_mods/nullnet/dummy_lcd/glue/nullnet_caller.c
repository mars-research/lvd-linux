#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include "../../glue_helper.h"
#include "../nullnet_caller.h"

#include <linux/hashtable.h>
#include <lcd_config/post_hook.h>

struct cptr sync_ep;
static struct glue_cspace *c_cspace;
extern struct thc_channel *net_async;

struct rtnl_link_ops *g_rtnl_link_ops;
void *data_pool;

/* XXX: How to determine this? */
#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(cptr_table, CPTR_HASH_BITS);

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
	hash_init(cptr_table);
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
		if (skb_c->skb == (struct sk_buff*) c.cptr) {
	                *skb_cout = skb_c;
		}
        }
        return 0;
}

void glue_remove_skbuff(struct sk_buff_container *skb_c)
{
	hash_del(&skb_c->hentry);
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

int create_async_channel(void)
{
	int ret;
	cptr_t tx, rx;
	struct thc_channel *chnl;
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
                goto fail3;
        }
	net_async = chnl;
	return ret;
fail3:
        //glue_cap_remove(c_cspace, ops_container->my_ref);
        //destroy_async_channel(chnl);
fail2:
	lcd_cap_delete(nullnet_sync_endpoint);
fail1:
	return ret;
}

//DONE
int __rtnl_link_register(struct rtnl_link_ops *ops)
{
	struct rtnl_link_ops_container *ops_container;
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

	ops_container = container_of(ops,
			struct rtnl_link_ops_container,
			rtnl_link_ops);
	ret = glue_cap_insert_rtnl_link_ops_type(c_cspace,
			ops_container,
			&ops_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail3;
	}

	lcd_set_r0(__RTNL_LINK_REGISTER);
        lcd_set_r1(cptr_val(ops_container->my_ref));
        lcd_set_cr0(nullnet_sync_endpoint);
        lcd_set_cr1(rx);
        lcd_set_cr2(tx);

	g_rtnl_link_ops = ops;

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

//DONE
void __rtnl_link_unregister(struct rtnl_link_ops *ops)
{
	int ret;
	int err;
	struct fipc_message *request;
	struct fipc_message *response;
	struct rtnl_link_ops_container *ops_container;

	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	ops_container = container_of(ops, struct rtnl_link_ops_container, rtnl_link_ops);
	async_msg_set_fn_type(request, __RTNL_LINK_UNREGISTER);
	fipc_set_reg1(request, ops_container->other_ref.cptr);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return;
}

//DONE
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

	rtnl_link_ops_container = container_of(dev->rtnl_link_ops, struct rtnl_link_ops_container, rtnl_link_ops);

	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		lcd_exit(-1);
	}
	async_msg_set_fn_type(request, REGISTER_NETDEVICE);
	fipc_set_reg1(request, dev_container->other_ref.cptr);
	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		lcd_exit(-1);
	}
	ret = fipc_get_reg4(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
	return ret;

}

//DONE
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

int sync_prep_data(void *data, unsigned long *sz, unsigned long *off, cptr_t *data_cptr)
{
    int ret;
	ret = lcd_virt_to_cptr(__gva((unsigned long)data), data_cptr, sz, off);
	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
	}
	return ret;
}


// DONE
int eth_mac_addr(struct net_device *dev, void *p)
{
	struct fipc_message *request;
	struct fipc_message *response;
	int sync_ret;
	unsigned 	long p_mem_sz;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	int ret;
	struct net_device_container *dev_container;
	uint32_t request_cookie;

	ret = async_msg_blocking_send_start(net_async, &request);

	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	dev_container = container_of(dev, struct net_device_container, net_device);

	fipc_set_reg1(request, dev_container->other_ref.cptr);

	async_msg_set_fn_type(request, ETH_MAC_ADDR);

	sync_ret = sync_prep_data(p, &p_mem_sz, &p_offset, &p_cptr);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}

	ret = thc_ipc_send_request(net_async, request, &request_cookie);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	lcd_set_r0(p_mem_sz);
	lcd_set_r1(p_offset);
	lcd_set_cr0(p_cptr);
	sync_ret = lcd_sync_send(nullnet_sync_endpoint);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to send");
		goto fail_sync;
	}

	ret = thc_ipc_recv_response(net_async,
				request_cookie,
				&response);
	if (ret) {
		LIBLCD_ERR("async recv failed");
		goto fail_ipc_recv;
	}

	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);

fail_async:
fail_sync:
fail_ipc_recv:
fail_ipc:
	return ret;
}

//DONE
int eth_validate_addr(struct net_device *dev)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *dev_container;

	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(request, ETH_VALIDATE_ADDR);
	
	dev_container = container_of(dev, struct net_device_container, net_device);

	fipc_set_reg1(request, dev_container->other_ref.cptr);

	LIBLCD_MSG("%s, cptr lcd %lu", __func__, dev_container->other_ref.cptr);

	ret = thc_ipc_call(net_async, request, &response);

	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	ret = fipc_get_reg1(response);
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);

fail_async:
fail_ipc:
	return ret;
}

//DONE
void free_netdev(struct net_device *dev)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *dev_container;

	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, FREE_NETDEV);

	dev_container = container_of(dev, struct net_device_container, net_device);

	fipc_set_reg1(request, dev_container->other_ref.cptr);

	ret = thc_ipc_call(net_async, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);

fail_async:
fail_ipc:
	return;
}

//DONE
void netif_carrier_off(struct net_device *dev)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *dev_container;

	dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}

	async_msg_set_fn_type(request, NETIF_CARRIER_OFF);
	fipc_set_reg1(request, dev_container->other_ref.cptr);
	
	ret = thc_ipc_call(net_async, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);
fail_async:
fail_ipc:
	return;
}

//DONE
void netif_carrier_on(struct net_device *dev)
{
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *dev_container;

	dev_container = container_of(dev, struct net_device_container, net_device);

	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, NETIF_CARRIER_ON);

	fipc_set_reg1(request, dev_container->other_ref.cptr);

	ret = thc_ipc_call(net_async, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}
	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);

fail_async:
fail_ipc:
	return;
}

// DONE
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
	fipc_set_reg2(request, ops_container->other_ref.cptr);

	err = thc_ipc_call(net_async, request, &response);
	if (err) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail2;
	}

	glue_cap_remove(c_cspace, ops_container->my_ref);

	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);

	//lcd_cap_delete(nullnet_sync_endpoint);
	if (0)
		destroy_async_channel(net_async);
fail2:
fail1:
	return;
}

// DONE
struct net_device *alloc_netdev_mqs(int sizeof_priv, const char *name, unsigned char name_assign_type, void (*setup)(struct net_device* dev), unsigned int txqs, unsigned int rxqs)
{
	struct setup_container *setup_container;
	int ret;
	int err;
	struct fipc_message *request;
	struct fipc_message *response;
	struct net_device_container *ret1;

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
	struct sk_buff_container *skb_c;
	int ret;
	struct fipc_message *request;
	struct fipc_message *response;
	unsigned long skb_sz, skb_off, skbh_sz, skbh_off;
	cptr_t skb_cptr, skbh_cptr;

	glue_lookup_skbuff(cptr_table,
		__cptr((unsigned long)skb), &skb_c);

	ret = async_msg_blocking_send_start(net_async, &request);
	if (ret) {
		LIBLCD_ERR("failed to get a send slot");
		goto fail_async;
	}
	async_msg_set_fn_type(request, CONSUME_SKB);
	fipc_set_reg0(request, skb_c->other_ref.cptr);

	if (!skb->private) {
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
	} else {
		/* free skb memory that was allocate by us */
		kfree(skb);
	}

	ret = thc_ipc_call(net_async, request, &response);
	if (ret) {
		LIBLCD_ERR("thc_ipc_call");
		goto fail_ipc;
	}

	glue_remove_skbuff(skb_c);
	kfree(skb_c);

	fipc_recv_msg_end(thc_channel_to_fipc(net_async), response);

fail_async:
fail_ipc:
fail_virt:
	return;
}

// DONE
int ndo_init_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct net_device_container *net_dev_container;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	int ret;
	cptr_t netdev_ref = __cptr(fipc_get_reg1(request));

	request_cookie = thc_get_request_cookie(request);

	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = glue_cap_lookup_net_device_type(c_cspace, netdev_ref, &net_dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	ret = net_dev_container->net_device.netdev_ops->ndo_init(&net_dev_container->net_device);

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	thc_ipc_reply(channel, request_cookie, response);
fail_lookup:
	return ret;
}

// DONE
int ndo_uninit_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
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
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	printk("%s called, triggering rpc\n", __func__);

	net_dev_container->net_device.netdev_ops->ndo_uninit(&net_dev_container->net_device);

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	thc_ipc_reply(channel, request_cookie, response);
fail_lookup:
	return ret;
}

int ndo_start_xmit_callee(struct fipc_message *_request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct sk_buff *skb;
	struct sk_buff_container *skb_c;
	struct net_device_container *net_dev_container;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	int ret;
	cptr_t skb_cptr, skbd_cptr;
	unsigned long skb_ord, skb_off;
	unsigned long skbd_ord, skbd_off;
	gva_t skb_gva, skbd_gva;
	xmit_type_t xmit_type;
	unsigned long skbh_offset, skb_end;
	struct skbuff_members *skb_lcd;
	__be16 proto;
	cptr_t skb_ref;
	unsigned int data_off;

	request_cookie = thc_get_request_cookie(_request);

	xmit_type = fipc_get_reg0(_request);

	ret = glue_cap_lookup_net_device_type(c_cspace,
			__cptr(fipc_get_reg1(_request)),
			&net_dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	skb_ref = __cptr(fipc_get_reg2(_request));

	skbh_offset = fipc_get_reg3(_request);

	skb_end = fipc_get_reg4(_request);
	proto = fipc_get_reg5(_request);

	fipc_recv_msg_end(thc_channel_to_fipc(net_async),
				_request);

	switch (xmit_type) {
	case VOLUNTEER_XMIT:
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

		lcd_set_cr0(skb_cptr);
		lcd_set_cr1(skbd_cptr);
		ret = lcd_sync_recv(sync_ep);
		lcd_set_cr0(CAP_CPTR_NULL);
		lcd_set_cr1(CAP_CPTR_NULL);
		if (ret) {
			LIBLCD_ERR("failed to recv");
			goto fail_sync;
		}
		skb_ord = lcd_r0();
		skb_off = lcd_r1();
		skbd_ord = lcd_r2();
		skbd_off = lcd_r3();
		data_off = lcd_r4();

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
		break;

	case SHARED_DATA_XMIT:
		skb = kzalloc(sizeof(struct sk_buff), GFP_KERNEL);

		if (!skb) {
			LIBLCD_MSG("out of mmeory");
			goto fail_alloc;
		}

		skb->head = (char*)data_pool + skbh_offset;
		skb->end = skb_end;

		skb_lcd = SKB_LCD_MEMBERS(skb);
		skb->private = true;

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

		break;

	default:
		LIBLCD_ERR("%s, unknown xmit type", __func__);
		break;
	}

	skb_c = kzalloc(sizeof(*skb_c), GFP_KERNEL);

	if (!skb_c)
		LIBLCD_MSG("no memory");

	skb_c->skb = skb;
	glue_insert_skbuff(cptr_table, skb_c);

	skb_c->other_ref = skb_ref;

	if (xmit_type == VOLUNTEER_XMIT) {
		skb_c->skbd_ord = skbd_ord;
		skb_c->skb_cptr = skb_cptr;
		skb_c->skbh_cptr = skbd_cptr;
	}

	ret = net_dev_container->net_device.
			netdev_ops->ndo_start_xmit(skb,
			&net_dev_container->net_device);

	if (async_msg_blocking_send_start(net_async, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	thc_ipc_reply(net_async, request_cookie, response);
fail_alloc:
fail_sync:
fail_lookup:
	return ret;
}

// DONE
int ndo_validate_addr_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct net_device_container *net_dev_container;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	int ret;
	cptr_t netdev_ref = __cptr(fipc_get_reg1(request));

	ret = glue_cap_lookup_net_device_type(c_cspace, netdev_ref, &net_dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	LIBLCD_MSG("%s, cptr lcd %lu", __func__, netdev_ref);
	LIBLCD_MSG("%s, looked up cptr lcd %p |  %lu", __func__, net_dev_container, net_dev_container->other_ref.cptr);

	request_cookie = thc_get_request_cookie(request);

	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = net_dev_container->net_device.netdev_ops->ndo_validate_addr(&net_dev_container->net_device);

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	thc_ipc_reply(channel, request_cookie, response);

fail_lookup:
	return ret;
}

// DONE
int ndo_set_rx_mode_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int ret;
	struct net_device_container *net_dev_container;
	struct fipc_message *response;
	unsigned 	int request_cookie;

	ret = glue_cap_lookup_net_device_type(c_cspace, __cptr(fipc_get_reg1(request)), &net_dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	net_dev_container->net_device.netdev_ops->ndo_set_rx_mode(&net_dev_container->net_device);

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}

	thc_ipc_reply(channel, request_cookie, response);

fail_lookup:
	return ret;
}

// DONE
int ndo_set_mac_address_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct fipc_message *response;
	struct net_device_container *net_dev_container;
	unsigned 	int request_cookie;
	int ret;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
	gva_t addr_gva;
	request_cookie = thc_get_request_cookie(request);

	ret = glue_cap_lookup_net_device_type(c_cspace, __cptr(fipc_get_reg1(request)), &net_dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	sync_ret = lcd_cptr_alloc(&addr_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_sync;
	}

	lcd_set_cr0(addr_cptr);
	sync_ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to recv");
		goto fail_sync;
	}
	mem_order = lcd_r0();
	addr_offset = lcd_r1();
	LIBLCD_MSG("%s: cptr %lu | order %lu | offset %lu",
		__func__, addr_cptr.cptr, mem_order, addr_offset);

	sync_ret = lcd_map_virt(addr_cptr, mem_order, &addr_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *addr");
		goto fail_sync;
	}

	ret = net_dev_container->net_device.netdev_ops->ndo_set_mac_address(&net_dev_container->net_device, (void*)(gva_val(addr_gva) + addr_offset));

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}

	fipc_set_reg1(response, ret);
	thc_ipc_reply(channel, request_cookie, response);
fail_lookup:
fail_sync:
	return 0;
}

// DONE
int ndo_get_stats64_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct fipc_message *response;
	unsigned 	int request_cookie;
	struct rtnl_link_stats64 stats;
	int ret;
	struct net_device_container *net_dev_container;
	cptr_t netdev_ref = __cptr(fipc_get_reg1(request));

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = glue_cap_lookup_net_device_type(c_cspace, netdev_ref,
			&net_dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	net_dev_container->net_device.
		netdev_ops->ndo_get_stats64(
			&net_dev_container->net_device,
			&stats);

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}

	fipc_set_reg1(response, stats.tx_packets);
	fipc_set_reg2(response, stats.tx_bytes);

	thc_ipc_reply(channel, request_cookie, response);

fail_lookup:
	return ret;
}

// DONE
int ndo_change_carrier_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct fipc_message *response;
	unsigned 	int request_cookie;
	int ret;
	struct net_device_container *net_dev_container;
	bool new_carrier = fipc_get_reg2(request);
	
	ret = glue_cap_lookup_net_device_type(c_cspace, __cptr(fipc_get_reg1(request)), &net_dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = net_dev_container->net_device.netdev_ops->ndo_change_carrier(&net_dev_container->net_device, new_carrier);

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}

	fipc_set_reg1(response, ret);
	thc_ipc_reply(channel, request_cookie, response);

fail_lookup:
	return ret;
}

// DONE
int setup_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int ret;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	struct net_device_container *net_dev_container;
	struct net_device_ops_container *netdev_ops_container;
	const struct net_device_ops *netdev_ops;
	cptr_t netdev_ref = __cptr(fipc_get_reg1(request));
	cptr_t netdev_ops_ref = __cptr(fipc_get_reg4(request));
	cptr_t netdev_other_ref = __cptr(fipc_get_reg3(request));
	cptr_t pool_cptr;
	gva_t pool_addr;
	unsigned int pool_ord;

	request_cookie = thc_get_request_cookie(request);

	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);

	ret = glue_cap_lookup_net_device_type(c_cspace, netdev_ref, &net_dev_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	// save other ref cptr
	net_dev_container->other_ref = netdev_other_ref;
	LIBLCD_MSG("%s, lcd other ref %p | %lu", __func__, net_dev_container, net_dev_container->other_ref.cptr);

	/* receive shared data pool */
	ret = lcd_cptr_alloc(&pool_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_cptr;
	}

	lcd_set_cr0(pool_cptr);
	printk("%s, calling sync recv", __func__);
	ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);

	pool_ord = lcd_r1();
	ret = lcd_map_virt(pool_cptr, pool_ord, &pool_addr);
	if (ret) {
		LIBLCD_ERR("failed to map pool");
		goto fail_pool;
	}

	LIBLCD_MSG("%s, mapping private pool %p | ord %d", __func__,
			gva_val(pool_addr), pool_ord);

	data_pool = (void*)gva_val(pool_addr);

	g_rtnl_link_ops->setup(&net_dev_container->net_device);

	netdev_ops = net_dev_container->net_device.netdev_ops;

	netdev_ops_container = container_of(netdev_ops, struct net_device_ops_container, net_device_ops);

	netdev_ops_container->other_ref = netdev_ops_ref;

	ret = glue_cap_insert_net_device_ops_type(c_cspace, netdev_ops_container, &netdev_ops_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}

	fipc_set_reg1(response, net_dev_container->net_device.flags);
	fipc_set_reg2(response, net_dev_container->net_device.priv_flags);
	fipc_set_reg3(response, net_dev_container->net_device.features);
	fipc_set_reg4(response, net_dev_container->net_device.hw_features);
	fipc_set_reg5(response, net_dev_container->net_device.hw_enc_features);
	fipc_set_reg6(response, netdev_ops_container->my_ref.cptr);

	thc_ipc_reply(channel, request_cookie, response);
fail_lookup:
fail_insert:
fail_pool:
fail_cptr:
	return ret;
}

// TODO:
int validate_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep)
{
	struct nlattr **tb;
	struct nlattr **data;
	struct fipc_message *response;
	unsigned 	int request_cookie;
	int ret;
	request_cookie = thc_get_request_cookie(request);
	fipc_recv_msg_end(thc_channel_to_fipc(channel), request);
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
	if (async_msg_blocking_send_start(channel, &response)) {
		LIBLCD_ERR("error getting response msg");
		return -EIO;
	}
	fipc_set_reg1(response, ret);
	thc_ipc_reply(channel, request_cookie, response);
	return ret;
}
