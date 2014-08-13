/* 
 * api-tests.c
 *
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

static int test01(void)
{
	return 0;
}

static int api_tests(void)
{
	if (test01())
		return -1;
	LCD_MSG("all api tests passed!");
	return 0;
}
