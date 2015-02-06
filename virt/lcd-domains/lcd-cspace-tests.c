/**
 * lcd-cspace-tests.c - tests for cap.c
 *
 * Authors:
 *     Muktesh Khole <mukteshkhole86@gmail.com>
 *     Anton Burtsev <aburtsev@flux.utah.edu>
 *     Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 * Mixture of black box and white box tests.
 */

static int test01(void)
{
	struct cspace cspace;
	int ret;

	ret = lcd_cap_init_cspace(&cspace);
	if (ret) {
		LCD_ERR("init cspace");
		return ret;
	}

	lcd_cap_destroy_cspace(&cspace);

	return 0;
}

static int cap_tests(void)
{
	int ret;

	ret = test01();
	if (ret)
		return ret;

	LCD_MSG("all lcd cap tests passed!");
	return 0;
}
