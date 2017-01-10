#ifndef __NULLNET_CALLER_H__
#define __NULLNET_CALLER_H__

#include "../glue_helper.h"

int glue_nullnet_init(struct cptr c, struct lcd_sync_channel_group *nullnet_group);
void glue_nullnet_exit(void);
int ndo_init_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int ndo_uninit_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int ndo_start_xmit_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int ndo_validate_addr_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int ndo_set_rx_mode_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int ndo_set_mac_address_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int ndo_get_stats64_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int ndo_change_carrier_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int setup_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);
int validate_callee(struct fipc_message *request, struct thc_channel *channel, struct cspace *cspace, struct cptr sync_ep);

#endif /* __NULLNET_CALLER_H__ */
