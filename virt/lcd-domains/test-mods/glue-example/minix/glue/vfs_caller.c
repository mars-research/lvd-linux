/*
 * vfs_caller.c
 *
 * Caller side of the VFS interface (defined in vfs.idl).
 */

/* HEADERS -------------------------------------------------- */

/* COMPILER: This header is always included (contains declarations and defns
 * for using the LIBLCD interface). */
#include <lcd-domains/liblcd.h>

/* COMPILER: This header is always included. */
#include <lcd-domains/dispatch_loop.h>

/* COMPILER: Since this header was included in vfs.idl, the include
 * is pasted here (the compiler does not need to be smart about paths - the
 * IDL writer needs to use relative or absolute paths that should just 
 * work). */
#include "../../include/vfs.h"

#define MINIX_CHANNEL_TYPE 1

/* INIT -------------------------------------------------- */

/* COMPILER: On the *caller* side, before any functions in the vfs interface
 * are invoked, the module must invoke glue_vfs_init. (In general, it is
 * glue_<module_name>_init). glue_vfs_init is defined here. Since
 * the vfs module takes one argument (vfs_chnl), glue_vfs_init takes
 * a capability to vfs_chnl as an argument, and initializes the static
 * global below.
 *
 * COMPILER: Furthermore, glue_vfs_init initializes a data store
 * for storing objects and corresponding references that the callee (vfs
 * in this example) will use to refer to those objects.
 *
 * COMPILER: For now, assume that there is one data store for
 * every channel argument.
 *
 * NOTE: We have to use globals since C functions in the original interface
 * are not modified to take extra arguments (like a capability to the ipc
 * channel). We do not use the trampoline trick (as we do for function
 * pointers) because we can only link one function at link time with the
 * unmodified code anyway. (Yes, we could get fancy and do some dynamic 
 * linking at runtime if we really wanted to ...) */
static cptr_t vfs_chnl;
static struct dstore *vfs_dstore;

/* COMPILER: We store a reference to the dispatch loop so we can 
 * dynamically add channels to it (see e.g., register_fs below). */
static struct dispatch_ctx *loop_ctx;

int glue_vfs_init(cptr_t _vfs_channel, struct dispatch_ctx *ctx)
{
	int ret;

	/* Store a reference to the dispatch loop context, so we
	 * can dynamically add channels to the loop later. */
	loop_ctx = ctx;

	/* Store reference to vfs channel so we can invoke functions
	 * on it later. */
	vfs_chnl = _vfs_channel;

	/* Initialize data store. */
	ret = lcd_dstore_init_dstore(&vfs_dstore);
	if (ret) {
		LIBLCD_ERR("dstore init");
		return ret;
	}

	return 0;
}

/* EXIT -------------------------------------------------- */

/* This is invoked by the dispatch code / some top level code when we
 * want to tear everything down. (In principle, since this is inside
 * a domain, we can just exit and the microkernel will free up all memory
 * anyway.) */

void glue_vfs_exit(void)
{
	/* Free vfs data store. */
	lcd_dstore_destroy(vfs_dstore);
}

/* CONTAINER STRUCTS -------------------------------------------------- */

/* COMPILER: It's not required to define these as named structs (you could
 * use anonymous structs if you wish). But for readability, I'm using named
 * structs in this example. */

/* COMPILER: For *every* struct type used in a projection, we generate
 * a container struct definition that has the following fields:
 *   -- the original struct
 *   -- two dptr_t fields: one for the remote reference the caller will
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
	dptr_t vfs_ref;
	dptr_t my_ref;
	cptr_t chnl;
};
struct fs_operations_container {
	struct fs_operations fs_operations;
	dptr_t vfs_ref;
	dptr_t my_ref;
	struct ipc_channel chnl;
};
struct file_container {
	struct file file;
	dptr_t vfs_ref;
	dptr_t my_ref;
};

/* COMPILER: Create a unique tag/id for each struct type used in a projection.
 * In this case, there are three. (You don't have to give them names, but
 * I'm doing so for readability.)
 */
#define STRUCT_FS_TAG 2
#define STRUCT_FS_OPERATIONS_TAG 3
#define STRUCT_FILE_TAG 4

