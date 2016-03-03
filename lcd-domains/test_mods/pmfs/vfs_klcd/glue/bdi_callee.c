/*
 * bdi_calle.c - callee side glue code of bdi interface
 *
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/dispatch_loop.h>
#include <liblcd/liblcd.h>
#include <linux/slab.h>
#include "../internal.h"

#include <lcd_config/post_hook.h>

/* GLOBALS -------------------------------------------------- */

//static struct dispatch_ctx *loop_ctx;
//static struct ipc_channel bdi_channel;
static struct glue_cspace *pmfs_cspace;

/* INIT/EXIT -------------------------------------------------- */

int glue_bdi_init(cptr_t bdi_chnl, struct dispatch_ctx *ctx)
{
	int ret;

	/* args ignored for now (we share channel with vfs glue) */
	//init_ipc_channel(&bdi_channel, BDI_CHANNEL_TYPE, bdi_chnl, 0);

	//loop_ctx = ctx;

	/* Add it to dispatch loop */
	//loop_add_channel(loop_ctx, &bdi_channel);

	/* Initialize pmfs data store. (This obviously doesn't generalize
	 * well. In general, the data store should be set up upon receiving
	 * a register call?) */
	ret = glue_cap_create(&pmfs_cspace);
	if (ret) {
		LIBLCD_ERR("create");
		return ret;
	}

	return 0;
}

void glue_bdi_exit(void)
{
	/*
	 * Destroy pmfs data stroe
	 */
	glue_cap_destroy(pmfs_cspace);
	/*
	 * Remove vfs channel from loop
	 */
	//loop_rm_channel(loop_ctx, &bdi_channel);
}

/* FUNCTIONS -------------------------------------------------- */

int bdi_init_callee(void)
{
	struct backing_dev_info_container *bdi_container;
	int ret;

	/*
	 * SET UP CONTAINER ----------------------------------------
	 */
	bdi_container = kzalloc(sizeof(*bdi_container), GFP_KERNEL);
	if (!bdi_container) {
		LIBLCD_ERR("kzalloc bdi container");
		ret = -ENOMEM;
		goto fail1;
	}

	ret = glue_cap_insert_backing_dev_info_type(
		pmfs_cspace,
		bdi_container,
		&bdi_container->my_ref);
	if (ret) {
		LIBLCD_ERR("dstore insert bdi container");
		goto fail2;
	}

	/* IPC UNMARSHALING ---------------------------------------- */
	
	/*
	 * Ref comes first, followed by two fields.
	 *
	 * XXX: For now, this is tailored for pmfs. (Don't worry about
	 * other unused fields.)
	 *
	 */
	bdi_container->pmfs_ref = __cptr(lcd_r1());
	bdi_container->backing_dev_info.ra_pages = lcd_r2();
	bdi_container->backing_dev_info.capabilities = lcd_r3();

	/*
	 * CALL REAL BDI_INIT ------------------------------
	 */

	ret = bdi_init(&bdi_container->backing_dev_info);
	if (ret) {
		LIBLCD_ERR("bdi init failed");
		goto fail3;
	}

	/*
	 * MARSHAL RETURN --------------------------------------------------
	 */

	/*
	 * Register fs return value
	 */
	lcd_set_r0(ret);
	/*
	 * Pass back a ref to our copy
	 */
	lcd_set_r1(cptr_val(bdi_container->my_ref));

	/* IPC REPLY -------------------------------------------------- */

	ret = lcd_sync_reply();
	if (ret) {
		LIBLCD_ERR("lcd_sync_reply");
		goto fail4;
	}

	return 0;

fail4:
	bdi_destroy(&bdi_container->backing_dev_info);
fail3:
	glue_cap_remove(pmfs_cspace, bdi_container->my_ref);
fail2:
	kfree(bdi_container);
fail1:
	return ret;
}

int bdi_destroy_callee(void)
{
	struct backing_dev_info_container *bdi_container;
	int ret;
	struct dstore_node *n;
	dptr_t ref;

	/* IPC UNMARSHALING ---------------------------------------- */

	/*
	 * We expect one ref to our copy
	 */

	ref = __cptr(lcd_r1());

	/* LOOK UP CONTAINER  ---------------------------------------- */

	ret = glue_cap_lookup_backing_dev_info_type(
		pmfs_cspace,
		ref,
		&bdi_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto out;
	}

	/* CALL REAL FUNCTION ---------------------------------------- */

	bdi_destroy(&bdi_container->backing_dev_info);

	/* CONTAINER DESTORY ---------------------------------------- */

	/*
	 * Remove from data store
	 */
	glue_cap_remove(pmfs_cspace, bdi_container->my_ref);
	/*
	 * Free container
	 */
	kfree(bdi_container);

	/* IPC REPLY -------------------------------------------------- */

	ret = 0;
	goto out;

out:
	if (lcd_sync_reply())
		LIBLCD_ERR("double fault");
	return ret;
}
