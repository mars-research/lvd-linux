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

#define glue_unpack_bind_or_new_shadow(pos, msg, ext, type, size, flags) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack or bind new shadow for type %s | size %llu\n", __func__, __LINE__, __stringify(type), (uint64_t) size); \
	(type)glue_unpack_bind_or_new_shadow_impl(glue_unpack(pos, msg, ext, void*), size, flags); })

#ifndef LCD_ISOLATE
#define glue_unpack_rpc_ptr(pos, msg, ext, name) \
	(fptr_##name)glue_unpack_rpc_ptr_impl(glue_unpack(pos, msg, ext, void*), LCD_DUP_TRAMPOLINE(trmp_##name), LCD_TRAMPOLINE_SIZE(trmp_##name))

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

static inline void* glue_unpack_bind_or_new_shadow_impl(const void* ptr, size_t size, gfp_t flags)
{
	void* shadow = 0;
	if (!ptr)
		return NULL;

	shadow = glue_user_map_to_shadow(ptr, false);
	if (!shadow) {
		shadow = glue_user_alloc(size, flags);
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
	RPC_ID_probe,
	RPC_ID_remove,
	RPC_ID_shutdown,
	RPC_ID_ndo_set_rx_mode,
	RPC_ID_ndo_stop,
	RPC_ID_ndo_open,
	RPC_ID_ndo_start_xmit,
	RPC_ID_ndo_features_check,
	RPC_ID_ndo_set_mac_address,
	RPC_ID_ndo_do_ioctl,
	RPC_ID_ndo_change_mtu,
	RPC_ID_ndo_tx_timeout,
	RPC_ID_ndo_get_stats64,
	RPC_ID_ndo_setup_tc,
	RPC_ID_ndo_fix_features,
	RPC_ID_ndo_set_features,
	RPC_ID_ndo_set_tx_maxrate,
	RPC_ID_del_timer_sync,
	RPC_ID_timer_func,
	RPC_ID_lvd_setup_timer,
	RPC_ID_pci_disable_msi,
	RPC_ID_pci_disable_msix,
	RPC_ID_pci_disable_device,
	RPC_ID_pci_bus_read_config_word,
	RPC_ID_pci_bus_write_config_word,
	RPC_ID_pci_dev_put,
	RPC_ID_queue_work_on,
	RPC_ID_pci_select_bars,
	RPC_ID_pcie_get_minimum_link,
	RPC_ID_pcie_capability_read_word,
	RPC_ID_pci_restore_state,
	RPC_ID_pci_set_power_state,
	RPC_ID_pci_save_state,
	RPC_ID_pci_set_master,
	RPC_ID_pci_request_selected_regions,
	RPC_ID_pci_release_selected_regions,
	RPC_ID_mod_timer,
	RPC_ID_pci_wake_from_d3,
	RPC_ID_pci_enable_device_mem,
	RPC_ID_ioremap_nocache,
	RPC_ID_msleep_interruptible,
	RPC_ID_netdev_rss_key_fill,
	RPC_ID_rtnl_is_locked,
	RPC_ID_skb_copy_bits,
	RPC_ID____pskb_trim,
	RPC_ID_pskb_expand_head,
	RPC_ID_consume_skb,
	RPC_ID___pskb_pull_tail,
	RPC_ID_skb_put,
	RPC_ID_skb_tstamp_tx,
	RPC_ID_skb_pad,
	RPC_ID_napi_consume_skb,
	RPC_ID___dev_kfree_skb_any,
	RPC_ID___napi_alloc_skb,
	RPC_ID_napi_gro_flush,
	RPC_ID_napi_complete_done,
	RPC_ID_rtnl_unlock,
	RPC_ID_rtnl_lock,
	RPC_ID_call_netdevice_notifiers,
	RPC_ID_dev_get_stats,
	RPC_ID_unregister_netdev,
	RPC_ID_free_netdev,
	RPC_ID_netif_napi_del,
	RPC_ID_napi_hash_del,
	RPC_ID_register_netdev,
	RPC_ID_netif_tx_stop_all_queues,
	RPC_ID_netif_carrier_on,
	RPC_ID_netif_carrier_off,
	RPC_ID_netdev_all_upper_get_next_dev_rcu,
	RPC_ID_napi_disable,
	RPC_ID_poll,
	RPC_ID_netif_napi_add,
	RPC_ID___napi_schedule_irqoff,
	RPC_ID_napi_gro_receive,
	RPC_ID_netif_receive_skb,
	RPC_ID_skb_checksum_help,
	RPC_ID_netif_device_attach,
	RPC_ID_netif_tx_wake_queue,
	RPC_ID_netif_device_detach,
	RPC_ID_netif_wake_subqueue,
	RPC_ID_netif_set_real_num_rx_queues,
	RPC_ID_netif_set_real_num_tx_queues,
	RPC_ID_netif_set_xps_queue,
	RPC_ID_thread_fn,
	RPC_ID_handler,
	RPC_ID_request_threaded_irq,
	RPC_ID_free_irq,
	RPC_ID_synchronize_irq,
	RPC_ID_irq_set_affinity_hint,
	RPC_ID_pci_enable_msix_range,
	RPC_ID_pci_enable_msi_range,
	RPC_ID_dev_addr_del,
	RPC_ID_dev_addr_add,
	RPC_ID_sync,
	RPC_ID_unsync,
	RPC_ID___hw_addr_unsync_dev,
	RPC_ID___hw_addr_sync_dev,
	RPC_ID_eth_platform_get_mac_address,
	RPC_ID_alloc_etherdev_mqs,
	RPC_ID_ndo_validate_addr,
	RPC_ID_dev_trans_start,
	RPC_ID_mdio_read,
	RPC_ID_mdio_write,
	RPC_ID_mdio_mii_ioctl,
	RPC_ID_mdio45_probe,
	RPC_ID_pci_unregister_driver,
	RPC_ID___pci_register_driver,
	RPC_ID_pci_enable_pcie_error_reporting,
	RPC_ID_pci_disable_pcie_error_reporting,
	RPC_ID_lvd_napi_enable,
	RPC_ID_lvd_netif_trans_update,
	RPC_ID_lvd_netif_tx_disable,
	RPC_ID_lvd_napi_schedule_irqoff,
	RPC_ID_lvd_netif_tx_wake_all_queues,
	RPC_ID_get_loops_per_jiffy,
	RPC_ID_get_jiffies,
	RPC_ID_eth_validate_addr,
	RPC_ID_ethtool_op_get_link,
	RPC_ID_ethtool_op_get_ts_info,
	RPC_ID_ipv6_find_hdr,
};

int try_dispatch(enum RPC_ID id, struct fipc_message* __msg, struct ext_registers* __ext);

typedef int (*fptr_probe)(struct pci_dev* pdev, struct pci_device_id* ent);
typedef int (*fptr_impl_probe)(fptr_probe target, struct pci_dev* pdev, struct pci_device_id* ent);

LCD_TRAMPOLINE_DATA(trmp_probe)
int LCD_TRAMPOLINE_LINKAGE(trmp_probe) trmp_probe(struct pci_dev* pdev, struct pci_device_id* ent);

typedef void (*fptr_remove)(struct pci_dev* pdev);
typedef void (*fptr_impl_remove)(fptr_remove target, struct pci_dev* pdev);

LCD_TRAMPOLINE_DATA(trmp_remove)
void LCD_TRAMPOLINE_LINKAGE(trmp_remove) trmp_remove(struct pci_dev* pdev);

typedef void (*fptr_shutdown)(struct pci_dev* pdev);
typedef void (*fptr_impl_shutdown)(fptr_shutdown target, struct pci_dev* pdev);

LCD_TRAMPOLINE_DATA(trmp_shutdown)
void LCD_TRAMPOLINE_LINKAGE(trmp_shutdown) trmp_shutdown(struct pci_dev* pdev);

typedef void (*fptr_ndo_set_rx_mode)(struct net_device* netdev);
typedef void (*fptr_impl_ndo_set_rx_mode)(fptr_ndo_set_rx_mode target, struct net_device* netdev);

LCD_TRAMPOLINE_DATA(trmp_ndo_set_rx_mode)
void LCD_TRAMPOLINE_LINKAGE(trmp_ndo_set_rx_mode) trmp_ndo_set_rx_mode(struct net_device* netdev);

typedef int (*fptr_ndo_stop)(struct net_device* netdev);
typedef int (*fptr_impl_ndo_stop)(fptr_ndo_stop target, struct net_device* netdev);

LCD_TRAMPOLINE_DATA(trmp_ndo_stop)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_stop) trmp_ndo_stop(struct net_device* netdev);

typedef int (*fptr_ndo_open)(struct net_device* netdev);
typedef int (*fptr_impl_ndo_open)(fptr_ndo_open target, struct net_device* netdev);

LCD_TRAMPOLINE_DATA(trmp_ndo_open)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_open) trmp_ndo_open(struct net_device* netdev);

