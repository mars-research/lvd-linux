#ifndef __NVME_CALLER_H__
#define __NVME_CALLER_H__

#include "../nvme_common.h"

void dispatch_sync_loop(void);
int dispatch_async_loop(struct fipc_message *message);
int glue_nvme_init(void);
void glue_nvme_exit(void);
int ndo_open_callee(struct fipc_message *_request);
int ndo_stop_callee(struct fipc_message *_request);

int poll_callee(struct fipc_message *_request);

int probe_callee(struct fipc_message *_request);

int remove_callee(struct fipc_message *_request);
int sync_callee(struct fipc_message *_request);
int unsync_callee(struct fipc_message *_request);

int nvme_service_event_schedule_callee(struct fipc_message *_request);
int msix_vector_handler_callee(struct fipc_message *_request);
int trigger_dump_callee(struct fipc_message *_request);

int queue_rq_fn_callee(struct fipc_message *request);
int map_queue_fn_callee(struct fipc_message *request);
int init_hctx_fn_callee(struct fipc_message *request);
int exit_hctx_fn_callee(struct fipc_message *request);
int init_request_fn_callee(struct fipc_message *request);
int poll_fn_callee(struct fipc_message *request);
int softirq_done_fn_callee(struct fipc_message *request);
int open_callee(struct fipc_message *request);
int release_callee(struct fipc_message *request);

int nvme_pci_reg_read32_callee(struct fipc_message *_request);
int nvme_pci_reg_write32_callee(struct fipc_message *_request);
int nvme_pci_reg_read64_callee(struct fipc_message *_request);
int nvme_pci_reset_ctrl_callee(struct fipc_message *_request);
int nvme_pci_free_ctrl_callee(struct fipc_message *_request);
int nvme_pci_post_scan_callee(struct fipc_message *_request);
int nvme_pci_submit_async_event_callee(struct fipc_message *_request);
#endif /* __NVME_CALLER_H__ */
