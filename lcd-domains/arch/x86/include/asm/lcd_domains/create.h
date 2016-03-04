/*
 * create.h
 *
 * Functions for creating and checking the configuration
 * of a VT-x container.
 *
 * Copyright: University of Utah
 */
#ifndef ASM_X86_LCD_DOMAINS_CREATE_H
#define ASM_X86_LCD_DOMAINS_CREATE_H

/**
 * Creates the arch-dependent part of an LCD (e.g., the ept).
 */
int lcd_arch_create(struct lcd_arch **out);
/**
 * Tears down arch-dep part of LCD.
 *
 * IMPORTANT: When the ept is torn down, any host memory that is still mapped
 * will be freed. This is for convenience. But beware.
 */
void lcd_arch_destroy(struct lcd_arch *lcd_arch);
/**
 * Does logical consistency checks (e.g., runs through checks
 * listed in Intel SDM V3 26.1, 26.2, and 26.3).
 */
int lcd_arch_check(struct lcd_arch *lcd_arch);
/**
 * Allocate memory for the VMCS for an LCD.
 */
struct lcd_arch_vmcs *lcd_arch_alloc_vmcs(int cpu);
/**
 * Free @vmcs page(s).
 */
void lcd_arch_free_vmcs(struct lcd_arch_vmcs *vmcs);

/* Some internal data used in create */
extern struct kmem_cache *lcd_arch_cache;
extern struct lcd_vmcs_config lcd_global_vmcs_config;
extern unsigned long *lcd_global_msr_bitmap;
extern struct lcd_vpids lcd_vpids;

#endif /* ASM_X86_LCD_DOMAINS_CREATE_H */
