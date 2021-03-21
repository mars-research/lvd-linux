#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void netif_carrier_on_callee(struct glue_message* msg)
{
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__netif_carrier_on__dev__in(msg, *dev_ptr);
	}

	netif_carrier_on(dev);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__netif_carrier_on__dev__in(msg, *dev_ptr);
	}

	msg->slots[0] = msg->position;
}

void netif_carrier_off_callee(struct glue_message* msg)
{
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__netif_carrier_off__dev__in(msg, *dev_ptr);
	}

	netif_carrier_off(dev);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__netif_carrier_off__dev__in(msg, *dev_ptr);
	}

	msg->slots[0] = msg->position;
}

void register_netdevice_callee(struct glue_message* msg)
{
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__register_netdevice__dev__in(msg, *dev_ptr);
	}

	ret = register_netdevice(dev);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__register_netdevice__dev__in(msg, *dev_ptr);
	}

	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
}

void consume_skb_callee(struct glue_message* msg)
{
	struct sk_buff* skb = 0;
	struct sk_buff** skb_ptr = &skb;
	
	*skb_ptr = glue_unpack(msg, struct sk_buff*);
	if (*skb_ptr) {
		callee_unmarshal_kernel__consume_skb__skb__in(msg, *skb_ptr);
	}

	consume_skb(skb);

	msg->position = 0;
	if (*skb_ptr) {
		callee_marshal_kernel__consume_skb__skb__in(msg, *skb_ptr);
	}

	msg->slots[0] = msg->position;
}

void alloc_netdev_mqs_callee(struct glue_message* msg)
{
	int sizeof_priv = 0;
	char const* name = 0;
	unsigned char name_assign_type = 0;
	fptr_setup setup = 0;
	unsigned int txqs = 0;
	unsigned int rxqs = 0;
	int* sizeof_priv_ptr = &sizeof_priv;
	char const** name_ptr = &name;
	unsigned char* name_assign_type_ptr = &name_assign_type;
	fptr_setup* setup_ptr = &setup;
	unsigned int* txqs_ptr = &txqs;
	unsigned int* rxqs_ptr = &rxqs;
	struct net_device* ret = 0;
	struct net_device** ret_ptr = &ret;
	
	*sizeof_priv_ptr = glue_unpack(msg, int);
	*name_ptr = glue_unpack_new_shadow(msg, char const*, sizeof(char) * glue_peek(msg));
	if (*name_ptr) {
		char* writable = (char*)*name_ptr;
		size_t i, len;
		char* array = writable;
		len = glue_unpack(msg, size_t) - 1;
		array[len] = '\0';
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(msg, char);
		}

	}

	*name_assign_type_ptr = glue_unpack(msg, unsigned char);
	*setup_ptr = glue_unpack_rpc_ptr(msg, setup);
	*txqs_ptr = glue_unpack(msg, unsigned int);
	*rxqs_ptr = glue_unpack(msg, unsigned int);
	ret = alloc_netdev_mqs(sizeof_priv, name, name_assign_type, setup, txqs, rxqs);

	msg->position = 0;
	(void)name_ptr;
	glue_pack(msg, *ret_ptr);
	if (*ret_ptr) {
		callee_marshal_kernel__alloc_netdev_mqs__ret_net_device__out(msg, *ret_ptr);
	}

	msg->slots[0] = msg->position;
}

void free_netdev_callee(struct glue_message* msg)
{
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__free_netdev__dev__in(msg, *dev_ptr);
	}

	free_netdev(dev);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__free_netdev__dev__in(msg, *dev_ptr);
	}

	msg->slots[0] = msg->position;
}

void rtnl_lock_callee(struct glue_message* msg)
{
	
	rtnl_lock();

	msg->position = 0;
	msg->slots[0] = msg->position;
}

void rtnl_unlock_callee(struct glue_message* msg)
{
	
	rtnl_unlock();

	msg->position = 0;
	msg->slots[0] = msg->position;
}

void rtnl_link_unregister_callee(struct glue_message* msg)
{
	struct rtnl_link_ops* _global_rtnl_link_ops = 0;
	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	
	*_global_rtnl_link_ops_ptr = glue_unpack(msg, struct rtnl_link_ops*);
	if (*_global_rtnl_link_ops_ptr) {
		callee_unmarshal_kernel___global_rtnl_link_ops__in(msg, *_global_rtnl_link_ops_ptr);
	}

	rtnl_link_unregister(_global_rtnl_link_ops);

	msg->position = 0;
	if (*_global_rtnl_link_ops_ptr) {
		callee_marshal_kernel___global_rtnl_link_ops__in(msg, *_global_rtnl_link_ops_ptr);
	}

	msg->slots[0] = msg->position;
}

void __rtnl_link_unregister_callee(struct glue_message* msg)
{
	struct rtnl_link_ops* _global_rtnl_link_ops = 0;
	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	
	*_global_rtnl_link_ops_ptr = glue_unpack(msg, struct rtnl_link_ops*);
	if (*_global_rtnl_link_ops_ptr) {
		callee_unmarshal_kernel___global_rtnl_link_ops__in(msg, *_global_rtnl_link_ops_ptr);
	}

	__rtnl_link_unregister(_global_rtnl_link_ops);

	msg->position = 0;
	if (*_global_rtnl_link_ops_ptr) {
		callee_marshal_kernel___global_rtnl_link_ops__in(msg, *_global_rtnl_link_ops_ptr);
	}

	msg->slots[0] = msg->position;
}

