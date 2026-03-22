/*
 * run.c
 *
 * Code for running and handling VM exits for LCDs
 *
 * Copyright: University of Utah
 */

#include <lcd_domains/types.h>
#include <asm/lcd_domains/types.h>
#include <asm/lcd_domains/microkernel.h>

DEFINE_PER_CPU(struct lcd_arch *, local_lcd_arch);
