/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

#ifndef LCD_LCD_H
#define LCD_LCD_H

#define LCD_NUM_REGS 		8
#define LCD_NUM_OUT_CAP_REGS 	8
#define LCD_NUM_IN_CAP_REGS 	LCD_NUM_OUT_CAP_REGS
#define LCD_NUM_BOOT_CPTRS 	8

struct cspace;
struct dstore;
typedef u64 cptr_t;

struct lcd {
	u64 badge;
	struct cspace *cspace;
	struct {
		cptr_t boot_cptrs[LCD_NUM_BOOT_CPTRS];
		u64 regs[LCD_NUM_REGS];
		cptr_t out_cap_regs[LCD_NUM_OUT_CAP_REGS];
		cptr_t in_cap_regs[LCD_NUM_IN_CAP_REGS];
		u8 valid_regs;
		u8 valid_out_cap_regs;
		u8 valid_in_cap_regs;
		cptr_t reply_cptr;
		cptr_t reply_badge;
	} utcb;
	struct dstore *dstore;
};

#endif /* LCD_LCD_H */
