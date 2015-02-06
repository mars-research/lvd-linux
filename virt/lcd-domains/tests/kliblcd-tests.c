/**
 * Regression tests for kliblcd.c.
 */

#include <linux/delay.h>

static int test01(void)
{
	int ret;

	ret = klcd_enter();
	if (ret) {
		LCD_ERR("enter");
		goto fail1;
	}
	klcd_exit();
	
	return 0;

fail1:
	return ret;
}

static int test02(void)
{
	int ret;
	cptr_t c;

	ret = klcd_enter();
	if (ret) {
		LCD_ERR("enter");
		goto fail1;
	}

	ret = lcd_alloc_cptr(&c);
	if (ret) {
		LCD_ERR("alloc cptr");
		goto fail2;
	}
	lcd_free_cptr(c);

	klcd_exit();
	
	return 0;

fail2:
	klcd_exit();
fail1:
	return ret;
}

static int test03(void)
{
	int ret;
	cptr_t c;
	struct page *p;

	p = alloc_page(GFP_KERNEL);
	if (!p) {
		LCD_ERR("alloc page");
		ret = -ENOMEM;
		goto fail0;
	}

	ret = klcd_enter();
	if (ret) {
		LCD_ERR("enter");
		goto fail1;
	}

	ret = klcd_add_page(p, &c);
	if (ret) {
		LCD_ERR("klcd add page");
		goto fail2;
	}

	klcd_rm_page(c);

	klcd_exit();
	
	return 0;

fail2:
	klcd_exit();
fail1:
	__free_pages(p, 0);
fail0:
	return ret;
}

static int test04(void)
{
	int ret;
	cptr_t c;
	gpa_t gpa;
	gva_t gva;

	ret = klcd_enter();
	if (ret) {
		LCD_ERR("enter");
		goto fail1;
	}

	ret = lcd_gfp(&c, &gpa, &gva);
	if (ret) {
		LCD_ERR("lcd gfp");
		goto fail2;
	}
	lcd_cap_delete(c);

	klcd_exit();
	
	return 0;

fail2:
	klcd_exit();
fail1:
	return ret;
}

static int test05(void)
{
	int ret;
	struct lcd_module_info *mi;

	ret = klcd_enter();
	if (ret) {
		LCD_ERR("enter");
		goto fail1;
	}
	ret = lcd_load_module("lcd_module_load_test", LCD_CPTR_NULL, &mi);
	if (ret) {
		LCD_ERR("lcd load module");
		goto fail2;
	}

	lcd_unload_module(mi, LCD_CPTR_NULL);

	klcd_exit();
	
	return 0;

fail2:
	klcd_exit();
fail1:
	return ret;
}

int kliblcd_tests(void)
{
	if (test01())
		return -1;
	if (test02())
		return -1;
	if (test03())
		return -1;
	if (test04())
		return -1;
	if (test05())
		return -1;
#if 0
	if (test06())
		return;
	if (test07())
		return;
	if (test08())
		return;
#endif
	LCD_MSG("all kliblcd tests passed!");
	return 0;
}
