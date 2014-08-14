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
 * Send data in from's lcd->utcb to first receiver in line for the
 * endpoint identified by c in t's cspace. The receiver will get
 * a copy of from's registers (up to max valid) and out capabilities
 * will be copied and inserted into the receiver's cspace, using
 * the in cap registers.
 *
 * If a receiver is not waiting, from is placed in the end point's
 * sender wait list, and from is put to sleep.
 */
int __lcd_send(struct task_struct *from, cptr_t c);
/**
 * Other side of send (see above).
 * 
 * If a sender is not waiting, to is placed in wait list and put to
 * sleep.
 */
int __lcd_recv(struct task_struct *to, cptr_t c);
/**
 * Like a send, but does a matching receive call on a capability just
 * for this purpose. The receiver gets a copy of this capability that
 * it can use once for a return call.
 */
int __lcd_call(struct task_struct *from, cptr_t c);
/**
 * Places from on receive queue for all end points identified by the 
 * cptr's cs. When a matching send is received, idx points to the
 * cptr in cs on which the receive happened.
 */
int __lcd_select(struct task_struct *from, cptr_t *cs, int cs_count,
		cptr_t *c_out);

#endif /* LCD_PROTOTYPE_API_INTERNAL_H */
