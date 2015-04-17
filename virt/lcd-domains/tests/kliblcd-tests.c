/**
 * Regression tests for kliblcd.c.
 */

#include <linux/delay.h>

static int test01(void)
{
	int ret;

	ret = lcd_enter();
	if (ret) {
		LCD_ERR("enter");
		goto fail1;
	}
	lcd_exit(0);
	
	return 0;

fail1:
	return ret;
}

static int test02(void)
{
	int ret;
	cptr_t c;

	ret = lcd_enter();
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

	lcd_exit(0);
	
	return 0;

fail2:
	lcd_exit(0);
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

	ret = lcd_enter();
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

	lcd_exit(0);
	
	return 0;

fail2:
	lcd_exit(0);
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

	ret = lcd_enter();
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

	lcd_exit(0);
	
	return 0;

fail2:
	lcd_exit(0);
fail1:
	return ret;
}

static int test05(void)
{
	struct cptr_cache *cache;
	int ret;
	cptr_t c;

	ret = cptr_cache_init(&cache);
	if (ret) {
		LCD_ERR("cache init");
		goto fail1;
	}
	ret = __lcd_alloc_cptr(cache, &c);
	if (ret) {
		LCD_ERR("cptr alloc");
		goto fail2;
	}
	if (cptr_val(c) != 3) {
		LCD_ERR("unexpected cptr %llu", cptr_val(c));
		goto fail3;
	}

	cptr_cache_destroy(cache);

	return 0;

fail3:
fail2:
	cptr_cache_destroy(cache);
fail1:
	return ret;
}

static cptr_t set_fo(cptr_t c, unsigned fo, int lvl)
{
	return __cptr(cptr_val(c) | 
		(fo << (LCD_CPTR_SLOT_BITS + lvl * LCD_CPTR_FANOUT_BITS)));
}

static cptr_t set_lvl(cptr_t c, int lvl)
{
	return __cptr(cptr_val(c) | (lvl << LCD_CPTR_LEVEL_SHIFT));
}

static int test06(void)
{
	cptr_t c;
	if (lcd_cptr_slot(__cptr(0)) != 0) {
		LCD_ERR("failed 1");
		return -1;
	}
	if (lcd_cptr_slot(__cptr((1 << LCD_CPTR_SLOT_BITS) - 1)) !=
		(1 << LCD_CPTR_SLOT_BITS) - 1) {
		LCD_ERR("failed 2");
		return -1;
	}
	c = __cptr(0);
	c = set_fo(c, (1 << LCD_CPTR_FANOUT_BITS) - 1, 0);
	c = set_lvl(c, 1);
	if (lcd_cptr_fanout(c, 0) != ((1 << LCD_CPTR_FANOUT_BITS) - 1)) {
		LCD_ERR("failed 3, fo = %llu", lcd_cptr_fanout(c, 0));
		return -1;
	}
	if (lcd_cptr_level(c) != 1) {
		LCD_ERR("failed 4, level is %llu", lcd_cptr_level(c));
		return -1;
	}
	c = __cptr(0);
	c = set_fo(c, 0, 0);
	c = set_fo(c, (1 << LCD_CPTR_FANOUT_BITS) - 1, 1);
	c = set_lvl(c, 2);
	if (lcd_cptr_fanout(c, 0) != 0) {
		LCD_ERR("failed 5");
		return -1;
	}
	if (lcd_cptr_fanout(c, 1) != ((1 << LCD_CPTR_FANOUT_BITS) - 1)) {
		LCD_ERR("failed 6");
		return -1;
	}
	if (lcd_cptr_level(c) != 2) {
		LCD_ERR("failed 7");
		return -1;
	}

	return 0;
}

static int test07(void)
{
	struct cptr_cache *cache;
	int ret;
	cptr_t c;
	int i;
	int top;

	ret = cptr_cache_init(&cache);
	if (ret) {
		LCD_ERR("cache init");
		goto fail1;
	}
	/*
	 * Alloc enough to fill root
	 */
	top = 1 << LCD_CPTR_SLOT_BITS;
	for (i = 3; i < top; i++) {
		ret = __lcd_alloc_cptr(cache, &c);
		if (ret) {
			LCD_ERR("cache alloc at %d", i);
			goto fail2;
		}
		if (lcd_cptr_level(c) != 0 || lcd_cptr_slot(c) != i) {
			LCD_ERR("bad cptr %llu", cptr_val(c));
			ret = -EINVAL;
			goto fail2;
		}
	}
	/*
	 * Alloc enough to fill next level
	 */
	top = (1 << LCD_CPTR_SLOT_BITS) * (1 << LCD_CPTR_FANOUT_BITS);
	for (i = 0; i < top; i++) {
		ret = __lcd_alloc_cptr(cache, &c);
		if (ret) {
			LCD_ERR("cache alloc at %d", i);
			goto fail2;
		}
		if (lcd_cptr_level(c) != 1 ||
			lcd_cptr_fanout(c, 1) != 
			lcd_cptr_fanout(__cptr(i), 1) ||
			lcd_cptr_slot(c) != lcd_cptr_slot(__cptr(i))) {
			LCD_ERR("bad cptr %llu", cptr_val(c));
			ret = -EINVAL;
			goto fail2;
		}
	}
	/*
	 * Free root
	 */
	top = 1 << LCD_CPTR_SLOT_BITS;
	for (i = 3; i < top; i++)
		__lcd_free_cptr(cache, __cptr(i));
	/*
	 * Re-alloc enough to fill root
	 */
	top = 1 << LCD_CPTR_SLOT_BITS;
	for (i = 3; i < top; i++) {
		ret = __lcd_alloc_cptr(cache, &c);
		if (ret) {
			LCD_ERR("cache alloc at %d", i);
			goto fail2;
		}
		if (lcd_cptr_level(c) != 0 || lcd_cptr_slot(c) != i) {
			LCD_ERR("bad cptr %llu", cptr_val(c));
			ret = -EINVAL;
			goto fail2;
		}
	}

	cptr_cache_destroy(cache); /* no need to free */

	return 0;

fail2:
	cptr_cache_destroy(cache);
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
	if (test06())
		return -1;
	if (test07())
		return -1;
	LCD_MSG("all kliblcd tests passed!");
	return 0;
}
