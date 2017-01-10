#include <lcd_config/pre_hook.h>

#include <linux/module.h>
/* COMPILER: This is always included. */
#include <liblcd/liblcd.h>

/* COMPILER: This is always included. */
#include <liblcd/sync_ipc_poll.h>

#include "./nullnet_callee.h"

/* COMPILER: This is always included after all includes. */
#include <lcd_config/post_hook.h>


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
	//LIBLCD_MSG(">>>>>> Entering LCD mode\n");
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter");
		goto fail1;
	}

	net_chnl = lcd_get_boot_info()->cptrs[0];

	/*
	 * Init net glue
	 */
//	ret = glue_nullnet_init();
	if (ret) {
		LIBLCD_ERR("net init");
		goto fail3;
	}
	/*
	 * Enter sync/async dispatch loop
	 */
	//LIBLCD_MSG(">>>>> Looping .... \n");
	loop(net_chnl);
	/*
	 * Tear down net glue
	 */
	//glue_nullnet_exit();

	lcd_exit(0);
	
	return 0;

fail3:
	lcd_exit(ret);
fail1:
	return ret;
}

static int __net_klcd_init(void)
{
	int ret = 0;
//	printk("net klcd_init\n");
	if (1) {
	LCD_MAIN({

			ret = net_klcd_init();

		});
	}
	return ret;
}

/* 
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit net_klcd_exit(void)
{
//	glue_nullnet_exit();

	lcd_exit(0);

	return;
}

module_init(__net_klcd_init);
module_exit(net_klcd_exit);
MODULE_LICENSE("GPL");