/* REGISTER_FS -------------------------------------------------- */

/* COMPILER: register_fs is declared in the top-level part of the vfs module
 * as an rpc function. The signature is inferred from looking at the
 * return value and arguments. Those that are projections are translated into
 * the structs that they represent.
 *
 * COMPILER: A tag (integer id) is created for register_fs. This tag should
 * be unique over all functions that use this channel. (How you will infer
 * this, I'm not sure. You will probably need to keep track of scope to
 * infer which channel should be used for a function.) Also, you don't
 * have to use a name, but I'm doing so for readability. */
#define REGISTER_FS 1
int register_fs(struct fs *fs)
{
	/* VARIABLE DECLARATIONS ---------------------------------------- */

	/* COMPILER: We need to access container struct metadata *every
	 * time* when the corresponding projection is "reachable" from
	 * the arguments. A projection is "reachable" if either conditions
	 * hold:
	 *    1 - the projection is an argument to the function
	 *    2 - the projection is referenced via a field that is part
	 *        of a *reachable* projection
	 * (Note this is a recursive definition.)
	 *
	 * In this example, projection <struct fs> and 
	 * projection <struct fs_operations> are reachable.
	 */
	int ret;
	struct fs_container *fs_container;
	struct fs_operations_container *fs_operations_container;

	/* CONTAINER INIT ---------------------------------------- */

	/* COMPILER: Both projections were marked as alloc(callee) in
	 * the scope of register_fs, so we know it's safe to use container_of 
	 * to get the containers (you could combine this with the above 
	 * variable declarations).
	 *
	 * NOTE: We probably could have inferred this from the fact that
	 * the caller is passing a pointer to struct fs, rather than
	 * a double pointer. (But then, fs_operations may be alloc(caller)
	 * or something. Furthermore, there are other functions that
	 * take a pointer to a struct fs, and we need to know which one
	 * should trigger initialization of the containers/private copies.)
	 */
	fs_container = container_of(fs,
				struct fs_container,
				fs);
	fs_operations_container = container_of(fs->fs_operations,
					struct fs_operations_container,
					fs_operations);

	/* COMPILER: The projection <struct fs> declared in the scope of
	 * register_fs allocated a channel. We create a channel here and
	 * store the capability to it in the variable used in the channel
	 * allocation - "chnl".
	 */
	ret = lcd_create_sync_endpoint(&fs_container->chnl);
	if (ret) {
		LIBLCD_ERR("lcd_create_sync_endpoint");
		lcd_exit(ret); /* abort */
	}

	/* COMPILER: The projection <struct fs_operations> takes one
	 * argument when it is instantiated - "chnl". It was instantiated
	 * with the channel we created above, so we initialize the
	 * ipc channel here, and add it to the dispatch loop.
	 */
	init_ipc_channel(&fs_operations_container->chnl,
			MINIX_CHANNEL_TYPE, 
			fs_container->chnl,
			0);

	loop_add_channel(loop_ctx, &fs_operations_container->chnl);

	/* COMPILER: Both projections were marked as alloc(callee) and
	 * are hence already allocated on this side (caller side). So, we 
	 * go ahead and insert them into the data store to get remote 
	 * references the callee will use to refer to our copy.
	 *
	 * Since this function (register_fs) uses vfs_chnl, we will
	 * use vfs_dstore to store the objects and create the remote
	 * references.
	 */
	ret = lcd_dstore_insert(vfs_dstore, 
					fs_container,
					STRUCT_FS_TAG, 
					&fs_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd_dstore_insert");
		lcd_exit(ret); /* abort */
	}
	ret = lcd_dstore_insert(vfs_dstore, 
					fs_operations_container,
					STRUCT_FS_OPERATIONS_TAG,
					&fs_operations_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd_dstore_insert");
		lcd_exit(ret); /* abort */
	}

	/* IPC MARSHALING ---------------------------------------- */

	/* COMPILER: Both projections were marked as alloc(callee), so
	 * we put our refs into registers.
	 *
	 * COMPILER: Furthermore, we marked id and size as [in], so those
	 * are marshaled.
	 *
	 * NOTE: Compiler can decide on the order these are marshaled.
	 *
	 * TODO: Use a message instead of the "global" registers.
	 */
	lcd_set_r1(dptr_val(fs_container->my_ref));
	lcd_set_r2(dptr_val(fs_operations_container->my_ref));
	lcd_set_r3(fs->id);
	lcd_set_r4(fs->size);

	/* COMPILER: projection <struct fs> allocated a channel as
	 * alloc(caller,callee), so we grant the callee a capability to
	 * the channel we created.
	 */

	lcd_set_cr1(fs_container->chnl);
	
	/* IPC CALL ---------------------------------------- */

	/* COMPILER: We use the REGISTER_FS tag here to identify the
	 * function to call.
	 */
	lcd_set_r0(REGISTER_FS);

	/* COMPILER: Do the call. register_fs is assigned to use
	 * the vfs_chnl.
	 */
	LIBLCD_MSG("minix calling register_fs in vfs");
	ret = lcd_call(vfs_chnl);
	if (ret) {
		LIBLCD_ERR("lcd_call");
		lcd_exit(ret); /* abort */
	}

	LIBLCD_MSG("minix got response from vfs (register_fs call)");

	/* IPC UNMARSHALING ---------------------------------------- */

	/* COMPILER: Both projections were marked as alloc(callee), so we
	 * expect remote references to come back. (You can decide on what
	 * order they should come back in).
	 */
	fs_container->vfs_ref = __dptr(lcd_r1());
	fs_operations_container->vfs_ref = __dptr(lcd_r2());

	/* COMPILER: projection <struct fs> had size marked as [out].
	 *
	 * TODO: May need to cast this to the right type.
	 */
	fs_container->fs.size = lcd_r3();

	/* Clear capability register */
	lcd_set_cr1(LCD_CPTR_NULL);

	/* COMPILER: Since register_fs returns a scalar value, r0 contains 
	 * the return value from the callee.
	 *
	 * TODO: May need to cast this to the right type.
	 */
	return lcd_r0();
}

