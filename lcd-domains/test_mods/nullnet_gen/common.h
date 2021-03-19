#ifndef COMMON_H
#define COMMON_H

#include <liblcd/trampoline.h>

#include "glue_user.h"

#define GLUE_MAX_SLOTS 128
#define glue_pack(msg, value) glue_pack_impl((msg), (uint64_t)(value))
#define glue_pack_shadow(msg, value) glue_pack_shadow_impl((msg), (value))
#define glue_unpack(msg, type) (type)glue_unpack_impl((msg))
#define glue_unpack_shadow(msg, type) (type)glue_unpack_shadow_impl(glue_unpack(msg, void*));
#define glue_unpack_new_shadow(msg, type, size) \
	(type)glue_unpack_new_shadow_impl(glue_unpack(msg, void*), size)

#define glue_unpack_rpc_ptr(msg, name) \
	glue_peek(msg) ? (fptr_##name)glue_unpack_rpc_ptr_impl(glue_unpack(msg, void*), LCD_DUP_TRAMPOLINE(trmp_##name)) : 0

#define glue_peek(msg) glue_peek_impl(msg)
#define glue_call_server(msg, rpc_id) \
	msg->slots[0] = msg->position; msg->position = 0; glue_user_call_server(msg->slots, rpc_id);

#define glue_call_client(msg, rpc_id) \
	msg->slots[0] = msg->position; msg->position = 0; glue_user_call_client(msg->slots, rpc_id);


void glue_user_init(void);
void glue_user_panic(const char* msg);
void glue_user_trace(const char* msg);
void* glue_user_map_to_shadow(const void* obj);
const void* glue_user_map_from_shadow(const void* shadow);
void glue_user_add_shadow(const void* ptr, void* shadow);
void* glue_user_alloc(size_t size);
void glue_user_free(void* ptr);
void glue_user_call_server(uint64_t* data, size_t rpc_id);
void glue_user_call_client(uint64_t* data, size_t rpc_id);

struct glue_message {
	uint64_t slots[GLUE_MAX_SLOTS];
	uint64_t position;
};
extern struct glue_message shared_buffer;

static inline struct glue_message* glue_init_msg(void)
{
	shared_buffer.position = 0;
	return &shared_buffer;
}

static inline void* glue_unpack_rpc_ptr_impl(void* target, struct lcd_trampoline_handle* handle)
{
if (!target)
		glue_user_panic("Target was NULL");

if (!handle)
		glue_user_panic("Trmp was NULL");

	handle->hidden_args = target;
	return LCD_HANDLE_TO_TRAMPOLINE(handle);
}

static inline void glue_pack_impl(struct glue_message* msg, uint64_t value)
{
	if (msg->position >= GLUE_MAX_SLOTS)
		glue_user_panic("Glue message was too large");
	msg->slots[msg->position++ + 1] = value;
}

static inline uint64_t glue_unpack_impl(struct glue_message* msg)
{
	if (msg->position >= msg->slots[0])
		glue_user_panic("Unpacked past end of glue message");
	return msg->slots[msg->position++ + 1];
}

static inline uint64_t glue_peek_impl(struct glue_message* msg)
{
	if (msg->position >= msg->slots[0])
		glue_user_panic("Peeked past end of glue message");
	return msg->slots[msg->position + 2];
}

static inline void* glue_unpack_new_shadow_impl(const void* ptr, size_t size)
{
	void* shadow = 0;	if (!ptr)
		return NULL;

	shadow = glue_user_alloc(size);
	glue_user_add_shadow(ptr, shadow);
	return shadow;
}

static inline void* glue_unpack_shadow_impl(const void* ptr)
{
	return ptr ? glue_user_map_to_shadow(ptr) : NULL;
}

static inline void glue_pack_shadow_impl(struct glue_message* msg, const void* ptr)
{
	glue_pack(msg, ptr ? glue_user_map_from_shadow(ptr) : NULL);
}

enum RPC_ID {
	MODULE_INIT,
	MODULE_EXIT,
	RPC_ID_netif_carrier_on,
	RPC_ID_netif_carrier_off,
	RPC_ID_register_netdevice,
	RPC_ID_consume_skb,
	RPC_ID_alloc_netdev_mqs,
	RPC_ID_free_netdev,
	RPC_ID_rtnl_lock,
	RPC_ID_rtnl_unlock,
	RPC_ID_rtnl_link_unregister,
	RPC_ID___rtnl_link_unregister,
	RPC_ID___rtnl_link_register,
	RPC_ID_ether_setup,
	RPC_ID_eth_validate_addr,
	RPC_ID_eth_mac_addr,
	RPC_ID_setup,
	RPC_ID_get_drvinfo,
	RPC_ID_ndo_init,
	RPC_ID_ndo_uninit,
	RPC_ID_ndo_start_xmit,
	RPC_ID_ndo_set_rx_mode,
	RPC_ID_ndo_get_stats64,
	RPC_ID_ndo_change_carrier,
	RPC_ID_validate,
};

int try_dispatch(enum RPC_ID id, struct glue_message* msg);

void caller_marshal_kernel__netif_carrier_on__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__netif_carrier_on__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__netif_carrier_on__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__netif_carrier_on__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel___global_netdev_ops__in(
	struct glue_message*,
	struct net_device_ops const*);

void callee_unmarshal_kernel___global_netdev_ops__in(
	struct glue_message*,
	struct net_device_ops*);

void callee_marshal_kernel___global_netdev_ops__in(
	struct glue_message*,
	struct net_device_ops const*);

void caller_unmarshal_kernel___global_netdev_ops__in(
	struct glue_message*,
	struct net_device_ops*);

void caller_marshal_kernel__netif_carrier_off__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__netif_carrier_off__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__netif_carrier_off__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__netif_carrier_off__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel__register_netdevice__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__register_netdevice__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__register_netdevice__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__register_netdevice__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel___global_ethtool_ops__in(
	struct glue_message*,
	struct ethtool_ops const*);

void callee_unmarshal_kernel___global_ethtool_ops__in(
	struct glue_message*,
	struct ethtool_ops*);

void callee_marshal_kernel___global_ethtool_ops__in(
	struct glue_message*,
	struct ethtool_ops const*);

void caller_unmarshal_kernel___global_ethtool_ops__in(
	struct glue_message*,
	struct ethtool_ops*);

void caller_marshal_kernel___global_rtnl_link_ops__in(
	struct glue_message*,
	struct rtnl_link_ops const*);

void callee_unmarshal_kernel___global_rtnl_link_ops__in(
	struct glue_message*,
	struct rtnl_link_ops*);

void callee_marshal_kernel___global_rtnl_link_ops__in(
	struct glue_message*,
	struct rtnl_link_ops const*);

void caller_unmarshal_kernel___global_rtnl_link_ops__in(
	struct glue_message*,
	struct rtnl_link_ops*);

void caller_marshal_kernel__consume_skb__skb__in(
	struct glue_message*,
	struct sk_buff const*);

void callee_unmarshal_kernel__consume_skb__skb__in(
	struct glue_message*,
	struct sk_buff*);

void callee_marshal_kernel__consume_skb__skb__in(
	struct glue_message*,
	struct sk_buff const*);

void caller_unmarshal_kernel__consume_skb__skb__in(
	struct glue_message*,
	struct sk_buff*);

void caller_marshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel___global_netdev_ops__out(
	struct glue_message*,
	struct net_device_ops const*);

void callee_unmarshal_kernel___global_netdev_ops__out(
	struct glue_message*,
	struct net_device_ops*);

void callee_marshal_kernel___global_netdev_ops__out(
	struct glue_message*,
	struct net_device_ops const*);

void caller_unmarshal_kernel___global_netdev_ops__out(
	struct glue_message*,
	struct net_device_ops*);

void caller_marshal_kernel___global_ethtool_ops__out(
	struct glue_message*,
	struct ethtool_ops const*);

void callee_unmarshal_kernel___global_ethtool_ops__out(
	struct glue_message*,
	struct ethtool_ops*);

void callee_marshal_kernel___global_ethtool_ops__out(
	struct glue_message*,
	struct ethtool_ops const*);

void caller_unmarshal_kernel___global_ethtool_ops__out(
	struct glue_message*,
	struct ethtool_ops*);

void caller_marshal_kernel___global_rtnl_link_ops__out(
	struct glue_message*,
	struct rtnl_link_ops const*);

void callee_unmarshal_kernel___global_rtnl_link_ops__out(
	struct glue_message*,
	struct rtnl_link_ops*);

void callee_marshal_kernel___global_rtnl_link_ops__out(
	struct glue_message*,
	struct rtnl_link_ops const*);

void caller_unmarshal_kernel___global_rtnl_link_ops__out(
	struct glue_message*,
	struct rtnl_link_ops*);

void caller_marshal_kernel__free_netdev__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__free_netdev__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__free_netdev__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__free_netdev__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	struct glue_message*,
	struct rtnl_link_ops const*);

