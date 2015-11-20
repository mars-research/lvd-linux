/*
 * vfs_caller.c - caller side of vfs interface
 */

#include <linux/fs.h>
#include <lcd-domains/liblcd.h>
#include "../internal.h"

/* GLOBALS -------------------------------------------------- */

static cptr_t vfs_chnl;
static struct dstore *vfs_dstore;
static struct dispatch_ctx *loop_ctx;

/* INIT/EXIT -------------------------------------------------- */

int glue_vfs_init(cptr_t _vfs_channel, struct dispatch_ctx *ctx)
{
	int ret;

	/* Store a reference to the dispatch loop context, so we
	 * can dynamically add channels to the loop later. */
	loop_ctx = ctx;

	/* Store reference to vfs channel so we can invoke functions
	 * on it later. */
	vfs_chnl = _vfs_channel;

	/* Initialize data store. */
	ret = lcd_dstore_init_dstore(&vfs_dstore);
	if (ret) {
		LIBLCD_ERR("dstore init");
		return ret;
	}

	return 0;
}

void glue_vfs_exit(void)
{
	/*
	 * Free vfs data store
	 */
	lcd_dstore_destroy(vfs_dstore);
}

/* CALLER FUNCTIONS -------------------------------------------------- */

int register_filesystem(struct file_system_type *fs)
{
	struct file_system_type_container *fs_container;
	int ret;
	cptr_t endpoint;
	/*
	 * Get container
	 */
	fs_container = container_of(fs, 
				struct file_system_type_container,
				file_system_type);
	/*
	 * SET UP CHANNEL ----------------------------------------
	 *
	 *
	 * Create the sync endpoint for function calls back to us (pmfs)
	 */
	ret = lcd_create_sync_endpoint(&endpoint);
	if (ret) {
		LIBLCD_ERR("lcd_create_sync_endpoint");
		lcd_exit(ret);
	}
	/*
	 * Install in dispatch loop
	 */
	init_ipc_channel(&fs_container->chnl, PMFS_CHANNEL_TYPE, endpoint, 0);
	loop_add_channel(loop_ctx, &fs_container->chnl);
	/*
	 * INSERT INTO DATA STORE ------------------------------
	 *
	 */
	ret = lcd_dstore_insert(vfs_dstore, 
				fs_container,
				STRUCT_FILE_SYSTEM_TYPE_TAG, 
				&fs_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd_dstore_insert");
		lcd_exit(ret); /* abort */
	}
	/*
	 * IPC MARSHALING --------------------------------------------------
	 *
	 */
	lcd_set_r1(dptr_val(fs_container->my_ref));
	/*
	 * XXX: We don't even pass the name string (otherwise the callee
	 * needs to keep track of a pesky 5 byte alloc). We just hard
	 * code it on the callee side for now.
	 *
	 * Will grant cap to endpoint
	 */
	lcd_set_cr1(endpoint);
	/*
	 * IPC CALL ----------------------------------------
	 */

	lcd_set_r0(REGISTER_FS);
	ret = lcd_call(vfs_chnl);
	if (ret) {
		LIBLCD_ERR("lcd_call");
		lcd_exit(ret);
	}

	/* IPC UNMARSHALING ---------------------------------------- */

	/*
	 * We expect a remote ref coming back
	 */
	fs_container->vfs_ref = __dptr(lcd_r1());

	/* Clear capability register */
	lcd_set_cr1(LCD_CPTR_NULL);

	/*
	 * Pass back return value
	 */
	return lcd_r0();
}

int unregister_filesystem(struct file_system_type *fs)
{
	int ret;
	struct file_system_type_container *fs_container;

	fs_container = container_of(fs,
				struct file_system_type_container,
				file_system_type);

	/* IPC MARSHALING ---------------------------------------- */

	/*
	 * Pass remote ref to vfs's copy
	 */
	lcd_set_r1(dptr_val(fs_container->vfs_ref));

	/* IPC CALL ---------------------------------------- */

	LIBLCD_MSG("calling unregister fs");

	lcd_set_r0(UNREGISTER_FS);
	ret = lcd_call(vfs_chnl);
	if (ret) {
		LIBLCD_ERR("lcd_call");
		lcd_exit(ret);
	}

	LIBLCD_MSG("done");

	/* POST-IPC ---------------------------------------- */

	/*
	 * Destroy pmfs channel, remove from dispatch loop
	 */
	loop_rm_channel(loop_ctx, &fs_container->chnl);

	lcd_cap_delete(fs_container->chnl.channel_cptr);
	/*
	 * Remove fs type from data store
	 */
	lcd_dstore_delete(vfs_dstore, fs_container->my_ref);
	/*
	 * Pass back return value
	 */
	return lcd_r0();
}
