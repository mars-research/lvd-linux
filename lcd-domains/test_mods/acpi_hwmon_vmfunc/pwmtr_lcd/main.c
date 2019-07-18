/*
 * main.c - runs when pwmtr_lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include "./acpi_hwmon_caller.h"

#include <lcd_config/post_hook.h>

struct glue_cspace *acpi_hwmon_cspace;
cptr_t acpi_hwmon_sync_endpoint;

int acpi_power_meter_init(void);
void acpi_power_meter_exit(void);

static int pwmtr_lcd_init(void)
{
	int ret = 0;

	printk("LCD enter \n");
	ret = lcd_enter();
	if (ret)
		goto fail1;
	/*
	 * Initialize acpi_hwmon glue
	 */
	ret = glue_acpi_hwmon_init();
	if (ret) {
		LIBLCD_ERR("acpi_hwmon init");
		goto fail2;
	}

	ret = acpi_power_meter_init();

	return ret;
fail2:
fail1:
	lcd_exit(ret);
}

int __pwmtr_lcd_init(void)
{
	return pwmtr_lcd_init();
}

static void pwmtr_lcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);

	acpi_power_meter_exit();

	glue_acpi_hwmon_exit();
	lvd_exit(0);
	return;
}

void __pwmtr_lcd_exit(void)
{
	pwmtr_lcd_exit();
}

module_init(__pwmtr_lcd_init);
module_exit(pwmtr_lcd_exit);
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
