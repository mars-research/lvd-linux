/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */

#ifdef CONFIG_RUN_IN_LCD

/* INTERNAL DEFS -------------------------------------------------- */

#include "dealer.h"

extern int __dealer_init(void);
extern void __dealer_exit(void);

/* IDL/LCD DEFS -------------------------------------------------- */

#include "dealer-idl.h"
#include "manufacturer-idl.h"
#include "../include/common.h"
#include "../include/ipc.h"
#include "../include/api.h"
#include "../include/utcb.h"

/* INTERFACE WRAPPERS -------------------------------------------------- */

cptr_t manufacturer_interface_cap;

/**
 * dealer_register_manufacturer_callee
 * -----------------------------------
 *
 * IN:
 * lcd_in_cap0 = cptr to capability for endpoint to manuf. interface
 *   (automatically transferred)
 * OUT:
 * lcd_r0 = 0
 */
static int dealer_register_manufacturer_callee(void)
{
	int ret;
	struct manufacturer_interface __mi = {
		.mk_engine = mk_engine_caller,
		.mk_automobile = mk_automobile_caller,
		.free_engine = free_engine_caller,
		.free_automobile = free_automobile_caller,
	};
	/*
	 * Register ...
	 */
	ret = dealer_register_manufacturer(&__mi);
	/*
	 * Reply
	 */
	lcd_store_r0(ret);
	ret = lcd_reply();
	if (ret) {
		LCD_ERR("couldn't reply");
		goto fail1;
	}

	return ret;

fail1:
	return ret;
}

/**
 * dealer_buy_car_callee
 * ---------------------
 * IN:
 * (reply endpoint cap)
 * OUT:
 * lcd_r0 = ret val
 * lcd_r1 = (cptr) to auto; used to refer to car in dealer space
 * lcd_r2 = (cptr) to engine inside auto
 */
static int * dealer_buy_car_callee(void)
{
	int ret;
	struct automobile *a;
	dsptr_t auto_dsptr;
	dsptr_t engine_dsptr;
	/*
	 * Call into internal code
	 */
	a = dealer_buy_car();
	if (!a) {
		LCD_ERR("bad auto");
		ret = -1;
		goto fail1;
	}
	/*
	 * Alloc capabilities
	 */
	ret = lcd_ds_store(a, &auto_dsptr, lcd_reply_badge());
	if(ret) {
		LCD_ERR("store auto");
		goto fail2;
	}
	ret = lcd_ds_store(a->engine, &engine_dsptr, lcd_reply_badge());
	if(ret) {
		LCD_ERR("store engine");
		goto fail3;
	}
	/*
	 * Reply
	 */
	lcd_store_r0(0);
	lcd_store_r1(auto_dsptr);
	lcd_store_r2(engine_dsptr);
	ret = lcd_reply();
	if (ret) {
		LCD_ERR("couldn't reply");
		goto fail4;
	}

	return ret;

fail4:
	lcd_ds_drop(lcd_reply_badge(), engine_dsptr);
fail3:
	lcd_ds_drop(lcd_reply_badge(), auto_dsptr);
fail2:
	dealer_return_car(a);
fail1:
	return ret;
}

/**
 * dealer_return_car_callee
 * ------------------------
 * IN:
 * lcd_r1 = dsptr to allocated auto (in dealer space)
 * OUT:
 * lcd_r0 = ret val
 */
static int dealer_return_car_callee(void)
{
	int ret;
	struct automobile *a;
	dsptr_t auto_dsptr;
	/*
	 * Get and remove engine from data store
	 */
	auto_dsptr = (dsptr_t)lcd_r1();
	a = lcd_ds_drop(lcd_reply_badge(), engine_dsptr);
	if (!a) {
		LCD_ERR("bad auto dsptr %lld", engine_dsptr);
		ret = -EINVAL;
		goto fail1;
	}
	/*
	 * Return car
	 */
	dealer_return_car(a);
	/*
	 * Reply
	 */
	lcd_store_r0(0);
	ret = lcd_reply();
	if (ret)
		LCD_ERR("couldn't reply");

	return ret;

fail1:
	return ret;
}

/**
 * dealer_die_callee
 * -----------------
 * IN:
 * (reply info)
 * OUT:
 * (nothing)
 */
int dealer_die_callee(void)
{
	int ret;
	/*
	 * Call internal exit
	 */
	__dealer_exit();
	/*
	 * Kill manufacturer
	 */
	lcd_store_r0(MANUFACTURER_DIE);
	ret = lcd_call(manfacturer_interface_cap);
	if (ret)
		return ret;
	/*
	 * Reply
	 */
	lcd_store_r0(0);
	ret = lcd_reply();
	if (ret)
		LCD_ERR("couldn't reply");
	return ret;
}

