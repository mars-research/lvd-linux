/*
 * internal.h - some internal common defs for vfs klcd
 */
#ifndef VFS_KLCD_INTERNAL_H
#define VFS_KLCD_INTERNAL_H

#define VFS_CHANNEL_TYPE 1

/*
 * Initializes vfs_callee glue code 
 */
int glue_vfs_init(cptr_t, struct dispatch_ctx *);
/*
 * Tears down vfs_callee glue code
 */
void glue_vfs_exit(void);

#endif /* VFS_KLCD_INTERNAL_H */
