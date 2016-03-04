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

#include <lcd_domains/types.h>
#include <asm/lcd_domains/types.h>
#include <linux/slab.h>

/**
 * Creates the arch-dependent part of an LCD (e.g., the ept).
 */
int lcd_arch_create(struct lcd_arch **out);
/**
 * Tears down arch-dep part of LCD.
 *
 * IMPORTANT: When the ept is torn down, any host memory that is still mapped
 * will *not* be freed. Beware.
 */
void lcd_arch_destroy(struct lcd_arch *lcd_arch);
/**
 * Set the lcd's program counter to the guest virtual address
 * a.
 */
int lcd_arch_set_pc(struct lcd_arch *lcd_arch, gva_t a);
/**
 * Read LCD's %rip
 */
static inline u64 lcd_arch_get_pc(struct lcd_arch *lcd)
{
	return lcd->regs.rip;
}
/**
 * Set the lcd's stack pointer to the guest virtual address
 * a.
 */
int lcd_arch_set_sp(struct lcd_arch *lcd_arch, gva_t a);
/**
 * Set the lcd's gva root pointer (for x86, %cr3) to the
 * guest physical address a.
 */
int lcd_arch_set_gva_root(struct lcd_arch *lcd_arch, gpa_t a);
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

#define MAX_PAT_ENTRY	7
#define VALID_PAT_TYPE	7

#endif /* ASM_X86_LCD_DOMAINS_CREATE_H */