typedef int (*fptr_ndo_start_xmit)(struct sk_buff* skb, struct net_device* netdev);
typedef int (*fptr_impl_ndo_start_xmit)(fptr_ndo_start_xmit target, struct sk_buff* skb, struct net_device* netdev);

LCD_TRAMPOLINE_DATA(trmp_ndo_start_xmit)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_start_xmit) trmp_ndo_start_xmit(struct sk_buff* skb, struct net_device* netdev);

typedef unsigned long long (*fptr_ndo_features_check)(struct sk_buff* skb, struct net_device* dev, unsigned long long features);
typedef unsigned long long (*fptr_impl_ndo_features_check)(fptr_ndo_features_check target, struct sk_buff* skb, struct net_device* dev, unsigned long long features);

LCD_TRAMPOLINE_DATA(trmp_ndo_features_check)
unsigned long long LCD_TRAMPOLINE_LINKAGE(trmp_ndo_features_check) trmp_ndo_features_check(struct sk_buff* skb, struct net_device* dev, unsigned long long features);

typedef int (*fptr_ndo_set_mac_address)(struct net_device* netdev, void* p);
typedef int (*fptr_impl_ndo_set_mac_address)(fptr_ndo_set_mac_address target, struct net_device* netdev, void* p);

LCD_TRAMPOLINE_DATA(trmp_ndo_set_mac_address)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_set_mac_address) trmp_ndo_set_mac_address(struct net_device* netdev, void* p);

typedef int (*fptr_ndo_do_ioctl)(struct net_device* netdev, struct ifreq* req, int cmd);
typedef int (*fptr_impl_ndo_do_ioctl)(fptr_ndo_do_ioctl target, struct net_device* netdev, struct ifreq* req, int cmd);

LCD_TRAMPOLINE_DATA(trmp_ndo_do_ioctl)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_do_ioctl) trmp_ndo_do_ioctl(struct net_device* netdev, struct ifreq* req, int cmd);

typedef int (*fptr_ndo_change_mtu)(struct net_device* netdev, int new_mtu);
typedef int (*fptr_impl_ndo_change_mtu)(fptr_ndo_change_mtu target, struct net_device* netdev, int new_mtu);

LCD_TRAMPOLINE_DATA(trmp_ndo_change_mtu)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_change_mtu) trmp_ndo_change_mtu(struct net_device* netdev, int new_mtu);

typedef void (*fptr_ndo_tx_timeout)(struct net_device* netdev);
typedef void (*fptr_impl_ndo_tx_timeout)(fptr_ndo_tx_timeout target, struct net_device* netdev);

LCD_TRAMPOLINE_DATA(trmp_ndo_tx_timeout)
void LCD_TRAMPOLINE_LINKAGE(trmp_ndo_tx_timeout) trmp_ndo_tx_timeout(struct net_device* netdev);

typedef struct rtnl_link_stats64* (*fptr_ndo_get_stats64)(struct net_device* netdev, struct rtnl_link_stats64* stats);
typedef struct rtnl_link_stats64* (*fptr_impl_ndo_get_stats64)(fptr_ndo_get_stats64 target, struct net_device* netdev, struct rtnl_link_stats64* stats);

LCD_TRAMPOLINE_DATA(trmp_ndo_get_stats64)
struct rtnl_link_stats64* LCD_TRAMPOLINE_LINKAGE(trmp_ndo_get_stats64) trmp_ndo_get_stats64(struct net_device* netdev, struct rtnl_link_stats64* stats);

typedef int (*fptr_ndo_setup_tc)(struct net_device* dev, unsigned int handle, unsigned short proto, struct tc_to_netdev* tc);
typedef int (*fptr_impl_ndo_setup_tc)(fptr_ndo_setup_tc target, struct net_device* dev, unsigned int handle, unsigned short proto, struct tc_to_netdev* tc);

LCD_TRAMPOLINE_DATA(trmp_ndo_setup_tc)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_setup_tc) trmp_ndo_setup_tc(struct net_device* dev, unsigned int handle, unsigned short proto, struct tc_to_netdev* tc);

typedef unsigned long long (*fptr_ndo_fix_features)(struct net_device* netdev, unsigned long long features);
typedef unsigned long long (*fptr_impl_ndo_fix_features)(fptr_ndo_fix_features target, struct net_device* netdev, unsigned long long features);

LCD_TRAMPOLINE_DATA(trmp_ndo_fix_features)
unsigned long long LCD_TRAMPOLINE_LINKAGE(trmp_ndo_fix_features) trmp_ndo_fix_features(struct net_device* netdev, unsigned long long features);

typedef int (*fptr_ndo_set_features)(struct net_device* netdev, unsigned long long features);
typedef int (*fptr_impl_ndo_set_features)(fptr_ndo_set_features target, struct net_device* netdev, unsigned long long features);

LCD_TRAMPOLINE_DATA(trmp_ndo_set_features)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_set_features) trmp_ndo_set_features(struct net_device* netdev, unsigned long long features);

typedef int (*fptr_ndo_set_tx_maxrate)(struct net_device* netdev, int queue_index, unsigned int maxrate);
typedef int (*fptr_impl_ndo_set_tx_maxrate)(fptr_ndo_set_tx_maxrate target, struct net_device* netdev, int queue_index, unsigned int maxrate);

LCD_TRAMPOLINE_DATA(trmp_ndo_set_tx_maxrate)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_set_tx_maxrate) trmp_ndo_set_tx_maxrate(struct net_device* netdev, int queue_index, unsigned int maxrate);

