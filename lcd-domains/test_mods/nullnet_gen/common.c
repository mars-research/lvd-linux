#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__netif_carrier_on__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	
	{
		glue_pack_shadow(pos, msg, ext, *netdev_ops_ptr);
		if (*netdev_ops_ptr) {
			caller_marshal_kernel___global_netdev_ops__in(pos, msg, ext, *netdev_ops_ptr);
		}

	}

}

void callee_unmarshal_kernel__netif_carrier_on__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	
	{
		*netdev_ops_ptr = glue_unpack(pos, msg, ext, struct net_device_ops const*);
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			callee_unmarshal_kernel___global_netdev_ops__in(pos, msg, ext, writable);
		}

	}

}

void callee_marshal_kernel__netif_carrier_on__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	
	{
		if (*netdev_ops_ptr) {
			callee_marshal_kernel___global_netdev_ops__in(pos, msg, ext, *netdev_ops_ptr);
		}

	}

}

void caller_unmarshal_kernel__netif_carrier_on__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	
	{
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			caller_unmarshal_kernel___global_netdev_ops__in(pos, msg, ext, writable);
		}

	}

}

void caller_marshal_kernel___global_netdev_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops const* ptr)
{
	fptr_ndo_init const* ndo_init_ptr = &ptr->ndo_init;
	fptr_ndo_uninit const* ndo_uninit_ptr = &ptr->ndo_uninit;
	fptr_ndo_start_xmit const* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_ndo_set_rx_mode const* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_ndo_get_stats64 const* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_ndo_change_carrier const* ndo_change_carrier_ptr = &ptr->ndo_change_carrier;
	
	{
		glue_pack(pos, msg, ext, *ndo_init_ptr);
	}

	{
		glue_pack(pos, msg, ext, *ndo_uninit_ptr);
	}

	{
		glue_pack(pos, msg, ext, *ndo_start_xmit_ptr);
	}

	{
		glue_pack(pos, msg, ext, *ndo_set_rx_mode_ptr);
	}

	{
		glue_pack(pos, msg, ext, *ndo_get_stats64_ptr);
	}

	{
		glue_pack(pos, msg, ext, *ndo_change_carrier_ptr);
	}

}

void callee_unmarshal_kernel___global_netdev_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops* ptr)
{
	fptr_ndo_init* ndo_init_ptr = &ptr->ndo_init;
	fptr_ndo_uninit* ndo_uninit_ptr = &ptr->ndo_uninit;
	fptr_ndo_start_xmit* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_ndo_set_rx_mode* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_ndo_get_stats64* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_ndo_change_carrier* ndo_change_carrier_ptr = &ptr->ndo_change_carrier;
	
	{
		*ndo_init_ptr = glue_unpack_rpc_ptr(pos, msg, ext, ndo_init);
	}

	{
		*ndo_uninit_ptr = glue_unpack_rpc_ptr(pos, msg, ext, ndo_uninit);
	}

	{
		*ndo_start_xmit_ptr = glue_unpack_rpc_ptr(pos, msg, ext, ndo_start_xmit);
	}

	{
		*ndo_set_rx_mode_ptr = glue_unpack_rpc_ptr(pos, msg, ext, ndo_set_rx_mode);
	}

	{
		*ndo_get_stats64_ptr = glue_unpack_rpc_ptr(pos, msg, ext, ndo_get_stats64);
	}

	{
		*ndo_change_carrier_ptr = glue_unpack_rpc_ptr(pos, msg, ext, ndo_change_carrier);
	}

}

void callee_marshal_kernel___global_netdev_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops const* ptr)
{
	
}

void caller_unmarshal_kernel___global_netdev_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops* ptr)
{
	
}

