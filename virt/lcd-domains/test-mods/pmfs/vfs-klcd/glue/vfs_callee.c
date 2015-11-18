

#include <lcd-domains/dispatch_loop.h>
#include "../internal.h"

struct dispatch_ctx *loop_ctx;
struct ipc_channel vfs_channel;

int glue_vfs_init(cptr_t vfs_chnl, struct dispatch_ctx *ctx)
{
//	int ret;

	/* Set up ipc channel */
	init_ipc_channel(&vfs_channel, VFS_CHANNEL_TYPE, vfs_chnl, 1);

	loop_ctx = ctx;

	/* Add it to dispatch loop */
	loop_add_channel(loop_ctx, &vfs_channel);

	/* Initialize minix data store */
	/* ret = lcd_dstore_init_dstore(&minix_dstore); */
	/* if (ret) { */
	/* 	LIBLCD_ERR("dstore init"); */
	/* 	return ret; */
	/* } */

	return 0;
}

void glue_vfs_exit(void)
{
//	lcd_dstore_destroy(minix_dstore);
	/*
	 * Remove channel from loop
	 */
	loop_rm_channel(loop_ctx, &vfs_channel);
}

