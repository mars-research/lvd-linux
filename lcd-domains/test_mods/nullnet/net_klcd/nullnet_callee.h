#ifndef __NULLNET_CALLEE_H__
#define __NULLNET_CALLEE_H__

#include <linux/etherdevice.h>

#include "../glue_helper.h"

/* GLUE SUPPORT FUNCTIONS ---------------------------------------- */

int glue_nullnet_init(void);

void glue_nullnet_exit(void);

int dispatch_sync_net_channel(void);

int dispatch_async_net_channel(struct thc_channel *chnl,
			struct fipc_message *msg,
			struct cspace *cspace,
			cptr_t sync_endpoint);



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
void dispatch_loop(void);

#endif /* __NULLNET_CALLEE_H__ */