/* DEPENDENCIES -------------------------------------------------- */

/* See callee (manufacturer) for interface */

/**
 * mk_engine
 * ---------
 */
struct engine * mk_engine_caller(int cylinders)
{
	struct engine *e;
	int ret;
	/*
	 * Alloc caller copy
	 */
	e = kmalloc(sizeof(e), GFP_KERNEL);
	if (!e)
		goto fail1;
	e->cylinders = cylinders;
	/*
	 * Alloc on callee side, and get pointer to callee copy
	 */
	lcd_store_r0(MANUFACTURER_MK_ENGINE);
	lcd_store_r1(cylinders);
	ret = lcd_call(manufacturer_interface_cap);
	if (ret) {
		LCD_ERR("call failed");
		goto fail2;
	}
	if (lcd_r0()) {
		LCD_ERR("non zero ret val");
		goto fail2;
	}
	e->self = lcd_r1();

	return e;	
fail2:
	kfree(e);
fail1:
	return NULL;
}

/**
 * mk_automobile
 * -------------
 */
struct automobile * mk_automobile_caller(struct engine *e, int doors)
{
	struct automobile *a;
	int ret;
	/*
	 * Alloc caller copy
	 */
	a = kmalloc(sizeof(a), GFP_KERNEL);
	if (!a)
		goto fail1;
	a->engine = e;
	a->doors = doors;
	/*
	 * Alloc on callee side, and get pointer to callee copy
	 */
	lcd_store_r0(MANUFACTURER_MK_AUTOMOBILE);
	lcd_store_r1(doors);
	lcd_store_r2(e->self);
	ret = lcd_call(manufacturer_interface_cap);
	if (ret) {
		LCD_ERR("call failed");
		goto fail2;
	}
	if (lcd_r0()) {
		LCD_ERR("non zero ret val");
		goto fail2;
	}
	a->self = lcd_r1();

	return a;	
fail2:
	kfree(a);
fail1:
	return NULL;
}

/**
 * free_engine
 * -----------
 */
void free_engine_caller(struct engine *e)
{
	/*
	 * Free on callee side
	 */
	lcd_store_r0(MANUFACTURER_FREE_ENGINE);
	lcd_store_r1(e->self);
	ret = lcd_call(manufacturer_interface_cap);
	if (ret) {
		LCD_ERR("call failed");
		goto fail;
	}
	if (lcd_r0()) {
		LCD_ERR("non zero ret val");
		goto fail;
	}

	kfree(e);

	return;
fail:
	return;
}

/**
 * free_automobile
 * ---------------
 */
void free_automobile_caller(struct automobile *a)
{
	/*
	 * Free on callee side
	 */
	lcd_store_r0(MANUFACTURER_FREE_AUTOMOBILE);
	lcd_store_r1(a->self);
	ret = lcd_call(manufacturer_interface_cap);
	if (ret) {
		LCD_ERR("call failed");
		goto fail;
	}
	if (lcd_r0()) {
		LCD_ERR("non zero ret val");
		goto fail;
	}

	kfree(a);

	return;
fail:
	return;
}

/* MAIN EXEC LOOP -------------------------------------------------- */

int execution_loop(void)
{
	int ret;
	/* 
	 * Loop unless there's an internal error, or we get killed
	 */
	for (;;) {
		/*
		 * Listen for incoming message
		 */
		ret = lcd_recv(DEALER_DEALER_INTERFACE_CAP);
		if (ret)
			goto out;
		switch (lcd_r0()) {
			case DEALER_REGISTER_MANUFACTURER:
				ret = dealer_register_manufacturer_callee();
				break;
			case DEALER_BUY_CAR:
				ret = dealer_buy_car_callee();
				break;
			case DEALER_RETURN_CAR:
				ret = dealer_return_car_callee();
				break;
			case DEALER_DIE:
				ret = dealer_die();
				goto out;
		}

		/* internal error ? */
		if (ret)
			goto out;
	}

out:
	return ret;
}

int dealer_start(void)
{
	int ret;
	/*
	 * Call dealer init
	 */
	ret = __dealer_init();
	if (ret) {
		LCD_ERR("dealer init");
		return -1;
	}
	/*
	 * Now enter loop and listen for calls
	 */
	return execution_loop();
}
EXPORT_SYMBOL(dealer_start);

int __init dealer_init(void)
{
	return 0;
}

/* manufacturer_die does the actual tear down */
void __exit dealer_exit(void)
{
	return;
}

module_init(dealer_init);
module_exit(dealer_exit);

#endif /* CONFIG_RUN_IN_LCD */
