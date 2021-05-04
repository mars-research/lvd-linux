#ifndef GLUE_USER_H
#define GLUE_USER_H

#include <net/sock.h>
#include <linux/can/core.h>

extern void *skb_data_pool;
extern void shared_mem_init(void);
extern void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext);

int __module_lcd_init(void);
void __module_lcd_exit(void);

struct uniqframe {
	int skbcnt;
	const struct sk_buff *skb;
	unsigned int join_rx_count;
};

struct raw_can_sock {
	struct sock sk;
	int bound;
	int ifindex;
	struct notifier_block notifier;
	int loopback;
	int recv_own_msgs;
	int fd_frames;
	int join_filters;
	int count;                 /* number of active filters */
	struct can_filter dfilter; /* default/single filter */
	struct can_filter *filter; /* pointer to filter(s) */
	can_err_mask_t err_mask;
	struct uniqframe __percpu *uniq;
};
#endif
