/**
 * Regression tests for lcd arch code.
 *
 * Included in lcd-domains-arch.c and
 * ran last in lcd_arch_init.
 */

static int test01(void)
{
	struct lcd_arch_vmcs *vmcs;
	vmcs = vmx_alloc_vmcs(raw_smp_processor_id());
	if (!vmcs) {
		printk(KERN_ERR "lcd arch: test01 failed\n");
		return -1;
	}
	vmx_free_vmcs(vmcs);
	return 0;
}

static int test02(void)
{
	struct lcd_arch *lcd;
	lcd = (struct lcd_arch *)kmalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		printk(KERN_ERR "lcd arch : test02 failed to alloc lcd\n");
		goto fail_alloc;
	}
	if (vmx_allocate_vpid(lcd)) {
		printk(KERN_ERR "lcd arch : test02 vpid alloc failed\n");
		goto fail;
	}
	if (lcd->vpid != 1) {
		printk(KERN_ERR "lcd arch : test02 actual vpid = %d\n",
			lcd->vpid);
		goto fail_free;
	}
	if (vmx_allocate_vpid(lcd)) {
		printk(KERN_ERR "lcd arch : test02 2nd vpid alloc failed\n");
		goto fail;
	}
	if (lcd->vpid != 1) {
		printk(KERN_ERR "lcd arch : test02 2nd actual vpid = %d\n",
			lcd->vpid);
		goto fail_free;
	}

	kfree(lcd);
	return 0;

fail_free:
	vmx_free_vpid(lcd);
fail:
	kfree(lcd);
fail_alloc:
	return -1;
}

static int test03(void)
{
	struct lcd_arch *lcd;
	int i;
	char *buf;
	lcd = (struct lcd_arch *)kmalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		printk(KERN_ERR "lcd arch : test03 failed to alloc lcd\n");
		goto fail_alloc;
	}
	if (vmx_init_ept(lcd)) {
		printk(KERN_ERR "lcd arch : test03 ept init failed\n");
		goto fail;
	}

	buf = (char *)__va(lcd->ept.root_hpa);
	for (i = 0; i < PAGE_SIZE; i++) {
		if (buf[i]) {
			printk(KERN_ERR "lcd arch : test03 nonzero in ept\n");
			goto fail_mem;
		}
	}		

	vmx_free_ept(lcd);
	kfree(lcd);

	return 0;

fail_mem:
	vmx_free_ept(lcd);
fail:
	kfree(lcd);
fail_alloc:
	return -1;
}

static int test04(void)
{
	struct lcd_arch *lcd;
	u64 hpa;

	lcd = (struct lcd_arch *)kmalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		printk(KERN_ERR "lcd arch : test04 failed to alloc lcd\n");
		goto fail_alloc;
	}
	if (vmx_init_ept(lcd)) {
		printk(KERN_ERR "lcd arch : test04 ept init failed\n");
		goto fail_ept;
	}
	if (vmx_init_gdt(lcd)) {
		printk(KERN_ERR "lcd arch : test04 gdt init failed\n");
		goto fail_gdt;
	}	
	if (lcd_arch_ept_gpa_to_hpa(lcd, LCD_ARCH_GDT_BASE, &hpa)) {
		printk(KERN_ERR "lcd arch : test04 lookup failed\n");
		goto fail_lookup;
	}
	if (hpa != __pa(lcd->gdt)) {
		printk(KERN_ERR "lcd arch : test04 unexpected gdt addr\n");
		goto fail_lookup;
	}

	vmx_free_ept(lcd); /* frees gdt */
	kfree(lcd);
	return 0;

fail_lookup:
fail_gdt:
	vmx_free_ept(lcd);
fail_ept:
	kfree(lcd);
fail_alloc:
	return -1;
}

static void lcd_arch_tests(void)
{
	if (test01())
		return;
	if (test02())
		return;
	if (test03())
		return;
	if (test04())
		return;

	return;
}
