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
#include <asm/lcd_domains/microkernel.h>
#include <asm/liblcd/address_spaces.h>

/* This controls how many stack addresses we print out */
#define debug_stack_lines 20 /* 20 lines max */
#define stack_words_per_line 3 /* 3 addresses per line */

static inline gva_t stack_bottom(gva_t sp)
{
	/*
	 * All stacks (including those used in async) should be
	 * LCD_STACK_SIZE bytes and LCD_STACK_SIZE aligned.
	 *
	 * ALIGN rounds *up* to the next multiple of LCD_STACK_SIZE.
	 */
	return __gva(ALIGN(gva_val(sp), LCD_STACK_SIZE) - 
		sizeof(unsigned long));
}

static inline gva_t stack_trace_bottom(gva_t sp)
{
	gva_t sbottom = stack_bottom(sp);
	/*
	 * We don't want to look up the 0x0 ret addr
	 */
	return __gva(gva_val(sbottom) - sizeof(unsigned long));
}

static inline int stack_addr_gva2hva(struct lcd_arch *lcd, gva_t gva, 
				hva_t *hva_out)
{
	gpa_t gpa;
	/*
	 * Resolve gva ==> gpa
	 */
	gpa = isolated_lcd_gva2gpa(gva);
	/*
	 * Resolve gpa ==> hva so we can read the memory there
	 */
	return lcd_arch_ept_gpa_to_hva(lcd, gpa, hva_out);
}

static int print_one_addr(gva_t addr, struct lcd_arch *lcd)
{
	char symname[KSYM_SYMBOL_LEN];
	/*
	 * Resove address to symbol info
	 *
	 * Since we map the code in the same location inside
	 * the LCD as it is in the host, gva == hva!
	 */
	if (__lcd_sprint_symbol(symname, __hva(gva_val(addr)), 
					lcd->kernel_module))
		printk("   [<0x%016lx>] %s\n", gva_val(addr), symname);
	else
		printk("   [<0x%016lx>] ??\n", gva_val(addr));
	return 0;
}

static void maybe_print_top_stack(gva_t sp, struct lcd_arch *lcd)
{
	char symname[KSYM_SYMBOL_LEN];
	int ret;
	hva_t sp_hva;
	gva_t addr;
	/*
	 * Try to resolve sp to hva
	 */
	ret = stack_addr_gva2hva(lcd, sp, &sp_hva);
	if (ret)
		return;
	/*
	 * Read value at that spot
	 */
	addr = __gva(*(unsigned long*)hva_val(sp_hva));
	/*
	 * Resove address to symbol info
	 *
	 * Since we map the code in the same location inside
	 * the LCD as it is in the host, gva == hva!
	 */
	if (__lcd_sprint_symbol(symname, __hva(gva_val(addr)), 
					lcd->kernel_module))
		printk("   [<0x%016lx>] %s\n", gva_val(addr), symname);
	return;
}


#if !defined(CONFIG_FRAME_POINTER)

/*
 * I don't think this case ever happens on x86, but just in case ...
 *
 * Stack trace from pointers found in stack, unaided by frame pointers.  For
 * caller convenience, this has the same prototype as its alternative, and
 * simply ignores the base pointer parameter.
 */

static int do_one_trace(gva_t ptr, struct lcd_arch *lcd)
{
	hva_t ptr_hva;
	gva_t addr;
	int ret;
	/*
	 * Resolve current stack pointer to hva
	 */
	ret = stack_addr_gva2hva(lcd, ptr, &ptr_hva);
	if (ret) { 
		LCD_ERR("Failed to resolve guest gva 0x%lx into hva\n",
			gva_val(ptr));
		return ret;
	}
	/*
	 * Resolve address on stack to hva
	 */
	addr = __gva(*(unsigned long *)hva_val(ptr_hva));
	ret = print_one_addr(addr, lcd);
	if (ret) {
		LCD_ERR("Failed to resolve guest addr 0x%lx into hva\n",
			gva_val(addr));
		return ret;
	}
}

