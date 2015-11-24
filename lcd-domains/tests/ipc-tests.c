/**
 * Regression tests for ipc.
 */

#include <lcd-domains/kliblcd.h>
#include <lcd-domains/tests-util.h>
#include "../microkernel/internal.h"

static int test01(void)
{
	int ret;
	cptr_t c;

	ret = lcd_enter();
	if (ret) {
		LCD_ERR("klcd enter");
		goto fail1;
	}
	ret = lcd_alloc_cptr(&c);
	if (ret) {
		LCD_ERR("alloc cptr");
		goto fail2;
	}
	ret = lcd_create_sync_endpoint(&c);
	if (ret) {
		LCD_ERR("create sync endpoint");
		goto fail3;
	}

	ret = 0;
	goto out;

out:
fail3:
fail2:
	/* should delete and destroy endpoint, if applicable */
	lcd_exit(0);
fail1:
	return ret;
}


void ipc_tests(void)
{
	int n = 0;
	int total = 1;

	RUN_TEST(test01, n);

	if (n < total) {
		LCD_ERR("%d of %d ipc tests failed",
			(total - n), total);
	} else {
		LCD_MSG("all ipc tests passed!");
	}
}
