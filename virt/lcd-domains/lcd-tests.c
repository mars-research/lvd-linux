/**
 * Regression tests for lcd code.
 *
 * Included in lcd-domains.c and
 * ran last in lcd_init.
 */

static int test01(void)
{
	struct lcd *lcd;
	int ret;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test01 failed to create lcd\n");
		goto fail;
	}

	lcd_destroy(lcd);

	return 0;

fail:
	return ret;
}

static int test02(void)
{
	struct lcd *lcd;
	int ret;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test02 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			LCD_ARCH_FREE + 4 * (1 << 20));
	if (ret) {
		printk(KERN_ERR "lcd test: test02 failed to init gva\n");
		goto fail2;
	}

	lcd_destroy(lcd);

	return 0;

fail2:
	lcd_destroy(lcd);
fail1:
	return ret;
}

static int test03(void)
{
	struct lcd *lcd;
	int ret;
	u64 gpa;
	u64 hpa;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test03 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			LCD_ARCH_FREE + 4 * (1 << 20));
	if (ret) {
		printk(KERN_ERR "lcd test: test03 failed to init gva\n");
		goto fail2;
	}

	ret = lcd_mm_gva_alloc(lcd, &gpa, &hpa);
	if (ret) {
		printk(KERN_ERR "lcd test: test03 failed to alloc pg mem\n");
		goto fail3;
	}
	
	/*
	 * The root pgd takes up the first page, so this gpa should be
	 * one page above
	 */
	if (gpa != LCD_ARCH_FREE + PAGE_SIZE) {
		printk(KERN_ERR "lcd test: test03 gpa at wrond addr %lx\n",
			(unsigned long)gpa);
		ret = -1;
		goto fail4;
	}		

	free_page((u64)__va(hpa));
	lcd_arch_ept_unmap_range(lcd->lcd_arch, gpa, 1);

	lcd_destroy(lcd);

	return 0;

fail4:
	free_page((u64)__va(hpa));
	lcd_arch_ept_unmap_range(lcd->lcd_arch, gpa, 1);
fail3:
fail2:
	lcd_destroy(lcd);
fail1:
	return ret;
}

static int test04(void)
{
	struct lcd *lcd;
	int ret;
	u64 gpa;
	u64 hpa;
	pmd_t *pmd_entry;
	pte_t *pt;
	pte_t *pte_entry;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test04 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			LCD_ARCH_FREE + 4 * (1 << 20));
	if (ret) {
		printk(KERN_ERR "lcd test: test04 failed to init gva\n");
		goto fail2;
	}

	ret = lcd_mm_gva_alloc(lcd, &gpa, &hpa);
	if (ret) {
		printk(KERN_ERR "lcd test: test04 failed to alloc pg mem\n");
		goto fail3;
	}

	/*
	 * Map gva = 0x4000 (start of 5th page frame) to gpa = 0x1234000UL
	 */
	pt = (pte_t *)__va(hpa);
	lcd_mm_gva_set(pt + pte_index(0x4000UL), 0x1234000UL);

	/*
	 * Set up pmd entry for look up, and find pte
	 */
	pmd_entry = (pmd_t *)kmalloc(sizeof(*pmd_entry), GFP_KERNEL);
	if (!pmd_entry) {
		goto fail4;
	}

	set_pmd(pmd_entry, __pmd(gpa | _KERNPG_TABLE));
	ret = lcd_mm_gva_lookup_pte(lcd, 0x4000UL, pmd_entry, &pte_entry);
	if (ret) {
		printk(KERN_ERR "lcd test: test04 failed to lookup pte\n");
		goto fail5;
	}

	/*
	 * Check
	 */
	if (lcd_mm_gva_get(pte_entry) != 0x1234000UL) {
		printk(KERN_ERR "lcd test: test04 pte gpa is %lx\n",
			(unsigned long)lcd_mm_gva_get(pte_entry));
		ret = -1;
		goto fail6;
	}

	free_page((u64)__va(hpa));
	lcd_arch_ept_unmap_range(lcd->lcd_arch, gpa, 1);
	kfree(pmd_entry);

	lcd_destroy(lcd);

	return 0;

fail6:
fail5:
	kfree(pmd_entry);
fail4:
	free_page((u64)__va(hpa));
	lcd_arch_ept_unmap_range(lcd->lcd_arch, gpa, 1);
fail3:
fail2:
	lcd_destroy(lcd);
fail1:
	return ret;
}

