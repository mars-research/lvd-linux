#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

struct glue_message shared_buffer = {0};

void caller_marshal_kernel__netif_carrier_on__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	
	glue_pack_shadow(msg, *netdev_ops_ptr);
	if (*netdev_ops_ptr) {
		caller_marshal_kernel___global_netdev_ops__in(msg, *netdev_ops_ptr);
	}

}

void callee_unmarshal_kernel__netif_carrier_on__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	
	*netdev_ops_ptr = glue_unpack(msg, struct net_device_ops const*);
	if (*netdev_ops_ptr) {
		struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
		callee_unmarshal_kernel___global_netdev_ops__in(msg, writable);
	}

}

void callee_marshal_kernel__netif_carrier_on__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	
	if (*netdev_ops_ptr) {
		callee_marshal_kernel___global_netdev_ops__in(msg, *netdev_ops_ptr);
	}

}

void caller_unmarshal_kernel__netif_carrier_on__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	
	if (*netdev_ops_ptr) {
		struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
		caller_unmarshal_kernel___global_netdev_ops__in(msg, writable);
	}

}

void caller_marshal_kernel___global_netdev_ops__in(
	struct glue_message* msg,
	struct net_device_ops const* ptr)
{
	fptr_ndo_init const* ndo_init_ptr = &ptr->ndo_init;
	fptr_ndo_uninit const* ndo_uninit_ptr = &ptr->ndo_uninit;
	fptr_ndo_start_xmit const* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_ndo_set_rx_mode const* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_ndo_get_stats64 const* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_ndo_change_carrier const* ndo_change_carrier_ptr = &ptr->ndo_change_carrier;
	
	glue_pack(msg, *ndo_init_ptr);
	glue_pack(msg, *ndo_uninit_ptr);
	glue_pack(msg, *ndo_start_xmit_ptr);
	glue_pack(msg, *ndo_set_rx_mode_ptr);
	glue_pack(msg, *ndo_get_stats64_ptr);
	glue_pack(msg, *ndo_change_carrier_ptr);
}

void callee_unmarshal_kernel___global_netdev_ops__in(
	struct glue_message* msg,
	struct net_device_ops* ptr)
{
	fptr_ndo_init* ndo_init_ptr = &ptr->ndo_init;
	fptr_ndo_uninit* ndo_uninit_ptr = &ptr->ndo_uninit;
	fptr_ndo_start_xmit* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_ndo_set_rx_mode* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_ndo_get_stats64* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_ndo_change_carrier* ndo_change_carrier_ptr = &ptr->ndo_change_carrier;
	
	*ndo_init_ptr = glue_unpack_rpc_ptr(msg, ndo_init);
	*ndo_uninit_ptr = glue_unpack_rpc_ptr(msg, ndo_uninit);
	*ndo_start_xmit_ptr = glue_unpack_rpc_ptr(msg, ndo_start_xmit);
	*ndo_set_rx_mode_ptr = glue_unpack_rpc_ptr(msg, ndo_set_rx_mode);
	*ndo_get_stats64_ptr = glue_unpack_rpc_ptr(msg, ndo_get_stats64);
	*ndo_change_carrier_ptr = glue_unpack_rpc_ptr(msg, ndo_change_carrier);
}

void callee_marshal_kernel___global_netdev_ops__in(
	struct glue_message* msg,
	struct net_device_ops const* ptr)
{
	
}

void caller_unmarshal_kernel___global_netdev_ops__in(
	struct glue_message* msg,
	struct net_device_ops* ptr)
{
	
}

void caller_marshal_kernel__netif_carrier_off__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__netif_carrier_off__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__netif_carrier_off__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__netif_carrier_off__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__register_netdevice__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int const* flags_ptr = &ptr->flags;
	unsigned int const* priv_flags_ptr = &ptr->priv_flags;
	unsigned char const* addr_assign_type_ptr = &ptr->addr_assign_type;
	struct rtnl_link_ops const* const* rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	glue_pack_shadow(msg, *netdev_ops_ptr);
	if (*netdev_ops_ptr) {
		caller_marshal_kernel___global_netdev_ops__in(msg, *netdev_ops_ptr);
	}

	glue_pack_shadow(msg, *ethtool_ops_ptr);
	if (*ethtool_ops_ptr) {
		caller_marshal_kernel___global_ethtool_ops__in(msg, *ethtool_ops_ptr);
	}

	glue_pack(msg, *flags_ptr);
	glue_pack(msg, *priv_flags_ptr);
	glue_pack(msg, *addr_assign_type_ptr);
	glue_pack_shadow(msg, *rtnl_link_ops_ptr);
	if (*rtnl_link_ops_ptr) {
		caller_marshal_kernel___global_rtnl_link_ops__in(msg, *rtnl_link_ops_ptr);
	}

}