void caller_marshal_kernel__netif_carrier_off__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__netif_carrier_off__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__register_netdevice__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int const* flags_ptr = &ptr->flags;
	unsigned int const* priv_flags_ptr = &ptr->priv_flags;
	unsigned char const* addr_assign_type_ptr = &ptr->addr_assign_type;
	struct rtnl_link_ops const* const* rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	{
		glue_pack(pos, msg, ext, *netdev_ops_ptr);
		if (*netdev_ops_ptr) {
			caller_marshal_kernel___global_netdev_ops__in(pos, msg, ext, *netdev_ops_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *ethtool_ops_ptr);
		if (*ethtool_ops_ptr) {
			caller_marshal_kernel___global_ethtool_ops__in(pos, msg, ext, *ethtool_ops_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *flags_ptr);
	}

	{
		glue_pack(pos, msg, ext, *priv_flags_ptr);
	}

	{
		glue_pack(pos, msg, ext, *addr_assign_type_ptr);
	}

	{
		glue_pack(pos, msg, ext, *rtnl_link_ops_ptr);
		if (*rtnl_link_ops_ptr) {
			caller_marshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, *rtnl_link_ops_ptr);
		}

	}

}

void callee_unmarshal_kernel__register_netdevice__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int* flags_ptr = &ptr->flags;
	unsigned int* priv_flags_ptr = &ptr->priv_flags;
	unsigned char* addr_assign_type_ptr = &ptr->addr_assign_type;
	struct rtnl_link_ops const** rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	{
		*netdev_ops_ptr = glue_unpack_shadow(pos, msg, ext, struct net_device_ops const*);
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			callee_unmarshal_kernel___global_netdev_ops__in(pos, msg, ext, writable);
		}

	}

	{
		*ethtool_ops_ptr = glue_unpack_shadow(pos, msg, ext, struct ethtool_ops const*);
		if (*ethtool_ops_ptr) {
			struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
			callee_unmarshal_kernel___global_ethtool_ops__in(pos, msg, ext, writable);
		}

	}

	{
		*flags_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*priv_flags_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*addr_assign_type_ptr = glue_unpack(pos, msg, ext, unsigned char);
	}

	{
		*rtnl_link_ops_ptr = glue_unpack_shadow(pos, msg, ext, struct rtnl_link_ops const*);
		if (*rtnl_link_ops_ptr) {
			struct rtnl_link_ops* writable = (struct rtnl_link_ops*)*rtnl_link_ops_ptr;
			callee_unmarshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, writable);
		}

	}

}

void callee_marshal_kernel__register_netdevice__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	unsigned long long const* features_ptr = &ptr->features;
	unsigned long long const* hw_features_ptr = &ptr->hw_features;
	unsigned long long const* hw_enc_features_ptr = &ptr->hw_enc_features;
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	struct rtnl_link_ops const* const* rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	{
		glue_pack(pos, msg, ext, *features_ptr);
	}

	{
		glue_pack(pos, msg, ext, *hw_features_ptr);
	}

	{
		glue_pack(pos, msg, ext, *hw_enc_features_ptr);
	}

	{
		if (*netdev_ops_ptr) {
			callee_marshal_kernel___global_netdev_ops__in(pos, msg, ext, *netdev_ops_ptr);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			callee_marshal_kernel___global_ethtool_ops__in(pos, msg, ext, *ethtool_ops_ptr);
		}

	}

	{
		if (*rtnl_link_ops_ptr) {
			callee_marshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, *rtnl_link_ops_ptr);
		}

	}

}

void caller_unmarshal_kernel__register_netdevice__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	unsigned long long* features_ptr = &ptr->features;
	unsigned long long* hw_features_ptr = &ptr->hw_features;
	unsigned long long* hw_enc_features_ptr = &ptr->hw_enc_features;
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	struct rtnl_link_ops const** rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	{
		*features_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

	{
		*hw_features_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

	{
		*hw_enc_features_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

	{
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			caller_unmarshal_kernel___global_netdev_ops__in(pos, msg, ext, writable);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
			caller_unmarshal_kernel___global_ethtool_ops__in(pos, msg, ext, writable);
		}

	}

	{
		if (*rtnl_link_ops_ptr) {
			struct rtnl_link_ops* writable = (struct rtnl_link_ops*)*rtnl_link_ops_ptr;
			caller_unmarshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, writable);
		}

	}

}

void caller_marshal_kernel___global_ethtool_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops const* ptr)
{
	fptr_get_drvinfo const* get_drvinfo_ptr = &ptr->get_drvinfo;
	
	{
		glue_pack(pos, msg, ext, *get_drvinfo_ptr);
	}

}