/* UNREGISTER_FS -------------------------------------------------- */

#define UNREGISTER_FS 2
void unregister_fs(struct fs *fs)
{
	/* VARIABLE DECLARATIONS ---------------------------------------- */

	int ret;
	struct fs_container *fs_container;
	struct fs_operations_container *fs_operations_container;

	/* CONTAINER INIT ---------------------------------------- */

	fs_container = container_of(fs,
				struct fs_container,
				fs);
	fs_operations_container = container_of(fs->fs_operations,
					struct fs_operations_container,
					fs_operations);

	/* IPC MARSHALING ---------------------------------------- */

	/* COMPILER: We pass all remote references to projections that
	 * are reachable from the function arguments. */
	lcd_set_r1(dptr_val(fs_container->vfs_ref));
	lcd_set_r2(dptr_val(fs_operations_container->vfs_ref));

	/* IPC CALL ---------------------------------------- */

	LIBLCD_MSG("minix calling unregister_fs");

	lcd_set_r0(UNREGISTER_FS);
	ret = lcd_call(vfs_chnl);
	if (ret) {
		LIBLCD_ERR("lcd_call");
		lcd_exit(ret); /* abort */
	}

	LIBLCD_MSG("minix got response from vfs (unregister_fs call)");

	/* POST-IPC ---------------------------------------- */

	/* (No unmarshaling to do.) */

	/* COMPILER: Channel inside projection <struct fs> marked as
	 * dealloc. We first remove it from the dispatch loop, and then
	 * delete the capability (perhaps we should revoke the capability
	 * and then delete it, or we may not want to delete the
	 * capability at all - if other interfaces are using the channel).
	 *
	 * XXX: We may need to synchronize somehow with the dispatch loop
	 * so we don't crash it. In this simple example, this won't happen
	 * (the dispatch loop is hacked to exit after receiving two messages,
	 * and unregister_fs is invoked when we're exiting the module
	 * entirely, after the dispatch loop is dead). */
	loop_rm_channel(loop_ctx, &fs_operations_container->chnl);

	lcd_cap_delete(fs_operations_container->chnl.channel_cptr);

	/* COMPILER: Both projections marked as dealloc. We remove them
	 * from the data store. The unmodified code (caller) is responsible 
	 * for freeing the structs themselves since they were only marked
	 * "dealloc(callee)".
	 *
	 * Furthermore, we use vfs_dstore since it is associated with
	 * vfs_chnl. */
	lcd_dstore_delete(vfs_dstore, fs_container->my_ref);
	lcd_dstore_delete(vfs_dstore, fs_operations_container->my_ref);

	/* (Nothing to return since this is a void function.) */
}

