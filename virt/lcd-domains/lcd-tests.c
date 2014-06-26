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

	lcd_destory(lcd);

	return 0;
}


static void lcd_tests(void)
{
	if (test01())
		return;
	return;
}
