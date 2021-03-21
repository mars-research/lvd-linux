#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void netif_carrier_on(struct net_device* dev)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__netif_carrier_on__dev__in(msg, *dev_ptr);
	}

	glue_call_server(msg, RPC_ID_netif_carrier_on);

	if (*dev_ptr) {
		caller_unmarshal_kernel__netif_carrier_on__dev__in(msg, *dev_ptr);
	}

}

void netif_carrier_off(struct net_device* dev)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__netif_carrier_off__dev__in(msg, *dev_ptr);
	}

	glue_call_server(msg, RPC_ID_netif_carrier_off);

	if (*dev_ptr) {
		caller_unmarshal_kernel__netif_carrier_off__dev__in(msg, *dev_ptr);
	}

}

int register_netdevice(struct net_device* dev)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__register_netdevice__dev__in(msg, *dev_ptr);
	}

	glue_call_server(msg, RPC_ID_register_netdevice);

	if (*dev_ptr) {
		caller_unmarshal_kernel__register_netdevice__dev__in(msg, *dev_ptr);
	}

	*ret_ptr = glue_unpack(msg, int);
	return ret;
}

void consume_skb(struct sk_buff* skb)
{
	struct glue_message *msg = glue_init_msg();

	struct sk_buff** skb_ptr = &skb;
	
	glue_pack_shadow(msg, *skb_ptr);
	if (*skb_ptr) {
		caller_marshal_kernel__consume_skb__skb__in(msg, *skb_ptr);
	}

	glue_call_server(msg, RPC_ID_consume_skb);

	if (*skb_ptr) {
		caller_unmarshal_kernel__consume_skb__skb__in(msg, *skb_ptr);
	}

}

struct net_device* alloc_netdev_mqs(int sizeof_priv, char const* name, unsigned char name_assign_type, fptr_setup setup, unsigned int txqs, unsigned int rxqs)
{
	struct glue_message *msg = glue_init_msg();

	int* sizeof_priv_ptr = &sizeof_priv;
	char const** name_ptr = &name;
	unsigned char* name_assign_type_ptr = &name_assign_type;
	fptr_setup* setup_ptr = &setup;
	unsigned int* txqs_ptr = &txqs;
	unsigned int* rxqs_ptr = &rxqs;
	struct net_device* ret = 0;
	struct net_device** ret_ptr = &ret;
	
	glue_pack(msg, *sizeof_priv_ptr);
	glue_pack(msg, *name_ptr);
	if (*name_ptr) {
		size_t i, len;
		char const* array = *name_ptr;
		for (len = 0; array[len]; ++len);
		glue_pack(msg, len + 1);
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(msg, *element);
		}

	}

	glue_pack(msg, *name_assign_type_ptr);
	glue_pack(msg, *setup_ptr);
	glue_pack(msg, *txqs_ptr);
	glue_pack(msg, *rxqs_ptr);
	glue_call_server(msg, RPC_ID_alloc_netdev_mqs);

	(void)name_ptr;
	*ret_ptr = glue_unpack_new_shadow(msg, struct net_device*, sizeof(struct net_device));
	if (*ret_ptr) {
		caller_unmarshal_kernel__alloc_netdev_mqs__ret_net_device__out(msg, *ret_ptr);
	}

	return ret;
}

void free_netdev(struct net_device* dev)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__free_netdev__dev__in(msg, *dev_ptr);
	}

	glue_call_server(msg, RPC_ID_free_netdev);

	if (*dev_ptr) {
		caller_unmarshal_kernel__free_netdev__dev__in(msg, *dev_ptr);
	}

}

void rtnl_lock(void)
{
	struct glue_message *msg = glue_init_msg();

	
	glue_call_server(msg, RPC_ID_rtnl_lock);

}

void rtnl_unlock(void)
{
	struct glue_message *msg = glue_init_msg();

	
	glue_call_server(msg, RPC_ID_rtnl_unlock);

}

