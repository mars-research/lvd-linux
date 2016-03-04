/*
 * microkernel.h
 *
 * x86 VT-x top-level include.
 *
 * Based on KVM and Dune.
 *
 * Authors:
 *
 *   Weibin Sun <wbsun@flux.utah.edu>
 *   Charlie Jacobsen <charlesj@cs.utah.edu>
 *   Anton Burtsev <aburtsev@cs.utah.edu>
 *
 * Copyright: University of Utah
 */
#ifndef ASM_X86_LCD_DOMAINS_MICROKERNEL_H
#define ASM_X86_LCD_DOMAINS_MICROKERNEL_H

#include <asm/vmx.h>
#include <linux/spinlock.h>
#include <linux/bitmap.h>
#include <linux/mutex.h>

#include <lcd_domains/types.h>
#include <asm/lcd_domains/types.h>
#include <asm/lcd_domains/ept.h>
#include <asm/lcd_domains/create.h>
#include <asm/lcd_domains/init.h>
#include <asm/lcd_domains/run.h>
#include <asm/lcd_domains/vmcs.h>

#endif  /* ASM_X86_LCD_DOMAINS_MICROKERNEL_H */
