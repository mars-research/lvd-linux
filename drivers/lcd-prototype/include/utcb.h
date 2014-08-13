/*
 * utcb.h - access helpers for utcb
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

#ifndef LCD_PROTOTYPE_UTCB_H
#define LCD_PROTOTYPE_UTCB_H

#include <lcd-prototype/lcd.h>

/* BOOT CPTRS -------------------------------------------------- */

static inline cptr_t lcd_boot_cptr(u8 idx)
{
	BUG_ON(idx >= LCD_NUM_BOOT_CPTRS);
	return current->lcd->utcb.boot_cptrs[idx];
}

/* GENERAL REGISTERS ---------------------------------------- */

static inline u8 lcd_max_valid_reg_idx(void)
{
	return current->lcd->utcb.max_valid_reg_idx;
}
static inline void lcd_update_max_valid_reg_idx(u8 idx)
{
	if (idx > current->lcd->utcb.max_valid_reg_idx)
		current->lcd->utcb.max_valid_reg_idx = idx;
}
#define LCD_MK_REG_ACCESS(idx)               \
static inline u64 lcd_r##idx(void)           \
{                                            \
        BUILD_BUG_ON(idx >= LCD_NUM_REGS);   \
	return current->lcd.utcb.regs[idx];  \
}                                            \
static inline void lcd_store_r##idx(u64 val) \
{                                            \
        BUILD_BUG_ON(idx >= LCD_NUM_REGS);   \
	lcd_update_max_valid_reg_idx(idx);   \
	current->lcd.utcb.regs[idx] = val;   \
}
LCD_MK_REG_ACCESS(0);
LCD_MK_REG_ACCESS(1);
LCD_MK_REG_ACCESS(2);
LCD_MK_REG_ACCESS(3);
LCD_MK_REG_ACCESS(4);
LCD_MK_REG_ACCESS(5);
LCD_MK_REG_ACCESS(6);
LCD_MK_REG_ACCESS(7);

/* OUTGOING CPTR REGISTERS ---------------------------------------- */

static inline u8 lcd_max_valid_out_cap_reg_idx(void)
{
	return current->lcd->utcb.max_valid_out_cap_reg_idx;
}
static inline void lcd_update_max_valid_out_cap_reg_idx(u8 idx)
{
	if (idx > current->lcd->utcb.max_valid_out_cap_reg_idx)
		current->lcd->utcb.max_valid_out_cap_reg_idx = idx;
}
#define LCD_MK_OUT_CAP_REG_ACCESS(idx)                \
static inline u64 lcd_out_cap##idx(void)              \
{                                                     \
        BUILD_BUG_ON(idx >= LCD_NUM_OUT_CAP_REGS);    \
	return current->lcd.utcb.out_cap_regs[idx];   \
}
static inline void lcd_store_out_cap##idx(cptr_t val) \
{                                                     \
        BUILD_BUG_ON(idx >= LCD_NUM_OUT_CAP_REGS);    \
	lcd_update_max_valid_out_cap_reg_idx(idx);    \
	current->lcd.utcb.out_cap_regs[idx] = val;    \
}
LCD_MK_OUT_CAP_REG_ACCESS(0);
LCD_MK_OUT_CAP_REG_ACCESS(1);
LCD_MK_OUT_CAP_REG_ACCESS(2);
LCD_MK_OUT_CAP_REG_ACCESS(3);
LCD_MK_OUT_CAP_REG_ACCESS(4);
LCD_MK_OUT_CAP_REG_ACCESS(5);
LCD_MK_OUT_CAP_REG_ACCESS(6);
LCD_MK_OUT_CAP_REG_ACCESS(7);

/* INCOMING CPTR REGISTERS ------------------------------------------------ */

static inline u8 lcd_max_valid_in_cap_reg_idx(void)
{
	return current->lcd->utcb.max_valid_in_cap_reg_idx;
}
static inline void lcd_update_max_valid_in_cap_reg_idx(u8 idx)
{
	if (idx > current->lcd->utcb.max_valid_in_cap_reg_idx)
		current->lcd->utcb.max_valid_in_cap_reg_idx = idx;
}
#define LCD_MK_IN_CAP_REG_ACCESS(idx)                 \
static inline u64 lcd_in_cap##idx(void)               \
{                                                     \
        BUILD_BUG_ON(idx >= LCD_NUM_IN_CAP_REGS);     \
	return current->lcd.utcb.in_cap_regs[idx];    \
}
static inline void lcd_store_in_cap##idx(cptr_t val)  \
{                                                     \
        BUILD_BUG_ON(idx >= LCD_NUM_IN_CAP_REGS);     \
	lcd_update_max_valid_in_cap_reg_idx(idx);     \
	current->lcd.utcb.in_cap_regs[idx] = val;     \
}
LCD_MK_IN_CAP_REG_ACCESS(0);
LCD_MK_IN_CAP_REG_ACCESS(1);
LCD_MK_IN_CAP_REG_ACCESS(2);
LCD_MK_IN_CAP_REG_ACCESS(3);
LCD_MK_IN_CAP_REG_ACCESS(4);
LCD_MK_IN_CAP_REG_ACCESS(5);
LCD_MK_IN_CAP_REG_ACCESS(6);
LCD_MK_IN_CAP_REG_ACCESS(7);

/* REPLY INFO -------------------------------------------------- */

static inline u64 lcd_reply_badge(void)
{
	return current->lcd->utcb.reply_badge;
}
static inline void lcd_store_reply_badge(u64 val)
{
	current->lcd->utcb.reply_badge = val;
}
static inline cptr_t lcd_reply_cap(void)
{
	return current->lcd->utcb.reply_cap;
}
static inline void lcd_store_reply_cap(cptr_t val)
{
	current->lcd->utcb.reply_cap = val;
}

#endif /* LCD_PROTOTYPE_UTCB_H */
