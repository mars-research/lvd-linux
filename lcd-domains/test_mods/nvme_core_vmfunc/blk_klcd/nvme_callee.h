#ifndef __NVME_CALLEE_H__
#define __NVME_CALLEE_H__

#include "../nvme_common.h"

int __pci_register_driver_callee(struct fipc_message *_request);
int pci_unregister_driver_callee(struct fipc_message *_request);
int device_release_driver_callee(struct fipc_message *_request);


int blk_mq_alloc_tag_set_callee(struct fipc_message *request);
int blk_mq_init_queue_callee(struct fipc_message *request);
int blk_mq_end_request_callee(struct fipc_message *request);
int blk_mq_free_tag_set_callee(struct fipc_message *request);
int blk_mq_start_request_callee(struct fipc_message *request);
int blk_mq_start_stopped_hw_queues_callee(struct fipc_message *request);
int blk_mq_map_queue_callee(struct fipc_message *request);
int blk_mq_stop_hw_queues_callee(struct fipc_message *_request);
int blk_mq_tagset_busy_iter_callee(struct fipc_message *_request);
int blk_queue_logical_block_size_callee(struct fipc_message *request);
int blk_queue_physical_block_size_callee(struct fipc_message *request);
int alloc_disk_node_callee(struct fipc_message *request);
int device_add_disk_callee(struct fipc_message *request);
int put_disk_callee(struct fipc_message *request);
int del_gendisk_callee(struct fipc_message *request);
int disk_node_callee(struct fipc_message *request);
int register_blkdev_callee(struct fipc_message *request);
int unregister_blkdev_callee(struct fipc_message *request);
int blk_cleanup_queue_callee(struct fipc_message *request);
int lcd_register_chardev_callee(struct fipc_message *request);
int init_hctx_sync_callee(struct fipc_message *request);

int pci_disable_device_callee(struct fipc_message *_request);
int pci_disable_pcie_error_reporting_callee(struct fipc_message *_request);
int pci_bus_read_config_word_callee(struct fipc_message *_request);
int pci_bus_write_config_word_callee(struct fipc_message *_request);
int pci_cleanup_aer_uncorrect_error_status_callee(struct fipc_message *_request);
int pci_disable_device_callee(struct fipc_message *_request);
int pci_disable_msix_callee(struct fipc_message *_request);
int pci_disable_msi_callee(struct fipc_message *_request);
int pci_enable_msix_callee(struct fipc_message *_request);
int pci_enable_msix_range_callee(struct fipc_message *_request);
int pci_enable_msi_range_callee(struct fipc_message *_request);
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
int get_device_callee(struct fipc_message *_request);
int put_device_callee(struct fipc_message *_request);
int request_threaded_irq_callee(struct fipc_message *_request);
int free_irq_callee(struct fipc_message *_request);
int irq_set_affinity_hint_callee(struct fipc_message *_request);

int synchronize_irq_callee(struct fipc_message *_request);

int dispatch_sync_loop(void);

int dispatch_async_loop(struct fipc_message *message);

int glue_nvme_init(void);
void glue_nvme_exit(void);

int sync_probe_callee(struct fipc_message *msg);
int sync_ndo_set_mac_address_callee(struct fipc_message *msg);

int probe(struct pci_dev *dev,
		const struct pci_device_id *id,
		struct trampoline_hidden_args *hidden_args);

void remove(struct pci_dev *dev,
		struct trampoline_hidden_args *hidden_args);

#endif /* __NVME_CALLEE_H__ */