static int test05(void)
{
	struct lcd *lcd;
	int ret;
	u64 gpa;
	u64 hpa;
	pud_t *pud_entry;
	pmd_t *pmd;
	pmd_t *pmd_entry;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test05 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			LCD_ARCH_FREE + 4 * (1 << 20));
	if (ret) {
		printk(KERN_ERR "lcd test: test05 failed to init gva\n");
		goto fail2;
	}

	ret = lcd_mm_gva_alloc(lcd, &gpa, &hpa);
	if (ret) {
		printk(KERN_ERR "lcd test: test05 failed to alloc pg mem\n");
		goto fail3;
	}

	/*
	 * Populate 5th entry in pmd to point to a (bogus) page table at
	 * gpa 0x1234000UL.
	 */
	pmd = (pmd_t *)__va(hpa);
	set_pmd(pmd + 4, __pmd(0x1234000UL));

	/*
	 * Set up pud entry for look up, and find pmd
	 */
	pud_entry = (pud_t *)kmalloc(sizeof(*pud_entry), GFP_KERNEL);
	if (!pud_entry) {
		goto fail4;
	}

	set_pud(pud_entry, __pud(gpa | _KERNPG_TABLE));
	ret = lcd_mm_gva_lookup_pmd(lcd, 0x4UL << PMD_SHIFT, 
				pud_entry, &pmd_entry);
	if (ret) {
		printk(KERN_ERR "lcd test: test05 failed to lookup pmd\n");
		goto fail5;
	}

	/*
	 * Check
	 */
	if (pmd_pfn(*pmd_entry) << PAGE_SHIFT != 0x1234000UL) {
		printk(KERN_ERR "lcd test: test05 pte gpa is %lx\n",
			(unsigned long)pmd_pfn(*pmd_entry) << PAGE_SHIFT);
		ret = -1;
		goto fail6;
	}

	free_page((u64)__va(hpa));
	lcd_arch_ept_unmap_range(lcd->lcd_arch, gpa, 1);
	kfree(pud_entry);

	lcd_destroy(lcd);

	return 0;

fail6:
fail5:
	kfree(pud_entry);
fail4:
	free_page((u64)__va(hpa));
	lcd_arch_ept_unmap_range(lcd->lcd_arch, gpa, 1);
fail3:
fail2:
	lcd_destroy(lcd);
fail1:
	return ret;
}

static int test06(void)
{
	struct lcd *lcd;
	int ret;
	u64 gpa;
	u64 hpa;
	pgd_t *pgd_entry;
	pud_t *pud;
	pud_t *pud_entry;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test06 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			LCD_ARCH_FREE + 4 * (1 << 20));
	if (ret) {
		printk(KERN_ERR "lcd test: test06 failed to init gva\n");
		goto fail2;
	}

	ret = lcd_mm_gva_alloc(lcd, &gpa, &hpa);
	if (ret) {
		printk(KERN_ERR "lcd test: test06 failed to alloc pg mem\n");
		goto fail3;
	}

	/*
	 * Populate 5th entry in pud to point to a (bogus) pmd at
	 * gpa 0x1234000UL.
	 */
	pud = (pud_t *)__va(hpa);
	set_pud(pud + 4, __pud(0x1234000UL));

	/*
	 * Set up pgd entry for look up, and find pud
	 */
	pgd_entry = kmalloc(sizeof(*pgd_entry), GFP_KERNEL);
	if (!pgd_entry) {
		goto fail4;
	}

	set_pgd(pgd_entry, __pgd(gpa | _KERNPG_TABLE));
	ret = lcd_mm_gva_lookup_pud(lcd, 0x4UL << PUD_SHIFT, 
				pgd_entry, &pud_entry);
	if (ret) {
		printk(KERN_ERR "lcd test: test06 failed to lookup pud\n");
		goto fail5;
	}

	/*
	 * Check
	 */
	if (pud_pfn(*pud_entry) << PAGE_SHIFT != 0x1234000UL) {
		printk(KERN_ERR "lcd test: test06 pmd gpa is %lx\n",
			(unsigned long)pud_pfn(*pud_entry) << PAGE_SHIFT);
		ret = -1;
		goto fail6;
	}

	free_page((u64)__va(hpa));
	lcd_arch_ept_unmap_range(lcd->lcd_arch, gpa, 1);
	kfree(pgd_entry);

	lcd_destroy(lcd);

	return 0;

fail6:
fail5:
	kfree(pgd_entry);
fail4:
	free_page((u64)__va(hpa));
	lcd_arch_ept_unmap_range(lcd->lcd_arch, gpa, 1);
fail3:
fail2:
	lcd_destroy(lcd);
fail1:
	return ret;
}

static int test07(void)
{
	struct lcd *lcd;
	int ret;
	u64 gpa;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test07 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			LCD_ARCH_FREE + 4 * (1 << 20));
	if (ret) {
		printk(KERN_ERR "lcd test: test07 failed to init gva\n");
		goto fail2;
	}

	/*
	 * Map gva 0x1234000UL to gpa 0x5678000UL
	 */
	ret = lcd_mm_gva_map(lcd, 0x1234000UL, 0x5678000UL);
	if (ret) {
		printk(KERN_ERR "lcd test: test07 failed to map\n");
		goto fail3;
	}

	/*
	 * Check
	 */
	ret = lcd_mm_gva_to_gpa(lcd, 0x1234000UL, &gpa);
	if (ret) {
		printk(KERN_ERR "lcd test: test07 failed to lookup\n");
		goto fail4;
	}

	if (gpa != 0x5678000UL) {
		printk(KERN_ERR "lcd test: test07 got phys addr %lx\n",
			(unsigned long)gpa);
		goto fail5;
	}

	lcd_destroy(lcd);

	return 0;

