#include "../common.h"

void free_netdev(struct net_device* dev) {
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	struct net_device* dev_key = fipc_get_remote(dev);
	fipc_marshal(dev_key);
	if (dev) {
	}
	fipc_destroy_shadow(dev_key);
	fipc_send(RPC_FREE_NETDEV, message);
	marshal_slot = 0;
	if (dev) {
	}
}

int eth_validate_addr(struct net_device* dev) {
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send(RPC_ETH_VALIDATE_ADDR, message);
	marshal_slot = 0;
	if (dev) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

int eth_mac_addr(struct net_device* dev) {
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send(RPC_ETH_MAC_ADDR, message);
	marshal_slot = 0;
	if (dev) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

void ether_setup(struct net_device* dev) {
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send(RPC_ETHER_SETUP, message);
	marshal_slot = 0;
	if (dev) {
	}
}

int register_netdevice(struct net_device* dev) {
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(dev);
	if (dev) {
		fipc_marshal(dev->flags);
		fipc_marshal(dev->priv_flags);
		fipc_marshal(dev->features);
		fipc_marshal(dev->hw_features);
		fipc_marshal(dev->hw_enc_features);
		fipc_marshal(dev->reg_state);
	}
	fipc_send(RPC_REGISTER_NETDEVICE, message);
	marshal_slot = 0;
	if (dev) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

void netif_carrier_off(struct net_device* dev) {
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send(RPC_NETIF_CARRIER_OFF, message);
	marshal_slot = 0;
	if (dev) {
	}
}

int __rtnl_link_register(struct rtnl_link_ops* ops) {
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(ops);
	if (ops) {
		fipc_marshal(ops->setup);
	}
	fipc_send(RPC___RTNL_LINK_REGISTER, message);
	marshal_slot = 0;
	if (ops) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

void __rtnl_link_unregister(struct rtnl_link_ops* ops) {
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(ops);
	if (ops) {
	}
	fipc_send(RPC___RTNL_LINK_UNREGISTER, message);
	marshal_slot = 0;
	if (ops) {
	}
}

void rtnl_link_unregister(struct rtnl_link_ops* ops) {
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(ops);
	if (ops) {
	}
	fipc_send(RPC_RTNL_LINK_UNREGISTER, message);
	marshal_slot = 0;
	if (ops) {
	}
}

struct net_device* alloc_netdev_mqs(int sizeof_priv, unsigned char name_assign_type, void (*setup)(struct net_device* dev), unsigned int txqs, unsigned int rxqs) {
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(sizeof_priv);
	fipc_marshal(name_assign_type);
	fipc_marshal(setup);
	fipc_marshal(txqs);
	fipc_marshal(rxqs);
	fipc_send(RPC_ALLOC_NETDEV_MQS, message);
	marshal_slot = 0;
	struct net_device* return_value = fipc_unmarshal(struct net_device*);
	if (return_value) {
	}
	return return_value;
}

void netif_carrier_on(struct net_device* dev) {
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send(RPC_NETIF_CARRIER_ON, message);
	marshal_slot = 0;
	if (dev) {
	}
}

void consume_skb(struct sk_buff* skb) {
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(skb);
	if (skb) {
	}
	fipc_send(RPC_CONSUME_SKB, message);
	marshal_slot = 0;
	if (skb) {
	}
}

void _int_1_kernel_nullnet_net_device_ndo_init_callee(struct fipc_message* message) {
	unsigned int marshal_slot = 0;
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
	marshal_slot = 0;
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		if (dev_dstats) {
		}
	}
	fipc_marshal(return_value);
}

void _void_1_kernel_nullnet_net_device_ndo_uninit_callee(struct fipc_message* message) {
	unsigned int marshal_slot = 0;
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
	marshal_slot = 0;
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		if (dev_dstats) {
		}
	}
}

void _int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats_callee(struct fipc_message* message) {
	unsigned int marshal_slot = 0;
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
	marshal_slot = 0;
	if (skb) {
	}
	if (dev) {
	}
	fipc_marshal(return_value);
}

void _1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64_callee(struct fipc_message* message) {
	unsigned int marshal_slot = 0;
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
	marshal_slot = 0;
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

void _void_1_kernel_nullnet_net_device_ndo_set_rx_mode_callee(struct fipc_message* message) {
	unsigned int marshal_slot = 0;
	void (*real_pointer)(struct net_device* dev) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	real_pointer(dev);
	marshal_slot = 0;
	if (dev) {
	}
}

void _int_1_kernel_nullnet_net_device_ndo_set_mac_address_callee(struct fipc_message* message) {
	unsigned int marshal_slot = 0;
	int (*real_pointer)(struct net_device* dev) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	int return_value = real_pointer(dev);
	marshal_slot = 0;
	if (dev) {
	}
	fipc_marshal(return_value);
}

void _int_1_kernel_nullnet_net_device_ndo_change_carrier_bool_callee(struct fipc_message* message) {
	unsigned int marshal_slot = 0;
	int (*real_pointer)(struct net_device* dev, bool new_carrier) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	bool new_carrier = fipc_unmarshal(bool);
	int return_value = real_pointer(dev, new_carrier);
	marshal_slot = 0;
	if (dev) {
	}
	fipc_marshal(return_value);
}

void _void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo_callee(struct fipc_message* message) {
	unsigned int marshal_slot = 0;
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
	marshal_slot = 0;
	if (dev) {
	}
	if (info) {
	}
}

void _void_1_kernel_nullnet_net_device_destructor_callee(struct fipc_message* message) {
	unsigned int marshal_slot = 0;
	void (*real_pointer)(struct net_device* dev) = fipc_unmarshal(void*);
	struct net_device* dev_key = fipc_unmarshal(struct net_device*);
	struct net_device* dev = fipc_get_local(dev_key);
	if (dev) {
	}
	real_pointer(dev);
	marshal_slot = 0;
	if (dev) {
	}
}

void _void_1_kernel_nullnet_net_device_setup_callee(struct fipc_message* message) {
	unsigned int marshal_slot = 0;
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
	marshal_slot = 0;
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

void _void_1_kernel_nullnet_net_device_setup_callee_alloc_callee(struct fipc_message* message) {
	unsigned int marshal_slot = 0;
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
	marshal_slot = 0;
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

LCD_TRAMPOLINE_DATA(trampoline_int_1_kernel_nullnet_net_device_ndo_init)
LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_net_device_ndo_init)
int trampoline_int_1_kernel_nullnet_net_device_ndo_init(struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_int_1_kernel_nullnet_net_device_ndo_init);
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		fipc_marshal(dev_dstats);
		if (dev_dstats) {
		}
	}
	fipc_send(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_INIT, message);
	marshal_slot = 0;
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		if (dev_dstats) {
		}
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

LCD_TRAMPOLINE_DATA(trampoline_void_1_kernel_nullnet_net_device_ndo_uninit)
LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_ndo_uninit)
void trampoline_void_1_kernel_nullnet_net_device_ndo_uninit(struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_void_1_kernel_nullnet_net_device_ndo_uninit);
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		fipc_marshal(dev_dstats);
		if (dev_dstats) {
		}
	}
	fipc_send(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_UNINIT, message);
	marshal_slot = 0;
	if (dev) {
		struct pcpu_dstats* dev_dstats = dev->dstats;
		if (dev_dstats) {
		}
	}
}

