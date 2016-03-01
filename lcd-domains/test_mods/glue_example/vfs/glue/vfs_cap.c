/*
 * vfs_cap.c
 *
 * cspace code for callee
 */

/* HEADERS -------------------------------------------------- */

/* COMPILER: This header is always included, and MUST be included before
 * any other code (controls compilation and configuration of code). */
#include <lcd_config/pre_hook.h>

/* COMPILER: This is always included in the capabilities source. */
#include <libcap.h>

/* COMPILER: This header is always included (contains declarations and defns
 * for using the LIBLCD interface). */
#include <liblcd/liblcd.h>

/* COMPILER: We probably always need the slab header in the caps source. */
#include <linux/slab.h>

/* COMPILER: We probably need this for vfs types. */
#include "../../include/vfs.h"

/* COMPILER: Bring in container defs, etc. */
#include "vfs_internal.h"

/* COMPILER: This header always comes after all other headers (it redefines
 * certain things for the isolated environment, etc.) */
#include <lcd_config/post_hook.h>

/* ------------------------------------------------------------ */

/* COMPILER: This is always defined, per interface. */
static struct cap_type_system *vfs_libcap_type_system;

/* COMPILER: struct for internal use to store all information about a type.
 * Feel free to use anonymous types if you wish. */
struct type_ops_id {
	struct cap_type_ops ops;
	cap_type_t libcap_type;
};

/* COMPILER: There is one enum per interface, and one type value per
 * container type. */
enum vfs_type {
	VFS_TYPE_FS,
	VFS_TYPE_FS_OPERATIONS,
	VFS_TYPE_FILE,
	VFS_NR_TYPES,
};

/* REVOCATION CALLBACKS -------------------------------------------------- */

/* COMPILER: One of these is defined for each container type. Until we
 * have reason to do otherwise, these will be no-ops. */

static int fs_container_revoke(struct cspace *cspace, struct cnode *cnode,
			void *object)
{
	return 0;
}

static int fs_operations_container_revoke(struct cspace *cspace, 
					struct cnode *cnode,
					void *object)
{
	return 0;
}

static int file_container_revoke(struct cspace *cspace, 
				struct cnode *cnode,
				void *object)
{
	return 0;
}

/* DELETE CALLBACKS -------------------------------------------------- */

/* COMPILER: One of these is defined for each container type. Until we
 * have reason to do otherwise, these will be no-ops. */

static int fs_container_delete(struct cspace *cspace, struct cnode *cnode,
			void *object)
{
	return 0;
}

static int fs_operations_container_delete(struct cspace *cspace, 
					struct cnode *cnode,
					void *object)
{
	return 0;
}

static int file_container_delete(struct cspace *cspace, 
				struct cnode *cnode,
				void *object)
{
	return 0;
}

/* INTERNAL DATA -------------------------------------------------- */

/* COMPILER: One of these is defined per interface. It is important
 * that these are in the same order as the enum values in enum vfs_types. */

static struct type_ops_id vfs_libcap_type_ops[VFS_NR_TYPES] = {
	{
		{
			.name = "fs",
			.delete = fs_container_delete,
			.revoke = fs_container_revoke,
		}
	},
	{
		{
			.name = "fs_operations",
			.delete = fs_operations_container_delete,
			.revoke = fs_operations_container_revoke,
		}
	},
	{
		{
			.name = "file",
			.delete = file_container_delete,
			.revoke = file_container_revoke,
		}
	},
};

/* INTERFACE TO REST OF GLUE ---------------------------------------- */

/* COMPILER: Most of this code will be the same for each interface, just
 * a few name changes. */

int vfs_cap_init(void)
{
	int ret;
	int i;
	cap_type_t libcap_type;
	/*
	 * Alloc and init microkernel type system
	 */
	ret = cap_type_system_alloc(&vfs_libcap_type_system);
	if (ret) {
		LIBLCD_ERR("alloc vfs type system failed");
		goto fail1;
	}
	ret = cap_type_system_init(vfs_libcap_type_system);
	if (ret) {
		LIBLCD_ERR("init vfs type system failed");
		goto fail2;
	}
	/*
	 * Add types
	 */
	for (i = 0; i < VFS_NR_TYPES; i++) {

		libcap_type = cap_register_private_type(
			vfs_libcap_type_system,
			0,
			&vfs_libcap_type_ops[i].ops);
		if (libcap_type == CAP_TYPE_ERR) {
			LIBLCD_ERR("failed to register vfs cap type %s",
				vfs_libcap_type_ops[i].ops.name);
			ret = -EIO;
			goto fail3;
		}
		vfs_libcap_type_ops[i].libcap_type = libcap_type;
	}

	return 0;

fail3:
	cap_type_system_destroy(vfs_libcap_type_system);
fail2:
	cap_type_system_free(vfs_libcap_type_system);
	vfs_libcap_type_system = NULL;
fail1:
	return ret;
}

int vfs_cap_create(struct glue_cspace **cspace_out)
{
	return glue_cspace_alloc_init(vfs_libcap_type_system, cspace_out);
}

void vfs_cap_destroy(struct glue_cspace *cspace)
{
	glue_cspace_destroy_free(cspace);
}

void vfs_cap_exit(void)
{
	/*
	 * Destroy and free type system if necessary
	 */
	if (vfs_libcap_type_system) {
		cap_type_system_destroy(vfs_libcap_type_system);
		cap_type_system_free(vfs_libcap_type_system);
		vfs_libcap_type_system = NULL;
	}
}

int vfs_cap_insert_fs_type(
	struct glue_cspace *cspace, 
	struct fs_container *fs_container,
	cptr_t *c_out)
{
	return glue_cspace_insert(cspace, fs_container,  
				vfs_libcap_type_ops[VFS_TYPE_FS].libcap_type,
				c_out);
}

int vfs_cap_insert_fs_operations_type(
	struct glue_cspace *cspace, 
	struct fs_operations_container *fs_operations_container,
	cptr_t *c_out)
{
	return glue_cspace_insert(
		cspace, 
		fs_operations_container,  
		vfs_libcap_type_ops[VFS_TYPE_FS_OPERATIONS].libcap_type,
		c_out);
}

int vfs_cap_insert_file_type(
	struct glue_cspace *cspace, 
	struct file_container *file_container,
	cptr_t *c_out)
{
	return glue_cspace_insert(
		cspace, 
		file_container,  
		vfs_libcap_type_ops[VFS_TYPE_FILE].libcap_type,
		c_out);
}

int vfs_cap_lookup_fs_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct fs_container **fs_container)
{
	return glue_cspace_lookup(cspace, c, 
				vfs_libcap_type_ops[VFS_TYPE_FS].libcap_type,
				(void **)fs_container);
}

int vfs_cap_lookup_fs_operations_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct fs_operations_container **fs_operations_container)
{
	return glue_cspace_lookup(
		cspace, c, 
		vfs_libcap_type_ops[VFS_TYPE_FS_OPERATIONS].libcap_type,
		(void **)fs_operations_container);
}

int vfs_cap_lookup_file_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct file_container **file_container)
{
	return glue_cspace_lookup(
		cspace, c, 
		vfs_libcap_type_ops[VFS_TYPE_FILE].libcap_type,
		(void **)file_container);
}

void vfs_cap_remove(
	struct glue_cspace *cspace, 
	cptr_t c)
{
	glue_cspace_remove(cspace, c);
}