void callee_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	struct glue_message*,
	struct rtnl_link_ops*);

void callee_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	struct glue_message*,
	struct rtnl_link_ops const*);

void caller_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	struct glue_message*,
	struct rtnl_link_ops*);

void caller_marshal_kernel__ether_setup__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__ether_setup__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__ether_setup__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__ether_setup__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel__eth_validate_addr__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__eth_validate_addr__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__eth_validate_addr__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__eth_validate_addr__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel__eth_mac_addr__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__eth_mac_addr__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__eth_mac_addr__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__eth_mac_addr__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel__eth_mac_addr__p__in(
	struct glue_message*,
	struct sockaddr const*);

void callee_unmarshal_kernel__eth_mac_addr__p__in(
	struct glue_message*,
	struct sockaddr*);

void callee_marshal_kernel__eth_mac_addr__p__in(
	struct glue_message*,
	struct sockaddr const*);

void caller_unmarshal_kernel__eth_mac_addr__p__in(
	struct glue_message*,
	struct sockaddr*);

void caller_marshal_kernel__setup__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__setup__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__setup__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__setup__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel__get_drvinfo__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__get_drvinfo__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__get_drvinfo__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__get_drvinfo__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel__get_drvinfo__info__in(
	struct glue_message*,
	struct ethtool_drvinfo const*);

