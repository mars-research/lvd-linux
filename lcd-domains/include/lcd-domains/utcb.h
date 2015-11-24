/**
 * utcb.h - header for user thread control block
 *
 * This contains the message / capability registers.
 */

#ifndef LCD_DOMAINS_UTCB_H
#define LCD_DOMAINS_UTCB_H

#include <lcd-domains/types.h>
#include <linux/types.h>

/*
 * --------------------------------------------------
 *
 * UTCB - user thread control block
 *
 * This is the message buffer mapped inside the LCD.
 *
 *   -- mr are general registers and are just copied during a send/recv
 *   -- cr are capability pointer registers, and the corresponding 
 *      capabilities are granted during a send/recv
 *   -- one debug register that won't squash the others
 *
 * NOTE: This needs to fit inside a page (4 KBs). It goes at the bottom
 * of the lcd's stack (so it should be reasonably small).
 */

#define LCD_NUM_REGS 8

struct lcd_utcb {
	u64 mr[LCD_NUM_REGS];
	cptr_t cr[LCD_NUM_REGS];
	u64 debug_reg;
};

#define UTCB_MK_REG_ACCESS(idx)						\
static inline u64 __lcd_r##idx(struct lcd_utcb *utcb)		        \
{									\
	return utcb->mr[idx];						\
}									\
static inline void __lcd_set_r##idx(struct lcd_utcb *utcb, u64 val)	\
{									\
	utcb->mr[idx] = val;						\
}									\
static inline cptr_t __lcd_cr##idx(struct lcd_utcb *utcb)		\
{									\
	return utcb->cr[idx];						\
}									\
static inline void __lcd_set_cr##idx(struct lcd_utcb *utcb, cptr_t val)	\
{									\
	utcb->cr[idx] = val;						\
}									\

UTCB_MK_REG_ACCESS(0)
UTCB_MK_REG_ACCESS(1)
UTCB_MK_REG_ACCESS(2)
UTCB_MK_REG_ACCESS(3)
UTCB_MK_REG_ACCESS(4)
UTCB_MK_REG_ACCESS(5)
UTCB_MK_REG_ACCESS(6)
UTCB_MK_REG_ACCESS(7)

static inline u64 __lcd_debug_reg(struct lcd_utcb *utcb)	        \
{									\
	return utcb->debug_reg;						\
}									\
static inline void __lcd_set_debug_reg(struct lcd_utcb *utcb, u64 val)	\
{									\
	utcb->debug_reg = val;						\
}									\


#endif /* LCD_DOMAINS_UTCB_H */