void rtnl_link_unregister(struct rtnl_link_ops* _global_rtnl_link_ops)
{
	struct glue_message *msg = glue_init_msg();

	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	
	glue_pack_shadow(msg, *_global_rtnl_link_ops_ptr);
	if (*_global_rtnl_link_ops_ptr) {
		caller_marshal_kernel___global_rtnl_link_ops__in(msg, *_global_rtnl_link_ops_ptr);
	}

	glue_call_server(msg, RPC_ID_rtnl_link_unregister);

	if (*_global_rtnl_link_ops_ptr) {
		caller_unmarshal_kernel___global_rtnl_link_ops__in(msg, *_global_rtnl_link_ops_ptr);
	}

}

void __rtnl_link_unregister(struct rtnl_link_ops* _global_rtnl_link_ops)
{
	struct glue_message *msg = glue_init_msg();

	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	
	glue_pack_shadow(msg, *_global_rtnl_link_ops_ptr);
	if (*_global_rtnl_link_ops_ptr) {
		caller_marshal_kernel___global_rtnl_link_ops__in(msg, *_global_rtnl_link_ops_ptr);
	}

	glue_call_server(msg, RPC_ID___rtnl_link_unregister);

	if (*_global_rtnl_link_ops_ptr) {
		caller_unmarshal_kernel___global_rtnl_link_ops__in(msg, *_global_rtnl_link_ops_ptr);
	}

}

int __rtnl_link_register(struct rtnl_link_ops* _global_rtnl_link_ops)
{
	struct glue_message *msg = glue_init_msg();

	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	int ret = 0;
	int* ret_ptr = &ret;
	
	glue_pack(msg, *_global_rtnl_link_ops_ptr);
	if (*_global_rtnl_link_ops_ptr) {
		caller_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(msg, *_global_rtnl_link_ops_ptr);
	}

	glue_call_server(msg, RPC_ID___rtnl_link_register);

	if (*_global_rtnl_link_ops_ptr) {
		caller_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(msg, *_global_rtnl_link_ops_ptr);
	}

	*ret_ptr = glue_unpack(msg, int);
	return ret;
}

void ether_setup(struct net_device* dev)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__ether_setup__dev__in(msg, *dev_ptr);
	}

	glue_call_server(msg, RPC_ID_ether_setup);

	if (*dev_ptr) {
		caller_unmarshal_kernel__ether_setup__dev__in(msg, *dev_ptr);
	}

}

int eth_validate_addr(struct net_device* dev)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__eth_validate_addr__dev__in(msg, *dev_ptr);
	}

	glue_call_server(msg, RPC_ID_eth_validate_addr);

	if (*dev_ptr) {
		caller_unmarshal_kernel__eth_validate_addr__dev__in(msg, *dev_ptr);
	}

	*ret_ptr = glue_unpack(msg, int);
	return ret;
}

int eth_mac_addr(struct net_device* dev, void* p)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	struct sockaddr** p_ptr = (struct sockaddr**)&p;
	int ret = 0;
	int* ret_ptr = &ret;
	
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__eth_mac_addr__dev__in(msg, *dev_ptr);
	}

	glue_pack_shadow(msg, *p_ptr);
	if (*p_ptr) {
		caller_marshal_kernel__eth_mac_addr__p__in(msg, *p_ptr);
	}

	glue_call_server(msg, RPC_ID_eth_mac_addr);

	if (*dev_ptr) {
		caller_unmarshal_kernel__eth_mac_addr__dev__in(msg, *dev_ptr);
	}

	if (*p_ptr) {
		caller_unmarshal_kernel__eth_mac_addr__p__in(msg, *p_ptr);
	}

	*ret_ptr = glue_unpack(msg, int);
	return ret;
}

