/**
 * Regression tests for entering/exiting lcd mode.
 */

#include <lcd-domains/kliblcd.h>
#include <lcd-domains/tests-util.h>
#include "../microkernel/internal.h"

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

void enterexit_tests(void)
{
	int n = 0;
	int total = 1;

	RUN_TEST(test01, n);

	if (n < total) {
		LCD_MSG("%d of %d enter/exit tests failed",
			(total - n), total);
	} else {
		LCD_MSG("all enter/exit tests passed!");
	}
}