static void _show_trace(struct lcd_arch *lcd, gva_t sp,
			gva_t __maybe_unused bp)
{
	gva_t stack_bottom_gva = stack_trace_bottom(sp);

	printk("Warning: no frame pointers, stack might be innacurate\n");

	while (gva_val(sp) <= gva_val(stack_bottom_gva)) {
		/*
		 * Resolve address on stack to symbol info
		 */
		if (do_one_trace(sp, lcd))
			return;
		sp = gva_add(sp, sizeof(unsigned long));
	}
}

#else

/* Stack trace from frames in the stack, using frame pointers */
static void _show_trace(struct lcd_arch *lcd, gva_t sp,
			gva_t __maybe_unused bp)
{
	gva_t frame_gva;
	hva_t frame_hva;
	gva_t ret_addr;
	int ret; 

	/* Bounds for range of valid frame pointer. */
	gva_t low = sp, high = stack_trace_bottom(sp);

	/* The initial frame pointer. */
	frame_gva = bp;

	for (;;) {
		/*
		 * Valid frame pointer?
		 */
		if ((gva_val(frame_gva) < gva_val(low)) || 
			(gva_val(frame_gva) >= gva_val(high)))
			break; /* no */
		/*
		 * Resolve frame gva ==> hva
		 */
		ret = stack_addr_gva2hva(lcd, frame_gva, &frame_hva);
		if (ret) {
			LCD_ERR("Error translating gva 0x%lx to hva\n",
				gva_val(frame_gva));
			return;
		}
		/*
		 * Frame points to memory that contains frame addr for
		 * prior frame, and the return address. Print out
		 * the symbol that resolves from the return address.
		 */
		frame_gva = __gva(*(unsigned long *)hva_val(frame_hva));
		ret_addr = __gva(*(unsigned long *)
				(hva_val(frame_hva) + sizeof(unsigned long)));
		ret = print_one_addr(ret_addr, lcd);
		if (ret) {
			LCD_ERR("Failed to resolve guest addr 0x%lx into hva\n",
				gva_val(ret_addr));
			return;
		}
	}
}
#endif

static void show_trace(struct lcd_arch *lcd)
{
	printk("LCD call trace:\n");
	/*
	 * Try to resolve the current %rip to a symbol (the current
	 * function we crashed in)
	 */
	print_one_addr(__gva((unsigned long)lcd->regs.rip), lcd);
	/*
	 * In case we faulted on a function call, we won't have
	 * a stack frame set up, but the return address will be
	 * sitting there. Print it if so.
	 */
	maybe_print_top_stack(__gva((unsigned long)lcd->regs.rsp), lcd);
	/*
	 * Try to resolve addresses on stack to function symbols
	 */
	_show_trace(lcd, __gva((unsigned long)lcd->regs.rsp), 
		__gva((unsigned long)lcd->regs.rbp));

	printk("\n");
}

static void lcd_show_stack(struct lcd_arch *lcd)
{
	gva_t stack_gva = __gva((unsigned long)lcd->regs.rsp);
	hva_t stack_hva;
	gva_t stack_bottom_gva = stack_bottom(stack_gva);
	unsigned long data;
    
	int i, ret;

	printk("LCD stack from rsp=0x%lx:\n  ", gva_val(stack_gva));

	for (i = 0; i < (debug_stack_lines * stack_words_per_line) &&
		     (gva_val(stack_gva) <= gva_val(stack_bottom_gva)); i++ ) {

		if ((i != 0) && ((i % stack_words_per_line) == 0))
			printk("\n  ");
		/*
		 * gva ==> hva
		 */
		ret = stack_addr_gva2hva(lcd, stack_gva, &stack_hva);
		if (ret) { 
			LCD_ERR("Failed to resolve guest stack gva 0x%lx into hpa\n",
				gva_val(stack_gva));
			return;
		}
		/*
		 * Print the memory contents there
		 */
		data = *(unsigned long *)hva_val(stack_hva);
		printk(" %016lx", data);
		/*
		 * Go to next word-size slot on stack
		 */
		stack_gva = gva_add(stack_gva, sizeof(unsigned long));
	}

	if (i == 0)
		printk("Stack empty.");

	printk("\n\n");
}


static void show_registers(const struct lcd_arch_regs *regs)

