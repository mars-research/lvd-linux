#ifndef _COMMON_H_
#define _COMMON_H_

#include <linux/types.h>

#include <liblcd/trampoline.h>
#include "dummy_driver_user.h"

#define MAX_MESSAGE_SLOTS 64

#define fipc_marshal(value) message->slots[marshal_slot++] = *(uint64_t*)&value
#define fipc_unmarshal(type) *(type*)&message->slots[marshal_slot++]
#define fipc_send(rpc, msg_ptr) /* TODO */
#define fipc_get_remote(local) NULL; (void)local
#define fipc_get_local(remote) NULL; (void)remote
#define fipc_create_shadow(remote) NULL; (void)remote
#define fipc_destroy_shadow(remote) (void)remote

#define inject_trampoline(id, pointer) inject_trampoline_impl(LCD_DUP_TRAMPOLINE(trampoline##id), pointer)

enum dispatch_id {
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
	RPC_PTR_1_KERNEL_NULLNET_RTNL_LINK_STATS64_1_KERNEL_NULLNET_NET_DEVICE_NDO_GET_STATS64_1_KERNEL_NULLNET_RTNL_LINK_STATS64_NDO_GET_STATS64,
	RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_RX_MODE,
	RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_SET_MAC_ADDRESS,
	RPC_PTR_INT_1_KERNEL_NULLNET_NET_DEVICE_NDO_CHANGE_CARRIER_BOOL,
	RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_GET_DRVINFO_1_KERNEL_NULLNET_ETHTOOL_DRVINFO_GET_DRVINFO,
	RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_DESTRUCTOR,
	RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP,
	RPC_PTR_VOID_1_KERNEL_NULLNET_NET_DEVICE_SETUP_CALLEE_ALLOC,
};

struct fipc_message {
	enum dispatch_id host_id;
	uint64_t slots[MAX_MESSAGE_SLOTS];
};

void* inject_trampoline_impl(lcd_trampoline_handle* handle, void* impl);

#endif