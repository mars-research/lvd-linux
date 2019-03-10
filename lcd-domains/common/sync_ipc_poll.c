/*
 * sync_ipc_poll.c
 *
 * Implementation of polling on synchronous IPC channels.
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <linux/list.h>
#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>

#include <lcd_config/post_hook.h>

#ifndef CONFIG_LVD
int lcd_sync_channel_group_item_init(struct lcd_sync_channel_group_item *c, 
				cptr_t chnl, int expected_cptrs,
				int (*dispatch_fn)(
					struct lcd_sync_channel_group_item *))
{
	c->channel_cptr = chnl;
	c->expected_cptrs = expected_cptrs;
	INIT_LIST_HEAD(&c->channel_list);
	c->dispatch_fn = dispatch_fn;

	return 0;
}

int lcd_sync_channel_group_init(struct lcd_sync_channel_group *group)
{
	INIT_LIST_HEAD(&group->channels);
	group->size = 0;

	return 0;
}

void lcd_sync_channel_group_add(struct lcd_sync_channel_group *group,
				struct lcd_sync_channel_group_item *channel)
{
	list_add(&channel->channel_list, &group->channels);
	group->size++;
}

void lcd_sync_channel_group_remove(struct lcd_sync_channel_group *group,
				struct lcd_sync_channel_group_item *channel)
{
	list_del_init(&channel->channel_list);
	group->size--;
}

static int alloc_cptrs_for_channel(struct lcd_sync_channel_group_item *c)
{
	int i;
	int ret;
	cptr_t t;
	
	if (c->expected_cptrs >= LCD_NUM_REGS - 1) {
		LIBLCD_ERR("too many cptrs");
		return -EINVAL;
	}

	for (i = 0; i < c->expected_cptrs; i++) {

		ret = lcd_cptr_alloc(&t);
		if (ret) {
			LIBLCD_ERR("alloc cptr");
			return ret;
		}
		lcd_get_utcb()->cr[i] = t;
	}
	
	return 0;
}

static void free_cptrs_for_channel(struct lcd_sync_channel_group_item *c)
{
	int i;

	for (i = 0; i < c->expected_cptrs; i++)
		lcd_cptr_free(lcd_get_utcb()->cr[i]);
}

int lcd_sync_channel_group_poll(struct lcd_sync_channel_group *group,
				struct lcd_sync_channel_group_item *_start,
				struct lcd_sync_channel_group_item **channel)
{
	int ret = -EWOULDBLOCK;
	struct lcd_sync_channel_group_item *cursor;
	struct list_head *start;

	if (_start)
		start = &_start->channel_list;
	else
		start = &group->channels;

	list_for_each_entry(cursor, start, channel_list) {
		/*
		 * Set up cptrs
		 */
		ret = alloc_cptrs_for_channel(cursor);
		if (ret)
			return ret;
		/*
		 * Try to receive
		 */
		ret = lcd_sync_poll_recv(cursor->channel_cptr);
		if (ret) {
			free_cptrs_for_channel(cursor);
			if (ret != -EWOULDBLOCK) {
				LIBLCD_ERR("error receiving on channel");
				return ret;
			} else
				continue;
		}
		/*
		 * We got a message (message contents are in global utcb)
		 */
		*channel = cursor;

		return 0;
	}

	return ret;
}

int lcd_sync_channel_group_recv(struct lcd_sync_channel_group *group,
				struct lcd_sync_channel_group_item *_start,
				struct lcd_sync_channel_group_item **channel)
{
	int ret = -EIO;
	struct lcd_sync_channel_group_item *cursor;
	struct list_head *start;

	if (_start)
		start = &_start->channel_list;
	else
		start = &group->channels;

	list_for_each_entry(cursor, start, channel_list) {
		/*
		 * Set up cptrs
		 */
		ret = alloc_cptrs_for_channel(cursor);
		if (ret)
			return ret;
		/*
		 * Try to receive
		 */
		ret = lcd_sync_recv(cursor->channel_cptr);
		if (ret) {
			free_cptrs_for_channel(cursor);
			LIBLCD_ERR("error receiving on channel");
			return ret;
		}
		/*
		 * We got a message (message contents are in global utcb)
		 */
		*channel = cursor;

		return 0;
	}

	return ret;
}

/* EXPORTS ------------------------------------------------------------ */

EXPORT_SYMBOL(lcd_sync_channel_group_item_init);
EXPORT_SYMBOL(lcd_sync_channel_group_init);
EXPORT_SYMBOL(lcd_sync_channel_group_add);
EXPORT_SYMBOL(lcd_sync_channel_group_remove);
EXPORT_SYMBOL(lcd_sync_channel_group_poll);
EXPORT_SYMBOL(lcd_sync_channel_group_recv);
#endif
