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

	buf = (char *)lcd->ept.root;
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

static int test04_help(struct lcd_arch *lcd, gpa_t base)
{
	hpa_t actual;
	unsigned long off;

	for (off = 0; off < 0x40000; off += PAGE_SIZE) {
		if (lcd_arch_ept_gpa_to_hpa(lcd, gpa_add(base, off), &actual)) {
			printk(KERN_ERR "lcd arch : test04 failed lookup at %lx\n",
				gpa_val(gpa_add(base, off)));
			return -1;
		}
		if (hpa_val(actual) != gpa_val(gpa_add(base, off))) {
			printk(KERN_ERR "lcd arch : test04 expected hpa %lx got %lx\n",
				gpa_val(gpa_add(base, off)),
				hpa_val(actual));
			return -1;
		}
	}
	return 0;
}

static int test04(void)
{
	struct lcd_arch *lcd;
	gpa_t base;
	int ret;

	ret = -1;

	lcd = (struct lcd_arch *)kmalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		printk(KERN_ERR "lcd arch : test04 failed to alloc lcd\n");
		goto fail1;
	}
	if (vmx_init_ept(lcd)) {
		printk(KERN_ERR "lcd arch : test04 ept init failed\n");
		goto fail2;
	}

	/*
	 * Map 0x0 - 0x400000 (first 4 MBs, takes two page tables)
	 */
	if (lcd_arch_ept_map_range(lcd, __gpa(0), __hpa(0), 1024)) {
		printk(KERN_ERR "lcd arch: test04 failed to map first 4 MBs\n");
		goto fail3;
	}

	/*
	 * Map 0x40000000 - 0x40400000 (1GB -- 1GB + 4MBs)
	 */
	if (lcd_arch_ept_map_range(lcd, __gpa(1 << 30), __hpa(1 << 30), 1024)) {
		printk(KERN_ERR "lcd arch: test04 failed to map 2nd 4 MBs\n");
		goto fail4;
	}

	/*
	 * Map 0x8000000000 - 0x8000400000 (512GB -- 512GB + 4MBs)
	 */
	if (lcd_arch_ept_map_range(lcd, __gpa(1UL << 39), 
					__hpa(1UL << 39), 1024)) {
		printk(KERN_ERR "lcd arch: test04 failed to map 3rd 4 MBs\n");
		goto fail5;
	}

	/*
	 * CHECK
	 */

	base = __gpa(0);
	if (test04_help(lcd, base))
		goto fail6;
	base = __gpa(1 << 30);
	if (test04_help(lcd, base))
		goto fail6;
	base = __gpa(1UL << 39);
	if (test04_help(lcd, base))
		goto fail6;

	ret = 0;
	goto done;

done:
fail6:
	lcd_arch_ept_unmap_range(lcd, __gpa(1UL << 39), 1024);
fail5:
	lcd_arch_ept_unmap_range(lcd, __gpa(1 << 30), 1024);
fail4:
	lcd_arch_ept_unmap_range(lcd, __gpa(0), 1024);
fail3:
	vmx_free_ept(lcd);
fail2:
	kfree(lcd);
fail1:
	return ret;
}


