#include "../common.h"

void rtnl_lock_callee(struct rpc_message* message) {
	rtnl_lock();
	message->end_slot = message->slots;
}

void rtnl_unlock_callee(struct rpc_message* message) {
	rtnl_unlock();
	message->end_slot = message->slots;
}

void free_netdev_callee(struct rpc_message* message) {
	struct net_device* dev = fipc_unmarshal(struct net_device*);
	if (dev) {
	}
	free_netdev(dev);
	message->end_slot = message->slots;
	if (dev) {
	}
}

void eth_validate_addr_callee(struct rpc_message* message) {
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	int return_value = eth_validate_addr(dev);
	message->end_slot = message->slots;
	if (dev) {
	}
	fipc_marshal(return_value);
}

void eth_mac_addr_callee(struct rpc_message* message) {
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	struct sockaddr_placeholder* p_key = (void*)fipc_unmarshal(struct sockaddr_placeholder*);
	struct sockaddr_placeholder* p = fipc_get_local(p_key);
	if (p) {
		p->sa_family = fipc_unmarshal(unsigned short);
		p->sa_data_0 = fipc_unmarshal(char);
		p->sa_data_1 = fipc_unmarshal(char);
		p->sa_data_2 = fipc_unmarshal(char);
		p->sa_data_3 = fipc_unmarshal(char);
		p->sa_data_4 = fipc_unmarshal(char);
		p->sa_data_5 = fipc_unmarshal(char);
		p->sa_data_6 = fipc_unmarshal(char);
		p->sa_data_7 = fipc_unmarshal(char);
		p->sa_data_8 = fipc_unmarshal(char);
		p->sa_data_9 = fipc_unmarshal(char);
		p->sa_data_10 = fipc_unmarshal(char);
		p->sa_data_11 = fipc_unmarshal(char);
		p->sa_data_12 = fipc_unmarshal(char);
		p->sa_data_13 = fipc_unmarshal(char);
	}
	int return_value = eth_mac_addr(dev, p);
	message->end_slot = message->slots;
	if (dev) {
	}
	if (p) {
	}
	fipc_marshal(return_value);
}

void ether_setup_callee(struct rpc_message* message) {
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	ether_setup(dev);
	message->end_slot = message->slots;
	if (dev) {
	}
}

void register_netdevice_callee(struct rpc_message* message) {
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
		dev->flags = fipc_unmarshal(unsigned int);
		dev->priv_flags = fipc_unmarshal(unsigned int);
		dev->features = fipc_unmarshal(unsigned long long);
		dev->hw_features = fipc_unmarshal(unsigned long long);
		dev->hw_enc_features = fipc_unmarshal(unsigned long long);
		dev->reg_state = fipc_unmarshal(int);
	}
	int return_value = register_netdevice(dev);
	message->end_slot = message->slots;
	if (dev) {
	}
	fipc_marshal(return_value);
}

void netif_carrier_off_callee(struct rpc_message* message) {
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	netif_carrier_off(dev);
	message->end_slot = message->slots;
	if (dev) {
	}
}

void __rtnl_link_register_callee(struct rpc_message* message) {
	struct rtnl_link_ops* ops_key = fipc_unmarshal(struct rtnl_link_ops*);
	struct rtnl_link_ops* ops = fipc_create_shadow(ops_key);
	if (ops) {
		void (*ops_setup)(struct net_device* dev) = fipc_unmarshal(void*);
		void (*ops_setup_trampoline)(struct net_device* dev) = inject_trampoline(_void_1_kernel_nullnet_net_device_setup, ops_setup);
		ops->setup = ops_setup_trampoline;
		int (*ops_validate)(struct nlattr** tb, struct nlattr** data) = fipc_unmarshal(void*);
		int (*ops_validate_trampoline)(struct nlattr** tb, struct nlattr** data) = inject_trampoline(_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr, ops_validate);
		ops->validate = ops_validate_trampoline;

		// CUSTOM
		ops->kind = "dummy";
	}
	int return_value = __rtnl_link_register(ops);
	printk("[DEBUG] rtnl register return value was: %d", return_value);
	message->end_slot = message->slots;
	if (ops) {
	}
	fipc_marshal(return_value);
}

