#include "../common.h"

void rtnl_lock() {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_send_to_klcd(RPC_RTNL_LOCK, message);
	message->end_slot = message->slots;
}

void rtnl_unlock() {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_send_to_klcd(RPC_RTNL_UNLOCK, message);
	message->end_slot = message->slots;
}

void free_netdev(struct net_device* dev) {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	struct net_device* dev_key = fipc_get_remote(dev);
	fipc_marshal(dev_key);
	if (dev) {
	}
	fipc_destroy_shadow(dev_key);
	fipc_send_to_klcd(RPC_FREE_NETDEV, message);
	message->end_slot = message->slots;
	if (dev) {
	}
}

int eth_validate_addr(struct net_device* dev) {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send_to_klcd(RPC_ETH_VALIDATE_ADDR, message);
	message->end_slot = message->slots;
	if (dev) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

int eth_mac_addr(struct net_device* dev, void* p_void) {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_marshal(p_void);
	if (p_void) {
		struct sockaddr_placeholder* p = p_void;
		fipc_marshal(p->sa_family);
		fipc_marshal(p->sa_data_0);
		fipc_marshal(p->sa_data_1);
		fipc_marshal(p->sa_data_2);
		fipc_marshal(p->sa_data_3);
		fipc_marshal(p->sa_data_4);
		fipc_marshal(p->sa_data_5);
		fipc_marshal(p->sa_data_6);
		fipc_marshal(p->sa_data_7);
		fipc_marshal(p->sa_data_8);
		fipc_marshal(p->sa_data_9);
		fipc_marshal(p->sa_data_10);
		fipc_marshal(p->sa_data_11);
		fipc_marshal(p->sa_data_12);
		fipc_marshal(p->sa_data_13);
	}
	fipc_send_to_klcd(RPC_ETH_MAC_ADDR, message);
	message->end_slot = message->slots;
	if (dev) {
	}
	if (p_void) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

void ether_setup(struct net_device* dev) {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	struct net_device* dev_key = fipc_get_remote(dev);
	fipc_marshal(dev_key);
	if (dev) {
	}
	fipc_send_to_klcd(RPC_ETHER_SETUP, message);
	message->end_slot = message->slots;
	if (dev) {
	}
}

int register_netdevice(struct net_device* dev) {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(dev);
	if (dev) {
		fipc_marshal(dev->flags);
		fipc_marshal(dev->priv_flags);
		fipc_marshal(dev->features);
		fipc_marshal(dev->hw_features);
		fipc_marshal(dev->hw_enc_features);
		fipc_marshal(dev->reg_state);
	}
	fipc_send_to_klcd(RPC_REGISTER_NETDEVICE, message);
	message->end_slot = message->slots;
	if (dev) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

void netif_carrier_off(struct net_device* dev) {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send_to_klcd(RPC_NETIF_CARRIER_OFF, message);
	message->end_slot = message->slots;
	if (dev) {
	}
}

int __rtnl_link_register(struct rtnl_link_ops* ops) {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(ops);
	if (ops) {
		fipc_marshal(ops->setup);
		fipc_marshal(ops->validate);
	}
	fipc_send_to_klcd(RPC___RTNL_LINK_REGISTER, message);
	message->end_slot = message->slots;
	if (ops) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

void __rtnl_link_unregister(struct rtnl_link_ops* ops) {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(ops);
	if (ops) {
	}
	fipc_send_to_klcd(RPC___RTNL_LINK_UNREGISTER, message);
	message->end_slot = message->slots;
	if (ops) {
	}
}

void rtnl_link_unregister(struct rtnl_link_ops* ops) {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(ops);
	if (ops) {
	}
	fipc_send_to_klcd(RPC_RTNL_LINK_UNREGISTER, message);
	message->end_slot = message->slots;
	if (ops) {
	}
}

struct net_device* alloc_netdev_mqs(int sizeof_priv, const char* name, unsigned char name_assign_type, void (*setup)(struct net_device* dev), unsigned int txqs, unsigned int rxqs) {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(sizeof_priv);
	printk("[DEBUG] *real* sizeof_priv: %d\n", sizeof_priv);
	fipc_marshal(name);
	printk("[DEBUG] *real* name ptr: %p\n", name);

	unsigned len = strlen(name);
	printk("[DEBUG] *real* name len: %u\n", len);
	fipc_marshal(len);
	for (unsigned i = 0; i < len; ++i) {
		fipc_marshal(name[i]);
	}	
	
	fipc_marshal(name_assign_type);
	fipc_marshal(setup);
	fipc_marshal(txqs);
	fipc_marshal(rxqs);
	fipc_send_to_klcd(RPC_ALLOC_NETDEV_MQS, message);
	message->end_slot = message->slots;
	if (name) {
	}
	struct net_device* return_value = fipc_unmarshal(struct net_device*);
	if (return_value) {
	}
	return return_value;
}

void netif_carrier_on(struct net_device* dev) {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send_to_klcd(RPC_NETIF_CARRIER_ON, message);
	message->end_slot = message->slots;
	if (dev) {
	}
}

void consume_skb(struct sk_buff* skb) {
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(skb);
	if (skb) {
	}
	fipc_send_to_klcd(RPC_CONSUME_SKB, message);
	message->end_slot = message->slots;
	if (skb) {
	}
}

void _int_1_kernel_nullnet_net_device_ndo_init_callee(struct rpc_message* message)
{
	int (*real_pointer)(struct net_device* dev) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
		struct pcpu_dstats* dev_dstats_key = fipc_unmarshal(struct pcpu_dstats*);
		struct pcpu_dstats* dev_dstats = fipc_get_local(dev_dstats_key);
		dev->dstats = dev_dstats;
		if (dev_dstats) {
		}
	}
	int return_value = real_pointer(dev);
	message->end_slot = message->slots;
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		if (dev_dstats) {
		}
	}
	fipc_marshal(return_value);
}

void _void_1_kernel_nullnet_net_device_ndo_uninit_callee(struct rpc_message* message)
{
	void (*real_pointer)(struct net_device* dev) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
		struct pcpu_dstats* dev_dstats_key = fipc_unmarshal(struct pcpu_dstats*);
		struct pcpu_dstats* dev_dstats = fipc_get_local(dev_dstats_key);
		dev->dstats = dev_dstats;
		if (dev_dstats) {
		}
	}
	real_pointer(dev);
	message->end_slot = message->slots;
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		if (dev_dstats) {
		}
	}
}