void callee_unmarshal_kernel__register_netdevice__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int* flags_ptr = &ptr->flags;
	unsigned int* priv_flags_ptr = &ptr->priv_flags;
	unsigned char* addr_assign_type_ptr = &ptr->addr_assign_type;
	struct rtnl_link_ops const** rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	*netdev_ops_ptr = glue_unpack(msg, struct net_device_ops const*);
	if (*netdev_ops_ptr) {
		struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
		callee_unmarshal_kernel___global_netdev_ops__in(msg, writable);
	}

	*ethtool_ops_ptr = glue_unpack(msg, struct ethtool_ops const*);
	if (*ethtool_ops_ptr) {
		struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
		callee_unmarshal_kernel___global_ethtool_ops__in(msg, writable);
	}

	*flags_ptr = glue_unpack(msg, unsigned int);
	*priv_flags_ptr = glue_unpack(msg, unsigned int);
	*addr_assign_type_ptr = glue_unpack(msg, unsigned char);
	*rtnl_link_ops_ptr = glue_unpack(msg, struct rtnl_link_ops const*);
	if (*rtnl_link_ops_ptr) {
		struct rtnl_link_ops* writable = (struct rtnl_link_ops*)*rtnl_link_ops_ptr;
		callee_unmarshal_kernel___global_rtnl_link_ops__in(msg, writable);
	}

}

void callee_marshal_kernel__register_netdevice__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	unsigned long long const* features_ptr = &ptr->features;
	unsigned long long const* hw_features_ptr = &ptr->hw_features;
	unsigned long long const* hw_enc_features_ptr = &ptr->hw_enc_features;
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	struct rtnl_link_ops const* const* rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	glue_pack(msg, *features_ptr);
	glue_pack(msg, *hw_features_ptr);
	glue_pack(msg, *hw_enc_features_ptr);
	if (*netdev_ops_ptr) {
		callee_marshal_kernel___global_netdev_ops__in(msg, *netdev_ops_ptr);
	}

	if (*ethtool_ops_ptr) {
		callee_marshal_kernel___global_ethtool_ops__in(msg, *ethtool_ops_ptr);
	}

	if (*rtnl_link_ops_ptr) {
		callee_marshal_kernel___global_rtnl_link_ops__in(msg, *rtnl_link_ops_ptr);
	}

}

void caller_unmarshal_kernel__register_netdevice__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	unsigned long long* features_ptr = &ptr->features;
	unsigned long long* hw_features_ptr = &ptr->hw_features;
	unsigned long long* hw_enc_features_ptr = &ptr->hw_enc_features;
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	struct rtnl_link_ops const** rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	*features_ptr = glue_unpack(msg, unsigned long long);
	*hw_features_ptr = glue_unpack(msg, unsigned long long);
	*hw_enc_features_ptr = glue_unpack(msg, unsigned long long);
	if (*netdev_ops_ptr) {
		struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
		caller_unmarshal_kernel___global_netdev_ops__in(msg, writable);
	}

	if (*ethtool_ops_ptr) {
		struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
		caller_unmarshal_kernel___global_ethtool_ops__in(msg, writable);
	}

	if (*rtnl_link_ops_ptr) {
		struct rtnl_link_ops* writable = (struct rtnl_link_ops*)*rtnl_link_ops_ptr;
		caller_unmarshal_kernel___global_rtnl_link_ops__in(msg, writable);
	}

}

void caller_marshal_kernel___global_ethtool_ops__in(
	struct glue_message* msg,
	struct ethtool_ops const* ptr)
{
	fptr_get_drvinfo const* get_drvinfo_ptr = &ptr->get_drvinfo;
	
	glue_pack(msg, *get_drvinfo_ptr);
}

void callee_unmarshal_kernel___global_ethtool_ops__in(
	struct glue_message* msg,
	struct ethtool_ops* ptr)
{
	fptr_get_drvinfo* get_drvinfo_ptr = &ptr->get_drvinfo;
	
	*get_drvinfo_ptr = glue_unpack_rpc_ptr(msg, get_drvinfo);
}

void callee_marshal_kernel___global_ethtool_ops__in(
	struct glue_message* msg,
	struct ethtool_ops const* ptr)
{
	
}

void caller_unmarshal_kernel___global_ethtool_ops__in(
	struct glue_message* msg,
	struct ethtool_ops* ptr)
{
	
}

