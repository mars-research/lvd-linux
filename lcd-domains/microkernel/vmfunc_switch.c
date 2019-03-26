#include <linux/mm.h>
#include <lcd_domains/types.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <asm/lcd_domains/ept_lcd.h>
#include <asm/liblcd/address_spaces.h>
#include <libfipc.h>
#include <linux/kallsyms.h>

#define NUM_LCDS		5
/* this is the only function Intel VT-x support */
#define VM_FUNCTION	0

/* exported by the microkernel. We trust that it's sane */
extern void *cpuid_page;
extern struct lcd *lcd_list[NUM_LCDS];

int do_check(int ept)
{
	unsigned long vmfunc_load_addr;
	unsigned long vmfunc_sb_load_addr;
	int ret;
	static int once = 1;
	gpa_t gpa_vmfunc_lcd;
	gpa_t gpa_vmfunc_sb_lcd;
	hpa_t hpa_vmfunc;
	hpa_t hpa_vmfunc_sb;
	gpa_t gpa_lcd_stack;
	hpa_t hpa_lcd_stack;

	if (!once)
		return 0;
	vmfunc_load_addr = kallsyms_lookup_name("__vmfunc_trampoline_load_addr");
	vmfunc_sb_load_addr = kallsyms_lookup_name("__vmfunc_sboard_load_addr");
	gpa_vmfunc_lcd = isolated_lcd_gva2gpa(__gva(vmfunc_load_addr));
	gpa_vmfunc_sb_lcd = isolated_lcd_gva2gpa(__gva(vmfunc_sb_load_addr));
	gpa_lcd_stack = gpa_add(LCD_STACK_GP_ADDR, LCD_STACK_SIZE - sizeof(void*));

	ret = lcd_arch_ept_gpa_to_hpa_cpu(lcd_list[ept]->lcd_arch,
			gpa_vmfunc_lcd, &hpa_vmfunc, 1, smp_processor_id());
	if (ret) {
		printk("%s, Unable to find GPA to HPA mapping for vmfunc page! gva:%lx, gpa:%lx, ret = %d\n",
				__func__, vmfunc_load_addr, gpa_val(gpa_vmfunc_lcd), ret);
		return ret;
	}

	ret = lcd_arch_ept_gpa_to_hpa_cpu(lcd_list[ept]->lcd_arch,
			gpa_vmfunc_sb_lcd, &hpa_vmfunc_sb, 1, smp_processor_id());
	if (ret) {
		printk("%s, Unable to find GPA to HPA mapping for vmfunc page! gva:%lx, gpa:%lx, ret = %d\n",
				__func__, vmfunc_sb_load_addr, gpa_val(gpa_vmfunc_sb_lcd), ret);
		return ret;
	}

	printk("%s, Mapping found on LCDs EPT for trampoline GVA: %lx, GPA: %lx , HPA: %lx\n",
			__func__, vmfunc_load_addr, gpa_val(gpa_vmfunc_lcd),
			hpa_val(hpa_vmfunc));

	printk("%s, Mapping found on LCDs EPT for sboard GVA: %lx, GPA: %lx , HPA: %lx\n",
			__func__, vmfunc_sb_load_addr, gpa_val(gpa_vmfunc_sb_lcd),
			hpa_val(hpa_vmfunc_sb));


	if (once) {
		once = 0;
		print_hex_dump(KERN_DEBUG, ".vmfunc.trampoline.text.kernel: ", DUMP_PREFIX_ADDRESS,
			       16, 1, (void*) vmfunc_load_addr, 0x100, false);

		print_hex_dump(KERN_DEBUG, "vmfunc.trampoline.text.lcd: ", DUMP_PREFIX_ADDRESS,
			       16, 1, __va(hpa_val(hpa_vmfunc)), 0x100, false);

		print_hex_dump(KERN_DEBUG, ".vmfunc.sb.text.kernel: ", DUMP_PREFIX_ADDRESS,
			       16, 1, (void*) vmfunc_sb_load_addr, 0x100, false);

		print_hex_dump(KERN_DEBUG, "vmfunc.sb.text.lcd: ", DUMP_PREFIX_ADDRESS,
			       16, 1, __va(hpa_val(hpa_vmfunc_sb)), 0x100, false);

	}

	ret = lcd_arch_ept_gpa_to_hpa_cpu(lcd_list[ept]->lcd_arch,
			gpa_lcd_stack, &hpa_lcd_stack, 1, smp_processor_id());
	if (ret) {
		printk("%s, Unable to find GPA to HPA mapping for stack! gva:%lx, gpa:%lx, ret = %d\n",
				__func__, gva_val(isolated_lcd_gpa2gva(gpa_lcd_stack)), gpa_val(gpa_lcd_stack), ret);
		return ret;
	}

	printk("%s, Mapping found on LCDs EPT for stack GVA: %lx, GPA: %lx , HPA: %lx\n",
			__func__, gva_val(isolated_lcd_gpa2gva(gpa_lcd_stack)), gpa_val(gpa_lcd_stack),
			hpa_val(hpa_lcd_stack));

	return ret;
}

