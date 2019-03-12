#include <asm/lcd_domains/bflank.h> 

/*  Add LCD's EPT to the hypervisor */

void on_cpu_install_vmfunc_ept_page(void *info) {
	struct page *eptp_list_page = __this_cpu_read(vmfunc_eptp_list_page);

	bfcall_install_vmfunc_ept_page(eptp_list_page);  	
	return; 
};