void callee_unmarshal_kernel___global_ethtool_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops* ptr)
{
	fptr_get_drvinfo* get_drvinfo_ptr = &ptr->get_drvinfo;
	
	{
		*get_drvinfo_ptr = glue_unpack_rpc_ptr(pos, msg, ext, get_drvinfo);
	}

}

void callee_marshal_kernel___global_ethtool_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops const* ptr)
{
	
}

void caller_unmarshal_kernel___global_ethtool_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops* ptr)
{
	
}

void caller_marshal_kernel___global_rtnl_link_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_ops const* ptr)
{
	fptr_setup const* setup_ptr = &ptr->setup;
	fptr_validate const* validate_ptr = &ptr->validate;
	
	{
		glue_pack(pos, msg, ext, *setup_ptr);
	}

	{
		glue_pack(pos, msg, ext, *validate_ptr);
	}

}

void callee_unmarshal_kernel___global_rtnl_link_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_ops* ptr)
{
	fptr_setup* setup_ptr = &ptr->setup;
	fptr_validate* validate_ptr = &ptr->validate;
	
	{
		*setup_ptr = glue_unpack_rpc_ptr(pos, msg, ext, setup);
	}

	{
		*validate_ptr = glue_unpack_rpc_ptr(pos, msg, ext, validate);
	}

}

void callee_marshal_kernel___global_rtnl_link_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_ops const* ptr)
{
	
}

void caller_unmarshal_kernel___global_rtnl_link_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_ops* ptr)
{
	
}

void caller_marshal_kernel__consume_skb__skb__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	
	{
		glue_pack(pos, msg, ext, *len_ptr);
	}

}

void callee_unmarshal_kernel__consume_skb__skb__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	
	{
		*len_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__consume_skb__skb__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel__consume_skb__skb__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	struct rtnl_link_ops const* const* rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	{
		if (*netdev_ops_ptr) {
			caller_marshal_kernel___global_netdev_ops__in(pos, msg, ext, *netdev_ops_ptr);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			caller_marshal_kernel___global_ethtool_ops__in(pos, msg, ext, *ethtool_ops_ptr);
		}

	}

	{
		if (*rtnl_link_ops_ptr) {
			caller_marshal_kernel___global_rtnl_link_ops__out(pos, msg, ext, *rtnl_link_ops_ptr);
		}

	}

}

void callee_unmarshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	struct rtnl_link_ops const** rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	{
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			callee_unmarshal_kernel___global_netdev_ops__in(pos, msg, ext, writable);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
			callee_unmarshal_kernel___global_ethtool_ops__in(pos, msg, ext, writable);
		}

	}

	{
		if (*rtnl_link_ops_ptr) {
			struct rtnl_link_ops* writable = (struct rtnl_link_ops*)*rtnl_link_ops_ptr;
			callee_unmarshal_kernel___global_rtnl_link_ops__out(pos, msg, ext, writable);
		}

	}

}

void callee_marshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
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
	
	{
		glue_pack(pos, msg, ext, *features_ptr);
	}

	{
		glue_pack(pos, msg, ext, *hw_features_ptr);
	}

	{
		glue_pack(pos, msg, ext, *hw_enc_features_ptr);
	}

	{
		glue_pack_shadow(pos, msg, ext, *netdev_ops_ptr);
		if (*netdev_ops_ptr) {
			callee_marshal_kernel___global_netdev_ops__in(pos, msg, ext, *netdev_ops_ptr);
		}

	}

	{
		glue_pack_shadow(pos, msg, ext, *ethtool_ops_ptr);
		if (*ethtool_ops_ptr) {
			callee_marshal_kernel___global_ethtool_ops__in(pos, msg, ext, *ethtool_ops_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *flags_ptr);
	}

	{
		glue_pack(pos, msg, ext, *priv_flags_ptr);
	}

	{
		glue_pack(pos, msg, ext, *addr_assign_type_ptr);
	}

	{
		glue_pack_shadow(pos, msg, ext, *rtnl_link_ops_ptr);
		if (*rtnl_link_ops_ptr) {
			callee_marshal_kernel___global_rtnl_link_ops__out(pos, msg, ext, *rtnl_link_ops_ptr);
		}

	}

}

