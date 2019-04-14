#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../ixgbe_callee.h"
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
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case __PCI_REGISTER_DRIVER:
			trace(__PCI_REGISTER_DRIVER);
			return __pci_register_driver_callee(message);

		case PCI_UNREGISTER_DRIVER:
			trace(PCI_UNREGISTER_DRIVER);
			return pci_unregister_driver_callee(message);

		case ALLOC_ETHERDEV_MQS:
			trace(ALLOC_ETHERDEV_MQS);
			return alloc_etherdev_mqs_callee(message);

		case REGISTER_NETDEV:
			trace(REGISTER_NETDEVICE);
			return register_netdev_callee(message);

		case ETHER_SETUP:
			trace(ETHER_SETUP);
			return ether_setup_callee(message);

		case ETH_MAC_ADDR:
			trace(ETH_MAC_ADDR);
			return eth_mac_addr_callee(message);

		case ETH_VALIDATE_ADDR:
			trace(ETH_VALIDATE_ADDR);
			return eth_validate_addr_callee(message);

		case FREE_NETDEV:
			trace(FREE_NETDEV);
			return free_netdev_callee(message);

		case NETIF_CARRIER_OFF:
			trace(NETIF_CARRIER_OFF);
			return netif_carrier_off_callee(message);

		case NETIF_CARRIER_ON:
			trace(NETIF_CARRIER_ON);
			return netif_carrier_on_callee(message);

		case NETIF_DEVICE_ATTACH:
			trace(NETIF_DEVICE_ATTACH);
			return netif_device_attach_callee(message);

		case NETIF_DEVICE_DETACH:
			trace(NETIF_DEVICE_DETACH);
			return netif_device_detach_callee(message);

		case NETIF_SET_REAL_NUM_RX_QUEUES:
			trace(NETIF_SET_REAL_NUM_RX_QUEUES);
			return netif_set_real_num_rx_queues_callee(message);

		case NETIF_SET_REAL_NUM_TX_QUEUES:
			trace(NETIF_SET_REAL_NUM_TX_QUEUES);
			return netif_set_real_num_tx_queues_callee(message);

		case CONSUME_SKB:
			return consume_skb_callee(message);

		case NAPI_CONSUME_SKB:
			return napi_consume_skb_callee(message);

		case UNREGISTER_NETDEV:
			trace(UNREGISTER_NETDEV);
			return unregister_netdev_callee(message);

		case ETH_PLATFORM_GET_MAC_ADDRESS:
			trace(ETH_PLATFORM_GET_MAC_ADDRESS);
			return eth_platform_get_mac_address_callee(message);

		case DEV_ADDR_ADD:
			trace(DEV_ADDR_ADD);
			return dev_addr_add_callee(message);

		case DEV_ADDR_DEL:
			trace(DEV_ADDR_DEL);
			return dev_addr_del_callee(message);

		case DEVICE_SET_WAKEUP_ENABLE:
			trace(DEVICE_SET_WAKEUP_ENABLE);
			return device_set_wakeup_enable_callee(message);
#if 0
		case ETH_GET_HEADLEN:
			trace(ETH_GET_HEADLEN);
			return eth_get_headlen_callee(message);
