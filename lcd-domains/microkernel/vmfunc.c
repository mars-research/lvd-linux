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