/* (new_file not in vfs.idl, so we don't generate glue code.) */

/* (rm_file not in vfs.idl, so we don't generate glue code.) */

/* ---------------------------------------------------------------------- */

/* COMPILER: For every bare function pointer type used in the interface
 * and every function pointer passed within a struct, we generate
 * callee glue code. The dispatch loop will invoke these functions
 * just like other callee functions. */

/* NEW_FILE (function pointer) ---------------------------------------- */

void new_file_callee(void)
{
	/* VARIABLE DECLARATIONS ---------------------------------------- */

	/* COMPILER: new_file was declared as a function pointer inside
	 * projection <struct fs_operations>, so we need to get the
	 * fs_operations container so we can call the real new_file.
	 *
	 * COMPILER: Furthermore, new_file takes *one* projection.
	 * projection <struct file> is marked as alloc(caller),
	 * so we expect the real new_file to allocate a struct file
	 * when we call it, and that the struct file will already be
	 * wrapped in a container. */

	struct fs_operations_container *fs_operations_container;
	struct file *file = NULL;
	struct file_container *file_container;

	/* These are used in code below. */
	int ret;
	int new_file_ret;
	dptr_t fs_operations_ref;
	dptr_t file_remote_ref;
	dptr_t file_my_ref = LCD_DPTR_NULL;
	struct dstore_node *n;
	int id;

	/* IPC UNMARSHALING ---------------------------------------- */

	/* COMPILER: We expect two refs since there are 2 projections
	 * involved. (1 should already have been initialized,
	 * mentioned above (projection <struct fs_operations>).
	 * The 3rd - projection <struct file> is being allocated. 
	 * The caller always provides their remote ref as part of the call.)
	 *
	 * We also expect the id param to be marshaled. */

	fs_operations_ref = __dptr(lcd_r1());
	file_remote_ref = __dptr(lcd_r2());
	id = lcd_r3();

	/* BIND CONTAINERS ---------------------------------------- */

	/* Look up containers from data store using references passed
	 * by caller. */
	
	/* COMPILER: We use vfs_dstore to resolve references to
	 * our private copies because the projections we are looking up
	 * were passed to the domain on the other side connected to
	 * the vfs channel. */
	
	ret = lcd_dstore_get(vfs_dstore, fs_operations_ref,
			STRUCT_FS_OPERATIONS_TAG, &n);
	if (ret) {
		LIBLCD_ERR("lcd_dstore_get");
		lcd_exit(ret);
	}
	fs_operations_container = lcd_dstore_node_object(n);
	lcd_dstore_put(n);

	/* INVOKE NEW_FILE ---------------------------------------- */

	/* COMPILER: Since new_file returns a scalar, we store that in
	 * r0. We are expecting the unmodified code to allocate a
	 * struct file (wrapped inside a container) since the projection
	 * was marked alloc(caller). */

	LIBLCD_MSG("minix invoking real new_file");

	new_file_ret = 
		fs_operations_container->fs_operations.new_file(
			id,
			&file);

	/* COMPILER: For every object allocated by the unmodified callee
	 * code, we need to ensure it is valid.
	 *
	 * XXX: For now, we assume the pointer is valid iff it is non-null.
	 * This probably won't be true in general though. */
	if (!file)
		goto null_file; /* Return null reference */

	/* STORE FILE CONTAINER ---------------------------------------- */

	/* COMPILER: (We assume) It is safe to do container_of, insert the
	 * container into the data store, and return a remote ref to the
	 * caller. (As noted above, this may not be a valid step.) */

	file_container = container_of(file,
				struct file_container,
				file);
	file_container->vfs_ref = file_remote_ref;

	/* COMPILER: We use vfs_dstore to carry out the next
	 * operation - inserting into it - because we know the vfs
	 * domain (or some domain vfs granted rights to the channel to -
	 * they are indistinguishable for us) is the one invoking this
	 * function via IPC. */

	ret = lcd_dstore_insert(vfs_dstore, 
				file_container,
				STRUCT_FILE_TAG, 
				&file_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd_dstore_insert");
		lcd_exit(ret);
	}
	file_my_ref = file_container->my_ref;
	
	/* IPC MARSHALING ---------------------------------------- */

	/* COMPILER: projection <struct file> had id marked as [out],
	 * so we marshal that here. (We put it *after* the remote reference,
	 * to be set below ...) */
	lcd_set_r2(file->id);	
	
null_file:	

	/* COMPILER: The caller always expects a remote refernce when
	 * a projection is marked as alloc (in this case,
	 * projection <struct file>). */
	lcd_set_r1(dptr_val(file_my_ref));

	/* COMPILER: Marshal return value in r0. */
	lcd_set_r0(new_file_ret);

	/* IPC REPLY ---------------------------------------- */

	LIBLCD_MSG("minix replying to vfs (new_file return)");

	ret = lcd_reply();
	if (ret) {
		LIBLCD_ERR("lcd_reply");
		lcd_exit(ret);
	}
}

