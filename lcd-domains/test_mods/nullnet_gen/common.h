#ifndef COMMON_H
#define COMMON_H

#include <liblcd/trampoline.h>
#include <libfipc.h>
#include <liblcd/boot_info.h>
#include <asm/cacheflush.h>
#include <lcd_domains/microkernel.h>
#include <liblcd/liblcd.h>

#include "glue_user.h"

#define verbose_debug 1
#define glue_pack(pos, msg, ext, value) glue_pack_impl((pos), (msg), (ext), (uint64_t)(value))
#define glue_pack_shadow(pos, msg, ext, value) glue_pack_shadow_impl((pos), (msg), (ext), (value))
#define glue_unpack(pos, msg, ext, type) (type)glue_unpack_impl((pos), (msg), (ext))
#define glue_unpack_shadow(pos, msg, ext, type) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack shadow for type %s\n", __func__, __LINE__, __stringify(type)); \
	(type)glue_unpack_shadow_impl(glue_unpack(pos, msg, ext, void*)); })

#define glue_unpack_new_shadow(pos, msg, ext, type, size) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack new shadow for type %s | size %llu\n", __func__, __LINE__, __stringify(type), (uint64_t) size); \
	(type)glue_unpack_new_shadow_impl(glue_unpack(pos, msg, ext, void*), size); })

#define glue_unpack_bind_or_new_shadow(pos, msg, ext, type, size) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack or bind new shadow for type %s | size %llu\n", __func__, __LINE__, __stringify(type), (uint64_t) size); \
	(type)glue_unpack_bind_or_new_shadow_impl(glue_unpack(pos, msg, ext, void*), size); })

#ifndef LCD_ISOLATE
#define glue_unpack_rpc_ptr(pos, msg, ext, name) \
	glue_peek(pos, msg, ext) ? (fptr_##name)glue_unpack_rpc_ptr_impl(glue_unpack(pos, msg, ext, void*), LCD_DUP_TRAMPOLINE(trmp_##name), LCD_TRAMPOLINE_SIZE(trmp_##name)) : NULL

#else
#define glue_unpack_rpc_ptr(pos, msg, ext, name) NULL; glue_user_panic("Trampolines cannot be used on LCD side")
#endif

#define glue_peek(pos, msg, ext) glue_peek_impl(pos, msg, ext)
#define glue_call_server(pos, msg, rpc_id) \
	msg->regs[0] = *pos; *pos = 0; glue_user_call_server(msg, rpc_id);

#define glue_remove_shadow(shadow) glue_user_remove_shadow(shadow)
#define glue_call_client(pos, msg, rpc_id) \
	msg->regs[0] = *pos; *pos = 0; glue_user_call_client(msg, rpc_id);

void glue_user_init(void);
void glue_user_panic(const char* msg);
void glue_user_trace(const char* msg);
void* glue_user_map_to_shadow(const void* obj);
const void* glue_user_map_from_shadow(const void* shadow);
void glue_user_add_shadow(const void* ptr, void* shadow);
void* glue_user_alloc(size_t size);
void glue_user_free(void* ptr);
void glue_user_call_server(struct fipc_message* msg, size_t rpc_id);
void glue_user_call_client(struct fipc_message* msg, size_t rpc_id);
void glue_user_remove_shadow(void* shadow);

static inline void* glue_unpack_rpc_ptr_impl(void* target, struct lcd_trampoline_handle* handle, size_t size)
{
	if (!target)
		glue_user_panic("Target was NULL");

	if (!handle)
		glue_user_panic("Trmp was NULL");

	set_memory_x(((unsigned long)handle) & PAGE_MASK, ALIGN(size, PAGE_SIZE) >> PAGE_SHIFT);
	handle->hidden_args = target;
	return LCD_HANDLE_TO_TRAMPOLINE(handle);
}

static inline void
glue_pack_impl(size_t* pos, struct fipc_message* msg, struct ext_registers* ext, uint64_t value)
{
	if (*pos >= 128)
		glue_user_panic("Glue message was too large");
	if (*pos < 6)
		msg->regs[(*pos)++ + 1] = value;
	else
		ext->regs[(*pos)++ + 1] = value;
}

static inline uint64_t
glue_unpack_impl(size_t* pos, const struct fipc_message* msg, const struct ext_registers* ext)
{
	if (*pos >= msg->regs[0])
		glue_user_panic("Unpacked past end of glue message");
	if (*pos < 6)
		return msg->regs[(*pos)++ + 1];
	else
		return ext->regs[(*pos)++ + 1];
}

static inline uint64_t
glue_peek_impl(size_t* pos, const struct fipc_message* msg, const struct ext_registers* ext)
{
	if (*pos >= msg->regs[0])
		glue_user_panic("Peeked past end of glue message");
	if (*pos < 5)
		return msg->regs[*pos + 2];
	else
		return ext->regs[*pos + 2];
}

static inline void* glue_unpack_new_shadow_impl(const void* ptr, size_t size)
{
	void* shadow = 0;
	if (!ptr)
		return NULL;

	shadow = glue_user_alloc(size);
	glue_user_add_shadow(ptr, shadow);
	return shadow;
}

static inline void* glue_unpack_bind_or_new_shadow_impl(const void* ptr, size_t size)
{
	void* shadow = 0;
	if (!ptr)
		return NULL;

	shadow = glue_user_map_to_shadow(ptr);
	if (!shadow) {
		shadow = glue_user_alloc(size);
		glue_user_add_shadow(ptr, shadow);
	}
	return shadow;
}

static inline void* glue_unpack_shadow_impl(const void* ptr)
{
	return ptr ? glue_user_map_to_shadow(ptr) : NULL;
}

static inline void glue_pack_shadow_impl(size_t* pos, struct fipc_message* msg, struct ext_registers* ext, const void* ptr)
{
	glue_pack(pos, msg, ext, ptr ? glue_user_map_from_shadow(ptr) : NULL);
}

#ifdef LCD_ISOLATE
void shared_mem_init(void);
#else
void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext);
#endif	/* LCD_ISOLATE */


