/*
 * run.h
 *
 * Functions for running a VT-x container and handling
 * vmcalls.
 *
 * Copyright: University of Utah
 */
#ifndef ASM_X86_LCD_DOMAINS_RUN_H
#define ASM_X86_LCD_DOMAINS_RUN_H

/**
 * Runs lcd_arch on the calling cpu. (If the LCD is active on
 * a different cpu, it will become inactive there.) Kernel
 * preemption is disabled while t is launched, but
 * external interrupts are not disabled and will be handled.
 *
 * Unless the caller does otherwise, kernel preemption is
 * enabled before returning.
 *
 * Returns status code (e.g., LCD_ARCH_STATUS_PAGE_FAULT)
 * so that caller knows why lcd_arch exited and can respond.
 */
int lcd_arch_run(struct lcd_arch *lcd_arch);

/**
 * Status codes for running LCDs.
 */
enum lcd_arch_status {
	LCD_ARCH_STATUS_PAGE_FAULT = 0,
	LCD_ARCH_STATUS_EXT_INTR   = 1,
	LCD_ARCH_STATUS_EPT_FAULT  = 2,
	LCD_ARCH_STATUS_CR3_ACCESS = 3,
	LCD_ARCH_STATUS_SYSCALL    = 4,
};

/**
 * Set the lcd's program counter to the guest virtual address
 * a.
 */
int lcd_arch_set_pc(struct lcd_arch *lcd_arch, gva_t a);
/**
 * Read LCD's %rip
 */
static inline u64 lcd_arch_get_pc(struct lcd_arch *lcd)
{
	return lcd->regs.rip;
}
/**
 * Set the lcd's stack pointer to the guest virtual address
 * a.
 */
int lcd_arch_set_sp(struct lcd_arch *lcd_arch, gva_t a);
/**
 * Set the lcd's gva root pointer (for x86, %cr3) to the
 * guest physical address a.
 */
int lcd_arch_set_gva_root(struct lcd_arch *lcd_arch, gpa_t a);

/*
 * Accessor Macro for syscalls
 * ===========================
 */
static inline u64 lcd_arch_get_syscall_num(struct lcd_arch *lcd)
{
	return lcd->regs.rax;
}
static inline u64 lcd_arch_get_syscall_arg0(struct lcd_arch *lcd)
{
	return lcd->regs.r8;
}
static inline u64 lcd_arch_get_syscall_arg1(struct lcd_arch *lcd)
{
	return lcd->regs.r9;
}
static inline u64 lcd_arch_get_syscall_arg2(struct lcd_arch *lcd)
{
	return lcd->regs.r10;
}
static inline u64 lcd_arch_get_syscall_arg3(struct lcd_arch *lcd)
{
	return lcd->regs.r11;
}
static inline u64 lcd_arch_get_syscall_arg4(struct lcd_arch *lcd)
{
	return lcd->regs.r12;
}
static inline void lcd_arch_set_syscall_ret(struct lcd_arch *lcd, u64 val)
{
	lcd->regs.rax = val;
}

/* Stack and register state */

void lcd_show_execution_state(struct lcd_arch *lcd, const struct cpu_user_regs *regs);
void lcd_show_registers(const struct cpu_user_regs *regs);

#endif /* ASM_X86_LCD_DOMAINS_RUN_H */
