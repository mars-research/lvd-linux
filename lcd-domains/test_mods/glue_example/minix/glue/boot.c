/*
 * boot.c - boot and dispatch loop for minix lcd
 */

/* HEADERS -------------------------------------------------- */

/* COMPILER: This goes before all includes */
#include <lcd_config/pre_hook.h>

/* COMPILER: This is always included. */
#include <liblcd/liblcd.h>

/* COMPILER: This is always included. */
#include <liblcd/sync_ipc_poll.h>

#include <linux/module.h>
/* COMPILER: This always goes after all includes */
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

		/* HACK: Break out of this loop after we receive two
		 * calls: one to new_file and one to rm_file. We'll
		 * exit, and then unregister_fs will fire. */
		if (count >= 2)
			return;
	}
}

/* INIT -------------------------------------------------- */

int glue_vfs_init(cptr_t vfs_chnl, struct lcd_sync_channel_group *group);
void glue_vfs_exit(void);

int original_minix_lcd_init(void);
void original_minix_lcd_exit(void);

static struct lcd_sync_channel_group group;

static int minix_lcd_boot(void)
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

	/* COMPILER: Because minix required vfs, whoever boots minix should
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
	loop(&group);

	/* Call original exit. */
	original_minix_lcd_exit();

	

	/* Done */
	lcd_exit(0);
}

static int minix_lcd_init(void)
{
	int ret;

	LCD_MAIN({

			ret = minix_lcd_boot();

		});

	return ret;
}

static void __exit minix_lcd_exit(void)
{
	/* Never called, for now. */
}

module_init(minix_lcd_init);
module_exit(minix_lcd_exit);