void _int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats_callee(struct rpc_message* message)
{
	int (*real_pointer)(struct sk_buff* skb, struct net_device* dev) = fipc_unmarshal(void*);
	struct sk_buff* skb_key = fipc_unmarshal(struct sk_buff*);
	struct sk_buff* skb = fipc_get_local(skb_key);
	if (skb) {
		skb->len = fipc_unmarshal(int);
	}
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	int return_value = real_pointer(skb, dev);
	message->end_slot = message->slots;
	if (skb) {
	}
	if (dev) {
	}
	fipc_marshal(return_value);
}

void _int_1_kernel_nullnet_net_device_ndo_validate_addr_callee(struct rpc_message* message)
{
	int (*real_pointer)(struct net_device* dev) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
		struct sockaddr_placeholder* dev_dev_addr_key = fipc_unmarshal(struct sockaddr_placeholder*);
		struct sockaddr_placeholder* dev_dev_addr = fipc_get_local(dev_dev_addr_key);
		dev->dev_addr = (void*)dev_dev_addr;
		if (dev_dev_addr) {
			dev_dev_addr->sa_family = fipc_unmarshal(unsigned short);
			dev_dev_addr->sa_data_0 = fipc_unmarshal(char);
			dev_dev_addr->sa_data_1 = fipc_unmarshal(char);
			dev_dev_addr->sa_data_2 = fipc_unmarshal(char);
			dev_dev_addr->sa_data_3 = fipc_unmarshal(char);
			dev_dev_addr->sa_data_4 = fipc_unmarshal(char);
			dev_dev_addr->sa_data_5 = fipc_unmarshal(char);
			dev_dev_addr->sa_data_6 = fipc_unmarshal(char);
			dev_dev_addr->sa_data_7 = fipc_unmarshal(char);
			dev_dev_addr->sa_data_8 = fipc_unmarshal(char);
			dev_dev_addr->sa_data_9 = fipc_unmarshal(char);
			dev_dev_addr->sa_data_10 = fipc_unmarshal(char);
			dev_dev_addr->sa_data_11 = fipc_unmarshal(char);
			dev_dev_addr->sa_data_12 = fipc_unmarshal(char);
			dev_dev_addr->sa_data_13 = fipc_unmarshal(char);
		}
	}
	int return_value = real_pointer(dev);
	message->end_slot = message->slots;
	if (dev) {
		struct sockaddr_placeholder* dev_dev_addr = (void*)dev->dev_addr;
		if (dev_dev_addr) {
		}
	}
	fipc_marshal(return_value);
}

