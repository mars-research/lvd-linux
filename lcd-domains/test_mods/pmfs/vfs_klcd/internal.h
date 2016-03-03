/*
 * internal.h - some internal common defs for vfs klcd
 */
#ifndef VFS_KLCD_INTERNAL_H
#define VFS_KLCD_INTERNAL_H

#include <linux/fs.h>
#include <linux/module.h>
#include <linux/backing-dev.h>

#include <liblcd/liblcd.h>
#include <liblcd/dispatch_loop.h>
#include <liblcd/glue_cspace.h>

/* MACROS -------------------------------------------------- */

/* Channel types */
#define VFS_CHANNEL_TYPE 1
#define BDI_CHANNEL_TYPE 2

/* Function flags */
#define REGISTER_FILESYSTEM 1
#define UNREGISTER_FILESYSTEM 2
#define BDI_INIT 3
#define BDI_DESTROY 4

/* STRUCT DEFS -------------------------------------------------- */

struct file_system_type_container {
	struct file_system_type file_system_type;
	cptr_t my_ref;
	cptr_t pmfs_ref;
	cptr_t pmfs_channel;
};

struct module_container {
	struct module module;
	cptr_t my_ref;
	cptr_t pmfs_ref;
};

struct backing_dev_info_container {
	struct backing_dev_info backing_dev_info;
	cptr_t my_ref;
	cptr_t pmfs_ref;
	/* no channel since pmfs doesn't implement fn ptrs */
};

/*
 * ============================================================
 * ============================================================
 */

/* VFS -------------------------------------------------- */

/*
 * Initializes vfs_callee glue code 
 */
int glue_vfs_init(cptr_t, struct dispatch_ctx *);
/*
 * Tears down vfs_callee glue code
 */
void glue_vfs_exit(void);

int register_filesystem_callee(void);
int unregister_filesystem_callee(void);

/* BDI -------------------------------------------------- */

/*
 * Initializes bdi_callee glue code 
 */
int glue_bdi_init(cptr_t, struct dispatch_ctx *);
/*
 * Tears down bdi_callee glue code
 */
void glue_bdi_exit(void);

int bdi_init_callee(void);
int bdi_destroy_callee(void);

/* CSPACES ------------------------------------------------------------ */

int glue_cap_init(void);

int glue_cap_create(struct glue_cspace **cspace);

void glue_cap_destroy(struct glue_cspace *cspace);

void glue_cap_exit(void);

int glue_cap_insert_file_system_type_type(
	struct glue_cspace *cspace, 
	struct file_system_type_container *file_system_type_container,
	cptr_t *c_out);

int glue_cap_insert_backing_dev_info_type(
	struct glue_cspace *cspace, 
	struct backing_dev_info_container *backing_dev_info_container,
	cptr_t *c_out);

int glue_cap_insert_module_type(
	struct glue_cspace *cspace, 
	struct module_container *module_container,
	cptr_t *c_out);

int glue_cap_lookup_file_system_type_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct file_system_type_container **file_system_type_container);

int glue_cap_lookup_backing_dev_info_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct backing_dev_info_container **backing_dev_info_container);

int glue_cap_lookup_module_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct module_container **module_container);

void glue_cap_remove(
	struct glue_cspace *cspace, 
	cptr_t c);

#endif /* VFS_KLCD_INTERNAL_H */
