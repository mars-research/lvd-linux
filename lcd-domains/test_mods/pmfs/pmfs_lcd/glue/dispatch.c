/*
 * dispatch.c
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include "../internal.h"

#include <lcd_config/post_hook.h>


int dispatch_fs_channel(struct lcd_sync_channel_group_item *chnl)
{
	return 0; /* no op for now */
}
