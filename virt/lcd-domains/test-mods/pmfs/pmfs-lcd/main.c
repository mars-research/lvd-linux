/*
 * main.c - runs when pmfs lcd boots
 */

#include <lcd-domains/liblcd-config.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <lcd-domains/liblcd.h>
#include <lcd-domains/dispatch_loop.h>

#include <lcd-domains/liblcd-hacks.h>

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

static int __noreturn __init pmfs_lcd_init(void) 
{
	int r;
	r = lcd_enter();
	if (r)
		goto fail1;

	r = init_pmfs_fs();
	if (r)
		goto fail1;

	exit_pmfs_fs();


fail1:
	lcd_exit(r);
}

static void __exit pmfs_lcd_exit(void)
{
	return;
}

module_init(pmfs_lcd_init);
module_exit(pmfs_lcd_exit);
