/**
 * Regression tests for creating klcd's.
 */

#include <lcd-domains/kliblcd.h>
#include <lcd-domains/tests-util.h>
#include "../internal.h"

static int test01(void)
{
	int ret;
	cptr_t klcd;

	ret = lcd_enter();
	if (ret) {
		LCD_ERR("enter klcd");
		goto fail1;
	}

	ret = klcd_create_klcd(&klcd);
	if (ret) {
		LCD_ERR("create klcd");
		goto fail2;
	}

	lcd_cap_delete(klcd);
	
	ret = 0;
	goto out;

out:
fail2:
	lcd_exit(0);
fail1:
	return ret;
}

void create_klcd_tests(void)
{
	int n = 0;
	int total = 1;

	RUN_TEST(test01, n);

	if (n < total) {
		LCD_MSG("%d of %d create_klcd tests failed",
			(total - n), total);
	} else {
		LCD_MSG("all create_klcd tests passed!");
	}
}

