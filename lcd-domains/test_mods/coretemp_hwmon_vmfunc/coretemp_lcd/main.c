/*
 * main.c - runs when coretemp_lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include "./coretemp_hwmon_caller.h"

#include <lcd_config/post_hook.h>

struct glue_cspace *coretemp_hwmon_cspace;
cptr_t coretemp_hwmon_sync_endpoint;

int coretemp_init(void);
void coretemp_exit(void);

static int coretemp_lcd_init(void)
{
	int ret = 0;

	printk("LCD enter \n");
	ret = lcd_enter();
	if (ret)
		goto fail1;
	/*
	 * Initialize coretemp_hwmon glue
	 */
	ret = glue_coretemp_hwmon_init();
	if (ret) {
		LIBLCD_ERR("coretemp_hwmon init");
		goto fail2;
	}

	ret = coretemp_init();

	return ret;
fail2:
fail1:
	lcd_exit(ret);
}

int __coretemp_lcd_init(void)
{
	return coretemp_lcd_init();
}

static void coretemp_lcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);

	coretemp_exit();

	glue_coretemp_hwmon_exit();
	lvd_exit(0);
	return;
}

void __coretemp_lcd_exit(void)
{
	coretemp_lcd_exit();
}

module_init(__coretemp_lcd_init);
module_exit(coretemp_lcd_exit);
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
