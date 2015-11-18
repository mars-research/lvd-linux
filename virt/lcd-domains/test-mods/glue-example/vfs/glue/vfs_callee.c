/*
 * vfs_callee.c
 *
 * Callee side of the VFS interface (defined in vfs.idl).
 */

/* HEADERS -------------------------------------------------- */

/* COMPILER: This header is always included (contains declarations and defns
 * for using the LIBLCD interface). */
#include <lcd-domains/liblcd.h>

/* COMPILER: This is always included. */
#include <lcd-domains/dispatch_loop.h>

/* COMPILER: We probably always need the slab header. */
#include <linux/slab.h>

/* COMPILER: We need this for the trampolines. */
#include <lcd-domains/trampoline.h>

/* COMPILER: Since this header was included in vfs.idl, the include
 * is pasted here (the compiler does not need to be smart about paths - the
 * IDL writer needs to use relative or absolute paths that should just 
 * work). */
#include "../../include/vfs.h"

/* INIT -------------------------------------------------- */

/* (Init is at bottom of file in this case.) */

/* COMPILER: We need to figure out what to do in general. In this example,
 * I will use just one data store (for minix, the caller), and I'll just
 * init the data store in the init function. In general, we
 * may want to use seL4-style badges. The caller code could create a new
 * data store for each badge it sees (each badge roughly corresponds to
 * a domain id). */
static struct dstore *minix_dstore;

/* COMPILER: It's possible, but maybe unlikely, that the callee code
 * will dynamically add channels to the dispatch loop. We store a
 * reference to it. (This is also convenient for init/teardown to
 * add/remove the main channel to the dispatch loop.)
 */
static struct dispatch_ctx *loop_ctx;

/* CONTAINER STRUCTS -------------------------------------------------- */

/* COMPILER: See notes in vfs_caller.c for projections. */

struct fs_container {
	struct fs fs;
	dptr_t minix_ref;
	dptr_t my_ref;
	cptr_t chnl;
};
struct fs_operations_container {
	struct fs_operations fs_operations;
	dptr_t minix_ref;
	dptr_t my_ref;
	cptr_t chnl;
};
struct file_container {
	struct file file;
	dptr_t minix_ref;
	dptr_t my_ref;
};

#define STRUCT_FS_TAG 2
#define STRUCT_FS_OPERATIONS_TAG 3
#define STRUCT_FILE_TAG 4

/* TRAMPOLINE CONTAINERS ---------------------------------------- */

/* TODO: This part may not be valid for all uses of function pointers.
 * It assumes the caller passes a pointer to a function that it defines
 * (rather than the callee). Furthermore, we assume the callee that
 * receives the function pointer doesn't try to do pointer comparisons
 * with other function pointers it has received (so we can allocate
 * a new trampoline for every function pointer we receive). */

/* COMPILER: For every function pointer used in a projection, and
 * for every bare function pointer passed as an argument to a function,
 * we create a trampoline. Each trampoline is allocated inside a
 * a container of this form:
 *
 *    struct trampoline_container {
 *       struct some_other_container *c;
 *       struct dstore *d;
 *       char func[0];
 *    };
 *
 * c points to some other container that has information the trampoline
 * will use to translate the function invocation into IPC.
 *
 * d points to a data store that the glue code should use to store
 * any new objects it has to create (see new_file).
 *
 * func contains the first byte of the first instruction inside the
 * trampoline.
 *
 * The callee allocates the trampolines in the glue code that receives
 * the function pointer (either bare or inside a struct).
 *
 * In this example, there are 2 function pointers used inside
 * projection <struct fs_operations>. *Since* the function pointers are
 * declared inside this projection, we have a pointer to that container
 * stored in the trampoline container.
 *
 * The following named struct types are not required, but improve
 * readability. */

struct new_file_hidden_args {
	struct fs_operations_container *fs_operations_container;
	struct dstore *dstore;
	struct lcd_trampoline_handle *t_handle;
};

struct rm_file_hidden_args {
	struct fs_operations_container *fs_operations_container;
	struct dstore *dstore;
	struct lcd_trampoline_handle *t_handle;
};

/* FUNCTION POINTERS -------------------------------------------------- */

/* COMPILER: We generate a trampoline and caller glue code for each function 
 * pointer (see above). */

/* NEW_FILE -------------------------------------------------- */

/* COMPILER: This stub is called by the trampoline (see below).
 * It always has the same signature - plus the extra arguments:
 *   1 - the pointer to the container struct
 *   2 - the pointer to the data store to use
 *
 * Here, the container struct is struct fs_operations_container.
 *
 * COMPILER: In addition, we generate a tag for the function to
 * identify it. This somehow needs to be unique over all the tags
 * used over the channel we invoke this on. */
