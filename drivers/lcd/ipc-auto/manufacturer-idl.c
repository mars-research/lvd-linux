/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */

#ifdef CONFIG_RUN_IN_LCD

/* INTERNAL DEFS -------------------------------------------------- */

#include "dealer.h"
#include "common.h"

extern int __manufacturer_init(void);
extern void __manufacturer_exit(void);

/* IDL/LCD DEFS -------------------------------------------------- */

#include "manufacturer-idl.h"

/* INTERFACE WRAPPERS -------------------------------------------------- */

struct manufacturer_interface *mi;
struct {
	cptr_t mk_engine;
	cptr_t mk_automobile;
	cptr_t free_engine;
	cptr_t free_automobile;
} manufacturer_interface_cptrs;

/**
 * mk_engine_callee
 * ----------------
 * IN:
 * lcd_r0 = number of cylinders
 * lcd_reply_cap = cptr to reply rendezvous point
 * lcd_reply_badge = badge identifying sender
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
	cylinders = (int)lcd_r0();
	e = mi->mk_engine(cylinders);
	if (!e) {
		AU_ERR("bad engine");
		ret = -1;
		goto fail1;
	}
	/*
	 * Put in data store
	 */
	ret = lcd_ds_store(e, &engine_dsptr, lcd_reply_badge());
	if(ret) {
		AU_ERR("store engine");
		goto fail2;
	}
	/*
	 * Reply
	 */
	lcd_store_r0(0);
	lcd_store_r1(engine_dsptr);
	ret = lcd_reply();
	if (ret) {
		AU_ERR("couldn't reply");
		goto fail3;
	}

	return ret;
fail3:
	__lcd_ds_drop(lcd_reply_badge(), engine_dsptr);
fail2:
	mi->free_engine(e);
fail1:
	return ret;
}

/**
 * mk_automobile_callee
 * --------------------
 * IN:
 * lcd_r0 = number of doors
 * lcd_r1 = dsptr to allocated engine
 * lcd_reply_cap = cptr to reply rendezvous point
 * lcd_reply_badge = badge identifying sender
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
	engine_dsptr = (dsptr_t)lcd_r1();
	e = lcd_ds_read(lcd_reply_badge(), engine_dsptr);
	if (!e) {
		AU_ERR("bad engine dsptr %lld", engine_dsptr);
		ret = -EINVAL;
		goto fail1;
	}
	/*
	 * Build car
	 */
	doors = (int)lcd_r0();
	a = mi->mk_automobile(e, doors);
	if (!a) {
		AU_ERR("bad auto");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Put in data store
	 */
	ret = lcd_ds_store(a, &auto_dsptr, lcd_reply_badge());
	if(ret) {
		AU_ERR("store auto");
		goto fail2;
	}
	/*
	 * Reply
	 */
	lcd_store_r0(0);
	lcd_store_r1(auto_dsptr);
	ret = lcd_reply();
	if (ret) {
		AU_ERR("couldn't reply");
		goto fail3;
	}

	return ret;

fail3:
	__lcd_ds_drop(lcd_reply_badge(), auto_dsptr);
fail2:
	mi->free_automobile(a);
fail1:
	return ret;
}

