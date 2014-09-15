/**
 * manufacturer.c - car manufacturer subsystem for test
 *
 *
 * Authors: Anton Burtsev    <aburtsev@flux.utah.edu>
 *          Charles Jacobsen <charlesj@cs.utah.edu>
 *
 * Copyright: University of Utah
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

#include "../include/common.h"
#include "dealer.h"

static struct engine * mk_engine(int cylinders)
{
	struct engine *e;
	LCD_MSG("making %d-cylinder engine", cylinders);
	e = kmalloc(sizeof(*e), GFP_KERNEL);
	if (!e)
		LCD_ERR("no memory for engine");
	e->cylinders = 8;
	return e;
}

static struct automobile * mk_automobile(struct engine *e, int doors)
{
	struct automobile *a;

	if (!e) {
		LCD_ERR("engine is null");
		return NULL;
	}

	LCD_MSG("making %d-door auto with %d-cylinder engine",
		doors, e->cylinders);

	a = kmalloc(sizeof(*a), GFP_KERNEL);
	if (!a) {
		LCD_ERR("no memory for auto");
		return NULL;
	}
	a->engine = e;
	a->doors = doors;
	return a;
}

static void free_engine(struct engine *e)
{
	if (!e) {
		LCD_ERR("engine is null");
		return;
	}
	LCD_MSG("freeing %d-cylinder engine", e->cylinders);
	kfree(e);
}

static void free_automobile(struct automobile *a)
{
	if (!a) {
		LCD_ERR("auto is null");
		return;
	}
	LCD_MSG("freeing %d-door auto", a->doors);
	kfree(a);
}

static struct manufacturer_interface mi = {
	.mk_engine = mk_engine,
	.mk_automobile = mk_automobile,
	.free_engine = free_engine,
	.free_automobile = free_automobile,
};

#ifdef CONFIG_LCD_PROTOTYPE_TEST_IN_LCD
int __manufacturer_init(void)
#else
static int __init manufacturer_init(void)
#endif
{
	int ret;
	ret = dealer_register_manufacturer(&mi);
	if (ret) {
		LCD_ERR("registering manufacturer");
		return -1;
	}
	LCD_MSG("manufacturer registered and initialized");
	return 0;
}

#ifdef CONFIG_LCD_PROTOTYPE_TEST_IN_LCD
void __manufacturer_exit(void)
#else
static void __exit manufacturer_exit(void)
#endif
{
	LCD_MSG("manufacturer exited");
}

#ifndef CONFIG_LCD_PROTOTYPE_TEST_IN_LCD
module_init(manufacturer_init);
module_exit(manufacturer_exit);
#endif
