/*
 * main.c - runs when pmfs lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/dispatch_loop.h>
#include "internal.h"

#include <lcd_config/post_hook.h>

/* LOOP ---------------------------------------- */

#if 0
static int dispatch_minix_channel(struct ipc_channel *channel)
{
	switch (lcd_r0()) {

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
	}
}
#endif
/* INIT/EXIT -------------------------------------------------- */

int init_pmfs_fs(void);
void exit_pmfs_fs(void);

struct dispatch_ctx ctx;

static int __noreturn __init pmfs_lcd_init(void) 
{
	int r = 0;
	cptr_t vfs_chnl;
//	cptr_t bdi_chnl;

	r = lcd_enter();
	if (r)
		goto fail1;
	/*
	 * Initialize dispatch loop context
	 */
	init_dispatch_ctx(&ctx);
	/*
	 * Get the vfs and bdi channel cptrs from boot info
	 */
	vfs_chnl = lcd_get_boot_info()->cptrs[0];
//	bdi_chnl = lcd_get_boot_info()->cptrs[1];
	/*
	 * Initialize vfs glue
	 */
	r = glue_vfs_init(vfs_chnl, &ctx);
	if (r) {
		LIBLCD_ERR("vfs init");
		goto fail2;
	}
	/*
	 * Initialize bdi glue
	 */
	r = glue_bdi_init(vfs_chnl, &ctx);
	if (r) {
		LIBLCD_ERR("bdi init");
		goto fail3;
	}

	/* EXECUTE REAL CODE ---------------------------------------- */

	/*
	 * Initialize pmfs
	 */
	r = init_pmfs_fs();
	if (r)
		goto fail4;


	LIBLCD_MSG("SUCCESSFULLY REGISTERED PMFS!");

	// loop(&ctx);

	/*
	 * Tear down pmfs
	 */

	exit_pmfs_fs();

	LIBLCD_MSG("SUCCESSFULLY UNREGISTERED PMFS!");

	/* REAL CODE DONE; CLEAN UP ---------------------------------------- */

	glue_vfs_exit();
	glue_bdi_exit();

	lcd_exit(0); /* doesn't return */

fail4:
	glue_bdi_exit();
fail3:
	glue_vfs_exit();
fail2:
fail1:
	lcd_exit(r);
}

static void __exit pmfs_lcd_exit(void)
{
	return;
}

module_init(pmfs_lcd_init);
module_exit(pmfs_lcd_exit);
MODULE_LICENSE("GPL");
