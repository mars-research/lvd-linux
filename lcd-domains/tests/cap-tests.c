/**
 * Regression tests for capabilities.
 */


#include <linux/slab.h>
#include <lcd-domains/kliblcd.h>
#include <lcd-domains/tests-util.h>
#include "../microkernel/internal.h"

static int test01(void)
{
	int ret;
	struct page *p;
	cptr_t cp;

	p = alloc_page(GFP_KERNEL);
	if (!p) {
		LCD_ERR("alloc page");
		ret = -ENOMEM;
		goto fail0;
	}
	ret = lcd_enter();
	if (ret) {
		LCD_ERR("enter klcd");
		goto fail1;
	}
	ret = klcd_add_page(p, &cp);
	if (ret) {
		LCD_ERR("add page");
		goto fail2;
	}
	klcd_rm_page(cp);
	
	ret = 0;
	goto out;

out:
fail2:
	lcd_exit(0);
fail1:
	__free_pages(p, 0);
fail0:
	return ret;
}

void cap_tests(void)
{
	int n = 0;
	int total = 1;

	RUN_TEST(test01, n);

	if (n < total) {
		LCD_MSG("%d of %d cap tests failed",
			(total - n), total);
	} else {
		LCD_MSG("all cap tests passed!");
	}
}

