/*
 * check.h
 * 
 * This implements the checks stipulated in the Intel manual.
 * 
 * The exit status information is not very informative if
 * you misconfigure the VMCS, so it can be hard to debug. So,
 * this does the checks for you.
 *
 * Copyright: University of Utah
 */
#ifndef ASM_X86_LCD_DOMAINS_CHECK_H
#define ASM_X86_LCD_DOMAINS_CHECK_H

#include <asm/lcd_domains/types.h>

/**
 * Does logical consistency checks (e.g., runs through checks
 * listed in Intel SDM V3 26.1, 26.2, and 26.3).
 */
int lcd_arch_check(struct lcd_arch *lcd_arch);


#endif /* ASM_X86_LCD_DOMAINS_CHECK_H */
