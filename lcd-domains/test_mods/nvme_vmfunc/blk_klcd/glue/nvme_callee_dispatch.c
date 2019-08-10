#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../nvme_callee.h"

#ifdef CONFIG_LCD_TRACE_BUFFER
#include <linux/lcd_trace.h>
#endif

#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

int blk_klcd_syncipc_dispatch(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);

	switch (fn_type) {

		case SYNC_PROBE:
			sync_probe_callee(message);
			break;

		default:
			LIBLCD_ERR("%s, unexpected function label %d", __func__, fn_type);
			return -EINVAL;
	}
	return 0;
}

int blk_klcd_dispatch_async_loop(struct fipc_message *message)
{
	int fn_type;
	int ret;
	fn_type = async_msg_get_fn_type(message);

#ifdef CONFIG_LCD_TRACE_BUFFER
	add_trace_entry(EVENT_VMFUNC_SBOARD_KLCD_ENTER, async_msg_get_fn_type(message));
#endif

	switch (fn_type) {
		case __PCI_REGISTER_DRIVER:
			trace(__PCI_REGISTER_DRIVER);
			ret =  __pci_register_driver_callee(message);
			break;

		case PCI_UNREGISTER_DRIVER:
			trace(PCI_UNREGISTER_DRIVER);
			ret =  pci_unregister_driver_callee(message);
			break;

		case PCI_DISABLE_PCIE_ERROR_REPORTING:
			trace(PCI_DISABLE_PCIE_ERROR_REPORTING);
			ret =  pci_disable_pcie_error_reporting_callee(message);
			break;

		case PCI_BUS_READ_CONFIG_WORD:
			trace(PCI_BUS_READ_CONFIG_WORD);
			ret =  pci_bus_read_config_word_callee(message);
			break;

		case PCI_BUS_WRITE_CONFIG_WORD:
			trace(PCI_BUS_WRITE_CONFIG_WORD);
			ret =  pci_bus_write_config_word_callee(message);
			break;

		case PCI_CLEANUP_AER_UNCORRECT_ERROR_STATUS:
			trace(PCI_CLEANUP_AER_UNCORRECT_ERROR_STATUS);
			ret =  pci_cleanup_aer_uncorrect_error_status_callee(message);
			break;

		case PCI_DISABLE_DEVICE:
			trace(PCI_DISABLE_DEVICE);
			ret =  pci_disable_device_callee(message);
			break;

		case PCI_DISABLE_MSI:
			trace(PCI_DISABLE_MSI);
			ret =  pci_disable_msi_callee(message);
			break;

		case PCI_DISABLE_MSIX:
			trace(PCI_DISABLE_MSIX);
			ret =  pci_disable_msix_callee(message);
			break;

		case PCI_ENABLE_MSIX:
			trace(PCI_ENABLE_MSIX);
			return pci_enable_msix_callee(message);

		case PCI_ENABLE_MSIX_RANGE:
			trace(PCI_ENABLE_MSIX_RANGE);
			ret =  pci_enable_msix_range_callee(message);
			break;

		case PCI_ENABLE_PCIE_ERROR_REPORTING:
			trace(PCI_ENABLE_PCIE_ERROR_REPORTING);
			ret =  pci_enable_pcie_error_reporting_callee(message);
			break;

		case PCIE_CAPABILITY_READ_WORD:
			trace(PCIE_CAPABILITY_READ_WORD);
			ret =  pcie_capability_read_word_callee(message);
			break;

		case PCIE_GET_MINIMUM_LINK:
			trace(PCIE_GET_MINIMUM_LINK);
			ret =  pcie_get_minimum_link_callee(message);
			break;

		case PCI_ENABLE_DEVICE_MEM:
			trace(PCI_ENABLE_DEVICE_MEM);
			ret =  pci_enable_device_mem_callee(message);
			break;

		case PCI_REQUEST_SELECTED_REGIONS:
			trace(PCI_REQUEST_SELECTED_REGIONS);
			ret =  pci_request_selected_regions_callee(message);
			break;

		case PCI_REQUEST_SELECTED_REGIONS_EXCLUSIVE:
			trace(PCI_REQUEST_SELECTED_REGIONS_EXCLUSIVE);
			ret =  pci_request_selected_regions_exclusive_callee(message);
			break;

		case PCI_SET_MASTER:
			trace(PCI_SET_MASTER);
			ret =  pci_set_master_callee(message);
			break;

		case PCI_SAVE_STATE:
			trace(PCI_SAVE_STATE);
			ret =  pci_save_state_callee(message);
			break;

		case PCI_RELEASE_SELECTED_REGIONS:
			trace(PCI_RELEASE_SELECTED_REGIONS);
			ret =  pci_release_selected_regions_callee(message);
			break;

		case PCI_SELECT_BARS:
			trace(PCI_SELECT_BARS);
			ret =  pci_select_bars_callee(message);
			break;

		case PCI_WAKE_FROM_D3:
			trace(PCI_WAKE_FROM_D3);
			ret =  pci_wake_from_d3_callee(message);
			break;

		case BLK_MQ_ALLOC_TAG_SET:
			trace(BLK_MQ_ALLOC_TAG_SET);
			return blk_mq_alloc_tag_set_callee(message);

		case BLK_MQ_INIT_QUEUE:
			trace(BLK_MQ_INIT_QUEUE);
			return blk_mq_init_queue_callee(message);

		case BLK_MQ_END_REQUEST:
			/* trace(BLK_MQ_END_REQUEST); */
			return blk_mq_end_request_callee(message);

		case BLK_MQ_FREE_TAG_SET:
			trace(BLK_MQ_FREE_TAG_SET);
			return blk_mq_free_tag_set_callee(message);

		case BLK_MQ_START_REQUEST:
			/* trace(BLK_MQ_START_REQUEST); */
			return blk_mq_start_request_callee(message);

		case BLK_MQ_START_STOPPED_HW_QUEUES:
			trace(BLK_MQ_START_STOPPED_HW_QUEUES);
			return blk_mq_start_stopped_hw_queues_callee(message);

		case BLK_MQ_MAP_QUEUE:
			trace(BLK_MQ_MAP_QUEUE);
			return blk_mq_map_queue_callee(message);

		case BLK_QUEUE_LOGICAL_BLOCK_SIZE:
			trace(BLK_QUEUE_LOGICAL_BLOCK_SIZE);
			return blk_queue_logical_block_size_callee(message);

		case BLK_QUEUE_PHYSICAL_BLOCK_SIZE:
			trace(BLK_QUEUE_PHYSICAL_BLOCK_SIZE);
			return blk_queue_physical_block_size_callee(message);

		case BLK_CLEANUP_QUEUE:
			trace(BLK_CLEANUP_QUEUE);
			return blk_cleanup_queue_callee(message);

		case BLK_MQ_STOP_HW_QUEUES:
			trace(BLK_MQ_STOP_HW_QUEUES);
			return blk_mq_stop_hw_queues_callee(message);

		case IRQ_SET_AFFINITY_HINT:
			trace(IRQ_SET_AFFINITY_HINT);
			return irq_set_affinity_hint_callee(message);

		case ALLOC_DISK:
			trace(ALLOC_DISK);
			return alloc_disk_node_callee(message);

		case DEVICE_ADD_DISK:
			trace(ADD_DISK);
			return device_add_disk_callee(message);

		case PUT_DISK:
			trace(PUT_DISK);
			return put_disk_callee(message);

		case DEL_GENDISK:
			trace(DEL_GENDISK);
			return del_gendisk_callee(message);

		case REGISTER_BLKDEV:
			trace(REGISTER_BLKDEV);
			return register_blkdev_callee(message);

		case UNREGISTER_BLKDEV:
			trace(UNREGISTER_BLKDEV);
			return unregister_blkdev_callee(message);

		case GET_DEVICE:
			trace(GET_DEVICE);
			return get_device_callee(message);

		case PUT_DEVICE:
			trace(PUT_DEVICE);
			return put_device_callee(message);

		case REQUEST_THREADED_IRQ:
			trace(REQUEST_THREADED_IRQ);
			ret =  request_threaded_irq_callee(message);
			break;

		case FREE_IRQ:
			trace(FREE_IRQ);
			ret =  free_irq_callee(message);
			break;

		case SYNCHRONIZE_IRQ:
			trace(SYNCHRONIZE_IRQ);
			ret =  synchronize_irq_callee(message);
			break;

		case NVME_ALLOC_REQUEST:
			trace(NVME_ALLOC_REQUEST);
			break;

		case NVME_CANCEL_REQUEST:
			trace(NVME_CANCEL_REQUEST);
			break;

		case NVME_COMPLETE_ASYNC_EVENT:
			trace(NVME_COMPLETE_ASYNC_EVENT);
			break;

		case NVME_DISABLE_CTRL:
			trace(NVME_DISABLE_CTRL);
			return nvme_disable_ctrl_callee(message);

		case NVME_ENABLE_CTRL:
			trace(NVME_ENABLE_CTRL);
			return nvme_enable_ctrl_callee(message);

		case NVME_INIT_CTRL:
			trace(NVME_INIT_CTRL);
			return nvme_init_ctrl_callee(message);

		case NVME_UNINIT_CTRL:
			trace(NVME_UNINIT_CTRL);
			return nvme_uninit_ctrl_callee(message);

		case NVME_CHANGE_CTRL_STATE:
			trace(NVME_CHANGE_CTRL_STATE);
			return nvme_change_ctrl_state_callee(message);

		case NVME_INIT_IDENTIFY:
			trace(NVME_INIT_IDENTIFY);
			return nvme_init_identify_callee(message);

		case NVME_IO_TIMEOUT_DISPATCH:
			trace(NVME_IO_TIMEOUT_DISPATCH);
			break;

		case NVME_KILL_QUEUES:
			trace(NVME_KILL_QUEUES);
			return nvme_kill_queues_callee(message);

		case NVME_MAX_RETRIES:
			trace(NVME_MAX_RETRIES);
			break;

		case NVME_PUT_CTRL:
			trace(NVME_PUT_CTRL);
			return nvme_put_ctrl_callee(message);

		case NVME_QUEUE_ASYNC_EVENTS:
			trace(NVME_QUEUE_ASYNC_EV);
			return nvme_queue_async_events_callee(message);
			break;

		case NVME_QUEUE_SCAN:
			trace(NVME_QUEUE_SCAN);
			return nvme_queue_scan_callee(message);

		case NVME_REMOVE_NAMESPACES:
			trace(NVME_REMOVE_NAMESPACE);
			return nvme_remove_namespaces_callee(message);

		case NVME_REQUEUE_REQ:
			trace(NVME_REQUEUE_REQ);
			break;

		case NVME_SET_QUEUE_COUNT:
			trace(NVME_SET_QUEUE_COUNT);
			return nvme_set_queue_count_callee(message);

		case NVME_SETUP_CMD:
			trace(NVME_SETUP_CMD);
			break;

		case NVME_SHUTDOWN_CTRL:
			trace(NVME_SHUTDOWN_CTRL);
			return nvme_shutdown_ctrl_callee(message);

		case NVME_START_QUEUES:
			trace(NVME_START_QUEUES);
			return nvme_start_queues_callee(message);

		case NVME_STOP_QUEUES:
			trace(NVME_STOP_QUEUES);
			return nvme_stop_queues_callee(message);

		case NVME_SUBMIT_SYNC_CMD:
			trace(NVME_SUBMIT_SYNC_CMD);
			break;

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			ret =  -EINVAL;
			break;

	}

#ifdef CONFIG_LCD_TRACE_BUFFER
	add_trace_entry(EVENT_VMFUNC_SBOARD_KLCD_LEAVE, async_msg_get_fn_type(message));
#endif
	return ret;
}
