/*
 * dispatch_loop.h - definitions and helpers for setting up
 *                   the dispatch loop in the glue code
 */
#ifndef LCD_DOMAINS_DISPATCH_LOOP_H
#define LCD_DOMAINS_DISPATCH_LOOP_H

#include <linux/list.h>
#include <lcd-domains/types.h>

/**
 * This represents an ipc channel.
 *
 *  type:          the integer flag the dispatch loop should use
 *                 to select from a subset of the function labels
 *
 *  channel_cptr:  the cptr to the actual ipc channel / endpoint
 *
 *  channel_list:  the dispatch loop (context) puts channels in a list
 */
struct ipc_channel {
	int type;
	cptr_t channel_cptr;
	struct list_head channel_list;
	int expected_cptrs;
};

static inline void init_ipc_channel(struct ipc_channel *c, int type,
				cptr_t chnl, int expected_cptrs)
{
	c->type = type;
	c->channel_cptr = chnl;
	c->expected_cptrs = expected_cptrs;
	INIT_LIST_HEAD(&c->channel_list);
}

/**
 * This represents the data used by the dispatch loop.
 *
 *   channel_list: list of channels registered with the dispatch loop
 *
 * Call loop_add_channel/loop_rm_channel below to add/remove a channel
 * from the dispatch loop context (from the dispatch loop).
 */
struct dispatch_ctx {
	struct list_head channel_list;
};

static inline void init_dispatch_ctx(struct dispatch_ctx *ctx)
{
	INIT_LIST_HEAD(&ctx->channel_list);
}

static inline void loop_add_channel(struct dispatch_ctx *ctx, 
			struct ipc_channel *channel)
{
	list_add(&channel->channel_list, &ctx->channel_list);
}

static inline void loop_rm_channel(struct dispatch_ctx *ctx, 
			struct ipc_channel *channel)
{
	list_del_init(&channel->channel_list);
}

#endif /* LCD_DOMAINS_DISPATCH_LOOP_H */