void caller_marshal_kernel___global_rtnl_link_ops__in(
	struct glue_message* msg,
	struct rtnl_link_ops const* ptr)
{
	fptr_setup const* setup_ptr = &ptr->setup;
	fptr_validate const* validate_ptr = &ptr->validate;
	
	glue_pack(msg, *setup_ptr);
	glue_pack(msg, *validate_ptr);
}

void callee_unmarshal_kernel___global_rtnl_link_ops__in(
	struct glue_message* msg,
	struct rtnl_link_ops* ptr)
{
	fptr_setup* setup_ptr = &ptr->setup;
	fptr_validate* validate_ptr = &ptr->validate;
	
	*setup_ptr = glue_unpack_rpc_ptr(msg, setup);
	*validate_ptr = glue_unpack_rpc_ptr(msg, validate);
}

void callee_marshal_kernel___global_rtnl_link_ops__in(
	struct glue_message* msg,
	struct rtnl_link_ops const* ptr)
{
	
}

void caller_unmarshal_kernel___global_rtnl_link_ops__in(
	struct glue_message* msg,
	struct rtnl_link_ops* ptr)
{
	
}

void caller_marshal_kernel__consume_skb__skb__in(
	struct glue_message* msg,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	
	glue_pack(msg, *len_ptr);
}

void callee_unmarshal_kernel__consume_skb__skb__in(
	struct glue_message* msg,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	
	*len_ptr = glue_unpack(msg, unsigned int);
}

void callee_marshal_kernel__consume_skb__skb__in(
	struct glue_message* msg,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel__consume_skb__skb__in(
	struct glue_message* msg,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	struct rtnl_link_ops const* const* rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	if (*netdev_ops_ptr) {
		caller_marshal_kernel___global_netdev_ops__out(msg, *netdev_ops_ptr);
	}

	if (*ethtool_ops_ptr) {
		caller_marshal_kernel___global_ethtool_ops__out(msg, *ethtool_ops_ptr);
	}

	if (*rtnl_link_ops_ptr) {
		caller_marshal_kernel___global_rtnl_link_ops__out(msg, *rtnl_link_ops_ptr);
	}

}

void callee_unmarshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	struct glue_message* msg,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	struct rtnl_link_ops const** rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	if (*netdev_ops_ptr) {
		struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
		callee_unmarshal_kernel___global_netdev_ops__out(msg, writable);
	}

	if (*ethtool_ops_ptr) {
		struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
		callee_unmarshal_kernel___global_ethtool_ops__out(msg, writable);
	}

	if (*rtnl_link_ops_ptr) {
		struct rtnl_link_ops* writable = (struct rtnl_link_ops*)*rtnl_link_ops_ptr;
		callee_unmarshal_kernel___global_rtnl_link_ops__out(msg, writable);
	}

}

void callee_marshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	unsigned long long const* features_ptr = &ptr->features;
	unsigned long long const* hw_features_ptr = &ptr->hw_features;
	unsigned long long const* hw_enc_features_ptr = &ptr->hw_enc_features;
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int const* flags_ptr = &ptr->flags;
	unsigned int const* priv_flags_ptr = &ptr->priv_flags;
	unsigned char const* addr_assign_type_ptr = &ptr->addr_assign_type;
	struct rtnl_link_ops const* const* rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	glue_pack(msg, *features_ptr);
	glue_pack(msg, *hw_features_ptr);
	glue_pack(msg, *hw_enc_features_ptr);
	glue_pack_shadow(msg, *netdev_ops_ptr);
	if (*netdev_ops_ptr) {
		callee_marshal_kernel___global_netdev_ops__out(msg, *netdev_ops_ptr);
	}

	glue_pack_shadow(msg, *ethtool_ops_ptr);
	if (*ethtool_ops_ptr) {
		callee_marshal_kernel___global_ethtool_ops__out(msg, *ethtool_ops_ptr);
	}

	glue_pack(msg, *flags_ptr);
	glue_pack(msg, *priv_flags_ptr);
	glue_pack(msg, *addr_assign_type_ptr);
	glue_pack_shadow(msg, *rtnl_link_ops_ptr);
	if (*rtnl_link_ops_ptr) {
		callee_marshal_kernel___global_rtnl_link_ops__out(msg, *rtnl_link_ops_ptr);
	}

}

