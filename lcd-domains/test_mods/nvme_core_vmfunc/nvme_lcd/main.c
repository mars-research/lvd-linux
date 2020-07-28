/*
 * main.c - runs when nvme lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include "./nvme_caller.h"

#include <lcd_config/post_hook.h>

int nvme_init(void);
int nvme_core_init(void);

void nvme_exit(void);
void nvme_core_exit(void);
unsigned long loops_per_jiffy;

static int nvme_lcd_init(void)
{
	int ret = 0;

	printk("NVME LCD enter \n");

	ret = lcd_enter();

	if (ret)
		goto fail1;

	/*
	 * Initialize nvme glue
	 */
	ret = glue_nvme_init();

	if (ret) {
		LIBLCD_ERR("nvme init");
		goto fail2;
	}

	ret = nvme_core_init();

	ret = nvme_init();

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

int __nvme_lcd_init(void)
{
	return nvme_lcd_init();
}

static void nvme_lcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);

	nvme_exit();

	nvme_core_exit();

	glue_nvme_exit();

	lvd_exit(0);

	return;
}

void __nvme_lcd_exit(void)
{
	nvme_lcd_exit();
}

module_init(nvme_lcd_init);
module_exit(nvme_lcd_exit);
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
