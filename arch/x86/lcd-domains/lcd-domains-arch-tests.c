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
	vmx_free_vpid(lcd);
	if (vmx_allocate_vpid(lcd)) {
		printk(KERN_ERR "lcd arch : test02 2nd vpid alloc failed\n");
		goto fail;
	}
	if (lcd->vpid != 1) {
		printk(KERN_ERR "lcd arch : test02 2nd actual vpid = %d\n",
			lcd->vpid);
		goto fail_free;
	}

	vmx_free_vpid(lcd);
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
	u64 base;
	u64 actual;
	u64 off;

	lcd = (struct lcd_arch *)kmalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		printk(KERN_ERR "lcd arch : test04 failed to alloc lcd\n");
		goto fail_alloc;
	}
	if (vmx_init_ept(lcd)) {
		printk(KERN_ERR "lcd arch : test04 ept init failed\n");
		goto fail;
	}

	/*
	 * Map 0x0 - 0x400000 (first 4 MBs, takes two page dirs)
	 */
	base = 0;
	for (off = 0; off < 0x40000; off += PAGE_SIZE) {
		if (lcd_arch_ept_map_gpa_to_hpa(lcd, base + off,
							base + off,
							1,
							0)) {
			printk(KERN_ERR "lcd arch : test04 failed to map %lx\n",
				(unsigned long)(base + off));
			goto fail_map;
		}
	}

	/*
	 * Map 0x40000000 - 0x40400000 (1GB -- 1GB + 4MBs, takes two page dirs)
	 */
	base = 0x40000000;
	for (off = 0; off < 0x40000; off += PAGE_SIZE) {
		if (lcd_arch_ept_map_gpa_to_hpa(lcd, base + off,
							base + off,
							1,
							0)) {
			printk(KERN_ERR "lcd arch : test04 failed to map %lx\n",
				(unsigned long)(base + off));
			goto fail_map;
		}
	}

	/*
	 * Map 0x8000000000 - 0x8000400000 (512GB -- 512GB + 4MBs, 
	 * takes two page dirs)
	 */
	base = 0x8000000000;
	for (off = 0; off < 0x40000; off += PAGE_SIZE) {
		if (lcd_arch_ept_map_gpa_to_hpa(lcd, base + off,
							base + off,
							1,
							0)) {
			printk(KERN_ERR "lcd arch : test04 failed to map %lx\n",
				(unsigned long)(base + off));
			goto fail_map;
		}
	}

	/*
	 * CHECK
	 */

	base = 0;
	for (off = 0; off < 0x40000; off += PAGE_SIZE) {
		if (lcd_arch_ept_gpa_to_hpa(lcd, base + off, &actual)) {
			printk(KERN_ERR "lcd arch : test04 failed lookup at %lx\n",
				(unsigned long)(base + off));
			goto fail_map;
		}
		if (actual != (base + off)) {
			printk(KERN_ERR "lcd arch : test04 expected hpa %lx got %lx\n",
				(unsigned long)(base + off),
				(unsigned long)actual);

			goto fail_map;
		}
	}

	base = 0x40000000;
	for (off = 0; off < 0x40000; off += PAGE_SIZE) {
		if (lcd_arch_ept_gpa_to_hpa(lcd, base + off, &actual)) {
			printk(KERN_ERR "lcd arch : test04 failed lookup at %lx\n",
				(unsigned long)(base + off));
			goto fail_map;
		}
		if (actual != (base + off)) {
			printk(KERN_ERR "lcd arch : test04 expected hpa %lx got %lx\n",
				(unsigned long)(base + off),
				(unsigned long)actual);

			goto fail_map;
		}
	}

	base = 0x8000000000;
	for (off = 0; off < 0x40000; off += PAGE_SIZE) {
		if (lcd_arch_ept_gpa_to_hpa(lcd, base + off, &actual)) {
			printk(KERN_ERR "lcd arch : test04 failed lookup at %lx\n",
				(unsigned long)(base + off));
			goto fail_map;
		}
		if (actual != (base + off)) {
			printk(KERN_ERR "lcd arch : test04 expected hpa %lx got %lx\n",
				(unsigned long)(base + off),
				(unsigned long)actual);

			goto fail_map;
		}
	}

	vmx_free_ept(lcd);
	kfree(lcd);

	return 0;