void caller_unmarshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	struct glue_message* msg,
	struct net_device* ptr)
{
	unsigned long long* features_ptr = &ptr->features;
	unsigned long long* hw_features_ptr = &ptr->hw_features;
	unsigned long long* hw_enc_features_ptr = &ptr->hw_enc_features;
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int* flags_ptr = &ptr->flags;
	unsigned int* priv_flags_ptr = &ptr->priv_flags;
	unsigned char* addr_assign_type_ptr = &ptr->addr_assign_type;
	struct rtnl_link_ops const** rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	*features_ptr = glue_unpack(msg, unsigned long long);
	*hw_features_ptr = glue_unpack(msg, unsigned long long);
	*hw_enc_features_ptr = glue_unpack(msg, unsigned long long);
	*netdev_ops_ptr = glue_unpack(msg, struct net_device_ops const*);
	if (*netdev_ops_ptr) {
		struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
		caller_unmarshal_kernel___global_netdev_ops__out(msg, writable);
	}

	*ethtool_ops_ptr = glue_unpack(msg, struct ethtool_ops const*);
	if (*ethtool_ops_ptr) {
		struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
		caller_unmarshal_kernel___global_ethtool_ops__out(msg, writable);
	}

	*flags_ptr = glue_unpack(msg, unsigned int);
	*priv_flags_ptr = glue_unpack(msg, unsigned int);
	*addr_assign_type_ptr = glue_unpack(msg, unsigned char);
	*rtnl_link_ops_ptr = glue_unpack(msg, struct rtnl_link_ops const*);
	if (*rtnl_link_ops_ptr) {
		struct rtnl_link_ops* writable = (struct rtnl_link_ops*)*rtnl_link_ops_ptr;
		caller_unmarshal_kernel___global_rtnl_link_ops__out(msg, writable);
	}

}

void caller_marshal_kernel___global_netdev_ops__out(
	struct glue_message* msg,
	struct net_device_ops const* ptr)
{
	
}

void callee_unmarshal_kernel___global_netdev_ops__out(
	struct glue_message* msg,
	struct net_device_ops* ptr)
{
	
}

void callee_marshal_kernel___global_netdev_ops__out(
	struct glue_message* msg,
	struct net_device_ops const* ptr)
{
	fptr_ndo_init const* ndo_init_ptr = &ptr->ndo_init;
	fptr_ndo_uninit const* ndo_uninit_ptr = &ptr->ndo_uninit;
	fptr_ndo_start_xmit const* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_ndo_set_rx_mode const* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_ndo_get_stats64 const* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_ndo_change_carrier const* ndo_change_carrier_ptr = &ptr->ndo_change_carrier;
	
	glue_pack(msg, *ndo_init_ptr);
	glue_pack(msg, *ndo_uninit_ptr);
	glue_pack(msg, *ndo_start_xmit_ptr);
	glue_pack(msg, *ndo_set_rx_mode_ptr);
	glue_pack(msg, *ndo_get_stats64_ptr);
	glue_pack(msg, *ndo_change_carrier_ptr);
}

void caller_unmarshal_kernel___global_netdev_ops__out(
	struct glue_message* msg,
	struct net_device_ops* ptr)
{
	fptr_ndo_init* ndo_init_ptr = &ptr->ndo_init;
	fptr_ndo_uninit* ndo_uninit_ptr = &ptr->ndo_uninit;
	fptr_ndo_start_xmit* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_ndo_set_rx_mode* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_ndo_get_stats64* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_ndo_change_carrier* ndo_change_carrier_ptr = &ptr->ndo_change_carrier;
	
	*ndo_init_ptr = glue_unpack_rpc_ptr(msg, ndo_init);
	*ndo_uninit_ptr = glue_unpack_rpc_ptr(msg, ndo_uninit);
	*ndo_start_xmit_ptr = glue_unpack_rpc_ptr(msg, ndo_start_xmit);
	*ndo_set_rx_mode_ptr = glue_unpack_rpc_ptr(msg, ndo_set_rx_mode);
	*ndo_get_stats64_ptr = glue_unpack_rpc_ptr(msg, ndo_get_stats64);
	*ndo_change_carrier_ptr = glue_unpack_rpc_ptr(msg, ndo_change_carrier);
}

void caller_marshal_kernel___global_ethtool_ops__out(
	struct glue_message* msg,
	struct ethtool_ops const* ptr)
{
	
}

void callee_unmarshal_kernel___global_ethtool_ops__out(
	struct glue_message* msg,
	struct ethtool_ops* ptr)
{
	
}

void callee_marshal_kernel___global_ethtool_ops__out(
	struct glue_message* msg,
	struct ethtool_ops const* ptr)
{
	fptr_get_drvinfo const* get_drvinfo_ptr = &ptr->get_drvinfo;
	
