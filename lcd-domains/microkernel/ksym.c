/*
 * ksym.c
 *
 * Address -> symbol name resolution
 *
 * Copyright: University of Utah
 */

#include <lcd_domains/microkernel.h>
#include <linux/module.h>
#include <linux/kallsyms.h>

int __lcd_set_struct_module_hva(struct lcd *caller, cptr_t lcd, 
				hva_t module_addr)
{
	struct cnode *cnode;
	struct lcd *lcd_struct;
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
	if (!lcd_struct->lcd_arch) {
		LCD_ERR("no vm");
		ret = -EINVAL;
		goto fail2;
	}
	/*
	 * Store module address in lcd_arch
	 */
	lcd_struct->lcd_arch->kernel_module = hva2va(module_addr);

	__lcd_put(caller, cnode, lcd_struct);

	return 0;

fail2:
	__lcd_put(caller, cnode, lcd_struct);
fail1:
	return ret;
}

static inline int in_module(struct module *module, hva_t hva)
{
	/*
	 * XXX: Danger section: The struct module is inside the LCD; it
	 * could have messed with these fields. This is for debug only.
	 */

	/* in module's init? */
	if(hva2va(hva) >= module->module_init &&
		hva2va(hva) < (module->module_init + module->init_size))
		return 1;
	/* in module's core? */
	if(hva2va(hva) >= module->module_core &&
		hva2va(hva) < (module->module_core + module->core_size))
		return 1;
	/* not in module */
	return 0;	
}

unsigned long
__lcd_sprint_symbol(char *buffer, hva_t hva, struct module *extra_module)
{
	const char *sym_name;
	char *modname = NULL;
	unsigned long offset;
	unsigned long size;
	unsigned long len;
	/*
	 * This code is motivated by kernel/kallsyms.c:__sprint_symbol
	 */
	if (extra_module && in_module(extra_module, hva)) {
		/*
		 * Symbol lands in our special LCD .ko image; look up
		 * the symbol in there
		 *
		 * XXX: Danger: This is for debug only. The struct module
		 * is mapped inside an LCD which could mess with it.
		 */
		sym_name = get_ksymbol(extra_module, hva_val(hva), 
				&size, &offset);
		modname = extra_module->name;
		strncpy(buffer, sym_name, KSYM_NAME_LEN - 1);
	} else {
		/*
		 * Resolve symbol using host
		 */
		sym_name = kallsyms_lookup(hva_val(hva), &size, &offset, 
					&modname, buffer);
		if (!sym_name)
			return sprintf(buffer, "0x%lx", hva_val(hva));
		if (sym_name != buffer)
			strcpy(buffer, sym_name);
	}
	/*
	 * According to doc in kernel/kallsyms.c:sprint_backtrace,
	 * we need to add 1 to the address to handle noreturn gcc
	 * optimizations.
	 *
	 * Since we always use this code for backtrace, always inc by 1
	 */
	offset += 1;
	len = strlen(buffer);
	len += sprintf(buffer + len, "+%#lx/%#lx", offset, size);
	if (modname)
		len += sprintf(buffer + len, " [%s]", modname);

	return len;
}
