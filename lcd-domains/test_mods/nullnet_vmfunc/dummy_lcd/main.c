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

struct thc_channel *net_async;
struct glue_cspace *nullnet_cspace;
cptr_t nullnet_sync_endpoint;

int dummy_init_module(void);

static int __noreturn dummy_lcd_init(void) 
{
	int ret = 0;

	printk("LCD enter \n");
	ret = lcd_enter();
	if (ret)
		goto fail1;
	/*
	 * Initialize nullnet glue
	 */
	ret = glue_nullnet_init();
	if (ret) {
		LIBLCD_ERR("nullnet init");
		goto fail2;
	}

	ret = dummy_init_module();

fail2:
fail1:
	lcd_exit(ret);
}

static int __dummy_lcd_init(void)
{
	return dummy_lcd_init();
}

static void __exit dummy_lcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);
	glue_nullnet_exit();
	lcd_exit(0);
	return;
}

module_init(__dummy_lcd_init);
module_exit(dummy_lcd_exit);
MODULE_LICENSE("GPL");

