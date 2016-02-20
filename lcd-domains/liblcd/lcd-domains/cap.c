/*
 * cap.c
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <asm/lcd_domains/liblcd.h>
#include <lcd_domains/liblcd.h>
#include <liblcd/cap.h>

#include <lcd_config/post_hook.h>

/* CAP DEBUGGING -------------------------------------------------- */

int cap_debug_level = LIBLCD_LIBCAP_DEBUG_LVL;

/* FUNCTIONS -------------------------------------------------- */

void lcd_cap_delete(cptr_t slot)
{
	lcd_syscall_cap_delete(slot);
	lcd_cptr_free(slot);
}

int lcd_cap_revoke(cptr_t slot)
{
	return lcd_syscall_cap_revoke(slot);
}

int lcd_cptr_alloc(cptr_t *slot_out)
{
	return cptr_alloc(&lcd_get_boot_info()->cptr_cache, slot_out);
}

void lcd_cptr_free(cptr_t slot)
{
	cptr_free(&lcd_get_boot_info()->cptr_cache, slot);
}
