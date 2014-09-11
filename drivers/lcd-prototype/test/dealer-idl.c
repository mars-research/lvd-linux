/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */

#ifdef CONFIG_RUN_IN_LCD

/* INTERNAL DEFS -------------------------------------------------- */

#include "dealer.h"

extern int __manufacturer_init(void);
extern void __manufacturer_exit(void);

/* IDL/LCD DEFS -------------------------------------------------- */

#include "dealer-idl.h"
#include "manufacturer-idl.h"
#include "../include/common.h"
#include "../include/ipc.h"
#include "../include/api.h"
#include "../include/utcb.h"

/* INTERFACE WRAPPERS -------------------------------------------------- */

struct manufacturer_interface __mi = {



};

/**
 * dealer_register_manufacturer_callee
 * -----------------------------------
 *
 * IN:
 * lcd_in_cap0 = cptr to capability for endpoint to manuf. interface
 * OUT:
 * lcd_r0 = 0
 */
static int dealer_register_manufacturer_callee(void)
{
	
	
	/*
	 * Reply
	 */
	lcd_store_r0(0);
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
 */
static int * dealer_buy_car_callee(void)
{
	int ret;
	struct automobile *a;
	dsptr_t auto_dsptr;
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
	 * Alloc capability
	 */
	ret = lcd_ds_store(a, &auto_dsptr, lcd_reply_badge());
	if(ret) {
		LCD_ERR("store auto");
		goto fail2;
	}
	/*
	 * Reply
	 */
	lcd_store_r0(0);
	lcd_store_r1(auto_dsptr);
	ret = lcd_reply();
	if (ret) {
		LCD_ERR("couldn't reply");
		goto fail3;
	}

	return ret;
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
	ret = lcd_call(lcd_boot_cap(DEALER_MANUFACTURER_INTERFACE_CAP));
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

/**
 * dealer_register_manufacturer
 * ----------------------------
 *
 * See callee for interface.
 */
int dealer_register_manufacturer(struct manufacturer_interface *__mi)
{
	int ret;
	/*
	 * Store pointer to original interface
	 */
	mi = __mi;
	/*
	 * Create endpoint for interface
	 */
	ret = lcd_mk_sync_endpoint(&manufacturer_interface_cap);
	if(ret) {
		LCD_ERR("mk manufacturer iface endpoint");
		goto fail1;
	}

	/*
	 * Store in the cap regs for the caller, and send
	 */
	lcd_store_out_cap0(manufacturer_interface_cap);
	ret = lcd_call(lcd_boot_cptr(MANUFACTURER_DEALER_INTERFACE_CAP));
	if (ret) {
		LCD_ERR("call to dealer");
		goto fail2;
	}

	return (int)lcd_r0();

fail2:
	lcd_rm_sync_endpoint(manufacturer_interface_cap);
fail1:
	return -1;
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
		ret = lcd_recv(manufacturer_interface_cap);
		if (ret)
			goto out;
		switch (lcd_r0()) {

			case MANUFACTURER_MK_ENGINE:
				ret = mk_engine_callee();
				break;
			case MANUFACTURER_MK_AUTOMOBILE:
				ret = mk_automobile_callee();
				break;
			case MANUFACTURER_FREE_ENGINE:
				ret = free_engine_callee();
				break;
			case MANUFACTURER_FREE_AUTOMOBILE:
				ret = free_automobile_callee();
				break;
			case MANUFACTURER_DIE:
				manufacturer_die();
				goto out;
		}

		/* internal error ? */
		if (ret)
			goto out;
	}

out:
	return ret;
}

int manufacturer_start(void)
{
	int ret;
	/*
	 * Call manufacturer init, should register interface.
	 */
	ret = __manufacturer_init();
	if (ret) {
		LCD_ERR("manufacturer init");
		return -1;
	}
	/*
	 * Ensure interface is set
	 */
	if (!mi) {
		LCD_ERR("interface not set");
		return -1;
	}
	/*
	 * Now enter loop and listen for calls
	 */
	return execution_loop();
}
EXPORT_SYMBOL(manufacturer_start);

int __init manufacturer_init(void)
{
	return 0;
}

/* manufacturer_die does the actual tear down */
void __exit manufacturer_exit(void)
{
	return;
}

module_init(manufacturer_init);
module_exit(manufacturer_exit);

#endif /* CONFIG_RUN_IN_LCD */