void trmp_impl_setup(fptr_setup target, struct net_device* dev)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	
	glue_pack(msg, target);
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__setup__dev__in(msg, *dev_ptr);
	}

	glue_call_server(msg, RPC_ID_setup);

	if (*dev_ptr) {
		caller_unmarshal_kernel__setup__dev__in(msg, *dev_ptr);
	}

}

void setup_callee(struct glue_message* msg)
{
	fptr_setup function_ptr = glue_unpack(msg, fptr_setup);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__setup__dev__in(msg, *dev_ptr);
	}

	function_ptr(dev);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__setup__dev__in(msg, *dev_ptr);
	}

	msg->slots[0] = msg->position;
}

LCD_TRAMPOLINE_DATA(trmp_setup)
void LCD_TRAMPOLINE_LINKAGE(trmp_setup) trmp_setup(struct net_device* dev)
{
	volatile fptr_impl_setup impl;
	fptr_setup target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_setup);
	impl = trmp_impl_setup;
	return impl(target, dev);
}

void trmp_impl_get_drvinfo(fptr_get_drvinfo target, struct net_device* dev, struct ethtool_drvinfo* info)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	struct ethtool_drvinfo** info_ptr = &info;
	
	glue_pack(msg, target);
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__get_drvinfo__dev__in(msg, *dev_ptr);
	}

	glue_pack_shadow(msg, *info_ptr);
	if (*info_ptr) {
		caller_marshal_kernel__get_drvinfo__info__in(msg, *info_ptr);
	}

	glue_call_server(msg, RPC_ID_get_drvinfo);

	if (*dev_ptr) {
		caller_unmarshal_kernel__get_drvinfo__dev__in(msg, *dev_ptr);
	}

	if (*info_ptr) {
		caller_unmarshal_kernel__get_drvinfo__info__in(msg, *info_ptr);
	}

}

void get_drvinfo_callee(struct glue_message* msg)
{
	fptr_get_drvinfo function_ptr = glue_unpack(msg, fptr_get_drvinfo);
	struct net_device* dev = 0;
	struct ethtool_drvinfo* info = 0;
	struct net_device** dev_ptr = &dev;
	struct ethtool_drvinfo** info_ptr = &info;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__get_drvinfo__dev__in(msg, *dev_ptr);
	}

	*info_ptr = glue_unpack(msg, struct ethtool_drvinfo*);
	if (*info_ptr) {
		callee_unmarshal_kernel__get_drvinfo__info__in(msg, *info_ptr);
	}

	function_ptr(dev, info);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__get_drvinfo__dev__in(msg, *dev_ptr);
	}

	if (*info_ptr) {
		callee_marshal_kernel__get_drvinfo__info__in(msg, *info_ptr);
	}

	msg->slots[0] = msg->position;
}

LCD_TRAMPOLINE_DATA(trmp_get_drvinfo)
void LCD_TRAMPOLINE_LINKAGE(trmp_get_drvinfo) trmp_get_drvinfo(struct net_device* dev, struct ethtool_drvinfo* info)
{
	volatile fptr_impl_get_drvinfo impl;
	fptr_get_drvinfo target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_get_drvinfo);
	impl = trmp_impl_get_drvinfo;
	return impl(target, dev, info);
}

int trmp_impl_ndo_init(fptr_ndo_init target, struct net_device* dev)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	glue_pack(msg, target);
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__ndo_init__dev__in(msg, *dev_ptr);
	}

	glue_call_server(msg, RPC_ID_ndo_init);

	if (*dev_ptr) {
		caller_unmarshal_kernel__ndo_init__dev__in(msg, *dev_ptr);
	}

	*ret_ptr = glue_unpack(msg, int);
	return ret;
}

void ndo_init_callee(struct glue_message* msg)
{
	fptr_ndo_init function_ptr = glue_unpack(msg, fptr_ndo_init);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__ndo_init__dev__in(msg, *dev_ptr);
	}

	ret = function_ptr(dev);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__ndo_init__dev__in(msg, *dev_ptr);
	}

	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
}

