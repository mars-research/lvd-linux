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
			gpa_add(LCD_ARCH_FREE, 4 * (1 << 20)));
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
	gpa_t gpa;
	hpa_t hpa;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test03 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			gpa_add(LCD_ARCH_FREE, 4 * (1 << 20)));
	if (ret) {
		printk(KERN_ERR "lcd test: test03 failed to init gva\n");
		goto fail2;
	}

	ret = lcd_mm_gva_alloc(lcd, &gpa, &hpa);
	if (ret) {
		printk(KERN_ERR "lcd test: test03 failed to alloc pg mem\n");
		goto fail3;
	}
	
	ret = 0;
	goto done;

done:
	free_page(hva_val(hpa2hva(hpa)));
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
	gpa_t gpa;
	hpa_t hpa;
	pmd_t *pmd_entry;
	pte_t *pt;
	pte_t *pte_entry;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test04 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			gpa_add(LCD_ARCH_FREE, 4 * (1 << 20)));
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
	pt = (pte_t *)hpa2va(hpa);
	set_pte_gpa(pt + 4, __gpa(0x1234000UL));

	/*
	 * Set up pmd entry for look up, and find pte
	 */
	pmd_entry = (pmd_t *)kmalloc(sizeof(*pmd_entry), GFP_KERNEL);
	if (!pmd_entry) {
		goto fail4;
	}

	set_pmd(pmd_entry, __pmd(gpa_val(gpa) | _KERNPG_TABLE));
	ret = lcd_mm_gva_lookup_pte(lcd, __gva(0x4000UL), 
				pmd_entry, &pte_entry);
	if (ret) {
		printk(KERN_ERR "lcd test: test04 failed to lookup pte\n");
		goto fail5;
	}

	/*
	 * Check
	 */
	if (gpa_val(pte_gpa(pte_entry)) != 0x1234000UL) {
		printk(KERN_ERR "lcd test: test04 pte gpa is %lx\n",
			gpa_val(pte_gpa(pte_entry)));
		ret = -1;
		goto fail6;
	}

	ret = 0;
	goto done;

done:
fail6:
fail5:
	kfree(pmd_entry);
fail4:
	free_page(hva_val(hpa2hva(hpa)));
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
	gpa_t gpa;
	hpa_t hpa;
	pud_t *pud_entry;
	pmd_t *pmd;
	pmd_t *pmd_entry;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test05 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			gpa_add(LCD_ARCH_FREE, 4 * (1 << 20)));
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
	pmd = (pmd_t *)hpa2va(hpa);
	set_pmd_gpa(pmd + 4, __gpa(0x1234000UL));

	/*
	 * Set up pud entry for look up, and find pmd
	 */
	pud_entry = (pud_t *)kmalloc(sizeof(*pud_entry), GFP_KERNEL);
	if (!pud_entry) {
		goto fail4;
	}

	set_pud_gpa(pud_entry, gpa);
	ret = lcd_mm_gva_lookup_pmd(lcd, __gva(0x4UL << PMD_SHIFT), 
				pud_entry, &pmd_entry);
	if (ret) {
		printk(KERN_ERR "lcd test: test05 failed to lookup pmd\n");
		goto fail5;
	}

	/*
	 * Check
	 */
	if (gpa_val(pmd_gpa(pmd_entry)) != 0x1234000UL) {
		printk(KERN_ERR "lcd test: test05 pte gpa is %lx\n",
			gpa_val(pmd_gpa(pmd_entry)));
		ret = -1;
		goto fail6;
	}

	ret = 0;
	goto done;

done:
fail6:
fail5:
	kfree(pud_entry);