/**
 * free_engine_callee
 * ------------------
 * IN:
 * lcd_r0 = dsptr to allocated engine
 * lcd_reply_cap = cptr to reply rendezvous point
 * lcd_reply_badge = badge identifying sender
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
	engine_dsptr = (dsptr_t)lcd_r0();
	e = lcd_ds_drop(lcd_reply_badge(), engine_dsptr);
	if (!e) {
		AU_ERR("bad engine dsptr %lld", engine_dsptr);
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
		AU_ERR("couldn't reply");

	return ret;

fail1:
	return ret;
}

/**
 * free_automobile_callee
 * ----------------------
 * IN:
 * lcd_r0 = dsptr to allocated auto
 * lcd_reply_cap = cptr to reply rendezvous point
 * lcd_reply_badge = badge identifying sender
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
	auto_dsptr = (dsptr_t)lcd_r0();
	e = lcd_ds_drop(lcd_reply_badge(), auto_dsptr);
	if (!e) {
		AU_ERR("bad auto dsptr %lld", auto_dsptr);
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
		AU_ERR("couldn't reply");

	return ret;

fail1:
	return ret;
}

/**
 * manufacturer_die_callee
 * -----------------------
 * IN:
 * lcd_reply_cap = cptr to reply rendezvous point
 * lcd_reply_badge = badge identifying sender
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
		AU_ERR("couldn't reply");
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
	 * Create endpoints for functions in interface
	 */
	ret = lcd_mk_sync_endpoint(manufacturer_interface_cptrs.mk_engine); 
	if(ret) {
		AU_ERR("mk engine endpoint");
		goto fail1;
	}
	ret = lcd_mk_sync_endpoint(manufacturer_interface_cptrs.mk_automobile); 
	if(ret) {
		AU_ERR("mk auto endpoint");
		goto fail2;
	}
	ret = lcd_mk_sync_endpoint(manufacturer_interface_cptrs.free_engine); 
	if(ret) {
		AU_ERR("free engine endpoint");
		goto fail3;
	}
	ret = lcd_mk_sync_endpoint(manufacturer_interface_cptrs.free_automobile); 
	if(ret) {
		AU_ERR("free auto endpoint");
		goto fail4;
	}

	/*
	 * Store in the cap regs for the caller, and send
	 */
	lcd_store_cap0(manufacturer_interface_cptrs.mk_engine);
	lcd_store_cap1(manufacturer_interface_cptrs.mk_automobile);
	lcd_store_cap2(manufacturer_interface_cptrs.free_engine);
	lcd_store_cap3(manufacturer_interface_cptrs.free_automobile);
	ret = lcd_call(lcd_boot_cptr(DEALER_REGISTER_MANUFACTURER));
	if (ret) {
		AU_ERR("call to dealer");
		goto fail5;
	}

	return (int)lcd_r0();

fail5:
	lcd_destroy_sync_endpoint(manufacturer_interface_cptrs.free_automobile);
fail4:
	lcd_destroy_sync_endpoint(manufacturer_interface_cptrs.free_engine);
fail3:
	lcd_destroy_sync_endpoint(manufacturer_interface_cptrs.mk_automobile);
fail2:
	lcd_destroy_sync_endpoint(manufacturer_interface_cptrs.mk_engine);
fail1:
	return -1;
}

/* MAIN EXEC LOOP -------------------------------------------------- */

int execution_loop(void)
{
	int ret;
	struct lcd_handler *handlers[] = { 
		{ manufacturer_interface_cptrs.mk_engine,
		  mk_engine_callee },
		{ manufacturer_interface_cptrs.mk_automobile,
		  mk_automobile_callee },
		{ manufacturer_interface_cptrs.free_engine,
		  free_engine_callee },
		{ manufacturer_interface_cptrs.free_automobile,
		  free_automobile_callee },
		{ lcd_boot_cptr(MANUFACTURER_DIE),
		  manufacturer_die_callee } };
	/* 
	 * Loop unless there's an internal error, or we get killed
	 */
	for (;;) {
		/*
		 * Listen for incoming message
		 */
		ret = lcd_select(handlers, 5);
		if (ret < 0) {
			AU_ERR("receive via select");
			goto out;
		}
		if (ret > 0) {
			AU_MSG("dying");
			goto out;
		}
	}

out:
	return ret;
}

int __init manufacturer_init(void)
{
	int ret;
	/*
	 * Call manufacturer init, should register interface.
	 */
	ret = __manufacturer_init();
	if (ret) {
		AU_ERR("manufacturer init");
		return -1;
	}
	/*
	 * Ensure interface is set
	 */
	if (!mi) {
		AU_ERR("interface not set");
		return -1;
	}
	/*
	 * Now enter loop and listen for calls
	 */
	return execution_loop();
}

/* manufacturer_die does the actual tear down */
void __exit manufacturer_exit(void)
{
	return;
}

module_init(manufacturer_init);
module_exit(manufacturer_exit);

#endif /* CONFIG_RUN_IN_LCD */
