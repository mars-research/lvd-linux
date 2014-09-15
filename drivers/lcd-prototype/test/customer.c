/**
 * customer.c - car buyer subsystem for test
 *
 *
 * Authors: Anton Burtsev    <aburtsev@flux.utah.edu>
 *          Charles Jacobsen <charlesj@cs.utah.edu>
 *
 * Copyright: University of Utah
 */

#include <linux/kernel.h>
#include <linux/module.h>

#include "../include/common.h"
#include "dealer.h"

#ifdef CONFIG_LCD_PROTOTYPE_TEST_IN_LCD
int __customer_init(void)
#else
static int __init customer_init(void)
#endif
{
	struct automobile *a;
	a = dealer_buy_car();
	if (!a) {
		LCD_ERR("bad car");
		return -1;
	}
	LCD_MSG("got a %d-door car with %d cylinders",
		a->doors, a->engine->cylinders);
	dealer_return_car(a);
	LCD_MSG("customer initialized (and done)");
	return 0;
}

#ifdef CONFIG_LCD_PROTOTYPE_TEST_IN_LCD
void __customer_exit(void)
#else
static void __exit customer_exit(void)
#endif
{
	LCD_MSG("customer exited");
}

#ifndef CONFIG_LCD_PROTOTYPE_TEST_IN_LCD
module_init(customer_init);
module_exit(customer_exit);
#endif