fail4:
	free_page(hva_val(hpa2hva(hpa)));
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
	gpa_t gpa;
	hpa_t hpa;
	pgd_t *pgd_entry;
	pud_t *pud;
	pud_t *pud_entry;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test06 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			gpa_add(LCD_ARCH_FREE, 4 * (1 << 20)));
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
	pud = (pud_t *)hpa2va(hpa);
	set_pud_gpa(pud + 4, __gpa(0x1234000UL));

	/*
	 * Set up pgd entry for look up, and find pud
	 */
	pgd_entry = kmalloc(sizeof(*pgd_entry), GFP_KERNEL);
	if (!pgd_entry) {
		goto fail4;
	}

	set_pgd_gpa(pgd_entry, gpa);
	ret = lcd_mm_gva_lookup_pud(lcd, __gva(0x4UL << PUD_SHIFT), 
				pgd_entry, &pud_entry);
	if (ret) {
		printk(KERN_ERR "lcd test: test06 failed to lookup pud\n");
		goto fail5;
	}

	/*
	 * Check
	 */
	if (gpa_val(pud_gpa(pud_entry)) != 0x1234000UL) {
		printk(KERN_ERR "lcd test: test06 pmd gpa is %lx\n",
			gpa_val(pud_gpa(pud_entry)));
		ret = -1;
		goto fail6;
	}

	ret = 0;
	goto done;

done:
fail6:
fail5:
	kfree(pgd_entry);
fail4:
	free_page(hva_val(hpa2hva(hpa)));
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
	gpa_t gpa;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test07 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			gpa_add(LCD_ARCH_FREE, 4 * (1 << 20)));
	if (ret) {
		printk(KERN_ERR "lcd test: test07 failed to init gva\n");
		goto fail2;
	}

	/*
	 * Map gva 0x1234000UL to gpa 0x5678000UL
	 */
	ret = lcd_mm_gva_map(lcd, __gva(0x1234000UL), __gpa(0x5678000UL));
	if (ret) {
		printk(KERN_ERR "lcd test: test07 failed to map\n");
		goto fail3;
	}

	/*
	 * Check
	 */
	ret = lcd_mm_gva_to_gpa(lcd, __gva(0x1234000UL), &gpa);
	if (ret) {
		printk(KERN_ERR "lcd test: test07 failed to lookup\n");
		goto fail4;
	}

	if (gpa_val(gpa) != 0x5678000UL) {
		printk(KERN_ERR "lcd test: test07 got phys addr %lx\n",
			gpa_val(gpa));
		goto fail5;
	}

	ret = lcd_mm_gva_unmap(lcd, __gva(0x1234000UL));
	if (ret) {
		printk(KERN_ERR "lcd test: test07 failed to unmap\n");
		goto fail6;
	}

	ret = 0;
	goto done;

done:
fail6:
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
	gpa_t gpa1;
	gpa_t gpa2;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test08 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE, 
			gpa_add(LCD_ARCH_FREE, 2 * PAGE_SIZE));
	if (ret) {
		printk(KERN_ERR "lcd test: test08 failed to init gva\n");
		goto fail2;
	}

	ret = lcd_mm_gva_walk_pgd(lcd, __gva(0x1234000UL), &pgd_entry1);
	if (ret) {
		printk(KERN_ERR "lcd test: test08 failed to walk pgd\n");
		goto fail3;
	}

	ret = lcd_mm_gva_walk_pgd(lcd, __gva(0x1234000UL), &pgd_entry2);
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

	gpa1 = pgd_gpa(pgd_entry1);
	gpa2 = pgd_gpa(pgd_entry2);
	if (gpa_val(gpa1) < gpa_val(LCD_ARCH_FREE) || 
		gpa_val(gpa1) > gpa_val(gpa_add(LCD_ARCH_FREE,2 * PAGE_SIZE))){
		printk(KERN_ERR "lcd test: test08 bad gpa %lx\n",
			gpa_val(gpa1));
		ret = -1;
		goto fail5;
	}
	
	if (gpa_val(gpa1) != gpa_val(gpa2)) {
		printk(KERN_ERR "lcd test: test08 two diff gpa's: first = %lx, second %lx\n",
			gpa_val(gpa1), gpa_val(gpa2));
		ret = -1;
		goto fail5;
	}

	ret = 0;
	goto done;

