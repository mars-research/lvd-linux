/*
 * main.c - module init/exit for vfs klcd
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#include "internal.h"

#include <lcd_config/post_hook.h>

/* LOOP -------------------------------------------------- */

static void loop(struct lcd_sync_channel_group *group)
{
	struct lcd_sync_channel_group_item *chnl = NULL;
	int ret;
	int count = 0;

	for (;;) {

		if (kthread_should_stop())
			return;

		count += 1;

		ret = lcd_sync_channel_group_recv(group, chnl, &chnl);
		if (ret) {
			LIBLCD_ERR("lcd recv");
			return;
		}

		chnl->dispatch_fn(chnl);

		chnl = lcd_sync_channel_group_item_next(chnl);
			
		if (count >= 4)
			return;
	}
}

/* INIT / EXIT ---------------------------------------- */

static struct lcd_sync_channel_group group;

static int vfs_klcd_init(void) 
{
	int ret;
	cptr_t vfs_chnl;
	/*
	 * Set up cptr cache, etc.
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter");
		goto fail1;
	}
	/*
	 * XXX: Hack: boot provided us with one cptr for the vfs channel
	 */
	ret = lcd_cptr_alloc(&vfs_chnl);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto fail2;
	}
	/*
	 * Initialize dispatch loop context
	 */
	lcd_sync_channel_group_init(&group);
	/*
	 * Initialize the vfs interface glue code
	 */
	ret = glue_vfs_init(vfs_chnl, &group);
	if (ret) {
		LIBLCD_ERR("vfs init");
		goto fail3;
	}
	/*
	 * Enter loop to listen for PMFS
	 */
	loop(&group);
	/*
	 * Fire vfs and bdi glue code exit to tear them down
	 */
	glue_vfs_exit();

	lcd_exit(0);
	
	return 0;

fail3:
fail2:
	lcd_exit(ret);
fail1:
	return ret;
}

static int __vfs_klcd_init(void)
{
	int ret;

	LCD_MAIN({

			ret = vfs_klcd_init();

		});

	return ret;
}

/* 
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit vfs_klcd_exit(void)
{
	return;
}

module_init(__vfs_klcd_init);
module_exit(vfs_klcd_exit);
MODULE_LICENSE("GPL");
