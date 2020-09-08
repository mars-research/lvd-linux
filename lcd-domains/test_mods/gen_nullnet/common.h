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

#define fipc_marshal(value) *(message->end_slot++) = (uint64_t)value
#define fipc_unmarshal(type) (type)*(message->end_slot++)
#define fipc_create_shadow(remote) fipc_create_shadow_impl(remote, sizeof(*remote))

#define inject_trampoline(id, pointer) inject_trampoline_impl(LCD_DUP_TRAMPOLINE(trampoline##id), trampoline_impl##id, LCD_TRAMPOLINE_SIZE(trampoline##id), pointer)

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
		if (local_node->key == remote) {
			hash_del(&local_node->hentry);
			break;
		}
	}

	void* local = local_node->ptr;
	hash_for_each_possible(remotes, remote_node, hentry, (unsigned long)local) {
		if (remote_node->key == local) {
			hash_del(&remote_node->hentry);
			break;
		}
	}

	kfree(local);
	kfree(local_node);
	kfree(remote_node);
}

static inline void* inject_trampoline_impl(struct lcd_trampoline_handle* handle, void* impl, size_t size, void* real_ptr) {
	struct trampoline_args* args = kmalloc(sizeof(struct trampoline_args), GFP_KERNEL);
	args->impl = impl;
	args->real_ptr = real_ptr;
	handle->hidden_args = args;
	set_memory_x(((unsigned long)handle) & PAGE_MASK, ALIGN(size, PAGE_SIZE) >> PAGE_SHIFT);
	return handle->trampoline;
}

static inline void fipc_translate(struct fipc_message* msg, enum dispatch_id* rpc, struct rpc_message* pckt) {
	pckt->end_slot = pckt->slots; // NEW

	unsigned slots_used = msg->rpc_id >> 16;
	*rpc = msg->rpc_id & 0xFFFF;

	unsigned fast_slots = min(slots_used, (unsigned)(FIPC_NR_REGS));
	unsigned slow_slots = (slots_used > FIPC_NR_REGS) ? slots_used - FIPC_NR_REGS : 0; // NEW

	for (unsigned i = 0; i < fast_slots; ++i) {
		LIBLCD_MSG("Unpacking fast slot %u: %u64", i, msg->regs[i]);
		pckt->slots[i] = msg->regs[i];
	}

	if (slow_slots) {
		LIBLCD_MSG("Preparing ext page");
		unsigned id = smp_processor_id();
		LIBLCD_MSG("On core %u", id);
		struct ext_registers* ext = get_register_page(id);
		for (unsigned i = 0; i < slow_slots; ++i) {
			LIBLCD_MSG("Unpacking slow slot %u: %u64", i, ext->regs[i]);
			pckt->slots[i + FIPC_NR_REGS] = ext->regs[i];
		}
	}

}

static inline void fipc_pack(struct fipc_message* fmsg, enum dispatch_id rpc, struct rpc_message* msg) {
	unsigned slots_used = msg->end_slot - msg->slots;
	fmsg->vmfunc_id = VMFUNC_RPC_CALL;
	fmsg->rpc_id = rpc | (slots_used << 16);

	unsigned fast_slots = min(slots_used, (unsigned)(FIPC_NR_REGS));
	unsigned slow_slots = (slots_used > FIPC_NR_REGS) ? slots_used - FIPC_NR_REGS : 0; // NEW

	for (unsigned i = 0; i < fast_slots; ++i) {
		LIBLCD_MSG("Packing fast slot %u: %u64", i, msg->slots[i]);
		fmsg->regs[i] = msg->slots[i];
	}

	if (slow_slots) {
		LIBLCD_MSG("Preparing ext page");
		unsigned id = smp_processor_id();
		LIBLCD_MSG("On core %u", id);
		struct ext_registers* ext = get_register_page(id);
		for (unsigned i = 0; i < slow_slots; ++i) {
			LIBLCD_MSG("Packing slow slot %u: %u64", i, msg->slots[i + FIPC_NR_REGS]);
			ext->regs[i] = msg->slots[i + FIPC_NR_REGS];
		}
	}

}

static inline void fipc_send_to_klcd(enum dispatch_id rpc, struct rpc_message* msg) {
	struct fipc_message fmsg;
	fipc_pack(&fmsg, rpc, msg);
	vmfunc_wrapper(&fmsg);
	fipc_translate(&fmsg, &rpc, msg);
}

static inline void fipc_send_to_lcd(enum dispatch_id rpc, struct rpc_message* msg) {
	struct fipc_message fmsg;
	fipc_pack(&fmsg, rpc, msg);
	vmfunc_klcd_wrapper(&fmsg, OTHER_DOMAIN);
	fipc_translate(&fmsg, &rpc, msg);
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