#ifndef COMMON_H
#define COMMON_H

#include <liblcd/trampoline.h>
#include <libfipc.h>
#include <liblcd/boot_info.h>
#include <asm/cacheflush.h>
#include <lcd_domains/microkernel.h>
#include <liblcd/liblcd.h>

#include "glue_user.h"

#define DEFAULT_GFP_FLAGS  (GFP_KERNEL)
#define verbose_debug 1
#define glue_pack(pos, msg, ext, value) glue_pack_impl((pos), (msg), (ext), (uint64_t)(value))
#define glue_pack_shadow(pos, msg, ext, value) glue_pack_shadow_impl((pos), (msg), (ext), (value))
#define glue_unpack(pos, msg, ext, type) (type)glue_unpack_impl((pos), (msg), (ext))
#define glue_unpack_shadow(pos, msg, ext, type) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack shadow for type %s\n", __func__, __LINE__, __stringify(type)); \
	(type)glue_unpack_shadow_impl(glue_unpack(pos, msg, ext, void*)); })

#define glue_unpack_new_shadow(pos, msg, ext, type, size, flags) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack new shadow for type %s | size %llu\n", __func__, __LINE__, __stringify(type), (uint64_t) size); \
	(type)glue_unpack_new_shadow_impl(glue_unpack(pos, msg, ext, void*), size, flags); })

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
void* glue_user_map_to_shadow(const void* obj, bool fail);
const void* glue_user_map_from_shadow(const void* shadow);
void glue_user_add_shadow(const void* ptr, void* shadow);
void* glue_user_alloc(size_t size, gfp_t flags);
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
	if (*pos >= 512)
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

static inline void* glue_unpack_new_shadow_impl(const void* ptr, size_t size, gfp_t flags)
{
	void* shadow = 0;
	if (!ptr)
		return NULL;

	shadow = glue_user_alloc(size, flags);
	glue_user_add_shadow(ptr, shadow);
	return shadow;
}

static inline void* glue_unpack_bind_or_new_shadow_impl(const void* ptr, size_t size)
{
	void* shadow = 0;
	if (!ptr)
		return NULL;

	shadow = glue_user_map_to_shadow(ptr, false);
	if (!shadow) {
		shadow = glue_user_alloc(size, DEFAULT_GFP_FLAGS);
		glue_user_add_shadow(ptr, shadow);
	}
	return shadow;
}

static inline void* glue_unpack_shadow_impl(const void* ptr)
{
	return ptr ? glue_user_map_to_shadow(ptr, true) : NULL;
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
	RPC_ID_ethtool_ops_set_settings,
	RPC_ID_ethtool_ops_get_settings,
	RPC_ID_ethtool_ops_get_msglevel,
	RPC_ID_ethtool_ops_set_msglevel,
	RPC_ID_thread_fn,
	RPC_ID_handler,
	RPC_ID_request_threaded_irq,
	RPC_ID_free_irq,
	RPC_ID_synchronize_irq,
	RPC_ID_rtnl_unlock,
	RPC_ID_rtnl_lock,
	RPC_ID_cancel_work_sync,
	RPC_ID_probe,
	RPC_ID_remove,
	RPC_ID_pci_unregister_driver,
	RPC_ID___pci_register_driver,
	RPC_ID_ethtool_ops_get_link,
	RPC_ID_pci_enable_msi_range,
	RPC_ID_ethtool_ops_get_pauseparam,
	RPC_ID_ethtool_ops_set_pauseparam,
	RPC_ID_ethtool_ops_get_strings,
	RPC_ID_ethtool_ops_get_ethtool_stats,
	RPC_ID_ethtool_ops_get_sset_count,
	RPC_ID_net_device_ops_ndo_open,
	RPC_ID_net_device_ops_ndo_stop,
	RPC_ID_net_device_ops_ndo_start_xmit,
	RPC_ID_net_device_ops_ndo_set_rx_mode,
	RPC_ID_net_device_ops_ndo_set_mac_address,
	RPC_ID_net_device_ops_ndo_do_ioctl,
	RPC_ID_net_device_ops_ndo_change_mtu,
	RPC_ID_net_device_ops_ndo_tx_timeout,
	RPC_ID_net_device_ops_ndo_get_stats64,
	RPC_ID_net_device_ops_ndo_fix_features,
	RPC_ID_pci_bus_write_config_word,
	RPC_ID_pci_bus_read_config_word,
	RPC_ID_mdio_read,
	RPC_ID_mdio_write,
	RPC_ID_mdio_mii_ioctl,
	RPC_ID_pci_disable_msi,
	RPC_ID_pci_disable_device,
	RPC_ID_queue_work_on,
	RPC_ID_consume_skb,
	RPC_ID_napi_complete_done,
	RPC_ID_alloc_etherdev_mqs,
	RPC_ID_free_netdev,
	RPC_ID_netif_napi_del,
	RPC_ID_rtnl_is_locked,
	RPC_ID_net_device_ops_ndo_validate_addr,
	RPC_ID_napi_disable,
	RPC_ID_poll,
	RPC_ID_netif_napi_add,
	RPC_ID___napi_schedule,
	RPC_ID___dev_kfree_skb_any,
	RPC_ID_netif_device_attach,
	RPC_ID_netif_tx_wake_queue,
	RPC_ID_netif_device_detach,
	RPC_ID_netif_schedule_queue,
	RPC_ID_unregister_netdev,
	RPC_ID_register_netdev,
	RPC_ID_netdev_update_features,
	RPC_ID_netif_carrier_on,
	RPC_ID_netif_carrier_off,
	RPC_ID_napi_gro_receive,
	RPC_ID_skb_put,
	RPC_ID___netdev_alloc_skb,
	RPC_ID_pci_select_bars,
	RPC_ID_pcie_set_readrq,
	RPC_ID_pcie_get_readrq,
	RPC_ID_pci_set_master,
	RPC_ID_pci_enable_pcie_error_reporting,
	RPC_ID_pci_disable_pcie_error_reporting,
	RPC_ID_pci_request_selected_regions,
	RPC_ID_pci_release_selected_regions,
	RPC_ID_pci_enable_device,
	RPC_ID_pci_enable_device_mem,
	RPC_ID_pci_ioremap_bar,
	RPC_ID_get_loops_per_jiffy,
	RPC_ID_get_jiffies,
	RPC_ID_eth_validate_addr,
	RPC_ID_ethtool_op_get_link,
};

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext);