void caller_unmarshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
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
	
	{
		*features_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

	{
		*hw_features_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

	{
		*hw_enc_features_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

	{
		*netdev_ops_ptr = glue_unpack(pos, msg, ext, struct net_device_ops const*);
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			caller_unmarshal_kernel___global_netdev_ops__in(pos, msg, ext, writable);
		}

	}

	{
		*ethtool_ops_ptr = glue_unpack(pos, msg, ext, struct ethtool_ops const*);
		if (*ethtool_ops_ptr) {
			struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
			caller_unmarshal_kernel___global_ethtool_ops__in(pos, msg, ext, writable);
		}

	}

	{
		*flags_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*priv_flags_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*addr_assign_type_ptr = glue_unpack(pos, msg, ext, unsigned char);
	}

	{
		*rtnl_link_ops_ptr = glue_unpack(pos, msg, ext, struct rtnl_link_ops const*);
		if (*rtnl_link_ops_ptr) {
			struct rtnl_link_ops* writable = (struct rtnl_link_ops*)*rtnl_link_ops_ptr;
			caller_unmarshal_kernel___global_rtnl_link_ops__out(pos, msg, ext, writable);
		}

	}

}

void caller_marshal_kernel___global_rtnl_link_ops__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_ops const* ptr)
{
	
}

void callee_unmarshal_kernel___global_rtnl_link_ops__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_ops* ptr)
{
	
}

void callee_marshal_kernel___global_rtnl_link_ops__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_ops const* ptr)
{
	fptr_setup const* setup_ptr = &ptr->setup;
	fptr_validate const* validate_ptr = &ptr->validate;
	
	{
		glue_pack(pos, msg, ext, *setup_ptr);
	}

	{
		glue_pack(pos, msg, ext, *validate_ptr);
	}

}

void caller_unmarshal_kernel___global_rtnl_link_ops__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_ops* ptr)
{
	fptr_setup* setup_ptr = &ptr->setup;
	fptr_validate* validate_ptr = &ptr->validate;
	
	{
		*setup_ptr = glue_unpack_rpc_ptr(pos, msg, ext, setup);
	}

	{
		*validate_ptr = glue_unpack_rpc_ptr(pos, msg, ext, validate);
	}

}

void caller_marshal_kernel__free_netdev__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
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
	
	{
		glue_pack(pos, msg, ext, *features_ptr);
	}

	{
		glue_pack(pos, msg, ext, *hw_features_ptr);
	}

	{
		glue_pack(pos, msg, ext, *hw_enc_features_ptr);
	}

	{
		glue_pack_shadow(pos, msg, ext, *netdev_ops_ptr);
		if (*netdev_ops_ptr) {
			caller_marshal_kernel___global_netdev_ops__in(pos, msg, ext, *netdev_ops_ptr);
		}

	}

	{
		glue_pack_shadow(pos, msg, ext, *ethtool_ops_ptr);
		if (*ethtool_ops_ptr) {
			caller_marshal_kernel___global_ethtool_ops__in(pos, msg, ext, *ethtool_ops_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *flags_ptr);
	}

	{
		glue_pack(pos, msg, ext, *priv_flags_ptr);
	}

	{
		glue_pack(pos, msg, ext, *addr_assign_type_ptr);
	}

	{
		glue_pack_shadow(pos, msg, ext, *rtnl_link_ops_ptr);
		if (*rtnl_link_ops_ptr) {
			caller_marshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, *rtnl_link_ops_ptr);
		}

	}

}

