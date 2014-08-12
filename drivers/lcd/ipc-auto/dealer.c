/**
 * dealer.c - car dealer subsystem for test
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
MODULE_DESCRIPTION("Auto dealer (LCD test)");

struct manufacturer_interface *manufacturer;

int dealer_register_manufacturer(struct manufacturer_interface *m)
{
	manufacturer = m;
	return 0;
}
EXPORT_SYMBOL(dealer_register_manufacturer);

struct automobile * dealer_buy_car(void)
{
	struct engine *e;
	struct automobile *a;

	/*
	 * Make an 8 cylinder engine
	 */
	e = manufacturer->mk_engine(8);
	if (!e || e->cylinders != 8) {
		AU_ERR("client: bad engine");
		return NULL;
	}
	/*
	 * Make a 4-door car with e as its engine.
	 */
	a = manufacturer->mk_automobile(e, 4);
	if (!a || a->engine != e || a->doors != 4) {
		AU_ERR("client: bad auto");
		return NULL;
	}
	return a;
}
EXPORT_SYMBOL(dealer_buy_car);

void dealer_return_car(struct automobile *a)
{
	/*
	 * Free the data
	 */
	manufacturer->free_engine(a->engine);
	manufacturer->free_automobile(a);
}
EXPORT_SYMBOL(dealer_return_car);

#ifdef CONFIG_RUN_IN_LCD
int dealer_init(void)
#else
static int __init dealer_init(void)
#endif
{
	AU_MSG("dealer initialized");
	return 0;
}

#ifdef CONFIG_RUN_IN_LCD
void dealer_exit(void)
#else
static void __init dealer_exit(void)
#endif
{
	AU_MSG("dealer exited");
}

#ifndef CONFIG_RUN_IN_LCD
module_init(dealer_init);
module_exit(dealer_exit);
#endif
