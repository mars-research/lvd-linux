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
 * Note 3: The macros, of course, would be implemented differently
 * for a truly isolated LCD.
 *
 * An lcd_thread is made up of:
 *   -- a task_struct (corresponding to the scheduable 
 *      thread on the host)
 *   -- an lcd_tcb
 *   -- pointer to containing lcd
 *   -- (two fields for lists of lcd_threads)
 *
 * An lcd is made up of:
 *    -- a cspace
 *    -- a list of lcd_threads (that are inside
 *       the lcd)
 */

#ifndef LCD_LCD_H
#define LCD_LCD_H

#include <linux/sched.h>
#include <linux/types.h>
#include <lcd-prototype/lcd-tcb.h>
#include <linux/mutex.h>

#define LCD_NUM_MACHINE_REGS 	8
#define LCD_NUM_MESSAGE_REGS 	8
#define LCD_NUM_OUT_CAP_REGS 	8
#define LCD_NUM_IN_CAP_REGS 	8

typedef u64 cptr_t;
struct cspace;

struct lcd_tcb {
	u64 r[LCD_NUM_MACHINE_REGS];
	unsigned int max_r;
	u64 m[LCD_NUM_MESSAGE_REGS];
	unsigned int max_m;
	cptr_t oc[LCD_NUM_OUT_CAP_REGS];
	int oc_rights[LCD_NUM_OUT_CAP_REGS];
	unsigned int max_oc;
	cptr_t ic[LCD_NUM_IN_CAP_REGS];
	unsigned int max_ic;
};

struct lcd_thread {

	struct task_struct *thread; /* host thread */

	struct lcd_tcb *tcb;        /* this thread's registers, etc. */
	struct lcd *lcd;            /* containing lcd */

	cptr_t call_cptr;           /* cptr to rvp temp granted during call */
	cptr_t reply_cptr;          /* cptr to rvp that a reply will go thru */

	struct list_head threads;   /* list of lcd_threads inside an lcd */
	struct list_head rvp_queue; /* list of lcd_threads in an rvp queue */

	int is_calling;             /* non-zero if thread is sending and send */
                                    /* was part of a call                     */
};

struct lcd {
	struct cspace *cspace;      /* lcd's cspace, shared by all threads */
	struct list_head threads;   /* list of all contained threads */
	struct list_head all_list;  /* list of all lcds */
};

/* MACHINE REGISTERS ---------------------------------------- */

static inline unsigned int lcd_max_r(struct lcd_tcb *tcb)
{
	return tcb->max_r;
}
static inline unsigned int lcd_set_max_r(struct lcd_tcb *tcb, unsigned int val)
{
	BUG_ON(val > 8);
	tcb->max_r = val;
}
static inline void lcd_update_max_r(struct lcd_tcb *tcb, unsigned int idx)
{
	if (idx >= lcd_max_r(tcb))
		lcd_set_max_r(tcb, idx + 1);
}
#define LCD_MK_MACHINE_REG_ACCESS(idx)                  \
static inline u64 __lcd_r##idx(struct lcd_thread *t)    \
{                                                       \
        struct lcd_tcb *tcb = t->tcb;                   \
        BUILD_BUG_ON(idx >= LCD_NUM_MACHINE_REGS);      \
	return tcb->r[idx];                             \
}                                                       \
static inline void __lcd_set_r##idx(struct lcd_thread *t, u64 val) \
{                                                       \
        struct lcd_tcb *tcb = t->tcb;                   \
        BUILD_BUG_ON(idx >= LCD_NUM_MACHINE_REGS);      \
	lcd_update_max_r(tcb, idx);                     \
	tcb->r[idx] = val;                              \
}                                                       \
static inline u64 lcd_r##idx(void)                      \
{                                                       \
        return __lcd_r##idx(current->lcd_thread);       \
}                                                       \
static inline void lcd_set_r##idx(u64 val)              \
{                                                       \
        __lcd_set_r##idx(current->lcd_thread, val);     \
}
LCD_MK_MACHINE_REG_ACCESS(0);
LCD_MK_MACHINE_REG_ACCESS(1);
LCD_MK_MACHINE_REG_ACCESS(2);
LCD_MK_MACHINE_REG_ACCESS(3);
LCD_MK_MACHINE_REG_ACCESS(4);
LCD_MK_MACHINE_REG_ACCESS(5);
LCD_MK_MACHINE_REG_ACCESS(6);
LCD_MK_MACHINE_REG_ACCESS(7);

