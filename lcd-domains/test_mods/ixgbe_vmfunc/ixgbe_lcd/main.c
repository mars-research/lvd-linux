/*
 * main.c - runs when ixgbe lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include "./ixgbe_caller.h"

#include <lcd_config/post_hook.h>

int ixgbe_done = 0;

int ixgbe_init_module(void);
void ixgbe_exit_module(void);
unsigned long loops_per_jiffy;
bool poll_start = false;
extern int __ixgbe_poll(void);
unsigned long poll_state = 0ul;

int ixgbe_lcd_init(void)
{
	int r = 0;

	printk("IXGBE LCD enter \n");

	r = lcd_enter();
	if (r)
		goto fail1;

	printk("ixgbe lpj %lu\n", loops_per_jiffy);
	/*
	 * Initialize ixgbe glue
	 */
	r = glue_ixgbe_init();
	if (r) {
		LIBLCD_ERR("ixgbe init");
		goto fail2;
	}
	return r;
fail2:
fail1:
	lcd_exit(r);
}

static int __ixgbe_lcd_init(void)
{
	return ixgbe_lcd_init();
}

static void __exit ixgbe_lcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);
	return;
}

module_init(__ixgbe_lcd_init);
module_exit(ixgbe_lcd_exit);
MODULE_LICENSE("GPL");
MODULE_INFO(livepatch, "Y");