typedef int (*fptr_ethtool_ops_set_settings)(struct net_device* netdev, struct ethtool_cmd* ecmd);
typedef int (*fptr_impl_ethtool_ops_set_settings)(fptr_ethtool_ops_set_settings target, struct net_device* netdev, struct ethtool_cmd* ecmd);

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_set_settings)
int LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_set_settings) trmp_ethtool_ops_set_settings(struct net_device* netdev, struct ethtool_cmd* ecmd);

typedef int (*fptr_ethtool_ops_get_settings)(struct net_device* netdev, struct ethtool_cmd* ecmd);
typedef int (*fptr_impl_ethtool_ops_get_settings)(fptr_ethtool_ops_get_settings target, struct net_device* netdev, struct ethtool_cmd* ecmd);

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_settings)
int LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_settings) trmp_ethtool_ops_get_settings(struct net_device* netdev, struct ethtool_cmd* ecmd);

typedef unsigned int (*fptr_ethtool_ops_get_msglevel)(struct net_device* netdev);
typedef unsigned int (*fptr_impl_ethtool_ops_get_msglevel)(fptr_ethtool_ops_get_msglevel target, struct net_device* netdev);

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_msglevel)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_msglevel) trmp_ethtool_ops_get_msglevel(struct net_device* netdev);

typedef void (*fptr_ethtool_ops_set_msglevel)(struct net_device* netdev, unsigned int data);
typedef void (*fptr_impl_ethtool_ops_set_msglevel)(fptr_ethtool_ops_set_msglevel target, struct net_device* netdev, unsigned int data);

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_set_msglevel)
void LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_set_msglevel) trmp_ethtool_ops_set_msglevel(struct net_device* netdev, unsigned int data);

typedef unsigned int (*fptr_thread_fn)(int irq, void* id);
typedef unsigned int (*fptr_impl_thread_fn)(fptr_thread_fn target, int irq, void* id);

LCD_TRAMPOLINE_DATA(trmp_thread_fn)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_thread_fn) trmp_thread_fn(int irq, void* id);

typedef unsigned int (*fptr_handler)(int irq, void* id);
typedef unsigned int (*fptr_impl_handler)(fptr_handler target, int irq, void* id);

LCD_TRAMPOLINE_DATA(trmp_handler)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_handler) trmp_handler(int irq, void* id);

typedef int (*fptr_probe)(struct pci_dev* pdev, struct pci_device_id* ent);
typedef int (*fptr_impl_probe)(fptr_probe target, struct pci_dev* pdev, struct pci_device_id* ent);

LCD_TRAMPOLINE_DATA(trmp_probe)
int LCD_TRAMPOLINE_LINKAGE(trmp_probe) trmp_probe(struct pci_dev* pdev, struct pci_device_id* ent);

typedef void (*fptr_remove)(struct pci_dev* pdev);
typedef void (*fptr_impl_remove)(fptr_remove target, struct pci_dev* pdev);

LCD_TRAMPOLINE_DATA(trmp_remove)
void LCD_TRAMPOLINE_LINKAGE(trmp_remove) trmp_remove(struct pci_dev* pdev);

typedef unsigned int (*fptr_ethtool_ops_get_link)(struct net_device* netdev);
typedef unsigned int (*fptr_impl_ethtool_ops_get_link)(fptr_ethtool_ops_get_link target, struct net_device* netdev);

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_link)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_link) trmp_ethtool_ops_get_link(struct net_device* netdev);

typedef void (*fptr_ethtool_ops_get_pauseparam)(struct net_device* netdev, struct ethtool_pauseparam* pause);
typedef void (*fptr_impl_ethtool_ops_get_pauseparam)(fptr_ethtool_ops_get_pauseparam target, struct net_device* netdev, struct ethtool_pauseparam* pause);

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_pauseparam)
void LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_pauseparam) trmp_ethtool_ops_get_pauseparam(struct net_device* netdev, struct ethtool_pauseparam* pause);

typedef int (*fptr_ethtool_ops_set_pauseparam)(struct net_device* netdev, struct ethtool_pauseparam* pause);
typedef int (*fptr_impl_ethtool_ops_set_pauseparam)(fptr_ethtool_ops_set_pauseparam target, struct net_device* netdev, struct ethtool_pauseparam* pause);

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_set_pauseparam)
int LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_set_pauseparam) trmp_ethtool_ops_set_pauseparam(struct net_device* netdev, struct ethtool_pauseparam* pause);

