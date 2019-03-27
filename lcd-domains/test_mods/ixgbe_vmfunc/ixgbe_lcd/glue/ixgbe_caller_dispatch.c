#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../ixgbe_caller.h"
#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

extern void ixgbe_exit_module(void);
extern bool poll_start;

int dispatch_async_loop(struct fipc_message *message)
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
			trace(NDO_GET_STATS64);
			return ndo_get_stats64_callee(message);

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
			trace(POLL);
			return poll_callee(message);

		case TRIGGER_EXIT:
			trace(TRIGGER_EXIT);
			poll_start = false;
			ixgbe_exit_module();
			/* XXX: return failure to exit the dispatch
			 * loop. After exit, there is no reason to
			 * be spinning on the loop
			 */
			return -1;

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;

}

