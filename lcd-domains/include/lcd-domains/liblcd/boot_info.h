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
	 * Capabilities to memory objects
	 */
	cptr_t module_init_cptr;
	cptr_t module_core_cptr;
	cptr_t stack_cptr;
	cptr_t gv_pgd_cptr;
	cptr_t gv_pud_cptr;
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
