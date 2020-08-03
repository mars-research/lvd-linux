#ifndef __SKX_EDAC_CALLER_H__
#define __SKX_EDAC_CALLER_H__

dispatch_sync_loop(struct fipc_message *message);
int dispatch_async_loop(struct fipc_message *message);
int glue_skx_edac_init(void);
void glue_skx_edac_exit(void);
int get_callee(struct fipc_message *_request);
int set_callee(struct fipc_message *_request);

#endif	/* __SKX_EDAC_CALLER_H__ */