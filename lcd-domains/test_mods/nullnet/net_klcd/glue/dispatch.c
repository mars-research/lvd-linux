/*
 * dispatch.c
 *
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../nullnet_callee.h"

#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

int dispatch_sync_loop(void)
{
	int fn_type = lcd_r0();

	switch (fn_type) {

	case __RTNL_LINK_REGISTER:
		trace(RTNL_LINK_REGISTER);
		return __rtnl_link_register_callee();
	default:
		LIBLCD_ERR("unexpected function label %d", fn_type);
		return -EINVAL;
	}

	return 0;
}


int dispatch_async_loop(struct thc_channel *channel, 
			struct fipc_message *message,
			struct glue_cspace *cspace,
			cptr_t sync_endpoint)
{
	int ret;
	int fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
	case REGISTER_NETDEVICE:
		LIBLCD_MSG("Calling function register_netdevice");
		ret = register_netdevice_callee(message, channel, cspace,
						sync_endpoint);
		break;
	case ETHER_SETUP:
		LIBLCD_MSG("Calling function ether_setup");
		ret = ether_setup_callee(message, channel, cspace,
						sync_endpoint);
		break;
	case ETH_MAC_ADDR:
		LIBLCD_MSG("Calling function eth_mac_addr");
		ret = eth_mac_addr_callee(message, channel, cspace,
						sync_endpoint);
		break;
	case ETH_VALIDATE_ADDR:
		LIBLCD_MSG("Calling function eth_validate_addr");
		ret = eth_validate_addr_callee(message, channel, cspace,
						sync_endpoint);
		break;
	case FREE_NETDEV:
		LIBLCD_MSG("Calling function free_netdev");
		ret = free_netdev_callee(message, channel, cspace,
						sync_endpoint);
		break;
	case NETIF_CARRIER_OFF:
		LIBLCD_MSG("Calling function netif_carrier_off");
		ret = netif_carrier_off_callee(message, channel, cspace,
						sync_endpoint);
		break;
	case NETIF_CARRIER_ON:
		LIBLCD_MSG("Calling function netif_carrier_on");
		ret = netif_carrier_on_callee(message, channel, cspace,
						sync_endpoint);
		break;
	case __RTNL_LINK_UNREGISTER:
		LIBLCD_MSG("Calling function __rtnl_link_unregister");
		ret = __rtnl_link_unregister_callee(message, channel, cspace,
						sync_endpoint);
		break;
	case RTNL_LINK_UNREGISTER:
		LIBLCD_MSG("Calling function rtnl_link_unregister");
		ret = rtnl_link_unregister_callee(message, channel, cspace,
						sync_endpoint);
		break;
	case ALLOC_NETDEV_MQS:
		LIBLCD_MSG("Calling function alloc_netdev_mqs");
		ret = alloc_netdev_mqs_callee(message, channel, cspace,
						sync_endpoint);
		break;
	case CONSUME_SKB:
		//LIBLCD_MSG("Calling function consume_skb");
		ret = consume_skb_callee(message, channel, cspace,
						sync_endpoint);
		break;

	default:
		LIBLCD_ERR("unexpected function label %d", fn_type);
		return -EINVAL;
	}

	return 0;
}