LCD_TRAMPOLINE_DATA(trmp_ndo_init)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_init) trmp_ndo_init(struct net_device* dev)
{
	volatile fptr_impl_ndo_init impl;
	fptr_ndo_init target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_init);
	impl = trmp_impl_ndo_init;
	return impl(target, dev);
}

void trmp_impl_ndo_uninit(fptr_ndo_uninit target, struct net_device* dev)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	
	glue_pack(msg, target);
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__ndo_uninit__dev__in(msg, *dev_ptr);
	}

	glue_call_server(msg, RPC_ID_ndo_uninit);

	if (*dev_ptr) {
		caller_unmarshal_kernel__ndo_uninit__dev__in(msg, *dev_ptr);
	}

}

void ndo_uninit_callee(struct glue_message* msg)
{
	fptr_ndo_uninit function_ptr = glue_unpack(msg, fptr_ndo_uninit);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__ndo_uninit__dev__in(msg, *dev_ptr);
	}

	function_ptr(dev);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__ndo_uninit__dev__in(msg, *dev_ptr);
	}

	msg->slots[0] = msg->position;
}

LCD_TRAMPOLINE_DATA(trmp_ndo_uninit)
void LCD_TRAMPOLINE_LINKAGE(trmp_ndo_uninit) trmp_ndo_uninit(struct net_device* dev)
{
	volatile fptr_impl_ndo_uninit impl;
	fptr_ndo_uninit target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_uninit);
	impl = trmp_impl_ndo_uninit;
	return impl(target, dev);
}

long long trmp_impl_ndo_start_xmit(fptr_ndo_start_xmit target, struct sk_buff* skb, struct net_device* dev)
{
	struct glue_message *msg = glue_init_msg();

	struct sk_buff** skb_ptr = &skb;
	struct net_device** dev_ptr = &dev;
	long long ret = 0;
	long long* ret_ptr = &ret;
	
	glue_pack(msg, target);
	glue_pack_shadow(msg, *skb_ptr);
	if (*skb_ptr) {
		caller_marshal_kernel__ndo_start_xmit__skb__in(msg, *skb_ptr);
	}

	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__ndo_start_xmit__dev__in(msg, *dev_ptr);
	}

	glue_call_server(msg, RPC_ID_ndo_start_xmit);

	if (*skb_ptr) {
		caller_unmarshal_kernel__ndo_start_xmit__skb__in(msg, *skb_ptr);
	}

	if (*dev_ptr) {
		caller_unmarshal_kernel__ndo_start_xmit__dev__in(msg, *dev_ptr);
	}

	*ret_ptr = glue_unpack(msg, long long);
	return ret;
}

void ndo_start_xmit_callee(struct glue_message* msg)
{
	fptr_ndo_start_xmit function_ptr = glue_unpack(msg, fptr_ndo_start_xmit);
	struct sk_buff* skb = 0;
	struct net_device* dev = 0;
	struct sk_buff** skb_ptr = &skb;
	struct net_device** dev_ptr = &dev;
	long long ret = 0;
	long long* ret_ptr = &ret;
	
	*skb_ptr = glue_unpack(msg, struct sk_buff*);
	if (*skb_ptr) {
		callee_unmarshal_kernel__ndo_start_xmit__skb__in(msg, *skb_ptr);
	}

	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__ndo_start_xmit__dev__in(msg, *dev_ptr);
	}

	ret = function_ptr(skb, dev);

	msg->position = 0;
	if (*skb_ptr) {
		callee_marshal_kernel__ndo_start_xmit__skb__in(msg, *skb_ptr);
	}

	if (*dev_ptr) {
		callee_marshal_kernel__ndo_start_xmit__dev__in(msg, *dev_ptr);
	}

	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
}