#define NEW_FILE 1
int  noinline
new_file(int id, struct file **file_out,
	struct fs_operations_container *fs_operations_container,
	struct dstore *dstore)
{
	/* VARIABLE DECLARATIONS ------------------------------ */

	/* COMPILER: There are 2 projections involved.
	 * projection <struct fs_operations> is already available
	 * as an argument. */

	struct file_container *file_container;

	int ret;

	/* CALLER ALLOC STRUCT FILE ------------------------------ */

	/* COMPILER: The projection <struct file> is marked as
	 * alloc(caller), so we do that here before the function
	 * invocation (so we can pass our ref to the callee). */

	file_container = kzalloc(sizeof(*file_container), GFP_KERNEL);
	if (!file_container) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1); /* abort */
	}

	/* COMPILER: We insert the file container into the data store
	 * we were provided with. */
	
	ret = lcd_dstore_insert(dstore, file_container,
				STRUCT_FILE_TAG, 
				&file_container->my_ref);
	if (ret) {
		LIBLCD_ERR("dstore insert");
		lcd_exit(-1); /* abort */
	}

	/* IPC MARSHALING ---------------------------------------- */

	/* COMPILER: We always send the remote ref to the struct that
	 * contains this function pointer (so the callee can resolve
	 * to the real function pointer). */
	lcd_set_r1(dptr_val(fs_operations_container->minix_ref));

	/* COMPILER: projection <struct file> is marked as alloc, so we
	 * pass our ref here. */
	lcd_set_r2(dptr_val(file_container->my_ref));

	/* COMPILER: We marshal the scalar id arg. */
	lcd_set_r3(id);

	/* IPC CALL ---------------------------------------- */

	/* COMPILER: We use the NEW_FILE function label. */
	lcd_set_r0(NEW_FILE);

	/* COMPILER: Since new_file was declared inside
	 * projection <struct fs_operations>, by default we use the
	 * channel that was used to instantiate that projection. */

	LIBLCD_MSG("vfs calling new_file (dest = minix)");

	ret = lcd_call(fs_operations_container->chnl);
	if (ret) {
		LIBLCD_ERR("lcd call");
		lcd_exit(-1);
	}

	/* IPC UNMARSHALING ---------------------------------------- */

	/* COMPILER: projection <struct file> was marked as alloc, so
	 * we expect a remote ref here from the callee. */
	file_container->minix_ref = __dptr(lcd_r1());

	/* COMPILER: id on struct file marked as [out], so we expect that
	 * next. */
	file_container->file.id = lcd_r2();

	/* COMPILER: struct file marked as [out], so we store in the
	 * function pointer arg. (Are we relying on the idl writer to
	 * do the right thing? Will we need to do any checking when we
	 * compile?)  */
	*file_out = &file_container->file;

	/* COMPILER: r0 contains the return value from the callee. */
	return lcd_r0();
}

LCD_TRAMPOLINE_DATA(new_file_trampoline);
int 
LCD_TRAMPOLINE_LINKAGE(new_file_trampoline)
new_file_trampoline(int id, struct file **file_out)
{
	int (*volatile new_filep)(int, struct file **,
				struct fs_operations_container *,
				struct dstore *);
	struct new_file_hidden_args *hidden_args;

	LCD_TRAMPOLINE_PROLOGUE(hidden_args, new_file_trampoline);

	new_filep = new_file;

	return new_filep(id, file_out,
			hidden_args->fs_operations_container,
			hidden_args->dstore);
}

/* RM_FILE -------------------------------------------------- */

#define RM_FILE 2
void noinline
rm_file(struct file *file,
	struct fs_operations_container *fs_operations_container,
	struct dstore *dstore)
{
	/* VARIABLE DECLARATIONS ------------------------------ */

	/* COMPILER: There are 2 projections involved. */
	
	struct file_container *file_container;

	int ret;

	/* CONTAINER INIT ---------------------------------------- */

	/* COMPILER: There is 1 projection reachable from the arguments. */

	file_container = container_of(file,
				struct file_container,
				file);

	/* IPC MARSHALING ---------------------------------------- */

	/* COMPILER: We always pass a ref to the projection that contains
	 * this function pointer. */
	lcd_set_r1(dptr_val(fs_operations_container->minix_ref));

	/* COMPILER: There is only *one* projection reachable from the
	 * arguments. */
	lcd_set_r2(dptr_val(file_container->minix_ref));

