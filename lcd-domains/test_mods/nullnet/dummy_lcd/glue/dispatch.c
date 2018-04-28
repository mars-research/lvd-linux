#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../nullnet_caller.h"

#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

extern void dummy_cleanup_module(void);
int cleanup_channel_group(struct fipc_message *, struct thc_channel *);

int dispatch_async_loop(struct thc_channel *channel,
			struct fipc_message *message,
			struct glue_cspace *cspace,
			struct cptr sync_ep)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);

	switch (fn_type) {
		case NDO_INIT:
			trace(NDO_INIT);
			return ndo_init_callee(message, channel,
					cspace, sync_ep);
		case NDO_UNINIT:
			trace(NDO_UNINIT);
			ndo_uninit_callee(message, channel,
					cspace, sync_ep);
			/* wait until uninit is called */
			return -1;
		case NDO_START_XMIT:
//			trace(NDO_START_XMIT);
			/* XXX: xmit never reaches the dispatch loop */
			return ndo_start_xmit_async_bare_callee(message,
					channel, cspace, sync_ep);
		case NDO_VALIDATE_ADDR:
			trace(NDO_VALIDATE_ADDR);
			return ndo_validate_addr_callee(message,
					channel, cspace, sync_ep);
		case NDO_GET_STATS64:
			trace(NDO_GET_STATS64);
			return ndo_get_stats64_callee(message,
					channel, cspace, sync_ep);
		case NDO_SET_RX_MODE:
			trace(NDO_SET_RX_MODE);
			return ndo_set_rx_mode_callee(message,
					channel, cspace, sync_ep);
		case NDO_SET_MAC_ADDRESS:
			trace(NDO_SET_MAC_ADDRESS);
			return ndo_set_mac_address_callee(message,
					channel, cspace, sync_ep);
		case NDO_CHANGE_CARRIER:
			trace(NDO_CHANGE_CARRIER);
			return ndo_change_carrier_callee(message,
					channel, cspace, sync_ep);
		case SETUP:
			trace(SETUP);
			return setup_callee(message, channel,
					cspace, sync_ep);
		case VALIDATE:
			trace(VALIDATE);
			return validate_callee(message, channel,
					cspace, sync_ep);
		case PREP_CHANNEL:
			trace(PREP_CHANNEL);
			return prep_channel_callee(message, channel,
					cspace, sync_ep);

		case TRIGGER_EXIT:
			trace(TRIGGER_EXIT);
			/* call nullnet exit */
			dummy_cleanup_module();
			return 0;
		case TRIGGER_CLEAN:
			trace(TRIGGER_CLEAN);
			return cleanup_channel_group(message, channel);
		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;
	}
	return 0;
}
