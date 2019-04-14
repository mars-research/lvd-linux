
#include <asm/desc.h>
#include <asm/tlbflush.h>
#include <asm/lcd_domains/bflank.h> 
#include <asm/lcd_domains/vmfunc.h>

/*  Add LCD's EPT to the hypervisor */

void on_cpu_install_vmfunc_ept_page(void *info) {
	struct page *eptp_list_page = __this_cpu_read(vmfunc_eptp_list_page);

	bfcall_install_vmfunc_ept_page(eptp_list_page);	
	return; 
};

/*  Add an GPA to HPA mapping to kernel's EPT on this CPU */
void on_cpu_ept_map_page(void *info) {
	struct gpa_hpa_pair *pair = (struct gpa_hpa_pair *)info; 
	struct page *gpa = pair->gpa;
	struct page *hpa = pair->hpa;

	bfcall_ept_map_page(gpa, hpa); 
	return; 
};

static inline unsigned long read_gs_base(void) {
	unsigned long rax;

	asm volatile("rdgsbase %%rax"
	    : "=a" (rax)
	    : 
	    : "memory");

	return rax; 
}


void on_cpu_setup_vmfunc_page(void *vmfunc_page) {

	union vmfunc_state_page *this_cpu_vmfunc_page = vmfunc_page; 

	/* set X86_CR4_FSGSBASE bit so we can read/write gs base */
	cr4_set_bits(X86_CR4_FSGSBASE);
	this_cpu_vmfunc_page->vmfunc_state.kernel_gs_base = read_gs_base(); 
	//this_cpu_vmfunc_page->vmfunc_state.in_kernel = 0; 
	return; 
};
