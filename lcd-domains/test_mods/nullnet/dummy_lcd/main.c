/*
 * main.c - runs when dummy lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include "./nullnet_caller.h"

#include <lcd_config/post_hook.h>

cptr_t nullnet_register_channel;
struct thc_channel *nullnet_async_chnl;
struct glue_cspace *nullnet_cspace;
cptr_t nullnet_sync_endpoint;
int dummy_done;
int dummy_init_module(void);
void dummy_cleanup_module(void);

/* LOOP ---------------------------------------- */

static void main_and_loop(void)
{
	int ret;
	int stop = 0;
	struct fipc_message *msg;
	return;
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
			/*
			 * Do one async receive
			 */
			ret = thc_ipc_poll_recv(nullnet_async_chnl, &msg);
			if (ret) {
				if (ret == -EWOULDBLOCK) {
					continue;
				} else {
					LIBLCD_ERR("async recv failed");
					stop = 1; /* stop */
				}
			}
			/*
			 * Got a message. Dispatch.
			 */
			ASYNC(

//				ret = dispatch_fs_channel(nullnet_async_chnl, msg,
//							nullnet_cspace, 
//							nullnet_sync_endpoint);
				if (ret) {
					LIBLCD_ERR("async dispatch failed");
					stop = 1;
				}

				);
		}
		
		LIBLCD_MSG("PMFS EXITED DISPATCH LOOP");

		);

	/*
	 * We don't expect any requests coming back to us, so it's safe
	 * to just run this without a loop (it's effectively polling since
	 * only one awe will run in this do-finish).
	 */
	DO_FINISH(
		ASYNC(
			dummy_cleanup_module();

			
				LIBLCD_MSG("SUCCESSFULLY UNREGISTERED PMFS!");

			);
		);

	LIBLCD_MSG("EXITED PMFS DO_FINISH");

	return;
}

static int dummy_lcd_init(void) 
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
	/*
	 * Initialize nullnet glue
	 */
	//r = glue_nullnet_init();
	if (r) {
		LIBLCD_ERR("nullnet init");
		goto fail2;
	}

	/* RUN CODE / LOOP ---------------------------------------- */

	main_and_loop();

	/* DONE -------------------------------------------------- */

	//glue_nullnet_exit();

	lcd_exit(0); /* doesn't return */
	return 0;

fail2:
fail1:
	lcd_exit(r);
}

static int __dummy_lcd_init(void)
{
	int ret;

	LCD_MAIN({

			ret = dummy_lcd_init();

		});

	return ret;
}

static void __exit dummy_lcd_exit(void)
{
	//glue_nullnet_exit();

	lcd_exit(0);

	return;
}

module_init(__dummy_lcd_init);
module_exit(dummy_lcd_exit);
MODULE_LICENSE("GPL");

