#ifndef GLUE_USER_H
#define GLUE_USER_H

#include <linux/blk-mq.h>

extern void *skb_data_pool;
extern void shared_mem_init(void);
extern void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext);

int __module_lcd_init(void);
void __module_lcd_exit(void);

/*
 * Keep this for knowing the sizeof the original nullb_cmd.
 * This is never used!
 */
struct __nullb_cmd {
	struct list_head list;
	struct llist_node ll_list;
	struct call_single_data csd;
	void *rq;
	void *bio;
	unsigned int tag;
	void *nq;
	struct hrtimer timer;
};

#endif
