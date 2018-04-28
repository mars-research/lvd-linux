#ifndef __NULLNET_CALLEE_H__
#define __NULLNET_CALLEE_H__

enum dispatch_t {
	REGISTER_NETDEVICE,
	ETHER_SETUP,
	ETH_MAC_ADDR,
	ETH_VALIDATE_ADDR,
	FREE_NETDEV,
	NETIF_CARRIER_OFF,
	NETIF_CARRIER_ON,
	__RTNL_LINK_REGISTER,
	__RTNL_LINK_UNREGISTER,
	RTNL_LINK_UNREGISTER,
	ALLOC_NETDEV_MQS,
	CONSUME_SKB,
	NDO_INIT,
	NDO_UNINIT,
	NDO_START_XMIT,
	NDO_VALIDATE_ADDR,
	NDO_SET_RX_MODE,
	NDO_SET_MAC_ADDRESS,
	NDO_GET_STATS64,
	NDO_CHANGE_CARRIER,
	SETUP,
	VALIDATE,
	SETUP
};
int register_netdevice_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int ether_setup_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int eth_mac_addr_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int eth_validate_addr_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int free_netdev_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int netif_carrier_off_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int netif_carrier_on_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int __rtnl_link_register_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int __rtnl_link_unregister_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int rtnl_link_unregister_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int alloc_netdev_mqs_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int consume_skb_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
void dispatch_loop();

#endif /* __NULLNET_CALLEE_H__ */
