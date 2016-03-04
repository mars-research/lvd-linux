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
				struct module *mod)
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
	lcd_struct->lcd_arch->kernel_module = mod;

	__lcd_put(caller, cnode, lcd_struct);

	return 0;

fail2:
	__lcd_put(caller, cnode, lcd_struct);
fail1:
	return ret;
}

static inline const char *adjust_addr(const char *addr, struct module *mod,
				hva_t new_m_core_addr)
{
	/* unsigned long offset; */
	/*
	 * Compute offset into module's core
	 */
	return addr;
	/* offset = ((unsigned long)addr) - ((unsigned long)mod->module_core); */
	/* /\* */
	/*  * Add to new core location */
	/*  *\/ */
	/* return (const char *)(hva_val(new_m_core_addr) + offset); */
}

static const char *get_ksymbol(struct module *mod,
			hva_t addr,
			unsigned long *size,
			unsigned long *offset)
{
	unsigned int i, best = 0;
	unsigned long nextval;

	/* At worse, next value is at end of module */
	if (within_module_init(hva_val(addr), mod))
		nextval = (unsigned long)mod->module_init+mod->init_text_size;
	else
		nextval = (unsigned long)mod->module_core+mod->core_text_size;

	/* Scan for closest preceding symbol, and next symbol. (ELF
	   starts real symbols at 1). */
	for (i = 1; i < mod->num_symtab; i++) {
		if (mod->symtab[i].st_shndx == SHN_UNDEF)
			continue;

		/* We ignore unnamed symbols: they're uninformative
		 * and inserted at a whim. */
		if (mod->symtab[i].st_value <= hva_val(addr)
			&& mod->symtab[i].st_value > mod->symtab[best].st_value && 
			*(mod->strtab + mod->symtab[i].st_name) != '\0')
				best = i;
		if (mod->symtab[i].st_value > hva_val(addr)
			&& mod->symtab[i].st_value < nextval
			&& *(mod->strtab + mod->symtab[i].st_name) != '\0')
			nextval = mod->symtab[i].st_value;
	}

	if (!best)
		return NULL;

	if (size)
		*size = nextval - mod->symtab[best].st_value;
	if (offset)
		*offset = hva_val(addr) - mod->symtab[best].st_value;
	return mod->strtab + mod->symtab[best].st_name;
}


static inline int in_module(struct module *module, hva_t hva)
{
	/*
	 * XXX: Danger section: The struct module is inside the LCD; it
	 * could have messed with these fields. This is for debug only.
	 */
	return within_module_init(hva_val(hva), module) ||
		within_module_core(hva_val(hva), module);
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
		sym_name = get_ksymbol(extra_module, hva,
				&size, &offset);
		if (!sym_name)
			return 0;
		modname = extra_module->name;
		strncpy(buffer, sym_name, KSYM_NAME_LEN - 1);
	} else {
		/*
		 * Resolve symbol using host
		 */
		sym_name = kallsyms_lookup(hva_val(hva), &size, &offset, 
					&modname, buffer);
		if (!sym_name)
			return 0;
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
