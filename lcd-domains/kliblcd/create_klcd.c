/**
 * create_klcd.c - Code for creating a non-isolated LCD.
 *
 * In some cases, a klcd may want to create another klcd - basically,
 * create another thread and grant it capabilities. lcd_enter/lcd_exit
 * are insufficient for this. (You could imagine that perhaps instead
 * the klcd would load a kernel module that runs non-isolated and the
 * thread that runs the module's init does lcd_enter first thing. But then
 * the creator can't grant caps to that thread, or talk to it in any
 * way via the microkernel interfaces.)
 *
 * Authors:
 *   Charlie Jacobsen  <charlesj@cs.utah.edu>
 */

#include <linux/slab.h>
#include <linux/mm.h>
#include <asm/page.h>
#include <lcd-domains/kliblcd.h>
#include <lcd-domains/utcb.h>
#include <lcd-domains/types.h>
#include <linux/mutex.h>
#include "../microkernel/internal.h"

int klcd_create_klcd(cptr_t *slot_out)
{
	int ret;
	/*
	 * Alloc cptr
	 */
	ret = lcd_alloc_cptr(slot_out);
	if (ret)
		goto fail1;
	ret = __klcd_create_klcd(current->lcd, *slot_out);
	if (ret)
		goto fail2;

	return 0;

fail2:
	lcd_free_cptr(*slot_out);
fail1:
	return ret;
}

/* KLCD CREATE WITH MODULE ---------------------------------------- */

int klcd_create_module_klcd(cptr_t *slot_out, char *mdir, char *mname)
{
	int ret;
	struct module *m;
	/*
	 * Create a klcd
	 */
	ret = klcd_create_klcd(slot_out);
	if (ret) {
		LCD_ERR("klcd create failed");
		goto fail0;
	}
	/*
	 * Load module
	 */
	ret = __klcd_get_module(mdir, mname, &m);
	if (ret) {
		LCD_ERR("error getting module");
		goto fail1;
	}
	/*
	 * Configure klcd
	 *
	 * NOTE: For now, the microkernel ignores everything
	 * except the program counter, so we just pass junk for those
	 * other arguments.
	 */
	ret = lcd_config(*slot_out, 
			__gva(hva_val(va2hva(m->init))), 
			__gva(0),
			__gpa(0),
			__gpa(0));
	if (ret) {
		LCD_ERR("failed to config lcd");
		goto fail2;
	}
	/*
	 * Done!
	 */
	return 0;

fail2:
	/* 
	 * Remove module from host
	 */
	__klcd_put_module(mname);	
fail1:
	/*
	 * Should destroy lcd since this is the one and only capability to
	 * it.
	 */
	lcd_cap_delete(*slot_out);
fail0:
	return ret;
}

void klcd_destroy_module_klcd(cptr_t klcd, char *module_name)
{
	/*
	 * We *must* delete the lcd first before unloading the module.
	 * Otherwise, if the lcd is still running, it will use the freed
	 * module pages.
	 */
	lcd_cap_delete(klcd);
	__klcd_put_module(module_name);
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(klcd_create_klcd);
EXPORT_SYMBOL(klcd_create_module_klcd);
EXPORT_SYMBOL(klcd_destroy_module_klcd);
