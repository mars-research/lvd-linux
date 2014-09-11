/*
 * api-internal.h - direct access to hypervisor routines
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 * Use the higher-level calls in ipc.h. In the future, 
 * these won't be directly callable.
 */

#ifndef LCD_PROTOTYPE_API_INTERNAL_H
#define LCD_PROTOTYPE_API_INTERNAL_H

#include <lcd-prototype/lcd.h>

/*
 * These will be replaced by "hypercall" handlers in the real thing,
 * so the task argument most likely won't be needed.
 *
 * Note that lcd thread's directly call into these routines,
 * violating the isolation of the lcd's from the api code.
 */

/**
 * Send data in lcd->utcb to first receiver in line for the
 * endpoint identified by c in lcd's cspace. The receiver will get
 * a copy of lcd's registers (up to max valid) and out capabilities
 * will be copied and inserted into the receiver's cspace, using
 * the in cap registers.
 *
 * If a receiver is not waiting, lcd is placed in the end point's
 * sender wait list and put to sleep.
 */
int __lcd_send(struct lcd *lcd, cptr_t c);
/**
 * Other side of send (see above).
 * 
 * If a sender is not waiting, lcd is placed in wait list and put to
 * sleep.
 */
int __lcd_recv(struct lcd *lcd, cptr_t c);
/**
 * Like a send, but does a matching receive call on a capability just
 * for this purpose. The receiver gets a copy of this capability that
 * it can use once for a return call.
 */
int __lcd_call(struct lcd *lcd, cptr_t c);
/**
 * Other side of a lcd call.
 */
int __lcd_reply(struct lcd *lcd);
/**
 * Create a synchronous end point, and install it in lcd's cspace
 * at location cptr.
 */
int __lcd_mk_sync_endpoint(struct lcd *lcd, cptr_t cptr);
/**
 * Remove synchronous end point identified by cptr in lcd's cspace.
 */
int __lcd_rm_sync_endpoint(struct lcd *lcd, cptr_t cptr);


/**
 * Places from on receive queue for all end points identified by the 
 * cptr's cs. When a matching send is received, idx points to the
 * cptr in cs on which the receive happened.
 */
int __lcd_select(struct lcd *from, cptr_t *cs, int cs_count,
		cptr_t *c_out);

#endif /* LCD_PROTOTYPE_API_INTERNAL_H */