void callee_unmarshal_kernel__free_netdev__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
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
	
	{
		*features_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

	{
		*hw_features_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

	{
		*hw_enc_features_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

	{
		*netdev_ops_ptr = glue_unpack(pos, msg, ext, struct net_device_ops const*);
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			callee_unmarshal_kernel___global_netdev_ops__in(pos, msg, ext, writable);
		}

	}

	{
		*ethtool_ops_ptr = glue_unpack(pos, msg, ext, struct ethtool_ops const*);
		if (*ethtool_ops_ptr) {
			struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
			callee_unmarshal_kernel___global_ethtool_ops__in(pos, msg, ext, writable);
		}

	}

	{
		*flags_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*priv_flags_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*addr_assign_type_ptr = glue_unpack(pos, msg, ext, unsigned char);
	}

	{
		*rtnl_link_ops_ptr = glue_unpack(pos, msg, ext, struct rtnl_link_ops const*);
		if (*rtnl_link_ops_ptr) {
			struct rtnl_link_ops* writable = (struct rtnl_link_ops*)*rtnl_link_ops_ptr;
			callee_unmarshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, writable);
		}

	}

}

void callee_marshal_kernel__free_netdev__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	struct rtnl_link_ops const* const* rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	{
		if (*netdev_ops_ptr) {
			callee_marshal_kernel___global_netdev_ops__in(pos, msg, ext, *netdev_ops_ptr);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			callee_marshal_kernel___global_ethtool_ops__in(pos, msg, ext, *ethtool_ops_ptr);
		}

	}

	{
		if (*rtnl_link_ops_ptr) {
			callee_marshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, *rtnl_link_ops_ptr);
		}

	}

}

void caller_unmarshal_kernel__free_netdev__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	struct rtnl_link_ops const** rtnl_link_ops_ptr = &ptr->rtnl_link_ops;
	
	{
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			caller_unmarshal_kernel___global_netdev_ops__in(pos, msg, ext, writable);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
			caller_unmarshal_kernel___global_ethtool_ops__in(pos, msg, ext, writable);
		}

	}

	{
		if (*rtnl_link_ops_ptr) {
			struct rtnl_link_ops* writable = (struct rtnl_link_ops*)*rtnl_link_ops_ptr;
			caller_unmarshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, writable);
		}

	}

}

void caller_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_ops const* ptr)
{
	char const* const* kind_ptr = &ptr->kind;
	fptr_setup const* setup_ptr = &ptr->setup;
	fptr_validate const* validate_ptr = &ptr->validate;
	
	{
		glue_pack(pos, msg, ext, *kind_ptr);
		if (*kind_ptr) {
			size_t i, len;
			char const* array = *kind_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(pos, msg, ext, *element);
			}

		}

	}

	{
		glue_pack(pos, msg, ext, *setup_ptr);
	}

	{
		glue_pack(pos, msg, ext, *validate_ptr);
	}

}

void callee_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_ops* ptr)
{
	char const** kind_ptr = &ptr->kind;
	fptr_setup* setup_ptr = &ptr->setup;
	fptr_validate* validate_ptr = &ptr->validate;
	
	{
		*kind_ptr = glue_unpack_new_shadow(pos, msg, ext, char const*, sizeof(char) * glue_peek(pos, msg, ext));
		if (*kind_ptr) {
			char* writable = (char*)*kind_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(pos, msg, ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(pos, msg, ext, char);
			}

		}

	}

	{
		*setup_ptr = glue_unpack_rpc_ptr(pos, msg, ext, setup);
	}

	{
		*validate_ptr = glue_unpack_rpc_ptr(pos, msg, ext, validate);
	}

}

void callee_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_ops const* ptr)
{
	char const* const* kind_ptr = &ptr->kind;
	
	{
		(void)kind_ptr;
	}

}

void caller_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_ops* ptr)
{
	char const** kind_ptr = &ptr->kind;
	
	{
		(void)kind_ptr;
	}

}

void caller_marshal_kernel__ether_setup__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ether_setup__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ether_setup__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	unsigned int const* flags_ptr = &ptr->flags;
	unsigned int const* priv_flags_ptr = &ptr->priv_flags;
	
	{
		glue_pack(pos, msg, ext, *flags_ptr);
	}

	{
		glue_pack(pos, msg, ext, *priv_flags_ptr);
	}

}

void caller_unmarshal_kernel__ether_setup__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	unsigned int* flags_ptr = &ptr->flags;
	unsigned int* priv_flags_ptr = &ptr->priv_flags;
	
	{
		*flags_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*priv_flags_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel__eth_validate_addr__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__eth_validate_addr__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__eth_mac_addr__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	unsigned int const* priv_flags_ptr = &ptr->priv_flags;
	
	{
		glue_pack(pos, msg, ext, *priv_flags_ptr);
	}

}

