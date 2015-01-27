/**
 * Regression tests for lcd code.
 *
 * Included in lcd-domains.c and
 * ran last in lcd_init.
 */

#include <linux/delay.h>

static int test01(void)
{
	struct lcd *lcd;
	int ret = 0;

	lcd = __lcd_create();
	if (!lcd) {
		LCD_ERR("failed to create lcd");
		ret = -1;
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
	int ret = 0;
	gpa_t gpa;
	hva_t hva;
	pmd_t *pmd_entry;
	pte_t *pt;
	pte_t *pte_entry;

	lcd = __lcd_create();
	if (!lcd) {
		LCD_ERR("failed to create lcd");
		goto fail1;
	}
	
	ret = lcd_mm_gv_init(lcd);
	if (ret) {
		LCD_ERR("failed to init gv");
		goto fail2;
	}

	ret = lcd_mm_gv_gfp(lcd, &gpa, &hva);
	if (ret) {
		LCD_ERR("failed to alloc pg mem");
		goto fail3;
	}

	/*
	 * Map gva = 0x4000 (start of 5th page frame) to gpa = 0x1234000UL
	 */
	pt = (pte_t *)hva2va(hva);
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
		LCD_ERR("failed to lookup pte");
		goto fail5;
	}

	/*
	 * Check
	 */
	if (gpa_val(pte_gpa(pte_entry)) != 0x1234000UL) {
		LCD_ERR("pte gpa is %lx\n", gpa_val(pte_gpa(pte_entry)));
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
	lcd_mm_gp_free_page(lcd, gpa);
fail3:
fail2:
	lcd_destroy(lcd);
fail1:
	return ret;
}

static int test03(void)
{
	struct lcd *lcd;
	int ret = 0;
	gpa_t gpa;
	hva_t hva;
	pud_t *pud_entry;
	pmd_t *pmd;
	pmd_t *pmd_entry;

	lcd = __lcd_create();
	if (!lcd) {
		LCD_ERR("failed to create lcd");
		goto fail1;
	}
	
	ret = lcd_mm_gv_init(lcd);
	if (ret) {
		LCD_ERR("failed to init gva");
		goto fail2;
	}

	ret = lcd_mm_gv_gfp(lcd, &gpa, &hva);
	if (ret) {
		LCD_ERR("failed to alloc pg mem");
		goto fail3;
	}

	/*
	 * Populate 5th entry in pmd to point to a (bogus) page table at
	 * gpa 0x1234000UL.
	 */
	pmd = (pmd_t *)hva2va(hva);
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
		LCD_ERR("failed to lookup pmd");
		goto fail5;
	}

	/*
	 * Check
	 */
	if (gpa_val(pmd_gpa(pmd_entry)) != 0x1234000UL) {
		LCD_ERR("pte gpa is %lx\n", gpa_val(pmd_gpa(pmd_entry)));
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
	free_page(hva_val(hva));
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
	int ret = 0;
	gpa_t gpa;
	hva_t hva;
	pgd_t *pgd_entry;
	pud_t *pud;
	pud_t *pud_entry;

	lcd = __lcd_create();
	if (!lcd) {
		LCD_ERR("failed to create lcd");
		goto fail1;
	}
	
	ret = lcd_mm_gv_init(lcd);
	if (ret) {
		LCD_ERR("failed to init gv");
		goto fail2;
	}

	ret = lcd_mm_gv_gfp(lcd, &gpa, &hva);
	if (ret) {
		LCD_ERR("failed to alloc pg mem");
		goto fail3;
	}

	/*
	 * Populate 5th entry in pud to point to a (bogus) pmd at
	 * gpa 0x1234000UL.
	 */
	pud = (pud_t *)hva2va(hva);
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
		LCD_ERR("failed to lookup pud");
		goto fail5;
	}

	/*
	 * Check
	 */
	if (gpa_val(pud_gpa(pud_entry)) != 0x1234000UL) {
		LCD_ERR("pmd gpa is %lx\n", gpa_val(pud_gpa(pud_entry)));
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
	free_page(hva_val(hva));
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
	int ret = 0;
	gpa_t gpa;

	lcd = __lcd_create();
	if (!lcd) {
		LCD_ERR("failed to create lcd");
		goto fail1;
	}
	
	ret = lcd_mm_gv_init(lcd);
	if (ret) {
		LCD_ERR("failed to init gv");
		goto fail2;
	}

	/*
	 * Map gva 0x1234000UL to gpa 0x5678000UL
	 */
	ret = lcd_mm_gva_map(lcd, __gva(0x1234000UL), __gpa(0x5678000UL));
	if (ret) {
		LCD_ERR("failed to map");
		goto fail3;
	}

	/*
	 * Check
	 */
	ret = lcd_mm_gva_to_gpa(lcd, __gva(0x1234000UL), &gpa);
	if (ret) {
		LCD_ERR("failed to lookup");
		goto fail4;
	}

	if (gpa_val(gpa) != 0x5678000UL) {
		LCD_ERR("got phys addr %lx\n", gpa_val(gpa));
		ret = -1;
		goto fail5;
	}

	ret = lcd_mm_gva_unmap(lcd, __gva(0x1234000UL));
	if (ret) {
		LCD_ERR("failed to unmap");
		ret = -1;
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

static int test06(void)
{
	struct lcd *lcd;
	int ret = 0;
	pgd_t *pgd_entry1;
	pgd_t *pgd_entry2;

	lcd = __lcd_create();
	if (!lcd) {
		LCD_ERR("failed to create lcd");
		goto fail1;
	}
	
	ret = lcd_mm_gv_init(lcd);
	if (ret) {
		LCD_ERR("failed to init gv");
		goto fail2;
	}

	/* (should map entry on the fly) */
	ret = lcd_mm_gva_walk_pgd(lcd, __gva(0x1234000UL), &pgd_entry1);
	if (ret) {
		LCD_ERR("failed to walk pgd");
		goto fail3;
	}

	/* (should map entry on the fly) */
	ret = lcd_mm_gva_walk_pgd(lcd, __gva(0x1234000UL), &pgd_entry2);
	if (ret) {
		LCD_ERR("failed to walk pgd2");
		goto fail4;
	}

	if (pgd_entry1 != pgd_entry2) {
		LCD_ERR("entries differ");
		ret = -1;
		goto fail5;
	}

	if (!pgd_present(*pgd_entry1) || !pgd_present(*pgd_entry2)) {
		LCD_ERR("entries not present\n");
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

static int test07_help(struct lcd *lcd, unsigned long base)
{
	unsigned long off;
	gpa_t actual;

	for (off = 0; off < (1 << 22); off += PAGE_SIZE) {
		if (lcd_mm_gva_to_gpa(lcd, __gva(base + off), &actual)) {
			LCD_ERR("failed lookup at %lx", base + off);
			return -1;
		}
		if (gpa_val(actual) != base + off) {
			LCD_ERR("expected gpa %lx got %lx",
				base + off,
				gpa_val(actual));
			return -1;
		}
	}
	return 0;
}

static int test07(void)
{
	struct lcd *lcd;
	int ret = 0;
	unsigned long base;

	lcd = __lcd_create();
	if (!lcd) {
		LCD_ERR("failed to create lcd");
		goto fail1;
	}
	
	ret = lcd_mm_gv_init(lcd);
	if (ret) {
		LCD_ERR("failed to init gv");
		goto fail2;
	}

	/*
	 * Map 0x0 - 0x400000 (first 4 MBs, takes two page tables)
	 */
	ret = lcd_mm_gva_map_range(lcd, __gva(0), __gpa(0), 1024);
	if (ret) {
		LCD_ERR("failed to map first 4 MBs");
		goto fail3;
	}

	/*
	 * Map 0x40000000 - 0x40400000 (1GB -- 1GB + 4MBs)
	 */
	ret = lcd_mm_gva_map_range(lcd, __gva(1 << 30), __gpa(1 << 30), 1024);
	if (ret) {
		LCD_ERR("failed to map 2nd 4 MBs");
		goto fail4;
	}

	/*
	 * Map 0x8000000000 - 0x8000400000 (512GB -- 512GB + 4MBs)
	 */
	ret = lcd_mm_gva_map_range(lcd, __gva(1UL << 39), 
				__gpa(1UL << 39), 1024);
	if (ret) {
		LCD_ERR("failed to map 3rd 4 MBs");
		goto fail5;
	}

	/*
	 * CHECK
	 */

	base = 0;
	if (test07_help(lcd, base))
		goto fail6;
	base = 1 << 30;
	if (test07_help(lcd, base))
		goto fail6;
	base = 1UL << 39;
	if (test07_help(lcd, base))
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


static int test08(void)
{
	struct lcd *lcd;
	int ret;

	/*
	 * lcd-module-load-test.c is in virt/lcd-domains/
	 */

	ret = lcd_create("lcd_module_load_test", &lcd);
	if (ret) {
		LCD_ERR("creating lcd");
		goto fail1;
	}

	/*
	 * Run init thread
	 */
	ret = lcd_thread_start(lcd->init_thread);
	if (ret) {
		LCD_ERR("run");
		goto fail2;
	}

	LCD_MSG("test module sleeping for 100 ms.");
	msleep(100);

	/*
	 * Tear it down
	 */
	ret = lcd_thread_kill(lcd->init_thread);
	LCD_MSG("lcd_module_load_test ret val = %d", ret);
	lcd_destroy(lcd);

	return 0;

fail2:
	lcd_thread_kill(lcd->init_thread);
	lcd_destroy(lcd);
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
	LCD_MSG("all tests passed!");
	return;
}