LCD_TRAMPOLINE_DATA(trampoline_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats)
LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats)
int trampoline_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats(struct sk_buff* skb, struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats);
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(skb);
	if (skb) {
		fipc_marshal(skb->len);
	}
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send(RPC_PTR_INT_1_KERNEL_NULLNET_SK_BUFF_NDO_START_XMIT_1_KERNEL_NULLNET_NET_DEVICE_STATS, message);
	marshal_slot = 0;
	if (skb) {
	}
	if (dev) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

LCD_TRAMPOLINE_DATA(trampoline_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64)
LCD_TRAMPOLINE_LINKAGE(trampoline_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64)
struct rtnl_link_stats64* trampoline_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64(struct net_device* dev, struct rtnl_link_stats64* storage) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64);
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
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
	marshal_slot = 0;
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

LCD_TRAMPOLINE_DATA(trampoline_void_1_kernel_nullnet_net_device_ndo_set_rx_mode)
LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_ndo_set_rx_mode)
void trampoline_void_1_kernel_nullnet_net_device_ndo_set_rx_mode(struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_void_1_kernel_nullnet_net_device_ndo_set_rx_mode);
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_RX_MODE, message);
	marshal_slot = 0;
	if (dev) {
	}
}

LCD_TRAMPOLINE_DATA(trampoline_int_1_kernel_nullnet_net_device_ndo_set_mac_address)
LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_net_device_ndo_set_mac_address)
int trampoline_int_1_kernel_nullnet_net_device_ndo_set_mac_address(struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_int_1_kernel_nullnet_net_device_ndo_set_mac_address);
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_MAC_ADDRESS, message);
	marshal_slot = 0;
	if (dev) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

