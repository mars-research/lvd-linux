#ifndef __NULLB_CALLEE_H__
#define __NULLB_CALLEE_H__

#include "../glue_helper.h"

int glue_blk_init(void);
void glue_blk_exit(void);

int blk_mq_alloc_tag_set_callee(struct fipc_message *request);
int blk_mq_init_queue_callee(struct fipc_message *request);
int blk_mq_end_request_callee(struct fipc_message *request);
int blk_mq_free_tag_set_callee(struct fipc_message *request);
int blk_mq_start_request_callee(struct fipc_message *request);
int blk_mq_map_queue_callee(struct fipc_message *request);
int blk_queue_logical_block_size_callee(struct fipc_message *request);
int blk_queue_physical_block_size_callee(struct fipc_message *request);
int alloc_disk_node_callee(struct fipc_message *request);
int add_disk_callee(struct fipc_message *request);
int put_disk_callee(struct fipc_message *request);
int del_gendisk_callee(struct fipc_message *request);
int disk_node_callee(struct fipc_message *request);
int register_blkdev_callee(struct fipc_message *request);
int unregister_blkdev_callee(struct fipc_message *request);
int blk_cleanup_queue_callee(struct fipc_message *request);
int lcd_register_chardev_callee(struct fipc_message *request);

#endif /* __NULLB_CALLEE_H__ */