	/* IPC CALL ---------------------------------------- */

	/* COMPILER: Use RM_FILE tag. */
	lcd_set_r0(RM_FILE);

	/* COMPILER: rm_file was defined inside projection fs_operations,
	 * so we use its channel. */

	LIBLCD_MSG("vfs calling rm_file (dest = minix)");
	ret = lcd_call(fs_operations_container->chnl);
	if (ret) {
		LIBLCD_ERR("lcd_call");
		lcd_exit(ret); /* abort */
	}

	/* IPC UNMARSHALING ---------------------------------------- */

	/* (None in this case.) */

	/* FREE FILE CONTAINER ---------------------------------------- */

	/* COMPILER: projection <struct file> marked as dealloc(caller),
	 * so we remove our copy from the data store and free it. */
	lcd_dstore_delete(dstore, file_container->my_ref);
	kfree(file_container);

	/* ---------------------------------------- */

	/* COMPILER: Function is void, so no return status in r0. */
	return;
}

LCD_TRAMPOLINE_DATA(rm_file_trampoline);
void 
LCD_TRAMPOLINE_LINKAGE(rm_file_trampoline)
rm_file_trampoline(struct file *file)
{
	void (*volatile rm_filep)(struct file *,
				struct fs_operations_container *,
				struct dstore *);
	struct new_file_hidden_args *hidden_args;

	LCD_TRAMPOLINE_PROLOGUE(hidden_args, rm_file_trampoline);

	rm_filep = rm_file;

	rm_filep(file,
		hidden_args->fs_operations_container,
		hidden_args->dstore);
}

/* CALLEE CODE -------------------------------------------------- */

/* COMPILER: See notes in vfs_caller.c for new_file_callee, etc. for
 * more info about callee code generation. */

/* REGISTER_FS -------------------------------------------------- */