typedef void (*fptr_ethtool_ops_get_strings)(struct net_device* netdev, unsigned int stringset, unsigned char* buf);
typedef void (*fptr_impl_ethtool_ops_get_strings)(fptr_ethtool_ops_get_strings target, struct net_device* netdev, unsigned int stringset, unsigned char* buf);

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_strings)
void LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_strings) trmp_ethtool_ops_get_strings(struct net_device* netdev, unsigned int stringset, unsigned char* buf);

typedef void (*fptr_ethtool_ops_get_ethtool_stats)(struct net_device* netdev, struct ethtool_stats* estats, unsigned long long* data);
typedef void (*fptr_impl_ethtool_ops_get_ethtool_stats)(fptr_ethtool_ops_get_ethtool_stats target, struct net_device* netdev, struct ethtool_stats* estats, unsigned long long* data);

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_ethtool_stats)
void LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_ethtool_stats) trmp_ethtool_ops_get_ethtool_stats(struct net_device* netdev, struct ethtool_stats* estats, unsigned long long* data);

typedef int (*fptr_ethtool_ops_get_sset_count)(struct net_device* netdev, int sset);
typedef int (*fptr_impl_ethtool_ops_get_sset_count)(fptr_ethtool_ops_get_sset_count target, struct net_device* netdev, int sset);

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_sset_count)
int LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_sset_count) trmp_ethtool_ops_get_sset_count(struct net_device* netdev, int sset);

typedef int (*fptr_net_device_ops_ndo_open)(struct net_device* netdev);
typedef int (*fptr_impl_net_device_ops_ndo_open)(fptr_net_device_ops_ndo_open target, struct net_device* netdev);

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_open)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_open) trmp_net_device_ops_ndo_open(struct net_device* netdev);

typedef int (*fptr_net_device_ops_ndo_stop)(struct net_device* netdev);
typedef int (*fptr_impl_net_device_ops_ndo_stop)(fptr_net_device_ops_ndo_stop target, struct net_device* netdev);

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_stop)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_stop) trmp_net_device_ops_ndo_stop(struct net_device* netdev);

typedef int (*fptr_net_device_ops_ndo_start_xmit)(struct sk_buff* skb, struct net_device* netdev);
typedef int (*fptr_impl_net_device_ops_ndo_start_xmit)(fptr_net_device_ops_ndo_start_xmit target, struct sk_buff* skb, struct net_device* netdev);

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_start_xmit)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_start_xmit) trmp_net_device_ops_ndo_start_xmit(struct sk_buff* skb, struct net_device* netdev);

typedef void (*fptr_net_device_ops_ndo_set_rx_mode)(struct net_device* netdev);
typedef void (*fptr_impl_net_device_ops_ndo_set_rx_mode)(fptr_net_device_ops_ndo_set_rx_mode target, struct net_device* netdev);

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_set_rx_mode)
void LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_set_rx_mode) trmp_net_device_ops_ndo_set_rx_mode(struct net_device* netdev);

typedef int (*fptr_net_device_ops_ndo_set_mac_address)(struct net_device* netdev, struct sockaddr* data);
typedef int (*fptr_impl_net_device_ops_ndo_set_mac_address)(fptr_net_device_ops_ndo_set_mac_address target, struct net_device* netdev, struct sockaddr* data);

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_set_mac_address)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_set_mac_address) trmp_net_device_ops_ndo_set_mac_address(struct net_device* netdev, struct sockaddr* data);

typedef int (*fptr_net_device_ops_ndo_do_ioctl)(struct net_device* netdev, struct ifreq* ifr, int cmd);
typedef int (*fptr_impl_net_device_ops_ndo_do_ioctl)(fptr_net_device_ops_ndo_do_ioctl target, struct net_device* netdev, struct ifreq* ifr, int cmd);

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_do_ioctl)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_do_ioctl) trmp_net_device_ops_ndo_do_ioctl(struct net_device* netdev, struct ifreq* ifr, int cmd);

typedef int (*fptr_net_device_ops_ndo_change_mtu)(struct net_device* netdev, int mtu);
typedef int (*fptr_impl_net_device_ops_ndo_change_mtu)(fptr_net_device_ops_ndo_change_mtu target, struct net_device* netdev, int mtu);

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_change_mtu)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_change_mtu) trmp_net_device_ops_ndo_change_mtu(struct net_device* netdev, int mtu);

typedef void (*fptr_net_device_ops_ndo_tx_timeout)(struct net_device* dev);
typedef void (*fptr_impl_net_device_ops_ndo_tx_timeout)(fptr_net_device_ops_ndo_tx_timeout target, struct net_device* dev);

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_tx_timeout)
void LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_tx_timeout) trmp_net_device_ops_ndo_tx_timeout(struct net_device* dev);

typedef struct rtnl_link_stats64* (*fptr_net_device_ops_ndo_get_stats64)(struct net_device* dev, struct rtnl_link_stats64* net_stats);
typedef struct rtnl_link_stats64* (*fptr_impl_net_device_ops_ndo_get_stats64)(fptr_net_device_ops_ndo_get_stats64 target, struct net_device* dev, struct rtnl_link_stats64* net_stats);

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_get_stats64)
struct rtnl_link_stats64* LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_get_stats64) trmp_net_device_ops_ndo_get_stats64(struct net_device* dev, struct rtnl_link_stats64* net_stats);

typedef unsigned long long (*fptr_net_device_ops_ndo_fix_features)(struct net_device* netdev, unsigned long long features);
typedef unsigned long long (*fptr_impl_net_device_ops_ndo_fix_features)(fptr_net_device_ops_ndo_fix_features target, struct net_device* netdev, unsigned long long features);

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_fix_features)
unsigned long long LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_fix_features) trmp_net_device_ops_ndo_fix_features(struct net_device* netdev, unsigned long long features);