/* MESSAGE REGISTERS ---------------------------------------- */

static inline unsigned int lcd_max_m(struct lcd_tcb *tcb)
{
	return tcb->max_m;
}
static inline unsigned int lcd_set_max_m(struct lcd_tcb *tcb, unsigned int val)
{
	BUG_ON(val > 8);
	tcb->max_m = val;
}
static inline void lcd_update_max_m(struct lcd_tcb *tcb, unsigned int idx)
{
	if (idx >= lcd_max_m(tcb))
		lcd_set_max_m(tcb, idx + 1);
}
#define LCD_MK_MESSAGE_REG_ACCESS(idx)                  \
static inline u64 __lcd_m##idx(struct lcd_thread *t)    \
{                                                       \
        struct lcd_tcb *tcb = t->tcb;                   \
        BUILD_BUG_ON(idx >= LCD_NUM_MESSAGE_REGS);      \
	return tcb->m[idx];                             \
}                                                       \
static inline void __lcd_set_m##idx(struct lcd_thread *t, u64 val) \
{                                                       \
        struct lcd_tcb *tcb = t->tcb;                   \
        BUILD_BUG_ON(idx >= LCD_NUM_MESSAGE_REGS);      \
	lcd_update_max_m(tcb, idx);                     \
	tcb->m[idx] = val;                              \
}                                                       \
static inline u64 lcd_m##idx(void)                      \
{                                                       \
        return __lcd_m##idx(current->lcd_thread);       \
}                                                       \
static inline void lcd_set_m##idx(u64 val)              \
{                                                       \
        __lcd_set_m##idx(current->lcd_thread, val);     \
}
LCD_MK_MESSAGE_REG_ACCESS(0);
LCD_MK_MESSAGE_REG_ACCESS(1);
LCD_MK_MESSAGE_REG_ACCESS(2);
LCD_MK_MESSAGE_REG_ACCESS(3);
LCD_MK_MESSAGE_REG_ACCESS(4);
LCD_MK_MESSAGE_REG_ACCESS(5);
LCD_MK_MESSAGE_REG_ACCESS(6);
LCD_MK_MESSAGE_REG_ACCESS(7);

/* OUTGOING CPTR REGISTERS ---------------------------------------- */

static inline unsigned int lcd_max_oc(struct lcd_tcb *tcb)
{
	return tcb->max_oc;
}
static inline unsigned int lcd_set_max_oc(struct lcd_tcb *tcb, unsigned int val)
{
	BUG_ON(val > 8);
	tcb->max_oc = val;
}
static inline void lcd_update_max_oc(struct lcd_tcb *tcb, unsigned int idx)
{
	if (idx >= lcd_max_oc(tcb))
		lcd_set_max_oc(tcb, idx + 1);
}
#define LCD_MK_OUT_CAP_REG_ACCESS(idx)					\
static inline cptr_t __lcd_oc##idx(struct lcd_thread *t)	        \
{									\
        struct lcd_tcb *tcb = t->tcb;					\
        BUILD_BUG_ON(idx >= LCD_NUM_OUT_CAP_REGS);			\
	return tcb->oc[idx];						\
}									\
static inline unsigned int __lcd_oc_rights##idx(struct lcd_thread *t)   \
{                                                                       \
        struct lcd_tcb *tcb = t->tcb;                                   \
        BUILD_BUG_ON(idx >= LCD_NUM_OUT_CAP_REGS);                      \
	return tcb->oc_rights[idx];                                     \
}                                                                       \
static inline void __lcd_set_oc##idx(struct lcd_thread *t, cptr_t c,	\
				unsigned int rights)			\
{									\
        struct lcd_tcb *tcb = t->tcb;					\
        BUILD_BUG_ON(idx >= LCD_NUM_OUT_CAP_REGS);			\
	lcd_update_max_oc(tcb, idx);					\
	tcb->oc[idx] = cptr;						\
	tcb->oc_rights[idx] = rights;					\
}									\
static inline u64 lcd_oc##idx(void)					\
{									\
        return __lcd_oc##idx(current->lcd_thread);			\
}									\
static inline u64 lcd_oc_rights##idx(void)				\
{									\
        return __lcd_oc_rights##idx(current->lcd_thread);		\
}									\
static inline void lcd_set_oc##idx(u64 val)				\
{									\
        __lcd_set_oc##idx(current->lcd_thread, val);			\
}
LCD_MK_OUT_CAP_REG_ACCESS(0);
LCD_MK_OUT_CAP_REG_ACCESS(1);
LCD_MK_OUT_CAP_REG_ACCESS(2);
LCD_MK_OUT_CAP_REG_ACCESS(3);
LCD_MK_OUT_CAP_REG_ACCESS(4);
LCD_MK_OUT_CAP_REG_ACCESS(5);
LCD_MK_OUT_CAP_REG_ACCESS(6);
LCD_MK_OUT_CAP_REG_ACCESS(7);

