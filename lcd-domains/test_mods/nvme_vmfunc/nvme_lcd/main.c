/*
 * main.c - runs when ixgbe lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include "./nvme_caller.h"

#include <lcd_config/post_hook.h>

int ixgbe_init_module(void);
void ixgbe_exit_module(void);
unsigned long loops_per_jiffy;

static int nvme_lcd_init(void)
{
	int ret = 0;

	printk("IXGBE LCD enter \n");

	ret = lcd_enter();

	if (ret)
		goto fail1;

	/* loops_per_jiffy = lcd_get_boot_info()->cptrs[1].cptr; */

	printk("ixgbe lpj %lu\n", loops_per_jiffy);
	/*
	 * Initialize ixgbe glue
	 */
	ret = glue_ixgbe_init();

	if (ret) {
		LIBLCD_ERR("ixgbe init");
		goto fail2;
	}

	ret = ixgbe_init_module();

	if (ret) {
		LIBLCD_ERR("ixgbe register failed");
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

	ixgbe_exit_module();

	glue_ixgbe_exit();

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
