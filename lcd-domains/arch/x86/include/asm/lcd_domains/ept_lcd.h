/*
 * ept.h
 *
 * Functions for modifying an LCD's guest physical
 * address space.
 *
 * Copyright: University of Utah
 */
#ifndef ASM_X86_LCD_DOMAINS_EPT_LCD_H
#define ASM_X86_LCD_DOMAINS_EPT_LCD_H

#include <lcd_domains/microkernel.h>
#include <asm/lcd_domains/types.h>

/**
 * Lookup ept entry for guest physical address a.
 *
 * Set create = 1 to allocate ept page table data structures
 * along the path as needed.
 */
int lcd_arch_ept_walk_cpu(lcd_arch_epte_t *dir, gpa_t a, int create,
		lcd_arch_epte_t **epte_out);

/**
 * Simple routine combining ept walk and set.
 *
 * overwrite = 0  => do not overwrite if ept entry is already present
 * overwrite = 1  => overwrite any existing ept entry
 */
int lcd_arch_ept_map_all_cpus(struct lcd_arch *lcd, gpa_t ga, hpa_t ha,
		int create, int overwrite);

int lcd_arch_ept_map_cpu(struct lcd_arch *lcd, gpa_t ga, hpa_t ha,
		int create, int overwrite, int cpu);

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
int lcd_arch_ept_map_range_all_cpus(struct lcd_arch *lcd, gpa_t ga_start,
			hpa_t ha_start, unsigned long npages);

int lcd_arch_ept_map_range_cpu(struct lcd_arch *lcd, gpa_t ga_start,
			hpa_t ha_start, unsigned long npages, int cpu);
/**
 * Simple routine combining ept walk and unset.
 */
int lcd_arch_ept_unmap_all_cpus(struct lcd_arch *lcd, gpa_t a);

int lcd_arch_ept_unmap_cpu(struct lcd_arch *lcd, gpa_t a, int cpu);

/**
 * Unmaps
 *
 *    ga_start --> ga_start + npages * PAGE_SIZE
 *
 * in lcd's ept.
 */
int lcd_arch_ept_unmap_range_all_cpus(struct lcd_arch *lcd, gpa_t ga_start,
			unsigned long npages);
/**
 * Simple routine combinding ept walk and get.
 */
int lcd_arch_ept_gpa_to_hpa_cpu(struct lcd_arch *lcd, gpa_t ga, hpa_t *ha_out, bool verbose, int cpu);

/**
 * Initializes the EPT's root global page directory page, the
 * VMCS pointer, and the spinlock.
 */
unsigned long lcd_arch_ept_init_one(void);

/**
 * Free an LCD's EPT tables.
 *
 * Frees all memory associated with ept structures
 * (but not the mapped memory itself! -- this can
 * lead to memory leaks, but is better than potential
 * double frees that crash the machine or scrog the disk!).
 */
void lcd_arch_ept_free_all_cpus(struct lcd_arch *lcd);
/**
 * Dump EPTs to console.
 */
void lcd_arch_ept_dump_all_cpus(struct lcd_arch *lcd);

void lcd_arch_ept_dump_this_cpu(struct lcd_arch *lcd);

/**
 * Translate guest physical address to host virtual.
 */
static inline int lcd_arch_ept_gpa_to_hva_cpu(struct lcd_arch *lcd, gpa_t gpa,
			hva_t *hva_out, int cpu)
{
	hpa_t hpa;
	int ret;

	ret = lcd_arch_ept_gpa_to_hpa_cpu(lcd, gpa, &hpa, true, cpu);

	if (ret)
		return ret;
	*hva_out = hpa2hva(hpa);
	return 0;
}
#endif /* ASM_X86_LCD_DOMAINS_EPT_LCD_H */
