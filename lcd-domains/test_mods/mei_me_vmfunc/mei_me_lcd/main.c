/*
 * main.c - runs when mei lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include "./mei_caller.h"

#include <lcd_config/post_hook.h>

int mei_me_init(void);
int mei_me_core_init(void);

void mei_me_exit(void);
void mei_me_core_exit(void);
unsigned long loops_per_jiffy;

static int mei_me_lcd_init(void)
{
	int ret = 0;

	printk("NVME LCD enter \n");

	ret = lcd_enter();

	if (ret)
		goto fail1;

	/*
	 * Initialize mei_me glue
	 */
	ret = glue_mei_init();

	if (ret) {
		LIBLCD_ERR("mei_me init");
		goto fail2;
	}

	ret = mei_me_init();

	if (ret) {
		LIBLCD_ERR("mei_me register failed");
		goto fail3;
	}
	return ret;

fail3:
fail2:
fail1:
	lcd_exit(ret);
}

int __mei_me_lcd_init(void)
{
	return mei_me_lcd_init();
}

static void mei_me_lcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);

	mei_me_exit();

	glue_mei_exit();

	lvd_exit(0);

	return;
}

void __mei_me_lcd_exit(void)
{
	mei_me_lcd_exit();
}

module_init(mei_me_lcd_init);
module_exit(mei_me_lcd_exit);
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
