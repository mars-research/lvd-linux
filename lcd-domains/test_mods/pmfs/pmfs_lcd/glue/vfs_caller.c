/*
 * vfs_caller.c - caller side of vfs interface
 */

#include <lcd_config/pre_hook.h>

#include <linux/fs.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include "../internal.h"

#include <lcd_config/post_hook.h>

/* GLOBALS -------------------------------------------------- */

static cptr_t vfs_chnl;
static struct glue_cspace *vfs_cspace;
static struct lcd_sync_channel_group *group;

/* INIT/EXIT -------------------------------------------------- */

int glue_vfs_init(cptr_t _vfs_channel, struct lcd_sync_channel_group *_group)
{
	int ret;

	/* Store a reference to the dispatch loop context, so we
	 * can dynamically add channels to the loop later. */
	group = _group;

	/* Store reference to vfs channel so we can invoke functions
	 * on it later. */
	vfs_chnl = _vfs_channel;

	/* Initialize cspace system */
	ret = glue_cap_init();
	if (ret) {
		LIBLCD_ERR("cap init");
		return ret;
	}

	/* Initialize glue cspace. */
	ret = glue_cap_create(&vfs_cspace);
	if (ret) {
		LIBLCD_ERR("glue cspace init");
		return ret;
	}

	return 0;
}

void glue_vfs_exit(void)
{
	/*
	 * Free vfs glue cspace and tear down cap system
	 */
	glue_cap_destroy(vfs_cspace);
	glue_cap_exit();
}

/* CALLER FUNCTIONS -------------------------------------------------- */

int register_filesystem(struct file_system_type *fs)
{
	struct file_system_type_container *fs_container;
	struct module_container *module_container;
	int ret;
	cptr_t endpoint;
	/*
	 * Get containers
	 */
	fs_container = container_of(fs, 
				struct file_system_type_container,
				file_system_type);
	module_container = container_of(fs->owner,
					struct module_container,
					module);
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
	lcd_sync_channel_group_item_init(&fs_container->chnl, endpoint, 0,
					dispatch_fs_channel);
	lcd_sync_channel_group_add(group, &fs_container->chnl);
	/*
	 * INSERT INTO DATA STORE ------------------------------
	 *
	 */
	ret = glue_cap_insert_file_system_type_type(
		vfs_cspace, 
		fs_container,
		&fs_container->my_ref);
	if (ret) {
		LIBLCD_ERR("insert");
		lcd_exit(ret); /* abort */
	}
	ret = glue_cap_insert_module_type(
		vfs_cspace, 
		module_container,
		&module_container->my_ref);
	if (ret) {
		LIBLCD_ERR("insert");
		lcd_exit(ret); /* abort */
	}

	/*
	 * IPC MARSHALING --------------------------------------------------
	 *
	 */
	lcd_set_r1(cptr_val(fs_container->my_ref));
	lcd_set_r2(cptr_val(module_container->my_ref));
	/*
	 * XXX: We don't even pass the name string (otherwise the callee
	 * needs to keep track of a pesky 5 byte alloc). We just hard
	 * code it on the callee side for now.
	 *
	 * Will grant cap to endpoint
	 */
	lcd_set_cr0(endpoint);
	/*
	 * IPC CALL ----------------------------------------
	 */

	lcd_set_r0(REGISTER_FS);
	ret = lcd_sync_call(vfs_chnl);
	if (ret) {
		LIBLCD_ERR("lcd_call");
		lcd_exit(ret);
	}

	/* IPC UNMARSHALING ---------------------------------------- */

	/*
	 * We expect a remote ref coming back
	 */
	fs_container->vfs_ref = __cptr(lcd_r1());
	module_container->vfs_ref = __cptr(lcd_r2());

	/* Clear capability register */
	lcd_set_cr0(CAP_CPTR_NULL);

	/*
	 * Pass back return value
	 */
	return lcd_r0();
}

int unregister_filesystem(struct file_system_type *fs)
{
	int ret;
	struct file_system_type_container *fs_container;
	struct module_container *module_container;

	fs_container = container_of(fs,
				struct file_system_type_container,
				file_system_type);
	module_container = container_of(fs->owner,
					struct module_container,
					module);

	/* IPC MARSHALING ---------------------------------------- */

	/*
	 * Pass remote refs to vfs's copies
	 */
	lcd_set_r1(cptr_val(fs_container->vfs_ref));
	lcd_set_r2(cptr_val(module_container->vfs_ref));

	/* IPC CALL ---------------------------------------- */

	lcd_set_r0(UNREGISTER_FS);
	ret = lcd_sync_call(vfs_chnl);
	if (ret) {
		LIBLCD_ERR("lcd_call");
		lcd_exit(ret);
	}

	/* POST-IPC ---------------------------------------- */

	/*
	 * Destroy pmfs channel, remove from dispatch loop
	 */
	lcd_sync_channel_group_remove(group, &fs_container->chnl);

	lcd_cap_delete(fs_container->chnl.channel_cptr);
	/*
	 * Remove fs type and module from data store
	 */
	glue_cap_remove(vfs_cspace, fs_container->my_ref);
	glue_cap_remove(vfs_cspace, module_container->my_ref);
	/*
	 * Pass back return value
	 */
	return lcd_r0();
}

int bdi_init(struct backing_dev_info *bdi)
{
	struct backing_dev_info_container *bdi_container;
	int ret;
	/*
	 * Get container
	 */
	bdi_container = container_of(bdi, 
				struct backing_dev_info_container,
				backing_dev_info);
	/*
	 * INSERT INTO DATA STORE ------------------------------
	 *
	 */
	ret = glue_cap_insert_backing_dev_info_type(
		vfs_cspace, 
		bdi_container,
		&bdi_container->my_ref);
	if (ret) {
		LIBLCD_ERR("insert");
		lcd_exit(ret); /* abort */
	}
	/*
	 * IPC MARSHALING --------------------------------------------------
	 *
	 */
	lcd_set_r1(cptr_val(bdi_container->my_ref));
	lcd_set_r2(bdi_container->backing_dev_info.ra_pages);
	lcd_set_r3(bdi_container->backing_dev_info.capabilities);
	/*
	 * IPC CALL ----------------------------------------
	 */

	lcd_set_r0(BDI_INIT);
	ret = lcd_sync_call(vfs_chnl);
	if (ret) {
		LIBLCD_ERR("lcd_call");
		lcd_exit(ret);
	}

	/* IPC UNMARSHALING ---------------------------------------- */

	/*
	 * We expect a remote ref coming back
	 */
	bdi_container->bdi_ref = __cptr(lcd_r1());

	/*
	 * Pass back return value
	 */
	return lcd_r0();
}

void bdi_destroy(struct backing_dev_info *bdi)
{
	int ret;
	struct backing_dev_info_container *bdi_container;

	bdi_container = container_of(bdi,
				struct backing_dev_info_container,
				backing_dev_info);

	/* IPC MARSHALING ---------------------------------------- */

	/*
	 * Pass remote ref to bdi's copy
	 */
	lcd_set_r1(cptr_val(bdi_container->bdi_ref));

	/* IPC CALL ---------------------------------------- */

	lcd_set_r0(BDI_DESTROY);
	ret = lcd_sync_call(vfs_chnl);
	if (ret) {
		LIBLCD_ERR("lcd_call");
		lcd_exit(ret);
	}

	/* POST-IPC ---------------------------------------- */

	/*
	 * Remove bdi from data store
	 */
	glue_cap_remove(vfs_cspace, bdi_container->my_ref);
	/*
	 * (no return value)
	 */
}