static int vmfunc_prepare_switch(int ept)
{
	struct lcd *lcd = NULL;
	phys_addr_t cr3_base;
	gpa_t gpa_cr3;
	hpa_t hpa_lcd_cr3;

	asm volatile("mov %%cr3, %[cr3_base]"
			: [cr3_base]"=r"(cr3_base));

	/* gpa is the same as phys_addr */
	gpa_cr3 = __gpa(cr3_base);

	lcd = lcd_list[ept];
	current->vmfunc_lcd = lcd;

	if (lcd && !current->cr3_remapping) {
		hpa_lcd_cr3 = lcd->lcd_arch->hpa_cr3;

		LCD_MSG("Add mapping gpa: %lx, hpa: %lx", gpa_val(gpa_cr3),
				hpa_val(hpa_lcd_cr3));

		lcd_arch_ept_map_all_cpus(lcd_list[ept]->lcd_arch, gpa_cr3,
			hpa_lcd_cr3,
			1, /* create, if not present */
			0 /* don't overwrite, if present */);

		current->cr3_remapping = 1;
		/* lcd_arch_ept_dump_this_cpu(lcd_list[ept]->lcd_arch); */
	}

	return 0;
}

int vmfunc_klcd_wrapper(struct fipc_message *msg, unsigned int ept)
{
	int ret;
	if (ept > 511) {
		ret = -EINVAL;
		goto exit;
	}
	vmfunc_prepare_switch(ept);
	ret = do_check(ept);
	if (ret)
		goto exit;
//	local_irq_disable();
	vmfunc_trampoline_entry(msg);
//	local_irq_enable();
exit:
	return ret;
}

int vmfunc_klcd_test_wrapper(struct fipc_message *msg, unsigned int ept, vmfunc_test_t test)
{
	int ret;
	if (ept > 511) {
		ret = -EINVAL;
		goto exit;
	}
	/* FIXME: This should be done only once per process.
	 * create a TLS variable to mark if cr3 switch is already active.
	 */
	vmfunc_prepare_switch(ept);
	ret = do_check(ept);
	if (ret)
		goto exit;
#if 1
//	local_irq_disable();
	ret = vmfunc_test_wrapper(msg, test);
//	local_irq_enable();
#endif
exit:
	return ret;
}

int noinline
//__vmfunc_wrapper
vmfunc_test_wrapper(struct fipc_message *request, vmfunc_test_t test)
{

	switch (test) {
	case VMFUNC_TEST_EMPTY_SWITCH:
		printk("%s: Invoking EMPTY_SWITCH test\n", __func__);
		vmfunc_call_empty_switch();
		break;
	case VMFUNC_TEST_DUMMY_CALL:
		/* only upto vmfunc_id 0x3 is handled */
		request->vmfunc_id = 0x4;
		printk("%s: Invoking DUMMY_CALL test with vmfunc_id: %d\n",
					__func__, request->vmfunc_id);
		vmfunc_trampoline_entry(request);
		break;
	case VMFUNC_TEST_RPC_CALL:
		request->vmfunc_id = VMFUNC_RPC_CALL;
		vmfunc_trampoline_entry(request);
		break;
	case VMFUNC_TEST_RPC_CALLBACK:
		request->vmfunc_id = VMFUNC_RPC_CALL;
		vmfunc_trampoline_entry(request);
		break;

	}
	return 0;
}

EXPORT_SYMBOL(vmfunc_klcd_test_wrapper);
EXPORT_SYMBOL(vmfunc_klcd_wrapper);
EXPORT_SYMBOL(vmfunc_test_wrapper);