void callee_unmarshal_kernel__get_drvinfo__info__in(
	struct glue_message*,
	struct ethtool_drvinfo*);

void callee_marshal_kernel__get_drvinfo__info__in(
	struct glue_message*,
	struct ethtool_drvinfo const*);

void caller_unmarshal_kernel__get_drvinfo__info__in(
	struct glue_message*,
	struct ethtool_drvinfo*);

void caller_marshal_kernel__ndo_init__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__ndo_init__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__ndo_init__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__ndo_init__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel__ndo_uninit__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__ndo_uninit__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__ndo_uninit__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__ndo_uninit__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel__ndo_start_xmit__skb__in(
	struct glue_message*,
	struct sk_buff const*);

void callee_unmarshal_kernel__ndo_start_xmit__skb__in(
	struct glue_message*,
	struct sk_buff*);

void callee_marshal_kernel__ndo_start_xmit__skb__in(
	struct glue_message*,
	struct sk_buff const*);

void caller_unmarshal_kernel__ndo_start_xmit__skb__in(
	struct glue_message*,
	struct sk_buff*);

void caller_marshal_kernel__ndo_start_xmit__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__ndo_start_xmit__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__ndo_start_xmit__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__ndo_start_xmit__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel__ndo_set_rx_mode__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__ndo_set_rx_mode__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__ndo_set_rx_mode__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__ndo_set_rx_mode__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	struct glue_message*,
	struct rtnl_link_stats64 const*);

void callee_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	struct glue_message*,
	struct rtnl_link_stats64*);

void callee_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	struct glue_message*,
	struct rtnl_link_stats64 const*);

void caller_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	struct glue_message*,
	struct rtnl_link_stats64*);

void caller_marshal_kernel__ndo_get_stats64__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__ndo_get_stats64__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__ndo_get_stats64__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__ndo_get_stats64__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel__ndo_get_stats64__stats__in(
	struct glue_message*,
	struct rtnl_link_stats64 const*);

void callee_unmarshal_kernel__ndo_get_stats64__stats__in(
	struct glue_message*,
	struct rtnl_link_stats64*);

void callee_marshal_kernel__ndo_get_stats64__stats__in(
	struct glue_message*,
	struct rtnl_link_stats64 const*);

void caller_unmarshal_kernel__ndo_get_stats64__stats__in(
	struct glue_message*,
	struct rtnl_link_stats64*);

void caller_marshal_kernel__ndo_change_carrier__dev__in(
	struct glue_message*,
	struct net_device const*);

void callee_unmarshal_kernel__ndo_change_carrier__dev__in(
	struct glue_message*,
	struct net_device*);

void callee_marshal_kernel__ndo_change_carrier__dev__in(
	struct glue_message*,
	struct net_device const*);

void caller_unmarshal_kernel__ndo_change_carrier__dev__in(
	struct glue_message*,
	struct net_device*);

void caller_marshal_kernel__validate__tb__in(
	struct glue_message*,
	struct nlattr const*);

void callee_unmarshal_kernel__validate__tb__in(
	struct glue_message*,
	struct nlattr*);

void callee_marshal_kernel__validate__tb__in(
	struct glue_message*,
	struct nlattr const*);

