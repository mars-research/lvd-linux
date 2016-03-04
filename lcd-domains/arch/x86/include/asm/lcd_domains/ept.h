/*
 * ept.h
 *
 * Functions for modifying an LCD's guest physical
 * address space.
 *
 * Copyright: University of Utah
 */
#ifndef ASM_X86_LCD_DOMAINS_EPT_H
#define ASM_X86_LCD_DOMAINS_EPT_H

#include <asm/lcd_domains/types.h>

hva_t hpa2hva(hpa_t hpa);

/**
 * Lookup ept entry for guest physical address a.
 *
 * Set create = 1 to allocate ept page table data structures
 * along the path as needed.
 */
int lcd_arch_ept_walk(struct lcd_arch *lcd, gpa_t a, int create,
		lcd_arch_epte_t **epte_out);
/**
 * Set the guest physical => host physical mapping in the ept entry.
 */
void lcd_arch_ept_set(lcd_arch_epte_t *epte, hpa_t a);
/**
 * Read the host physical address stored in epte.
 */
hpa_t lcd_arch_ept_hpa(lcd_arch_epte_t *epte);
/**
 * Clears guest physical => host physical mapping in the ept.
 *
 * (This is not going to free up potentially empty paging structures
 * higher up in the ept hierarchy; but, for now, unset is used when
 * we're tearing down the lcd -- due to an error -- so the paging
 * structures will be freed up when the ept is torn down.)
 */
int lcd_arch_ept_unset(lcd_arch_epte_t *epte);
/**
 * Simple routine combining ept walk and set.
 *
 * overwrite = 0  => do not overwrite if ept entry is already present
 * overwrite = 1  => overwrite any existing ept entry
 */
int lcd_arch_ept_map(struct lcd_arch *lcd, gpa_t ga, hpa_t ha,
		int create, int overwrite);
/**
 * Maps 
 *
 *    ga_start --> ga_start + npages * PAGE_SIZE
 *
 * to
 *
 *    ha_start --> ha_start + npages * PAGE_SIZE
 *
 * in lcd's ept.
 */
int lcd_arch_ept_map_range(struct lcd_arch *lcd, gpa_t ga_start, 
			hpa_t ha_start, unsigned long npages);
/**
 * Simple routine combining ept walk and unset.
 */
int lcd_arch_ept_unmap(struct lcd_arch *lcd, gpa_t a);
/**
 * Like unmpa, but returns old hpa in hpa_out if it is non-NULL.
 */
int lcd_arch_ept_unmap2(struct lcd_arch *lcd, gpa_t a, hpa_t *hpa_out);
/**
 * Unmaps 
 *
 *    ga_start --> ga_start + npages * PAGE_SIZE
 *
 * in lcd's ept.
 */
int lcd_arch_ept_unmap_range(struct lcd_arch *lcd, gpa_t ga_start, 
			unsigned long npages);
/**
 * Simple routine combinding ept walk and get.
 */
int lcd_arch_ept_gpa_to_hpa(struct lcd_arch *lcd, gpa_t ga, hpa_t *ha_out);
/**
 * Translate guest physical address to host virtual.
 */
static inline int lcd_arch_ept_gpa_to_hva(struct lcd_arch *lcd, gpa_t gpa,
			hva_t *hva_out)
{
	hpa_t hpa;
	int ret;
	ret = lcd_arch_ept_gpa_to_hpa(lcd, gpa, &hpa);
	if (ret)
		return ret;
	*hva_out = hpa2hva(hpa);
	return 0;
}
/**
 * Invalidate all EPT mappings.
 */
int lcd_arch_ept_global_invalidate(void);
/**
 * Initializes the EPT's root global page directory page, the
 * VMCS pointer, and the spinlock.
 */
int lcd_arch_ept_init(struct lcd_arch *lcd_arch);
/**
 * Free an LCD's EPT tables.
 *
 * Frees all memory associated with ept structures
 * (but not the mapped memory itself! -- this can
 * lead to memory leaks, but is better than potential
 * double frees that crash the machine or scrog the disk!).
 */
void lcd_arch_ept_free(struct lcd_arch *lcd);
/**
 * Dump EPTs to console.
 */
void lcd_arch_ept_dump(struct lcd_arch *lcd);

#endif /* ASM_X86_LCD_DOMAINS_EPT_H */