typedef void (*fptr_timer_func)(unsigned long data);
typedef void (*fptr_impl_timer_func)(fptr_timer_func target, unsigned long data);

LCD_TRAMPOLINE_DATA(trmp_timer_func)
void LCD_TRAMPOLINE_LINKAGE(trmp_timer_func) trmp_timer_func(unsigned long data);

typedef int (*fptr_poll)(struct napi_struct* napi, int budget);
typedef int (*fptr_impl_poll)(fptr_poll target, struct napi_struct* napi, int budget);

LCD_TRAMPOLINE_DATA(trmp_poll)
int LCD_TRAMPOLINE_LINKAGE(trmp_poll) trmp_poll(struct napi_struct* napi, int budget);

typedef unsigned int (*fptr_thread_fn)(int irq, void* id);
typedef unsigned int (*fptr_impl_thread_fn)(fptr_thread_fn target, int irq, void* id);

LCD_TRAMPOLINE_DATA(trmp_thread_fn)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_thread_fn) trmp_thread_fn(int irq, void* id);

typedef unsigned int (*fptr_handler)(int irq, void* id);
typedef unsigned int (*fptr_impl_handler)(fptr_handler target, int irq, void* id);

LCD_TRAMPOLINE_DATA(trmp_handler)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_handler) trmp_handler(int irq, void* id);

typedef int (*fptr_sync)(struct net_device* dev, unsigned char const* addr);
typedef int (*fptr_impl_sync)(fptr_sync target, struct net_device* dev, unsigned char const* addr);

LCD_TRAMPOLINE_DATA(trmp_sync)
int LCD_TRAMPOLINE_LINKAGE(trmp_sync) trmp_sync(struct net_device* dev, unsigned char const* addr);

typedef int (*fptr_unsync)(struct net_device* dev, unsigned char const* addr);
typedef int (*fptr_impl_unsync)(fptr_unsync target, struct net_device* dev, unsigned char const* addr);

LCD_TRAMPOLINE_DATA(trmp_unsync)
int LCD_TRAMPOLINE_LINKAGE(trmp_unsync) trmp_unsync(struct net_device* dev, unsigned char const* addr);

typedef int (*fptr_ndo_validate_addr)(struct net_device* dev);
typedef int (*fptr_impl_ndo_validate_addr)(fptr_ndo_validate_addr target, struct net_device* dev);

LCD_TRAMPOLINE_DATA(trmp_ndo_validate_addr)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_validate_addr) trmp_ndo_validate_addr(struct net_device* dev);

typedef int (*fptr_mdio_read)(struct net_device* dev, int prtad, int devad, unsigned short addr);
typedef int (*fptr_impl_mdio_read)(fptr_mdio_read target, struct net_device* dev, int prtad, int devad, unsigned short addr);

LCD_TRAMPOLINE_DATA(trmp_mdio_read)
int LCD_TRAMPOLINE_LINKAGE(trmp_mdio_read) trmp_mdio_read(struct net_device* dev, int prtad, int devad, unsigned short addr);

typedef int (*fptr_mdio_write)(struct net_device* dev, int prtad, int devad, unsigned short addr, unsigned short val);
typedef int (*fptr_impl_mdio_write)(fptr_mdio_write target, struct net_device* dev, int prtad, int devad, unsigned short addr, unsigned short val);

LCD_TRAMPOLINE_DATA(trmp_mdio_write)
int LCD_TRAMPOLINE_LINKAGE(trmp_mdio_write) trmp_mdio_write(struct net_device* dev, int prtad, int devad, unsigned short addr, unsigned short val);

struct probe_call_ctx {
	struct pci_dev* pdev;
	struct pci_device_id* ent;
};

struct remove_call_ctx {
	struct pci_dev* pdev;
};

struct shutdown_call_ctx {
	struct pci_dev* pdev;
};

struct ndo_set_rx_mode_call_ctx {
	struct net_device* netdev;
};

struct ndo_stop_call_ctx {
	struct net_device* netdev;
};

struct ndo_open_call_ctx {
	struct net_device* netdev;
};

struct ndo_start_xmit_call_ctx {
	struct sk_buff* skb;
	struct net_device* netdev;
};

struct ndo_features_check_call_ctx {
	struct sk_buff* skb;
	struct net_device* dev;
	unsigned long long features;
};

struct ndo_set_mac_address_call_ctx {
	struct net_device* netdev;
	void* p;
};

struct ndo_do_ioctl_call_ctx {
	struct net_device* netdev;
	struct ifreq* req;
	int cmd;
};

struct ndo_change_mtu_call_ctx {
	struct net_device* netdev;
	int new_mtu;
};

struct ndo_tx_timeout_call_ctx {
	struct net_device* netdev;
};

struct ndo_get_stats64_call_ctx {
	struct net_device* netdev;
	struct rtnl_link_stats64* stats;
};

struct ndo_setup_tc_call_ctx {
	struct net_device* dev;
	unsigned int handle;
	unsigned short proto;
	struct tc_to_netdev* tc;
};

struct ndo_fix_features_call_ctx {
	struct net_device* netdev;
	unsigned long long features;
};

struct ndo_set_features_call_ctx {
	struct net_device* netdev;
	unsigned long long features;
};

struct ndo_set_tx_maxrate_call_ctx {
	struct net_device* netdev;
	int queue_index;
	unsigned int maxrate;
};

struct del_timer_sync_call_ctx {
	struct timer_list* timer;
};

struct timer_func_call_ctx {
	unsigned long data;
};

struct lvd_setup_timer_call_ctx {
	struct timer_list* timer;
	fptr_timer_func func;
	unsigned long data;
};

struct pci_disable_msi_call_ctx {
	struct pci_dev* dev;
};

struct pci_disable_msix_call_ctx {
	struct pci_dev* dev;
};

struct pci_disable_device_call_ctx {
	struct pci_dev* dev;
};

struct pci_bus_read_config_word_call_ctx {
	struct pci_bus* bus;
	unsigned int devfn;
	int pos;
	unsigned short* value;
};

struct pci_bus_write_config_word_call_ctx {
	struct pci_bus* bus;
	unsigned int devfn;
	int pos;
	unsigned short value;
};

struct pci_dev_put_call_ctx {
	struct pci_dev* dev;
};

struct queue_work_on_call_ctx {
	int cpu;
	struct workqueue_struct* wq;
	struct work_struct* work;
};

struct pci_select_bars_call_ctx {
	struct pci_dev* dev;
	unsigned long flags;
};

struct pcie_get_minimum_link_call_ctx {
	struct pci_dev* dev;
	unsigned int* speed;
	unsigned int* width;
};

struct pcie_capability_read_word_call_ctx {
	struct pci_dev* dev;
	int pos;
	unsigned short* val;
};

struct pci_restore_state_call_ctx {
	struct pci_dev* dev;
};

struct pci_set_power_state_call_ctx {
	struct pci_dev* dev;
	int state;
};

struct pci_save_state_call_ctx {
	struct pci_dev* dev;
};