void register_fs_callee(void)
{
	/* VARIABLE DECLARATIONS ---------------------------------------- */

	/* COMPILER: There are two projections involved. */
	struct fs_container *fs_container;
	struct fs_operations_container *fs_operations_container;

	/* COMPILER: projection <struct fs_operations> is marked as
	 * alloc(callee) and it contains function pointers, so we
	 * will allocate trampolines for each one here. */
	struct new_file_hidden_args *new_file_hidden_args;
	struct rm_file_hidden_args *rm_file_hidden_args;

	/* These are used below. */
	int ret;
	int register_fs_ret;
	dptr_t fs_minix_ref;
	dptr_t fs_operations_minix_ref;

	/* CONTAINER INIT ---------------------------------------- */

	/* COMPILER: In general, this section may need to be half way
	 * through "ipc unmarshaling" (i.e., unmarshal remote refs,
	 * but wait to unmarshal other data until we have initialized
	 * the containers). */

	/* COMPILER: Two projections were marked as alloc(callee). We 
	 * allocate the containers here, and insert them into the
	 * data store.
	 *
	 * XXX: In general, there will be some kind of logic for determing
	 * which data store to use, or even setting up a new data store.
	 * We can discuss more at some point. For this simple example,
	 * the glue just assumes the caller is minix, and uses the global. */

	fs_container = kzalloc(sizeof(*fs_container), GFP_KERNEL);
	if (!fs_container) {
		LIBLCD_ERR("kzalloc fs container");
		lcd_exit(-1); /* abort */
	}
	ret = lcd_dstore_insert(minix_dstore,
				fs_container,
				STRUCT_FS_TAG,
				&fs_container->my_ref);
	if (ret) {
		LIBLCD_ERR("dstore insert fs container");
		lcd_exit(-1);
	}

	fs_operations_container = kzalloc(sizeof(*fs_operations_container),
					GFP_KERNEL);
	if (!fs_operations_container) {
		LIBLCD_ERR("kzalloc fs ops container");
		lcd_exit(-1); /* abort */
	}
	ret = lcd_dstore_insert(minix_dstore,
				fs_operations_container,
				STRUCT_FS_OPERATIONS_TAG,
				&fs_operations_container->my_ref);
	if (ret) {
		LIBLCD_ERR("dstore insert");
		lcd_exit(-1);
	}

	/* COMPILER: Link the fs ops to the fs. */
	fs_container->fs.fs_operations = &fs_operations_container->fs_operations;

	/* TRAMPOLINES ---------------------------------------- */

	/* COMPILER: This section needs to come after container
	 * allocation. */
	
	/* COMPILER: As mentioned above, we need to allocate the trampolines
	 * here, and install them in the struct fs_operations. */

	/* alloc */
	new_file_hidden_args = kzalloc(sizeof(*new_file_hidden_args),
				GFP_KERNEL);
	if (!new_file_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}
	/* dup trampoline */
	new_file_hidden_args->t_handle = 
		LCD_DUP_TRAMPOLINE(new_file_trampoline);
	if (!new_file_hidden_args->t_handle) {
		LIBLCD_ERR("dup trampoline");
		lcd_exit(-1);
	}

	/* Store hidden args above trampoline */
	new_file_hidden_args->t_handle->hidden_args = new_file_hidden_args;
	/* container */
	new_file_hidden_args->fs_operations_container = 
		fs_operations_container;
	/* data store */
	new_file_hidden_args->dstore = minix_dstore;
	/* install trampoline in fs ops */
	fs_operations_container->fs_operations.new_file =
		LCD_HANDLE_TO_TRAMPOLINE(new_file_hidden_args->t_handle);

	/* alloc */
	rm_file_hidden_args = kzalloc(sizeof(*rm_file_hidden_args),
				GFP_KERNEL);
	if (!rm_file_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		lcd_exit(-1);
	}
	/* dup trampoline */
	rm_file_hidden_args->t_handle = 
		LCD_DUP_TRAMPOLINE(rm_file_trampoline);
	if (!rm_file_hidden_args->t_handle) {
		LIBLCD_ERR("dup trampoline");
		lcd_exit(-1);
	}
	/* Store hidden args above trampoline */
	rm_file_hidden_args->t_handle->hidden_args = rm_file_hidden_args;
	/* container */
	rm_file_hidden_args->fs_operations_container = fs_operations_container;
	/* data store */
	rm_file_hidden_args->dstore = minix_dstore;
	/* install trampoline in fs ops */
	fs_operations_container->fs_operations.rm_file =
		LCD_HANDLE_TO_TRAMPOLINE(rm_file_hidden_args->t_handle);

	/* IPC UNMARSHALING ---------------------------------------- */

	/* COMPILER: There are two projections passed, so we expect
	 * 2 remote references (to minix's copies) here. */

	fs_minix_ref = __dptr(lcd_r1());
	fs_operations_minix_ref = __dptr(lcd_r2());

	fs_container->minix_ref = fs_minix_ref;
	fs_operations_container->minix_ref = fs_operations_minix_ref;
	
	/* COMPILER: The projection <struct fs> fields "id" and "size"
	 * were marked as [in], so we unmarshal them here. */

	fs_container->fs.id = lcd_r3();
	fs_container->fs.size = lcd_r4();

	/* COMPILER: A channel was allocated inside the 
	 * projection <struct fs> in the IDL, so we expect a capability
	 * to the channel here. */

	fs_container->chnl = lcd_cr1();
	
	/* COMPILER: projection <struct fs_operations> takes a capability
	 * to a channel as an argument, so we expect that here as
	 * well. (Technically, this should be in a separate register
	 * since the capability stored in the fs container may not be
	 * the same.) */

	fs_operations_container->chnl = lcd_cr1();

	/* Clear cr1 */
	lcd_set_cr1(LCD_CPTR_NULL);

	/* INVOKE REGISTER_FS ---------------------------------------- */

	register_fs_ret = register_fs(&fs_container->fs);

	/* IPC MARSHALING ---------------------------------------- */

	/* COMPILER: Marshal scalar return value. */
	lcd_set_r0(register_fs_ret);

	/* COMPILER: There are two projections that were marked as
	 * alloc, so we pass back our reference here. */

	lcd_set_r1(dptr_val(fs_container->my_ref));
	lcd_set_r2(dptr_val(fs_operations_container->my_ref));

	/* COMPILER: size was marked as [out] in projection <struct fs>, so
	 * we pass that back here. */

	lcd_set_r3(fs_container->fs.size);

	/* IPC REPLY -------------------------------------------------- */

	LIBLCD_MSG("vfs replying to register_fs caller");
	ret = lcd_reply();
	if (ret) {
		LIBLCD_ERR("lcd_reply");
		lcd_exit(-1);
	}
}

/* UNREGISTER_FS -------------------------------------------------- */

