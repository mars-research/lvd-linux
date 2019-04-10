#ifndef __NULLB_CALLEE_H__
#define __NULLB_CALLEE_H__

#include "../glue_helper.h"

struct drv_info {
	/* auxiliary dispatch channel */
	struct thc_channel *aux_chnl;
	struct glue_cspace *cspace;
	struct thc_channel_group *ch_grp;
	cptr_t sync_endpoint;
	struct list_head list;
};
int glue_blk_init(void);

void glue_blk_exit(void);
void blk_exit(struct thc_channel *channel);
int dispatch_sync_loop (void);

struct drv_info * 
add_drv(struct thc_channel_group_item *curr_item, struct glue_cspace *cspace,
        cptr_t sync_endpoint, struct thc_channel *aux_chnl);

void remove_drv(struct drv_info *fs);

struct drv_info* get_drvinfo(void);

int dispatch_async_loop(struct thc_channel *chnl,
                        struct fipc_message *msg,
                        struct glue_cspace *cspace,
                        cptr_t sync_endpoint);

void init_chnl_group(struct thc_channel_group *ch_grp);
void add_chnl_group_item(struct thc_channel_group_item *item, struct thc_channel_group *ch_grp);
void del_chnl_group_list(int chnl_id, struct thc_channel_group *ch_grp);
void remove_chnl_group_item(struct thc_channel_group_item *item);

int blk_mq_alloc_tag_set_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int blk_mq_init_queue_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int blk_mq_end_request_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int blk_mq_free_tag_set_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int blk_mq_start_request_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int blk_mq_map_queue_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int blk_queue_logical_block_size_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int blk_queue_physical_block_size_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int alloc_disk_node_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int add_disk_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int put_disk_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int del_gendisk_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int disk_node_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
//int register_blkdev_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int register_blkdev_callee(void);
int unregister_blkdev_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int dispatch_async_loop(struct thc_channel *channel, struct fipc_message *message, struct glue_cspace *cspace, cptr_t sync_ep);
int blk_cleanup_queue_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);
int lcd_register_chardev_callee(struct fipc_message *request, struct thc_channel *channel, struct glue_cspace *cspace, cptr_t sync_ep);


int glue_nullb_init(void);
void glue_nullb_exit(void);

#endif /* __NULLB_CALLEE_H__ */