struct pci_set_master_call_ctx {
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

struct mod_timer_call_ctx {
	struct timer_list* timer;
	unsigned long expires;
};

struct pci_wake_from_d3_call_ctx {
	struct pci_dev* dev;
	bool enable;
};

struct pci_enable_device_mem_call_ctx {
	struct pci_dev* dev;
};

struct ioremap_nocache_call_ctx {
	unsigned long long phys_addr;
	unsigned long size;
};

struct msleep_interruptible_call_ctx {
	unsigned int msecs;
};

struct netdev_rss_key_fill_call_ctx {
	void* buffer;
	unsigned long len;
};

struct rtnl_is_locked_call_ctx {
};

struct skb_copy_bits_call_ctx {
	struct sk_buff const* skb;
	int offset;
	void* to;
	int len;
};

struct ___pskb_trim_call_ctx {
	struct sk_buff* skb;
	unsigned int len;
};

struct pskb_expand_head_call_ctx {
	struct sk_buff* skb;
	int nhead;
	int ntail;
	unsigned int gfp_mask;
};

struct consume_skb_call_ctx {
	struct sk_buff* skb;
};

struct __pskb_pull_tail_call_ctx {
	struct sk_buff* skb;
	int delta;
};

struct skb_put_call_ctx {
	struct sk_buff* skb;
	unsigned int len;
};

struct skb_tstamp_tx_call_ctx {
	struct sk_buff* orig_skb;
	struct skb_shared_hwtstamps* hwtstamps;
};

struct skb_pad_call_ctx {
	struct sk_buff* skb;
	int pad;
};

struct napi_consume_skb_call_ctx {
	struct sk_buff* skb;
	int budget;
};

struct __dev_kfree_skb_any_call_ctx {
	struct sk_buff* skb;
	unsigned int reason;
};

struct __napi_alloc_skb_call_ctx {
	struct napi_struct* napi;
	unsigned int len;
	unsigned int gfp_mask;
};

struct napi_gro_flush_call_ctx {
	struct napi_struct* napi;
	bool flush_old;
};

struct napi_complete_done_call_ctx {
	struct napi_struct* n;
	int work_done;
};

struct rtnl_unlock_call_ctx {
};

struct rtnl_lock_call_ctx {
};

struct call_netdevice_notifiers_call_ctx {
	unsigned long val;
	struct net_device* dev;
};

struct dev_get_stats_call_ctx {
	struct net_device* dev;
	struct rtnl_link_stats64* storage;
};

struct unregister_netdev_call_ctx {
	struct net_device* dev;
};

struct free_netdev_call_ctx {
	struct net_device* dev;
};

struct netif_napi_del_call_ctx {
	struct napi_struct* napi;
};

struct napi_hash_del_call_ctx {
	struct napi_struct* napi;
};

struct register_netdev_call_ctx {
	struct net_device* dev;
};

struct netif_tx_stop_all_queues_call_ctx {
	struct net_device* dev;
};

struct netif_carrier_on_call_ctx {
	struct net_device* dev;
};

struct netif_carrier_off_call_ctx {
	struct net_device* dev;
};

struct netdev_all_upper_get_next_dev_rcu_call_ctx {
	struct net_device* dev;
	struct list_head** iter;
};

struct napi_disable_call_ctx {
	struct napi_struct* n;
};

struct poll_call_ctx {
	struct napi_struct* napi;
	int budget;
};

struct netif_napi_add_call_ctx {
	struct net_device* dev;
	struct napi_struct* napi;
	fptr_poll poll;
	int weight;
};

struct __napi_schedule_irqoff_call_ctx {
	struct napi_struct* n;
};

struct napi_gro_receive_call_ctx {
	struct napi_struct* napi;
	struct sk_buff* skb;
};

struct netif_receive_skb_call_ctx {
	struct sk_buff* skb;
};

struct skb_checksum_help_call_ctx {
	struct sk_buff* skb;
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

struct netif_wake_subqueue_call_ctx {
	struct net_device* dev;
	unsigned short queue_index;
};

struct netif_set_real_num_rx_queues_call_ctx {
	struct net_device* dev;
	unsigned int rxq;
};

struct netif_set_real_num_tx_queues_call_ctx {
	struct net_device* dev;
	unsigned int txq;
};

struct netif_set_xps_queue_call_ctx {
	struct net_device* dev;
	struct cpumask const* mask;
	unsigned short index;
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

struct irq_set_affinity_hint_call_ctx {
	unsigned int irq;
	struct cpumask const* m;
};

struct pci_enable_msix_range_call_ctx {
	struct pci_dev* dev;
	struct msix_entry* entries;
	int minvec;
	int maxvec;
};

struct pci_enable_msi_range_call_ctx {
	struct pci_dev* dev;
	int minvec;
	int maxvec;
};

struct dev_addr_del_call_ctx {
	struct net_device* dev;
	unsigned char const* addr;
	unsigned char addr_type;
};

struct dev_addr_add_call_ctx {
	struct net_device* dev;
	unsigned char const* addr;
	unsigned char addr_type;
};

struct sync_call_ctx {
	struct net_device* dev;
	unsigned char const* addr;
};

struct unsync_call_ctx {
	struct net_device* dev;
	unsigned char const* addr;
};

struct __hw_addr_unsync_dev_call_ctx {
	struct netdev_hw_addr_list* list;
	struct net_device* dev;
	fptr_unsync unsync;
};

struct __hw_addr_sync_dev_call_ctx {
	struct netdev_hw_addr_list* list;
	struct net_device* dev;
	fptr_sync sync;
	fptr_unsync unsync;
};

struct eth_platform_get_mac_address_call_ctx {
	struct device* dev;
	unsigned char* mac_addr;
};

struct alloc_etherdev_mqs_call_ctx {
	int sizeof_priv;
	unsigned int txqs;
	unsigned int rxqs;
};

struct ndo_validate_addr_call_ctx {
	struct net_device* dev;
};

struct dev_trans_start_call_ctx {
	struct net_device* dev;
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

struct mdio45_probe_call_ctx {
	struct mdio_if_info* mdio;
	int prtad;
};

struct pci_unregister_driver_call_ctx {
	struct pci_driver* drv;
};

struct __pci_register_driver_call_ctx {
	struct pci_driver* drv;
	struct module* owner;
	char const* mod_name;
};

struct pci_enable_pcie_error_reporting_call_ctx {
	struct pci_dev* dev;
};

struct pci_disable_pcie_error_reporting_call_ctx {
	struct pci_dev* dev;
};

struct lvd_napi_enable_call_ctx {
	struct napi_struct* napi;
};

struct lvd_netif_trans_update_call_ctx {
	struct net_device* dev;
};

struct lvd_netif_tx_disable_call_ctx {
	struct net_device* dev;
};

struct lvd_napi_schedule_irqoff_call_ctx {
	struct napi_struct* napi;
};

struct lvd_netif_tx_wake_all_queues_call_ctx {
	struct net_device* dev;
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

struct ethtool_op_get_ts_info_call_ctx {
	struct net_device* dev;
	struct ethtool_ts_info* info;
};

struct ipv6_find_hdr_call_ctx {
	struct sk_buff const* skb;
	unsigned int* offset;
	int target;
	unsigned short* fragoff;
	int* flags;
};

void caller_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__probe__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__probe__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__probe__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__probe__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__probe__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__probe__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__probe__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__probe__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__probe__kobject__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct kobject const* ptr);

void callee_unmarshal_kernel__probe__kobject__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct kobject* ptr);

void callee_marshal_kernel__probe__kobject__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct kobject const* ptr);

void caller_unmarshal_kernel__probe__kobject__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct kobject* ptr);

void caller_marshal_kernel__probe__resource__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct resource const* ptr);