void __rtnl_link_unregister_callee(struct rpc_message* message) {
	struct rtnl_link_ops* ops_key = fipc_unmarshal(struct rtnl_link_ops*);
	struct rtnl_link_ops* ops = fipc_get_local(ops_key);
	if (ops) {
	}
	__rtnl_link_unregister(ops);
	message->end_slot = message->slots;
	if (ops) {
	}
	fipc_destroy_shadow(ops_key);
}

void rtnl_link_unregister_callee(struct rpc_message* message) {
	struct rtnl_link_ops* ops_key = fipc_unmarshal(struct rtnl_link_ops*);
	struct rtnl_link_ops* ops = fipc_get_local(ops_key);
	if (ops) {
	}
	rtnl_link_unregister(ops);
	message->end_slot = message->slots;
	if (ops) {
	}
	fipc_destroy_shadow(ops_key);
}

void alloc_netdev_mqs_callee(struct rpc_message* message) {
	int sizeof_priv = fipc_unmarshal(int);

	char* name_key = fipc_unmarshal(char*);
	unsigned long len = fipc_unmarshal(unsigned long);
	char* name = fipc_create_shadow_impl(name_key, len + 1);
	name[len] = '\0';
	for (unsigned i = 0; i < len; ++i) {
		name[i] = fipc_unmarshal(char);
	}

	unsigned char name_assign_type = fipc_unmarshal(unsigned char);
	void (*setup_remote)(struct net_device* dev) = fipc_unmarshal(void*);
	void (*setup)(struct net_device* dev) = inject_trampoline(_void_1_kernel_nullnet_net_device_setup_callee_alloc, setup_remote);
	unsigned int txqs = fipc_unmarshal(unsigned int);
	unsigned int rxqs = fipc_unmarshal(unsigned int);
	struct net_device* return_value = alloc_netdev_mqs(sizeof_priv, name, name_assign_type, setup, txqs, rxqs);
	message->end_slot = message->slots;
	if (name) {
	}
	struct net_device* return_value_key = fipc_get_remote(return_value);
	fipc_marshal(return_value_key);
	if (return_value) {
	}
}

void netif_carrier_on_callee(struct rpc_message* message) {
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	netif_carrier_on(dev);
	message->end_slot = message->slots;
	if (dev) {
	}
}

void consume_skb_callee(struct rpc_message* message) {
	struct sk_buff* skb_key = fipc_unmarshal(struct sk_buff*);
	struct sk_buff* skb = fipc_get_local(skb_key);
	if (skb) {
	}
	consume_skb(skb);
	message->end_slot = message->slots;
	if (skb) {
	}
}

