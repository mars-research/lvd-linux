#ifndef _COMMON_H_
#define _COMMON_H_

#include <asm/pgtable_types.h>
#include <asm/cacheflush.h>
#include <linux/types.h>
#include <linux/hashtable.h>
#include <libfipc.h>
#include <liblcd/boot_info.h>
#include <liblcd/trampoline.h>

#include "nullnet_user.h"

#include <linux/etherdevice.h>
#include <net/rtnetlink.h>
#include <asm/lcd_domains/libvmfunc.h>

#define MAX_MESSAGE_SLOTS 64

#define src_loc __FILE__ ":" __LINE__

#define fipc_marshal(value) *(message->end_slot++) = (uint64_t)value
#define fipc_unmarshal(type) (type)*(message->end_slot++)

struct ptr_node {
	void* ptr;
	void* key;
	struct hlist_node hentry;
};

enum dispatch_id {
	RPC_LCD_INIT,
	RPC_LCD_EXIT,
	RPC_RTNL_LOCK,
	RPC_RTNL_UNLOCK,
	RPC_FREE_NETDEV,
	RPC_ETH_VALIDATE_ADDR,
	RPC_ETH_MAC_ADDR,
	RPC_ETHER_SETUP,
	RPC_REGISTER_NETDEVICE,
	RPC_NETIF_CARRIER_OFF,
	RPC___RTNL_LINK_REGISTER,
	RPC___RTNL_LINK_UNREGISTER,
	RPC_RTNL_LINK_UNREGISTER,
	RPC_ALLOC_NETDEV_MQS,
	RPC_NETIF_CARRIER_ON,
	RPC_CONSUME_SKB,
	RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_INIT,
	RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_UNINIT,
	RPC_PTR_INT_1_KERNEL_NULLNET_SK_BUFF_NDO_START_XMIT_1_KERNEL_NULLNET_NET_DEVICE_STATS,
	RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_VALIDATE_ADDR,
	RPC_PTR_1_KERNEL_NULLNET_RTNL_LINK_STATS64_1_KERNEL_NULLNET_NET_DEVICE_NDO_GET_STATS64_1_KERNEL_NULLNET_RTNL_LINK_STATS64_NDO_GET_STATS64,
	RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_RX_MODE,
	RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_MAC_ADDRESS_1_KERNEL_NULLNET_SOCKADDR,
	RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_CHANGE_CARRIER_BOOL,
	RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_GET_DRVINFO_1_KERNEL_NULLNET_ETHTOOL_DRVINFO_GET_DRVINFO,
	RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_DESTRUCTOR,
	RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP,
	RPC_PTR_INT_1_KERNEL_NULLNET_VOID_PTR_1_KERNEL_NULLNET_VOID_PTR,
	RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP_CALLEE_ALLOC,
};

struct rpc_message {
	uint64_t* end_slot;
	uint64_t slots[MAX_MESSAGE_SLOTS];
};

struct trampoline_args {
	void* impl;
	void* real_ptr;
};

extern DECLARE_HASHTABLE(locals, 5);
extern DECLARE_HASHTABLE(remotes, 5);

static inline void trace_call(const char* name, const char* file, unsigned line)
{
	LIBLCD_MSG("%s:%u: %s", file, line, name);
}

#define fipc_create_shadow(remote) (trace_call("fipc_create_shadow", __FILE__, __LINE__), fipc_create_shadow_impl(remote, sizeof(*remote)))
#define inject_trampoline(id, pointer) (trace_call("inject_trampoline", __FILE__, __LINE__), inject_trampoline_impl(LCD_DUP_TRAMPOLINE(trampoline##id), trampoline_impl##id, LCD_TRAMPOLINE_SIZE(trampoline##id), pointer))
#define fipc_get_remote(local) (trace_call("fipc_get_remote", __FILE__, __LINE__), fipc_get_remote_impl(local))
#define fipc_get_local(remote) (trace_call("fipc_get_local", __FILE__, __LINE__), fipc_get_local_impl(remote))
#define fipc_destroy_shadow(remote) (trace_call("fipc_destroy_shadow", __FILE__, __LINE__), fipc_destroy_shadow_impl(remote))
#define fipc_translate(msg, rpc, pckt) (trace_call("fipc_translate", __FILE__, __LINE__), fipc_translate_impl(msg, rpc, pckt))
#define fipc_pack(fmsg, rpc, msg) (trace_call("fipc_pack", __FILE__, __LINE__), fipc_pack_impl(fmsg, rpc, msg))
#define fipc_send_to_klcd(rpc, msg) (trace_call("fipc_send_to_klcd", __FILE__, __LINE__), fipc_send_to_klcd_impl(rpc, msg))
#define fipc_send_to_lcd(rpc, msg) (trace_call("fipc_send_to_lcd", __FILE__, __LINE__), fipc_send_to_lcd_impl(rpc, msg))