void callee_unmarshal_kernel__probe__resource__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct resource* ptr);

void callee_marshal_kernel__probe__resource__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct resource const* ptr);

void caller_unmarshal_kernel__probe__resource__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct resource* ptr);

void caller_marshal_kernel__probe__ent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void callee_unmarshal_kernel__probe__ent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void callee_marshal_kernel__probe__ent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void caller_unmarshal_kernel__probe__ent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void caller_marshal_kernel__remove__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct remove_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct remove_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__remove__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct remove_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct remove_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct shutdown_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct shutdown_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct shutdown_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct shutdown_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_set_rx_mode__netdev_hw_addr_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* call_ctx,
	struct netdev_hw_addr_list const* ptr);

void callee_unmarshal_kernel__ndo_set_rx_mode__netdev_hw_addr_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* call_ctx,
	struct netdev_hw_addr_list* ptr);

void callee_marshal_kernel__ndo_set_rx_mode__netdev_hw_addr_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* call_ctx,
	struct netdev_hw_addr_list const* ptr);

void caller_unmarshal_kernel__ndo_set_rx_mode__netdev_hw_addr_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* call_ctx,
	struct netdev_hw_addr_list* ptr);

void caller_marshal_kernel__ndo_set_rx_mode__list_head__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* call_ctx,
	struct list_head const* ptr);

void callee_unmarshal_kernel__ndo_set_rx_mode__list_head__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* call_ctx,
	struct list_head* ptr);

void callee_marshal_kernel__ndo_set_rx_mode__list_head__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* call_ctx,
	struct list_head const* ptr);

void caller_unmarshal_kernel__ndo_set_rx_mode__list_head__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* call_ctx,
	struct list_head* ptr);

void caller_marshal_kernel__ndo_stop__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_stop_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_stop__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_stop_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_stop__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_stop_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_stop__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_stop_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_open__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_open_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_open__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_open_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_open__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_open_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_open__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_open_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_start_xmit__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__ndo_start_xmit__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__ndo_start_xmit__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__ndo_start_xmit__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__ndo_start_xmit__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_start_xmit__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_start_xmit__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_start_xmit__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_features_check__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__ndo_features_check__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__ndo_features_check__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__ndo_features_check__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__ndo_features_check__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_features_check__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_features_check__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_features_check__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_set_mac_address__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_mac_address_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_set_mac_address__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_mac_address_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_set_mac_address__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_mac_address_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_set_mac_address__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_mac_address_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_do_ioctl__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_do_ioctl__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_do_ioctl__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_do_ioctl__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_do_ioctl__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* call_ctx,
	struct ifreq const* ptr);

void callee_unmarshal_kernel__ndo_do_ioctl__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* call_ctx,
	struct ifreq* ptr);

void callee_marshal_kernel__ndo_do_ioctl__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* call_ctx,
	struct ifreq const* ptr);

void caller_unmarshal_kernel__ndo_do_ioctl__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* call_ctx,
	struct ifreq* ptr);

void caller_marshal_kernel__ndo_change_mtu__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_change_mtu_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_change_mtu__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_change_mtu_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_change_mtu__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_change_mtu_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_change_mtu__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_change_mtu_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_tx_timeout__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_tx_timeout_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_tx_timeout__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_tx_timeout_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_tx_timeout__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_tx_timeout_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_tx_timeout__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_tx_timeout_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64 const* ptr);

void callee_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64* ptr);

void callee_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64 const* ptr);

void caller_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64* ptr);

void caller_marshal_kernel__ndo_get_stats64__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_get_stats64__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_get_stats64__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_get_stats64__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_get_stats64__stats__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64 const* ptr);

void callee_unmarshal_kernel__ndo_get_stats64__stats__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64* ptr);

void callee_marshal_kernel__ndo_get_stats64__stats__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64 const* ptr);

void caller_unmarshal_kernel__ndo_get_stats64__stats__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* call_ctx,
	struct rtnl_link_stats64* ptr);

void caller_marshal_kernel__ndo_setup_tc__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_setup_tc__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_setup_tc__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_setup_tc__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_setup_tc__netdev_tc_txq__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* call_ctx,
	struct netdev_tc_txq const* ptr);

void callee_unmarshal_kernel__ndo_setup_tc__netdev_tc_txq__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* call_ctx,
	struct netdev_tc_txq* ptr);

void callee_marshal_kernel__ndo_setup_tc__netdev_tc_txq__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* call_ctx,
	struct netdev_tc_txq const* ptr);

void caller_unmarshal_kernel__ndo_setup_tc__netdev_tc_txq__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* call_ctx,
	struct netdev_tc_txq* ptr);

void caller_marshal_kernel__ndo_setup_tc__tc__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* call_ctx,
	struct tc_to_netdev const* ptr);

void callee_unmarshal_kernel__ndo_setup_tc__tc__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* call_ctx,
	struct tc_to_netdev* ptr);

void callee_marshal_kernel__ndo_setup_tc__tc__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* call_ctx,
	struct tc_to_netdev const* ptr);

void caller_unmarshal_kernel__ndo_setup_tc__tc__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* call_ctx,
	struct tc_to_netdev* ptr);

void caller_marshal_kernel__ndo_fix_features__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_fix_features_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_fix_features__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_fix_features_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_fix_features__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_fix_features_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_fix_features__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_fix_features_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_set_features__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_features_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_set_features__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_features_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_set_features__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_features_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_set_features__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_features_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_set_tx_maxrate__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_tx_maxrate_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_set_tx_maxrate__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_tx_maxrate_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_set_tx_maxrate__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_tx_maxrate_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_set_tx_maxrate__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_tx_maxrate_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void callee_unmarshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* call_ctx,
	struct timer_list* ptr);

void callee_marshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void caller_unmarshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* call_ctx,
	struct timer_list* ptr);

void caller_marshal_kernel__lvd_setup_timer__timer__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_setup_timer_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void callee_unmarshal_kernel__lvd_setup_timer__timer__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_setup_timer_call_ctx const* call_ctx,
	struct timer_list* ptr);

void callee_marshal_kernel__lvd_setup_timer__timer__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_setup_timer_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void caller_unmarshal_kernel__lvd_setup_timer__timer__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_setup_timer_call_ctx const* call_ctx,
	struct timer_list* ptr);

void caller_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_bus_read_config_word_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_bus_read_config_word_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_bus_read_config_word_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_bus_read_config_word_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_bus_write_config_word_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_bus_write_config_word_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_bus_write_config_word_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_bus_write_config_word_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_dev_put_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_dev_put_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_dev_put_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_dev_put_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void callee_unmarshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void callee_marshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct const* ptr);

void caller_unmarshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct workqueue_struct* ptr);

void caller_marshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void callee_unmarshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct* ptr);

void callee_marshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct const* ptr);

void caller_unmarshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct work_struct* ptr);

void caller_marshal_kernel__queue_work_on__list_head__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct list_head const* ptr);

void callee_unmarshal_kernel__queue_work_on__list_head__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct list_head* ptr);