typedef int (*fptr_mdio_read)(struct net_device* dev, int prtad, int devad, unsigned short addr);
typedef int (*fptr_impl_mdio_read)(fptr_mdio_read target, struct net_device* dev, int prtad, int devad, unsigned short addr);

LCD_TRAMPOLINE_DATA(trmp_mdio_read)
int LCD_TRAMPOLINE_LINKAGE(trmp_mdio_read) trmp_mdio_read(struct net_device* dev, int prtad, int devad, unsigned short addr);

typedef int (*fptr_mdio_write)(struct net_device* dev, int prtad, int devad, unsigned short addr, unsigned short val);
typedef int (*fptr_impl_mdio_write)(fptr_mdio_write target, struct net_device* dev, int prtad, int devad, unsigned short addr, unsigned short val);

LCD_TRAMPOLINE_DATA(trmp_mdio_write)
int LCD_TRAMPOLINE_LINKAGE(trmp_mdio_write) trmp_mdio_write(struct net_device* dev, int prtad, int devad, unsigned short addr, unsigned short val);

typedef int (*fptr_net_device_ops_ndo_validate_addr)(struct net_device* dev);
typedef int (*fptr_impl_net_device_ops_ndo_validate_addr)(fptr_net_device_ops_ndo_validate_addr target, struct net_device* dev);

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_validate_addr)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_validate_addr) trmp_net_device_ops_ndo_validate_addr(struct net_device* dev);

typedef int (*fptr_poll)(struct napi_struct* napi, int weight);
typedef int (*fptr_impl_poll)(fptr_poll target, struct napi_struct* napi, int weight);

LCD_TRAMPOLINE_DATA(trmp_poll)
int LCD_TRAMPOLINE_LINKAGE(trmp_poll) trmp_poll(struct napi_struct* napi, int weight);

struct ethtool_ops_set_settings_call_ctx {
	struct net_device* netdev;
	struct ethtool_cmd* ecmd;
};

struct ethtool_ops_get_settings_call_ctx {
	struct net_device* netdev;
	struct ethtool_cmd* ecmd;
};

struct ethtool_ops_get_msglevel_call_ctx {
	struct net_device* netdev;
};

struct ethtool_ops_set_msglevel_call_ctx {
	struct net_device* netdev;
	unsigned int data;
};

struct thread_fn_call_ctx {
	int irq;
	void* id;
};

struct handler_call_ctx {
	int irq;
	void* id;
};

struct request_threaded_irq_call_ctx {
	unsigned int irq;
	fptr_handler handler;
	fptr_thread_fn thread_fn;
	unsigned long irqflags;
	char const* devname;
	void* dev_id;
};

struct free_irq_call_ctx {
	unsigned int irq;
	void* dev_id;
};

struct synchronize_irq_call_ctx {
	unsigned int irq;
};

struct rtnl_unlock_call_ctx {
};

struct rtnl_lock_call_ctx {
};

struct cancel_work_sync_call_ctx {
	struct work_struct* work;
};

struct probe_call_ctx {
	struct pci_dev* pdev;
	struct pci_device_id* ent;
};

struct remove_call_ctx {
	struct pci_dev* pdev;
};

struct pci_unregister_driver_call_ctx {
	struct pci_driver* drv;
};

struct __pci_register_driver_call_ctx {
	struct pci_driver* drv;
	struct module* owner;
	char const* mod_name;
};

struct ethtool_ops_get_link_call_ctx {
	struct net_device* netdev;
};

struct pci_enable_msi_range_call_ctx {
	struct pci_dev* dev;
	int minvec;
	int maxvec;
};

struct ethtool_ops_get_pauseparam_call_ctx {
	struct net_device* netdev;
	struct ethtool_pauseparam* pause;
};

struct ethtool_ops_set_pauseparam_call_ctx {
	struct net_device* netdev;
	struct ethtool_pauseparam* pause;
};

struct ethtool_ops_get_strings_call_ctx {
	struct net_device* netdev;
	unsigned int stringset;
	unsigned char* buf;
};

struct ethtool_ops_get_ethtool_stats_call_ctx {
	struct net_device* netdev;
	struct ethtool_stats* estats;
	unsigned long long* data;
};

struct ethtool_ops_get_sset_count_call_ctx {
	struct net_device* netdev;
	int sset;
};

struct net_device_ops_ndo_open_call_ctx {
	struct net_device* netdev;
};

struct net_device_ops_ndo_stop_call_ctx {
	struct net_device* netdev;
};

struct net_device_ops_ndo_start_xmit_call_ctx {
	struct sk_buff* skb;
	struct net_device* netdev;
};

struct net_device_ops_ndo_set_rx_mode_call_ctx {
	struct net_device* netdev;
};

struct net_device_ops_ndo_set_mac_address_call_ctx {
	struct net_device* netdev;
	struct sockaddr* data;
};

struct net_device_ops_ndo_do_ioctl_call_ctx {
	struct net_device* netdev;
	struct ifreq* ifr;
	int cmd;
};

struct net_device_ops_ndo_change_mtu_call_ctx {
	struct net_device* netdev;
	int mtu;
};

struct net_device_ops_ndo_tx_timeout_call_ctx {
	struct net_device* dev;
};

struct net_device_ops_ndo_get_stats64_call_ctx {
	struct net_device* dev;
	struct rtnl_link_stats64* net_stats;
};

struct net_device_ops_ndo_fix_features_call_ctx {
	struct net_device* netdev;
	unsigned long long features;
};