void _1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64_callee(struct rpc_message* message)
{
	struct rtnl_link_stats64* (*real_pointer)(struct net_device* dev, struct rtnl_link_stats64* storage) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
		struct pcpu_dstats* dev_dstats_key = fipc_unmarshal(struct pcpu_dstats*);
		struct pcpu_dstats* dev_dstats = fipc_get_local(dev_dstats_key);
		dev->dstats = dev_dstats;
		if (dev_dstats) {
		}
	}
	struct rtnl_link_stats64* storage_key = fipc_unmarshal(struct rtnl_link_stats64*);
	struct rtnl_link_stats64* storage = fipc_get_local(storage_key);
	if (storage) {
		storage->tx_bytes = fipc_unmarshal(unsigned int);
		storage->tx_packets = fipc_unmarshal(unsigned int);
	}
	struct rtnl_link_stats64* return_value = real_pointer(dev, storage);
	message->end_slot = message->slots;
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		if (dev_dstats) {
		}
	}
	if (storage) {
	}
	struct rtnl_link_stats64* return_value_key = fipc_get_remote(return_value);
	fipc_marshal(return_value_key);
	if (return_value) {
	}
}

void _void_1_kernel_nullnet_net_device_ndo_set_rx_mode_callee(struct rpc_message* message)
{
	void (*real_pointer)(struct net_device* dev) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	real_pointer(dev);
	message->end_slot = message->slots;
	if (dev) {
	}
}

void _int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr_callee(struct rpc_message* message)
{
	int (*real_pointer)(struct net_device* dev, struct sockaddr_placeholder* addr) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	struct sockaddr_placeholder* addr_key = fipc_unmarshal(struct sockaddr_placeholder*);
	struct sockaddr_placeholder* addr = fipc_get_local(addr_key);
	if (addr) {
		addr->sa_family = fipc_unmarshal(unsigned short);
		addr->sa_data_0 = fipc_unmarshal(char);
		addr->sa_data_1 = fipc_unmarshal(char);
		addr->sa_data_2 = fipc_unmarshal(char);
		addr->sa_data_3 = fipc_unmarshal(char);
		addr->sa_data_4 = fipc_unmarshal(char);
		addr->sa_data_5 = fipc_unmarshal(char);
		addr->sa_data_6 = fipc_unmarshal(char);
		addr->sa_data_7 = fipc_unmarshal(char);
		addr->sa_data_8 = fipc_unmarshal(char);
		addr->sa_data_9 = fipc_unmarshal(char);
		addr->sa_data_10 = fipc_unmarshal(char);
		addr->sa_data_11 = fipc_unmarshal(char);
		addr->sa_data_12 = fipc_unmarshal(char);
		addr->sa_data_13 = fipc_unmarshal(char);
	}
	int return_value = real_pointer(dev, addr);
	message->end_slot = message->slots;
	if (dev) {
	}
	if (addr) {
	}
	fipc_marshal(return_value);
}

