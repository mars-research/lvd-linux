/*
 * init.c
 *
 * Code for initializing the VMFUNC part of the microkernel for LVDs
 *
 * Copyright: University of Utah
 */

#include <lcd_domains/types.h>
#include <asm/lcd_domains/microkernel.h>
#include <lcd_domains/microkernel.h>
#include <asm/lcd_domains/vmfunc.h>
#include <asm/lcd_domains/bflank.h>

DEFINE_PER_CPU(struct page *, vmfunc_eptp_list_page);
DEFINE_PER_CPU(struct page *, vmfunc_state_hpa_page);
struct lcd_vmx_capability lcd_vmx_capability;
extern union vmfunc_state_page vmfunc_state_page;

static int vmx_alloc_vmfunc_ept_switching_page(void)
{
	int cpu;

	LCD_MSG("Initializing VMFUNC EPT list page\n");

	for_each_possible_cpu(cpu) {
		struct page *eptp_list_page;
		eptp_list_page = alloc_page(GFP_KERNEL | __GFP_ZERO);

		per_cpu(vmfunc_eptp_list_page, cpu) = eptp_list_page;
		if (!eptp_list_page)
			return -ENOMEM;

	}

	/* Install LCDs EPT on each CPU */
	on_each_cpu(on_cpu_install_vmfunc_ept_page, NULL, 1);

	bfcall_dump_stack();

	return 0;
}

static int vmx_map_vmfunc_state_page(void)
{
	int cpu;
	struct gpa_hpa_pair gpa_hpa_pair;

	LCD_MSG("Mapping VMFUNC state page on all CPUs\n");

	gpa_hpa_pair.gpa = virt_to_page(&vmfunc_state_page);

	for_each_possible_cpu(cpu) {
		struct page *page;
		union vmfunc_state_page *this_cpu_vmfunc_page;
		int ret;

		page = alloc_page(GFP_KERNEL | __GFP_ZERO);

		per_cpu(vmfunc_state_hpa_page, cpu) = page;
		if (!page)
			return -ENOMEM;

		gpa_hpa_pair.hpa = page;

		//memcpy(this_cpu_vmfunc_page, &vmfunc_state_page, sizeof(struct vmfunc_state_page));

		/* We have to set up general params, e.g, cpuid and in_kernel
		 * before mapping this page, however we need to setup gs and
		 * only then setup in_kernel = 0 on a specific CPU.
		 *
		 * We do this in the on_cpu_setup_vmfunc_page() function
		 *
		 * */

		this_cpu_vmfunc_page = page_to_virt(page);
		this_cpu_vmfunc_page->vmfunc_state.cpuid = cpu;
		this_cpu_vmfunc_page->vmfunc_state.in_kernel = 1;

		this_cpu_vmfunc_page->vmfunc_state.debug_stack = per_cpu(lvd_debug_stack_addr, cpu);

		ret = smp_call_function_single(cpu,
				on_cpu_ept_map_page,
				&gpa_hpa_pair, 1);
		if(ret)
			return -EIO;

		ret = smp_call_function_single(cpu,
				on_cpu_setup_vmfunc_page,
				this_cpu_vmfunc_page, 1);
		if(ret)
			return -EIO;

		LCD_MSG("vmfunc state page: gs_base:0x%lx, cpu:%d, debug_stack:%lx\n",
			this_cpu_vmfunc_page->vmfunc_state.kernel_gs_base,
			this_cpu_vmfunc_page->vmfunc_state.cpuid,
			this_cpu_vmfunc_page->vmfunc_state.debug_stack);

	}

	return 0;
}

int lcd_arch_vmfunc_init(void)
{
	int ret = 0;

	vmx_alloc_vmfunc_ept_switching_page();

	vmx_map_vmfunc_state_page();

	/*
	 * Init lcd_arch_thread cache (using instead of kmalloc since
	 * these structs need to be aligned properly)
	 */
	lcd_arch_cache = KMEM_CACHE(lcd_arch, 0);
	if (!lcd_arch_cache) {
		LCD_ERR("failed to set up kmem cache\n");
		ret = -ENOMEM;
	}

	return ret;
}

void lcd_arch_vmfunc_exit(void)
{
	kmem_cache_destroy(lcd_arch_cache);
}