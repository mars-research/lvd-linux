/*
 * sync_ipc.h
 *
 * Synchonous IPC. This part of the interface contains:
 *
 *    - User Thread Control Block (UTCB). This is the message buffer
 *      used for synchronous IPC. Each thread has exactly one
 *      UTCB.
 *    - Accessors for the UTCB.
 *    - Functions for doing synchronous IPC.
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_SYNC_IPC_H
#define LCD_DOMAINS_SYNC_IPC_H

#include <linux/bug.h>
#include <libcap.h>

/* UTCB ---------------------------------------- */

#define LCD_NUM_REGS 8
struct lcd_utcb {
	/**
	 * mr registers are general purpose 64-bit registers. You can
	 * store any scalars in here.
	 */
	unsigned long mr[LCD_NUM_REGS];
	/**
	 * cr registers are for cptr_t's. These registers are used during
	 * the capability grant process. See lcd_sync_send for more
	 * details.
	 */
	cptr_t cr[LCD_NUM_REGS];
};

/* UTCB ACCESS ---------------------------------------- */

/**
 * lcd_get_utcb -- Get pointer to calling thread's UTCB.
 */
struct lcd_utcb *lcd_get_utcb(void);

/**
 * Accessors for UTCB. For example, lcd_r0 reads mr[0], lcd_set_cr2 stores
 * a value in cr[2], and so on.
 */
#define LCD_MK_REG_ACCESS(idx)						\
static inline unsigned long lcd_r##idx(void)			        \
{									\
	BUILD_BUG_ON(idx >= LCD_NUM_REGS);				\
	return lcd_get_utcb()->mr[idx];					\
}									\
static inline void lcd_set_r##idx(unsigned long val)			\
{									\
	lcd_get_utcb()->mr[idx] = val;					\
}									\
static inline cptr_t lcd_cr##idx(void)					\
{									\
        return lcd_get_utcb()->cr[idx];					\
}								        \
static inline void lcd_set_cr##idx(cptr_t val)				\
{									\
	lcd_get_utcb()->cr[idx] = val;					\
}									
LCD_MK_REG_ACCESS(0)
LCD_MK_REG_ACCESS(1)
LCD_MK_REG_ACCESS(2)
LCD_MK_REG_ACCESS(3)
LCD_MK_REG_ACCESS(4)
LCD_MK_REG_ACCESS(5)
LCD_MK_REG_ACCESS(6)
LCD_MK_REG_ACCESS(7)

/* SYNCHRONOUS IPC ---------------------------------------- */

/**
 * lcd_create_sync_endpoint -- Create synchronous IPC channel/endpoint.
 * @slot_out: slot where new endpoint was stored in caller's cspace
 *
 * Allocates a slot in the caller's cspace. Calls into microkernel to
 * create a new synchronous IPC channel; tells microkernel to store
 * capability to new channel in the slot allocated. Returns the
 * slot as an out parameter.
 */
int lcd_create_sync_endpoint(cptr_t *slot_out);

/**
 * __lcd_create_sync_endpoint -- Create synchronous ep, store cap in slot.
 * @slot: slot in caller's cspace where capability to new ep should be stored
 *
 * Similar to lcd_create_sync_endpoint, but the caller should have allocated
 * a slot in their cspace (at @slot) where the capability to the new
 * endpoint should be stored.
 */
int __lcd_create_sync_endpoint(cptr_t slot);

/**
 * lcd_sync_send -- synchronous IPC send
 * @endpoint: cptr to a capability for a synchronous IPC channel/endpoint
 *
 * Prior to calling this function, the caller should store values into
 * its UTCB using lcd_set_r0, lcd_set_cr0, and so on.
 *
 * This call blocks until there is a matching receiver on the channel. Once
 * there is a matching receiver, the microkernel pairs sender and receiver
 * and transfers the sender's UTCB contents to the receiver's UTCB, as
 * follows:
 *
 *    - all scalar registers are memcpy'd from sender to receiver
 *    - for each cptr register in the sender's UTCB, if the register
 *      is some non-null value X *and* the corresponding cptr register in 
 *      the receiver's UTCB is some non-null value Y, the microkernel does 
 *      a grant from the sender's cspace to the receiver's cspace; the 
 *      capability in slot X in the sender's cspace is basically copied
 *      into slot Y in the receiver's cspace (along with updating the cdt)
 */
int lcd_sync_send(cptr_t endpoint);

/**
 * lcd_sync_recv -- synchronous IPC receive
 * @endpoint: cptr to a capability for a synchronous IPC channel/endpoint
 *
 * This is the receive side of synchronous IPC. See lcd_sync_send for
 * more details.
 */
int lcd_sync_recv(cptr_t endpoint);

/**
 * lcd_sync_call -- synchronous IPC call
 * @endpoint: cptr to a capability for a synchronous IPC channel/endpoint
 *
 * This is equivalent to an lcd_sync_send on @endpoint followed by
 * an lcd_sync_recv on the caller's special reply endpoint. The
 * receiver of the lcd_sync_send will be granted a temporary
 * capability to @endpoint. When the receiver invokes a send or
 * reply on this temporary capability, it will be revoked.
 */
int lcd_sync_call(cptr_t endpoint);

/**
 * lcd_sync_reply -- synchronous IPC reply
 * @endpoint: cptr to a capability for a synchronous IPC channel/endpoint
 *
 * This is equivalent to an lcd_sync_send on the temporary
 * capability to an endpoint the caller received from a prior
 * lcd_sync_recv from another LCD (the other LCD did lcd_sync_call). 
 * This temporary capability will be revoked when the send completes.
 */
int lcd_sync_reply(void);


#endif /* LCD_DOMAINS_SYNC_IPC_H */
