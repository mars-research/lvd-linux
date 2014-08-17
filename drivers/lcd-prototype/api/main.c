/* 
 * api.c
 *
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

#include <linux/init.h>
#include <linux/module.h>
#include "../include/common.h"
#include "defs.h"

int api_tests(void);

int __init api_init(void)
{
	LCD_MSG("lcd prototype api starting");
	
	/*
	 * Initialize capability module
	 */
	lcd_cap_init();

	/*
	 * Run regression tests
	 */
	if (api_tests()) {
		return -1;
	}

	return 0;
}

void __exit api_exit(void)
{
	return;
}

module_init(api_init);
module_exit(api_exit);

