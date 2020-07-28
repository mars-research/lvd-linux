#ifndef __MEI_CALLER_H__
#define __MEI_CALLER_H__

#include "../mei_glue_helper.h"

int dispatch_async_loop(struct fipc_message *message);
int glue_mei_init(void);
void glue_mei_exit(void);
int host_is_ready_callee(struct fipc_message *_request);
int hw_is_ready_callee(struct fipc_message *_request);
int hw_reset_callee(struct fipc_message *_request);
int hw_start_callee(struct fipc_message *_request);
int hw_config_callee(struct fipc_message *_request);
int fw_status_callee(struct fipc_message *_request);
int pg_state_callee(struct fipc_message *_request);
int pg_in_transition_callee(struct fipc_message *_request);
int pg_is_enabled_callee(struct fipc_message *_request);
int intr_clear_callee(struct fipc_message *_request);
int intr_enable_callee(struct fipc_message *_request);
int intr_disable_callee(struct fipc_message *_request);
int hbuf_free_slots_callee(struct fipc_message *_request);
int hbuf_is_ready_callee(struct fipc_message *_request);
int hbuf_max_len_callee(struct fipc_message *_request);
int write_callee(struct fipc_message *_request);
int rdbuf_full_slots_callee(struct fipc_message *_request);
int read_hdr_callee(struct fipc_message *_request);
int read_callee(struct fipc_message *_request);
int irq_handler_t_callee(struct fipc_message *_request);

#endif	/* __MEI_CALLER_H__ */
