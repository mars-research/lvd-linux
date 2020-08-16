#ifndef _COMMON_H_
#define _COMMON_H_

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

#define fipc_marshal(value) *(message->end_slot++) = (uint64_t)value
#define fipc_unmarshal(type) (type)*(message->end_slot++)
#define fipc_create_shadow(remote) fipc_create_shadow_impl(remote, sizeof(*remote))

#define inject_trampoline(id, pointer) inject_trampoline_impl(LCD_DUP_TRAMPOLINE(trampoline##id), pointer)

struct ptr_node {
	void* ptr;
	void* key;
	struct hlist_node hentry;
};

DECLARE_HASHTABLE(locals, 5);
DECLARE_HASHTABLE(remotes, 5);

static inline void* fipc_get_remote(void* local) {
	struct ptr_node* node;
	hash_for_each_possible(remotes, node, hentry, (unsigned long)local) {
		if (node->key == local) return node->ptr;
	}

	BUG();
	return NULL;
}

static inline void* fipc_get_local(void* remote) {
	struct ptr_node* node;
	hash_for_each_possible(locals, node, hentry, (unsigned long)remote) {
		if (node->key == remote) return node->ptr;
	}

	BUG();
	return NULL;
}

static inline void* fipc_create_shadow_impl(void* remote, size_t size) {
	void* local = kmalloc(size, GFP_KERNEL);
	struct ptr_node* local_node = kmalloc(sizeof(struct ptr_node), GFP_KERNEL);
	struct ptr_node* remote_node = kmalloc(sizeof(struct ptr_node), GFP_KERNEL);
	local_node->ptr = local;
	local_node->key = remote;
	remote_node->ptr = remote;
	remote_node->key = local;
	hash_add(locals, &local_node->hentry, (unsigned long)remote);
	hash_add(remotes, &remote_node->hentry, (unsigned long)local);
	return local;
}

static inline void fipc_destroy_shadow(void* remote) {
	struct ptr_node* local_node;
	struct ptr_node* remote_node;
	hash_for_each_possible(locals, local_node, hentry, (unsigned long)remote) {
		if (local_node->key == remote) hash_del(&local_node->hentry);
	}

	void* local = local_node->ptr;
	hash_for_each_possible(remotes, remote_node, hentry, (unsigned long)local) {
		if (remote_node->key == local) hash_del(&remote_node->hentry);
	}

	kfree(local);
	kfree(local_node);
	kfree(remote_node);
}

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

static inline void* inject_trampoline_impl(struct lcd_trampoline_handle* handle, void* impl) {
	handle->hidden_args = impl;
	return handle->trampoline;
}

static inline void fipc_send(enum dispatch_id rpc, struct rpc_message* msg) {
	unsigned slots_used = msg->end_slot - msg->slots;
	struct fipc_message fmsg;
	fmsg.vmfunc_id = VMFUNC_RPC_CALL;
	fmsg.rpc_id = rpc | (slots_used << 16);

	unsigned fast_slots = min(slots_used, (unsigned)(FIPC_NR_REGS));
	unsigned slow_slots = min(slots_used - (unsigned)(FIPC_NR_REGS), 0u);

	for (unsigned i = 0; i < fast_slots; ++i) {
		fmsg.regs[i] = msg->slots[i];
	}

	if (slow_slots) {
		struct ext_registers* ext = get_register_page(smp_processor_id());
		for (unsigned i = 0; i < slow_slots; ++i) {
			ext->regs[i] = msg->slots[i + FIPC_NR_REGS];
		}
	}

	vmfunc_wrapper(&fmsg);
}

static inline void fipc_translate(struct fipc_message* msg, enum dispatch_id* rpc, struct rpc_message* pckt) {
	unsigned slots_used = msg->rpc_id >> 16;
	*rpc = msg->rpc_id & 0xFFFF;

	unsigned fast_slots = min(slots_used, (unsigned)(FIPC_NR_REGS));
	unsigned slow_slots = min(slots_used - (unsigned)(FIPC_NR_REGS), 0u);

	for (unsigned i = 0; i < fast_slots; ++i) {
		pckt->slots[i] = msg->regs[i];
	}

	if (slow_slots) {
		struct ext_registers* ext = get_register_page(smp_processor_id());
		for (unsigned i = 0; i < slow_slots; ++i) {
			pckt->slots[i + FIPC_NR_REGS] = ext->regs[i];
		}
	}

}

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


#endif