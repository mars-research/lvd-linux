#ifndef __IXGBE_CALLEE_H__
#define __IXGBE_CALLEE_H__

#include "../ixgbe_common.h"

int __pci_register_driver_callee(struct fipc_message *_request);
int pci_unregister_driver_callee(struct fipc_message *_request);

int alloc_etherdev_mqs_callee(struct fipc_message *_request);
int register_netdev_callee(struct fipc_message *_request);
int ether_setup_callee(struct fipc_message *_request);
int eth_mac_addr_callee(struct fipc_message *_request);
int eth_validate_addr_callee(struct fipc_message *_request);
int free_netdev_callee(struct fipc_message *_request);
int netif_carrier_off_callee(struct fipc_message *_request);
int __netif_tx_disable_callee(struct fipc_message *_request);
int netif_carrier_on_callee(struct fipc_message *_request);
int netif_device_attach_callee(struct fipc_message *_request);
int netif_device_detach_callee(struct fipc_message *_request);
int netif_set_real_num_rx_queues_callee(struct fipc_message *_request);
int netif_set_real_num_tx_queues_callee(struct fipc_message *_request);
int consume_skb_callee(struct fipc_message *_request);
int napi_consume_skb_callee(struct fipc_message *_request);

int unregister_netdev_callee(struct fipc_message *_request);
int eth_platform_get_mac_address_callee(struct fipc_message *_request);
int dev_addr_add_callee(struct fipc_message *_request);
int dev_addr_del_callee(struct fipc_message *_request);
int device_set_wakeup_enable_callee(struct fipc_message *_request);
int eth_get_headlen_callee(struct fipc_message *_request);
int netif_tx_stop_all_queues_callee(struct fipc_message *_request);
int _netif_tx_wake_all_queues_callee(struct fipc_message *_request);
int pci_disable_device_callee(struct fipc_message *_request);
int pci_disable_pcie_error_reporting_callee(struct fipc_message *_request);
int pci_bus_read_config_word_callee(struct fipc_message *_request);
int pci_bus_write_config_word_callee(struct fipc_message *_request);
int pci_cleanup_aer_uncorrect_error_status_callee(struct fipc_message *_request);
int pci_disable_device_callee(struct fipc_message *_request);
int pci_disable_msix_callee(struct fipc_message *_request);
int pci_enable_msix_range_callee(struct fipc_message *_request);
int pci_enable_pcie_error_reporting_callee(struct fipc_message *_request);
int pcie_capability_read_word_callee(struct fipc_message *_request);
int pcie_get_minimum_link_callee(struct fipc_message *_request);
int pci_enable_device_mem_callee(struct fipc_message *_request);
int pci_request_selected_regions_callee(struct fipc_message *_request);
int pci_request_selected_regions_exclusive_callee(struct fipc_message *_request);
int pci_set_master_callee(struct fipc_message *_request);
int pci_save_state_callee(struct fipc_message *_request);
int pci_release_selected_regions_callee(struct fipc_message *_request);
int pci_select_bars_callee(struct fipc_message *_request);
int pci_wake_from_d3_callee(struct fipc_message *_request);
int __hw_addr_sync_dev_callee(struct fipc_message *_request);
int __hw_addr_unsync_dev_callee(struct fipc_message *_request);

int netif_napi_add_callee(struct fipc_message *_request);
int netif_napi_del_callee(struct fipc_message *_request);
int netif_wake_subqueue_callee(struct fipc_message *_request);

int request_threaded_irq_callee(struct fipc_message *_request);
int free_irq_callee(struct fipc_message *_request);
int netif_receive_skb_callee(struct fipc_message *_request);
int napi_gro_receive_callee(struct fipc_message *_request);

int __napi_alloc_skb_callee(struct fipc_message *_request);
int __napi_schedule_irqoff_callee(struct fipc_message *_request);
int ___napi_schedule_irqoff_callee(struct fipc_message *_request);
int __napi_enable_callee(struct fipc_message *_request);
int napi_disable_callee(struct fipc_message *_request);
int napi_complete_done_callee(struct fipc_message *_request);
int napi_hash_del_callee(struct fipc_message *_request);
int synchronize_irq_callee(struct fipc_message *_request);

int eth_type_trans_callee(struct fipc_message *_request);
int skb_add_rx_frag_callee(struct fipc_message *_request);

void rtnl_lock_callee(struct fipc_message *request);
void rtnl_unlock_callee(struct fipc_message *request);

int dispatch_sync_loop(void);

int dispatch_async_loop(struct fipc_message *message);

int glue_ixgbe_init(void);
void glue_ixgbe_exit(void);

int sync_probe_callee(struct fipc_message *msg);
int sync_ndo_set_mac_address_callee(struct fipc_message *msg);

int probe(struct pci_dev *dev,
		const struct pci_device_id *id,
		struct trampoline_hidden_args *hidden_args);
void remove(struct pci_dev *dev,
		struct trampoline_hidden_args *hidden_args);

/* XXX: How to determine this? */
#define SKB_HASH_BITS      8

struct skb_hash_table {
	DECLARE_HASHTABLE(skb_table, SKB_HASH_BITS);
};

#endif /* __IXGBE_CALLEE_H__ */
