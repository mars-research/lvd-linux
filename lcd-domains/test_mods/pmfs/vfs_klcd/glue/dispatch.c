/*
 * dispatch.c
 *
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/sync_ipc_poll.h>
#include <liblcd/liblcd.h>
#include "../internal.h"

#include <lcd_config/post_hook.h>

int dispatch_vfs_channel(struct lcd_sync_channel_group_item *channel)
{
	switch (lcd_r0()) {

	case REGISTER_FILESYSTEM:
		return register_filesystem_callee();
		break;

	case UNREGISTER_FILESYSTEM:
		return unregister_filesystem_callee();
		break;

	case BDI_INIT:
		return bdi_init_callee();
		break;

	case BDI_DESTROY:
		return bdi_destroy_callee();
		break;

	default:
		LIBLCD_ERR("unexpected function label %d",
			lcd_r0());
		return -EINVAL;
	}

	return 0;
}