struct pci_bus_write_config_word_call_ctx {
	struct pci_bus* bus;
	unsigned int devfn;
	int pos;
	unsigned short value;
};

struct pci_bus_read_config_word_call_ctx {
	struct pci_bus* bus;
	unsigned int devfn;
	int pos;
	unsigned short* value;
};

struct mdio_read_call_ctx {
	struct net_device* dev;
	int prtad;
	int devad;
	unsigned short addr;
};

struct mdio_write_call_ctx {
	struct net_device* dev;
	int prtad;
	int devad;
	unsigned short addr;
	unsigned short val;
};

struct mdio_mii_ioctl_call_ctx {
	struct mdio_if_info const* mdio;
	struct mii_ioctl_data* mii_data;
	int cmd;
};

struct pci_disable_msi_call_ctx {
	struct pci_dev* dev;
};

struct pci_disable_device_call_ctx {
	struct pci_dev* dev;
};

struct queue_work_on_call_ctx {
	int cpu;
	struct workqueue_struct* wq;
	struct work_struct* work;
};

struct consume_skb_call_ctx {
	struct sk_buff* skb;
};

struct napi_complete_done_call_ctx {
	struct napi_struct* n;
	int work_done;
};

struct alloc_etherdev_mqs_call_ctx {
	int sizeof_priv;
	unsigned int txqs;
	unsigned int rxqs;
};

struct free_netdev_call_ctx {
	struct net_device* dev;
};

struct netif_napi_del_call_ctx {
	struct napi_struct* napi;
};

struct rtnl_is_locked_call_ctx {
};

struct net_device_ops_ndo_validate_addr_call_ctx {
	struct net_device* dev;
};

struct napi_disable_call_ctx {
	struct napi_struct* n;
};

struct poll_call_ctx {
	struct napi_struct* napi;
	int weight;
};

struct netif_napi_add_call_ctx {
	struct net_device* dev;
	struct napi_struct* napi;
	fptr_poll poll;
	int weight;
};

struct __napi_schedule_call_ctx {
	struct napi_struct* n;
};

struct __dev_kfree_skb_any_call_ctx {
	struct sk_buff* skb;
	unsigned int reason;
};

struct netif_device_attach_call_ctx {
	struct net_device* dev;
};

struct netif_tx_wake_queue_call_ctx {
	struct netdev_queue* dev_queue;
};

struct netif_device_detach_call_ctx {
	struct net_device* dev;
};

struct netif_schedule_queue_call_ctx {
	struct netdev_queue* txq;
};

struct unregister_netdev_call_ctx {
	struct net_device* dev;
};

struct register_netdev_call_ctx {
	struct net_device* dev;
};

struct netdev_update_features_call_ctx {
	struct net_device* dev;
};

struct netif_carrier_on_call_ctx {
	struct net_device* dev;
};

struct netif_carrier_off_call_ctx {
	struct net_device* dev;
};

struct napi_gro_receive_call_ctx {
	struct napi_struct* napi;
	struct sk_buff* skb;
};

struct skb_put_call_ctx {
	struct sk_buff* skb;
	unsigned int len;
};

struct __netdev_alloc_skb_call_ctx {
	struct net_device* dev;
	unsigned int len;
	unsigned int gfp_mask;
};

struct pci_select_bars_call_ctx {
	struct pci_dev* dev;
	unsigned long flags;
};

struct pcie_set_readrq_call_ctx {
	struct pci_dev* dev;
	int rq;
};

struct pcie_get_readrq_call_ctx {
	struct pci_dev* dev;
};

struct pci_set_master_call_ctx {
	struct pci_dev* dev;
};

struct pci_enable_pcie_error_reporting_call_ctx {
	struct pci_dev* dev;
};

struct pci_disable_pcie_error_reporting_call_ctx {
	struct pci_dev* dev;
};

struct pci_request_selected_regions_call_ctx {
	struct pci_dev* pdev;
	int bars;
	char const* res_name;
};

struct pci_release_selected_regions_call_ctx {
	struct pci_dev* pdev;
	int bars;
};

struct pci_enable_device_call_ctx {
	struct pci_dev* dev;
};

struct pci_enable_device_mem_call_ctx {
	struct pci_dev* dev;
};

struct pci_ioremap_bar_call_ctx {
	struct pci_dev* pdev;
	int bar;
};

struct get_loops_per_jiffy_call_ctx {
};

struct get_jiffies_call_ctx {
};

struct eth_validate_addr_call_ctx {
	struct net_device* dev;
};

struct ethtool_op_get_link_call_ctx {
	struct net_device* dev;
};

void caller_marshal_kernel__ethtool_ops_set_settings__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ethtool_ops_set_settings__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ethtool_ops_set_settings__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ethtool_ops_set_settings__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ethtool_ops_set_settings__ecmd__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* call_ctx,
	struct ethtool_cmd const* ptr);

void callee_unmarshal_kernel__ethtool_ops_set_settings__ecmd__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* call_ctx,
	struct ethtool_cmd* ptr);

void callee_marshal_kernel__ethtool_ops_set_settings__ecmd__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* call_ctx,
	struct ethtool_cmd const* ptr);

void caller_unmarshal_kernel__ethtool_ops_set_settings__ecmd__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* call_ctx,
	struct ethtool_cmd* ptr);

void caller_marshal_kernel__ethtool_ops_get_settings__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ethtool_ops_get_settings__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ethtool_ops_get_settings__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ethtool_ops_get_settings__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ethtool_ops_get_settings__ecmd__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* call_ctx,
	struct ethtool_cmd const* ptr);

