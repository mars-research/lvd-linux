#include <lcd_config/pre_hook.h>

#include <linux/module.h>
/* COMPILER: This is always included. */
#include <liblcd/liblcd.h>

/* COMPILER: This is always included. */
#include <liblcd/sync_ipc_poll.h>

#include "./nullnet_callee.h"

/* COMPILER: This is always included after all includes. */
#include <lcd_config/post_hook.h>
/* LOOP ------------------------------------------------------------ */
#if 0
static int async_loop(struct fs_info **fs_out, struct fipc_message **msg_out)
{
	struct fs_info *cursor, *next;
	int ret;

	list_for_each_entry_safe(cursor, next, &fs_infos, list) {

		ret = thc_ipc_poll_recv(cursor->chnl, msg_out);
		if (ret == -EPIPE) {
			/*
			 * fs channel is dead; free the channel,
			 * and remove from list
			 */
			kfree(cursor->chnl);
			remove_fs(cursor);
		} else if (ret == -EWOULDBLOCK) {
			/*
			 * Skip over empty channels
			 */
			continue;
		} else if (ret) {
			/*
			 * Unexpected error
			 */
			LIBLCD_ERR("error ret = %d on async channel");
			return ret;
		} else {
			/*
			 * Got a msg
			 */
			*fs_out = cursor;
			return 0;
		}

	}
	/*
	 * Nothing for us to recv right now
	 */
	return -EWOULDBLOCK;
}

static int do_one_register(cptr_t register_chnl)
{
	int ret;
	cptr_t sync_endpoint, tx, rx;
	/*
	 * Set up cptrs
	 */
	ret = lcd_cptr_alloc(&sync_endpoint);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	ret = lcd_cptr_alloc(&tx);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail2;
	}
	ret = lcd_cptr_alloc(&rx);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail3;
	}
	/*
	 * Set up regs and poll
	 */
	lcd_set_cr0(sync_endpoint);
	lcd_set_cr1(tx);
	lcd_set_cr2(rx);
	ret = lcd_sync_poll_recv(register_chnl);
	if (ret) {
		if (ret == -EWOULDBLOCK)
			ret = 0;
		goto free_cptrs;
	}
	/*
	 * Dispatch to register handler
	 */
	ret = dispatch_sync_loop();
	if (ret)
		return ret; /* dispatch fn is responsible for cptr cleanup */

	return 0;

free_cptrs:
	lcd_set_cr0(CAP_CPTR_NULL);
	lcd_set_cr1(CAP_CPTR_NULL);
	lcd_set_cr2(CAP_CPTR_NULL);
	lcd_cptr_free(sync_endpoint);
fail3:
	lcd_cptr_free(tx);
fail2:
	lcd_cptr_free(rx);
fail1:
	return ret;
}
#endif
/* LOOP -------------------------------------------------- */

//static void loop(struct lcd_sync_channel_group *group)
static void loop(cptr_t register_chnl)
{
	struct lcd_sync_channel_group_item *chnl = NULL;
	int ret = 0 ;
	int count = 0;
	return;
	for (;;) {

		count += 1;

//		ret = lcd_sync_channel_group_recv(group, chnl, &chnl);
		if (ret) {
			LIBLCD_ERR("lcd recv");
			return;
		}

		chnl->dispatch_fn(chnl);

//		chnl = lcd_sync_channel_group_item_next(chnl);
			
		/* HACK: VFS exits after receiving two calls (one to
		 * register_fs and one to unregister_fs). */
		if (count >= 2)
			return;
	}
}

/* INIT / EXIT ---------------------------------------- */

static int net_klcd_init(void) 
{
	int ret;
	cptr_t net_chnl;
	/*
	 * Set up cptr cache, etc.
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter");
		goto fail1;
	}

	/*
	 * XXX: Hack: boot provided us with one cptr for the net chnl
	 */
	ret = lcd_cptr_alloc(&net_chnl);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto fail2;
	}

	/*
	 * Init net glue
	 */
	ret = glue_nullnet_init();
	if (ret) {
		LIBLCD_ERR("net init");
		goto fail3;
	}
	/*
	 * Enter sync/async dispatch loop
	 */
	LIBLCD_MSG(">>>>> Looping .... \n");
	loop(net_chnl);
	/*
	 * Tear down net glue
	 */
	glue_nullnet_exit();

	lcd_exit(0);
	
	return 0;

fail3:
fail2:
	lcd_exit(ret);
fail1:
	return ret;
}

static int __net_klcd_init(void)
{
	int ret = 0;
	LCD_MAIN({

			ret = net_klcd_init();

		});
	return ret;
}

/* 
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit net_klcd_exit(void)
{
	return;
}

module_init(__net_klcd_init);
module_exit(net_klcd_exit);
MODULE_LICENSE("GPL");