	glue_pack(msg, *get_drvinfo_ptr);
}

void caller_unmarshal_kernel___global_ethtool_ops__out(
	struct glue_message* msg,
	struct ethtool_ops* ptr)
{
	fptr_get_drvinfo* get_drvinfo_ptr = &ptr->get_drvinfo;
	
	*get_drvinfo_ptr = glue_unpack_rpc_ptr(msg, get_drvinfo);
}

void caller_marshal_kernel___global_rtnl_link_ops__out(
	struct glue_message* msg,
	struct rtnl_link_ops const* ptr)
{
	
}

void callee_unmarshal_kernel___global_rtnl_link_ops__out(
	struct glue_message* msg,
	struct rtnl_link_ops* ptr)
{
	
}

void callee_marshal_kernel___global_rtnl_link_ops__out(
	struct glue_message* msg,
	struct rtnl_link_ops const* ptr)
{
	fptr_setup const* setup_ptr = &ptr->setup;
	fptr_validate const* validate_ptr = &ptr->validate;
	
	glue_pack(msg, *setup_ptr);
	glue_pack(msg, *validate_ptr);
}

void caller_unmarshal_kernel___global_rtnl_link_ops__out(
	struct glue_message* msg,
	struct rtnl_link_ops* ptr)
{
	fptr_setup* setup_ptr = &ptr->setup;
	fptr_validate* validate_ptr = &ptr->validate;
	
	*setup_ptr = glue_unpack_rpc_ptr(msg, setup);
	*validate_ptr = glue_unpack_rpc_ptr(msg, validate);
}

void caller_marshal_kernel__free_netdev__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	unsigned long long const* features_ptr = &ptr->features;
	unsigned long long const* hw_features_ptr = &ptr->hw_features;
	unsigned long long const* hw_enc_features_ptr = &ptr->hw_enc_features;
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int const* flags_ptr = &ptr->flags;
	unsigned int const* priv_flags_ptr = &ptr->priv_flags;
	unsigned char const* addr_assign_type_ptr = &ptr->addr_assign_type;
	struct rtnl_link_ops const* const* rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	glue_pack(msg, *features_ptr);
	glue_pack(msg, *hw_features_ptr);
	glue_pack(msg, *hw_enc_features_ptr);
	glue_pack_shadow(msg, *netdev_ops_ptr);
	if (*netdev_ops_ptr) {
		caller_marshal_kernel___global_netdev_ops__in(msg, *netdev_ops_ptr);
	}

	glue_pack_shadow(msg, *ethtool_ops_ptr);
	if (*ethtool_ops_ptr) {
		caller_marshal_kernel___global_ethtool_ops__in(msg, *ethtool_ops_ptr);
	}

	glue_pack(msg, *flags_ptr);
	glue_pack(msg, *priv_flags_ptr);
	glue_pack(msg, *addr_assign_type_ptr);
	glue_pack_shadow(msg, *rtnl_link_ops_ptr);
	if (*rtnl_link_ops_ptr) {
		caller_marshal_kernel___global_rtnl_link_ops__in(msg, *rtnl_link_ops_ptr);
	}

}

void callee_unmarshal_kernel__free_netdev__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	unsigned long long* features_ptr = &ptr->features;
	unsigned long long* hw_features_ptr = &ptr->hw_features;
	unsigned long long* hw_enc_features_ptr = &ptr->hw_enc_features;
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int* flags_ptr = &ptr->flags;
	unsigned int* priv_flags_ptr = &ptr->priv_flags;
	unsigned char* addr_assign_type_ptr = &ptr->addr_assign_type;
	struct rtnl_link_ops const** rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	*features_ptr = glue_unpack(msg, unsigned long long);
	*hw_features_ptr = glue_unpack(msg, unsigned long long);
	*hw_enc_features_ptr = glue_unpack(msg, unsigned long long);
	*netdev_ops_ptr = glue_unpack(msg, struct net_device_ops const*);
	if (*netdev_ops_ptr) {
		struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
		callee_unmarshal_kernel___global_netdev_ops__in(msg, writable);
	}

	*ethtool_ops_ptr = glue_unpack(msg, struct ethtool_ops const*);
	if (*ethtool_ops_ptr) {
		struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
		callee_unmarshal_kernel___global_ethtool_ops__in(msg, writable);
	}

	*flags_ptr = glue_unpack(msg, unsigned int);
	*priv_flags_ptr = glue_unpack(msg, unsigned int);
	*addr_assign_type_ptr = glue_unpack(msg, unsigned char);
	*rtnl_link_ops_ptr = glue_unpack(msg, struct rtnl_link_ops const*);
	if (*rtnl_link_ops_ptr) {
		struct rtnl_link_ops* writable = (struct rtnl_link_ops*)*rtnl_link_ops_ptr;
		callee_unmarshal_kernel___global_rtnl_link_ops__in(msg, writable);
	}

}

