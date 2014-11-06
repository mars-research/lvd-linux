/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 * An lcd_tcb (thread control block) is made up of:
 *
 *   -- array of 8 machine registers
 *   -- unsigned int of how many machine registers
 *      to copy during a send/call
 *
 *   -- array of 8 message registers
 *   -- unsigned int of how many message registers
 *      to copy during a send/call
 *
 *   -- array of 8 outgoing cptr's
 *   -- array of 8 outgoing rights for outgoing cptr's
 *   -- unsigned int of how many outgoing cptr
 *      regs contain valid cptr's to grant to the
 *      receiver
 *
 *   -- array of 8 incoming cptr's
 *   -- unsigned int of how many incoming cap's
 *      to receive (how many incoming cptr's
 *      are valid)
 *
 * Note 1: This differs from seL4. The `minting' of capabilities
 * occurs *during* the ipc send/recv, rather than *before* with
 * a separate system call. The microkernel confirms the sender
 * has grant rights during the ipc, and only those sender caps
 * that have grant rights are transferred; the rest are
 * ignored. I did this because if the sender is expected to
 * mint before sending, the microkernel would need to confirm
 * that only minted caps are sent in the outgoing caps registers.
 * (Or, as seL4 does, do a redundant grant rights check, as far
 * as I understand seL4.)
 *
 * Note 2: The unsigned int's could be `coalesced' into one
 * since only 3 bits are needed to record the highest valid
 * register index
 *
 * An lcd_thread is made up of:
 *   -- a task_struct (corresponding to the scheduable 
 *      thread on the host)
 *   -- an lcd_tcb
 *   -- (two fields for lists of lcd_threads)
 *
 * An lcd is made up of:
 *    -- a cspace
 *    -- a list of lcd_threads (that are inside
 *       the lcd)
 */

#ifndef LCD_LCD_H
#define LCD_LCD_H

#define LCD_NUM_MACHINE_REGS 	8
#define LCD_NUM_MESSAGE_REGS 	8
#define LCD_NUM_OUT_CAP_REGS 	8
#define LCD_NUM_IN_CAP_REGS 	8

struct cspace;
typedef u64 cptr_t;

struct lcd_tcb {
	u64 r[LCD_NUM_MACHINE_REGS];
	unsigned int max_r;
	u64 m[LCD_NUM_MESSAGE_REGS];
	unsigned int max_m;
	u64 oc[LCD_NUM_OUT_CAP_REGS];
	int oc_rights[LCD_NUM_OUT_CAP_REGS];
	unsigned int max_oc;
	u64 ic[LCD_NUM_IN_CAP_REGS];
	unsigned int max_ic;
};

struct lcd_thread {
	struct *task_struct;
	struct *lcd_tcb;
	struct list_head threads;
	struct list_head rvp_queue;
};

struct lcd {
	struct cspace *cspace;
	struct list_head threads;
};

#endif /* LCD_LCD_H */
