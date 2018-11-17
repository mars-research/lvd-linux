/*
 * rpc.h
 *
 * Internal defs
 *
 * Copyright: University of Utah
 */
#ifndef LCD_ASYNC_RPC_TEST_H
#define LCD_ASYNC_RPC_TEST_H

#include <libfipc.h>

enum fn_type {
	NULL_INVOCATION, 
	ADD_CONSTANT, 
	ADD_NUMS, 
	ADD_3_NUMS,
	ADD_4_NUMS, 
	ADD_5_NUMS, 
	ADD_6_NUMS
};

/* must be divisible by 6... because I call 6 functions in the callee.c */
#define TRANSACTIONS 60

/* Each buffer is 2^ASYNC_RPC_BUFFER_ORDER bytes. Must be >= PAGE_SHIFT. */
#define ASYNC_RPC_EXAMPLE_BUFFER_ORDER 12

static inline
int
get_fn_type(struct fipc_message *msg)
{
	return fipc_get_flags(msg);
}

static inline
void
set_fn_type(struct fipc_message *msg, enum fn_type type)
{
	fipc_set_flags(msg, type);
}

static inline
int
test_fipc_blocking_recv_start(struct fipc_ring_channel *chnl, 
			struct fipc_message **out)
{
	int ret;
	for (;;) {
		/* Poll until we get a message or error */
		ret = fipc_recv_msg_start(chnl, out);
		if (!ret || ret != -EWOULDBLOCK)
			return ret;
		cpu_relax();
	}
}

static inline
int
test_fipc_blocking_send_start(struct fipc_ring_channel *chnl, 
			struct fipc_message **out)
{
	int ret;
	for (;;) {
		/* Poll until we get a free slot or error */
		ret = fipc_send_msg_start(chnl, out);
		if (!ret || ret != -EWOULDBLOCK)
			return ret;
		cpu_relax();
	}
}

#endif /* LCD_ASYNC_RPC_TEST_H */
