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
#include "../microkernel/internal.h"

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

/* ADDR -> CPTR ---------------------------------------- */

static int fits_in_page(unsigned long data, unsigned long len)
{
	return (data & PAGE_MASK) == ((data + len) & PAGE_MASK);
}

int klcd_phys_addr_to_page_cptr(unsigned long data, unsigned long len, 
				cptr_t *page_cptr_out, 
				unsigned long *offset_out)
{
	int ret;
	struct page *p;
	/*
	 * Make sure data doesn't trail off page
	 */
	if (!fits_in_page(data, len)) {
		LIBLCD_ERR("data (pa=%lx,len=%lx) spans more than one page",
			data, len);
		ret = -EINVAL;
		goto fail1;
	}
	/*
	 * Get struct page for physical address
	 */
	p = pfn_to_page(data >> PAGE_SHIFT);
	if (!p) {
		LIBLCD_ERR("error getting page for pa=%lx", data);
		ret = -EINVAL;
		goto fail2;
	}
	/*
	 * "Volunteer" page
	 */
	ret = klcd_add_page(p, page_cptr_out);
	if (ret)
		goto fail3;
	/*
	 * Calculate offset into page
	 */
	*offset_out = data & ~PAGE_MASK;

	return 0;

fail3:
fail2:
fail1:
	return ret;
}

int klcd_virt_addr_to_page_cptr(char *data, unsigned long len, 
				cptr_t *page_cptr_out, 
				unsigned long *offset_out)
{
	/*
	 * Use physical address for look up
	 */
	return lcd_phys_addr_to_page_cptr(__pa(data),
					len,
					page_cptr_out, 
					offset_out);
}

int klcd_addr_to_page_cptr_cleanup(cptr_t page_cptr)
{
	klcd_rm_page(page_cptr);
	return 0;
}

/* PAGE MAPPING ---------------------------------------- */

int klcd_map_pages_phys(cptr_t *pages, gpa_t *base_out, unsigned order)
{
	int ret;
	struct cnode *cnode;
	unsigned long hpa;
	/*
	 * Look up first page
	 */
	ret = __lcd_cnode_get(&current->lcd->cspace, pages[0], &cnode);
	if (ret) {
		LCD_ERR("couldn't find page in caller's cspace");
		goto fail1;
	}
	/*
	 * Confirm it's a page, and get the physical address.
	 */
	switch (cnode->type) {
	case LCD_CAP_TYPE_PAGE:
		hpa = __pa(page_address(cnode->object));
		break;
	case LCD_CAP_TYPE_KPAGE:
		hpa = __pa(page_address(cnode->object));
		break;
	default:
		LCD_ERR("not a page");
		goto fail2;
	}

	*base_out = __gpa(hpa); /* gpa == hpa for non-isolated code */

	ret = 0;
	goto out;

out:
fail2:
	__lcd_cnode_put(cnode);
fail1:
	return ret;
}

int klcd_map_pages_both(cptr_t *pages, gva_t *base_out, unsigned order)
{
	int ret;
	gpa_t gpa_base;
	/*
	 * "Map" it in physical, and get the base phys address.
	 *
	 * (This basically boils down to just looking up the physical
	 * address of the pages. The pages are assumed to be contiguous.)
	 */
	ret = klcd_map_pages_phys(pages, &gpa_base, order);
	if (ret) {
		LCD_ERR("error 'mapping' in phys");
		goto fail1;
	}

#ifndef CONFIG_HIGHMEM
	/*
	 * For e.g. x86_64, all of phys memory is already mapped in
	 * the kernel's address space. (See Documentation/x86/x86_64/mm.txt.)
	 * So, we can just return the page's address.
	 *
	 * Remember that gva == hva for non-isolated code.
	 */
	*base_out = __gva(hva_val(va2hva(__va(gpa_val(gpa_base)))));
#else /* CONFIG_HIGHMEM */
	#error "We don't support this kind of arch right now."
#endif

	return 0;

fail1:
	return ret;
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(klcd_add_page);
EXPORT_SYMBOL(klcd_rm_page);
EXPORT_SYMBOL(klcd_page_alloc);
EXPORT_SYMBOL(klcd_gfp);
EXPORT_SYMBOL(klcd_phys_addr_to_page_cptr);
EXPORT_SYMBOL(klcd_virt_addr_to_page_cptr);
EXPORT_SYMBOL(klcd_addr_to_page_cptr_cleanup);
EXPORT_SYMBOL(klcd_map_pages_both);
EXPORT_SYMBOL(klcd_map_pages_phys);
