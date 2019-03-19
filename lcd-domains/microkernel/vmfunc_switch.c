#include <linux/mm.h>
#include <lcd_domains/types.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <asm/lcd_domains/ept_lcd.h>
#include <asm/liblcd/address_spaces.h>
#include <libfipc.h>
#include <linux/kallsyms.h>

#define NUM_LCDS		5

extern struct lcd *lcd_list[NUM_LCDS];

int do_check(int ept)
{
	unsigned long vmfunc_load_addr;
	int ret;
	gpa_t gpa_vmfunc_lcd;
	hpa_t hpa_vmfunc;
	vmfunc_load_addr = kallsyms_lookup_name("__vmfunc_load_addr");
	gpa_vmfunc_lcd = isolated_lcd_gva2gpa(__gva(vmfunc_load_addr));
	ret = lcd_arch_ept_gpa_to_hpa_cpu(lcd_list[ept]->lcd_arch,
			gpa_vmfunc_lcd, &hpa_vmfunc, 1, smp_processor_id());
	if (ret) {
		printk("%s, Unable to find GPA to HPA mapping for gva:%lx, gpa:%lx, ret = %d\n",
				__func__, vmfunc_load_addr, gpa_val(gpa_vmfunc_lcd), ret);
		return ret;
	}
	printk("%s, Mapping found on LCDs EPT for GVA: %lx, GPA: %lx , HPA: %lx\n",
			__func__, vmfunc_load_addr, gpa_val(gpa_vmfunc_lcd),
			hpa_val(hpa_vmfunc));

	print_hex_dump(KERN_DEBUG, "vmfunc_lcd: ", DUMP_PREFIX_ADDRESS,
			       16, 1, __va(hpa_val(hpa_vmfunc)), 0x100, false);

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

	if (lcd) {
		hpa_lcd_cr3 = lcd->lcd_arch->hpa_cr3;

		LCD_MSG("Add mapping gpa: %lx, hpa: %lx", gpa_val(gpa_cr3),
				hpa_val(hpa_lcd_cr3));

		lcd_arch_ept_map_all_cpus(lcd_list[ept]->lcd_arch, gpa_cr3,
			hpa_lcd_cr3,
			1, /* create, if not present */
			0 /* don't overwrite, if present */);

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
	local_irq_disable();
	ret = vmfunc_wrapper(msg);
	local_irq_enable();
exit:
	return ret;
}

EXPORT_SYMBOL(vmfunc_klcd_wrapper);