void callee_marshal_kernel__free_netdev__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	struct rtnl_link_ops const* const* rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	if (*netdev_ops_ptr) {
		callee_marshal_kernel___global_netdev_ops__in(msg, *netdev_ops_ptr);
	}

	if (*ethtool_ops_ptr) {
		callee_marshal_kernel___global_ethtool_ops__in(msg, *ethtool_ops_ptr);
	}

	if (*rtnl_link_ops_ptr) {
		callee_marshal_kernel___global_rtnl_link_ops__in(msg, *rtnl_link_ops_ptr);
	}

}

void caller_unmarshal_kernel__free_netdev__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	struct rtnl_link_ops const** rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	if (*netdev_ops_ptr) {
		struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
		caller_unmarshal_kernel___global_netdev_ops__in(msg, writable);
	}

	if (*ethtool_ops_ptr) {
		struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
		caller_unmarshal_kernel___global_ethtool_ops__in(msg, writable);
	}

	if (*rtnl_link_ops_ptr) {
		struct rtnl_link_ops* writable = (struct rtnl_link_ops*)*rtnl_link_ops_ptr;
		caller_unmarshal_kernel___global_rtnl_link_ops__in(msg, writable);
	}

}

void caller_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	struct glue_message* msg,
	struct rtnl_link_ops const* ptr)
{
	char const* const* kind_ptr = &ptr->kind;
	fptr_setup const* setup_ptr = &ptr->setup;
	fptr_validate const* validate_ptr = &ptr->validate;
	
	glue_pack(msg, *kind_ptr);
	if (*kind_ptr) {
		size_t i, len;
		char const* array = *kind_ptr;
		for (len = 0; array[len]; ++len);
		glue_pack(msg, len + 1);
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(msg, *element);
		}

	}

	glue_pack(msg, *setup_ptr);
	glue_pack(msg, *validate_ptr);
}

void callee_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	struct glue_message* msg,
	struct rtnl_link_ops* ptr)
{
	char const** kind_ptr = &ptr->kind;
	fptr_setup* setup_ptr = &ptr->setup;
	fptr_validate* validate_ptr = &ptr->validate;
	
	*kind_ptr = glue_unpack_new_shadow(msg, char const*, sizeof(char) * glue_peek(msg));
	if (*kind_ptr) {
		char* writable = (char*)*kind_ptr;
		size_t i, len;
		char* array = writable;
		len = glue_unpack(msg, size_t) - 1;
		array[len] = '\0';
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(msg, char);
		}

	}

	*setup_ptr = glue_unpack_rpc_ptr(msg, setup);
	*validate_ptr = glue_unpack_rpc_ptr(msg, validate);
}

void callee_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	struct glue_message* msg,
	struct rtnl_link_ops const* ptr)
{
	char const* const* kind_ptr = &ptr->kind;
	
	(void)kind_ptr;
}

void caller_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	struct glue_message* msg,
	struct rtnl_link_ops* ptr)
{
	char const** kind_ptr = &ptr->kind;
	
	(void)kind_ptr;
}

void caller_marshal_kernel__ether_setup__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ether_setup__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ether_setup__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	unsigned int const* flags_ptr = &ptr->flags;
	unsigned int const* priv_flags_ptr = &ptr->priv_flags;
	
	glue_pack(msg, *flags_ptr);
	glue_pack(msg, *priv_flags_ptr);
}

void caller_unmarshal_kernel__ether_setup__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	unsigned int* flags_ptr = &ptr->flags;
	unsigned int* priv_flags_ptr = &ptr->priv_flags;
	
	*flags_ptr = glue_unpack(msg, unsigned int);
	*priv_flags_ptr = glue_unpack(msg, unsigned int);
}

void caller_marshal_kernel__eth_validate_addr__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__eth_validate_addr__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__eth_validate_addr__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__eth_validate_addr__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__eth_mac_addr__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	unsigned int const* priv_flags_ptr = &ptr->priv_flags;
	
	glue_pack(msg, *priv_flags_ptr);
}

void callee_unmarshal_kernel__eth_mac_addr__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	unsigned int* priv_flags_ptr = &ptr->priv_flags;
	
	*priv_flags_ptr = glue_unpack(msg, unsigned int);
}

