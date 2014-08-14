/* 
 * api-tests.c
 *
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

#define LCD_FAIL(msg...) do{ LCD_ERR(msg); return -1; } while(0)
#define LCD_PASS() do{ return 0; } while(0)

static int test01(void)
{
	struct cspace *cspace;
	if (lcd_mk_cspace(&cspace))
		LCD_FAIL("mk cspace");
	lcd_rm_cspace(cspace);
	LCD_PASS();
}

static int test02(void)
{
	struct cspace *cspace;
	int ret;
	if (lcd_mk_cspace(&cspace))
		LCD_FAIL("mk cspace");
	ret = lcd_lock_cspace(cspace);
	if (ret)
		LCD_FAIL("lock cspace returned %d", ret);
	lcd_unlock_cspace(cspacse);
	lcd_rm_cspace(cspace);
	LCD_PASS();
}

static int api_tests(void)
{
	if (test01())
		return -1;
	if (test02())
		return -1;
	LCD_MSG("all api tests passed!");
	return 0;
}
