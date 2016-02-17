/*
 * internal.h - some internal common defs for vfs klcd
 */
#ifndef VFS_KLCD_INTERNAL_H
#define VFS_KLCD_INTERNAL_H

#include <linux/fs.h>
#include <linux/backing-dev.h>

#include <lcd-domains/kliblcd.h>
#include <lcd-domains/dispatch_loop.h>

/* MACROS -------------------------------------------------- */

/* Channel types */
#define VFS_CHANNEL_TYPE 1
#define BDI_CHANNEL_TYPE 2

/* Function flags */
#define REGISTER_FILESYSTEM 1
#define UNREGISTER_FILESYSTEM 2
#define BDI_INIT 3
#define BDI_DESTROY 4

/* Struct tags */
#define STRUCT_FILE_SYSTEM_TYPE_TAG 2
#define STRUCT_BACKING_DEV_INFO_TAG 3

/* STRUCT DEFS -------------------------------------------------- */

struct file_system_type_container {
	struct file_system_type file_system_type;
	dptr_t my_ref;
	dptr_t pmfs_ref;
	cptr_t pmfs_channel;
};

struct backing_dev_info_container {
	struct backing_dev_info backing_dev_info;
	dptr_t my_ref;
	dptr_t pmfs_ref;
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


#endif /* VFS_KLCD_INTERNAL_H */