void callee_unmarshal_kernel__eth_mac_addr__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	unsigned int* priv_flags_ptr = &ptr->priv_flags;
	
	{
		*priv_flags_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__eth_mac_addr__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__eth_mac_addr__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__eth_mac_addr__p__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sockaddr const* ptr)
{
	
}

void callee_unmarshal_kernel__eth_mac_addr__p__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sockaddr* ptr)
{
	
}

void callee_marshal_kernel__eth_mac_addr__p__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sockaddr const* ptr)
{
	
}

void caller_unmarshal_kernel__eth_mac_addr__p__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sockaddr* ptr)
{
	
}

void caller_marshal_kernel__setup__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned char const* addr_assign_type_ptr = &ptr->addr_assign_type;
	
	{
		if (*netdev_ops_ptr) {
			caller_marshal_kernel___global_netdev_ops__out(pos, msg, ext, *netdev_ops_ptr);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			caller_marshal_kernel___global_ethtool_ops__out(pos, msg, ext, *ethtool_ops_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *addr_assign_type_ptr);
	}

}

void callee_unmarshal_kernel__setup__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned char* addr_assign_type_ptr = &ptr->addr_assign_type;
	
	{
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			callee_unmarshal_kernel___global_netdev_ops__out(pos, msg, ext, writable);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
			callee_unmarshal_kernel___global_ethtool_ops__out(pos, msg, ext, writable);
		}

	}

	{
		*addr_assign_type_ptr = glue_unpack(pos, msg, ext, unsigned char);
	}

}

void callee_marshal_kernel__setup__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	unsigned long long const* features_ptr = &ptr->features;
	unsigned long long const* hw_features_ptr = &ptr->hw_features;
	unsigned long long const* hw_enc_features_ptr = &ptr->hw_enc_features;
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int const* flags_ptr = &ptr->flags;
	unsigned int const* priv_flags_ptr = &ptr->priv_flags;
	
	{
		glue_pack(pos, msg, ext, *features_ptr);
	}

	{
		glue_pack(pos, msg, ext, *hw_features_ptr);
	}

	{
		glue_pack(pos, msg, ext, *hw_enc_features_ptr);
	}

	{
		glue_pack(pos, msg, ext, *netdev_ops_ptr);
		if (*netdev_ops_ptr) {
			callee_marshal_kernel___global_netdev_ops__out(pos, msg, ext, *netdev_ops_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *ethtool_ops_ptr);
		if (*ethtool_ops_ptr) {
			callee_marshal_kernel___global_ethtool_ops__out(pos, msg, ext, *ethtool_ops_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *flags_ptr);
	}

	{
		glue_pack(pos, msg, ext, *priv_flags_ptr);
	}

}

void caller_unmarshal_kernel__setup__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	unsigned long long* features_ptr = &ptr->features;
	unsigned long long* hw_features_ptr = &ptr->hw_features;
	unsigned long long* hw_enc_features_ptr = &ptr->hw_enc_features;
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops const** ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int* flags_ptr = &ptr->flags;
	unsigned int* priv_flags_ptr = &ptr->priv_flags;
	
	{
		*features_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

	{
		*hw_features_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

	{
		*hw_enc_features_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

	{
		*netdev_ops_ptr = glue_unpack_new_shadow(pos, msg, ext, struct net_device_ops const*, sizeof(struct net_device_ops));
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			caller_unmarshal_kernel___global_netdev_ops__out(pos, msg, ext, writable);
		}

	}

	{
		*ethtool_ops_ptr = glue_unpack_new_shadow(pos, msg, ext, struct ethtool_ops const*, sizeof(struct ethtool_ops));
		if (*ethtool_ops_ptr) {
			struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
			caller_unmarshal_kernel___global_ethtool_ops__out(pos, msg, ext, writable);
		}

	}

	{
		*flags_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*priv_flags_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel___global_netdev_ops__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops const* ptr)
{
	
}

void callee_unmarshal_kernel___global_netdev_ops__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops* ptr)
{
	
}

void callee_marshal_kernel___global_netdev_ops__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops const* ptr)
{
	fptr_ndo_init const* ndo_init_ptr = &ptr->ndo_init;
	fptr_ndo_uninit const* ndo_uninit_ptr = &ptr->ndo_uninit;
	fptr_ndo_start_xmit const* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_ndo_set_rx_mode const* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_ndo_get_stats64 const* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_ndo_change_carrier const* ndo_change_carrier_ptr = &ptr->ndo_change_carrier;
	
	{
		glue_pack(pos, msg, ext, *ndo_init_ptr);
	}

	{
		glue_pack(pos, msg, ext, *ndo_uninit_ptr);
	}

	{
		glue_pack(pos, msg, ext, *ndo_start_xmit_ptr);
	}

	{
		glue_pack(pos, msg, ext, *ndo_set_rx_mode_ptr);
	}

	{
		glue_pack(pos, msg, ext, *ndo_get_stats64_ptr);
	}

	{
		glue_pack(pos, msg, ext, *ndo_change_carrier_ptr);
	}

}

void caller_unmarshal_kernel___global_netdev_ops__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops* ptr)
{
	fptr_ndo_init* ndo_init_ptr = &ptr->ndo_init;
	fptr_ndo_uninit* ndo_uninit_ptr = &ptr->ndo_uninit;
	fptr_ndo_start_xmit* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_ndo_set_rx_mode* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_ndo_get_stats64* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_ndo_change_carrier* ndo_change_carrier_ptr = &ptr->ndo_change_carrier;
	
	{
		*ndo_init_ptr = glue_unpack_rpc_ptr(pos, msg, ext, ndo_init);
	}

	{
		*ndo_uninit_ptr = glue_unpack_rpc_ptr(pos, msg, ext, ndo_uninit);
	}

	{
		*ndo_start_xmit_ptr = glue_unpack_rpc_ptr(pos, msg, ext, ndo_start_xmit);
	}

	{
		*ndo_set_rx_mode_ptr = glue_unpack_rpc_ptr(pos, msg, ext, ndo_set_rx_mode);
	}

	{
		*ndo_get_stats64_ptr = glue_unpack_rpc_ptr(pos, msg, ext, ndo_get_stats64);
	}

	{
		*ndo_change_carrier_ptr = glue_unpack_rpc_ptr(pos, msg, ext, ndo_change_carrier);
	}

}

