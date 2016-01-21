/*
 * mem.c
 *
 * Copyright: University of Utah
 */

#include <libcap.h>
#include <lcd-domains/liblcd.h>
#include "../microkernel/internal.h"

/* LOW-LEVEL PAGE ALLOC -------------------------------------------------- */

int _lcd_alloc_pages_exact_node(int nid, unsigned int flags, 
				unsigned int order, cptr_t *slot_out)
{
	struct lcd *lcd = current->lcd;
	cptr_t slot;
	int ret;
	/*
	 * Allocate a slot in caller's cspace
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	/*
	 * Allocate pages
	 */
	ret = __lcd_alloc_pages_exact_node(lcd, slot, nid, flags, order);
	if (ret) {
		LIBLCD_ERR("alloc pages exact node failed");
		goto fail2;
	}

	*slot_out = slot;
	return 0;

fail2:
	lcd_cptr_free(slot);
fail1:
	return ret;
}

int _lcd_alloc_pages(unsigned int flags, unsigned int order,
		cptr_t *slot_out)
{
	struct lcd *lcd = current->lcd;
	cptr_t slot;
	int ret;
	/*
	 * Allocate a slot in caller's cspace
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	/*
	 * Allocate pages
	 */
	ret = __lcd_alloc_pages(lcd, slot, flags, order);
	if (ret) {
		LIBLCD_ERR("alloc pages failed");
		goto fail2;
	}

	*slot_out = slot;
	return 0;

fail2:
	lcd_cptr_free(slot);
fail1:
	return ret;
}

/* LOW-LEVEL MAP -------------------------------------------------- */

int _lcd_mmap(cptr_t mo, gpa_t base)
{
	/* rb tree? */
}

void _lcd_munmap(cptr_t mo)
{
	/* rb tree? */
}

/* HIGH-LEVEL ALLOC/FREE ---------------------------------------- */

struct page *lcd_alloc_pages_exact_node(int nid, unsigned int flags, 
					unsigned int order)
{
	struct lcd *lcd = current->lcd;
	int ret;
	cptr_t slot;
	struct lcd_memory_object *mo;
	struct cnode *cnode;
	struct page *p = NULL;
	/*
	 * Do lower-level alloc so that pages go into caller's cspace
	 */
	ret = _lcd_alloc_pages_exact_node(nid, flags, order, &slot);
	if (ret) {
		LIBLCD_ERR("lower level alloc failed");
		goto fail1;
	}
	/*
	 * Look up memory object so we can get struct page pointer
	 */
	ret = __lcd_get_memory_object(lcd, slot, &cnode, &mo);
	if (ret) {
		LIBLCD_ERR("internal error: mem lookup failed");
		goto fail2;
	}
	p = mo->object;
	__lcd_put_memory_object(lcd, cnode, mo);

	return p;

fail2:
	lcd_cap_delete(slot); /* frees pages, etc. */
fail1:
	return NULL;
}

struct page *lcd_alloc_pages(unsigned int flags, unsigned int order)
{
	struct lcd *lcd = current->lcd;
	int ret;
	cptr_t slot;
	struct lcd_memory_object *mo;
	struct cnode *cnode;
	struct page *p = NULL;
	/*
	 * Do lower-level alloc so that pages go into caller's cspace
	 */
	ret = _lcd_alloc_pages_node(flags, order, &slot);
	if (ret) {
		LIBLCD_ERR("lower level alloc failed");
		goto fail1;
	}
	/*
	 * Look up memory object so we can get struct page pointer
	 */
	ret = __lcd_get_memory_object(lcd, slot, &cnode, &mo);
	if (ret) {
		LIBLCD_ERR("internal error: mem lookup failed");
		goto fail2;
	}
	p = mo->object;
	__lcd_put_memory_object(lcd, cnode, mo);

	return p;

fail2:
	lcd_cap_delete(slot); /* frees pages, etc. */
fail1:
	return NULL;
}

void lcd_free_pages(struct page *base, unsigned int order)
{
	cptr_t page_cptr;
	int ret;
	gpa_t gpa;
	unsigned int actual_order;
	/*
	 * Translate page to physical address.
	 *
	 * This line asserts that guest physical == host physical for
	 * non-isolated code.
	 */
	gpa = __gpa(hpa_val(va2hpa(page_address(base))));
	/*
	 * Resolve pages to cptr
	 */
	ret = lcd_phys_to_cptr(gpa, &page_cptr, &actual_order);
	if (ret) {
		LIBLCD_ERR("warning: pages not found, so not freed");
		return;
	}
	if (order != actual_order)
		LIBLCD_ERR("warning: order doesn't match actual order");
	/*
	 * Free pages
	 */
	lcd_cap_delete(page_cptr);
}

/* HIGH-LEVEL MAP/UNMAP ---------------------------------------- */

