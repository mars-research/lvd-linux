#ifndef __NVME_CALLEE_H__
#define __NVME_CALLEE_H__

#include "../nvme_common.h"

int __pci_register_driver_callee(struct fipc_message *_request);
int pci_unregister_driver_callee(struct fipc_message *_request);
int device_release_driver_callee(struct fipc_message *_request);


int blk_mq_alloc_tag_set_callee(struct fipc_message *request);
int blk_mq_init_queue_callee(struct fipc_message *request);
int blk_get_queue_callee(struct fipc_message *request);
int blk_mq_end_request_callee(struct fipc_message *request);
int blk_mq_free_tag_set_callee(struct fipc_message *request);
int blk_mq_start_request_callee(struct fipc_message *request);
int blk_mq_complete_request_callee(struct fipc_message *request);
int blk_mq_free_request_callee(struct fipc_message *request);
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

int capable_callee(struct fipc_message *_request);

int synchronize_irq_callee(struct fipc_message *_request);

int __unregister_chrdev_callee(struct fipc_message *_request);
int __register_chrdev_callee(struct fipc_message *_request);
int ida_destroy_callee(struct fipc_message *_request);
int ida_get_new_above_callee(struct fipc_message *_request);
int ida_init_callee(struct fipc_message *_request);
int ida_pre_get_callee(struct fipc_message *_request);
int ida_remove_callee(struct fipc_message *_request);
int ida_simple_get_callee(struct fipc_message *_request);
int ida_simple_remove_callee(struct fipc_message *_request);
int blk_set_queue_dying_callee(struct fipc_message *_request);
int blk_rq_unmap_user_callee(struct fipc_message *_request);
int blk_rq_map_kern_callee(struct fipc_message *_request);
int blk_queue_write_cache_callee(struct fipc_message *_request);
int blk_queue_virt_boundary_callee(struct fipc_message *_request);
int blk_queue_max_segments_callee(struct fipc_message *_request);
int blk_queue_max_hw_sectors_callee(struct fipc_message *_request);
int blk_queue_max_discard_sectors_callee(struct fipc_message *_request);
int blk_queue_chunk_sectors_callee(struct fipc_message *_request);
int blk_mq_unfreeze_queue_callee(struct fipc_message *_request);
int blk_mq_stop_hw_queues_callee(struct fipc_message *_request);
int blk_mq_requeue_request_callee(struct fipc_message *_request);
int blk_mq_request_started_callee(struct fipc_message *_request);
int blk_mq_kick_requeue_list_callee(struct fipc_message *_request);
int blk_mq_freeze_queue_callee(struct fipc_message *_request);
int blk_mq_cancel_requeue_work_callee(struct fipc_message *_request);
int blk_mq_alloc_request_hctx_callee(struct fipc_message *_request);
int blk_mq_alloc_request_callee(struct fipc_message *_request);
int blk_mq_abort_requeue_list_callee(struct fipc_message *_request);
int blk_execute_rq_callee(struct fipc_message *_request);
int bdput_callee(struct fipc_message *_request);
int bdget_disk_callee(struct fipc_message *_request);
int __class_create_callee(struct fipc_message *_request);
int class_destroy_callee(struct fipc_message *_request);
int device_create_callee(struct fipc_message *_request);
int device_destroy_callee(struct fipc_message *_request);
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
