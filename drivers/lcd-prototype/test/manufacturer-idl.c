/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */

#ifdef CONFIG_LCD_PROTOTYPE_TEST_IN_LCD

/* INTERNAL DEFS -------------------------------------------------- */

#include "dealer.h"

extern int __manufacturer_init(void);
extern void __manufacturer_exit(void);

/* IDL/LCD DEFS -------------------------------------------------- */

#include "manufacturer-idl.h"
#include "dealer-idl.h"
#include "../include/common.h"
#include "../include/ipc.h"
#include "../include/api-internal.h"
#include "../include/utcb.h"

/* INTERFACE WRAPPERS -------------------------------------------------- */

struct manufacturer_interface *mi;
cptr_t manufacturer_interface_cap;

/**
 * mk_engine_callee
 * ----------------
 * IN:
 * lcd_r1 = number of cylinders
 * OUT:
 * lcd_r0 = 0
 * lcd_r1 = dsptr to allocated engine
 */
static int mk_engine_callee(void)
{
	int ret;
	struct engine *e;
	int cylinders;
	dsptr_t engine_dsptr;
	/*
	 * Build engine
	 */
	cylinders = (int)lcd_r1();
	e = mi->mk_engine(cylinders);
	if (!e) {
		LCD_ERR("bad engine");
		ret = -1;
		goto fail1;
	}
	/*
	 * Alloc capability
	 */
	ret = lcd_ds_store(e, lcd_reply_badge(), &engine_dsptr);
	if(ret) {
		LCD_ERR("store engine");
		goto fail2;
	}
	/*
	 * Reply
	 */
	lcd_store_r0(0);
	lcd_store_r1(engine_dsptr);
	ret = lcd_reply();
	if (ret) {
		LCD_ERR("couldn't reply");
		goto fail3;
	}

	return ret;
fail3:
	lcd_ds_drop(lcd_reply_badge(), engine_dsptr);
fail2:
	mi->free_engine(e);
fail1:
	return ret;
}

/**
 * mk_automobile_callee
 * --------------------
 * IN:
 * lcd_r1 = number of doors
 * lcd_r2 = dsptr to allocated engine
 * OUT:
 * lcd_r0 = 0
 * lcd_r1 = dsptr to allocated auto
 */
static int mk_automobile_callee(void)
{
	int ret;
	struct automobile *a;
	struct engine *e;
	dsptr_t engine_dsptr;
	int doors;
	dsptr_t auto_dsptr;
	/*
	 * Get the engine from the data store
	 */
	engine_dsptr = (dsptr_t)lcd_r2();
	e = lcd_ds_read(lcd_reply_badge(), engine_dsptr);
	if (!e) {
		LCD_ERR("bad engine dsptr %lld", engine_dsptr);
		ret = -EINVAL;
		goto fail1;
	}
	/*
	 * Build car
	 */
	doors = (int)lcd_r1();
	a = mi->mk_automobile(e, doors);
	if (!a) {
		LCD_ERR("bad auto");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Put in data store
	 */
	ret = lcd_ds_store(a, lcd_reply_badge(), &auto_dsptr);
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
	mi->free_automobile(a);
fail1:
	return ret;
}

/**
 * free_engine_callee
 * ------------------
 * IN:
 * lcd_r1 = dsptr to allocated engine
 * OUT:
 * lcd_r0 = 0
 */
static int free_engine_callee(void)
{
	int ret;
	struct engine *e;
	dsptr_t engine_dsptr;
	/*
	 * Get and remove engine from data store
	 */
	engine_dsptr = (dsptr_t)lcd_r1();
	e = lcd_ds_drop(lcd_reply_badge(), engine_dsptr);
	if (!e) {
		LCD_ERR("bad engine dsptr %lld", engine_dsptr);
		ret = -EINVAL;
		goto fail1;
	}
	/*
	 * Free engine
	 */
	mi->free_engine(e);
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
 * free_automobile_callee
 * ----------------------
 * IN:
 * lcd_r1 = dsptr to allocated auto
 * OUT:
 * lcd_r0 = 0
 */
static int free_automobile_callee(void)
{
	int ret;
	struct automobile *a;
	dsptr_t auto_dsptr;
	/*
	 * Get and remove auto from data store
	 */
	auto_dsptr = (dsptr_t)lcd_r1();
	a = lcd_ds_drop(lcd_reply_badge(), auto_dsptr);
	if (!a) {
		LCD_ERR("bad auto dsptr %lld", auto_dsptr);
		ret = -EINVAL;
		goto fail1;
	}
	/*
	 * Free auto
	 */
	mi->free_automobile(a);
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
 * manufacturer_die_callee
 * -----------------------
 * IN:
 * (reply info)
 * OUT:
 * (nothing)
 */
int manufacturer_die_callee(void)
{
	int ret;
	/*
	 * Call internal exit
	 */
	__manufacturer_exit();
	/*
	 * Reply
	 */
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
	
	/*
	 * Set up call.
	 *
	 * r0 = enum for calling reg manufacturer in dealer interface
	 * boot cptr = points to end point for dealer interface
	 */
	lcd_store_r0(DEALER_REGISTER_MANUFACTURER);
	ret = lcd_call(MANUFACTURER_DEALER_INTERFACE_CAP);
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
				LCD_MSG("manufacturer handling mk engine");
				ret = mk_engine_callee();
				break;
			case MANUFACTURER_MK_AUTOMOBILE:
				LCD_MSG("manufacturer handling mk auto");
				ret = mk_automobile_callee();
				break;
			case MANUFACTURER_FREE_ENGINE:
				LCD_MSG("manufacturer handling free engine");
				ret = free_engine_callee();
				break;
			case MANUFACTURER_FREE_AUTOMOBILE:
				LCD_MSG("manufacturer handling free auto");
				ret = free_automobile_callee();
				break;
			case MANUFACTURER_DIE:
				LCD_MSG("manufacturer dying");
				manufacturer_die_callee();
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
	LCD_MSG("manufacturer entering loop");
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

#endif /* CONFIG_LCD_PROTOTYPE_TEST_IN_LCD */