int lcd_map_phys(cptr_t pages, unsigned int order, gpa_t *base_out)
{
	int ret;
	struct page *p;
	/*
	 * Ignore order, etc., since non-isolated code already has
	 * access to all physical memory. (We could maybe check that
	 * the order is correct.)
	 *
	 * XXX: It's significant that we don't call
	 * __lcd_map_memory_object. This means we won't update any
	 * mapping metadata for the non-isolated thread's pages
	 * capability, and so nothing will be done when the
	 * capability is revoked.
	 *
	 * Look up memory object so we can get struct page pointer,
	 * and then translate to physical address
	 */
	ret = __lcd_get_memory_object(lcd, slot, &cnode, &mo);
	if (ret) {
		LIBLCD_ERR("internal error: mem lookup failed");
		goto fail1;
	}
	p = mo->object;
	__lcd_put_memory_object(lcd, cnode, mo);
	
	/* guest physical == host physical for non-isolated */
	*base_out = __gpa(hpa_val(va2hpa(page_address(p))));
	
	return 0;
fail1:
	return ret;
}

int lcd_map_virt(gpa_t base, unsigned int order, gva_t *gva_out)
{
	/*
	 * On x86_64, all RAM is mapped already. Note that we
	 * don't bother checking whether the non-isolated thread
	 * actually has a capability to the RAM.
	 *
	 * XXX: For arch's with smaller virtual address spaces, 
	 * we need to kmap or similar.
	 *
	 * guest virtual == host virtual for non-isolated
	 */
	*gva_out = __gva(hva_val(pa2hva(gpa_val(base))));
	
	return 0;
}

int lcd_map_both(cptr_t pages, unsigned int order, gva_t *gva_out)
{
	int ret;
	gpa_t gpa;
	/*
	 * "Map" in physical (this is more like cptr -> phys addr)
	 */
	ret = lcd_map_phys(pages, order, &gpa);
	if (ret)
		return ret;
	/*
	 * "Map" in virtual
	 */
	return lcd_map_virt(gpa, order, gva_out);
}

void lcd_unmap_phys(gpa_t base, unsigned int order)
{
	/*
	 * No-op. No need to unmap in physical for the host.
	 *
	 * We don't mark the non-isolated thread's capability metadata
	 * as is_mapped to begin with, so no need to unset it.
	 */
}

void lcd_unmap_virt(gva_t base, unsigned int order)
{
	/*
	 * No-op.
	 *
	 * XXX: At least for x86_64. For arch's with smaller virtual
	 * address spaces, we need to kunmap or similar.
	 *
	 * We don't mark the non-isolated thread's capability metadata
	 * as is_mapped to begin with, so no need to unset it.
	 */
}

void lcd_unmap_both(gva_t base, unsigned int order)
{
	/*
	 * No-op.
	 */
}

/* "VOLUNTEERING" PAGES ---------------------------------------- */

int lcd_volunteer_pages(struct page *base, unsigned int order,
			cptr_t *slot_out)
{
	/* insert into cspace */

	/* insert into rb tree */
}

void lcd_unvolunteer_pages(cptr_t pages)
{
	lcd_cap_delete(pages);
}

/* "VOLUNTEERING" DEVICE MEMORY ---------------------------------------- */

int lcd_volunteer_dev_mem(gpa_t base, unsigned int order,
			cptr_t *slot_out)
{
	/* insert into cspace */

	/* insert into rb tree */
}

void lcd_unvolunteer_dev_mem(cptr_t devmem)
{
	lcd_cap_delete(devmem);
}

/* ADDRESS -> CPTR TRANSLATION ---------------------------------------- */

int lcd_phys_to_cptr(gpa_t paddr, cptr_t *c_out, unsigned int *order_out)
{
	/* rb tree lookup */
}

int lcd_virt_to_cptr(gva_t vaddr, cptr_t *c_out, unsigned int *order_out)
{
	gpa_t gpa;
	/*
	 * Translate virtual-> physical. guest phys = host phys.
	 */
	gpa = __gpa(hpa_val(hva2hpa(__hva(gva_val(vaddr)))));
	return lcd_phys_to_cptr(gpa, c_out, order_out);
}

/* MISCELLANEOUS -------------------------------------------------- */

struct page *lcd_virt_to_head_page(const void *addr)
{
	/* 
	 * Note: we don't bother checking that addr corresponds to
	 * memory that the caller has a capability to.
	 */
	return virt_to_head_page(addr);
}

void *lcd_page_address(const struct page *page)
{
	/* 
	 * Note: we don't bother checking that page corresponds to
	 * memory that the caller has a capability to.
	 */
	return page_address(page);
}

void lcd_free_memcg_kmem_pages(unsigned long addr, unsigned int order)
{
	/* Non-isolated code probably won't ever call this. */
	free_memcg_kmem_pages(addr, order);
}
