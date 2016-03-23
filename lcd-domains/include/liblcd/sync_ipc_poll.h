/*
 * sync_ipc_poll.h
 *
 * Poll on a group of synchronous IPC channels.
 *
 * Copyright: University of Utah
 */
#ifndef LIBLCD_SYNC_IPC_POLL_H
#define LIBLCD_SYNC_IPC_POLL_H

#include <linux/list.h>
#include <libcap.h>

struct lcd_sync_channel_group_item {
	cptr_t channel_cptr;
	struct list_head channel_list;
	int expected_cptrs;
	int (*dispatch_fn)(struct lcd_sync_channel_group_item *);
};

struct lcd_sync_channel_group {
	struct list_head channels;
	int size;
};

/**
 * lcd_sync_channel_group_item_init -- Initialize a sync poll item
 * @c: the poll item to initialize
 * @chnl: the cptr to the sync IPC channel to poll on
 * @expected_cptrs: the max number of capabilities we expect to be granted
 *                  on this channel
 * @dispatch_fn: the function to invoke when we receive a message on the 
 *               channel
 *
 * You should create a group item for every channel you want to add (poll
 * on).
 */
int lcd_sync_channel_group_item_init(struct lcd_sync_channel_group_item *c, 
				cptr_t chnl, int expected_cptrs,
				int (*dispatch_fn)(
					struct lcd_sync_channel_group_item *));
/**
 * lcd_sync_channel_group_init -- Initialize a sync poll group
 * @group: the poll group to initialize
 */
int lcd_sync_channel_group_init(struct lcd_sync_channel_group *group);
/**
 * lcd_sync_channel_group_add -- Add a poll item to the group
 */
void lcd_sync_channel_group_add(struct lcd_sync_channel_group *group,
				struct lcd_sync_channel_group_item *channel);
/**
 * lcd_sync_channel_group_remove -- Remove a poll item from the group
 *
 * Warning: there is no check to ensure the poll item is actually a member
 * of the group.
 */
void lcd_sync_channel_group_remove(struct lcd_sync_channel_group *group,
				struct lcd_sync_channel_group_item *channel);
/**
 * lcd_sync_channel_group_poll -- Poll on the channels in the group
 * @group: the channel group
 * @start: the channel we should start polling on
 * @channel: out param, the channel we received a message on
 *
 * If @start is NULL, this function starts polling with the first channel
 * in the group. Otherwise, polling starts at @start. In this manner, the
 * caller can implement a round robin poll (rather than always starting
 * at the beginning of the poll group).
 *
 * If we receive a message on a channel, @channel points to the channel.
 *
 * Returns non-zero on error. Returns -EWOULDBLOCK if we loop through all
 * of the channels and don't receive anything (including when group is
 * empty).
 */ 
int lcd_sync_channel_group_poll(struct lcd_sync_channel_group *group,
				struct lcd_sync_channel_group_item *start,
				struct lcd_sync_channel_group_item **channel);
/**
 * lcd_sync_channel_group_recv -- Receive from channels in a group
 *
 * Like poll, but instead of polling on channels, does a blocking
 * lcd_sync_recv. This allows the caller to enjoy all of the channel
 * group infrastructure, but do blocking receives.
 *
 * Returns -EIO if group is empty.
 */
int lcd_sync_channel_group_recv(struct lcd_sync_channel_group *group,
				struct lcd_sync_channel_group_item *_start,
				struct lcd_sync_channel_group_item **channel);

#endif /* LIBLCD_SYNC_IPC_POLL_H */
