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
#include <liblcd/glue_cspace.h>

/* COMPILER: We need this for vfs types. */
#include "../../include/vfs.h"

/* COMPILER: We need this for ipc_channel type */
#include <liblcd/dispatch_loop.h>

/* CONTAINER STRUCTS -------------------------------------------------- */

/* COMPILER: It's not required to define these as named structs (you could
 * use anonymous structs if you wish). But for readability, I'm using named
 * structs in this example. */

/* COMPILER: For *every* struct type used in a projection, we generate
 * a container struct definition that has the following fields:
 *   -- the original struct
 *   -- two cptr_t fields: one for the remote reference the caller will
 *      use to refer to the callee's private copy, and one for the reference 
 *      the callee uses to refer to the caller's private copy
 *   -- any arguments to the projection (right now, these fields will
 *      always be cptr_t's to ipc channels)
 *   -- any fields ever allocated in any use of the projection (right now,
 *      these fields will always be cptr_t's to ipc channels)
 *
 * In this example, there are 3 struct types: struct fs, 
 * struct fs_operations, and struct file. One of the projections for 
 * struct fs allocates a channel and stores it in the chnl field, so we
 * add "chnl" to the fields for the container struct for struct fs. 
 *
 * One of the projections for struct fs_operations takes an argument named
 * "chnl", so we add that field to the conatiner struct. NOTE: We wrap
 * the cptr inside a struct ipc_channel, since this is the data structure
 * required for interfacing with the dispatch loop.
 */
struct fs_container {
	struct fs fs;
	cptr_t vfs_ref;
	cptr_t my_ref;
	cptr_t chnl;
};
struct fs_operations_container {
	struct fs_operations fs_operations;
	cptr_t vfs_ref;
	cptr_t my_ref;
	struct ipc_channel chnl;
};
struct file_container {
	struct file file;
	cptr_t vfs_ref;
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