void _int_1_kernel_nullnet_net_device_ndo_change_carrier_bool_callee(struct rpc_message* message)
{
	int (*real_pointer)(struct net_device* dev, bool new_carrier) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	bool new_carrier = fipc_unmarshal(bool);
	int return_value = real_pointer(dev, new_carrier);
	message->end_slot = message->slots;
	if (dev) {
	}
	fipc_marshal(return_value);
}

void _void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo_callee(struct rpc_message* message)
{
	void (*real_pointer)(struct net_device* dev, struct ethtool_drvinfo* info) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	struct ethtool_drvinfo* info_key = fipc_unmarshal(struct ethtool_drvinfo*);
	struct ethtool_drvinfo* info = fipc_get_local(info_key);
	if (info) {
	}
	real_pointer(dev, info);
	message->end_slot = message->slots;
	if (dev) {
	}
	if (info) {
	}
}

void _void_1_kernel_nullnet_net_device_destructor_callee(struct rpc_message* message)
{
	void (*real_pointer)(struct net_device* dev) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	real_pointer(dev);
	message->end_slot = message->slots;
	if (dev) {
	}
}

void _void_1_kernel_nullnet_net_device_setup_callee(struct rpc_message* message)
{
	void (*real_pointer)(struct net_device* dev) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
		dev->flags = fipc_unmarshal(unsigned int);
		dev->priv_flags = fipc_unmarshal(unsigned int);
		dev->features = fipc_unmarshal(unsigned long long);
		dev->hw_features = fipc_unmarshal(unsigned long long);
		dev->hw_enc_features = fipc_unmarshal(unsigned long long);
	}
	real_pointer(dev);
	message->end_slot = message->slots;
	if (dev) {
		struct net_device_ops* dev_netdev_ops = dev->netdev_ops;
		if (dev_netdev_ops) {
		}
		struct ethtool_ops* dev_ethtool_ops = dev->ethtool_ops;
		if (dev_ethtool_ops) {
		}
		fipc_marshal(dev->destructor);
	}
}

void _int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr_callee(struct rpc_message* message)
{
	// EMPTY
}

void _void_1_kernel_nullnet_net_device_setup_callee_alloc_callee(struct rpc_message* message)
{
	void (*real_pointer)(struct net_device* dev) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_create_shadow(dev_key);
	if (dev) {
		dev->flags = fipc_unmarshal(unsigned int);
		dev->priv_flags = fipc_unmarshal(unsigned int);
		dev->features = fipc_unmarshal(unsigned long long);
		dev->hw_features = fipc_unmarshal(unsigned long long);
		dev->hw_enc_features = fipc_unmarshal(unsigned long long);
	}
	real_pointer(dev);
	message->end_slot = message->slots;
	if (dev) {
		struct net_device_ops* dev_netdev_ops = dev->netdev_ops;
		if (dev_netdev_ops) {
		}
		struct ethtool_ops* dev_ethtool_ops = dev->ethtool_ops;
		if (dev_ethtool_ops) {
		}
		fipc_marshal(dev->destructor);
	}
}

int trampoline_impl_int_1_kernel_nullnet_net_device_ndo_init(void* real_pointer, struct net_device* dev)
{
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		fipc_marshal(dev_dstats);
		if (dev_dstats) {
		}
	}
	fipc_send_to_klcd(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_INIT, message);
	message->end_slot = message->slots;
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		if (dev_dstats) {
		}
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

void trampoline_impl_void_1_kernel_nullnet_net_device_ndo_uninit(void* real_pointer, struct net_device* dev)
{
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		fipc_marshal(dev_dstats);
		if (dev_dstats) {
		}
	}
	fipc_send_to_klcd(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_UNINIT, message);
	message->end_slot = message->slots;
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		if (dev_dstats) {
		}
	}
}