#endif
		case NETIF_TX_STOP_ALL_QUEUES:
			trace(NETIF_TX_STOP_ALL_QUEUES);
			return netif_tx_stop_all_queues_callee(message);

		case NETIF_TX_WAKE_ALL_QUEUES:
			trace(NETIF_TX_WAKE_ALL_QUEUES);
			return _netif_tx_wake_all_queues_callee(message);


		case PCI_DISABLE_PCIE_ERROR_REPORTING:
			trace(PCI_DISABLE_PCIE_ERROR_REPORTING);
			return pci_disable_pcie_error_reporting_callee(message);

		case PCI_BUS_READ_CONFIG_WORD:
			trace(PCI_BUS_READ_CONFIG_WORD);
			return pci_bus_read_config_word_callee(message);

		case PCI_BUS_WRITE_CONFIG_WORD:
			trace(PCI_BUS_WRITE_CONFIG_WORD);
			return pci_bus_write_config_word_callee(message);

		case PCI_CLEANUP_AER_UNCORRECT_ERROR_STATUS:
			trace(PCI_CLEANUP_AER_UNCORRECT_ERROR_STATUS);
			return pci_cleanup_aer_uncorrect_error_status_callee(message);

		case PCI_DISABLE_DEVICE:
			trace(PCI_DISABLE_DEVICE);
			return pci_disable_device_callee(message);

		case PCI_DISABLE_MSIX:
			trace(PCI_DISABLE_MSIX);
			return pci_disable_msix_callee(message);

		case PCI_ENABLE_MSIX_RANGE:
			trace(PCI_ENABLE_MSIX_RANGE);
			return pci_enable_msix_range_callee(message);

		case PCI_ENABLE_PCIE_ERROR_REPORTING:
			trace(PCI_ENABLE_PCIE_ERROR_REPORTING);
			return pci_enable_pcie_error_reporting_callee(message);

		case PCIE_CAPABILITY_READ_WORD:
			trace(PCIE_CAPABILITY_READ_WORD);
			return pcie_capability_read_word_callee(message);

		case PCIE_GET_MINIMUM_LINK:
			trace(PCIE_GET_MINIMUM_LINK);
			return pcie_get_minimum_link_callee(message);

		case PCI_ENABLE_DEVICE_MEM:
			trace(PCI_ENABLE_DEVICE_MEM);
			return pci_enable_device_mem_callee(message);

		case PCI_REQUEST_SELECTED_REGIONS:
			trace(PCI_REQUEST_SELECTED_REGIONS);
			return pci_request_selected_regions_callee(message);

		case PCI_REQUEST_SELECTED_REGIONS_EXCLUSIVE:
			trace(PCI_REQUEST_SELECTED_REGIONS_EXCLUSIVE);
			return pci_request_selected_regions_exclusive_callee(message);

		case PCI_SET_MASTER:
			trace(PCI_SET_MASTER);
			return pci_set_master_callee(message);

		case PCI_SAVE_STATE:
			trace(PCI_SAVE_STATE);
			return pci_save_state_callee(message);

		case PCI_RELEASE_SELECTED_REGIONS:
			trace(PCI_RELEASE_SELECTED_REGIONS);
			return pci_release_selected_regions_callee(message);

		case PCI_SELECT_BARS:
			trace(PCI_SELECT_BARS);
			return pci_select_bars_callee(message);

		case PCI_WAKE_FROM_D3:
			trace(PCI_WAKE_FROM_D3);
			return pci_wake_from_d3_callee(message);


		case __HW_ADDR_SYNC_DEV:
			trace(__HW_ADDR_SYNC_DEV);
			return __hw_addr_sync_dev_callee(message);

		case __HW_ADDR_UNSYNC_DEV:
			trace(__HW_ADDR_UNSYNC_DEV);
			return __hw_addr_unsync_dev_callee(message);

		case REQUEST_THREADED_IRQ:
			trace(REQUEST_THREADED_IRQ);
			return request_threaded_irq_callee(message);

		case FREE_IRQ:
			trace(FREE_IRQ);
			return free_irq_callee(message);

		case NETIF_NAPI_ADD:
			trace(NETIF_NAPI_ADD);
			return netif_napi_add_callee(message);

		case NETIF_NAPI_DEL:
			trace(NETIF_NAPI_DEL);
			return netif_napi_del_callee(message);

		case NETIF_WAKE_SUBQUEUE:
			trace(NETIF_WAKE_SUBQUEUE);
			return netif_wake_subqueue_callee(message);

		case NETIF_RECEIVE_SKB:
			return netif_receive_skb_callee(message);

		case NAPI_GRO_RECEIVE:
			return napi_gro_receive_callee(message);

		case __NAPI_ALLOC_SKB:
			trace(NAPI_ALLOC_SKB);
			return __napi_alloc_skb_callee(message);

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;

}
