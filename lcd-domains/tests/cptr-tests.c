/**
 * Regression tests for cptr caches.
 *
 * These are a little bit more white box.
 */


#include <lcd-domains/kliblcd.h>
#include "../microkernel/internal.h"
#include <lcd-domains/tests-util.h>

static int test01(void)
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
		LCD_ERR("cptr alloc");
		goto fail2;
	}
	if (cptr_val(c) != 3) {
		LCD_ERR("unexpected cptr %llu", cptr_val(c));
		goto fail3;
	}

	ret = 0;
	goto out;

out:
fail3:
fail2:
	lcd_exit(0);
fail1:
	return ret;
}

static int test03(void)
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
	c = lcd_cptr_set_fanout(c, (1 << LCD_CPTR_FANOUT_BITS) - 1, 0);
	c = lcd_cptr_set_lvl(c, 1);
	if (lcd_cptr_fanout(c, 0) != ((1 << LCD_CPTR_FANOUT_BITS) - 1)) {
		LCD_ERR("failed 3, fo = %llu", lcd_cptr_fanout(c, 0));
		return -1;
	}
	if (lcd_cptr_level(c) != 1) {
		LCD_ERR("failed 4, level is %llu", lcd_cptr_level(c));
		return -1;
	}
	c = __cptr(0);
	c = lcd_cptr_set_fanout(c, 0, 0);
	c = lcd_cptr_set_fanout(c, (1 << LCD_CPTR_FANOUT_BITS) - 1, 1);
	c = lcd_cptr_set_lvl(c, 2);
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

static int test04(void)
{
	int ret;
	cptr_t c;
	int i;
	int top;

	ret = lcd_enter();
	if (ret) {
		LCD_ERR("lcd enter");
		goto fail1;
	}
	/*
	 * Alloc enough to fill root
	 */
	top = 1 << LCD_CPTR_SLOT_BITS;
	for (i = 3; i < top; i++) {
		ret = lcd_alloc_cptr(&c);
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
		ret = lcd_alloc_cptr(&c);
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
		lcd_free_cptr(__cptr(i));
	/*
	 * Re-alloc enough to fill root
	 */
	top = 1 << LCD_CPTR_SLOT_BITS;
	for (i = 3; i < top; i++) {
		ret = lcd_alloc_cptr(&c);
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

	ret = 0;
	goto out;

out:
fail2:
	lcd_exit(0);
fail1:
	return ret;
}

void cptr_tests(void)
{
	int n = 0;
	int total = 4;

	RUN_TEST(test01, n);
	RUN_TEST(test02, n);
	RUN_TEST(test03, n);
	RUN_TEST(test04, n);

	if (n < total) {
		LCD_MSG("%d of %d cptr tests failed",
			(total - n), total);
	} else {
		LCD_MSG("all cptr tests passed!");
	}
}