void callee_unmarshal_kernel__ethtool_ops_get_settings__ecmd__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* call_ctx,
	struct ethtool_cmd* ptr);

void callee_marshal_kernel__ethtool_ops_get_settings__ecmd__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* call_ctx,
	struct ethtool_cmd const* ptr);

void caller_unmarshal_kernel__ethtool_ops_get_settings__ecmd__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* call_ctx,
	struct ethtool_cmd* ptr);

void caller_marshal_kernel__ethtool_ops_get_msglevel__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_msglevel_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ethtool_ops_get_msglevel__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_msglevel_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ethtool_ops_get_msglevel__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_msglevel_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ethtool_ops_get_msglevel__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_msglevel_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ethtool_ops_set_msglevel__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_msglevel_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ethtool_ops_set_msglevel__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_msglevel_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ethtool_ops_set_msglevel__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_msglevel_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ethtool_ops_set_msglevel__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_msglevel_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__cancel_work_sync__work__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct cancel_work_sync_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void callee_unmarshal_kernel__cancel_work_sync__work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct cancel_work_sync_call_ctx const* call_ctx,
	struct work_struct* ptr);

void callee_marshal_kernel__cancel_work_sync__work__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct cancel_work_sync_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void caller_unmarshal_kernel__cancel_work_sync__work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct cancel_work_sync_call_ctx const* call_ctx,
	struct work_struct* ptr);

void caller_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__probe__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__probe__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__probe__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__probe__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__probe__pci_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__probe__pci_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__probe__pci_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__probe__pci_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__probe__ent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void callee_unmarshal_kernel__probe__ent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void callee_marshal_kernel__probe__ent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void caller_unmarshal_kernel__probe__ent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void caller_marshal_kernel__remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct remove_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct remove_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct remove_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct remove_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void callee_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void callee_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void caller_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void caller_marshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void callee_unmarshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void callee_marshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void caller_unmarshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void caller_marshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void callee_unmarshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void callee_marshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void caller_unmarshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void caller_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module const* ptr);

void callee_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module* ptr);

void callee_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module const* ptr);

void caller_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module* ptr);

void caller_marshal_kernel__ethtool_ops_get_link__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_link_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ethtool_ops_get_link__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_link_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ethtool_ops_get_link__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_link_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ethtool_ops_get_link__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_link_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__ethtool_ops_get_pauseparam__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ethtool_ops_get_pauseparam__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ethtool_ops_get_pauseparam__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ethtool_ops_get_pauseparam__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ethtool_ops_get_pauseparam__pause__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* call_ctx,
	struct ethtool_pauseparam const* ptr);

void callee_unmarshal_kernel__ethtool_ops_get_pauseparam__pause__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* call_ctx,
	struct ethtool_pauseparam* ptr);

void callee_marshal_kernel__ethtool_ops_get_pauseparam__pause__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* call_ctx,
	struct ethtool_pauseparam const* ptr);

void caller_unmarshal_kernel__ethtool_ops_get_pauseparam__pause__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* call_ctx,
	struct ethtool_pauseparam* ptr);

void caller_marshal_kernel__ethtool_ops_set_pauseparam__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ethtool_ops_set_pauseparam__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ethtool_ops_set_pauseparam__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ethtool_ops_set_pauseparam__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ethtool_ops_set_pauseparam__pause__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* call_ctx,
	struct ethtool_pauseparam const* ptr);

void callee_unmarshal_kernel__ethtool_ops_set_pauseparam__pause__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* call_ctx,
	struct ethtool_pauseparam* ptr);

void callee_marshal_kernel__ethtool_ops_set_pauseparam__pause__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* call_ctx,
	struct ethtool_pauseparam const* ptr);

void caller_unmarshal_kernel__ethtool_ops_set_pauseparam__pause__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* call_ctx,
	struct ethtool_pauseparam* ptr);

void caller_marshal_kernel__ethtool_ops_get_strings__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_strings_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ethtool_ops_get_strings__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_strings_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ethtool_ops_get_strings__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_strings_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ethtool_ops_get_strings__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_strings_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ethtool_ops_get_ethtool_stats__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ethtool_ops_get_ethtool_stats__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ethtool_ops_get_ethtool_stats__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ethtool_ops_get_ethtool_stats__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ethtool_ops_get_ethtool_stats__estats__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* call_ctx,
	struct ethtool_stats const* ptr);

void callee_unmarshal_kernel__ethtool_ops_get_ethtool_stats__estats__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* call_ctx,
	struct ethtool_stats* ptr);

void callee_marshal_kernel__ethtool_ops_get_ethtool_stats__estats__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* call_ctx,
	struct ethtool_stats const* ptr);

void caller_unmarshal_kernel__ethtool_ops_get_ethtool_stats__estats__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* call_ctx,
	struct ethtool_stats* ptr);

void caller_marshal_kernel__ethtool_ops_get_sset_count__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_sset_count_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ethtool_ops_get_sset_count__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_sset_count_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ethtool_ops_get_sset_count__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_sset_count_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ethtool_ops_get_sset_count__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_sset_count_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__net_device_ops_ndo_open__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_open_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_open__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_open_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__net_device_ops_ndo_open__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_open_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_open__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_open_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__net_device_ops_ndo_stop__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_stop_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_stop__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_stop_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__net_device_ops_ndo_stop__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_stop_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_stop__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_stop_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__net_device_ops_ndo_start_xmit__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_start_xmit__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__net_device_ops_ndo_start_xmit__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_start_xmit__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__net_device_ops_ndo_start_xmit__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_start_xmit__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__net_device_ops_ndo_start_xmit__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_start_xmit__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__net_device_ops_ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__net_device_ops_ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__net_device_ops_ndo_set_rx_mode__netdev_hw_addr_list__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* call_ctx,
	struct netdev_hw_addr_list const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__netdev_hw_addr_list__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* call_ctx,
	struct netdev_hw_addr_list* ptr);