void _int_1_kernel_nullnet_net_device_ndo_init_callee(struct rpc_message* message) {
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

void _void_1_kernel_nullnet_net_device_ndo_uninit_callee(struct rpc_message* message) {
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

void _int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats_callee(struct rpc_message* message) {
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

void _int_1_kernel_nullnet_net_device_ndo_validate_addr_callee(struct rpc_message* message) {
	int (*real_pointer)(struct net_device* dev) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
		struct sockaddr_placeholder* dev_dev_addr_key = (void*)fipc_unmarshal(struct sockaddr_placeholder*);
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

void _1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64_callee(struct rpc_message* message) {
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

void _void_1_kernel_nullnet_net_device_ndo_set_rx_mode_callee(struct rpc_message* message) {
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

void _int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr_callee(struct rpc_message* message) {
	int (*real_pointer)(struct net_device* dev, struct sockaddr_placeholder* addr) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	struct sockaddr_placeholder* addr_key = (void*)fipc_unmarshal(struct sockaddr_placeholder*);
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

void _int_1_kernel_nullnet_net_device_ndo_change_carrier_bool_callee(struct rpc_message* message) {
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

void _void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo_callee(struct rpc_message* message) {
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

void _void_1_kernel_nullnet_net_device_destructor_callee(struct rpc_message* message) {
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

void _void_1_kernel_nullnet_net_device_setup_callee(struct rpc_message* message) {
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

void _int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr_callee(struct rpc_message* message) {
	// EMPTY
}

void _void_1_kernel_nullnet_net_device_setup_callee_alloc_callee(struct rpc_message* message) {
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

LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_net_device_ndo_init)
int trampoline_int_1_kernel_nullnet_net_device_ndo_init(struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_int_1_kernel_nullnet_net_device_ndo_init);
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
	fipc_send(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_INIT, message);
	message->end_slot = message->slots;
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		if (dev_dstats) {
		}
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_ndo_uninit)
void trampoline_void_1_kernel_nullnet_net_device_ndo_uninit(struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_void_1_kernel_nullnet_net_device_ndo_uninit);
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
	fipc_send(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_UNINIT, message);
	message->end_slot = message->slots;
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		if (dev_dstats) {
		}
	}
}

LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats)
int trampoline_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats(struct sk_buff* skb, struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats);
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
	fipc_send(RPC_PTR_INT_1_KERNEL_NULLNET_SK_BUFF_NDO_START_XMIT_1_KERNEL_NULLNET_NET_DEVICE_STATS, message);
	message->end_slot = message->slots;
	if (skb) {
	}
	if (dev) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_net_device_ndo_validate_addr)
int trampoline_int_1_kernel_nullnet_net_device_ndo_validate_addr(struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_int_1_kernel_nullnet_net_device_ndo_validate_addr);
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
	fipc_send(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_VALIDATE_ADDR, message);
	message->end_slot = message->slots;
	if (dev) {
		struct sockaddr_placeholder* dev_dev_addr = (void*)dev->dev_addr;
		if (dev_dev_addr) {
		}
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

LCD_TRAMPOLINE_LINKAGE(trampoline_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64)
struct rtnl_link_stats64* trampoline_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64(struct net_device* dev, struct rtnl_link_stats64* storage) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64);
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
	fipc_send(RPC_PTR_1_KERNEL_NULLNET_RTNL_LINK_STATS64_1_KERNEL_NULLNET_NET_DEVICE_NDO_GET_STATS64_1_KERNEL_NULLNET_RTNL_LINK_STATS64_NDO_GET_STATS64, message);
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

LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_ndo_set_rx_mode)
void trampoline_void_1_kernel_nullnet_net_device_ndo_set_rx_mode(struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_void_1_kernel_nullnet_net_device_ndo_set_rx_mode);
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_RX_MODE, message);
	message->end_slot = message->slots;
	if (dev) {
	}
}

LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr)
int trampoline_int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr(struct net_device* dev, struct sockaddr_placeholder* addr) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr);
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
	fipc_send(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_MAC_ADDRESS_1_KERNEL_NULLNET_SOCKADDR, message);
	message->end_slot = message->slots;
	if (dev) {
	}
	if (addr) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool)
int trampoline_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool(struct net_device* dev, bool new_carrier) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool);
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_marshal(new_carrier);
	fipc_send(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_CHANGE_CARRIER_BOOL, message);
	message->end_slot = message->slots;
	if (dev) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo)
void trampoline_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo(struct net_device* dev, struct ethtool_drvinfo* info) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo);
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
	fipc_send(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_GET_DRVINFO_1_KERNEL_NULLNET_ETHTOOL_DRVINFO_GET_DRVINFO, message);
	message->end_slot = message->slots;
	if (dev) {
	}
	if (info) {
	}
}

LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_destructor)
void trampoline_void_1_kernel_nullnet_net_device_destructor(struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_void_1_kernel_nullnet_net_device_destructor);
	struct rpc_message message_buffer = {0};
	message_buffer.end_slot = message_buffer.slots;
	struct rpc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_DESTRUCTOR, message);
	message->end_slot = message->slots;
	if (dev) {
	}
}

LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_setup)
void trampoline_void_1_kernel_nullnet_net_device_setup(struct net_device* dev) {
	LIBLCD_MSG("Trampoline #1 is being called");

	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_void_1_kernel_nullnet_net_device_setup);
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
	fipc_send(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP, message);
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

LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr)
int trampoline_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr(struct nlattr** tb, struct nlattr** data) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr);
	printk("validate() was called: %p\n", real_pointer);
	return 1;
}

LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_setup_callee_alloc)
void trampoline_void_1_kernel_nullnet_net_device_setup_callee_alloc(struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_void_1_kernel_nullnet_net_device_setup_callee_alloc);
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
	fipc_send(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP_CALLEE_ALLOC, message);
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

