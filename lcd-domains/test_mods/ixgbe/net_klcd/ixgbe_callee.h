#ifndef __IXGBE_CALLEE_H__
#define __IXGBE_CALLEE_H__

#include "../ixgbe_common.h"

int __pci_register_driver_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int pci_unregister_driver_callee(struct fipc_message *_request,
		struct thc_channel *_channel,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int dispatch_sync_loop(void);
int dispatch_async_loop(struct thc_channel *_channel,
		struct fipc_message *message,
		struct glue_cspace *cspace,
		struct cptr sync_ep);
int glue_ixgbe_init(void);
void glue_ixgbe_exit(void);

int probe(struct pci_dev *dev,
		struct pci_device_id *id,
		struct trampoline_hidden_args *hidden_args);

#endif /* __IXGBE_CALLEE_H__ */
