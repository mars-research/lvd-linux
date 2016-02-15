/*
 * mem.c
 *
 * Isolated liblcd page allocator and related
 * code.
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/mem.h>

#include <lcd_config/post_hook.h>

/* LOW-LEVEL SYSCALLS -------------------------------------------------- */

int _lcd_alloc_pages_exact_node(int nid, unsigned int flags, unsigned int order,
				cptr_t *slot_out)
{
	cptr_t slot;
	int ret;
	/*
	 * Get a free cptr
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("out of cptr's");
		goto fail1;
	}
	/*
	 * Alloc pages
	 */
	ret = lcd_syscall_alloc_pages_exact_node(nid, flags, order, &slot);
	if (ret) {
		LIBLCD_ERR("alloc pages syscall failed");
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
	cptr_t slot;
	int ret;
	/*
	 * Get a free cptr
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("out of cptr's");
		goto fail1;
	}
	/*
	 * Alloc pages
	 */
	ret = lcd_syscall_alloc_pages(flags, order, &slot);
	if (ret) {
		LIBLCD_ERR("alloc pages syscall failed");
		goto fail2;
	}

	*slot_out = slot;
	
	return 0;

fail2:
	lcd_cptr_free(slot);
fail1:
	return ret;
}

int _lcd_vmalloc(unsigned int order, cptr_t *slot_out)
{
	cptr_t slot;
	int ret;
	/*
	 * Get a free cptr
	 */
	ret = lcd_cptr_alloc(&slot);
	if (ret) {
		LIBLCD_ERR("out of cptr's");
		goto fail1;
	}
	/*
	 * Alloc pages
	 */
	ret = lcd_syscall_vmalloc(order, &slot);
	if (ret) {
		LIBLCD_ERR("vmalloc syscall failed");
		goto fail2;
	}

	*slot_out = slot;
	
	return 0;

fail2:
	lcd_cptr_free(slot);
fail1:
	return ret;
}

int _lcd_mmap(cptr_t mo, unsigned int order, gpa_t base)
{
	int ret;
	/*
	 * Do low level syscall to map memory object
	 */
	ret = lcd_syscall_mmap(mo, base);
	if (ret) {
		LIBLCD_ERR("low level mmap failed");
		goto fail1;
	}
	/*
	 * Insert into resource tree (unlike kliblcd, all of our
	 * memory objects are always contiguous in guest physical)
	 */
	ret = __liblcd_mem_itree_insert(base, (1UL << (PAGE_SHIFT + order)),
					mo);
	if (ret) {
		LIBLCD_ERR("error inserting into mem itree");
		goto fail2;
	}

	return 0;

fail2:
	lcd_syscall_munmap(mo);
fail1:
	return ret;
}

void _lcd_munmap(cptr_t mo, gpa_t base)
{
	int ret;
	struct lcd_resource_node *n;
	/*
	 * Look up resource node for memory object in itree
	 */
	ret = lcd_phys_to_resource_node(base, &n);
	if (ret) {
		LIBLCD_ERR("couldn't find memory object in tree");
		return;
	}
	/*
	 * Remove from tree
	 */
	__liblcd_mem_itree_delete(n);
	/*
	 * Unmap memory object
	 */
	lcd_syscall_munmap(mo);
}

/* PAGE ALLOCATOR -------------------------------------------------- */







/* VOLUNTEERING -------------------------------------------------- */

int lcd_volunteer_pages(struct page *base, unsigned int order,
			cptr_t *slot_out)
{
	unsigned long size;
	return lcd_phys_to_cptr(
		lcd_gva2gpa(__gva((unsigned long)lcd_page_address(base))), 
		slot_out, &size);
}

void lcd_unvolunteer_pages(cptr_t pages)
{
	return;
}

int lcd_volunteer_dev_mem(gpa_t base, unsigned int order,
			cptr_t *slot_out)
{
	unsigned long size;
	return lcd_phys_to_cptr(base, slot_out, &size);
}

void lcd_unvolunteer_dev_mem(cptr_t devmem)
{
	return;
}

int lcd_volunteer_vmalloc_mem(gva_t base, unsigned int order,
			cptr_t *slot_out)
{
	unsigned long size;
	return lcd_virt_to_cptr(base, slot_out, &size);
}

void lcd_unvolunteer_vmalloc_mem(cptr_t vmalloc_mem)
{
	return;
}

/* ADDRESS TRANSLATION -------------------------------------------------- */

gpa_t lcd_gva2gpa(gva_t gva)
{
	return __gpa(LCD_PHYS_BASE + (gva_val(gva) - LCD_VIRT_BASE));
}

gva_t lcd_gpa2gva(gpa_t gpa)
{
	return __gva(LCD_VIRT_BASE + (gpa_val(gpa) - LCD_PHYS_BASE));
}

/* INIT/EXIT -------------------------------------------------- */

int __liblcd_mem_init(void)
{


}
