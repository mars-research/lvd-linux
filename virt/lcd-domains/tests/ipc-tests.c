/**
 * Regression tests for ipc.c.
 */

static int test01(void)
{
	int ret;

	ret = __klcd_enter();
	if (ret) {
		LCD_ERR("klcd enter");
		goto fail1;
	}
	ret = __lcd_create_sync_endpoint(current->lcd, __cptr(5));
	if (ret) {
		LCD_ERR("create sync endpoint");
		goto fail2;
	}
	/* should delete and destroy endpoint */
	__klcd_exit();

	return 0;

fail2:
	__klcd_exit();
fail1:
	return ret;
}


static int ipc_tests(void)
{
	if (test01())
		return -1;

	LCD_MSG("all ipc tests passed!");
	return 0;
}
