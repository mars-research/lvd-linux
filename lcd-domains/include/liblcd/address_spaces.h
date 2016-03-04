/*
 * address_spaces.h
 *
 * The initial virtual and physical address spaces inside
 * an LCD.
 *
 * Copyright: University of Utah
 */
#ifndef LIBLCD_ADDRESS_SPACES_H
#define LIBLCD_ADDRESS_SPACES_H

#include <lcd_domains/types.h>
#include <asm/lcd_domains/address_spaces.h> /* contains layouts */

/* HELPERS -------------------------------------------------- */

/**
 * lcd_gva2gpa -- Convert a guest virtual to a guest physical address
 * @gva: the guest virtual address to convert
 *
 * For non-isolated code, this is equivalent to __pa.
 *
 * For isolated code, this is LCD_PHYS_BASE + (gva - LCD_VIRT_BASE)
 * Of course, this will give garbage results if gva is garbage.
 */
gpa_t lcd_gva2gpa(gva_t gva);
/**
 * lcd_gpa2gva -- Convert a guest physical to a guest virtual address
 * @gva: the guest physical address to convert
 *
 * For non-isolated code, this is equivalent to __va.
 *
 * For isolated code, this is LCD_VIRT_BASE + (gpa - LCD_PHYS_BASE)
 * Of course, this will give garbage results if gpa is garbage.
 */
gva_t lcd_gpa2gva(gpa_t gpa);


#endif /* LIBLCD_ADDRESS_SPACES_H */