fail5:
fail4:
fail3:
fail2:
	lcd_destroy(lcd);
fail1:
	return ret;
}

static int test08(void)
{
	struct lcd *lcd;
	int ret;
	pgd_t *pgd_entry1;
	pgd_t *pgd_entry2;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test08 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE, 
			LCD_ARCH_FREE + 2 * PAGE_SIZE);
	if (ret) {
		printk(KERN_ERR "lcd test: test08 failed to init gva\n");
		goto fail2;
	}

	ret = lcd_mm_gva_walk_pgd(lcd, 0x1234000UL, &pgd_entry1);
	if (ret) {
		printk(KERN_ERR "lcd test: test08 failed to walk pgd\n");
		goto fail3;
	}

	ret = lcd_mm_gva_walk_pgd(lcd, 0x1234000UL, &pgd_entry2);
	if (ret) {
		printk(KERN_ERR "lcd test: test08 failed to walk pgd2\n");
		goto fail4;
	}

	if (pgd_entry1 != pgd_entry2) {
		printk(KERN_ERR "lcd test: test08 entries differ\n");
		ret = -1;
		goto fail5;
	}

	if (!pgd_present(*pgd_entry1) || !pgd_present(*pgd_entry2)) {
		printk(KERN_ERR "lcd test: test08 entries not present\n");
		ret = -1;
		goto fail5;
	}

	lcd_destroy(lcd);

	return 0;

fail5:
fail4:
fail3:
fail2:
	lcd_destroy(lcd);
fail1:
	return ret;
}

#if 0
static int test09(void)
{
	struct lcd *lcd;
	int ret;
	u64 base;
	u64 off;
	u64 actual;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test09 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			LCD_ARCH_FREE + 4 * (1 << 20));
	if (ret) {
		printk(KERN_ERR "lcd test: test09 failed to init gva\n");
		goto fail2;
	}

	/*
	 * Map 0x0 - 0x400000 (first 4 MBs, takes two page dirs)
	 */
	ret = lcd_mm_gva_map_range(lcd, 0, 0, 1024);
	if (ret) {
		printk(KERN_ERR "lcd test: test09 failed to map first 4 MBs\n");
		goto fail3;
	}

	/*
	 * Map 0x40000000 - 0x40400000 (1GB -- 1GB + 4MBs, takes two page dirs)
	 */
	ret = lcd_mm_gva_map_range(lcd, 0x40000000, 0x40000000, 1024);
	if (ret) {
		printk(KERN_ERR "lcd test: test09 failed to map 2nd 4 MBs\n");
		goto fail3;
	}

	/*
	 * Map 0x8000000000 - 0x8000400000 (512GB -- 512GB + 4MBs, 
	 * takes two page dirs)
	 */
	ret = lcd_mm_gva_map_range(lcd, 0x8000000000, 0x8000000000, 1024);
	if (ret) {
		printk(KERN_ERR "lcd test: test09 failed to map 3rd 4 MBs\n");
		goto fail3;
	}

	/*
	 * CHECK
	 */

	base = 0;
	for (off = 0; off < 0x40000; off += PAGE_SIZE) {
		if (lcd_mm_gva_to_gpa(lcd, base + off, &actual)) {
			printk(KERN_ERR "lcd test: test09 failed lookup at %lx\n",
				(unsigned long)(base + off));
			goto fail3;
		}
		if (actual != (base + off)) {
			printk(KERN_ERR "lcd test: test09 expected gpa %lx got %lx\n",
				(unsigned long)(base + off),
				(unsigned long)actual);

			goto fail3;
		}
	}

	base = 0x40000000;
	for (off = 0; off < 0x40000; off += PAGE_SIZE) {
		if (lcd_mm_gva_to_gpa(lcd, base + off, &actual)) {
			printk(KERN_ERR "lcd test: test09 failed lookup at %lx\n",
				(unsigned long)(base + off));
			goto fail3;
		}
		if (actual != (base + off)) {
			printk(KERN_ERR "lcd test: test09 expected gpa %lx got %lx\n",
				(unsigned long)(base + off),
				(unsigned long)actual);

			goto fail3;
		}
	}

	base = 0x8000000000;
	for (off = 0; off < 0x40000; off += PAGE_SIZE) {
		if (lcd_mm_gva_to_gpa(lcd, base + off, &actual)) {
			printk(KERN_ERR "lcd test: test09 failed lookup at %lx\n",
				(unsigned long)(base + off));
			goto fail3;
		}
		if (actual != (base + off)) {
			printk(KERN_ERR "lcd test: test09 expected gpa %lx got %lx\n",
				(unsigned long)(base + off),
				(unsigned long)actual);

			goto fail3;
		}
	}

	lcd_destroy(lcd);

	return 0;

fail3:
fail2:
	lcd_destroy(lcd);
fail1:
	return ret;
}
#endif
static void lcd_tests(void)
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
//	if (test09())
//		return;
	return;
}