void callee_marshal_kernel__eth_mac_addr__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__eth_mac_addr__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__eth_mac_addr__p__in(
	struct glue_message* msg,
	struct sockaddr const* ptr)
{
	
}

void callee_unmarshal_kernel__eth_mac_addr__p__in(
	struct glue_message* msg,
	struct sockaddr* ptr)
{
	
}

void callee_marshal_kernel__eth_mac_addr__p__in(
	struct glue_message* msg,
	struct sockaddr const* ptr)
{
	
}

void caller_unmarshal_kernel__eth_mac_addr__p__in(
	struct glue_message* msg,
	struct sockaddr* ptr)
{
	
}

void caller_marshal_kernel__setup__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned char const* addr_assign_type_ptr = &ptr->addr_assign_type;
	
	glue_pack_shadow(msg, *netdev_ops_ptr);
	if (*netdev_ops_ptr) {
		caller_marshal_kernel___global_netdev_ops__out(msg, *netdev_ops_ptr);
	}

	glue_pack_shadow(msg, *ethtool_ops_ptr);
	if (*ethtool_ops_ptr) {
		caller_marshal_kernel___global_ethtool_ops__out(msg, *ethtool_ops_ptr);
	}

	glue_pack(msg, *addr_assign_type_ptr);
}

void callee_unmarshal_kernel__setup__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned char* addr_assign_type_ptr = &ptr->addr_assign_type;
	
	*netdev_ops_ptr = glue_unpack(msg, struct net_device_ops const*);
	if (*netdev_ops_ptr) {
		struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
		callee_unmarshal_kernel___global_netdev_ops__out(msg, writable);
	}

	*ethtool_ops_ptr = glue_unpack(msg, struct ethtool_ops const*);
	if (*ethtool_ops_ptr) {
		struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
		callee_unmarshal_kernel___global_ethtool_ops__out(msg, writable);
	}

	*addr_assign_type_ptr = glue_unpack(msg, unsigned char);
}

void callee_marshal_kernel__setup__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	unsigned long long const* features_ptr = &ptr->features;
	unsigned long long const* hw_features_ptr = &ptr->hw_features;
	unsigned long long const* hw_enc_features_ptr = &ptr->hw_enc_features;
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int const* flags_ptr = &ptr->flags;
	unsigned int const* priv_flags_ptr = &ptr->priv_flags;
	
	glue_pack(msg, *features_ptr);
	glue_pack(msg, *hw_features_ptr);
	glue_pack(msg, *hw_enc_features_ptr);
	if (*netdev_ops_ptr) {
		callee_marshal_kernel___global_netdev_ops__out(msg, *netdev_ops_ptr);
	}

	if (*ethtool_ops_ptr) {
		callee_marshal_kernel___global_ethtool_ops__out(msg, *ethtool_ops_ptr);
	}

	glue_pack(msg, *flags_ptr);
	glue_pack(msg, *priv_flags_ptr);
}

void caller_unmarshal_kernel__setup__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	unsigned long long* features_ptr = &ptr->features;
	unsigned long long* hw_features_ptr = &ptr->hw_features;
	unsigned long long* hw_enc_features_ptr = &ptr->hw_enc_features;
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int* flags_ptr = &ptr->flags;
	unsigned int* priv_flags_ptr = &ptr->priv_flags;
	
	*features_ptr = glue_unpack(msg, unsigned long long);
	*hw_features_ptr = glue_unpack(msg, unsigned long long);
	*hw_enc_features_ptr = glue_unpack(msg, unsigned long long);
	if (*netdev_ops_ptr) {
		struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
		caller_unmarshal_kernel___global_netdev_ops__out(msg, writable);
	}

	if (*ethtool_ops_ptr) {
		struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
		caller_unmarshal_kernel___global_ethtool_ops__out(msg, writable);
	}

	*flags_ptr = glue_unpack(msg, unsigned int);
	*priv_flags_ptr = glue_unpack(msg, unsigned int);
}

void caller_marshal_kernel__get_drvinfo__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__get_drvinfo__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__get_drvinfo__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__get_drvinfo__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__get_drvinfo__info__in(
	struct glue_message* msg,
	struct ethtool_drvinfo const* ptr)
{
	
}

void callee_unmarshal_kernel__get_drvinfo__info__in(
	struct glue_message* msg,
	struct ethtool_drvinfo* ptr)
{
	
}

void callee_marshal_kernel__get_drvinfo__info__in(
	struct glue_message* msg,
	struct ethtool_drvinfo const* ptr)
{
	
}

void caller_unmarshal_kernel__get_drvinfo__info__in(
	struct glue_message* msg,
	struct ethtool_drvinfo* ptr)
{
	
}