enum RPC_ID {
	MODULE_INIT,
	MODULE_EXIT,
	RPC_ID_shared_mem_init,
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

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext);

void caller_marshal_kernel__netif_carrier_on__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_carrier_on__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__netif_carrier_on__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_carrier_on__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel___global_netdev_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops const* ptr);

void callee_unmarshal_kernel___global_netdev_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops* ptr);

void callee_marshal_kernel___global_netdev_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops const* ptr);

void caller_unmarshal_kernel___global_netdev_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops* ptr);

void caller_marshal_kernel__netif_carrier_off__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__netif_carrier_off__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel__register_netdevice__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__register_netdevice__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__register_netdevice__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__register_netdevice__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel___global_ethtool_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops const* ptr);

void callee_unmarshal_kernel___global_ethtool_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops* ptr);

void callee_marshal_kernel___global_ethtool_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops const* ptr);

void caller_unmarshal_kernel___global_ethtool_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops* ptr);

void caller_marshal_kernel___global_rtnl_link_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_ops const* ptr);

void callee_unmarshal_kernel___global_rtnl_link_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_ops* ptr);

void callee_marshal_kernel___global_rtnl_link_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_ops const* ptr);

void caller_unmarshal_kernel___global_rtnl_link_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_ops* ptr);

void caller_marshal_kernel__consume_skb__skb__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__consume_skb__skb__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sk_buff* ptr);

void callee_marshal_kernel__consume_skb__skb__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__consume_skb__skb__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sk_buff* ptr);

void caller_marshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__alloc_netdev_mqs__ret_net_device__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel___global_rtnl_link_ops__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_ops const* ptr);

void callee_unmarshal_kernel___global_rtnl_link_ops__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_ops* ptr);

void callee_marshal_kernel___global_rtnl_link_ops__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_ops const* ptr);

void caller_unmarshal_kernel___global_rtnl_link_ops__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_ops* ptr);

