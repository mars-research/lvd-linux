#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../nullnet_caller.h"

#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

extern void dummy_cleanup_module(void);
extern int __dummy_lcd_init(void);

int handle_rpc_calls(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);

	switch (fn_type) {
		case MODULE_INIT:
			trace(MODULE_INIT);
			return __dummy_lcd_init();
		case NDO_INIT:
			trace(NDO_INIT);
			return ndo_init_callee(message);
		case NDO_UNINIT:
			trace(NDO_UNINIT);
			ndo_uninit_callee(message);
			/* wait until uninit is called */
			return -1;
		case NDO_START_XMIT:
			trace(NDO_START_XMIT);
			return ndo_start_xmit_async_bare_callee(message);
		case NDO_VALIDATE_ADDR:
			trace(NDO_VALIDATE_ADDR);
			return ndo_validate_addr_callee(message);
		case NDO_GET_STATS64:
			trace(NDO_GET_STATS64);
			return ndo_get_stats64_callee(message);
		case NDO_SET_RX_MODE:
			trace(NDO_SET_RX_MODE);
			return ndo_set_rx_mode_callee(message);
		case NDO_SET_MAC_ADDRESS:
			trace(NDO_SET_MAC_ADDRESS);
			return ndo_set_mac_address_callee(message);
		case NDO_CHANGE_CARRIER:
			trace(NDO_CHANGE_CARRIER);
			return ndo_change_carrier_callee(message);
		case SETUP:
			trace(SETUP);
			return setup_callee(message);
		case VALIDATE:
			trace(VALIDATE);
			return validate_callee(message);
		case TRIGGER_EXIT:
			trace(TRIGGER_EXIT);
			/* call nullnet exit */
			dummy_cleanup_module();
			return 0;
		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;
	}
	return 0;
}