done:
fail5:
fail4:
fail3:
fail2:
	lcd_destroy(lcd);
fail1:
	return ret;
}

static int test09_help(struct lcd *lcd, unsigned long base)
{
	unsigned long off;
	gpa_t actual;

	for (off = 0; off < (1 << 22); off += PAGE_SIZE) {
		if (lcd_mm_gva_to_gpa(lcd, __gva(base + off), &actual)) {
			printk(KERN_ERR "lcd test: test09 failed lookup at %lx\n",
				base + off);
			return -1;
		}
		if (gpa_val(actual) != base + off) {
			printk(KERN_ERR "lcd test: test09 expected gpa %lx got %lx\n",
				base + off,
				gpa_val(actual));
			return -1;
		}
	}
	return 0;
}

static int test09(void)
{
	struct lcd *lcd;
	int ret;
	unsigned long base;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test09 failed to create lcd\n");
		goto fail1;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			gpa_add(LCD_ARCH_FREE, 4 * (1 << 20)));
	if (ret) {
		printk(KERN_ERR "lcd test: test09 failed to init gva\n");
		goto fail2;
	}

	/*
	 * Map 0x0 - 0x400000 (first 4 MBs, takes two page tables)
	 */
	ret = lcd_mm_gva_map_range(lcd, __gva(0), __gpa(0), 1024);
	if (ret) {
		printk(KERN_ERR "lcd test: test09 failed to map first 4 MBs\n");
		goto fail3;
	}

	/*
	 * Map 0x40000000 - 0x40400000 (1GB -- 1GB + 4MBs)
	 */
	ret = lcd_mm_gva_map_range(lcd, __gva(1 << 30), __gpa(1 << 30), 1024);
	if (ret) {
		printk(KERN_ERR "lcd test: test09 failed to map 2nd 4 MBs\n");
		goto fail4;
	}

	/*
	 * Map 0x8000000000 - 0x8000400000 (512GB -- 512GB + 4MBs)
	 */
	ret = lcd_mm_gva_map_range(lcd, __gva(1UL << 39), 
				__gpa(1UL << 39), 1024);
	if (ret) {
		printk(KERN_ERR "lcd test: test09 failed to map 3rd 4 MBs\n");
		goto fail5;
	}

	/*
	 * CHECK
	 */

	base = 0;
	if (test09_help(lcd, base))
		goto fail6;
	base = 1 << 30;
	if (test09_help(lcd, base))
		goto fail6;
	base = 1UL << 39;
	if (test09_help(lcd, base))
		goto fail6;

	ret = 0;
	goto done;

done:
fail6:
	lcd_mm_gva_unmap_range(lcd, __gva(1UL << 39), 1024);
fail5:
	lcd_mm_gva_unmap_range(lcd, __gva(1 << 30), 1024);
fail4:
	lcd_mm_gva_unmap_range(lcd, __gva(0), 1024);
fail3:
fail2:
	lcd_destroy(lcd);
fail1:
	return ret;
}

static int test10(void)
{
	struct task_struct *t;
	int r;

	/*
	 * lcd-module-load-test.c is in virt/lcd-domains/
	 */

	/*
	 * Create it
	 */
	t = lcd_create_as_module("lcd_module_load_test");
	if (!t) {
		LCD_ERR("create");
		goto fail1;
	}
	
	/*
	 * Run it (once)
	 */
	r = lcd_run_as_module(t);
	if (r) {
		LCD_ERR("run");
		goto fail2;
	}

	/*
	 * Tear it down
	 */
	lcd_destroy_as_module(t, "lcd_module_load_test");
	return 0;

fail2:
	lcd_destroy_as_module(t, "lcd_module_load_test");
fail1:
	return -1;
}

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
	if (test09())
		return;
	if (test10())
		return;
	printk(KERN_ERR "lcd-domains: all tests passed!\n");
	return;
}