/* INCOMING CPTR REGISTERS ---------------------------------------- */

static inline unsigned int lcd_max_ic(struct lcd_tcb *tcb)
{
	return tcb->max_ic;
}
static inline unsigned int lcd_set_max_ic(struct lcd_tcb *tcb, unsigned int val)
{
	BUG_ON(val > 8);
	tcb->max_ic = val;
}
static inline void lcd_update_max_ic(struct lcd_tcb *tcb, unsigned int idx)
{
	if (idx >= lcd_max_ic(tcb))
		lcd_set_max_ic(tcb, idx + 1);
}
#define LCD_MK_IN_CAP_REG_ACCESS(idx)			\
static inline u64 __lcd_ic##idx(struct lcd_thread *t)   \
{                                                       \
        struct lcd_tcb *tcb = t->tcb;                   \
        BUILD_BUG_ON(idx >= LCD_NUM_IN_CAP_REGS);	\
	return tcb->ic[idx];                            \
}                                                       \
static inline void __lcd_set_ic##idx(struct lcd_thread *t, cptr_t c) \
{                                                       \
        struct lcd_tcb *tcb = t->tcb;                   \
        BUILD_BUG_ON(idx >= LCD_NUM_IN_CAP_REGS);	\
	lcd_update_max_ic(tcb, idx);                    \
	tcb->ic[idx] = c;				\
}                                                       \
static inline u64 lcd_ic##idx(void)                     \
{                                                       \
        return __lcd_ic##idx(current->lcd_thread);	\
}                                                       \
static inline void lcd_set_ic##idx(cptr_t c)		\
{                                                       \
        __lcd_set_ic##idx(current->lcd_thread, c);	\
}
LCD_MK_IN_CAP_REG_ACCESS(0);
LCD_MK_IN_CAP_REG_ACCESS(1);
LCD_MK_IN_CAP_REG_ACCESS(2);
LCD_MK_IN_CAP_REG_ACCESS(3);
LCD_MK_IN_CAP_REG_ACCESS(4);
LCD_MK_IN_CAP_REG_ACCESS(5);
LCD_MK_IN_CAP_REG_ACCESS(6);
LCD_MK_IN_CAP_REG_ACCESS(7);

/* SYSTEM CALLS ---------------------------------------- */

#define LCD_SYSCALL_SEND  0
#define LCD_SYSCALL_RECV  1
#define LCD_SYSCALL_CALL  2
#define LCD_SYSCALL_REPLY 3

/* API CPTR ---------------------------------------- */

#define LCD_API_CPTR 1

/* ERROR CODES ---------------------------------------- */

#define LCD_EINVAL ((u64)-EINVAL)
#define LCD_EINTR  ((u64)-EINTR)
#define LCD_EIO    ((u64)-EIO)

/* GLUE MAIN DECLARATION MACRO ---------------------------------------- */

#define set_gmain(fn)				       \
	static int __##fn##(int argc, cptr_t *cptrs[]) \
	__attribute__((__section__(".gmain"))) = fn

#endif /* LCD_LCD_H */
