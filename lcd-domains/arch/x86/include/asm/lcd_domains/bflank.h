/*
 * bflank.h
 *
 * Interfaces to communicate with the Bareflank hypervisor
 *
 * Copyright: University of California, Irvine
 *
 */
#ifndef ASM_X86_LCD_DOMAINS_BFLANK_H
#define ASM_X86_LCD_DOMAINS_BFLANK_H

#include <asm/vmx.h>
#include <linux/kernel.h>

#include <lcd_domains/types.h>
#include <asm/lcd_domains/types.h>
#include <lcd_domains/microkernel.h>

#define LCD_BFCALL_ADD_EPT 0x4BF00031

/**
 * Add the page that contains EPT ids for VMFUNC calls
 */
static inline unsigned int bfcall_install_vmfunc_ept_page(u64 eptp)
{
	unsigned int eax, ebx, ecx, edx; 

	eax = LCD_BFCALL_ADD_EPT;
	ebx = (unsigned int)eptp;
	ecx = (unsigned int)(eptp >> 32); 
	edx = 0; 

	LCD_MSG("seting ept page 0x%llx, ebx:0x%lx, ecx:0x%lx\n",
		eptp, ebx, ecx);

	/* ecx is often an input as well as an output. */
	asm volatile("cpuid"
	    : "=a" (eax),
	      "=b" (ebx),
	      "=c" (ecx),
	      "=d" (edx)
	    : "0" (eax), "1" (ebx), "2" (ecx), "3" (edx)
	    : "memory");

	LCD_MSG("set ept, ret:%llx\n", eax);

	return eax; 
}

#endif /* ASM_X86_LCD_DOMAINS_BFLANK_H */
