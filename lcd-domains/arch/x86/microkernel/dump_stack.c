/*
 * dump_stack.c
 *
 * Dump the stack inside the LCD
 *
 * Copyright: University of Utah
 */

#include <linux/module.h>
#include <linux/kallsyms.h>

#include <lcd_domains/microkernel.h>
#include <asm/lcd_domains/main.h>
#include <liblcd/address_spaces.h>

static void lcd_show_registers(const struct cpu_user_regs *regs)

{
	printk("RIP:    %04x:[<%016llx>]", regs->cs, regs->rip);
	printk("RFLAGS: %016llx   \n", regs->rflags);
	printk("rax: %016llx   rbx: %016llx   rcx: %016llx\n",
		regs->rax, regs->rbx, regs->rcx);
	printk("rdx: %016llx   rsi: %016llx   rdi: %016llx\n",
		regs->rdx, regs->rsi, regs->rdi);
	printk("rbp: %016llx   rsp: %016llx   r8:  %016llx\n",
		regs->rbp, regs->rsp, regs->r8);
	printk("r9:  %016llx   r10: %016llx   r11: %016llx\n",
		regs->r9,  regs->r10, regs->r11);
        printk("r12: %016llx   r13: %016llx   r14: %016llx\n",
		regs->r12, regs->r13, regs->r14);
        printk("r15: %016llx   cr0: %016lx   cr4: %016lx\n",
		regs->r15, crs[0], crs[4]);
	printk("cr3: %016lx   cr2: %016lx\n", crs[3], crs[2]);
	printk("ds: %04x   es: %04x   fs: %04x   gs: %04x   "
		"ss: %04x   cs: %04x\n",
		regs->ds, regs->es, regs->fs,
		regs->gs, regs->ss, regs->cs);
}

void lcd_arch_show_execution_state(struct lcd_arch *lcd)
{
    lcd_show_registers(&lcd->regs);
    lcd_show_stack(lcd, regs);
}
