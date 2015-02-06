/**
 * Regression tests for main.c.
 */

static int test01(void)
{
	int ret;

	ret = __klcd_enter();
	if (ret) {
		LCD_ERR("klcd enter");
		return ret;
	}

	__klcd_exit();

	return 0;
}

int main_tests(void)
{
	if (test01())
		return -1;
	LCD_MSG("all main tests passed!");
	return 0;
}
