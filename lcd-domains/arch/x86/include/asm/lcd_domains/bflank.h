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
#define LCD_BFCALL_ABORT 0x4BF00033
#define LCD_BFCALL_MAP_PAGE 0x4BF00034
#define LCD_BFCALL_GUEST_PAGE_WALK 0x4BF00036
#define LCD_BFCALL_DUMP_PERF 0x4BF00037

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

/*  Add an GPA to HPA mapping to kernel's EPT on this CPU */
static inline unsigned int bfcall_ept_map_page(struct page *gpa, struct page *hpa)
{
	unsigned int eax, edx;
	u64 rbx, rcx; 


	eax = LCD_BFCALL_MAP_PAGE;
	rbx = page_to_phys(gpa);
	rcx = page_to_phys(hpa); 

	LCD_MSG("mapping a page gpa: 0x%llx, hpa:0x%llx",
		rbx, rcx);

	/* ecx is often an input as well as an output. */
	asm volatile("cpuid"
	    : "=a" (eax),
	      "=b" (rbx),
	      "=c" (rcx),
	      "=d" (edx)
	    : "0" (eax), "1" (rbx), "2" (rcx)
	    : "memory");

	return eax; 
}

static inline unsigned int bfcall_guest_page_walk(unsigned long gva, unsigned long cr3_base, unsigned int eptp_idx)
{
	unsigned int eax, edx;
	u64 rbx, rcx, r8;


	eax = LCD_BFCALL_GUEST_PAGE_WALK;
	rbx = (u64) gva;
	rcx = (u64) cr3_base;
	/* do not dump the whole vcpu state */
	edx = 0;
	r8 = (u64) eptp_idx; 

	LCD_MSG("%s page_walk for gva: %llx, cr3: 0x%llx",
		__func__, rbx, rcx);

	/* ecx is often an input as well as an output. */
	asm volatile(
		"mov %[r8], %%r8	\n\t"
		"cpuid"
	    : "=a" (eax),
	      "=b" (rbx),
	      "=c" (rcx),
	      "=d" (edx)
	    : "0" (eax), "1" (rbx), "2" (rcx), "3" (edx), [r8]"r"(r8)
	    : "memory", "r8");

	return eax;
}

static inline unsigned int bfcall_dump_perf(void)
{
	unsigned int eax, ebx = 0, ecx = 0, edx = 0; 

	eax = LCD_BFCALL_DUMP_PERF; 
	
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
