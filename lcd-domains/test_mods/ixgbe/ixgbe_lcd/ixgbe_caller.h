#ifndef __IXGBE_CALLER_H__
#define __IXGBE_CALLER_H__

#include "../ixgbe_common.h"

void dispatch_sync_loop(void);
int dispatch_async_loop(struct thc_channel *_channel,
		struct fipc_message *message,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int glue_ixgbe_init(void);
void glue_ixgbe_exit(void);
int ndo_open_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int ndo_stop_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int ndo_start_xmit_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int ndo_set_rx_mode_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int ndo_validate_addr_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int ndo_set_mac_address_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int ndo_change_mtu_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int ndo_tx_timeout_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int ndo_set_tx_maxrate_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int ndo_get_stats64_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);

int probe_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);

int remove_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int sync_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int unsync_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);

int ixgbe_service_event_schedule_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);

int trigger_dump_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);

#endif /* __IXGBE_CALLER_H__ */
