/*
 * bootinfo.h
 *
 * This is the data that is stored in the LCD's bootstrap
 * pages.
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_BOOTINFO_H
#define LCD_DOMAINS_BOOTINFO_H

#include <libcap.h>
#include <lcd-domains/types.h>

struct lcd_boot_info_for_page {
	cptr_t my_cptr;
	gpa_t page_gpa;
};

#define LCD_NUM_BOOT_CPTRS 8

struct lcd_boot_info {
	/*
	 * Bootstrap cptr cache
	 */
	struct cptr_cache cptr_cache;
	/*
	 * Bootstrap page info
	 */
	unsigned num_boot_mem_pi;
	unsigned num_stack_mem_pi;
	unsigned num_paging_mem_pi;
	unsigned num_free_mem_pi;
	struct lcd_boot_info_for_page *boot_mem_pi_start;
	struct lcd_boot_info_for_page *stack_mem_pi_start;
	struct lcd_boot_info_for_page *paging_mem_pi_start;
	struct lcd_boot_info_for_page *free_mem_pi_start;
	/*
	 * Other capabilities (e.g., endpoints)
	 */
	cptr_t cptrs[LCD_NUM_BOOT_CPTRS];
};

/**
 * lcd_get_boot_info -- Return the bootstrap info
 *
 * When executing inside an LCD, invoke this function, and it will
 * return a pointer to the LCD's bootstrap data.
 *
 * For non-isolated code, this function returns NULL.
 */
struct lcd_boot_info * lcd_get_boot_info(void);

#endif /* LCD_DOMAINS_BOOTINFO_H */