int trampoline_impl_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats(void* real_pointer, struct sk_buff* skb, struct net_device* dev)
{
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(skb);
	if (skb) {
		fipc_marshal(skb->len);
	}
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send_to_klcd(RPC_PTR_INT_1_KERNEL_NULLNET_SK_BUFF_NDO_START_XMIT_1_KERNEL_NULLNET_NET_DEVICE_STATS, message);
	message->end_slot = message->slots;
	if (skb) {
	}
	if (dev) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

int trampoline_impl_int_1_kernel_nullnet_net_device_ndo_validate_addr(void* real_pointer, struct net_device* dev)
{
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
		struct sockaddr_placeholder* dev_dev_addr = (void*)dev->dev_addr;
		fipc_marshal(dev_dev_addr);
		if (dev_dev_addr) {
			fipc_marshal(dev_dev_addr->sa_family);
			fipc_marshal(dev_dev_addr->sa_data_0);
			fipc_marshal(dev_dev_addr->sa_data_1);
			fipc_marshal(dev_dev_addr->sa_data_2);
			fipc_marshal(dev_dev_addr->sa_data_3);
			fipc_marshal(dev_dev_addr->sa_data_4);
			fipc_marshal(dev_dev_addr->sa_data_5);
			fipc_marshal(dev_dev_addr->sa_data_6);
			fipc_marshal(dev_dev_addr->sa_data_7);
			fipc_marshal(dev_dev_addr->sa_data_8);
			fipc_marshal(dev_dev_addr->sa_data_9);
			fipc_marshal(dev_dev_addr->sa_data_10);
			fipc_marshal(dev_dev_addr->sa_data_11);
			fipc_marshal(dev_dev_addr->sa_data_12);
			fipc_marshal(dev_dev_addr->sa_data_13);
		}
	}
	fipc_send_to_klcd(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_VALIDATE_ADDR, message);
	message->end_slot = message->slots;
	if (dev) {
		struct sockaddr_placeholder* dev_dev_addr = (void*)dev->dev_addr;
		if (dev_dev_addr) {
		}
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

struct rtnl_link_stats64* trampoline_impl_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64(void* real_pointer, struct net_device* dev, struct rtnl_link_stats64* storage)
{
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		fipc_marshal(dev_dstats);
		if (dev_dstats) {
		}
	}
	fipc_marshal(storage);
	if (storage) {
		fipc_marshal(storage->tx_bytes);
		fipc_marshal(storage->tx_packets);
	}
	fipc_send_to_klcd(RPC_PTR_1_KERNEL_NULLNET_RTNL_LINK_STATS64_1_KERNEL_NULLNET_NET_DEVICE_NDO_GET_STATS64_1_KERNEL_NULLNET_RTNL_LINK_STATS64_NDO_GET_STATS64, message);
	message->end_slot = message->slots;
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		if (dev_dstats) {
		}
	}
	if (storage) {
	}
	struct rtnl_link_stats64* return_value = fipc_unmarshal(struct rtnl_link_stats64*);
	if (return_value) {
	}
	return return_value;
}

void trampoline_impl_void_1_kernel_nullnet_net_device_ndo_set_rx_mode(void* real_pointer, struct net_device* dev)
{
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send_to_klcd(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_RX_MODE, message);
	message->end_slot = message->slots;
	if (dev) {
	}
}

