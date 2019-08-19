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
int bd_open_callee(struct fipc_message *request);
int bd_release_callee(struct fipc_message *request);
int bd_ioctl_callee(struct fipc_message *_request);

int fops_open_callee(struct fipc_message *_request);
int fops_release_callee(struct fipc_message *_request);
int fops_unlocked_ioctl_callee(struct fipc_message *_request);

unsigned long _lcd_copy_from_user(void *to, const void *from, unsigned long n);
unsigned long _lcd_copy_to_user(void  *to, const void *from, unsigned long n);

#endif /* __NVME_CALLER_H__ */
