/*
 * cap.c
 *
 * Copyright: University of Utah
 */

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <lcd_domains/microkernel.h>

extern struct task_struct *klcd_thread;

void lcd_cap_delete(cptr_t slot)
{
	cap_delete(klcd_thread->lcd->cspace, slot);
	lcd_cptr_free(slot);
}

int lcd_cap_revoke(cptr_t slot)
{
	return cap_revoke(klcd_thread->lcd->cspace, slot);
}

int lcd_cptr_alloc(cptr_t *slot_out)
{
	return cptr_alloc(klcd_thread->cptr_cache, slot_out);
}

void lcd_cptr_free(cptr_t slot)
{
	cptr_free(klcd_thread->cptr_cache, slot);
}

struct cspace *get_current_cspace(struct task_struct *c)
{
	return klcd_thread->lcd ? klcd_thread->lcd->cspace : NULL;
}
/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_cap_delete);
EXPORT_SYMBOL(lcd_cap_revoke);
EXPORT_SYMBOL(lcd_cptr_alloc);
EXPORT_SYMBOL(lcd_cptr_free);
EXPORT_SYMBOL(get_current_cspace);
