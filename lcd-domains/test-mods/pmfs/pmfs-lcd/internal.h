/*
 * internal.h - some internal defs for glue code, etc.
 */
#ifndef PMFS_LCD_INTERNAL_H
#define PMFS_LCD_INTERNAL_H

#include <lcd-domains/liblcd-config.h>

#include <linux/fs.h>
#include <linux/backing-dev.h>

#include <lcd-domains/liblcd.h>
#include <lcd-domains/dispatch_loop.h>

/* MACROS/FLAGS -------------------------------------------------- */

/* Channel flags */
#define PMFS_CHANNEL_TYPE 1

/* Data store flags */
#define STRUCT_FILE_SYSTEM_TYPE_TAG 2
#define STRUCT_BACKING_DEV_INFO_TAG 3

/* Function flags */
#define REGISTER_FS 1
#define UNREGISTER_FS 2
#define BDI_INIT 3
#define BDI_DESTROY 4

/* STRUCT DEFS -------------------------------------------------- */

struct file_system_type_container {
	struct file_system_type file_system_type;
	dptr_t my_ref;
	dptr_t vfs_ref;
	struct ipc_channel chnl;
};

struct backing_dev_info_container {
	struct backing_dev_info backing_dev_info;
	dptr_t my_ref;
	dptr_t bdi_ref;
	/* no channel since pmfs doesn't implement function ptrs */
};

/* FUNCTIONS -------------------------------------------------- */

int glue_vfs_init(cptr_t _vfs_channel, struct dispatch_ctx *ctx);

void glue_vfs_exit(void);

int glue_bdi_init(cptr_t _bdi_channel, struct dispatch_ctx *ctx);

void glue_bdi_exit(void);


#endif /* PMFS_LCD_INTERNAL_H */
