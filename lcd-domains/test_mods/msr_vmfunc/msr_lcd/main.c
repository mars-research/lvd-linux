/*
 * main.c - runs when msr_lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include "./msr_caller.h"

#include <lcd_config/post_hook.h>

struct glue_cspace *msr_cspace;
cptr_t msr_sync_endpoint;

int msr_init(void);
void msr_exit(void);

static int msr_lcd_init(void)
{
	int ret = 0;

	printk("LCD enter \n");
	ret = lcd_enter();
	if (ret)
		goto fail1;
	/*
	 * Initialize msr glue
	 */
	ret = glue_msr_init();
	if (ret) {
		LIBLCD_ERR("msr init");
		goto fail2;
	}

	ret = msr_init();

	return ret;
fail2:
fail1:
	lcd_exit(ret);
}

int __msr_lcd_init(void)
{
	return msr_lcd_init();
}

static void msr_lcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);

	msr_exit();

	glue_msr_exit();
	lvd_exit(0);
	return;
}

void __msr_lcd_exit(void)
{
	msr_lcd_exit();
}

module_init(__msr_lcd_init);
module_exit(msr_lcd_exit);
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
