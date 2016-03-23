/*
 * vfs_calle.c - callee side glue code of vfs interface
 *
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/sync_ipc_poll.h>
#include <liblcd/liblcd.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include "../internal.h"

#include <lcd_config/post_hook.h>

/* GLOBALS -------------------------------------------------- */

static struct lcd_sync_channel_group *group;
static struct lcd_sync_channel_group_item vfs_channel;
static struct glue_cspace *pmfs_cspace;

/* INIT/EXIT -------------------------------------------------- */

int glue_vfs_init(cptr_t vfs_chnl, struct lcd_sync_channel_group *_group)
{
	int ret;

	/* Set up ipc channel */
	lcd_sync_channel_group_item_init(&vfs_channel, vfs_chnl, 1,
					dispatch_vfs_channel);

	group = _group;

	/* Add it to dispatch loop */
	lcd_sync_channel_group_add(group, &vfs_channel);

	/* Initialize pmfs data store. (This obviously doesn't generalize
	 * well. In general, the data store should be set up upon receiving
	 * a register call?) */
	ret = glue_cap_init();
	if (ret) {
		LIBLCD_ERR("cap init");
		goto fail1;
	}

	ret = glue_cap_create(&pmfs_cspace);
	if (ret) {
		LIBLCD_ERR("create");
		goto fail2;
	}

	return 0;

fail2:
	glue_cap_exit();
fail1:
	return ret;
}

void glue_vfs_exit(void)
{
	/*
	 * Destroy pmfs data stroe
	 */
	glue_cap_destroy(pmfs_cspace);
	/*
	 * Remove vfs channel from loop
	 */
	lcd_sync_channel_group_remove(group, &vfs_channel);

	glue_cap_exit();
}

/* FUNCTIONS -------------------------------------------------- */

int register_filesystem_callee(void)
{
	struct file_system_type_container *fs_container;
	struct module_container *module_container;
	int ret;

	/*
	 * SET UP CONTAINERS ----------------------------------------
	 */
	fs_container = kzalloc(sizeof(*fs_container), GFP_KERNEL);
	if (!fs_container) {
		LIBLCD_ERR("kzalloc fs container");
		ret = -ENOMEM;
		goto fail1;
	}

	ret = glue_cap_insert_file_system_type_type(
		pmfs_cspace,
		fs_container,
		&fs_container->my_ref);
	if (ret) {
		LIBLCD_ERR("dstore insert fs container");
		goto fail2;
	}


	module_container = kzalloc(sizeof(*module_container), GFP_KERNEL);
	if (!module_container) {
		LIBLCD_ERR("kzalloc module container");
		ret = -ENOMEM;
		goto fail3;
	}

	ret = glue_cap_insert_module_type(
		pmfs_cspace,
		module_container,
		&module_container->my_ref);
	if (ret) {
		LIBLCD_ERR("insert");
		goto fail4;
	}

	/* IPC UNMARSHALING ---------------------------------------- */
	
	/*
	 * Refs come first. 
	 *
	 * struct file_system_type
	 * -----------------------
	 *
	 * XXX: We don't bother passing fs name for now. Just hard code
	 * it to "pmfs".
	 */
	fs_container->pmfs_ref = __cptr(lcd_r1());
	fs_container->file_system_type.name = "pmfs";
	/*
	 * Get cptr to pmfs channel (the channel we will use to invoke
	 * functions directed to pmfs - like mount)
	 */
	fs_container->pmfs_channel = lcd_cr0();
	/*
	 * struct module
	 * -------------
	 *
	 * Pull out remote ref, and link with fs type
	 */
	module_container->pmfs_ref = __cptr(lcd_r2());
	fs_container->file_system_type.owner = &module_container->module;

	/*
	 * CALL REAL REGISTER_FILESYSTEM ------------------------------
	 */

	ret = register_filesystem(&fs_container->file_system_type);
	if (ret) {
		LIBLCD_ERR("register fs failed");
		goto fail5;
	}

	/*
	 * MARSHAL RETURN --------------------------------------------------
	 */

	/*
	 * Register fs return value
	 */
	lcd_set_r0(ret);
	/*
	 * Pass back a ref to our copies
	 */
	lcd_set_r1(cptr_val(fs_container->my_ref));
	lcd_set_r2(cptr_val(module_container->my_ref));
	/*
	 * Clear cr1
	 */
	lcd_set_cr0(CAP_CPTR_NULL);

	/* IPC REPLY -------------------------------------------------- */

	ret = lcd_sync_reply();
	if (ret) {
		LIBLCD_ERR("lcd_reply");
		goto fail6;
	}

	return 0;

fail6:
	if (unregister_filesystem(&fs_container->file_system_type) != 0)
		LIBLCD_ERR("double fault");
fail5:
	glue_cap_remove(pmfs_cspace, module_container->my_ref);
fail4:
	kfree(module_container);
fail3:
	glue_cap_remove(pmfs_cspace, fs_container->my_ref);
fail2:
	kfree(fs_container);
fail1:
	/* XXX: For now, we assume the grant was successful, so we need
	 * to delete the cap and free the cptr */
	lcd_cap_delete(lcd_cr0());
	lcd_cptr_free(lcd_cr0());
	/* Clear cr1 */
	lcd_set_cr0(CAP_CPTR_NULL);
	return ret;
}

int unregister_filesystem_callee(void)
{
	struct file_system_type_container *fs_container;
	struct module_container *module_container;
	int ret;
	cptr_t fs_ref, m_ref;

	/* CLEAR UNUSED SLOT ------------------------------ */
	
	/* Unlike register, we don't expect a capability. */

	/* XXX: Really need to update microkernel, otherwise this
	 * could be a vulnerability (caller could grant us something
	 * and fill up the slot). */

	lcd_cptr_free(lcd_cr0());
	lcd_set_cr0(CAP_CPTR_NULL);

	/* IPC UNMARSHALING ---------------------------------------- */

	/*
	 * We expect refs to our copies
	 */

	fs_ref = __cptr(lcd_r1());
	m_ref = __cptr(lcd_r2());

	/* LOOK UP CONTAINERS  ---------------------------------------- */

	ret = glue_cap_lookup_file_system_type_type(
		pmfs_cspace,
		fs_ref,
		&fs_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto out;
	}

	ret = glue_cap_lookup_module_type(
		pmfs_cspace,
		m_ref,
		&module_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto out;
	}

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
	glue_cap_remove(pmfs_cspace, fs_container->my_ref);
	glue_cap_remove(pmfs_cspace, module_container->my_ref);
	/*
	 * Delete cap to pmfs channel
	 */
	lcd_cap_delete(fs_container->pmfs_channel);
	/*
	 * Free containers
	 */
	kfree(fs_container);
	kfree(module_container);

	/* IPC REPLY -------------------------------------------------- */


	ret = 0;
	goto out;

out:
	lcd_set_r0(ret);
	if (lcd_sync_reply())
		LIBLCD_ERR("double fault");
	return ret;
}

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
	cptr_t ref;

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
