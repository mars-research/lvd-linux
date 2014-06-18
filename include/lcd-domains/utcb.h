#ifndef LCD_DOMAINS_UTCB_H
#define LCD_DOMAINS_UTCB_H

/*
 * User thread control block
 * =========================
 *
 * This is accessible (mapped) inside the lcd.
 */
struct lcd_utcb {
	/*
	 * IPC registers
	 * =============
	 *
	 * Capability Register     machine register (arch-dep)
	 * Badge Register          machine register (arch-dep)
	 * Message tag             machine register (arch-dep)
	 * Message registers       6 total. All message registers are 
	 *                         available in the ipc.mr array. Some 
	 *                         message registers may also be accessible 
	 *                         via machine registers. Use arch-dep macros 
	 *                         for guaranteed fast and portable access.
	 *
	 * For portability, use the arch-dep macros provided by the arch code
	 * to access ipc registers.
	 *
	 * Based on seL4 message register design. See seL4 Manual, 4.1.
	 */
	struct {
		u64 mr[6];
	} ipc;
};

#endif /* LCD_DOMAINS_UTCB_H */
