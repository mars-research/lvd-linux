/*
 * bdi_calle.c - callee side glue code of bdi interface
 *
 */

#include <lcd-domains/dispatch_loop.h>
#include <lcd-domains/kliblcd.h>
#include <linux/slab.h>
#include "../internal.h"

/* GLOBALS -------------------------------------------------- */

//static struct dispatch_ctx *loop_ctx;
//static struct ipc_channel bdi_channel;
static struct dstore *pmfs_dstore;

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
	ret = lcd_dstore_init_dstore(&pmfs_dstore);
	if (ret) {
		LIBLCD_ERR("dstore init");
		return ret;
	}

	return 0;
}

void glue_bdi_exit(void)
{
	/*
	 * Destroy pmfs data stroe
	 */
	lcd_dstore_destroy(pmfs_dstore);
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

	ret = lcd_dstore_insert(pmfs_dstore,
				bdi_container,
				STRUCT_BACKING_DEV_INFO_TAG,
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
	bdi_container->pmfs_ref = __dptr(lcd_r1());
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
	lcd_set_r1(dptr_val(bdi_container->my_ref));

	/* IPC REPLY -------------------------------------------------- */

	ret = lcd_reply();
	if (ret) {
		LIBLCD_ERR("lcd_reply");
		goto fail4;
	}

	return 0;

fail4:
	bdi_destroy(&bdi_container->backing_dev_info);
fail3:
	lcd_dstore_delete(pmfs_dstore, bdi_container->my_ref);
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

	ref = __dptr(lcd_r1());

	/* LOOK UP CONTAINER  ---------------------------------------- */

	ret = lcd_dstore_get(pmfs_dstore,
			ref,
			STRUCT_BACKING_DEV_INFO_TAG,
			&n);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto out;
	}
	bdi_container = lcd_dstore_node_object(n);
	lcd_dstore_put(n);

	/* CALL REAL FUNCTION ---------------------------------------- */

	bdi_destroy(&bdi_container->backing_dev_info);

	/* CONTAINER DESTORY ---------------------------------------- */

	/*
	 * Remove from data store
	 */
	lcd_dstore_delete(pmfs_dstore, bdi_container->my_ref);
	/*
	 * Free container
	 */
	kfree(bdi_container);

	/* IPC REPLY -------------------------------------------------- */

	ret = 0;
	goto out;

out:
	if (lcd_reply())
		LIBLCD_ERR("double fault");
	return ret;
}
