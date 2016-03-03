/*
 * ksym.c
 *
 * Address -> symbol name resolution
 *
 * Copyright: University of Utah
 */

#include <lcd_domains/microkernel.h>
#include <linux/module.h>

int __lcd_set_struct_module_hva(struct lcd *caller, cptr_t lcd, 
				hva_t module_addr)
{
	struct cnode *cnode;
	struct lcd lcd_struct;
	int ret;
	/*
	 * Look up LCD
	 */
	ret = __lcd_get(caller, lcd, &cnode, &lcd_struct);
	if (ret) {
		LCD_ERR("lookup failed");
		goto fail1;
	}
	/*
	 * If it doesn't have an lcd_arch, fail (no underlying VM)
	 */
	if (!lcd->lcd_arch) {
		LCD_ERR("no vm");
		ret = -EINVAL;
		goto fail2;
	}
	/*
	 * Store module address in lcd_arch
	 */
	lcd->lcd_arch->kernel_module = hva2va(module_addr);

	__lcd_put(caller, cnode, lcd_struct);

	return 0;

fail2:
	__lcd_put(caller, cnode, lcd_struct);
fail1:
	return ret;
}
