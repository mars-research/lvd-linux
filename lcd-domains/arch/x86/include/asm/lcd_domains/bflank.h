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
#include <linux/mm.h>

#include <lcd_domains/types.h>
#include <asm/lcd_domains/types.h>
#include <lcd_domains/microkernel.h>

#define LCD_BFCALL_ADD_EPT 0x4BF00031
#define LCD_BFCALL_DUMP_STACK 0x4BF00032

/**
 * Add the page that contains EPT ids for VMFUNC calls
 */
static inline unsigned int bfcall_install_vmfunc_ept_page(struct page *eptp_list_page)
{
	unsigned int eax, ebx, ecx, edx; 
	u64 kernel_ept_phys; 
	u64 *eptp_list; 


	eax = LCD_BFCALL_ADD_EPT;
	ebx = (unsigned int)page_to_phys(eptp_list_page);
	ecx = (unsigned int)(page_to_phys(eptp_list_page) >> 32); 
	edx = 0; 

	LCD_MSG("seting ept page 0x%llx, ebx:0x%lx, ecx:0x%lx",
		page_to_phys(eptp_list_page), ebx, ecx);

	/* ecx is often an input as well as an output. */
	asm volatile("cpuid"
	    : "=a" (eax),
	      "=b" (ebx),
	      "=c" (ecx),
	      "=d" (edx)
	    : "0" (eax), "1" (ebx), "2" (ecx), "3" (edx)
	    : "memory");

	LCD_MSG("set ept, eax:0x%lx, ebx:0x%lx, ecx:0x%lx",
			eax, ebx, ecx);

	/* Bareflank returns the physical address of the kernel EPT */
	kernel_ept_phys = (((u64)ecx) << 32) | ebx; 

	LCD_MSG("set ept[0]:0x%llx", kernel_ept_phys);

	eptp_list = phys_to_virt(page_to_phys(eptp_list_page)); 
	eptp_list[0] = kernel_ept_phys; 

	return eax; 
}

static inline unsigned int bfcall_dump_stack(void)
{
	unsigned int eax, ebx = 0, ecx = 0, edx = 0; 

	eax = LCD_BFCALL_DUMP_STACK; 
	
	LCD_MSG("Trigger dump stack\n");

	/* ecx is often an input as well as an output. */
	asm volatile("cpuid"
	    : "=a" (eax),
	      "=b" (ebx),
	      "=c" (ecx),
	      "=d" (edx)
	    : "0" (eax), "1" (ebx), "2" (ecx), "3" (edx)
	    : "memory");

	return eax; 
}


#endif /* ASM_X86_LCD_DOMAINS_BFLANK_H */