void callee_marshal_kernel__queue_work_on__list_head__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct list_head const* ptr);

void caller_unmarshal_kernel__queue_work_on__list_head__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* call_ctx,
	struct list_head* ptr);

void caller_marshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pcie_get_minimum_link__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pcie_get_minimum_link__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pcie_get_minimum_link__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pcie_get_minimum_link__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pcie_get_minimum_link__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pcie_get_minimum_link__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pcie_get_minimum_link__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pcie_get_minimum_link__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__pcie_capability_read_word__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pcie_capability_read_word__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pcie_capability_read_word__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pcie_capability_read_word__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pcie_capability_read_word__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void callee_unmarshal_kernel__pcie_capability_read_word__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void callee_marshal_kernel__pcie_capability_read_word__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* call_ctx,
	struct pci_bus const* ptr);

void caller_unmarshal_kernel__pcie_capability_read_word__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* call_ctx,
	struct pci_bus* ptr);

void caller_marshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_set_power_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_power_state_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_set_power_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_power_state_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_set_power_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_power_state_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_set_power_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_power_state_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__mod_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void callee_unmarshal_kernel__mod_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* call_ctx,
	struct timer_list* ptr);

void callee_marshal_kernel__mod_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* call_ctx,
	struct timer_list const* ptr);

void caller_unmarshal_kernel__mod_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* call_ctx,
	struct timer_list* ptr);

void caller_marshal_kernel__pci_wake_from_d3__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_wake_from_d3_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_wake_from_d3__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_wake_from_d3_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_wake_from_d3__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_wake_from_d3_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_wake_from_d3__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_wake_from_d3_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__skb_copy_bits__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_copy_bits_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__skb_copy_bits__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_copy_bits_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__skb_copy_bits__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_copy_bits_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__skb_copy_bits__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_copy_bits_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel_____pskb_trim__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ___pskb_trim_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel_____pskb_trim__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ___pskb_trim_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel_____pskb_trim__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ___pskb_trim_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel_____pskb_trim__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ___pskb_trim_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__pskb_expand_head__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pskb_expand_head_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__pskb_expand_head__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pskb_expand_head_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__pskb_expand_head__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pskb_expand_head_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__pskb_expand_head__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pskb_expand_head_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct consume_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct consume_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct consume_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct consume_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel____pskb_pull_tail__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pskb_pull_tail_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel____pskb_pull_tail__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pskb_pull_tail_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel____pskb_pull_tail__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pskb_pull_tail_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel____pskb_pull_tail__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pskb_pull_tail_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_put_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__skb_put__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_put_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_put_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__skb_put__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_put_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__skb_tstamp_tx__orig_skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__skb_tstamp_tx__orig_skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__skb_tstamp_tx__orig_skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__skb_tstamp_tx__orig_skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__skb_tstamp_tx__hwtstamps__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* call_ctx,
	struct skb_shared_hwtstamps const* ptr);

void callee_unmarshal_kernel__skb_tstamp_tx__hwtstamps__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* call_ctx,
	struct skb_shared_hwtstamps* ptr);

void callee_marshal_kernel__skb_tstamp_tx__hwtstamps__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* call_ctx,
	struct skb_shared_hwtstamps const* ptr);

void caller_unmarshal_kernel__skb_tstamp_tx__hwtstamps__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* call_ctx,
	struct skb_shared_hwtstamps* ptr);

void caller_marshal_kernel__skb_pad__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_pad_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__skb_pad__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_pad_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__skb_pad__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_pad_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__skb_pad__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_pad_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__napi_consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_consume_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__napi_consume_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_consume_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__napi_consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_consume_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__napi_consume_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_consume_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __dev_kfree_skb_any_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __dev_kfree_skb_any_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __dev_kfree_skb_any_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __dev_kfree_skb_any_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel____napi_alloc_skb__ret_sk_buff__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel____napi_alloc_skb__ret_sk_buff__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel____napi_alloc_skb__ret_sk_buff__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel____napi_alloc_skb__ret_sk_buff__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel____napi_alloc_skb__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel____napi_alloc_skb__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel____napi_alloc_skb__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel____napi_alloc_skb__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel____napi_alloc_skb__napi_struct_gro_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel____napi_alloc_skb__napi_struct_gro_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel____napi_alloc_skb__napi_struct_gro_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel____napi_alloc_skb__napi_struct_gro_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__napi_gro_flush__napi__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__napi_gro_flush__napi__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__napi_gro_flush__napi__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__napi_gro_flush__napi__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__napi_gro_flush__napi_struct_gro_list__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__napi_gro_flush__napi_struct_gro_list__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__napi_gro_flush__napi_struct_gro_list__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__napi_gro_flush__napi_struct_gro_list__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__napi_complete_done__n__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__napi_complete_done__n__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__napi_complete_done__n__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__napi_complete_done__n__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__napi_complete_done__napi_struct_gro_list__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__napi_complete_done__napi_struct_gro_list__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__napi_complete_done__napi_struct_gro_list__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__napi_complete_done__napi_struct_gro_list__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__call_netdevice_notifiers__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct call_netdevice_notifiers_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__call_netdevice_notifiers__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct call_netdevice_notifiers_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__call_netdevice_notifiers__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct call_netdevice_notifiers_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__call_netdevice_notifiers__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct call_netdevice_notifiers_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__dev_get_stats__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* call_ctx,
	struct rtnl_link_stats64 const* ptr);

void callee_unmarshal_kernel__dev_get_stats__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* call_ctx,
	struct rtnl_link_stats64* ptr);

void callee_marshal_kernel__dev_get_stats__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* call_ctx,
	struct rtnl_link_stats64 const* ptr);

void caller_unmarshal_kernel__dev_get_stats__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* call_ctx,
	struct rtnl_link_stats64* ptr);

void caller_marshal_kernel__dev_get_stats__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__dev_get_stats__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__dev_get_stats__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__dev_get_stats__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__dev_get_stats__storage__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* call_ctx,
	struct rtnl_link_stats64 const* ptr);

void callee_unmarshal_kernel__dev_get_stats__storage__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* call_ctx,
	struct rtnl_link_stats64* ptr);

void callee_marshal_kernel__dev_get_stats__storage__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* call_ctx,
	struct rtnl_link_stats64 const* ptr);

void caller_unmarshal_kernel__dev_get_stats__storage__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* call_ctx,
	struct rtnl_link_stats64* ptr);

void caller_marshal_kernel__unregister_netdev__dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct unregister_netdev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__unregister_netdev__dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct unregister_netdev_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__unregister_netdev__dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct unregister_netdev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__unregister_netdev__dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct unregister_netdev_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct free_netdev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct free_netdev_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct free_netdev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct free_netdev_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__netif_napi_del__napi_struct_gro_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__netif_napi_del__napi_struct_gro_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__netif_napi_del__napi_struct_gro_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__netif_napi_del__napi_struct_gro_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__napi_hash_del__napi__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__napi_hash_del__napi__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__napi_hash_del__napi__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__napi_hash_del__napi__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__napi_hash_del__napi_struct_gro_list__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__napi_hash_del__napi_struct_gro_list__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__napi_hash_del__napi_struct_gro_list__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__napi_hash_del__napi_struct_gro_list__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct register_netdev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct register_netdev_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct register_netdev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct register_netdev_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__net_device__global_netdev_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct net_device_ops const* ptr);

