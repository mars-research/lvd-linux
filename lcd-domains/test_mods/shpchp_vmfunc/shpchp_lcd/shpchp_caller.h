#ifndef __SHPCHP_CALLER_H__
#define __SHPCHP_CALLER_H__

dispatch_sync_loop(struct fipc_message *message);
int dispatch_async_loop(struct fipc_message *message);
int glue_shpchp_init(void);
void glue_shpchp_exit(void);
int irq_handler_callee(struct fipc_message *_request);
int irq_handler_tnread_callee(struct fipc_message *_request);

#endif	/* __SHPCHP_CALLER_H__ */