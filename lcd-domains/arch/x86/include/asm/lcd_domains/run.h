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

#include <lcd_domains/types.h>
#include <asm/lcd_domains/types.h>

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

/*
 * Accessor Macros for syscalls
 * ============================
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

/**
 * Dump LCD executation state to console.
 *
 * Registers, stack, stack trace.
 */
void lcd_arch_dump_lcd(struct lcd_arch *lcd);
/**
 * Dump LCD inside a wrapping vmx_get_cpu/vmx_put_cpu.
 */
void __lcd_arch_dump_lcd(struct lcd_arch *lcd);

/* Some internal data used by run */

/*
 * Declared in inline assembly in vmx_enter
 *
 * This is how we remember what instruction to return to after the
 * LCD VM exits.
 */
extern const unsigned long vmx_return;

extern DEFINE_PER_CPU(struct lcd_arch *, local_lcd_arch);

#endif /* ASM_X86_LCD_DOMAINS_RUN_H */