LCD_TRAMPOLINE_DATA(trmp_ndo_start_xmit)
long long LCD_TRAMPOLINE_LINKAGE(trmp_ndo_start_xmit) trmp_ndo_start_xmit(struct sk_buff* skb, struct net_device* dev)
{
	volatile fptr_impl_ndo_start_xmit impl;
	fptr_ndo_start_xmit target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_start_xmit);
	impl = trmp_impl_ndo_start_xmit;
	return impl(target, skb, dev);
}

void trmp_impl_ndo_set_rx_mode(fptr_ndo_set_rx_mode target, struct net_device* dev)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	
	glue_pack(msg, target);
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__ndo_set_rx_mode__dev__in(msg, *dev_ptr);
	}

	glue_call_server(msg, RPC_ID_ndo_set_rx_mode);

	if (*dev_ptr) {
		caller_unmarshal_kernel__ndo_set_rx_mode__dev__in(msg, *dev_ptr);
	}

}

void ndo_set_rx_mode_callee(struct glue_message* msg)
{
	fptr_ndo_set_rx_mode function_ptr = glue_unpack(msg, fptr_ndo_set_rx_mode);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__ndo_set_rx_mode__dev__in(msg, *dev_ptr);
	}

	function_ptr(dev);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__ndo_set_rx_mode__dev__in(msg, *dev_ptr);
	}

	msg->slots[0] = msg->position;
}

LCD_TRAMPOLINE_DATA(trmp_ndo_set_rx_mode)
void LCD_TRAMPOLINE_LINKAGE(trmp_ndo_set_rx_mode) trmp_ndo_set_rx_mode(struct net_device* dev)
{
	volatile fptr_impl_ndo_set_rx_mode impl;
	fptr_ndo_set_rx_mode target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_set_rx_mode);
	impl = trmp_impl_ndo_set_rx_mode;
	return impl(target, dev);
}

struct rtnl_link_stats64* trmp_impl_ndo_get_stats64(fptr_ndo_get_stats64 target, struct net_device* dev, struct rtnl_link_stats64* stats)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	struct rtnl_link_stats64** stats_ptr = &stats;
	struct rtnl_link_stats64* ret = 0;
	struct rtnl_link_stats64** ret_ptr = &ret;
	
	glue_pack(msg, target);
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__ndo_get_stats64__dev__in(msg, *dev_ptr);
	}

	glue_pack_shadow(msg, *stats_ptr);
	if (*stats_ptr) {
		caller_marshal_kernel__ndo_get_stats64__stats__in(msg, *stats_ptr);
	}

	glue_call_server(msg, RPC_ID_ndo_get_stats64);

	if (*dev_ptr) {
		caller_unmarshal_kernel__ndo_get_stats64__dev__in(msg, *dev_ptr);
	}

	if (*stats_ptr) {
		caller_unmarshal_kernel__ndo_get_stats64__stats__in(msg, *stats_ptr);
	}

	*ret_ptr = glue_unpack(msg, struct rtnl_link_stats64*);
	if (*ret_ptr) {
		caller_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(msg, *ret_ptr);
	}

	return ret;
}

void ndo_get_stats64_callee(struct glue_message* msg)
{
	fptr_ndo_get_stats64 function_ptr = glue_unpack(msg, fptr_ndo_get_stats64);
	struct net_device* dev = 0;
	struct rtnl_link_stats64* stats = 0;
	struct net_device** dev_ptr = &dev;
	struct rtnl_link_stats64** stats_ptr = &stats;
	struct rtnl_link_stats64* ret = 0;
	struct rtnl_link_stats64** ret_ptr = &ret;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__ndo_get_stats64__dev__in(msg, *dev_ptr);
	}

	*stats_ptr = glue_unpack(msg, struct rtnl_link_stats64*);
	if (*stats_ptr) {
		callee_unmarshal_kernel__ndo_get_stats64__stats__in(msg, *stats_ptr);
	}

	ret = function_ptr(dev, stats);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__ndo_get_stats64__dev__in(msg, *dev_ptr);
	}

	if (*stats_ptr) {
		callee_marshal_kernel__ndo_get_stats64__stats__in(msg, *stats_ptr);
	}

	glue_pack_shadow(msg, *ret_ptr);
	if (*ret_ptr) {
		callee_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(msg, *ret_ptr);
	}

	msg->slots[0] = msg->position;
}