void caller_unmarshal_kernel__validate__tb__in(
	struct glue_message*,
	struct nlattr*);

void caller_marshal_kernel__validate__data__in(
	struct glue_message*,
	struct nlattr const*);

void callee_unmarshal_kernel__validate__data__in(
	struct glue_message*,
	struct nlattr*);

void callee_marshal_kernel__validate__data__in(
	struct glue_message*,
	struct nlattr const*);

void caller_unmarshal_kernel__validate__data__in(
	struct glue_message*,
	struct nlattr*);

typedef void (*fptr_setup)(struct net_device* dev);
typedef void (*fptr_impl_setup)(fptr_setup target, struct net_device* dev);

LCD_TRAMPOLINE_DATA(trmp_setup)
void LCD_TRAMPOLINE_LINKAGE(trmp_setup) trmp_setup(struct net_device* dev);

typedef void (*fptr_get_drvinfo)(struct net_device* dev, struct ethtool_drvinfo* info);
typedef void (*fptr_impl_get_drvinfo)(fptr_get_drvinfo target, struct net_device* dev, struct ethtool_drvinfo* info);

LCD_TRAMPOLINE_DATA(trmp_get_drvinfo)
void LCD_TRAMPOLINE_LINKAGE(trmp_get_drvinfo) trmp_get_drvinfo(struct net_device* dev, struct ethtool_drvinfo* info);

typedef int (*fptr_ndo_init)(struct net_device* dev);
typedef int (*fptr_impl_ndo_init)(fptr_ndo_init target, struct net_device* dev);

LCD_TRAMPOLINE_DATA(trmp_ndo_init)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_init) trmp_ndo_init(struct net_device* dev);

typedef void (*fptr_ndo_uninit)(struct net_device* dev);
typedef void (*fptr_impl_ndo_uninit)(fptr_ndo_uninit target, struct net_device* dev);

LCD_TRAMPOLINE_DATA(trmp_ndo_uninit)
void LCD_TRAMPOLINE_LINKAGE(trmp_ndo_uninit) trmp_ndo_uninit(struct net_device* dev);

typedef netdev_tx_t (*fptr_ndo_start_xmit)(struct sk_buff* skb, struct net_device* dev);
typedef long long (*fptr_impl_ndo_start_xmit)(fptr_ndo_start_xmit target, struct sk_buff* skb, struct net_device* dev);

LCD_TRAMPOLINE_DATA(trmp_ndo_start_xmit)
long long LCD_TRAMPOLINE_LINKAGE(trmp_ndo_start_xmit) trmp_ndo_start_xmit(struct sk_buff* skb, struct net_device* dev);

typedef void (*fptr_ndo_set_rx_mode)(struct net_device* dev);
typedef void (*fptr_impl_ndo_set_rx_mode)(fptr_ndo_set_rx_mode target, struct net_device* dev);

LCD_TRAMPOLINE_DATA(trmp_ndo_set_rx_mode)
void LCD_TRAMPOLINE_LINKAGE(trmp_ndo_set_rx_mode) trmp_ndo_set_rx_mode(struct net_device* dev);

typedef struct rtnl_link_stats64* (*fptr_ndo_get_stats64)(struct net_device* dev, struct rtnl_link_stats64* stats);
typedef struct rtnl_link_stats64* (*fptr_impl_ndo_get_stats64)(fptr_ndo_get_stats64 target, struct net_device* dev, struct rtnl_link_stats64* stats);

LCD_TRAMPOLINE_DATA(trmp_ndo_get_stats64)
struct rtnl_link_stats64* LCD_TRAMPOLINE_LINKAGE(trmp_ndo_get_stats64) trmp_ndo_get_stats64(struct net_device* dev, struct rtnl_link_stats64* stats);

typedef int (*fptr_ndo_change_carrier)(struct net_device* dev, bool new_carrier);
typedef int (*fptr_impl_ndo_change_carrier)(fptr_ndo_change_carrier target, struct net_device* dev, bool new_carrier);

LCD_TRAMPOLINE_DATA(trmp_ndo_change_carrier)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_change_carrier) trmp_ndo_change_carrier(struct net_device* dev, bool new_carrier);

typedef int (*fptr_validate)(struct nlattr** tb, struct nlattr** data);
typedef int (*fptr_impl_validate)(fptr_validate target, struct nlattr** tb, struct nlattr** data);

LCD_TRAMPOLINE_DATA(trmp_validate)
int LCD_TRAMPOLINE_LINKAGE(trmp_validate) trmp_validate(struct nlattr** tb, struct nlattr** data);


#endif
