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

int _lcd_mmap(cptr_t mo, gpa_t base)
{
	return lcd_syscall_mmap(mo, base);
}

void _lcd_munmap(cptr_t mo)
{
	return lcd_syscall_munmap(mo);
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
