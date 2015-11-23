/*
 * main.c - module init/exit for vfs klcd
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <lcd-domains/kliblcd.h>
#include <lcd-domains/dispatch_loop.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#include "internal.h"

/* LOOP -------------------------------------------------- */

static struct dispatch_ctx ctx;

static int dispatch_vfs_channel(struct ipc_channel *channel)
{
	switch (lcd_r0()) {

	case REGISTER_FILESYSTEM:
		return register_filesystem_callee();
		break;

	case UNREGISTER_FILESYSTEM:
		return unregister_filesystem_callee();
		break;

	case BDI_INIT:
		return bdi_init_callee();
		break;

	case BDI_DESTROY:
		return bdi_destroy_callee();
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

		if (kthread_should_stop())
			return;

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

		if (count >= 4)
			return;
	}
}

/* INIT / EXIT ---------------------------------------- */

static int __init vfs_klcd_init(void) 
{
	int ret;
	cptr_t vfs_chnl;
	cptr_t bdi_chnl;

	/*
	 * Set up cptr cache, etc.
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter");
		goto fail1;
	}
	/*
	 * XXX: Hack: boot has used two cptrs, alloc'd in this
	 * order:
	 *
	 *       vfs channel
	 *       bdi channel
	 *
	 * The bdi channel is not used (until we get async)
	 */
	ret = lcd_alloc_cptr(&vfs_chnl);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto fail2;
	}
	ret = lcd_alloc_cptr(&bdi_chnl);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto fail3;
	}
	/*
	 * Initialize dispatch loop context
	 */
	init_dispatch_ctx(&ctx);
	/*
	 * Initialize the vfs interface glue code
	 */
	ret = glue_vfs_init(vfs_chnl, &ctx);
	if (ret) {
		LIBLCD_ERR("vfs init");
		goto fail4;
	}
	/*
	 * Initialize the bdi interface glue code
	 */
	ret = glue_bdi_init(vfs_chnl, &ctx);
	if (ret) {
		LIBLCD_ERR("bdi init");
		goto fail5;
	}
	/*
	 * Enter loop to listen for PMFS
	 */
	loop(&ctx);
	/*
	 * Fire vfs and bdi glue code exit to tear them down
	 */
	glue_vfs_exit();
	glue_bdi_exit();

	lcd_exit(0);
	
	return 0;

fail5:
	glue_vfs_exit();
fail4:
fail3:
fail2:
	lcd_exit(ret);
fail1:
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

module_init(vfs_klcd_init);
module_exit(vfs_klcd_exit);
MODULE_LICENSE("GPL");