LCD_TRAMPOLINE_DATA(trmp_ndo_get_stats64)
struct rtnl_link_stats64* LCD_TRAMPOLINE_LINKAGE(trmp_ndo_get_stats64) trmp_ndo_get_stats64(struct net_device* dev, struct rtnl_link_stats64* stats)
{
	volatile fptr_impl_ndo_get_stats64 impl;
	fptr_ndo_get_stats64 target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_get_stats64);
	impl = trmp_impl_ndo_get_stats64;
	return impl(target, dev, stats);
}

int trmp_impl_ndo_change_carrier(fptr_ndo_change_carrier target, struct net_device* dev, bool new_carrier)
{
	struct glue_message *msg = glue_init_msg();

	struct net_device** dev_ptr = &dev;
	bool* new_carrier_ptr = &new_carrier;
	int ret = 0;
	int* ret_ptr = &ret;
	
	glue_pack(msg, target);
	glue_pack_shadow(msg, *dev_ptr);
	if (*dev_ptr) {
		caller_marshal_kernel__ndo_change_carrier__dev__in(msg, *dev_ptr);
	}

	glue_pack(msg, *new_carrier_ptr);
	glue_call_server(msg, RPC_ID_ndo_change_carrier);

	if (*dev_ptr) {
		caller_unmarshal_kernel__ndo_change_carrier__dev__in(msg, *dev_ptr);
	}

	*ret_ptr = glue_unpack(msg, int);
	return ret;
}

void ndo_change_carrier_callee(struct glue_message* msg)
{
	fptr_ndo_change_carrier function_ptr = glue_unpack(msg, fptr_ndo_change_carrier);
	struct net_device* dev = 0;
	bool new_carrier = 0;
	struct net_device** dev_ptr = &dev;
	bool* new_carrier_ptr = &new_carrier;
	int ret = 0;
	int* ret_ptr = &ret;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__ndo_change_carrier__dev__in(msg, *dev_ptr);
	}

	*new_carrier_ptr = glue_unpack(msg, bool);
	ret = function_ptr(dev, new_carrier);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__ndo_change_carrier__dev__in(msg, *dev_ptr);
	}

	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
}

LCD_TRAMPOLINE_DATA(trmp_ndo_change_carrier)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_change_carrier) trmp_ndo_change_carrier(struct net_device* dev, bool new_carrier)
{
	volatile fptr_impl_ndo_change_carrier impl;
	fptr_ndo_change_carrier target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_change_carrier);
	impl = trmp_impl_ndo_change_carrier;
	return impl(target, dev, new_carrier);
}

int trmp_impl_validate(fptr_validate target, struct nlattr** tb, struct nlattr** data)
{
	struct glue_message *msg = glue_init_msg();

	struct nlattr*** tb_ptr = &tb;
	struct nlattr*** data_ptr = &data;
	int ret = 0;
	int* ret_ptr = &ret;
	
	glue_pack(msg, target);
	glue_pack_shadow(msg, *tb_ptr);
	if (*tb_ptr) {
		glue_pack_shadow(msg, **tb_ptr);
		if (**tb_ptr) {
			caller_marshal_kernel__validate__tb__in(msg, **tb_ptr);
		}

	}

	glue_pack_shadow(msg, *data_ptr);
	if (*data_ptr) {
		glue_pack_shadow(msg, **data_ptr);
		if (**data_ptr) {
			caller_marshal_kernel__validate__data__in(msg, **data_ptr);
		}

	}

	glue_call_server(msg, RPC_ID_validate);

	if (*tb_ptr) {
		if (**tb_ptr) {
			caller_unmarshal_kernel__validate__tb__in(msg, **tb_ptr);
		}

	}

	if (*data_ptr) {
		if (**data_ptr) {
			caller_unmarshal_kernel__validate__data__in(msg, **data_ptr);
		}

	}

	*ret_ptr = glue_unpack(msg, int);
	return ret;
}

