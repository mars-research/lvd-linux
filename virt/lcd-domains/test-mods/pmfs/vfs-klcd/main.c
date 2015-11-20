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

		if (count >= 2)
			return;
	}
}

/* INIT / EXIT ---------------------------------------- */

static int __init vfs_klcd_init(void) 
{
	int ret;
	cptr_t vfs_chnl;
	cptr_t unused;

	/*
	 * Set up cptr cache, etc.
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter");
		goto out;
	}
	/*
	 * XXX: Hack: boot has used cptr 0x3, so we just alloc / mark
	 * that slot as occupied, so we don't try to re-use it for
	 * something else.
	 */
	ret = lcd_alloc_cptr(&unused);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto out;
	}
	/*
	 * Initialize dispatch loop context
	 */
	init_dispatch_ctx(&ctx);
	/*
	 * For now, we hack the cptr thing. This code expects
	 * the vfs channel to be in slot 3 (root table) in its cspace.
	 */
	vfs_chnl = __cptr(3);
	/*
	 * Initialize the vfs interface glue code
	 */
	ret = glue_vfs_init(vfs_chnl, &ctx);
	if (ret) {
		LIBLCD_ERR("vfs init");
		goto out;
	}
	/*
	 * Enter loop to listen for PMFS
	 */
	loop(&ctx);
	/*
	 * Fire vfs glue code exit to tear it down
	 */
	glue_vfs_exit();

	ret = 0;
	goto out;

out:
	lcd_exit(ret);
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
