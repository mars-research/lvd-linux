/*
 * boot.c - boot and dispatch loop for vfs lcd
 */

/* HEADERS -------------------------------------------------- */

/* COMPILER: This is always included. */
#include <lcd-domains/liblcd.h>

/* COMPILER: This is always included. */
#include <lcd-domains/dispatch_loop.h>

/* LOOP -------------------------------------------------- */

/* COMPILER: See notes in minix's boot.c. */
#define VFS_CHANNEL_TYPE 1

#define REGISTER_FS 1
#define UNREGISTER_FS 2
void register_fs_callee(void);
void unregister_fs_callee(void);

void do_stuff(void);

static int dispatch_vfs_channel(struct ipc_channel *channel)
{
	switch (lcd_r0()) {

	case REGISTER_FS:

		LIBLCD_MSG("vfs received register_fs message");

		register_fs_callee();
		
		/* HACK: (Instead of having another thread invoke
		 * the registered fs's new and rm file functions.) */
		do_stuff();

		break;

	case UNREGISTER_FS:

		LIBLCD_MSG("vfs received unregister_fs message");

		unregister_fs_callee();
		break;

	default:

		LIBLCD_ERR("unexpected function label %d",
			lcd_r0());
		return -EINVAL;
	}

	return 0;
}

static int dispatch_channel(struct ipc_channel *channel)
{
	switch (channel->type) {

	case VFS_CHANNEL_TYPE:
		
		return dispatch_vfs_channel(channel);

	default:
		
		LIBLCD_ERR("unexpected channel type %d",
			channel->type);
		return -EINVAL;
	}
}

static int alloc_cptrs_for_channel(struct ipc_channel *c)
{
	int i;
	int ret;
	cptr_t t;
	
	if (c->expected_cptrs >= LCD_NUM_REGS - 1) {
		LIBLCD_ERR("too many cptrs");
		return -EINVAL;
	}

	/* First cr is reserved */
	for (i = 0; i < c->expected_cptrs; i++) {

		ret = lcd_alloc_cptr(&t);
		if (ret) {
			LIBLCD_ERR("alloc cptr");
			return ret;
		}
		lcd_get_utcb()->cr[i + 1] = t;
	}
	
	return 0;
}

static void loop(struct dispatch_ctx *ctx)
{
	struct list_head *cursor;
	struct ipc_channel *channel;
	int ret;
	int count = 0;

	for (;;) {

		count += 1;

		list_for_each(cursor, &ctx->channel_list) {

			channel = list_entry(cursor,
					struct ipc_channel,
					channel_list);

			ret = alloc_cptrs_for_channel(channel);
			if (ret) {
				LIBLCD_ERR("cptrs not set up for recv");
				return;
			}
			
			ret = lcd_recv(channel->channel_cptr);
			if (ret) {
				LIBLCD_ERR("lcd recv");
				return;
			}

			ret = dispatch_channel(channel);
			if (ret) {
				LIBLCD_ERR("dispatch channel");
				return;
			}
		}
		
		/* HACK: VFS exits after receiving two calls (one to
		 * register_fs and one to unregister_fs). */
		if (count >= 2)
			return;
	}
}

/* INIT -------------------------------------------------- */

int glue_vfs_init(cptr_t vfs_chnl, struct dispatch_ctx *ctx);
void glue_vfs_exit(void);

int original_vfs_lcd_init(void);
void original_vfs_lcd_exit(void);

static struct dispatch_ctx ctx;

static int __init vfs_lcd_boot(void)
{
	int ret;
	cptr_t vfs_chnl;
	
	/* COMPILER: We always invoke lcd_enter before anything else.
	 * (This initializes the LCD environment.) */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd_enter");
		lcd_exit(ret);
	}

	/* COMPILER: Initialize dispatch loop context. This will be
	 * passed to each piece of interface glue. */
	init_dispatch_ctx(&ctx);

	/* COMPILER: Because minix required vfs, whoever boots vfs should
	 * provide it with a capability to a channel for invoking vfs
	 * functions. */
	vfs_chnl = lcd_get_boot_info()->cptrs[0];

	/* COMPILER: We initialize all interfaces next with all
	 * dependencies. */
	ret = glue_vfs_init(vfs_chnl, &ctx);
	if (ret) {
		LIBLCD_ERR("vfs init");
		lcd_exit(ret);
	}

	/* COMPILER: In general, we need to `concurrently' invoke the 
	 * module's original init *and* start the dispatch loop (the module's
	 * init may lead to some back-and-forth interactions). In the
	 * future, this will be done using async. 
	 *
	 * For this simple example, vfs's init is a no-op, so we can
	 * just invoke it and get it out of the way. */
	
	/* Call original init. */
	ret = original_vfs_lcd_init();
	if (ret) {
		LIBLCD_ERR("original init");
		lcd_exit(ret);
	}

	/* Enter loop. */
	loop(&ctx);

	/* COMPILER: Again, we're lucky here because the dispatch loop
	 * doesn't need to be 'live' since in this simple example,
	 * exit doesn't lead to complicated back and forth. */

	/* Call original exit. */
	original_vfs_lcd_exit();

	glue_vfs_exit();

	/* Done */
	lcd_exit(0);
}

static void __exit vfs_lcd_exit(void)
{
	/* Never called, for now. */
}

module_init(vfs_lcd_boot);
module_exit(vfs_lcd_exit);
