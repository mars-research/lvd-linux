#ifndef LCD_DOMAINS_LCD_DOMAINS_H
#define LCD_DOMAINS_LCD_DOMAINS_H

#include <asm/lcd-domains-arch.h>

struct lcd {
	/*
	 * Arch-dependent state of lcd
	 */
	struct lcd_arch *lcd_arch;
	/*
	 * Guest virtual paging.
	 */
	struct {
		/*
		 * = 0 if root_hva invalid
		 * = 1 if root_hva valid
		 */
		int present;
		/*
		 * Host virtual address of the root of the lcd's
		 * (initial) guest virtual paging hierarchy.
		 */
		pgd_t *root;
		/*
		 * Pointer to start of guest physical address space 
		 * used for paging.
		 */
		gpa_t paging_mem_bot;
		/*
		 * Pointer to next free page in guest physical
		 * address space that can be used for a page table.
		 */
		gpa_t paging_mem_brk;
		/*
		 * Top of region in guest physical address space
		 * for page tables.
		 */
		gpa_t paging_mem_top;
	} gv;
};

#endif /* LCD_DOMAINS_LCD_DOMAINS_H */