void callee_marshal_kernel__net_device_ops_ndo_set_rx_mode__netdev_hw_addr_list__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* call_ctx,
	struct netdev_hw_addr_list const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__netdev_hw_addr_list__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* call_ctx,
	struct netdev_hw_addr_list* ptr);

void caller_marshal_kernel__net_device_ops_ndo_set_rx_mode__list_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* call_ctx,
	struct list_head const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__list_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* call_ctx,
	struct list_head* ptr);

void callee_marshal_kernel__net_device_ops_ndo_set_rx_mode__list_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* call_ctx,
	struct list_head const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__list_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* call_ctx,
	struct list_head* ptr);

void caller_marshal_kernel__net_device_ops_ndo_set_mac_address__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_set_mac_address__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__net_device_ops_ndo_set_mac_address__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_set_mac_address__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__net_device_ops_ndo_set_mac_address__sockaddr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* call_ctx,
	struct sockaddr const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_set_mac_address__sockaddr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* call_ctx,
	struct sockaddr* ptr);

void callee_marshal_kernel__net_device_ops_ndo_set_mac_address__sockaddr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* call_ctx,
	struct sockaddr const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_set_mac_address__sockaddr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* call_ctx,
	struct sockaddr* ptr);

void caller_marshal_kernel__net_device_ops_ndo_do_ioctl__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_do_ioctl__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__net_device_ops_ndo_do_ioctl__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_do_ioctl__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__net_device_ops_ndo_do_ioctl__ifr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* call_ctx,
	struct ifreq const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_do_ioctl__ifr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* call_ctx,
	struct ifreq* ptr);

void callee_marshal_kernel__net_device_ops_ndo_do_ioctl__ifr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* call_ctx,
	struct ifreq const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_do_ioctl__ifr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* call_ctx,
	struct ifreq* ptr);

void caller_marshal_kernel__net_device_ops_ndo_change_mtu__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_change_mtu_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_change_mtu__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_change_mtu_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__net_device_ops_ndo_change_mtu__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_change_mtu_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_change_mtu__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_change_mtu_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__net_device_ops_ndo_tx_timeout__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_tx_timeout_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_tx_timeout__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_tx_timeout_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__net_device_ops_ndo_tx_timeout__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_tx_timeout_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_tx_timeout__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_tx_timeout_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__net_device_ops_ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64 const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64* ptr);

void callee_marshal_kernel__net_device_ops_ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64 const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64* ptr);

void caller_marshal_kernel__net_device_ops_ndo_get_stats64__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_get_stats64__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__net_device_ops_ndo_get_stats64__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_get_stats64__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__net_device_ops_ndo_get_stats64__net_stats__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64 const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_get_stats64__net_stats__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64* ptr);

void callee_marshal_kernel__net_device_ops_ndo_get_stats64__net_stats__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64 const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_get_stats64__net_stats__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64* ptr);

void caller_marshal_kernel__net_device_ops_ndo_fix_features__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_fix_features_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_fix_features__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_fix_features_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__net_device_ops_ndo_fix_features__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_fix_features_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_fix_features__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_fix_features_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_bus_write_config_word_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_bus_write_config_word_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_bus_write_config_word_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_bus_write_config_word_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_bus_read_config_word_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_bus_read_config_word_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_bus_read_config_word_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_bus_read_config_word_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_read_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_read_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_read_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_read_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_write_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_write_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_write_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_write_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mdio_if_info const* ptr);

void callee_unmarshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mdio_if_info* ptr);

void callee_marshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mdio_if_info const* ptr);

void caller_unmarshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mdio_if_info* ptr);

void caller_marshal_kernel__mdio_mii_ioctl__mii_data__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mii_ioctl_data const* ptr);

void callee_unmarshal_kernel__mdio_mii_ioctl__mii_data__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mii_ioctl_data* ptr);

void callee_marshal_kernel__mdio_mii_ioctl__mii_data__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mii_ioctl_data const* ptr);

void caller_unmarshal_kernel__mdio_mii_ioctl__mii_data__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mii_ioctl_data* ptr);

void caller_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void callee_unmarshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void callee_marshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void caller_unmarshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void caller_marshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void callee_unmarshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct* ptr);

void callee_marshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void caller_unmarshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct* ptr);

void caller_marshal_kernel__queue_work_on__atomic64_t__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct atomic64_t const* ptr);

void callee_unmarshal_kernel__queue_work_on__atomic64_t__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct atomic64_t* ptr);

void callee_marshal_kernel__queue_work_on__atomic64_t__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct atomic64_t const* ptr);

void caller_unmarshal_kernel__queue_work_on__atomic64_t__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct atomic64_t* ptr);

void caller_marshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct consume_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct consume_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct consume_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct consume_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__napi_complete_done__n__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_complete_done_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__napi_complete_done__n__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_complete_done_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__napi_complete_done__n__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_complete_done_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__napi_complete_done__n__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_complete_done_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__alloc_etherdev_mqs__net_device__tx__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* call_ctx,
	struct netdev_queue const* ptr);

