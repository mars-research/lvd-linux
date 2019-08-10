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
        /*
		case SYNC_PROBE:
			sync_probe_callee(message);
			break;
		case SYNC_NDO_SET_MAC_ADDRESS:
			sync_ndo_set_mac_address_callee(message);
			break;
            */
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

		case PCI_DISABLE_MSIX:
			trace(PCI_DISABLE_MSIX);
			ret =  pci_disable_msix_callee(message);
			break;

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
