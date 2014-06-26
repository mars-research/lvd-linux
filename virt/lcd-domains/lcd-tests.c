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
		return ret;
	}

	lcd_destroy(lcd);

	return 0;
}

static int test02(void)
{
	struct lcd *lcd;
	int ret;

	ret = lcd_create(&lcd);
	if (ret) {
		printk(KERN_ERR "lcd test: test02 failed to create lcd\n");
		return ret;
	}
	
	ret = lcd_mm_gva_init(lcd, LCD_ARCH_FREE,
			LCD_ARCH_FREE + 4 * (1 << 20));
	if (ret) {
		printk(KERN_ERR "lcd test: test02 failed to init gva\n");
		return ret;
	}

	lcd_destroy(lcd);

	return 0;
}


static void lcd_tests(void)
{
	if (test01())
		return;
	if (test02())
		return;
	return;
}
