/*
 * init.h
 *
 * Functions for initializing the VT-x code itself.
 *
 * Copyright: University of Utah
 */
#ifndef ASM_X86_LCD_DOMAINS_INIT_H
#define ASM_X86_LCD_DOMAINS_INIT_H

/**
 * Initializes VMFUNC for LVDs (EPT switching, state pages).
 */
int lcd_arch_vmfunc_init(void);

/**
 * Tears down VMFUNC resources.
 *
 * Important: All LCDs should be destroyed before calling this
 * routine (otherwise, memory will leak).
 */
void lcd_arch_vmfunc_exit(void);

/* Some internal data used by init */
extern struct lcd_vmx_capability lcd_vmx_capability;

#endif /* ASM_X86_LCD_DOMAINS_INIT_H */
