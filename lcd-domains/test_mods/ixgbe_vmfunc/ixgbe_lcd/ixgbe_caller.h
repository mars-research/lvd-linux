#ifndef __IXGBE_CALLER_H__
#define __IXGBE_CALLER_H__

#include "../ixgbe_common.h"

void dispatch_sync_loop(void);
int dispatch_async_loop(struct fipc_message *message);
int glue_ixgbe_init(void);
void glue_ixgbe_exit(void);
int ndo_open_callee(struct fipc_message *_request);
int ndo_stop_callee(struct fipc_message *_request);

int ndo_start_xmit_bare_callee(struct fipc_message *_request);

int ndo_start_xmit_clean_callee(struct fipc_message *_request);

int ndo_start_xmit_callee(struct fipc_message *_request);
int ndo_set_rx_mode_callee(struct fipc_message *_request);
int ndo_validate_addr_callee(struct fipc_message *_request);
int ndo_set_mac_address_callee(struct fipc_message *_request);
int ndo_change_mtu_callee(struct fipc_message *_request);
int ndo_tx_timeout_callee(struct fipc_message *_request);
int ndo_set_tx_maxrate_callee(struct fipc_message *_request);
int ndo_get_stats64_callee(struct fipc_message *_request);

int probe_callee(struct fipc_message *_request);

int remove_callee(struct fipc_message *_request);
int sync_callee(struct fipc_message *_request);
int unsync_callee(struct fipc_message *_request);

int ixgbe_service_event_schedule_callee(struct fipc_message *_request);

int trigger_dump_callee(struct fipc_message *_request);

int poll_callee(struct fipc_message *_request);

#endif /* __IXGBE_CALLER_H__ */
