#ifndef __NULLB_CALLER_H__
#define __NULLB_CALLER_H__

#include "../glue_helper.h"

void dispatch_sync_loop(void);
int dispatch_async_loop(struct fipc_message *message);
int glue_nullb_init(void);
void glue_nullb_exit(void);
int queue_rq_fn_callee(struct fipc_message *request);
int map_queue_fn_callee(struct fipc_message *request);
int init_hctx_fn_callee(struct fipc_message *request);
int softirq_done_fn_callee(struct fipc_message *request);
int open_callee(struct fipc_message *request);
int release_callee(struct fipc_message *request);
int null_exit_callee(struct fipc_message *request);
int nullbu_mmap_user(struct fipc_message *request);

#endif /* __NULLB_CALLER_H__ */
