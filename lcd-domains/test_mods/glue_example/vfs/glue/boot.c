/*
 * boot.c - boot and dispatch loop for vfs lcd
 */

/* HEADERS -------------------------------------------------- */

/* COMPILER: This is always included before any other includes. */
#include <lcd_config/pre_hook.h>

/* COMPILER: This is always included. */
#include <liblcd/liblcd.h>

/* COMPILER: This is always included. */
#include <liblcd/sync_ipc_poll.h>

#include <linux/module.h>

/* COMPILER: This is always included after all includes. */
#include <lcd_config/post_hook.h>

/* LOOP -------------------------------------------------- */

static void loop(struct lcd_sync_channel_group *group)
{
	struct lcd_sync_channel_group_item *chnl = NULL;
	int ret;
	int count = 0;

	for (;;) {

		count += 1;

		ret = lcd_sync_channel_group_recv(group, chnl, &chnl);
		if (ret) {
			LIBLCD_ERR("lcd recv");
			return;
		}

		chnl->dispatch_fn(chnl);

		chnl = lcd_sync_channel_group_item_next(chnl);
			
		/* HACK: VFS exits after receiving two calls (one to
		 * register_fs and one to unregister_fs). */
		if (count >= 2)
			return;
	}
}

/* INIT -------------------------------------------------- */

int glue_vfs_init(cptr_t vfs_chnl, struct lcd_sync_channel_group *group);
void glue_vfs_exit(void);

int original_vfs_lcd_init(void);
void original_vfs_lcd_exit(void);

static struct lcd_sync_channel_group group;

static int vfs_lcd_boot(void)
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
	lcd_sync_channel_group_init(&group);

	/* COMPILER: Because minix required vfs, whoever boots vfs should
	 * provide it with a capability to a channel for invoking vfs
	 * functions. */
	vfs_chnl = lcd_get_boot_info()->cptrs[0];

	/* COMPILER: We initialize all interfaces next with all
	 * dependencies. */
	ret = glue_vfs_init(vfs_chnl, &group);
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
	loop(&group);

	/* COMPILER: Again, we're lucky here because the dispatch loop
	 * doesn't need to be 'live' since in this simple example,
	 * exit doesn't lead to complicated back and forth. */

	/* Call original exit. */
	original_vfs_lcd_exit();

	glue_vfs_exit();

	/* Done */
	lcd_exit(0);
}

static int vfs_lcd_init(void)
{
	int ret;

	LCD_MAIN({

			ret = vfs_lcd_boot();

		});
	
	return ret;
}

static void __exit vfs_lcd_exit(void)
{
	/* Never called, for now. */
}

module_init(vfs_lcd_init);
module_exit(vfs_lcd_exit);
