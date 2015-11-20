/**
 * mem.c - Implementation of kliblcd mem mgt code.
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
#include "../internal.h"

/**
 * Allows a KLCD to add a regular page to it's cspace so that the
 * microkernel can start tracking it. (Motivation: If a kthread allocates
 * a page using some other non-microkernel interface, e.g. a regular
 * alloc_page, this will not be trackable by the microkernel, and the
 * KLCD can't share the page with another LCD.)
 */
int klcd_add_page(struct page *p, cptr_t *slot_out)
{
	int ret;
	/*
	 * Alloc cptr
	 */
	ret = lcd_alloc_cptr(slot_out);
	if (ret)
		goto fail1;
	/*
	 * Insert page
	 */
	ret = __lcd_cap_insert(&current->lcd->cspace, *slot_out, p,
			LCD_CAP_TYPE_KPAGE);
	if (ret)
		goto fail2;

	return 0;

fail2:
	lcd_free_cptr(*slot_out);
fail1:
	return ret;
}

/**
 * Remove a page from microkernel tracking (does not free the
 * page itself).
 */
void klcd_rm_page(cptr_t slot)
{
	/*
	 * Delete page from my cspace
	 */
	lcd_cap_delete(slot);
	/*
	 * Return cptr to cache
	 */
	lcd_free_cptr(slot);
}

static int __lcd_page_alloc(cptr_t *slot_out, hpa_t *hpa_out, hva_t *hva_out)
{
	int ret;
	/*
	 * Allocate a cptr
	 */
	ret = lcd_alloc_cptr(slot_out);
	if (ret)
		goto fail1;
	/*
	 * Get free page
	 */
	ret = __klcd_page_zalloc(current->lcd, *slot_out, hpa_out, hva_out);
	if (ret)
		goto fail2;

	return 0;

fail2:
	lcd_free_cptr(*slot_out);
fail1:
	return ret;
}

int klcd_page_alloc(cptr_t *slot_out, gpa_t gpa)
{
	hpa_t hpa;
	hva_t hva;
	/*
	 * Ignore gpa, since klcd's don't have control over where
	 * page is mapped.
	 */
	return __lcd_page_alloc(slot_out, &hpa, &hva);
}

int klcd_pages_alloc(cptr_t *slots_out, hpa_t *hp_base_out, 
		hva_t *hv_base_out, unsigned order)
{
	int ret;
	int i, j;
	/*
	 * Get cptr's from cache
	 */
	for (i = 0; i < (1 << order); i++) {
		ret = lcd_alloc_cptr(&slots_out[i]);
		if (ret) {
			LCD_ERR("alloc cptr");
			goto fail;
		}
	}
	/*
	 * Get free pages
	 */
	ret = __klcd_pages_zalloc(current->lcd, slots_out, hp_base_out, 
				hv_base_out, order);
	if (ret) {
		LCD_ERR("pages alloc");
		goto fail;
	}

	return 0;

fail:
	for (j = 0; j < i; j++)
		lcd_free_cptr(slots_out[i]);
	return ret;
}

int klcd_gfp(cptr_t *slot_out, gpa_t *gpa_out, gva_t *gva_out)
{
	hpa_t hpa;
	hva_t hva;
	int ret;
	ret = __lcd_page_alloc(slot_out, &hpa, &hva);
	if (ret)
		goto fail1;
	*gpa_out = __gpa(hpa_val(hpa));
	*gva_out = __gva(hva_val(hva));

	return 0;

fail1:
	return ret;
}

int klcd_free_page(cptr_t page)
{
	/*
	 * Delete from cspace
	 */
	lcd_cap_delete(page);
	/*
	 * Free the slot
	 */
	lcd_free_cptr(page);

	return 0;
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(klcd_add_page);
EXPORT_SYMBOL(klcd_rm_page);
EXPORT_SYMBOL(klcd_page_alloc);
EXPORT_SYMBOL(klcd_gfp);