static int test05(void)
{
	struct lcd_arch *lcd;
	hpa_t hpa;

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
	if (hpa_val(hpa) != hpa_val(va2hpa(lcd->gdt))) {
		printk(KERN_ERR "lcd arch : test05 unexpected gdt addr\n");
		goto fail_lookup;
	}

	vmx_destroy_gdt(lcd);
	vmx_free_ept(lcd);
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
	hpa_t hpa;

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
	if (hpa_val(hpa) != hpa_val(va2hpa(lcd->tss))) {
		printk(KERN_ERR "lcd arch : test06 unexpected tss addr\n");
		goto fail_lookup;
	}

	vmx_destroy_tss(lcd);
	vmx_free_ept(lcd);
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
	hpa_t hpa;

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
	if (hpa_val(hpa) != hpa_val(va2hpa(lcd->utcb))) {
		printk(KERN_ERR "lcd arch : test07 unexpected utcb addr\n");
		goto fail_lookup;
	}

	vmx_destroy_stack(lcd);
	vmx_free_ept(lcd);
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
	hva_t pgd;
	int ret;

	lcd = lcd_arch_create();
	if (!lcd) {
		printk(KERN_ERR "lcd arch : test08 failed to create lcd\n");
		ret = 1;
		goto fail1;
	}
	
	pgd = __hva(__get_free_page(GFP_KERNEL));
	if (!hva_val(pgd)) {
		printk(KERN_ERR "lcd arch: test08 failed to alloc page\n");
		ret = 1;
		goto fail2;
	}
	ret = lcd_arch_ept_map(lcd, __gpa(0), hva2hpa(pgd), 1, 0);
	if (ret) {
		printk(KERN_ERR "lcd arch: test08 error mapping pgd\n");
		goto fail3;
	}
	ret = lcd_arch_set_gva_root(lcd, __gpa(0));
	if (ret) {
		printk(KERN_ERR "lcd arch: test08 error setting gva root\n");
		goto fail4;
	}
	ret = lcd_arch_set_pc(lcd, __gva(0));
	if (ret) {
		printk(KERN_ERR "lcd arch: test08 error setting pc\n");
		goto fail5;
	}

	if (lcd_arch_check(lcd)) {
		printk(KERN_ERR "lcd arch: test08 failed a check\n");
		ret = -1;
		goto fail6;
	}

	ret = 0;
	goto done;
done:
fail6:
fail5:
fail4:
	lcd_arch_ept_unmap(lcd, __gpa(0));
fail3:
	free_page(hva_val(pgd));
fail2:
	lcd_arch_destroy(lcd);
fail1:
	return ret;
}

static int test09(void)
{
	if (!vmx_addr_is_canonical(0UL)) {
		printk(KERN_ERR "lcd arch: test09.1 failed\n");
		return -1;
	}
	if (vmx_addr_is_canonical(1UL << 63)) {
		printk(KERN_ERR "lcd arch: test09.2 failed\n");
		return -1;
	}
	if (vmx_addr_is_canonical(0xFFFFUL << 48)) {
		printk(KERN_ERR "lcd arch: test09.3 failed\n");
		return -1;
	}
	if (!vmx_addr_is_canonical(0xFFFF8UL << 44)) {
		printk(KERN_ERR "lcd arch: test09.4 failed\n");
		return -1;
	}
	if (!vmx_addr_is_canonical(0x00007UL << 44)) {
		printk(KERN_ERR "lcd arch: test09.5 failed\n");
		return -1;
	}
	
	return 0;
}

static int test10(void)
{
	u32 width;

	width = cpuid_eax(0x80000008) & 0xff;

	if (vmx_bad_phys_addr(0xff)) {
		printk(KERN_ERR "lcd vmx: test10.0 failed\n");
		return -1;
	}
	
	if (vmx_bad_phys_addr(1UL << (width - 1))) {
		printk(KERN_ERR "lcd vmx: test10.1 failed\n");
		return -1;
	}
	
	if (!vmx_bad_phys_addr(1UL << width)) {
		printk(KERN_ERR "lcd vmx: test10.2 failed\n");
		return -1;
	}

	if (!vmx_bad_phys_addr(-1ULL)) {
		printk(KERN_ERR "lcd vmx: test10.3 failed\n");
		return -1;
	}

	if (width >= 40 && vmx_bad_phys_addr(0x30682f000)) {
		printk(KERN_ERR "lcd vmx: test10.4 failed\n");
		return -1;
	}

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
	if (test09())
		return;
	if (test10())
		return;
	printk(KERN_ERR "lcd vmx: all tests passed!\n");
	return;
}
