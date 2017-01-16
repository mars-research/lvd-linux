#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../nullnet_caller.h"

#include <lcd_config/post_hook.h>

int dispatch_async_loop(struct thc_channel *channel, struct fipc_message *message, struct glue_cspace *cspace, struct cptr sync_ep)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case NDO_INIT:
			LIBLCD_MSG("Calling function ndo_init");
			return ndo_init_callee(message, channel, cspace, sync_ep);
			break;
		case NDO_UNINIT:
			LIBLCD_MSG("Calling function ndo_uninit");
			return ndo_uninit_callee(message, channel, cspace, sync_ep);
			break;
		case NDO_START_XMIT:
			LIBLCD_MSG("Calling function ndo_start_xmit");
			return ndo_start_xmit_callee(message, channel, cspace, sync_ep);
			break;
		case NDO_VALIDATE_ADDR:
			LIBLCD_MSG("Calling function ndo_validate_addr");
			return ndo_validate_addr_callee(message, channel, cspace, sync_ep);
			break;
		case NDO_GET_STATS64:
			LIBLCD_MSG("Calling function ndo_get_stats64");
			return ndo_get_stats64_callee(message, channel, cspace, sync_ep);
			break;
		case NDO_SET_RX_MODE:
			LIBLCD_MSG("Calling function ndo_set_rx_mode");
			return ndo_set_rx_mode_callee(message, channel, cspace, sync_ep);
			break;
		case NDO_SET_MAC_ADDRESS:
			LIBLCD_MSG("Calling function ndo_set_mac_address");
			return ndo_set_mac_address_callee(message, channel, cspace, sync_ep);
			break;
		case NDO_CHANGE_CARRIER:
			LIBLCD_MSG("Calling function ndo_change_carrier");
			return ndo_change_carrier_callee(message, channel, cspace, sync_ep);
			break;
		case SETUP:
			LIBLCD_MSG("Calling function setup");
			return setup_callee(message, channel, cspace, sync_ep);
			break;
		case VALIDATE:
			LIBLCD_MSG("Calling function validate");
			return validate_callee(message, channel, cspace, sync_ep);
			break;
	}
	return 0;

}

