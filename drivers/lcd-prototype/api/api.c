/* 
 * api.c
 *
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include "../include/common.h"
#include "defs.h"

static int api_tests(void);

static int __init api_init(void)
{
	LCD_MSG("api starting");

	/*
	 * Run regression tests
	 */
	if (api_tests()) {
		return -1;
	}

	return 0;
}

static void __exit api_exit(void)
{
	return;
}

module_init(api_init);
module_exit(api_exit);

/* DEBUG -------------------------------------------------- */

#include "api-tests.c"