void* fipc_get_remote_impl(void* local);
void* fipc_get_local_impl(void* remote);
void* fipc_create_shadow_impl(void* remote, size_t size);
void fipc_destroy_shadow_impl(void* remote);
void* inject_trampoline_impl(struct lcd_trampoline_handle* handle, void* impl, size_t size, void* real_ptr);
void fipc_translate_impl(struct fipc_message* msg, enum dispatch_id* rpc, struct rpc_message* pckt);
void fipc_pack_impl(struct fipc_message* fmsg, enum dispatch_id rpc, struct rpc_message* msg);
void fipc_send_to_klcd_impl(enum dispatch_id rpc, struct rpc_message* msg);
void fipc_send_to_lcd_impl(enum dispatch_id rpc, struct rpc_message* msg);

LCD_TRAMPOLINE_DATA(trampoline_int_1_kernel_nullnet_net_device_ndo_init)
int trampoline_int_1_kernel_nullnet_net_device_ndo_init(struct net_device* dev);

LCD_TRAMPOLINE_DATA(trampoline_void_1_kernel_nullnet_net_device_ndo_uninit)
void trampoline_void_1_kernel_nullnet_net_device_ndo_uninit(struct net_device* dev);

LCD_TRAMPOLINE_DATA(trampoline_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats)
int trampoline_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats(struct sk_buff* skb, struct net_device* dev);

LCD_TRAMPOLINE_DATA(trampoline_int_1_kernel_nullnet_net_device_ndo_validate_addr)
int trampoline_int_1_kernel_nullnet_net_device_ndo_validate_addr(struct net_device* dev);

LCD_TRAMPOLINE_DATA(trampoline_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64)
struct rtnl_link_stats64* trampoline_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64(struct net_device* dev, struct rtnl_link_stats64* storage);

LCD_TRAMPOLINE_DATA(trampoline_void_1_kernel_nullnet_net_device_ndo_set_rx_mode)
void trampoline_void_1_kernel_nullnet_net_device_ndo_set_rx_mode(struct net_device* dev);

LCD_TRAMPOLINE_DATA(trampoline_int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr)
int trampoline_int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr(struct net_device* dev, struct sockaddr_placeholder* addr);

LCD_TRAMPOLINE_DATA(trampoline_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool)
int trampoline_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool(struct net_device* dev, bool new_carrier);

LCD_TRAMPOLINE_DATA(trampoline_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo)
void trampoline_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo(struct net_device* dev, struct ethtool_drvinfo* info);

LCD_TRAMPOLINE_DATA(trampoline_void_1_kernel_nullnet_net_device_destructor)
void trampoline_void_1_kernel_nullnet_net_device_destructor(struct net_device* dev);

LCD_TRAMPOLINE_DATA(trampoline_void_1_kernel_nullnet_net_device_setup)
void trampoline_void_1_kernel_nullnet_net_device_setup(struct net_device* dev);

LCD_TRAMPOLINE_DATA(trampoline_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr)
int trampoline_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr(struct nlattr** tb, struct nlattr** data);

LCD_TRAMPOLINE_DATA(trampoline_void_1_kernel_nullnet_net_device_setup_callee_alloc)
void trampoline_void_1_kernel_nullnet_net_device_setup_callee_alloc(struct net_device* dev);

int trampoline_impl_int_1_kernel_nullnet_net_device_ndo_init(void* real_pointer, struct net_device* dev);

void trampoline_impl_void_1_kernel_nullnet_net_device_ndo_uninit(void* real_pointer, struct net_device* dev);

int trampoline_impl_int_1_kernel_nullnet_sk_buff_ndo_start_xmit_1_kernel_nullnet_net_device_stats(void* real_pointer, struct sk_buff* skb, struct net_device* dev);

int trampoline_impl_int_1_kernel_nullnet_net_device_ndo_validate_addr(void* real_pointer, struct net_device* dev);

struct rtnl_link_stats64* trampoline_impl_1_kernel_nullnet_rtnl_link_stats64_1_kernel_nullnet_net_device_ndo_get_stats64_1_kernel_nullnet_rtnl_link_stats64_ndo_get_stats64(void* real_pointer, struct net_device* dev, struct rtnl_link_stats64* storage);

void trampoline_impl_void_1_kernel_nullnet_net_device_ndo_set_rx_mode(void* real_pointer, struct net_device* dev);

int trampoline_impl_int_1_kernel_nullnet_net_device_ndo_set_mac_address_1_kernel_nullnet_sockaddr(void* real_pointer, struct net_device* dev, struct sockaddr_placeholder* addr);

int trampoline_impl_int_1_kernel_nullnet_net_device_ndo_change_carrier_bool(void* real_pointer, struct net_device* dev, bool new_carrier);

void trampoline_impl_void_1_kernel_nullnet_net_device_get_drvinfo_1_kernel_nullnet_ethtool_drvinfo_get_drvinfo(void* real_pointer, struct net_device* dev, struct ethtool_drvinfo* info);

void trampoline_impl_void_1_kernel_nullnet_net_device_destructor(void* real_pointer, struct net_device* dev);

void trampoline_impl_void_1_kernel_nullnet_net_device_setup(void* real_pointer, struct net_device* dev);

int trampoline_impl_int_1_kernel_nullnet_void_ptr_1_kernel_nullnet_void_ptr(void* real_pointer, struct nlattr** tb, struct nlattr** data);

void trampoline_impl_void_1_kernel_nullnet_net_device_setup_callee_alloc(void* real_pointer, struct net_device* dev);


#endif