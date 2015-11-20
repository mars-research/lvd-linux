/*
 * vfs_calle.c - callee side glue code of vfs interface
 *
 */

#include <lcd-domains/dispatch_loop.h>
#include <lcd-domains/kliblcd.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include "../internal.h"

/* GLOBALS -------------------------------------------------- */

struct dispatch_ctx *loop_ctx;
struct ipc_channel vfs_channel;
struct dstore *pmfs_dstore;

/* INIT/EXIT -------------------------------------------------- */

int glue_vfs_init(cptr_t vfs_chnl, struct dispatch_ctx *ctx)
{
	int ret;

	/* Set up ipc channel */
	init_ipc_channel(&vfs_channel, VFS_CHANNEL_TYPE, vfs_chnl, 1);

	loop_ctx = ctx;

	/* Add it to dispatch loop */
	loop_add_channel(loop_ctx, &vfs_channel);

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

void glue_vfs_exit(void)
{
	/*
	 * Destroy pmfs data stroe
	 */
	lcd_dstore_destroy(pmfs_dstore);
	/*
	 * Remove vfs channel from loop
	 */
	loop_rm_channel(loop_ctx, &vfs_channel);
}

/* FUNCTIONS -------------------------------------------------- */

int register_filesystem_callee(void)
{
	struct file_system_type_container *fs_container;
	int ret;
	struct module *pmfs_module;

	/*
	 * SET UP CONTAINER ----------------------------------------
	 */
	fs_container = kzalloc(sizeof(*fs_container), GFP_KERNEL);
	if (!fs_container) {
		LIBLCD_ERR("kzalloc fs container");
		ret = -ENOMEM;
		goto fail1;
	}

	ret = lcd_dstore_insert(pmfs_dstore,
				fs_container,
				STRUCT_FILE_SYSTEM_TYPE_TAG,
				&fs_container->my_ref);
	if (ret) {
		LIBLCD_ERR("dstore insert fs container");
		goto fail2;
	}

	/* IPC UNMARSHALING ---------------------------------------- */
	
	/*
	 * Ref comes first. 
	 *
	 * XXX: We don't even bother passing the name for
	 * now, and just hard code it for pmfs. (Otherwise, we have to
	 * track a pesky 5 byte alloc.)
	 */
	fs_container->pmfs_ref = __dptr(lcd_r1());
	fs_container->file_system_type.name = "pmfs";
	/*
	 * Get cptr to pmfs channel (the channel we will use to invoke
	 * functions directed to pmfs - like mount)
	 */
	fs_container->pmfs_channel = lcd_cr1();
	/*
	 * XXX Hack: Look up struct module for pmfs
	 */
	mutex_lock(&module_mutex);
	pmfs_module = find_module("lcd_test_mod_pmfs_lcd");
	mutex_unlock(&module_mutex);	
	if (!pmfs_module) {
		LIBLCD_ERR("couldn't find module");
		ret = -EIO;
		goto fail3;
	}
	fs_container->file_system_type.owner = pmfs_module;

	/*
	 * CALL REAL REGISTER_FILESYSTEM ------------------------------
	 */

	ret = register_filesystem(&fs_container->file_system_type);
	if (ret) {
		LIBLCD_ERR("register fs failed");
		goto fail4;
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
	lcd_set_r1(dptr_val(fs_container->my_ref));
	/*
	 * Clear cr1
	 */
	lcd_set_cr1(LCD_CPTR_NULL);

	/* IPC REPLY -------------------------------------------------- */

	ret = lcd_reply();
	if (ret) {
		LIBLCD_ERR("lcd_reply");
		goto fail5;
	}

	return 0;

fail5:
	if (unregister_filesystem(&fs_container->file_system_type) != 0)
		LIBLCD_ERR("double fault");
fail4:
fail3:
	lcd_dstore_delete(pmfs_dstore, fs_container->my_ref);
fail2:
	kfree(fs_container);
fail1:
	/* XXX: For now, we assume the grant was successful, so we need
	 * to delete the cap and free the cptr */
	lcd_cap_delete(lcd_cr1());
	lcd_free_cptr(lcd_cr1());
	/* Clear cr1 */
	lcd_set_cr1(LCD_CPTR_NULL);
	return ret;
}

int unregister_filesystem_callee(void)
{
	struct file_system_type_container *fs_container;
	int ret;
	struct dstore_node *n;
	dptr_t ref;

	/* CLEAR UNUSED SLOT ------------------------------ */
	
	/* Unlike register, we don't expect a capability. */

	/* XXX: Really need to update microkernel, otherwise this
	 * could be a vulnerability (caller could grant us something
	 * and fill up the slot). */

	lcd_free_cptr(lcd_cr1());
	lcd_set_cr1(LCD_CPTR_NULL);

	/* IPC UNMARSHALING ---------------------------------------- */

	/*
	 * We expect one ref to our copy
	 */

	ref = __dptr(lcd_r1());

	/* LOOK UP CONTAINER  ---------------------------------------- */

	ret = lcd_dstore_get(pmfs_dstore,
			ref,
			STRUCT_FILE_SYSTEM_TYPE_TAG,
			&n);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto out;
	}
	fs_container = lcd_dstore_node_object(n);
	lcd_dstore_put(n);

	/* CALL REAL FUNCTION ---------------------------------------- */

	ret = unregister_filesystem(&fs_container->file_system_type);
	if (ret) {
		LIBLCD_ERR("unregister fs");
		goto out;
	}

	/* CONTAINER DESTORY ---------------------------------------- */

	/*
	 * Remove from data store
	 */
	lcd_dstore_delete(pmfs_dstore, fs_container->my_ref);
	/*
	 * Delete cap to pmfs channel
	 */
	lcd_cap_delete(fs_container->pmfs_channel);

	/* IPC REPLY -------------------------------------------------- */


	ret = 0;
	goto out;

out:
	lcd_set_r0(ret);
	if (lcd_reply())
		LIBLCD_ERR("double fault");
	return ret;
}