void unregister_fs_callee(void)
{
	/* VARIABLE DECLARACTIONS ---------------------------------------- */

	/* COMPILER: There are 2 projections involved. */

	struct fs_container *fs_container;
	struct fs_operations_container *fs_operations_container;

	/* These are used below. */
	int ret;
	struct dstore_node *n;
	dptr_t fs_minix_ref;
	dptr_t fs_operations_minix_ref;
	struct new_file_hidden_args *new_file_hidden_args;
	struct rm_file_hidden_args *rm_file_hidden_args;

	/* IPC UNMARSHALING ---------------------------------------- */

	/* COMPILER: There are 2 projections reachable from the arguments
	 * in the IDL spec, so we expect refs here. */

	fs_minix_ref = __dptr(lcd_r1());
	fs_operations_minix_ref = __dptr(lcd_r2());

	/* CONTAINER INIT ---------------------------------------- */

	/* COMPILER: The projections aren't marked as alloc, so we look them
	 * up (we expect they already exist on our side). */

	ret = lcd_dstore_get(minix_dstore,
			fs_minix_ref,
			STRUCT_FS_TAG,
			&n);
	if (ret) {
		LIBLCD_ERR("lookup");
		lcd_exit(-1);
	}
	fs_container = lcd_dstore_node_object(n);
	lcd_dstore_put(n);

	ret = lcd_dstore_get(minix_dstore,
			fs_operations_minix_ref,
			STRUCT_FS_OPERATIONS_TAG,
			&n);
	if (ret) {
		LIBLCD_ERR("lookup");
		lcd_exit(-1);
	}
	fs_operations_container = lcd_dstore_node_object(n);
	lcd_dstore_put(n);

	/* INVOKE UNREGISTER_FS ---------------------------------------- */

	unregister_fs(&fs_container->fs);

	/* CONTAINER DESTORY ---------------------------------------- */

	/* COMPILER: Both projections were marked as dealloc(callee), so
	 * we do that here (unregister_fs didn't do any dealloc). */

	/* COMPILER: First, we remove them from the data store. */
	lcd_dstore_delete(minix_dstore, fs_container->my_ref);
	lcd_dstore_delete(minix_dstore, fs_operations_container->my_ref);
	
	/* COMPILER: Second, we delete any capabilities from the cspace
	 * that are stored in the container.
	 *
	 * XXX: In general, fs_operations_container's chnl is different
	 * from fs_container's. Here, they are the same, so we only
	 * invoke delete once. */
	lcd_cap_delete(fs_container->chnl);

	/* COMPILER: Third, we free any trampolines and hidden args
	 * that were created for the projections we are destroying. In this 
	 * case, two trampolines were created for 
	 * projection <struct fs_operations>. */
	new_file_hidden_args = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(
		fs_operations_container->fs_operations.new_file);
	rm_file_hidden_args = LCD_TRAMPOLINE_TO_HIDDEN_ARGS(
		fs_operations_container->fs_operations.rm_file);

	kfree(new_file_hidden_args->t_handle);
	kfree(rm_file_hidden_args->t_handle);
	kfree(new_file_hidden_args);
	kfree(rm_file_hidden_args);

	/* COMPILER: Finally, we free the projections themselves. */
	kfree(fs_container);
	kfree(fs_operations_container);

	/* IPC REPLY -------------------------------------------------- */

	LIBLCD_MSG("vfs replying to unregister_fs caller");
	ret = lcd_reply();
	if (ret) {
		LIBLCD_ERR("lcd_reply");
		lcd_exit(-1);
	}
}

/* INIT -------------------------------------------------- */

/* COMPILER: The callee code always defines an init. This will
 * register an ipc channel with the dispatch loop context.
 *
 * This will also initialize the globals for the trampolines.
 *
 * XXX: In this simple example, it also initializes a data store,
 * but in the future, this may not be done in init. */
#define VFS_CHANNEL_TYPE 1
static struct ipc_channel vfs_channel;

int glue_vfs_init(cptr_t vfs_chnl, struct dispatch_ctx *ctx)
{
	int ret;

	/* Set up ipc channel */
	init_ipc_channel(&vfs_channel, VFS_CHANNEL_TYPE, vfs_chnl, 1);

	loop_ctx = ctx;

	/* Add it to dispatch loop */
	loop_add_channel(loop_ctx, &vfs_channel);

	/* Initialize minix data store */
	ret = lcd_dstore_init_dstore(&minix_dstore);
	if (ret) {
		LIBLCD_ERR("dstore init");
		return ret;
	}

	return 0;
}

void glue_vfs_exit(void)
{
	lcd_dstore_destroy(minix_dstore);

	/* We may as well remove the channel from the loop, though
	 * it doesn't matter in this simple example. (In general, we
	 * probably should.) */
	loop_rm_channel(loop_ctx, &vfs_channel);
}
