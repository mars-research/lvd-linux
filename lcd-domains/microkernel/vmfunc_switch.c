#include <linux/mm.h>
#include <lcd_domains/types.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <asm/lcd_domains/ept_lcd.h>
#include <libfipc.h>

#define NUM_LCDS		5

extern struct lcd *lcd_list[NUM_LCDS];

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

		lcd_arch_ept_map_this_cpu(lcd_list[ept]->lcd_arch, gpa_cr3,
			hpa_lcd_cr3,
			1, /* create, if not present */
			0 /* don't overwrite, if present */);

		lcd_arch_ept_dump_this_cpu(lcd_list[ept]->lcd_arch);
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
	local_irq_disable();
	ret = vmfunc_wrapper(msg);
	local_irq_enable();
exit:
	return ret;
}

EXPORT_SYMBOL(vmfunc_klcd_wrapper);
