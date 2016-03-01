/*
 * vfs_internal.h
 *
 * Internal vfs_cap definitions and container types.
 */
#ifndef LCD_VFS_INTERNAL_H
#define LCD_VFS_INTERNAL_H

/* COMPILER: We need this for cptr_t and cspace type defs. */
#include <libcap.h>

/* COMPILER: We need this for glue cspaces. */
#include <libcap/glue_cspace.h>

/* COMPILER: We need this for vfs types. */
#include "../../include/vfs.h"

/* CONTAINER STRUCTS -------------------------------------------------- */

struct fs_container {
	struct fs fs;
	cptr_t minix_ref;
	cptr_t my_ref;
	cptr_t chnl;
};
struct fs_operations_container {
	struct fs_operations fs_operations;
	cptr_t minix_ref;
	cptr_t my_ref;
	cptr_t chnl;
};
struct file_container {
	struct file file;
	cptr_t minix_ref;
	cptr_t my_ref;
};

int vfs_cap_init(void);

int vfs_cap_create(struct glue_cspace **cspace);

void vfs_cap_destroy(struct glue_cspace *cspace);

void vfs_cap_exit(void);

int vfs_cap_insert_fs_type(
	struct glue_cspace *cspace, 
	struct fs_container *fs_container,
	cptr_t *c_out);

int vfs_cap_insert_fs_operations_type(
	struct glue_cspace *cspace, 
	struct fs_operations_container *fs_operations_container,
	cptr_t *c_out);

int vfs_cap_insert_file_type(
	struct glue_cspace *cspace, 
	struct file_container *file_container,
	cptr_t *c_out);

int vfs_cap_lookup_fs_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct fs_container **fs_container);

int vfs_cap_lookup_fs_operations_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct fs_operations_container **fs_operations_container);

int vfs_cap_lookup_file_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct file_container **file_container);

void vfs_cap_remove(
	struct glue_cspace *cspace, 
	cptr_t c);


#endif /* LCD_VFS_INTERNAL_H */
