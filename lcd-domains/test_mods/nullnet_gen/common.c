#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_carrier_on_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_carrier_on_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_carrier_on_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_carrier_on_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_carrier_off_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_carrier_off_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_carrier_off_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_carrier_off_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__register_netdevice__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct register_netdevice_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__register_netdevice__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct register_netdevice_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__register_netdevice__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct register_netdevice_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__register_netdevice__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct register_netdevice_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct consume_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

}

void callee_unmarshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct consume_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct consume_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{

}

void caller_unmarshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct consume_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{

	{
	}
}

void caller_marshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct alloc_netdev_mqs_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct alloc_netdev_mqs_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct alloc_netdev_mqs_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct alloc_netdev_mqs_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct free_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct free_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct free_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct free_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel___global_rtnl_link_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct rtnl_link_ops const* ptr)
{
	fptr_setup const* setup_ptr = &ptr->setup;
	fptr_validate const* validate_ptr = &ptr->validate;

	{
		glue_pack(__pos, __msg, __ext, *setup_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *validate_ptr);
	}

}

void callee_unmarshal_kernel___global_rtnl_link_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct rtnl_link_ops* ptr)
{
	fptr_setup* setup_ptr = &ptr->setup;
	fptr_validate* validate_ptr = &ptr->validate;

	{
		*setup_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, setup);
	}

	{
		*validate_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, validate);
	}

	{
	}
}

void callee_marshal_kernel___global_rtnl_link_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct rtnl_link_ops const* ptr)
{

}

void caller_unmarshal_kernel___global_rtnl_link_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct rtnl_link_ops* ptr)
{

	{
	}
}

void caller_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __rtnl_link_register_call_ctx const* ctx,
	struct rtnl_link_ops const* ptr)
{
	char const* const* kind_ptr = &ptr->kind;
	fptr_setup const* setup_ptr = &ptr->setup;
	fptr_validate const* validate_ptr = &ptr->validate;

	{
		__maybe_unused const void* __adjusted = *kind_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*kind_ptr) {
			size_t i, len;
			char const* array = *kind_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *setup_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *validate_ptr);
	}

}

void callee_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __rtnl_link_register_call_ctx const* ctx,
	struct rtnl_link_ops* ptr)
{
	char const** kind_ptr = &ptr->kind;
	fptr_setup* setup_ptr = &ptr->setup;
	fptr_validate* validate_ptr = &ptr->validate;

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*kind_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*kind_ptr) {
			char* writable = (char*)*kind_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, __msg, __ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	{
		*setup_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, setup);
	}

	{
		*validate_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, validate);
	}

	{
	}
}

void callee_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __rtnl_link_register_call_ctx const* ctx,
	struct rtnl_link_ops const* ptr)
{
	char const* const* kind_ptr = &ptr->kind;

	{
		(void)kind_ptr;
	}

}

void caller_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __rtnl_link_register_call_ctx const* ctx,
	struct rtnl_link_ops* ptr)
{
	char const** kind_ptr = &ptr->kind;

	{
		(void)kind_ptr;
	}

	{
	}
}

void caller_marshal_kernel__ether_setup__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ether_setup_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ether_setup__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ether_setup_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ether_setup__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ether_setup_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* flags_ptr = &ptr->flags;
	unsigned int const* priv_flags_ptr = &ptr->priv_flags;

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *priv_flags_ptr);
	}

}

void caller_unmarshal_kernel__ether_setup__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ether_setup_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* flags_ptr = &ptr->flags;
	unsigned int* priv_flags_ptr = &ptr->priv_flags;

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*priv_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_validate_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_validate_addr_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_validate_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_validate_addr_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__eth_mac_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_mac_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__eth_mac_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_mac_addr_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__eth_mac_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_mac_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__eth_mac_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_mac_addr_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__eth_mac_addr__p__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_mac_addr_call_ctx const* ctx,
	struct sockaddr const* ptr)
{

}

void callee_unmarshal_kernel__eth_mac_addr__p__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_mac_addr_call_ctx const* ctx,
	struct sockaddr* ptr)
{

	{
	}
}

void callee_marshal_kernel__eth_mac_addr__p__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_mac_addr_call_ctx const* ctx,
	struct sockaddr const* ptr)
{

}

void caller_unmarshal_kernel__eth_mac_addr__p__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_mac_addr_call_ctx const* ctx,
	struct sockaddr* ptr)
{

	{
	}
}

void caller_marshal_kernel__setup__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct setup_call_ctx const* ctx,
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
		glue_pack(__pos, __msg, __ext, *features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *hw_features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *hw_enc_features_ptr);
	}

	{
		if (*netdev_ops_ptr) {
			caller_marshal_kernel___global_netdev_ops__out(__pos, __msg, __ext, *netdev_ops_ptr);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			caller_marshal_kernel___global_ethtool_ops__out(__pos, __msg, __ext, *ethtool_ops_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *priv_flags_ptr);
	}

}