{
	printk("RIP:    %04x:[<%016llx>]\n", regs->cs, regs->rip);
	printk("RFLAGS: %016llx   \n", regs->rflags);
	printk("rax: %016llx   rbx: %016llx\n",
		regs->rax, regs->rbx);
	printk("rcx: %016llx   rdx: %016llx\n",
		regs->rcx, regs->rdx);
	printk("rsi: %016llx   rdi: %016llx\n",
		regs->rsi, regs->rdi);
	printk("rbp: %016llx   rsp: %016llx\n",
		regs->rbp, regs->rsp);
	printk("r8:  %016llx   r9:  %016llx\n",
		regs->r8, regs->r9);
	printk("r10: %016llx   r11: %016llx\n",
		regs->r10, regs->r11);
        printk("r12: %016llx   r13: %016llx\n",
		regs->r12, regs->r13);
        printk("r14: %016llx   r15: %016llx\n",
		regs->r14, regs->r15);
        printk("cr0: %016llx   cr4: %016llx\n",
		regs->cr0, regs->cr4);
	printk("cr3: %016llx   cr2: %016llx\n", 
		regs->cr3, regs->cr2);
	printk("ds: %04x   es: %04x   fs: %04x\n",
		regs->ds, regs->es, regs->fs);
	printk("gs: %04x   ss: %04x   cs: %04x\n",
		regs->gs, regs->ss, regs->cs);
	printk("tr: %04x\n", regs->tr);
	printk("tr base:   %016llx   tr limit:   %08x\n",
		regs->tr_base, regs->tr_limit);
	printk("gdtr base: %016llx   gdtr limit: %08x\n",
		regs->gdtr_base, regs->gdtr_limit);
	printk("idtr base: %016llx   idtr limit: %08x\n",
		regs->idtr_base, regs->idtr_limit);
	printk("\n");
}

/**
 * Read some regs that aren't saved after every exit.
 */
static void update_regs(struct lcd_arch *lcd)
{
	lcd->regs.rip = vmcs_readl(GUEST_RIP);
	lcd->regs.rsp = vmcs_readl(GUEST_RSP);
	lcd->regs.rflags = vmcs_readl(GUEST_RFLAGS);

	lcd->regs.cr0 = vmcs_readl(GUEST_CR0);
	lcd->regs.cr3 = vmcs_readl(GUEST_CR3);
	lcd->regs.cr4 = vmcs_readl(GUEST_CR4);
	lcd->regs.cs  =	vmcs_read16(GUEST_CS_SELECTOR);
	lcd->regs.ds  =	vmcs_read16(GUEST_DS_SELECTOR);
	lcd->regs.es  =	vmcs_read16(GUEST_ES_SELECTOR);
	lcd->regs.fs  =	vmcs_read16(GUEST_FS_SELECTOR);
	lcd->regs.gs  =	vmcs_read16(GUEST_GS_SELECTOR);
	lcd->regs.ss  =	vmcs_read16(GUEST_SS_SELECTOR);
	lcd->regs.tr  =	vmcs_read16(GUEST_TR_SELECTOR);
	lcd->regs.tr_base = vmcs_readl(GUEST_TR_BASE);
	lcd->regs.tr_limit = vmcs_read32(GUEST_TR_LIMIT);
	lcd->regs.gdtr_base = vmcs_readl(GUEST_GDTR_BASE);
	lcd->regs.gdtr_limit = vmcs_read32(GUEST_GDTR_LIMIT);
	lcd->regs.idtr_base = vmcs_readl(GUEST_IDTR_BASE);
	lcd->regs.idtr_limit = vmcs_read32(GUEST_IDTR_LIMIT);
}

void __lcd_arch_dump_lcd(struct lcd_arch *lcd)
{
	printk(KERN_ERR "---- Begin LCD Arch Dump ----\n");
	printk(KERN_ERR "CPU %d VPID %d\n", lcd->cpu, lcd->vpid);

	update_regs(lcd);
	show_registers(&lcd->regs);
	lcd_show_stack(lcd);
	show_trace(lcd);

	printk(KERN_ERR "---- End LCD Arch Dump ----\n");
}

void lcd_arch_dump_lcd(struct lcd_arch *lcd)
{
	vmx_get_cpu(lcd);
	__lcd_arch_dump_lcd(lcd);
	vmx_put_cpu(lcd);
}