int dispatch(struct fipc_message* received) {
	enum dispatch_id rpc;
	struct rpc_message message_buffer;
	struct rpc_message* message = &message_buffer;
	LIBLCD_MSG("Started dispatching KLCD message");
	fipc_translate(received, &rpc, message);
	switch (rpc) {
	case RPC_RTNL_LOCK:
		klcd_trace(RPC_RTNL_LOCK);
		rtnl_lock_callee(message);
		break;

	case RPC_RTNL_UNLOCK:
		klcd_trace(RPC_RTNL_UNLOCK);
		rtnl_unlock_callee(message);
		break;
		
	case RPC_FREE_NETDEV:
		klcd_trace(RPC_FREE_NETDEV);
		free_netdev_callee(message);
		break;

	case RPC_ETH_VALIDATE_ADDR:
		klcd_trace(RPC_ETH_VALIDATE_ADDR);
		eth_validate_addr_callee(message);
		break;

	case RPC_ETH_MAC_ADDR:
		klcd_trace(RPC_ETH_MAC_ADDR);
		eth_mac_addr_callee(message);
		break;

	case RPC_ETHER_SETUP:
		klcd_trace(RPC_ETHER_SETUP);
		ether_setup_callee(message);
		break;

	case RPC_REGISTER_NETDEVICE:
		klcd_trace(RPC_REGISTER_NETDEVICE);
		register_netdevice_callee(message);
		break;

	case RPC_NETIF_CARRIER_OFF:
		klcd_trace(RPC_NETIF_CARRIER_OFF);
		netif_carrier_off_callee(message);
		break;

	case RPC___RTNL_LINK_REGISTER:
		klcd_trace(RPC___RTNL_LINK_REGISTER);
		__rtnl_link_register_callee(message);
		break;

	case RPC___RTNL_LINK_UNREGISTER:
		klcd_trace(RPC___RTNL_LINK_UNREGISTER);
		__rtnl_link_unregister_callee(message);
		break;

	case RPC_RTNL_LINK_UNREGISTER:
		klcd_trace(RPC_RTNL_LINK_UNREGISTER);
		rtnl_link_unregister_callee(message);
		break;

	case RPC_ALLOC_NETDEV_MQS:
		klcd_trace(RPC_ALLOC_NETDEV_MQS);
		alloc_netdev_mqs_callee(message);
		break;

	case RPC_NETIF_CARRIER_ON:
		klcd_trace(RPC_NETIF_CARRIER_ON);
		netif_carrier_on_callee(message);
		break;

	case RPC_CONSUME_SKB:
		klcd_trace(RPC_CONSUME_SKB);
		consume_skb_callee(message);
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_INIT:
		_int_1_kernel_nullnet_net_device_ndo_init_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_UNINIT:
		_void_1_kernel_nullnet_net_device_ndo_uninit_callee(message);
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_SK_BUFF_NDO_START_XMIT_1_KERNEL_NULLNET_NET_DEVICE_STATS:
		_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats_callee(message);
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_VALIDATE_ADDR:
		_int_1_kernel_nullnet_net_device_ndo_validate_addr_callee(message);
		break;

	case RPC_PTR_1_KERNEL_NULLNET_RTNL_LINK_STATS64_1_KERNEL_NULLNET_NET_DEVICE_NDO_GET_STATS64_1_KERNEL_NULLNET_RTNL_LINK_STATS64_NDO_GET_STATS64:
		_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_RX_MODE:
		_void_1_kernel_nullnet_net_device_ndo_set_rx_mode_callee(message);
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_MAC_ADDRESS_1_KERNEL_NULLNET_SOCKADDR:
		_int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr_callee(message);
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_CHANGE_CARRIER_BOOL:
		_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_GET_DRVINFO_1_KERNEL_NULLNET_ETHTOOL_DRVINFO_GET_DRVINFO:
		_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_DESTRUCTOR:
		_void_1_kernel_nullnet_net_device_destructor_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP:
		_void_1_kernel_nullnet_net_device_setup_callee(message);
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_VOID_PTR_1_KERNEL_NULLNET_VOID_PTR:
		_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP_CALLEE_ALLOC:
		_void_1_kernel_nullnet_net_device_setup_callee_alloc_callee(message);
		break;

	default:
		LIBLCD_ERR("Unknown message kind (KLCD)");
		BUG();
		break;
	}

	LIBLCD_MSG("Finished dispatching KLCD message");

	return 0;
}