void caller_marshal_kernel__free_netdev__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__free_netdev__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__free_netdev__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__free_netdev__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_ops const* ptr);

void callee_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_ops* ptr);

void callee_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_ops const* ptr);

void caller_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_ops* ptr);

void caller_marshal_kernel__ether_setup__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ether_setup__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__ether_setup__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ether_setup__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel__eth_validate_addr__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__eth_validate_addr__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel__eth_mac_addr__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__eth_mac_addr__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__eth_mac_addr__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__eth_mac_addr__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel__eth_mac_addr__p__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sockaddr const* ptr);

void callee_unmarshal_kernel__eth_mac_addr__p__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sockaddr* ptr);

void callee_marshal_kernel__eth_mac_addr__p__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sockaddr const* ptr);

void caller_unmarshal_kernel__eth_mac_addr__p__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sockaddr* ptr);

void caller_marshal_kernel__setup__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__setup__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__setup__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__setup__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel___global_netdev_ops__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops const* ptr);

void callee_unmarshal_kernel___global_netdev_ops__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops* ptr);

void callee_marshal_kernel___global_netdev_ops__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops const* ptr);

void caller_unmarshal_kernel___global_netdev_ops__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops* ptr);

void caller_marshal_kernel___global_ethtool_ops__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops const* ptr);

void callee_unmarshal_kernel___global_ethtool_ops__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops* ptr);

void callee_marshal_kernel___global_ethtool_ops__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops const* ptr);

void caller_unmarshal_kernel___global_ethtool_ops__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops* ptr);

void caller_marshal_kernel__get_drvinfo__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__get_drvinfo__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__get_drvinfo__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__get_drvinfo__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel__get_drvinfo__info__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_drvinfo const* ptr);

void callee_unmarshal_kernel__get_drvinfo__info__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_drvinfo* ptr);

void callee_marshal_kernel__get_drvinfo__info__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_drvinfo const* ptr);

void caller_unmarshal_kernel__get_drvinfo__info__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_drvinfo* ptr);

void caller_marshal_kernel__ndo_init__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_init__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_init__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_init__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_uninit__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_uninit__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_uninit__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_uninit__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_start_xmit__skb__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__ndo_start_xmit__skb__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sk_buff* ptr);

void callee_marshal_kernel__ndo_start_xmit__skb__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__ndo_start_xmit__skb__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sk_buff* ptr);

void caller_marshal_kernel__ndo_start_xmit__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_start_xmit__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_start_xmit__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_start_xmit__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_set_rx_mode__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_set_rx_mode__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_set_rx_mode__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_set_rx_mode__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_stats64 const* ptr);

void callee_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_stats64* ptr);

void callee_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_stats64 const* ptr);

void caller_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_stats64* ptr);

void caller_marshal_kernel__ndo_get_stats64__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_get_stats64__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_get_stats64__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_get_stats64__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_get_stats64__stats__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_stats64 const* ptr);

void callee_unmarshal_kernel__ndo_get_stats64__stats__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_stats64* ptr);

void callee_marshal_kernel__ndo_get_stats64__stats__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rtnl_link_stats64 const* ptr);

void caller_unmarshal_kernel__ndo_get_stats64__stats__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rtnl_link_stats64* ptr);

void caller_marshal_kernel__ndo_change_carrier__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_change_carrier__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_change_carrier__dev__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_change_carrier__dev__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device* ptr);

void caller_marshal_kernel__validate__tb__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct nlattr const* ptr);

void callee_unmarshal_kernel__validate__tb__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct nlattr* ptr);

void callee_marshal_kernel__validate__tb__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct nlattr const* ptr);

void caller_unmarshal_kernel__validate__tb__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct nlattr* ptr);

void caller_marshal_kernel__validate__data__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct nlattr const* ptr);

void callee_unmarshal_kernel__validate__data__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct nlattr* ptr);

void callee_marshal_kernel__validate__data__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct nlattr const* ptr);

void caller_unmarshal_kernel__validate__data__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct nlattr* ptr);

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
