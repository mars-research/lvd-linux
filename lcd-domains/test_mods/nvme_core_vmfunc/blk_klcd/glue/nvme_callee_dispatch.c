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

		case PCI_ENABLE_MSI_RANGE:
			trace(PCI_ENABLE_MSI_RANGE);
			return pci_enable_msi_range_callee(message);

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
			ret = pci_wake_from_d3_callee(message);
			break;

		case PCI_RESTORE_STATE:
			trace(PCI_RESTORE_STATE);
			ret = pci_restore_state_callee(message);
			break;

		case PCI_DEVICE_IS_PRESENT:
			trace(PCI_DEVICE_IS_PRESENT);
			ret = pci_device_is_present_callee(message);
			break;

		case BLK_MQ_ALLOC_TAG_SET:
			trace(BLK_MQ_ALLOC_TAG_SET);
			return blk_mq_alloc_tag_set_callee(message);

		case BLK_MQ_INIT_QUEUE:
			trace(BLK_MQ_INIT_QUEUE);
			return blk_mq_init_queue_callee(message);

		case BLK_GET_QUEUE:
			trace(BLK_GET_QUEUE);
			return blk_get_queue_callee(message);

		case BLK_PUT_QUEUE:
			trace(BLK_PUT_QUEUE);
			return blk_put_queue_callee(message);

		case BLK_MQ_END_REQUEST:
			/* trace(BLK_MQ_END_REQUEST); */
			return blk_mq_end_request_callee(message);

		case BLK_MQ_FREE_TAG_SET:
			trace(BLK_MQ_FREE_TAG_SET);
			return blk_mq_free_tag_set_callee(message);

		case BLK_MQ_START_REQUEST:
			trace(BLK_MQ_START_REQUEST);
			return blk_mq_start_request_callee(message);

		case BLK_MQ_COMPLETE_REQUEST:
			trace(BLK_MQ_COMPLETE_REQUEST);
			return blk_mq_complete_request_callee(message);

		case BLK_MQ_START_STOPPED_HW_QUEUES:
			trace(BLK_MQ_START_STOPPED_HW_QUEUES);
			return blk_mq_start_stopped_hw_queues_callee(message);

		case BLK_MQ_TAGSET_BUSY_ITER:
			trace(BLK_MQ_TAGSET_BUSY_ITER);
			return blk_mq_tagset_busy_iter_callee(message);

		case BLK_MQ_MAP_QUEUE:
			trace(BLK_MQ_MAP_QUEUE);
			return blk_mq_map_queue_callee(message);

		case BLK_RQ_MAP_KERN:
			trace(BLK_RQ_MAP_KERN);
			return blk_rq_map_kern_callee(message);

		case BLK_QUEUE_LOGICAL_BLOCK_SIZE:
			trace(BLK_QUEUE_LOGICAL_BLOCK_SIZE);
			return blk_queue_logical_block_size_callee(message);

		case BLK_QUEUE_PHYSICAL_BLOCK_SIZE:
			trace(BLK_QUEUE_PHYSICAL_BLOCK_SIZE);
			return blk_queue_physical_block_size_callee(message);

		case BLK_CLEANUP_QUEUE:
			trace(BLK_CLEANUP_QUEUE);
			return blk_cleanup_queue_callee(message);

		case IRQ_SET_AFFINITY_HINT:
			trace(IRQ_SET_AFFINITY_HINT);
			return irq_set_affinity_hint_callee(message);

		case ALLOC_DISK:
			trace(ALLOC_DISK);
			return alloc_disk_node_callee(message);

		case DEVICE_ADD_DISK:
			trace(ADD_DISK);
			return device_add_disk_callee(message);

		case DEVICE_RELEASE_DRIVER:
			trace(DEVICE_RELEASE_DRIVER);
			return device_release_driver_callee(message);

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

		case __UNREGISTER_CHRDEV:
			trace(__UNREGISTER_CHRDEV);
			return __unregister_chrdev_callee(message);

		case __REGISTER_CHRDEV:
			trace(__REGISTER_CHRDEV);
			return __register_chrdev_callee(message);

		case IDA_DESTROY:
			trace(IDA_DESTROY);
			return ida_destroy_callee(message);

		case IDA_GET_NEW_ABOVE:
			trace(IDA_GET_NEW_ABOVE);
			return ida_get_new_above_callee(message);

		case IDA_INIT:
			trace(IDA_INIT);
			return ida_init_callee(message);

		case IDA_PRE_GET:
			trace(IDA_PRE_GET);
			return ida_pre_get_callee(message);

		case IDA_REMOVE:
			trace(IDA_REMOVE);
			return ida_remove_callee(message);

		case IDA_SIMPLE_GET:
			trace(IDA_SIMPLE_GET);
			return ida_simple_get_callee(message);

		case IDA_SIMPLE_REMOVE:
			trace(IDA_SIMPLE_REMOVE);
			return ida_simple_remove_callee(message);

		case BLK_SET_QUEUE_DYING:
			trace(BLK_SET_QUEUE_DYING);
			return blk_set_queue_dying_callee(message);

		case BLK_RQ_UNMAP_USER:
			trace(BLK_RQ_UNMAP_USER);
			return blk_rq_unmap_user_callee(message);

		case BLK_QUEUE_WRITE_CACHE:
			trace(BLK_QUEUE_WRITE_CACHE);
			return blk_queue_write_cache_callee(message);

		case BLK_QUEUE_VIRT_BOUNDARY:
			trace(BLK_QUEUE_VIRT_BOUNDARY);
			return blk_queue_virt_boundary_callee(message);

		case BLK_QUEUE_MAX_SEGMENTS:
			trace(BLK_QUEUE_MAX_SEGMENTS);
			return blk_queue_max_segments_callee(message);

		case BLK_QUEUE_MAX_HW_SECTORS:
			trace(BLK_QUEUE_MAX_HW_SECTORS);
			return blk_queue_max_hw_sectors_callee(message);

		case BLK_QUEUE_MAX_DISCARD_SECTORS:
			trace(BLK_QUEUE_MAX_DISCARD_SECTORS);
			return blk_queue_max_discard_sectors_callee(message);

		case BLK_QUEUE_CHUNK_SECTORS:
			trace(BLK_QUEUE_CHUNK_SECTORS);
			return blk_queue_chunk_sectors_callee(message);

		case BLK_MQ_UNFREEZE_QUEUE:
			trace(BLK_MQ_UNFREEZE_QUEUE);
			return blk_mq_unfreeze_queue_callee(message);

		case BLK_MQ_UPDATE_NR_HW_QUEUES:
			trace(BLK_MQ_UPDATE_NR_HW_QUEUES);
			return blk_mq_update_nr_hw_queues_callee(message);

		case BLK_MQ_STOP_HW_QUEUES:
			trace(BLK_MQ_STOP_HW_QUEUES);
			return blk_mq_stop_hw_queues_callee(message);

		case BLK_MQ_REQUEUE_REQUEST:
			trace(BLK_MQ_REQUEUE_REQUEST);
			return blk_mq_requeue_request_callee(message);

		case BLK_MQ_REQUEST_STARTED:
			trace(BLK_MQ_REQUEST_STARTED);
			return blk_mq_request_started_callee(message);

		case BLK_MQ_KICK_REQUEUE_LIST:
			trace(BLK_MQ_KICK_REQUEUE_LIST);
			return blk_mq_kick_requeue_list_callee(message);

		case BLK_MQ_FREEZE_QUEUE:
			trace(BLK_MQ_FREEZE_QUEUE);
			return blk_mq_freeze_queue_callee(message);

		case BLK_MQ_CANCEL_REQUEUE_WORK:
			trace(BLK_MQ_CANCEL_REQUEUE_WORK);
			return blk_mq_cancel_requeue_work_callee(message);

		case BLK_MQ_ALLOC_REQUEST_HCTX:
			trace(BLK_MQ_ALLOC_REQUEST_HCTX);
			return blk_mq_alloc_request_hctx_callee(message);

		case BLK_MQ_ALLOC_REQUEST:
			trace(BLK_MQ_ALLOC_REQUEST);
			return blk_mq_alloc_request_callee(message);

		case BLK_MQ_FREE_REQUEST:
			trace(BLK_MQ_FREE_REQUEST);
			return blk_mq_free_request_callee(message);

		case BLK_MQ_ABORT_REQUEUE_LIST:
			trace(BLK_MQ_ABORT_REQUEUE_LIST);
			return blk_mq_abort_requeue_list_callee(message);

		case BLK_EXECUTE_RQ:
			trace(BLK_EXECUTE_RQ);
			return blk_execute_rq_callee(message);

		case BDPUT:
			trace(BDPUT);
			return bdput_callee(message);

		case BDGET_DISK:
			trace(BDGET_DISK);
			return bdget_disk_callee(message);

		case __CLASS_CREATE:
			trace(__CLASS_CREATE);
			return __class_create_callee(message);

		case CLASS_DESTROY:
			trace(CLASS_DESTROY);
			return class_destroy_callee(message);

		case DEVICE_CREATE:
			trace(DEVICE_CREATE);
			return device_create_callee(message);

		case DEVICE_DESTROY:
			trace(DEVICE_DESTROY);
			return device_destroy_callee(message);

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
