/*
 * internal.h - some internal defs for glue code, etc.
 */
#ifndef PMFS_LCD_INTERNAL_H
#define PMFS_LCD_INTERNAL_H

#include <linux/fs.h>
#include <linux/backing-dev.h>
#include <linux/module.h>

#include <libcap.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/liblcd.h>
#include <liblcd/dispatch_loop.h>

/* MACROS/FLAGS -------------------------------------------------- */

/* Channel flags */
#define PMFS_CHANNEL_TYPE 1

/* Function flags */
#define REGISTER_FS 1
#define UNREGISTER_FS 2
#define BDI_INIT 3
#define BDI_DESTROY 4

/* STRUCT DEFS -------------------------------------------------- */

struct file_system_type_container {
	struct file_system_type file_system_type;
	cptr_t my_ref;
	cptr_t vfs_ref;
	struct ipc_channel chnl;
};

struct backing_dev_info_container {
	struct backing_dev_info backing_dev_info;
	cptr_t my_ref;
	cptr_t bdi_ref;
	/* no channel since pmfs doesn't implement function ptrs */
};

struct module_container {
	struct module module;
	cptr_t my_ref;
	cptr_t vfs_ref;
};

/* FUNCTIONS -------------------------------------------------- */

int glue_vfs_init(cptr_t _vfs_channel, struct dispatch_ctx *ctx);

void glue_vfs_exit(void);

int glue_bdi_init(cptr_t _bdi_channel, struct dispatch_ctx *ctx);

void glue_bdi_exit(void);

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

#endif /* PMFS_LCD_INTERNAL_H */