void validate_callee(struct glue_message* msg)
{
	fptr_validate function_ptr = glue_unpack(msg, fptr_validate);
	struct nlattr** tb = 0;
	struct nlattr** data = 0;
	struct nlattr*** tb_ptr = &tb;
	struct nlattr*** data_ptr = &data;
	int ret = 0;
	int* ret_ptr = &ret;
	
	*tb_ptr = glue_unpack(msg, struct nlattr**);
	if (*tb_ptr) {
		**tb_ptr = glue_unpack(msg, struct nlattr*);
		if (**tb_ptr) {
			callee_unmarshal_kernel__validate__tb__in(msg, **tb_ptr);
		}

	}

	*data_ptr = glue_unpack(msg, struct nlattr**);
	if (*data_ptr) {
		**data_ptr = glue_unpack(msg, struct nlattr*);
		if (**data_ptr) {
			callee_unmarshal_kernel__validate__data__in(msg, **data_ptr);
		}

	}

	ret = function_ptr(tb, data);

	msg->position = 0;
	if (*tb_ptr) {
		if (**tb_ptr) {
			callee_marshal_kernel__validate__tb__in(msg, **tb_ptr);
		}

	}

	if (*data_ptr) {
		if (**data_ptr) {
			callee_marshal_kernel__validate__data__in(msg, **data_ptr);
		}

	}

	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
}

LCD_TRAMPOLINE_DATA(trmp_validate)
int LCD_TRAMPOLINE_LINKAGE(trmp_validate) trmp_validate(struct nlattr** tb, struct nlattr** data)
{
	volatile fptr_impl_validate impl;
	fptr_validate target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_validate);
	impl = trmp_impl_validate;
	return impl(target, tb, data);
}

int try_dispatch(enum RPC_ID id, struct glue_message* msg)
{
	switch(id) {
	case MODULE_INIT:
		glue_user_trace("__dummy_lcd_init");
		__dummy_lcd_init();
		break;
		
	case MODULE_EXIT:
		glue_user_trace("__dummy_lcd_exit");
		__dummy_lcd_exit();
		break;

	case RPC_ID_setup:
		glue_user_trace("setup\n");
		setup_callee(msg);
		break;

	case RPC_ID_get_drvinfo:
		glue_user_trace("get_drvinfo\n");
		get_drvinfo_callee(msg);
		break;

	case RPC_ID_ndo_init:
		glue_user_trace("ndo_init\n");
		ndo_init_callee(msg);
		break;

	case RPC_ID_ndo_uninit:
		glue_user_trace("ndo_uninit\n");
		ndo_uninit_callee(msg);
		break;

	case RPC_ID_ndo_start_xmit:
		glue_user_trace("ndo_start_xmit\n");
		ndo_start_xmit_callee(msg);
		break;

	case RPC_ID_ndo_set_rx_mode:
		glue_user_trace("ndo_set_rx_mode\n");
		ndo_set_rx_mode_callee(msg);
		break;

	case RPC_ID_ndo_get_stats64:
		glue_user_trace("ndo_get_stats64\n");
		ndo_get_stats64_callee(msg);
		break;

	case RPC_ID_ndo_change_carrier:
		glue_user_trace("ndo_change_carrier\n");
		ndo_change_carrier_callee(msg);
		break;

	case RPC_ID_validate:
		glue_user_trace("validate\n");
		validate_callee(msg);
		break;

	default:
		return 0;
	}

	return 1;
}

