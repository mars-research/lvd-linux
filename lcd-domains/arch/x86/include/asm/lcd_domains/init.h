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
 * Initializes the arch-dependent code for LCD (detects required
 * features, turns on VMX on *all* cpu's).
 */
int lcd_arch_init(void);
/**
 * Turns off VMX on *all* cpu's and tears down arch-dependent code.
 * 
 * Important: All LCDs should be destroyed before calling this
 * routine (otherwise, memory will leak).
 */
void lcd_arch_exit(void);

/* Some internal data used by init */
extern struct lcd_vmx_capability lcd_vmx_capability;

#endif /* ASM_X86_LCD_DOMAINS_INIT_H */
