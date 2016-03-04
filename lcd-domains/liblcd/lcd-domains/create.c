/*
 * create.c
 *
 * Simple LCD create functions, configuration.
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/create.h>
#include <asm/lcd_domains/liblcd.h>

#include <lcd_config/post_hook.h>

int lcd_create(cptr_t *lcd)
{
	cptr_t slot;
	int ret;
	/*
	 * Alloc cptr
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	/*
	 * Create LCD syscall
	 */
	ret = lcd_syscall_create(slot);
	if (ret) {
		LIBLCD_ERR("lcd create failed");
		goto fail2;
	}
	
	*lcd = slot;
	
	return 0;

fail2:
	lcd_cptr_free(slot);
fail1:
	return ret;
}

int lcd_create_klcd(cptr_t *klcd)
{
	return -ENOSYS; /* not available in isolated LCDs */
}

int lcd_config_registers(cptr_t lcd, gva_t pc, gva_t sp, gpa_t gva_root,
				 gpa_t utcb_page)
{
	return lcd_syscall_config_registers(lcd, pc, sp, gva_root, utcb_page);
}

int lcd_memory_grant_and_map(cptr_t lcd, cptr_t mo, cptr_t dest_slot,
			gpa_t base)
{
	return lcd_syscall_memory_grant_and_map(lcd, mo, dest_slot, base);
}

int lcd_cap_grant(cptr_t lcd, cptr_t src, cptr_t dest)
{
	return lcd_syscall_cap_grant(lcd, src, dest);
}

int lcd_set_struct_module_hva(cptr_t lcd, struct module *mod)
{
	return -ENOSYS; /* not allowed in isolated (for now I suppose) */
}

int lcd_run(cptr_t lcd)
{
	return lcd_syscall_run(lcd);
}