fail_map:
	vmx_free_ept(lcd);
fail:
	kfree(lcd);
fail_alloc:
	return -1;
}


static int test05(void)
{
	struct lcd_arch *lcd;
	u64 hpa;

	lcd = (struct lcd_arch *)kmalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		printk(KERN_ERR "lcd arch : test05 failed to alloc lcd\n");
		goto fail_alloc;
	}
	if (vmx_init_ept(lcd)) {
		printk(KERN_ERR "lcd arch : test05 ept init failed\n");
		goto fail_ept;
	}
	if (vmx_init_gdt(lcd)) {
		printk(KERN_ERR "lcd arch : test05 gdt init failed\n");
		goto fail_gdt;
	}	
	if (lcd_arch_ept_gpa_to_hpa(lcd, LCD_ARCH_GDTR_BASE, &hpa)) {
		printk(KERN_ERR "lcd arch : test05 lookup failed\n");
		goto fail_lookup;
	}
	if (hpa != __pa(lcd->gdt)) {
		printk(KERN_ERR "lcd arch : test05 unexpected gdt addr\n");
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

static int test06(void)
{
	struct lcd_arch *lcd;
	u64 hpa;

	lcd = (struct lcd_arch *)kmalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		printk(KERN_ERR "lcd arch : test06 failed to alloc lcd\n");
		goto fail_alloc;
	}
	if (vmx_init_ept(lcd)) {
		printk(KERN_ERR "lcd arch : test06 ept init failed\n");
		goto fail_ept;
	}
	if (vmx_init_tss(lcd)) {
		printk(KERN_ERR "lcd arch : test06 tss init failed\n");
		goto fail_tss;
	}	
	if (lcd_arch_ept_gpa_to_hpa(lcd, LCD_ARCH_TSS_BASE, &hpa)) {
		printk(KERN_ERR "lcd arch : test06 lookup failed\n");
		goto fail_lookup;
	}
	if (hpa != __pa(lcd->tss)) {
		printk(KERN_ERR "lcd arch : test06 unexpected tss addr\n");
		goto fail_lookup;
	}

	vmx_free_ept(lcd); /* frees tss */
	kfree(lcd);
	return 0;

fail_lookup:
fail_tss:
	vmx_free_ept(lcd);
fail_ept:
	kfree(lcd);
fail_alloc:
	return -1;
}

static int test07(void)
{
	struct lcd_arch *lcd;
	u64 hpa;

	lcd = (struct lcd_arch *)kmalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		printk(KERN_ERR "lcd arch : test07 failed to alloc lcd\n");
		goto fail_alloc;
	}
	if (vmx_init_ept(lcd)) {
		printk(KERN_ERR "lcd arch : test07 ept init failed\n");
		goto fail_ept;
	}
	if (vmx_init_stack(lcd)) {
		printk(KERN_ERR "lcd arch : test07 stack init failed\n");
		goto fail_stack;
	}	
	if (lcd_arch_ept_gpa_to_hpa(lcd, LCD_ARCH_UTCB, &hpa)) {
		printk(KERN_ERR "lcd arch : test07 lookup failed\n");
		goto fail_lookup;
	}
	if (hpa != __pa(lcd->utcb)) {
		printk(KERN_ERR "lcd arch : test07 unexpected utcb addr\n");
		goto fail_lookup;
	}

	vmx_free_ept(lcd); /* frees stack */
	kfree(lcd);
	return 0;

fail_lookup:
fail_stack:
	vmx_free_ept(lcd);
fail_ept:
	kfree(lcd);
fail_alloc:
	return -1;
}

static int test08(void)
{
	struct lcd_arch *lcd;

	lcd = lcd_arch_create();
	if (!lcd) {
		printk(KERN_ERR "lcd arch : test08 failed to create lcd\n");
		return -1;
	}

	lcd_arch_destroy(lcd);
	return 0;
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
	if (test05())
		return;
	if (test06())
		return;
	if (test07())
		return;
	if (test08())
		return;
	return;
}
