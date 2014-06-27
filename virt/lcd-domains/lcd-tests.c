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
	if (gpa != LCD_ARCH_FREE) {
		printk(KERN_ERR "lcd test: test03 gpa at wrond addr %lx\n",
			gpa);
		goto fail4;
	}		

	free_page((u64)__va(hpa));
	lcd_destroy(lcd);

	return 0;

fail4:
	free_page((u64)__va(hpa));
fail3:
fail2:
	lcd_destroy(lcd);
fail1:
	return ret;
}

#if 0
static int test03(void)
{
	struct lcd *lcd;
	int ret;
	u64 base;
	u64 off;
	u64 actual;

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

	/*
	 * Map 0x0 - 0x400000 (first 4 MBs, takes two page dirs)
	 */
	ret = lcd_mm_gva_map_range(lcd, 0, 0, 1024);
	if (ret) {
		printk(KERN_ERR "lcd test: test03 failed to map first 4 MBs\n");
		goto fail3;
	}

	/*
	 * Map 0x40000000 - 0x40400000 (1GB -- 1GB + 4MBs, takes two page dirs)
	 */
	ret = lcd_mm_gva_map_range(lcd, 0x40000000, 0x40000000, 1024);
	if (ret) {
		printk(KERN_ERR "lcd test: test03 failed to map 2nd 4 MBs\n");
		goto fail3;
	}

	/*
	 * Map 0x8000000000 - 0x8000400000 (512GB -- 512GB + 4MBs, 
	 * takes two page dirs)
	 */
	ret = lcd_mm_gva_map_range(lcd, 0x8000000000, 0x8000000000, 1024);
	if (ret) {
		printk(KERN_ERR "lcd test: test03 failed to map 3rd 4 MBs\n");
		goto fail3;
	}

	/*
	 * CHECK
	 */

	base = 0;
	for (off = 0; off < 0x40000; off += PAGE_SIZE) {
		if (lcd_mm_gva_to_gpa(lcd, base + off, &actual)) {
			printk(KERN_ERR "lcd test: test03 failed lookup at %lx\n",
				(unsigned long)(base + off));
			goto fail3;
		}
		if (actual != (base + off)) {
			printk(KERN_ERR "lcd test: test03 expected gpa %lx got %lx\n",
				(unsigned long)(base + off),
				(unsigned long)actual);

			goto fail3;
		}
	}

	base = 0x40000000;
	for (off = 0; off < 0x40000; off += PAGE_SIZE) {
		if (lcd_mm_gva_to_gpa(lcd, base + off, &actual)) {
			printk(KERN_ERR "lcd test: test03 failed lookup at %lx\n",
				(unsigned long)(base + off));
			goto fail3;
		}
		if (actual != (base + off)) {
			printk(KERN_ERR "lcd test: test03 expected gpa %lx got %lx\n",
				(unsigned long)(base + off),
				(unsigned long)actual);

			goto fail3;
		}
	}

	base = 0x8000000000;
	for (off = 0; off < 0x40000; off += PAGE_SIZE) {
		if (lcd_mm_gva_to_gpa(lcd, base + off, &actual)) {
			printk(KERN_ERR "lcd test: test03 failed lookup at %lx\n",
				(unsigned long)(base + off));
			goto fail3;
		}
		if (actual != (base + off)) {
			printk(KERN_ERR "lcd test: test03 expected gpa %lx got %lx\n",
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
	return;
}
