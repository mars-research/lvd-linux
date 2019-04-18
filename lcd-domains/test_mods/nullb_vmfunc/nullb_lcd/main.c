/*
 * main.c - runs when dummy lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include "./nullb_caller.h"

#include <lcd_config/post_hook.h>

int nullb_done = 0;

int null_init(void);
void null_exit(void);

static int nullb_lcd_init(void)
{
	int r = 0;

	printk("LCD enter \n");

	r = lcd_enter();

	if (r)
		goto fail1;
	/*
	 * Initialize nullb glue
	 */
	r = glue_nullb_init();
	if (r) {
		LIBLCD_ERR("nullb init");
		goto fail2;
	}

	r = null_init();

	return r;
fail2:
fail1:
	lcd_exit(r);
}

int __nullb_lcd_init(void)
{
	return nullb_lcd_init();
}

static void nullb_lcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);

	null_exit();

	glue_nullb_exit();

	lcd_exit(0);

	return;
}

void __nullb_lcd_exit(void)
{
	nullb_lcd_exit();
}

module_init(__nullb_lcd_init);
module_exit(nullb_lcd_exit);
MODULE_LICENSE("GPL");
MODULE_INFO(livepatch, "Y");

/* extract data from linker variables */
size_t vmfunc_sboard_page_size = (size_t)&__vmfunc_sboard_page_size;
unsigned long* vmfunc_sboard_load_addr = (unsigned long*) &__vmfunc_sboard_load_addr;

/* extract data from linker variables */
size_t vmfunc_trampoline_page_size = (size_t)&__vmfunc_trampoline_page_size;
unsigned long* vmfunc_trampoline_load_addr = (unsigned long*) &__vmfunc_trampoline_load_addr;

EXPORT_SYMBOL(vmfunc_sboard_load_addr);
EXPORT_SYMBOL(vmfunc_trampoline_load_addr);
