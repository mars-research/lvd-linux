#include <asm/lcd_domains/bflank.h> 

/*  Add LCD's EPT to the hypervisor */

void on_cpu_install_vmfunc_ept_page(void *info) {
	struct page *ept_pg = __this_cpu_read(vmfunc_epts_page);

	bfcall_install_vmfunc_ept_page((u64)ept_pg);  	
	return; 
};