void callee_unmarshal_kernel__alloc_etherdev_mqs__net_device__tx__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* call_ctx,
	struct netdev_queue* ptr);

void callee_marshal_kernel__alloc_etherdev_mqs__net_device__tx__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* call_ctx,
	struct netdev_queue const* ptr);

void caller_unmarshal_kernel__alloc_etherdev_mqs__net_device__tx__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* call_ctx,
	struct netdev_queue* ptr);

void caller_marshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct free_netdev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct free_netdev_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct free_netdev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct free_netdev_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_napi_del_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_napi_del_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_napi_del_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_napi_del_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__net_device_ops_ndo_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_validate_addr_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__net_device_ops_ndo_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_validate_addr_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__net_device_ops_ndo_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_validate_addr_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__net_device_ops_ndo_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_validate_addr_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__napi_disable__n__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_disable_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__napi_disable__n__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_disable_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__napi_disable__n__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_disable_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__napi_disable__n__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_disable_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__poll__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct poll_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__poll__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct poll_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__poll__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct poll_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__poll__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct poll_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel____napi_schedule__n__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __napi_schedule_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel____napi_schedule__n__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __napi_schedule_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel____napi_schedule__n__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __napi_schedule_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel____napi_schedule__n__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __napi_schedule_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __dev_kfree_skb_any_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __dev_kfree_skb_any_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __dev_kfree_skb_any_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __dev_kfree_skb_any_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_device_attach_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_device_attach_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_device_attach_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_device_attach_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_tx_wake_queue_call_ctx const* call_ctx,
	struct netdev_queue const* ptr);

void callee_unmarshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_tx_wake_queue_call_ctx const* call_ctx,
	struct netdev_queue* ptr);

void callee_marshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_tx_wake_queue_call_ctx const* call_ctx,
	struct netdev_queue const* ptr);

void caller_unmarshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_tx_wake_queue_call_ctx const* call_ctx,
	struct netdev_queue* ptr);

void caller_marshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_device_detach_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_device_detach_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_device_detach_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_device_detach_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_schedule_queue__txq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_schedule_queue_call_ctx const* call_ctx,
	struct netdev_queue const* ptr);

void callee_unmarshal_kernel__netif_schedule_queue__txq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_schedule_queue_call_ctx const* call_ctx,
	struct netdev_queue* ptr);

void callee_marshal_kernel__netif_schedule_queue__txq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_schedule_queue_call_ctx const* call_ctx,
	struct netdev_queue const* ptr);

void caller_unmarshal_kernel__netif_schedule_queue__txq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_schedule_queue_call_ctx const* call_ctx,
	struct netdev_queue* ptr);

void caller_marshal_kernel__unregister_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct unregister_netdev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__unregister_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct unregister_netdev_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__unregister_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct unregister_netdev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__unregister_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct unregister_netdev_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct register_netdev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct register_netdev_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct register_netdev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct register_netdev_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel____global_netdev_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops const* ptr);

void callee_unmarshal_kernel____global_netdev_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops* ptr);

void callee_marshal_kernel____global_netdev_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops const* ptr);

void caller_unmarshal_kernel____global_netdev_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops* ptr);

void caller_marshal_kernel____global_ethtool_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops const* ptr);

void callee_unmarshal_kernel____global_ethtool_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops* ptr);

void callee_marshal_kernel____global_ethtool_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops const* ptr);

void caller_unmarshal_kernel____global_ethtool_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops* ptr);

void caller_marshal_kernel__netdev_update_features__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netdev_update_features_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netdev_update_features__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netdev_update_features_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netdev_update_features__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netdev_update_features_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netdev_update_features__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netdev_update_features_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_carrier_on_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_carrier_on_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_carrier_on_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_carrier_on_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_carrier_off_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_carrier_off_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_carrier_off_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_carrier_off_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__napi_gro_receive__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__napi_gro_receive__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__napi_gro_receive__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__napi_gro_receive__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_put_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__skb_put__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_put_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_put_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__skb_put__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_put_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel____netdev_alloc_skb__ret_sk_buff__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel____netdev_alloc_skb__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel____netdev_alloc_skb__ret_sk_buff__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel____netdev_alloc_skb__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel____netdev_alloc_skb__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel____netdev_alloc_skb__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel____netdev_alloc_skb__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel____netdev_alloc_skb__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_select_bars_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_select_bars_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_select_bars_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_select_bars_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pcie_set_readrq__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pcie_set_readrq_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pcie_set_readrq__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pcie_set_readrq_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pcie_set_readrq__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pcie_set_readrq_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pcie_set_readrq__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pcie_set_readrq_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pcie_get_readrq__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pcie_get_readrq_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pcie_get_readrq__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pcie_get_readrq_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pcie_get_readrq__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pcie_get_readrq_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pcie_get_readrq__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pcie_get_readrq_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_request_selected_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_request_selected_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_request_selected_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_request_selected_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_release_selected_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_release_selected_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_release_selected_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_release_selected_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_ioremap_bar__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_ioremap_bar_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_ioremap_bar__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_ioremap_bar_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_ioremap_bar__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_ioremap_bar_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_ioremap_bar__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_ioremap_bar_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct eth_validate_addr_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct eth_validate_addr_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct eth_validate_addr_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct eth_validate_addr_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_op_get_link_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_op_get_link_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_op_get_link_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_op_get_link_call_ctx const* call_ctx,
	struct net_device* ptr);


#endif