void __rtnl_link_register_callee(struct glue_message* msg)
{
	struct rtnl_link_ops* _global_rtnl_link_ops = 0;
	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	int ret = 0;
	int* ret_ptr = &ret;
	
	*_global_rtnl_link_ops_ptr = glue_unpack_new_shadow(msg, struct rtnl_link_ops*, sizeof(struct rtnl_link_ops));
	if (*_global_rtnl_link_ops_ptr) {
		callee_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(msg, *_global_rtnl_link_ops_ptr);
	}

	ret = __rtnl_link_register(_global_rtnl_link_ops);

	msg->position = 0;
	if (*_global_rtnl_link_ops_ptr) {
		callee_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(msg, *_global_rtnl_link_ops_ptr);
	}

	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
}

void ether_setup_callee(struct glue_message* msg)
{
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__ether_setup__dev__in(msg, *dev_ptr);
	}

	ether_setup(dev);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__ether_setup__dev__in(msg, *dev_ptr);
	}

	msg->slots[0] = msg->position;
}

void eth_validate_addr_callee(struct glue_message* msg)
{
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__eth_validate_addr__dev__in(msg, *dev_ptr);
	}

	ret = eth_validate_addr(dev);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__eth_validate_addr__dev__in(msg, *dev_ptr);
	}

	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
}

void eth_mac_addr_callee(struct glue_message* msg)
{
	struct net_device* dev = 0;
	struct sockaddr* p = 0;
	struct net_device** dev_ptr = &dev;
	struct sockaddr** p_ptr = &p;
	int ret = 0;
	int* ret_ptr = &ret;
	
	*dev_ptr = glue_unpack(msg, struct net_device*);
	if (*dev_ptr) {
		callee_unmarshal_kernel__eth_mac_addr__dev__in(msg, *dev_ptr);
	}

	*p_ptr = glue_unpack(msg, struct sockaddr*);
	if (*p_ptr) {
		callee_unmarshal_kernel__eth_mac_addr__p__in(msg, *p_ptr);
	}

	ret = eth_mac_addr(dev, p);

	msg->position = 0;
	if (*dev_ptr) {
		callee_marshal_kernel__eth_mac_addr__dev__in(msg, *dev_ptr);
	}

	if (*p_ptr) {
		callee_marshal_kernel__eth_mac_addr__p__in(msg, *p_ptr);
	}

	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
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

	glue_call_client(msg, RPC_ID_setup);

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

	glue_call_client(msg, RPC_ID_get_drvinfo);

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

	glue_call_client(msg, RPC_ID_ndo_init);

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

	glue_call_client(msg, RPC_ID_ndo_uninit);

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

	glue_call_client(msg, RPC_ID_ndo_start_xmit);

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

	glue_call_client(msg, RPC_ID_ndo_set_rx_mode);

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

	glue_call_client(msg, RPC_ID_ndo_get_stats64);

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
	glue_call_client(msg, RPC_ID_ndo_change_carrier);

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

	glue_call_client(msg, RPC_ID_validate);

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
	case RPC_ID_netif_carrier_on:
		glue_user_trace("netif_carrier_on\n");
		netif_carrier_on_callee(msg);
		break;

	case RPC_ID_netif_carrier_off:
		glue_user_trace("netif_carrier_off\n");
		netif_carrier_off_callee(msg);
		break;

	case RPC_ID_register_netdevice:
		glue_user_trace("register_netdevice\n");
		register_netdevice_callee(msg);
		break;

	case RPC_ID_consume_skb:
		glue_user_trace("consume_skb\n");
		consume_skb_callee(msg);
		break;

	case RPC_ID_alloc_netdev_mqs:
		glue_user_trace("alloc_netdev_mqs\n");
		alloc_netdev_mqs_callee(msg);
		break;

	case RPC_ID_free_netdev:
		glue_user_trace("free_netdev\n");
		free_netdev_callee(msg);
		break;

	case RPC_ID_rtnl_lock:
		glue_user_trace("rtnl_lock\n");
		rtnl_lock_callee(msg);
		break;

	case RPC_ID_rtnl_unlock:
		glue_user_trace("rtnl_unlock\n");
		rtnl_unlock_callee(msg);
		break;

	case RPC_ID_rtnl_link_unregister:
		glue_user_trace("rtnl_link_unregister\n");
		rtnl_link_unregister_callee(msg);
		break;

	case RPC_ID___rtnl_link_unregister:
		glue_user_trace("__rtnl_link_unregister\n");
		__rtnl_link_unregister_callee(msg);
		break;

	case RPC_ID___rtnl_link_register:
		glue_user_trace("__rtnl_link_register\n");
		__rtnl_link_register_callee(msg);
		break;

	case RPC_ID_ether_setup:
		glue_user_trace("ether_setup\n");
		ether_setup_callee(msg);
		break;

	case RPC_ID_eth_validate_addr:
		glue_user_trace("eth_validate_addr\n");
		eth_validate_addr_callee(msg);
		break;

	case RPC_ID_eth_mac_addr:
		glue_user_trace("eth_mac_addr\n");
		eth_mac_addr_callee(msg);
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

