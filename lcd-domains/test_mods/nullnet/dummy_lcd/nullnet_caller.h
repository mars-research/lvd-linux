#ifndef __NULLNET_CALLER_H__
#define __NULLNET_CALLER_H__

#include "../glue_helper.h"

int dispatch_async_loop(struct thc_channel *channel, struct fipc_message *request, struct glue_cspace *cspace, struct cptr sync_ep);
int glue_nullnet_init(void);
void glue_nullnet_exit(void);
int ndo_init_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep);
int ndo_uninit_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep);
int ndo_start_xmit_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep);
int ndo_validate_addr_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep);
int ndo_set_rx_mode_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep);
int ndo_set_mac_address_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep);
int ndo_get_stats64_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep);
int ndo_change_carrier_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep);
int setup_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep);
int validate_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, struct cptr sync_ep);
int prep_channel_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);

int ndo_start_xmit_noawe(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);

int dummy_return_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);

#endif /* __NULLNET_CALLER_H__ */
