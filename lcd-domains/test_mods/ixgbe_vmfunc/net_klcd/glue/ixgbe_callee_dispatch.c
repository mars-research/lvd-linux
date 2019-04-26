#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../ixgbe_callee.h"

#ifdef CONFIG_LCD_TRACE_BUFFER
#include <linux/lcd_trace.h>
#endif

#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

int net_klcd_syncipc_dispatch(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);

	switch (fn_type) {
		case SYNC_PROBE:
			sync_probe_callee(message);
			break;
		case SYNC_NDO_SET_MAC_ADDRESS:
			sync_ndo_set_mac_address_callee(message);
			break;
		default:
			LIBLCD_ERR("%s, unexpected function label %d", __func__, fn_type);
			return -EINVAL;
	}
	return 0;
}

int net_klcd_dispatch_async_loop(struct fipc_message *message)
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

		case ALLOC_ETHERDEV_MQS:
			trace(ALLOC_ETHERDEV_MQS);
			ret =  alloc_etherdev_mqs_callee(message);
			break;

		case REGISTER_NETDEV:
			trace(REGISTER_NETDEVICE);
			ret =  register_netdev_callee(message);
			break;

		case ETHER_SETUP:
			trace(ETHER_SETUP);
			ret =  ether_setup_callee(message);
			break;

		case ETH_MAC_ADDR:
			trace(ETH_MAC_ADDR);
			ret =  eth_mac_addr_callee(message);
			break;

		case ETH_VALIDATE_ADDR:
			trace(ETH_VALIDATE_ADDR);
			ret =  eth_validate_addr_callee(message);
			break;

		case FREE_NETDEV:
			trace(FREE_NETDEV);
			ret =  free_netdev_callee(message);
			break;

		case NETIF_CARRIER_OFF:
			trace(NETIF_CARRIER_OFF);
			ret =  netif_carrier_off_callee(message);
			break;

		case NETIF_CARRIER_ON:
			trace(NETIF_CARRIER_ON);
			ret =  netif_carrier_on_callee(message);
			break;

		case NETIF_DEVICE_ATTACH:
			trace(NETIF_DEVICE_ATTACH);
			ret =  netif_device_attach_callee(message);
			break;

		case NETIF_DEVICE_DETACH:
			trace(NETIF_DEVICE_DETACH);
			ret =  netif_device_detach_callee(message);
			break;

		case NETIF_SET_REAL_NUM_RX_QUEUES:
			trace(NETIF_SET_REAL_NUM_RX_QUEUES);
			ret =  netif_set_real_num_rx_queues_callee(message);
			break;

		case NETIF_SET_REAL_NUM_TX_QUEUES:
			trace(NETIF_SET_REAL_NUM_TX_QUEUES);
			ret =  netif_set_real_num_tx_queues_callee(message);
			break;

		case CONSUME_SKB:
			ret =  consume_skb_callee(message);
			break;

		case NAPI_CONSUME_SKB:
			ret =  napi_consume_skb_callee(message);
			break;

		case UNREGISTER_NETDEV:
			trace(UNREGISTER_NETDEV);
			ret =  unregister_netdev_callee(message);
			break;

		case ETH_PLATFORM_GET_MAC_ADDRESS:
			trace(ETH_PLATFORM_GET_MAC_ADDRESS);
			ret =  eth_platform_get_mac_address_callee(message);
			break;

		case DEV_ADDR_ADD:
			trace(DEV_ADDR_ADD);
			ret =  dev_addr_add_callee(message);
			break;

		case DEV_ADDR_DEL:
			trace(DEV_ADDR_DEL);
			ret =  dev_addr_del_callee(message);
			break;

		case DEVICE_SET_WAKEUP_ENABLE:
			trace(DEVICE_SET_WAKEUP_ENABLE);
			ret =  device_set_wakeup_enable_callee(message);
			break;
#if 0
		case ETH_GET_HEADLEN:
			trace(ETH_GET_HEADLEN);
			ret =  eth_get_headlen_callee(message);
			break;
#endif
		case NETIF_TX_STOP_ALL_QUEUES:
			trace(NETIF_TX_STOP_ALL_QUEUES);
			ret =  netif_tx_stop_all_queues_callee(message);
			break;

		case NETIF_TX_WAKE_ALL_QUEUES:
			trace(NETIF_TX_WAKE_ALL_QUEUES);
			ret =  _netif_tx_wake_all_queues_callee(message);
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


		case __HW_ADDR_SYNC_DEV:
			trace(__HW_ADDR_SYNC_DEV);
			ret =  __hw_addr_sync_dev_callee(message);
			break;

		case __HW_ADDR_UNSYNC_DEV:
			trace(__HW_ADDR_UNSYNC_DEV);
			ret =  __hw_addr_unsync_dev_callee(message);
			break;

		case REQUEST_THREADED_IRQ:
			trace(REQUEST_THREADED_IRQ);
			ret =  request_threaded_irq_callee(message);
			break;

		case FREE_IRQ:
			trace(FREE_IRQ);
			ret =  free_irq_callee(message);
			break;

		case NETIF_NAPI_ADD:
			trace(NETIF_NAPI_ADD);
			ret =  netif_napi_add_callee(message);
			break;

		case NETIF_NAPI_DEL:
			trace(NETIF_NAPI_DEL);
			ret =  netif_napi_del_callee(message);
			break;

		case __NETIF_TX_DISABLE:
			trace(__NETIF_TX_DISABLE);
			ret =  __netif_tx_disable_callee(message);
			break;

		case NETIF_WAKE_SUBQUEUE:
			trace(NETIF_WAKE_SUBQUEUE);
			ret =  netif_wake_subqueue_callee(message);
			break;

		case NETIF_RECEIVE_SKB:
			ret =  netif_receive_skb_callee(message);
			break;

		case NAPI_GRO_RECEIVE:
			ret =  napi_gro_receive_callee(message);
			break;

		case __NAPI_ALLOC_SKB:
			trace(NAPI_ALLOC_SKB);
			ret =  __napi_alloc_skb_callee(message);
			break;

		case __NAPI_SCHEDULE_IRQOFF:
			trace(__NAPI_SCHEDULE_IRQOFF);
			ret =  __napi_schedule_irqoff_callee(message);
			break;

		case ___NAPI_SCHEDULE_IRQOFF:
			/* trace(___NAPI_SCHEDULE_IRQOFF); */
			ret =  ___napi_schedule_irqoff_callee(message);
			break;

		case __NAPI_ENABLE:
			trace(__NAPI_ENABLE);
			ret =  __napi_enable_callee(message);
			break;

		case NAPI_DISABLE:
			trace(NAPI_DISABLE);
			ret =  napi_disable_callee(message);
			break;

		case NAPI_HASH_DEL:
			trace(NAPI_HASH_DEL);
			ret =  napi_hash_del_callee(message);
			break;

		case NAPI_COMPLETE_DONE:
			/* trace(NAPI_COMPLETE_DONE); */
			ret =  napi_complete_done_callee(message);
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
