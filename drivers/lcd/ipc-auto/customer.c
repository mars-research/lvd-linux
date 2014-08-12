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

#include "common.h"
#include "dealer.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Auto buyer (LCD test)");

#ifdef CONFIG_RUN_IN_LCD
int customer_init(void)
#else
static int __init customer_init(void)
#endif
{
	struct automobile *a;
	a = dealer_buy_car();
	if (!a) {
		AU_ERR("bad car");
		return -1;
	}
	AU_MSG("got a %d-door car with %d cylinders",
		a->doors, a->engine->cylinders);
	dealer_return_car(a);
	AU_MSG("customer initialized (and done)");
	return 0;
}

#ifdef CONFIG_RUN_IN_LCD
void customer_exit(void)
#else
static void __init customer_exit(void)
#endif
{
	AU_MSG("customer exited");
}

#ifndef CONFIG_RUN_IN_LCD
module_init(customer_init);
module_exit(customer_exit);
#endif
