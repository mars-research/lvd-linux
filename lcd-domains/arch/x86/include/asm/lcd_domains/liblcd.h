/*
 * liblcd.h
 *
 * Low-level arch-dependent liblcd-internal details.
 *
 * Copyright: University of Utah
 */
#ifndef ASM_LCD_DOMAINS_LIBLCD_H
#define ASM_LCD_DOMAINS_LIBLCD_H

#ifdef CONFIG_LCD
#include <asm/lcd_domains/liblcd_vmcall.h>
#elif defined(CONFIG_LVD)
#include <asm/lcd_domains/liblcd_vmfunc.h>
#endif


#endif /* ASM_LCD_DOMAINS_LIBLCD_H */