LCD_TRAMPOLINE_DATA(trampoline_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool)
LCD_TRAMPOLINE_LINKAGE(trampoline_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool)
int trampoline_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool(struct net_device* dev, bool new_carrier) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool);
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_marshal(new_carrier);
	fipc_send(RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_CHANGE_CARRIER_BOOL, message);
	marshal_slot = 0;
	if (dev) {
	}
	int return_value = fipc_unmarshal(int);
	return return_value;
}

LCD_TRAMPOLINE_DATA(trampoline_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo)
LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo)
void trampoline_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo(struct net_device* dev, struct ethtool_drvinfo* info) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo);
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_marshal(info);
	if (info) {
	}
	fipc_send(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_GET_DRVINFO_1_KERNEL_NULLNET_ETHTOOL_DRVINFO_GET_DRVINFO, message);
	marshal_slot = 0;
	if (dev) {
	}
	if (info) {
	}
}

LCD_TRAMPOLINE_DATA(trampoline_void_1_kernel_nullnet_net_device_destructor)
LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_destructor)
void trampoline_void_1_kernel_nullnet_net_device_destructor(struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_void_1_kernel_nullnet_net_device_destructor);
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
	fipc_marshal(real_pointer);
	fipc_marshal(dev);
	if (dev) {
	}
	fipc_send(RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_DESTRUCTOR, message);
	marshal_slot = 0;
	if (dev) {
	}
}

LCD_TRAMPOLINE_DATA(trampoline_void_1_kernel_nullnet_net_device_setup)
LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_setup)
void trampoline_void_1_kernel_nullnet_net_device_setup(struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_void_1_kernel_nullnet_net_device_setup);
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
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
	marshal_slot = 0;
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

LCD_TRAMPOLINE_DATA(trampoline_void_1_kernel_nullnet_net_device_setup_callee_alloc)
LCD_TRAMPOLINE_LINKAGE(trampoline_void_1_kernel_nullnet_net_device_setup_callee_alloc)
void trampoline_void_1_kernel_nullnet_net_device_setup_callee_alloc(struct net_device* dev) {
	void* real_pointer;
	LCD_TRAMPOLINE_PROLOGUE(real_pointer, trampoline_void_1_kernel_nullnet_net_device_setup_callee_alloc);
	unsigned int marshal_slot = 0;
	struct fipc_message message_buffer = {0};
	struct fipc_message* message = &message_buffer;
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
	marshal_slot = 0;
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

void dispatch(struct fipc_message* message) {
	switch (message->host_id) {
	case RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_INIT:
		_int_1_kernel_nullnet_net_device_ndo_init_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_UNINIT:
		_void_1_kernel_nullnet_net_device_ndo_uninit_callee(message);
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_SK_BUFF_NDO_START_XMIT_1_KERNEL_NULLNET_NET_DEVICE_STATS:
		_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats_callee(message);
		break;

	case RPC_PTR_1_KERNEL_NULLNET_RTNL_LINK_STATS64_1_KERNEL_NULLNET_NET_DEVICE_NDO_GET_STATS64_1_KERNEL_NULLNET_RTNL_LINK_STATS64_NDO_GET_STATS64:
		_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64_callee(message);
		break;

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_RX_MODE:
		_void_1_kernel_nullnet_net_device_ndo_set_rx_mode_callee(message);
		break;

	case RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_MAC_ADDRESS:
		_int_1_kernel_nullnet_net_device_ndo_set_mac_address_callee(message);
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

	case RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP_CALLEE_ALLOC:
		_void_1_kernel_nullnet_net_device_setup_callee_alloc_callee(message);
		break;

	default:
		break;
	}
}

