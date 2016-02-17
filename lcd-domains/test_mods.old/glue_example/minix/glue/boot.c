/*
 * boot.c - boot and dispatch loop for minix lcd
 */

/* HEADERS -------------------------------------------------- */

/* COMPILER: This is always included. */
#include <lcd-domains/liblcd.h>

/* COMPILER: This is always included. */
#include <lcd-domains/dispatch_loop.h>

/* LOOP -------------------------------------------------- */

/* COMPILER: For every channel a module listens on for function calls,
 * there is a channel type. In this example, minix listens on one channel
 * for NEW_FILE and RM_FILE. In general, channels will include those that
 * are part of interfaces and projections of function pointers. */
#define MINIX_CHANNEL_TYPE 1

#define NEW_FILE 1
#define RM_FILE 2
void new_file_callee(void);
void rm_file_callee(void);

static int dispatch_minix_channel(struct ipc_channel *channel)
{
	switch (lcd_r0()) {

	case NEW_FILE:

		LIBLCD_MSG("minix got new_file message");

		new_file_callee();
		break;

	case RM_FILE:

		LIBLCD_MSG("minix got rm_file message");

		rm_file_callee();
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

	case MINIX_CHANNEL_TYPE:
		
		return dispatch_minix_channel(channel);

	default:
		
		LIBLCD_ERR("unexpected channel type %d",
			channel->type);
		return -EINVAL;
	}
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
		
		/* HACK: Break out of this loop after we receive two
		 * calls: one to new_file and one to rm_file. We'll
		 * exit, and then unregister_fs will fire. */
		if (count >= 2)
			return;
	}
}

/* INIT -------------------------------------------------- */

int glue_vfs_init(cptr_t vfs_chnl, struct dispatch_ctx *ctx);
void glue_vfs_exit(void);

int original_minix_lcd_init(void);
void original_minix_lcd_exit(void);

static struct dispatch_ctx ctx;

static int __init minix_lcd_boot(void)
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

	/* COMPILER: Because minix required vfs, whoever boots minix should
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
	 * future, this will be done using async. For this simple example,
	 * we are lucky as there is no complicated back-and-forth. We can
	 * invoke init, *and then* enter the dispatch loop. */

	/* Call original init. */
	ret = original_minix_lcd_init();
	if (ret) {
		LIBLCD_ERR("original init");
		lcd_exit(ret);
	}

	/* Enter loop. */
	loop(&ctx);

	/* Call original exit. */
	original_minix_lcd_exit();

	

	/* Done */
	lcd_exit(0);
}

static void __exit minix_lcd_exit(void)
{
	/* Never called, for now. */
}

module_init(minix_lcd_boot);
module_exit(minix_lcd_exit);
