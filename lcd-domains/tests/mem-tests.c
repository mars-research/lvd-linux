/**
 * Regression tests for memory.
 */

#include <lcd-domains/kliblcd.h>
#include <lcd-domains/tests-util.h>
#include "../microkernel/internal.h"

static int test01(void)
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

void mem_tests(void)
{
	int n = 0;
	int total = 1;

	RUN_TEST(test01, n);

	if (n < total) {
		LCD_MSG("%d of %d memory tests failed",
			(total - n), total);
	} else {
		LCD_MSG("all memory tests passed!");
	}
}
