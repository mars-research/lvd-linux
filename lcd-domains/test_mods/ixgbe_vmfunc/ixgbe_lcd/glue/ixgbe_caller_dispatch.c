#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../ixgbe_caller.h"
#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

int __ixgbe_lcd_init(void);
void __ixgbe_lcd_exit(void);

extern unsigned long loops_per_jiffy;

int handle_rpc_calls(struct fipc_message *message)
{
	int fn_type;

	fn_type = async_msg_get_fn_type(message);

	switch (fn_type) {
		case NDO_OPEN:
			trace(NDO_OPEN);
			return ndo_open_callee(message);

		case NDO_STOP:
			trace(NDO_STOP);
			return ndo_stop_callee(message);

		case NDO_START_XMIT:
			return ndo_start_xmit_callee(message);

		case NDO_START_XMIT_COPY:
			return ndo_start_xmit_copy_callee(message);

		case NDO_SET_RX_MODE:
			trace(NDO_SET_RX_MODE);
			return ndo_set_rx_mode_callee(message);

		case NDO_VALIDATE_ADDR:
			trace(NDO_VALIDATE_ADDR);
			return ndo_validate_addr_callee(message);

		case NDO_SET_MAC_ADDRESS:
			trace(NDO_SET_MAC_ADDRESS);
			return ndo_set_mac_address_callee(message);

		case NDO_CHANGE_MTU:
			trace(NDO_CHANGE_MTU);
			return ndo_change_mtu_callee(message);

		case NDO_TX_TIMEOUT:
			trace(NDO_TX_TIMEOUT);
			return ndo_tx_timeout_callee(message);

		case NDO_SET_TX_MAXRATE:
			trace(NDO_SET_TX_MAXRATE);
			return ndo_set_tx_maxrate_callee(message);

		case NDO_GET_STATS64:
			/* trace(NDO_GET_STATS64); */
			return ndo_get_stats64_callee(message);

		case NDO_SET_FEATURES:
			trace(NDO_SET_FEATURES);
			return ndo_set_features_callee(message);

		case NDO_FIX_FEATURES:
			trace(NDO_FIX_FEATURES);
			return ndo_fix_features_callee(message);

		case PROBE:
			trace(PROBE);
			return probe_callee(message);

		case REMOVE:
			trace(REMOVE);
			return remove_callee(message);

		case SYNC:
			trace(SYNC);
			return sync_callee(message);

		case UNSYNC:
			trace(UNSYNC);
			return unsync_callee(message);

		case SERVICE_EVENT_SCHED:
			return ixgbe_service_event_schedule_callee(message);

		case TRIGGER_DUMP:
			trace(TRIGGER_DUMP);
			return trigger_dump_callee(message);

		case POLL:
			/* trace(POLL); */
			return poll_callee(message);

		case MODULE_INIT:
			trace(MODULE_INIT);
			loops_per_jiffy = fipc_get_reg0(message);
			return __ixgbe_lcd_init();

		case MODULE_EXIT:
			trace(MODULE_EXIT);
			__ixgbe_lcd_exit();
			break;
		case MSIX_IRQ_HANDLER:
			msix_vector_handler_callee(message);
			break;

		case ETHTOOL_GET_RXNFC:
			get_rxnfc_callee(message);
			break;

		case ETHTOOL_SET_RXNFC:
			set_rxnfc_callee(message);
			break;

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;
	}
	return 0;
}