void callee_unmarshal_kernel__setup__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct setup_call_ctx const* ctx,
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
		*features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*hw_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*hw_enc_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			callee_unmarshal_kernel___global_netdev_ops__out(__pos, __msg, __ext, writable);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
			callee_unmarshal_kernel___global_ethtool_ops__out(__pos, __msg, __ext, writable);
		}

	}

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*priv_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__setup__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct setup_call_ctx const* ctx,
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
		glue_pack(__pos, __msg, __ext, *features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *hw_features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *hw_enc_features_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *netdev_ops_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*netdev_ops_ptr) {
			callee_marshal_kernel___global_netdev_ops__out(__pos, __msg, __ext, *netdev_ops_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ethtool_ops_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ethtool_ops_ptr) {
			callee_marshal_kernel___global_ethtool_ops__out(__pos, __msg, __ext, *ethtool_ops_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *priv_flags_ptr);
	}

}

void caller_unmarshal_kernel__setup__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct setup_call_ctx const* ctx,
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
		*features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*hw_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*hw_enc_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		size_t __size = sizeof(struct net_device_ops);
		*netdev_ops_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct net_device_ops const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*netdev_ops_ptr) {
			struct net_device_ops* writable = (struct net_device_ops*)*netdev_ops_ptr;
			caller_unmarshal_kernel___global_netdev_ops__out(__pos, __msg, __ext, writable);
		}

	}

	{
		size_t __size = sizeof(struct ethtool_ops);
		*ethtool_ops_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct ethtool_ops const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*ethtool_ops_ptr) {
			struct ethtool_ops* writable = (struct ethtool_ops*)*ethtool_ops_ptr;
			caller_unmarshal_kernel___global_ethtool_ops__out(__pos, __msg, __ext, writable);
		}

	}

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*priv_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel___global_netdev_ops__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct net_device_ops const* ptr)
{

}

void callee_unmarshal_kernel___global_netdev_ops__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct net_device_ops* ptr)
{

	{
	}
}

void callee_marshal_kernel___global_netdev_ops__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct net_device_ops const* ptr)
{
	fptr_ndo_init const* ndo_init_ptr = &ptr->ndo_init;
	fptr_ndo_uninit const* ndo_uninit_ptr = &ptr->ndo_uninit;
	fptr_ndo_start_xmit const* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_ndo_validate_addr const* ndo_validate_addr_ptr = &ptr->ndo_validate_addr;
	fptr_ndo_set_rx_mode const* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_ndo_set_mac_address const* ndo_set_mac_address_ptr = &ptr->ndo_set_mac_address;
	fptr_ndo_get_stats64 const* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_ndo_change_carrier const* ndo_change_carrier_ptr = &ptr->ndo_change_carrier;

	{
		glue_pack(__pos, __msg, __ext, *ndo_init_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_uninit_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_start_xmit_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_validate_addr_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_set_rx_mode_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_set_mac_address_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_get_stats64_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_change_carrier_ptr);
	}

}

void caller_unmarshal_kernel___global_netdev_ops__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct net_device_ops* ptr)
{
	fptr_ndo_init* ndo_init_ptr = &ptr->ndo_init;
	fptr_ndo_uninit* ndo_uninit_ptr = &ptr->ndo_uninit;
	fptr_ndo_start_xmit* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_ndo_validate_addr* ndo_validate_addr_ptr = &ptr->ndo_validate_addr;
	fptr_ndo_set_rx_mode* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_ndo_set_mac_address* ndo_set_mac_address_ptr = &ptr->ndo_set_mac_address;
	fptr_ndo_get_stats64* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_ndo_change_carrier* ndo_change_carrier_ptr = &ptr->ndo_change_carrier;

	{
		*ndo_init_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_init);
	}

	{
		*ndo_uninit_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_uninit);
	}

	{
		*ndo_start_xmit_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_start_xmit);
	}

	{
		*ndo_validate_addr_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_validate_addr);
	}

	{
		*ndo_set_rx_mode_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_set_rx_mode);
	}

	{
		*ndo_set_mac_address_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_set_mac_address);
	}

	{
		*ndo_get_stats64_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_get_stats64);
	}

	{
		*ndo_change_carrier_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_change_carrier);
	}

	{
	}
}

void caller_marshal_kernel___global_ethtool_ops__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_ops const* ptr)
{

}

void callee_unmarshal_kernel___global_ethtool_ops__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_ops* ptr)
{

	{
	}
}

void callee_marshal_kernel___global_ethtool_ops__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_ops const* ptr)
{
	fptr_get_drvinfo const* get_drvinfo_ptr = &ptr->get_drvinfo;

	{
		glue_pack(__pos, __msg, __ext, *get_drvinfo_ptr);
	}

}

void caller_unmarshal_kernel___global_ethtool_ops__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_ops* ptr)
{
	fptr_get_drvinfo* get_drvinfo_ptr = &ptr->get_drvinfo;

	{
		*get_drvinfo_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, get_drvinfo);
	}

	{
	}
}

