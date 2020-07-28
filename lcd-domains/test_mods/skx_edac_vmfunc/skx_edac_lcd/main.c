/*
 * main.c - runs when nvme lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include "./skx_edac_caller.h"

#include <lcd_config/post_hook.h>

int skx_init(void);
void skx_exit(void);

unsigned long loops_per_jiffy;

static int skx_edac_lcd_init(void)
{
	int ret = 0;

	printk("NVME LCD enter \n");

	ret = lcd_enter();

	if (ret)
		goto fail1;

	/*
	 * Initialize nvme glue
	 */
	ret = glue_skx_edac_init();

	if (ret) {
		LIBLCD_ERR("nvme init");
		goto fail2;
	}

	ret = skx_init();

	if (ret) {
		LIBLCD_ERR("nvme register failed");
		goto fail3;
	}
	return ret;

fail3:
fail2:
fail1:
	lcd_exit(ret);
}

int __skx_edac_lcd_init(void)
{
	return skx_edac_lcd_init();
}

static void skx_edac_lcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);

	skx_exit();
	
	glue_skx_edac_exit();

	lvd_exit(0);

	return;
}

void __skx_edac_lcd_exit(void)
{
	skx_edac_lcd_exit();
}

module_init(skx_edac_lcd_init);
module_exit(skx_edac_lcd_exit);
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
