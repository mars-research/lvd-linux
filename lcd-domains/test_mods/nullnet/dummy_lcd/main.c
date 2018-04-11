/*
 * main.c - runs when dummy lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include "./nullnet_caller.h"

#include "../rdtsc_helper.h"
#include <lcd_config/post_hook.h>

cptr_t nullnet_register_channel;
struct thc_channel *net_async;
struct glue_cspace *nullnet_cspace;
cptr_t nullnet_sync_endpoint;
int dummy_done = 0;
int dummy_init_module(void);
void dummy_cleanup_module(void);
struct thc_channel_group ch_grp;

bool tdiff_valid = false;
u64 tdiff_disp = 0ull;
TS_DECL(disp_loop);
/* LOOP ---------------------------------------- */

static void main_and_loop(void)
{
	int ret;
	int stop = 0;
	struct fipc_message *msg;
	DO_FINISH(

		ASYNC(
			ret = dummy_init_module();
			if (ret) {
				LIBLCD_ERR("dummy register failed");
				stop = 1;
			} else {
				LIBLCD_MSG("SUCCESSFULLY REGISTERED DUMMY!");
			}

			);

		/* By the time we hit this loop, the async channel
		 * will be set up (the awe running init_dummy_fs above
		 * will not yield until it tries to use the async
		 * channel). */
		while (!stop && !dummy_done) {
			struct thc_channel_group_item* curr_item;


			/*
			 * Do one async receive
			 */
//			ret = thc_ipc_poll_recv(net_async, &msg);
//			TS_START_LCD(disp_loop);
			ret = thc_poll_recv_group(&ch_grp, &curr_item, &msg);
			if (ret) {
				if (ret == -EWOULDBLOCK) {
					cpu_relax();
					continue;
				} else {
					LIBLCD_ERR("async recv failed");
					stop = 1; /* stop */
				}
			}
			/*
			 * Got a message. Dispatch.
			 */
/*			if (async_msg_get_fn_type(msg) ==
					NDO_START_XMIT) {
				//TS_STOP_LCD(disp_loop);
				//tdiff_disp = TS_DIFF(disp_loop);
				//tdiff_valid = true;
			} else {
				printk("Some other message");
			}
*/
			ASYNC(

			ret = dispatch_async_loop(curr_item->channel,
					msg,
					nullnet_cspace, 
					nullnet_sync_endpoint);
				if (ret) {
					LIBLCD_ERR("async dispatch failed");
					stop = 1;
				}

				);
		}
		
		LIBLCD_MSG("NULLNET EXITED DISPATCH LOOP");

		);

	LIBLCD_MSG("EXITED PMFS DO_FINISH");

	return;
}

static int __noreturn dummy_lcd_init(void) 
{
	int r = 0;

	printk("LCD enter \n");
	r = lcd_enter();
	if (r)
		goto fail1;
	/*
	 * Get the nullnet channel cptr from boot info
	 */
	nullnet_register_channel = lcd_get_boot_info()->cptrs[0];
	printk("nullnet reg channel %lu\n", nullnet_register_channel.cptr);
	/*
	 * Initialize nullnet glue
	 */
	r = glue_nullnet_init();
	if (r) {
		LIBLCD_ERR("nullnet init");
		goto fail2;
	}

	thc_channel_group_init(&ch_grp);
	/* RUN CODE / LOOP ---------------------------------------- */

	main_and_loop();

	/* DONE -------------------------------------------------- */

	glue_nullnet_exit();

	lcd_exit(0); /* doesn't return */
fail2:
fail1:
	lcd_exit(r);
}

static int __dummy_lcd_init(void)
{
	int ret;

	LIBLCD_MSG("%s: entering", __func__);

	LCD_MAIN({

			ret = dummy_lcd_init();

		});

	return ret;
}

static void __exit dummy_lcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);
	return;
}

module_init(__dummy_lcd_init);
module_exit(dummy_lcd_exit);
MODULE_LICENSE("GPL");

