/*
 * dispatch.c
 *
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../nullnet_callee.h"

#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

struct glue_cspace *cspace;
cptr_t sync_endpoint;
struct thc_channel *channel;

int net_klcd_dispatch_loop(struct fipc_message *message)
{
	int ret;
	int fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
	case __RTNL_LINK_REGISTER:
		trace(__RTNL_LINK_REGISTER);
		return __rtnl_link_register_callee(message);

	case REGISTER_NETDEVICE:
		LIBLCD_MSG("Calling function register_netdevice");
		ret = register_netdevice_callee(message);
		break;
	case ETHER_SETUP:
		LIBLCD_MSG("Calling function ether_setup");
		ret = ether_setup_callee(message);
		break;
	case ETH_MAC_ADDR:
		LIBLCD_MSG("Calling function eth_mac_addr");
		ret = eth_mac_addr_callee(message);
		break;
	case ETH_VALIDATE_ADDR:
		LIBLCD_MSG("Calling function eth_validate_addr");
		ret = eth_validate_addr_callee(message);
		break;
	case FREE_NETDEV:
		LIBLCD_MSG("Calling function free_netdev");
		ret = free_netdev_callee(message);
		break;
	case NETIF_CARRIER_OFF:
		LIBLCD_MSG("Calling function netif_carrier_off");
		ret = netif_carrier_off_callee(message);
		break;
	case NETIF_CARRIER_ON:
		LIBLCD_MSG("Calling function netif_carrier_on");
		ret = netif_carrier_on_callee(message);
		break;
	case __RTNL_LINK_UNREGISTER:
		LIBLCD_MSG("Calling function __rtnl_link_unregister");
		ret = __rtnl_link_unregister_callee(message);
		break;
	case RTNL_LINK_UNREGISTER:
		LIBLCD_MSG("Calling function rtnl_link_unregister");
		ret = rtnl_link_unregister_callee(message);
		break;
	case ALLOC_NETDEV_MQS:
		LIBLCD_MSG("Calling function alloc_netdev_mqs");
		ret = alloc_netdev_mqs_callee(message);
		break;
	case CONSUME_SKB:
		//LIBLCD_MSG("Calling function consume_skb");
		//trace(CONSUME_SKB);
#ifndef NO_HASHING
		ret = consume_skb_callee(message);
#else
		ret = 0;
#endif
		break;

	default:
		LIBLCD_ERR("unexpected function label %d", fn_type);
		return -EINVAL;
	}

	return 0;
}

int net_klcd_syncipc_dispatch(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);

	switch (fn_type) {
		case SYNC_SETUP:
			setup_sync_callee(message);
			break;
	default:
		LIBLCD_ERR("%s, unexpected function label %d", __func__, fn_type);
		return -EINVAL;
	}
	return 0;
}
