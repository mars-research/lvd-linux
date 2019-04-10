#ifndef __NULLB_CALLER_H__
#define __NULLB_CALLER_H__

#include "../glue_helper.h"

void dump_data(void);
void init_chnl_group(struct thc_channel_group *ch_grp);
void add_chnl_group_item(struct thc_channel_group_item *item,
		struct thc_channel_group *ch_grp);
void remove_chnl_group_item(int channel_id,  struct thc_channel_group *ch_grp);
void del_chnl_group_list(int chnl_id, struct thc_channel_group *ch_grp);
struct thc_channel* get_chnl_from_id(int channel_id, struct thc_channel_group *ch_grp);

void dispatch_sync_loop(void);
int dispatch_async_loop(struct thc_channel *channel, struct fipc_message *message, struct glue_cspace *cspace, cptr_t sync_ep);
int glue_nullb_init(void);
void glue_nullb_exit(void);
int queue_rq_fn_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int map_queue_fn_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int init_hctx_fn_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int softirq_done_fn_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int open_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int release_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int null_exit_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int nullbu_mmap_user(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);


int null_init(void);
void null_exit(void);

#endif /* __NULLB_CALLER_H__ */