/* RM_FILE (function pointer) ---------------------------------------- */

void rm_file_callee(void)
{
	/* VARIABLE DECLARATIONS ---------------------------------------- */

	/* COMPILER: There are 2 projections involved here. */
	struct fs_operations_container *fs_operations_container;
	struct file_container *file_container;

	/* These are used in code below. */
	int ret;
	dptr_t fs_operations_ref;
	dptr_t file_ref;
	struct dstore_node *n;

	/* IPC UNMARSHALING ---------------------------------------- */

	/* COMPILER: We *do not* expect a reference to 
	 * projection <struct fs> because it is not reachable from the
	 * function arguments in the IDL spec, nor is it the container
	 * for this function. */

	fs_operations_ref = __dptr(lcd_r1());
	file_ref = __dptr(lcd_r2());

	/* GET CONTAINERS ---------------------------------------- */

	/* Look up containers from data store using references passed
	 * by caller. */
	
	/* COMPILER: We use vfs_dstore for reasons explained in
	 * new_file_callee. */
	
	ret = lcd_dstore_get(vfs_dstore, fs_operations_ref,
			STRUCT_FS_OPERATIONS_TAG, &n);
	if (ret) {
		LIBLCD_ERR("lcd_dstore_get");
		lcd_exit(ret);
	}
	fs_operations_container = lcd_dstore_node_object(n);
	lcd_dstore_put(n);

	ret = lcd_dstore_get(vfs_dstore, file_ref,
			STRUCT_FILE_TAG, &n);
	if (ret) {
		LIBLCD_ERR("lcd_dstore_get");
		lcd_exit(ret);
	}
	file_container = lcd_dstore_node_object(n);
	lcd_dstore_put(n);

	/* INVOKE RM_FILE ---------------------------------------- */

	/* COMPILER: rm_file has a void return value. */

	LIBLCD_MSG("minix invoking real rm_file");

	fs_operations_container->fs_operations.rm_file(
		&file_container->file);

	/* REMOVE FILE CONTAINER ---------------------------------------- */

	/* COMPILER: The projection <struct file> was marked as
	 * dealloc(caller), so we expect both sides to deallocate here.
	 * The callee will dealloc their private copy. We must remove it
	 * from the data store though (it's ok to do this after the
	 * container was freed). */

	lcd_dstore_delete(vfs_dstore, file_ref);

	/* IPC MARSHALING ---------------------------------------- */

	/* (Nothing to do in this example.) */

	/* IPC REPLY ---------------------------------------- */

	LIBLCD_MSG("minix replying to vfs (rm_file return)");

	ret = lcd_reply();
	if (ret) {
		LIBLCD_ERR("lcd_reply");
		lcd_exit(ret);
	}
}
