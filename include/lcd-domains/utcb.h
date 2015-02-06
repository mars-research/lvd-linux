#ifndef LCD_DOMAINS_UTCB_H
#define LCD_DOMAINS_UTCB_H

#define LCD_NUM_REGS 8

/*
 * User thread control block
 * =========================
 *
 * Each lcd_thread has one of these. This is where it puts its
 * data for a message. mr are general registers and are just
 * copied during a send/recv. cr are capability pointer registers, and
 * the corresponding capabilities are granted during a send/recv (like
 * seL4).
 *
 * NOTE: This needs to fit inside a page (4 KBs). It goes at the bottom
 * of the lcd_thread's stack (so it should be reasonably small).
 */
struct lcd_utcb {
	u64 mr[LCD_NUM_REGS];
	cptr_t cr[LCD_NUM_REGS];
};

/* GENERAL REGISTERS ---------------------------------------- */

#define LCD_MK_REG_ACCESS(idx)                                 \
static inline u64 __lcd_r##idx(struct lcd_thread *t)           \
{									\
        BUILD_BUG_ON(idx >= LCD_NUM_REGS);				\
	return t->utcb.mr[idx];						\
}									\
static inline void __lcd_set_r##idx(struct lcd_thread *t, u64 val)	\
{									\
        BUILD_BUG_ON(idx >= LCD_NUM_REGS);				\
	t->utcb.mr[idx] = val;						\
}									\
static inline u64 lcd_r##idx(void)					\
{									\
        return __lcd_r##idx(current->lcd_thread);			\
}									\
static inline void lcd_set_r##idx(u64 val)				\
{									\
	__lcd_set_r##idx(current->lcd_thread, val);			\
}									\

LCD_MK_REG_ACCESS(0);
LCD_MK_REG_ACCESS(1);
LCD_MK_REG_ACCESS(2);
LCD_MK_REG_ACCESS(3);
LCD_MK_REG_ACCESS(4);
LCD_MK_REG_ACCESS(5);
LCD_MK_REG_ACCESS(6);
LCD_MK_REG_ACCESS(7);

/* CPTR REGISTERS ---------------------------------------- */

#define LCD_MK_CAP_REG_ACCESS(idx)		      \
static inline cptr_t __lcd_cr##idx(struct lcd_thread *t)	\
{								\
        BUILD_BUG_ON(idx >= LCD_NUM_REGS);			\
	return t->utcb.cr[idx];					\
}								\
static inline u64 __lcd_set_cr##idx(struct lcd_thread *t, cptr_t val)	\
{									\
        BUILD_BUG_ON(idx >= LCD_NUM_REGS);            			\
	t->utcb.cr[idx] = val;						\
}									\
static inline cptr_t lcd_cr##idx(void)					\
{									\
        return __lcd_cr##idx(current->lcd_thread);      		\
}								        \
static inline u64 lcd_set_cr##idx(cptr_t val)				\
{									\
	__lcd_set_cr##idx(current->lcd_thread, val);			\
}									\

LCD_MK_CAP_REG_ACCESS(0);
LCD_MK_CAP_REG_ACCESS(1);
LCD_MK_CAP_REG_ACCESS(2);
LCD_MK_CAP_REG_ACCESS(3);
LCD_MK_CAP_REG_ACCESS(4);
LCD_MK_CAP_REG_ACCESS(5);
LCD_MK_CAP_REG_ACCESS(6);
LCD_MK_CAP_REG_ACCESS(7);

#endif /* LCD_DOMAINS_UTCB_H */
