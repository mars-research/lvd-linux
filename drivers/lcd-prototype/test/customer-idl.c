/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */

#ifdef CONFIG_LCD_PROTOTYPE_TEST_IN_LCD

/* INTERNAL DEFS -------------------------------------------------- */

#include "dealer.h"

extern int __customer_init(void);
extern void __customer_exit(void);

/* IDL/LCD DEFS -------------------------------------------------- */

#include <linux/slab.h>
#include "dealer-idl.h"
#include "customer-idl.h"
#include "../include/common.h"
#include "../include/ipc.h"
#include "../include/api-internal.h"
#include "../include/utcb.h"

/* INTERFACE WRAPPERS -------------------------------------------------- */

/* (none) */

/* DEPENDENCIES -------------------------------------------------- */

/* See callee (dealer) for interface */

/**
 * dealer_buy_car
 * --------------
 */
struct automobile * dealer_buy_car(void)
{
	struct automobile *a;
	struct engine *e;
	int ret;

	LCD_MSG("customer buying car");
	/*
	 * Alloc caller copy of hierarchy
	 */
	a = kmalloc(sizeof(*a), GFP_KERNEL);
	if (!a)
		goto fail1;
	e = kmalloc(sizeof(*e), GFP_KERNEL);
	if (!e)
		goto fail2;
	a->engine = e;
	/*
	 * Alloc on callee side, and get pointer to callee copies
	 */
	lcd_store_r0(DEALER_BUY_CAR);
	ret = lcd_call(CUSTOMER_DEALER_INTERFACE_CAP);
	if (ret) {
		LCD_ERR("call failed");
		goto fail3;
	}
	if (lcd_r0()) {
		LCD_ERR("non zero ret val");
		goto fail3;
	}
	a->self = lcd_r1();
	a->engine->self = lcd_r2();

	/*
	 * XXX: How to automate code generation for transferring this info?
	 */
	a->doors = 4;
	a->engine->cylinders = 8;

	return a;	

fail3:
	kfree(e);
fail2:
	kfree(a);
fail1:
	return NULL;
}

/**
 * dealer_return_car
 * -----------------
 */
void dealer_return_car(struct automobile *a)
{
	int ret;
	/*
	 * Free on callee side
	 */
	lcd_store_r0(DEALER_RETURN_CAR);
	lcd_store_r1(a->self);
	ret = lcd_call(CUSTOMER_DEALER_INTERFACE_CAP);
	if (ret) {
		LCD_ERR("call failed");
		goto fail;
	}
	if (lcd_r0()) {
		LCD_ERR("non zero ret val");
		goto fail;
	}

	kfree(a->engine);
	kfree(a);

	return;

fail:
	return;
}

/* MAIN EXEC LOOP -------------------------------------------------- */

extern struct completion dealer_ready;

int customer_start(void)
{
	int ret;
	/*
	 * Wait until dealer is ready
	 */
	ret = wait_for_completion_interruptible(&dealer_ready);
	if (ret)
		return ret;
	LCD_MSG("customer ok to proceed");
	/*
	 * Call customer init (will buy car)
	 */
	ret = __customer_init();
	if (ret) {
		LCD_ERR("customer init");
		return -1;
	}
	/*
	 * Call customer exit
	 */
	__customer_exit();
	/*
	 * Now tear down
	 */
	lcd_store_r0(DEALER_DIE);
	ret = lcd_call(CUSTOMER_DEALER_INTERFACE_CAP);
	if (ret) {
		LCD_ERR("call failed");
		return ret;
	}

	LCD_MSG("customer idl done");

	return 0;
}
EXPORT_SYMBOL(customer_start);

int __init customer_init(void)
{
	return 0;
}

void __exit customer_exit(void)
{
	return;
}

module_init(customer_init);
module_exit(customer_exit);

#endif
