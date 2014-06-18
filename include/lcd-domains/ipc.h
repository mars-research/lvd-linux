#ifndef LCD_DOMAINS_IPC_H
#define LCD_DOMAINS_IPC_H

/*
 * Message Registers
 * =================
 *
 * Capability Register     machine register (arch-dep)
 * Badge Register          machine register (arch-dep)
 * Message tag             machine register (arch-dep)
 * Message registers       6 total. All message registers are available in
 *                         struct lcd_ipc_regs. Some message registers may
 *                         also be accessible via machine registers. Use
 *                         arch-dep macros for guaranteed fast and portable
 *                         access.
 *
 * For portability, use the arch-dep macros provided by the arch code
 * to access ipc registers.
 *
 * Based on seL4 message register design. See seL4 Manual, 4.1.
 */

struct lcd_ipc_regs {
	/*
	 * Capability, badge, and message tag are always machine regs.
	 *
	 * Message registers
	 */
	u64 mr[6];
};

#endif  /* LCD_DOMAINS_IPC_H */