void caller_marshal_kernel__ndo_init__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_init__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ndo_init__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_init__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ndo_uninit__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_uninit__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ndo_uninit__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_uninit__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ndo_start_xmit__skb__in(
	struct glue_message* msg,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	
	glue_pack(msg, *len_ptr);
}

void callee_unmarshal_kernel__ndo_start_xmit__skb__in(
	struct glue_message* msg,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	
	*len_ptr = glue_unpack(msg, unsigned int);
}

void callee_marshal_kernel__ndo_start_xmit__skb__in(
	struct glue_message* msg,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_start_xmit__skb__in(
	struct glue_message* msg,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel__ndo_start_xmit__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_start_xmit__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ndo_start_xmit__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_start_xmit__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ndo_set_rx_mode__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_set_rx_mode__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ndo_set_rx_mode__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_set_rx_mode__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	struct glue_message* msg,
	struct rtnl_link_stats64 const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	struct glue_message* msg,
	struct rtnl_link_stats64* ptr)
{
	
}

void callee_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	struct glue_message* msg,
	struct rtnl_link_stats64 const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	struct glue_message* msg,
	struct rtnl_link_stats64* ptr)
{
	
}

void caller_marshal_kernel__ndo_get_stats64__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_get_stats64__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ndo_get_stats64__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_get_stats64__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ndo_get_stats64__stats__in(
	struct glue_message* msg,
	struct rtnl_link_stats64 const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_get_stats64__stats__in(
	struct glue_message* msg,
	struct rtnl_link_stats64* ptr)
{
	
}

void callee_marshal_kernel__ndo_get_stats64__stats__in(
	struct glue_message* msg,
	struct rtnl_link_stats64 const* ptr)
{
	unsigned long long const* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long const* tx_bytes_ptr = &ptr->tx_bytes;
	
	glue_pack(msg, *tx_packets_ptr);
	glue_pack(msg, *tx_bytes_ptr);
}

void caller_unmarshal_kernel__ndo_get_stats64__stats__in(
	struct glue_message* msg,
	struct rtnl_link_stats64* ptr)
{
	unsigned long long* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long* tx_bytes_ptr = &ptr->tx_bytes;
	
	*tx_packets_ptr = glue_unpack(msg, unsigned long long);
	*tx_bytes_ptr = glue_unpack(msg, unsigned long long);
}

void caller_marshal_kernel__ndo_change_carrier__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	
	glue_pack_shadow(msg, *netdev_ops_ptr);
	if (*netdev_ops_ptr) {
		caller_marshal_kernel___global_netdev_ops__in(msg, *netdev_ops_ptr);
	}

}

void callee_unmarshal_kernel__ndo_change_carrier__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	
	*netdev_ops_ptr = glue_unpack(msg, struct net_device_ops const*);
	if (*netdev_ops_ptr) {
		struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
		callee_unmarshal_kernel___global_netdev_ops__in(msg, writable);
	}

}

void callee_marshal_kernel__ndo_change_carrier__dev__in(
	struct glue_message* msg,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	
	if (*netdev_ops_ptr) {
		callee_marshal_kernel___global_netdev_ops__in(msg, *netdev_ops_ptr);
	}

}

void caller_unmarshal_kernel__ndo_change_carrier__dev__in(
	struct glue_message* msg,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	
	if (*netdev_ops_ptr) {
		struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
		caller_unmarshal_kernel___global_netdev_ops__in(msg, writable);
	}

}

void caller_marshal_kernel__validate__tb__in(
	struct glue_message* msg,
	struct nlattr const* ptr)
{
	
}

void callee_unmarshal_kernel__validate__tb__in(
	struct glue_message* msg,
	struct nlattr* ptr)
{
	
}

void callee_marshal_kernel__validate__tb__in(
	struct glue_message* msg,
	struct nlattr const* ptr)
{
	
}

void caller_unmarshal_kernel__validate__tb__in(
	struct glue_message* msg,
	struct nlattr* ptr)
{
	
}

void caller_marshal_kernel__validate__data__in(
	struct glue_message* msg,
	struct nlattr const* ptr)
{
	
}

void callee_unmarshal_kernel__validate__data__in(
	struct glue_message* msg,
	struct nlattr* ptr)
{
	
}

void callee_marshal_kernel__validate__data__in(
	struct glue_message* msg,
	struct nlattr const* ptr)
{
	
}

void caller_unmarshal_kernel__validate__data__in(
	struct glue_message* msg,
	struct nlattr* ptr)
{
	
}

