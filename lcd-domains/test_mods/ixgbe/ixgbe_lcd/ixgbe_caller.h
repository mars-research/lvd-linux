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
int probe_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);

int remove_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);

#endif /* __IXGBE_CALLER_H__ */