void callee_unmarshal_kernel__net_device__global_netdev_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct net_device_ops* ptr);

void callee_marshal_kernel__net_device__global_netdev_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct net_device_ops const* ptr);

void caller_unmarshal_kernel__net_device__global_netdev_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct net_device_ops* ptr);

void caller_marshal_kernel__net_device__global_ethtool_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_ops const* ptr);

void callee_unmarshal_kernel__net_device__global_ethtool_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_ops* ptr);

void callee_marshal_kernel__net_device__global_ethtool_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_ops const* ptr);

void caller_unmarshal_kernel__net_device__global_ethtool_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_ops* ptr);

void caller_marshal_kernel__netif_tx_stop_all_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_tx_stop_all_queues_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_tx_stop_all_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_tx_stop_all_queues_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_tx_stop_all_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_tx_stop_all_queues_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_tx_stop_all_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_tx_stop_all_queues_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_carrier_on_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_carrier_on_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_carrier_on_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_carrier_on_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_carrier_off_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_carrier_off_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_carrier_off_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_carrier_off_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netdev_all_upper_get_next_dev_rcu__ret_net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__ret_net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netdev_all_upper_get_next_dev_rcu__ret_net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__ret_net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netdev_all_upper_get_next_dev_rcu__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netdev_all_upper_get_next_dev_rcu__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netdev_all_upper_get_next_dev_rcu__iter__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* call_ctx,
	struct list_head const* ptr);

void callee_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__iter__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* call_ctx,
	struct list_head* ptr);

void callee_marshal_kernel__netdev_all_upper_get_next_dev_rcu__iter__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* call_ctx,
	struct list_head const* ptr);

void caller_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__iter__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* call_ctx,
	struct list_head* ptr);

void caller_marshal_kernel__napi_disable__n__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_disable_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__napi_disable__n__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_disable_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__napi_disable__n__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_disable_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__napi_disable__n__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_disable_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__poll__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct poll_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__poll__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct poll_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__poll__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct poll_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__poll__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct poll_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel____napi_schedule_irqoff__n__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_schedule_irqoff_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel____napi_schedule_irqoff__n__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_schedule_irqoff_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel____napi_schedule_irqoff__n__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_schedule_irqoff_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel____napi_schedule_irqoff__n__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_schedule_irqoff_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__napi_gro_receive__napi_struct_gro_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__napi_gro_receive__napi_struct_gro_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__napi_gro_receive__napi_struct_gro_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__napi_gro_receive__napi_struct_gro_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__napi_gro_receive__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__napi_gro_receive__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__napi_gro_receive__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__napi_gro_receive__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__netif_receive_skb__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_receive_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__netif_receive_skb__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_receive_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__netif_receive_skb__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_receive_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__netif_receive_skb__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_receive_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__skb_checksum_help__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_checksum_help_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__skb_checksum_help__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_checksum_help_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__skb_checksum_help__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_checksum_help_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__skb_checksum_help__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_checksum_help_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_device_attach_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_device_attach_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_device_attach_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_device_attach_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_tx_wake_queue_call_ctx const* call_ctx,
	struct netdev_queue const* ptr);

void callee_unmarshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_tx_wake_queue_call_ctx const* call_ctx,
	struct netdev_queue* ptr);

void callee_marshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_tx_wake_queue_call_ctx const* call_ctx,
	struct netdev_queue const* ptr);

void caller_unmarshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_tx_wake_queue_call_ctx const* call_ctx,
	struct netdev_queue* ptr);

void caller_marshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_device_detach_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_device_detach_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_device_detach_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_device_detach_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_wake_subqueue__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_wake_subqueue_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_wake_subqueue__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_wake_subqueue_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_wake_subqueue__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_wake_subqueue_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_wake_subqueue__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_wake_subqueue_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_set_real_num_rx_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_real_num_rx_queues_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_set_real_num_rx_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_real_num_rx_queues_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_set_real_num_rx_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_real_num_rx_queues_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_set_real_num_rx_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_real_num_rx_queues_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_set_real_num_tx_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_real_num_tx_queues_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_set_real_num_tx_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_real_num_tx_queues_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_set_real_num_tx_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_real_num_tx_queues_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_set_real_num_tx_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_real_num_tx_queues_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_set_xps_queue__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__netif_set_xps_queue__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__netif_set_xps_queue__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__netif_set_xps_queue__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__netif_set_xps_queue__mask__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* call_ctx,
	struct cpumask const* ptr);

void callee_unmarshal_kernel__netif_set_xps_queue__mask__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* call_ctx,
	struct cpumask* ptr);

void callee_marshal_kernel__netif_set_xps_queue__mask__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* call_ctx,
	struct cpumask const* ptr);

void caller_unmarshal_kernel__netif_set_xps_queue__mask__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* call_ctx,
	struct cpumask* ptr);

void caller_marshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* call_ctx,
	struct cpumask const* ptr);

void callee_unmarshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* call_ctx,
	struct cpumask* ptr);

void callee_marshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* call_ctx,
	struct cpumask const* ptr);

void caller_unmarshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* call_ctx,
	struct cpumask* ptr);

void caller_marshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct msix_entry const* ptr);

void callee_unmarshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct msix_entry* ptr);

void callee_marshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct msix_entry const* ptr);

void caller_unmarshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* call_ctx,
	struct msix_entry* ptr);

void caller_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__dev_addr_del__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_addr_del_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__dev_addr_del__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_addr_del_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__dev_addr_del__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_addr_del_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__dev_addr_del__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_addr_del_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__dev_addr_add__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_addr_add_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__dev_addr_add__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_addr_add_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__dev_addr_add__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_addr_add_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__dev_addr_add__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_addr_add_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__sync__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sync_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__sync__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sync_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__sync__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sync_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__sync__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sync_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__unsync__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct unsync_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__unsync__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct unsync_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__unsync__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct unsync_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__unsync__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct unsync_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel____hw_addr_unsync_dev__list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct netdev_hw_addr_list const* ptr);

void callee_unmarshal_kernel____hw_addr_unsync_dev__list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct netdev_hw_addr_list* ptr);

void callee_marshal_kernel____hw_addr_unsync_dev__list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct netdev_hw_addr_list const* ptr);

void caller_unmarshal_kernel____hw_addr_unsync_dev__list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct netdev_hw_addr_list* ptr);

void caller_marshal_kernel____hw_addr_unsync_dev__list_head__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct list_head const* ptr);

void callee_unmarshal_kernel____hw_addr_unsync_dev__list_head__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct list_head* ptr);

void callee_marshal_kernel____hw_addr_unsync_dev__list_head__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct list_head const* ptr);

void caller_unmarshal_kernel____hw_addr_unsync_dev__list_head__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct list_head* ptr);

void caller_marshal_kernel____hw_addr_unsync_dev__list_head_next__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct list_head const* ptr);

void callee_unmarshal_kernel____hw_addr_unsync_dev__list_head_next__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct list_head* ptr);