int trampoline_impl_int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr(void* real_pointer, struct net_device* dev, struct sockaddr_placeholder* addr)
{
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_marshal(addr);
	if (addr) {
		fipc_marshal(addr->sa_family);
		fipc_marshal(addr->sa_data_0);
		fipc_marshal(addr->sa_data_1);
		fipc_marshal(addr->sa_data_2);
		fipc_marshal(addr->sa_data_3);
		fipc_marshal(addr->sa_data_4);
		fipc_marshal(addr->sa_data_5);
		fipc_marshal(addr->sa_data_6);
		fipc_marshal(addr->sa_data_7);
		fipc_marshal(addr->sa_data_8);
		fipc_marshal(addr->sa_data_9);
		fipc_marshal(addr->sa_data_10);
		fipc_marshal(addr->sa_data_11);
		fipc_marshal(addr->sa_data_12);
		fipc_marshal(addr->sa_data_13);
	}
	fipc_send_to_klcd(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_MAC_ADDRESS_1_KERNEL_NULLNET_SOCKADDR, message);
	message->end_slot = message->slots;
	if (dev) {
	}
	if (addr) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

int trampoline_impl_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool(void* real_pointer, struct net_device* dev, bool new_carrier)
{
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_marshal(new_carrier);
	fipc_send_to_klcd(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_CHANGE_CARRIER_BOOL, message);
	message->end_slot = message->slots;
	if (dev) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

void trampoline_impl_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo(void* real_pointer, struct net_device* dev, struct ethtool_drvinfo* info)
{
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_marshal(info);
	if (info) {
	}
	fipc_send_to_klcd(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_GET_DRVINFO_1_KERNEL_NULLNET_ETHTOOL_DRVINFO_GET_DRVINFO, message);
	message->end_slot = message->slots;
	if (dev) {
	}
	if (info) {
	}
}

void trampoline_impl_void_1_kernel_nullnet_net_device_destructor(void* real_pointer, struct net_device* dev)
{
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send_to_klcd(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_DESTRUCTOR, message);
	message->end_slot = message->slots;
	if (dev) {
	}
}

void trampoline_impl_void_1_kernel_nullnet_net_device_setup(void* real_pointer, struct net_device* dev)
{
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
		fipc_marshal(dev->flags);
		fipc_marshal(dev->priv_flags);
		fipc_marshal(dev->features);
		fipc_marshal(dev->hw_features);
		fipc_marshal(dev->hw_enc_features);
	}
	fipc_send_to_klcd(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP, message);
	message->end_slot = message->slots;
	if (dev) {
		struct net_device_ops* dev_netdev_ops = dev->netdev_ops;
		if (dev_netdev_ops) {
		}
		struct ethtool_ops* dev_ethtool_ops = dev->ethtool_ops;
		if (dev_ethtool_ops) {
		}
		void (*dev_destructor)(struct net_device* dev) = fipc_unmarshal(void*);
		void (*dev_destructor_trampoline)(struct net_device* dev) = inject_trampoline(_void_1_kernel_nullnet_net_device_destructor, dev_destructor);
		dev->destructor = dev_destructor_trampoline;
	}
}

int trampoline_impl_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr(void* real_pointer, struct nlattr** tb, struct nlattr** data)
{
	printk("validate() was called: %p\n", real_pointer);
	return 1;
}

void trampoline_impl_void_1_kernel_nullnet_net_device_setup_callee_alloc(void* real_pointer, struct net_device* dev)
{
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
		fipc_marshal(dev->flags);
		fipc_marshal(dev->priv_flags);
		fipc_marshal(dev->features);
		fipc_marshal(dev->hw_features);
		fipc_marshal(dev->hw_enc_features);
	}
	fipc_send_to_klcd(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP_CALLEE_ALLOC, message);
	message->end_slot = message->slots;
	if (dev) {
		struct net_device_ops* dev_netdev_ops = dev->netdev_ops;
		if (dev_netdev_ops) {
		}
		struct ethtool_ops* dev_ethtool_ops = dev->ethtool_ops;
		if (dev_ethtool_ops) {
		}
		void (*dev_destructor)(struct net_device* dev) = fipc_unmarshal(void*);
		void (*dev_destructor_trampoline)(struct net_device* dev) = inject_trampoline(_void_1_kernel_nullnet_net_device_destructor, dev_destructor);
		dev->destructor = dev_destructor_trampoline;
	}
}

LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_net_device_ndo_init)
int trampoline_int_1_kernel_nullnet_net_device_ndo_init(struct net_device* dev)
{
	struct trampoline_args* args;
	LCD_TRAMPOLINE_PROLOGUE(args, trampoline_int_1_kernel_nullnet_net_device_ndo_init);
	return ((typeof(trampoline_impl_int_1_kernel_nullnet_net_device_ndo_init)*)(args->impl))(args->real_ptr, dev);
}

LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_ndo_uninit)
void trampoline_void_1_kernel_nullnet_net_device_ndo_uninit(struct net_device* dev)
{
	struct trampoline_args* args;
	LCD_TRAMPOLINE_PROLOGUE(args, trampoline_void_1_kernel_nullnet_net_device_ndo_uninit);
	((typeof(trampoline_impl_void_1_kernel_nullnet_net_device_ndo_uninit)*)(args->impl))(args->real_ptr, dev);
}

LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats)
int trampoline_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats(struct sk_buff* skb, struct net_device* dev)
{
	struct trampoline_args* args;
	LCD_TRAMPOLINE_PROLOGUE(args, trampoline_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats);
	return ((typeof(trampoline_impl_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats)*)(args->impl))(args->real_ptr, skb, dev);
}

LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_net_device_ndo_validate_addr)
int trampoline_int_1_kernel_nullnet_net_device_ndo_validate_addr(struct net_device* dev)
{
	struct trampoline_args* args;
	LCD_TRAMPOLINE_PROLOGUE(args, trampoline_int_1_kernel_nullnet_net_device_ndo_validate_addr);
	return ((typeof(trampoline_impl_int_1_kernel_nullnet_net_device_ndo_validate_addr)*)(args->impl))(args->real_ptr, dev);
}

LCD_TRAMPOLINE_LINKAGE(trampoline_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64)
struct rtnl_link_stats64* trampoline_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64(struct net_device* dev, struct rtnl_link_stats64* storage)
{
	struct trampoline_args* args;
	LCD_TRAMPOLINE_PROLOGUE(args, trampoline_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64);
	return ((typeof(trampoline_impl_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64)*)(args->impl))(args->real_ptr, dev, storage);
}

LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_ndo_set_rx_mode)
void trampoline_void_1_kernel_nullnet_net_device_ndo_set_rx_mode(struct net_device* dev)
{
	struct trampoline_args* args;
	LCD_TRAMPOLINE_PROLOGUE(args, trampoline_void_1_kernel_nullnet_net_device_ndo_set_rx_mode);
	((typeof(trampoline_impl_void_1_kernel_nullnet_net_device_ndo_set_rx_mode)*)(args->impl))(args->real_ptr, dev);
}

LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr)
int trampoline_int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr(struct net_device* dev, struct sockaddr_placeholder* addr)
{
	struct trampoline_args* args;
	LCD_TRAMPOLINE_PROLOGUE(args, trampoline_int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr);
	return ((typeof(trampoline_impl_int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr)*)(args->impl))(args->real_ptr, dev, addr);
}

LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool)
int trampoline_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool(struct net_device* dev, bool new_carrier)
{
	struct trampoline_args* args;
	LCD_TRAMPOLINE_PROLOGUE(args, trampoline_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool);
	return ((typeof(trampoline_impl_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool)*)(args->impl))(args->real_ptr, dev, new_carrier);
}

LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo)
void trampoline_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo(struct net_device* dev, struct ethtool_drvinfo* info)
{
	struct trampoline_args* args;
	LCD_TRAMPOLINE_PROLOGUE(args, trampoline_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo);
	((typeof(trampoline_impl_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo)*)(args->impl))(args->real_ptr, dev, info);
}

LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_destructor)
void trampoline_void_1_kernel_nullnet_net_device_destructor(struct net_device* dev)
{
	struct trampoline_args* args;
	LCD_TRAMPOLINE_PROLOGUE(args, trampoline_void_1_kernel_nullnet_net_device_destructor);
	((typeof(trampoline_impl_void_1_kernel_nullnet_net_device_destructor)*)(args->impl))(args->real_ptr, dev);
}

LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_setup)
void trampoline_void_1_kernel_nullnet_net_device_setup(struct net_device* dev)
{
	struct trampoline_args* args;
	LCD_TRAMPOLINE_PROLOGUE(args, trampoline_void_1_kernel_nullnet_net_device_setup);
	((typeof(trampoline_impl_void_1_kernel_nullnet_net_device_setup)*)(args->impl))(args->real_ptr, dev);
}

LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr)
int trampoline_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr(struct nlattr** tb, struct nlattr** data)
{
	struct trampoline_args* args;
	LCD_TRAMPOLINE_PROLOGUE(args, trampoline_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr);
	return ((typeof(trampoline_impl_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr)*)(args->impl))(args->real_ptr, tb, data);
}

LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_setup_callee_alloc)
void trampoline_void_1_kernel_nullnet_net_device_setup_callee_alloc(struct net_device* dev)
{
	struct trampoline_args* args;
	LCD_TRAMPOLINE_PROLOGUE(args, trampoline_void_1_kernel_nullnet_net_device_setup_callee_alloc);
	((typeof(trampoline_impl_void_1_kernel_nullnet_net_device_setup_callee_alloc)*)(args->impl))(args->real_ptr, dev);
}

int handle_rpc_calls(struct fipc_message* received) {
	enum dispatch_id rpc;
	struct rpc_message message_buffer;
	struct rpc_message* message = &message_buffer;
	fipc_translate(received, &rpc, message);
	switch (rpc) {
	case RPC_LCD_INIT:
		lcd_trace(RPC_LCD_INIT);
		__dummy_lcd_init();
		break;

	case RPC_LCD_EXIT:
		lcd_trace(RPC_LCD_EXIT);
		__dummy_lcd_exit();
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_INIT:
		lcd_trace(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_INIT);
		_int_1_kernel_nullnet_net_device_ndo_init_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_UNINIT:
		lcd_trace(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_UNINIT);
		_void_1_kernel_nullnet_net_device_ndo_uninit_callee(message);
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_SK_BUFF_NDO_START_XMIT_1_KERNEL_NULLNET_NET_DEVICE_STATS:
		lcd_trace(RPC_PTR_INT_1_KERNEL_NULLNET_SK_BUFF_NDO_START_XMIT_1_KERNEL_NULLNET_NET_DEVICE_STATS);
		_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats_callee(message);
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_VALIDATE_ADDR:
		lcd_trace(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_VALIDATE_ADDR);
		_int_1_kernel_nullnet_net_device_ndo_validate_addr_callee(message);
		break;

	case RPC_PTR_1_KERNEL_NULLNET_RTNL_LINK_STATS64_1_KERNEL_NULLNET_NET_DEVICE_NDO_GET_STATS64_1_KERNEL_NULLNET_RTNL_LINK_STATS64_NDO_GET_STATS64:
		lcd_trace(RPC_PTR_1_KERNEL_NULLNET_RTNL_LINK_STATS64_1_KERNEL_NULLNET_NET_DEVICE_NDO_GET_STATS64_1_KERNEL_NULLNET_RTNL_LINK_STATS64_NDO_GET_STATS64);
		_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_RX_MODE:
		lcd_trace(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_RX_MODE);
		_void_1_kernel_nullnet_net_device_ndo_set_rx_mode_callee(message);
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_MAC_ADDRESS_1_KERNEL_NULLNET_SOCKADDR:
		lcd_trace(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_MAC_ADDRESS_1_KERNEL_NULLNET_SOCKADDR);
		_int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr_callee(message);
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_CHANGE_CARRIER_BOOL:
		lcd_trace(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_CHANGE_CARRIER_BOOL);
		_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_GET_DRVINFO_1_KERNEL_NULLNET_ETHTOOL_DRVINFO_GET_DRVINFO:
		lcd_trace(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_GET_DRVINFO_1_KERNEL_NULLNET_ETHTOOL_DRVINFO_GET_DRVINFO);
		_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_DESTRUCTOR:
		lcd_trace(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_DESTRUCTOR);
		_void_1_kernel_nullnet_net_device_destructor_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP:
		lcd_trace(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP);
		_void_1_kernel_nullnet_net_device_setup_callee(message);
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_VOID_PTR_1_KERNEL_NULLNET_VOID_PTR:
		lcd_trace(RPC_PTR_INT_1_KERNEL_NULLNET_VOID_PTR_1_KERNEL_NULLNET_VOID_PTR);
		_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP_CALLEE_ALLOC:
		lcd_trace(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP_CALLEE_ALLOC);
		_void_1_kernel_nullnet_net_device_setup_callee_alloc_callee(message);
		break;

	default:
		break;
	}

	fipc_pack(received, rpc, message);

	return 0;
}