void caller_marshal_kernel___global_ethtool_ops__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops const* ptr)
{
	
}

void callee_unmarshal_kernel___global_ethtool_ops__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops* ptr)
{
	
}

void callee_marshal_kernel___global_ethtool_ops__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops const* ptr)
{
	fptr_get_drvinfo const* get_drvinfo_ptr = &ptr->get_drvinfo;
	
	{
		glue_pack(pos, msg, ext, *get_drvinfo_ptr);
	}

}

void caller_unmarshal_kernel___global_ethtool_ops__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops* ptr)
{
	fptr_get_drvinfo* get_drvinfo_ptr = &ptr->get_drvinfo;
	
	{
		*get_drvinfo_ptr = glue_unpack_rpc_ptr(pos, msg, ext, get_drvinfo);
	}

}

void caller_marshal_kernel__get_drvinfo__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__get_drvinfo__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__get_drvinfo__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__get_drvinfo__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__get_drvinfo__info__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_drvinfo const* ptr)
{
	
}

void callee_unmarshal_kernel__get_drvinfo__info__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_drvinfo* ptr)
{
	
}

void callee_marshal_kernel__get_drvinfo__info__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_drvinfo const* ptr)
{
	char const* driver_ptr = ptr->driver;
	char const* version_ptr = ptr->version;
	
	{
		size_t i, len = 32;
		char const* array = driver_ptr;
		glue_pack(pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(pos, msg, ext, *element);
		}

	}

	{
		size_t i, len = 32;
		char const* array = version_ptr;
		glue_pack(pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(pos, msg, ext, *element);
		}

	}

}