void caller_marshal_kernel__get_drvinfo__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_drvinfo_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__get_drvinfo__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_drvinfo_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__get_drvinfo__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_drvinfo_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__get_drvinfo__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_drvinfo_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__get_drvinfo__info__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_drvinfo_call_ctx const* ctx,
	struct ethtool_drvinfo const* ptr)
{

}

void callee_unmarshal_kernel__get_drvinfo__info__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_drvinfo_call_ctx const* ctx,
	struct ethtool_drvinfo* ptr)
{

	{
	}
}

void callee_marshal_kernel__get_drvinfo__info__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_drvinfo_call_ctx const* ctx,
	struct ethtool_drvinfo const* ptr)
{
	char const* driver_ptr = ptr->driver;
	char const* version_ptr = ptr->version;

	{
		size_t i, len = 32;
		char const* array = driver_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 32;
		char const* array = version_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

}

void caller_unmarshal_kernel__get_drvinfo__info__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_drvinfo_call_ctx const* ctx,
	struct ethtool_drvinfo* ptr)
{
	char* driver_ptr = ptr->driver;
	char* version_ptr = ptr->version;

	{
		int i;
		char* array = driver_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, char);
		}

	}

	{
		int i;
		char* array = version_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, char);
		}

	}

	{
	}
}

void caller_marshal_kernel__ndo_init__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_init_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_init__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_init_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_init__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_init_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_init__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_init_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_uninit__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_uninit_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_uninit__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_uninit_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_uninit__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_uninit_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_uninit__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_uninit_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_start_xmit__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	unsigned char* const* data_ptr = &ptr->data;

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack(__pos, __msg, __ext, (void*)__adjusted - skb_data_pool);
	}

}

void callee_unmarshal_kernel__ndo_start_xmit__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	unsigned char** data_ptr = &ptr->data;

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_ptr = (unsigned char*)(glue_unpack(__pos, __msg, __ext, size_t) + skb_data_pool);
	}

	{
	}
}

void callee_marshal_kernel__ndo_start_xmit__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned char* const* data_ptr = &ptr->data;

	{
		(void)data_ptr;
	}

}

void caller_unmarshal_kernel__ndo_start_xmit__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned char** data_ptr = &ptr->data;

	{
		(void)data_ptr;
	}

	{
	}
}

void caller_marshal_kernel__ndo_start_xmit__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_start_xmit__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_start_xmit__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_start_xmit__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_set_rx_mode__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_set_rx_mode__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_set_rx_mode__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_set_rx_mode__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_validate_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_validate_addr_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_validate_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_validate_addr_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_set_mac_address__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_mac_address_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_set_mac_address__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_mac_address_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_set_mac_address__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_mac_address_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_set_mac_address__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_mac_address_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{

}

void callee_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{

}

void caller_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_get_stats64__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_get_stats64__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_get_stats64__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_get_stats64__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_get_stats64__stats__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{

}

void callee_unmarshal_kernel__ndo_get_stats64__stats__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_get_stats64__stats__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{
	unsigned long long const* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long const* tx_bytes_ptr = &ptr->tx_bytes;

	{
		glue_pack(__pos, __msg, __ext, *tx_packets_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *tx_bytes_ptr);
	}

}

void caller_unmarshal_kernel__ndo_get_stats64__stats__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{
	unsigned long long* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long* tx_bytes_ptr = &ptr->tx_bytes;

	{
		*tx_packets_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*tx_bytes_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void caller_marshal_kernel__ndo_change_carrier__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_change_carrier_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_change_carrier__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_change_carrier_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_change_carrier__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_change_carrier_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_change_carrier__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_change_carrier_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__validate__tb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct validate_call_ctx const* ctx,
	struct nlattr const* ptr)
{

}

void callee_unmarshal_kernel__validate__tb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct validate_call_ctx const* ctx,
	struct nlattr* ptr)
{

	{
	}
}

void callee_marshal_kernel__validate__tb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct validate_call_ctx const* ctx,
	struct nlattr const* ptr)
{

}

void caller_unmarshal_kernel__validate__tb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct validate_call_ctx const* ctx,
	struct nlattr* ptr)
{

	{
	}
}

void caller_marshal_kernel__validate__data__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct validate_call_ctx const* ctx,
	struct nlattr const* ptr)
{

}

void callee_unmarshal_kernel__validate__data__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct validate_call_ctx const* ctx,
	struct nlattr* ptr)
{

	{
	}
}

void callee_marshal_kernel__validate__data__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct validate_call_ctx const* ctx,
	struct nlattr const* ptr)
{

}

void caller_unmarshal_kernel__validate__data__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct validate_call_ctx const* ctx,
	struct nlattr* ptr)
{

	{
	}
}


#ifdef LCD_ISOLATE
__attribute__((weak)) void shared_mem_init(void) {
	LIBLCD_MSG("Weak shared_mem_init does nothing! Override if you want!");
}
#else
__attribute__((weak)) void shared_mem_init_callee(struct fipc_message *__msg, struct ext_registers* __ext) {
	LIBLCD_MSG("Weak shared_mem_init_callee does nothing! Override if you want!");
}
#endif	/* LCD_ISOLATE */

