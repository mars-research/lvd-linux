/*
 * VMFUNC support 
 *
 * Copyright: University of California, Irvine
 */
#ifndef ASM_X86_LCD_DOMAINS_VMFUNC_H
#define ASM_X86_LCD_DOMAINS_VMFUNC_H

void on_cpu_install_vmfunc_ept_page(void *info); 

struct gpa_hpa_pair {
	struct page *gpa;
	struct page *hpa;
};
void on_cpu_ept_map_page(void *gpa_hpa_pair);

int create_cpuid_pages(void);
void destroy_cpuid_pages(void);

#endif /* ASM_X86_LCD_DOMAINS_VMFUNC_H */