void caller_unmarshal_kernel__get_drvinfo__info__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_drvinfo* ptr)
{
	char* driver_ptr = ptr->driver;
	char* version_ptr = ptr->version;
	
	{
		int i;
		char* array = driver_ptr;
		size_t len = glue_unpack(pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(pos, msg, ext, char);
		}

	}

	{
		int i;
		char* array = version_ptr;
		size_t len = glue_unpack(pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(pos, msg, ext, char);
		}

	}

}

void caller_marshal_kernel__ndo_init__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_init__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ndo_init__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_init__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ndo_uninit__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_uninit__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ndo_uninit__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_uninit__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ndo_start_xmit__skb__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	
	{
		glue_pack(pos, msg, ext, *len_ptr);
	}

}

void callee_unmarshal_kernel__ndo_start_xmit__skb__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	
	{
		*len_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__ndo_start_xmit__skb__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_start_xmit__skb__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel__ndo_start_xmit__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_start_xmit__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ndo_start_xmit__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_start_xmit__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ndo_set_rx_mode__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_set_rx_mode__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ndo_set_rx_mode__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_set_rx_mode__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_stats64 const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_stats64* ptr)
{
	
}

void callee_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_stats64 const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_stats64* ptr)
{
	
}

void caller_marshal_kernel__ndo_get_stats64__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_get_stats64__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ndo_get_stats64__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ndo_get_stats64__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ndo_get_stats64__stats__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_stats64 const* ptr)
{
	
}

void callee_unmarshal_kernel__ndo_get_stats64__stats__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_stats64* ptr)
{
	
}

void callee_marshal_kernel__ndo_get_stats64__stats__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_stats64 const* ptr)
{
	unsigned long long const* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long const* tx_bytes_ptr = &ptr->tx_bytes;
	
	{
		glue_pack(pos, msg, ext, *tx_packets_ptr);
	}

	{
		glue_pack(pos, msg, ext, *tx_bytes_ptr);
	}

}

void caller_unmarshal_kernel__ndo_get_stats64__stats__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_stats64* ptr)
{
	unsigned long long* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long* tx_bytes_ptr = &ptr->tx_bytes;
	
	{
		*tx_packets_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

	{
		*tx_bytes_ptr = glue_unpack(pos, msg, ext, unsigned long long);
	}

}

void caller_marshal_kernel__ndo_change_carrier__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	
	{
		glue_pack_shadow(pos, msg, ext, *netdev_ops_ptr);
		if (*netdev_ops_ptr) {
			caller_marshal_kernel___global_netdev_ops__in(pos, msg, ext, *netdev_ops_ptr);
		}

	}

}

void callee_unmarshal_kernel__ndo_change_carrier__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	
	{
		*netdev_ops_ptr = glue_unpack(pos, msg, ext, struct net_device_ops const*);
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			callee_unmarshal_kernel___global_netdev_ops__in(pos, msg, ext, writable);
		}

	}

}

void callee_marshal_kernel__ndo_change_carrier__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr)
{
	struct net_device_ops const* const* netdev_ops_ptr = &ptr->netdev_ops;
	
	{
		if (*netdev_ops_ptr) {
			callee_marshal_kernel___global_netdev_ops__in(pos, msg, ext, *netdev_ops_ptr);
		}

	}

}

void caller_unmarshal_kernel__ndo_change_carrier__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr)
{
	struct net_device_ops const** netdev_ops_ptr = &ptr->netdev_ops;
	
	{
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			caller_unmarshal_kernel___global_netdev_ops__in(pos, msg, ext, writable);
		}

	}

}

void caller_marshal_kernel__validate__tb__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct nlattr const* ptr)
{
	
}

void callee_unmarshal_kernel__validate__tb__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct nlattr* ptr)
{
	
}

void callee_marshal_kernel__validate__tb__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct nlattr const* ptr)
{
	
}

void caller_unmarshal_kernel__validate__tb__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct nlattr* ptr)
{
	
}

void caller_marshal_kernel__validate__data__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct nlattr const* ptr)
{
	
}

void callee_unmarshal_kernel__validate__data__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct nlattr* ptr)
{
	
}

void callee_marshal_kernel__validate__data__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct nlattr const* ptr)
{
	
}

void caller_unmarshal_kernel__validate__data__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct nlattr* ptr)
{
	
}