void callee_marshal_kernel____hw_addr_unsync_dev__list_head_next__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct list_head const* ptr);

void caller_unmarshal_kernel____hw_addr_unsync_dev__list_head_next__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct list_head* ptr);

void caller_marshal_kernel____hw_addr_unsync_dev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel____hw_addr_unsync_dev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel____hw_addr_unsync_dev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel____hw_addr_unsync_dev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel____hw_addr_sync_dev__list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* call_ctx,
	struct netdev_hw_addr_list const* ptr);

void callee_unmarshal_kernel____hw_addr_sync_dev__list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* call_ctx,
	struct netdev_hw_addr_list* ptr);

void callee_marshal_kernel____hw_addr_sync_dev__list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* call_ctx,
	struct netdev_hw_addr_list const* ptr);

void caller_unmarshal_kernel____hw_addr_sync_dev__list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* call_ctx,
	struct netdev_hw_addr_list* ptr);

void caller_marshal_kernel____hw_addr_sync_dev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel____hw_addr_sync_dev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel____hw_addr_sync_dev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel____hw_addr_sync_dev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__eth_platform_get_mac_address__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_platform_get_mac_address_call_ctx const* call_ctx,
	struct device const* ptr);

void callee_unmarshal_kernel__eth_platform_get_mac_address__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_platform_get_mac_address_call_ctx const* call_ctx,
	struct device* ptr);

void callee_marshal_kernel__eth_platform_get_mac_address__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_platform_get_mac_address_call_ctx const* call_ctx,
	struct device const* ptr);

void caller_unmarshal_kernel__eth_platform_get_mac_address__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_platform_get_mac_address_call_ctx const* call_ctx,
	struct device* ptr);

void caller_marshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct alloc_etherdev_mqs_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct alloc_etherdev_mqs_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct alloc_etherdev_mqs_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct alloc_etherdev_mqs_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ndo_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_validate_addr_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ndo_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_validate_addr_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ndo_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_validate_addr_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ndo_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_validate_addr_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__dev_trans_start__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_trans_start_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__dev_trans_start__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_trans_start_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__dev_trans_start__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_trans_start_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__dev_trans_start__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_trans_start_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_read_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_read_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_read_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_read_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_write_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_write_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_write_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_write_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mdio_if_info const* ptr);

void callee_unmarshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mdio_if_info* ptr);

void callee_marshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mdio_if_info const* ptr);

void caller_unmarshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mdio_if_info* ptr);

void caller_marshal_kernel__mdio_mii_ioctl__mdio_if_info_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__mdio_mii_ioctl__mdio_if_info_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__mdio_mii_ioctl__mdio_if_info_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__mdio_mii_ioctl__mdio_if_info_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__mdio_mii_ioctl__mii_data__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mii_ioctl_data const* ptr);

void callee_unmarshal_kernel__mdio_mii_ioctl__mii_data__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mii_ioctl_data* ptr);

void callee_marshal_kernel__mdio_mii_ioctl__mii_data__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mii_ioctl_data const* ptr);

void caller_unmarshal_kernel__mdio_mii_ioctl__mii_data__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* call_ctx,
	struct mii_ioctl_data* ptr);

void caller_marshal_kernel__mdio45_probe__mdio__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* call_ctx,
	struct mdio_if_info const* ptr);

void callee_unmarshal_kernel__mdio45_probe__mdio__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* call_ctx,
	struct mdio_if_info* ptr);

void callee_marshal_kernel__mdio45_probe__mdio__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* call_ctx,
	struct mdio_if_info const* ptr);

void caller_unmarshal_kernel__mdio45_probe__mdio__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* call_ctx,
	struct mdio_if_info* ptr);

void caller_marshal_kernel__mdio45_probe__mdio_if_info_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__mdio45_probe__mdio_if_info_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__mdio45_probe__mdio_if_info_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__mdio45_probe__mdio_if_info_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void callee_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void callee_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void caller_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void caller_marshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void callee_unmarshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void callee_marshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_driver const* ptr);

void caller_unmarshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_driver* ptr);

void caller_marshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void callee_unmarshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void callee_marshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_device_id const* ptr);

void caller_unmarshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct pci_device_id* ptr);

void caller_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module const* ptr);

void callee_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module* ptr);

void callee_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module const* ptr);

void caller_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* call_ctx,
	struct module* ptr);

void caller_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void callee_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void callee_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev const* ptr);

void caller_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* call_ctx,
	struct pci_dev* ptr);

void caller_marshal_kernel__lvd_napi_enable__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_napi_enable_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__lvd_napi_enable__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_napi_enable_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__lvd_napi_enable__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_napi_enable_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__lvd_napi_enable__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_napi_enable_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__lvd_netif_trans_update__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_netif_trans_update_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__lvd_netif_trans_update__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_netif_trans_update_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__lvd_netif_trans_update__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_netif_trans_update_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__lvd_netif_trans_update__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_netif_trans_update_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__lvd_netif_tx_disable__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_netif_tx_disable_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__lvd_netif_tx_disable__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_netif_tx_disable_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__lvd_netif_tx_disable__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_netif_tx_disable_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__lvd_netif_tx_disable__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_netif_tx_disable_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__lvd_napi_schedule_irqoff__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_napi_schedule_irqoff_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void callee_unmarshal_kernel__lvd_napi_schedule_irqoff__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_napi_schedule_irqoff_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void callee_marshal_kernel__lvd_napi_schedule_irqoff__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_napi_schedule_irqoff_call_ctx const* call_ctx,
	struct napi_struct const* ptr);

void caller_unmarshal_kernel__lvd_napi_schedule_irqoff__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_napi_schedule_irqoff_call_ctx const* call_ctx,
	struct napi_struct* ptr);

void caller_marshal_kernel__lvd_netif_tx_wake_all_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_netif_tx_wake_all_queues_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__lvd_netif_tx_wake_all_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_netif_tx_wake_all_queues_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__lvd_netif_tx_wake_all_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_netif_tx_wake_all_queues_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__lvd_netif_tx_wake_all_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_netif_tx_wake_all_queues_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_validate_addr_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_validate_addr_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_validate_addr_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_validate_addr_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_op_get_link_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_op_get_link_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_op_get_link_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_op_get_link_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ethtool_op_get_ts_info__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__ethtool_op_get_ts_info__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__ethtool_op_get_ts_info__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__ethtool_op_get_ts_info__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__ethtool_op_get_ts_info__info__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* call_ctx,
	struct ethtool_ts_info const* ptr);

void callee_unmarshal_kernel__ethtool_op_get_ts_info__info__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* call_ctx,
	struct ethtool_ts_info* ptr);

void callee_marshal_kernel__ethtool_op_get_ts_info__info__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* call_ctx,
	struct ethtool_ts_info const* ptr);

void caller_unmarshal_kernel__ethtool_op_get_ts_info__info__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* call_ctx,
	struct ethtool_ts_info* ptr);

void caller_marshal_kernel__ipv6_find_hdr__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ipv6_find_hdr_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__ipv6_find_hdr__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ipv6_find_hdr_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__ipv6_find_hdr__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ipv6_find_hdr_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__ipv6_find_hdr__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ipv6_find_hdr_call_ctx const* call_ctx,
	struct sk_buff* ptr);


#endif
