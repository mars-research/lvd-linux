#ifndef __NULLNET_CALLEE_H__
#define __NULLNET_CALLEE_H__

#include <linux/etherdevice.h>

#include "../glue_helper.h"

/* GLUE SUPPORT FUNCTIONS ---------------------------------------- */

int glue_nullnet_init(void);

void glue_nullnet_exit(void);

#ifndef CONFIG_LVD
struct trampoline_hidden_args {
	void *struct_container;
	struct glue_cspace *cspace;
	struct lcd_trampoline_handle *t_handle;
	struct thc_channel *async_chnl;
	struct cptr sync_ep;
};
#endif

/* XXX: How to determine this? */
#define SKB_HASH_BITS      8

struct skb_hash_table {
	DECLARE_HASHTABLE(skb_table, SKB_HASH_BITS);
};

int __rtnl_link_register_callee(struct fipc_message *);

int register_netdevice_callee(struct fipc_message *request);
int ether_setup_callee(struct fipc_message *request);
int eth_mac_addr_callee(struct fipc_message *request);
int eth_validate_addr_callee(struct fipc_message *request);
int free_netdev_callee(struct fipc_message *request);
int netif_carrier_off_callee(struct fipc_message *request);
int netif_carrier_on_callee(struct fipc_message *request);
int __rtnl_link_unregister_callee(struct fipc_message *request);
int rtnl_link_unregister_callee(struct fipc_message *request);
int alloc_netdev_mqs_callee(struct fipc_message *request);
int consume_skb_callee(struct fipc_message *request);

void setup_sync_callee(struct fipc_message *msg);

#endif /* __NULLNET_CALLEE_H__ */
