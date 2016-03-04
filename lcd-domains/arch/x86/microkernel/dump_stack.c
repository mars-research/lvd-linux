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

/* This controls how many stack addresses we print out */
#define debug_stack_lines 20 /* 20 lines max */
#define stack_words_per_line 4 /* 4 addresses per line */

static inline gva_t stack_bottom(gva_t sp)
{
	/*
	 * All stacks (including those used in async) should be
	 * LCD_STACK_SIZE bytes and LCD_STACK_SIZE aligned.
	 *
	 * ALIGN rounds *up* to the next multiple of LCD_STACK_SIZE.
	 */
	return __gva(ALIGN(gva_val(sp), LCD_STACK_SIZE));
}

static inline int stack_addr_gva2hva(gva_t gva, hva_t *hva_out)
{
	gpa_t gpa;
	/*
	 * Resolve gva ==> gpa
	 */
	gpa = isolated_lcd_gva2gpa(gva);
	/*
	 * Resolve gpa ==> hva so we can read the memory there
	 */
	return lcd_arch_ept_gpa_to_hva(lcd, gpa, &hva_out);
}

static int print_one_addr(gva_t addr, struct lcd_arch *lcd)
{
	int ret;
	hva_t addr_hva;
	char symname[KSYM_SYMBOL_LEN];
	/*
	 * Resolve address on stack to hva
	 */
	ret = stack_addr_gva2hva(addr, &addr_hva);
	if (ret)
		return ret;
	/*
	 * Resove address to symbol info
	 */
	if (__lcd_sprint_symbol(symname, addr_hva, lcd->kernel_module))
		printk("[<0x%016x>] %s", symname);
	return 0;
}

static int do_one_trace(gva_t ptr, struct lcd_arch *lcd)
{
	hva_t ptr_hva;
	gva_t addr;
	int ret;
	/*
	 * Resolve current stack pointer to hva
	 */
	ret = stack_addr_gva2hva(sp, &stack_hva);
	if (ret) { 
		LCD_ERR("Failed to resolve guest gva 0x%lx into hva\n",
			gva_val(ptr));
		return ret;
	}
	/*
	 * Resolve address on stack to hva
	 */
	addr = __gva(*(unsigned long *)hva_val(stack_hva));
	ret = print_one_addr(addr, lcd);
	if (ret) {
		LCD_ERR("Failed to resolve guest addr 0x%lx into hva\n",
			gva_val(addr));
		return ret;
	}
}

#if !defined(CONFIG_FRAME_POINTER)
/*
 * I don't think this case ever happens on x86, but just in case ...
 *
 * Stack trace from pointers found in stack, unaided by frame pointers.  For
 * caller convenience, this has the same prototype as its alternative, and
 * simply ignores the base pointer parameter.
 */
static void _show_trace(struct lcd_arch *lcd, gva_t sp,
			gva_t __maybe_unused bp)
{
	hva_t stack_hva;
	gva_t stack_bottom_gva = stack_bottom(sp);
	gva_t addr;
	hva_t addr_hva;

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
	gva_t low = sp, high = stack_bottom(sp);

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
		ret = stack_addr_gva2hva(frame_gva, &frame_hva);
		if (ret) {
			LCD_ERR("Error translating gva 0x%lx to hva\n",
				gval_val(frame_gva));
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
			return ret;
		}
	}
}
#endif

static void show_trace(struct lcd_arch *lcd)
{
	int ret;

	printk("LCD call trace:\n");
	/*
	 * Try to resolve the current %rip to a symbol (the current
	 * function we crashed in)
	 */
	print_one_addr(__gva((unsigned long)lcd->regs->rip), lcd);
	/*
	 * Try to resolve addresses on stack to function symbols
	 */
	_show_trace(lcd, __gva((unsigned long)lcd->regs->rsp), 
		__gva((unsigned long)lcd->regs->rbp));

	printk("\n");
}

static void show_stack(struct lcd_arch *lcd)
{
	gva_t stack_gva = __gva((unsigned long)lcd->regs->rsp);
	gpa_t stack_gpa;
	hva_t stack_hva;
	gva_t stack_bottom_gva = stack_bottom(stack_gva);
	unsigned long data;
    
	int i, ret;

	printk("LCD stack trace from rsp=0x%lx:\n  ", gva_val(stack));

	for (i = 0; i < (debug_stack_lines * stack_words_per_line) &&
		     (gva_val(stack_gva) <= gva_val(stack_bottom_gva)); i++ ) {

		if ((i != 0) && ((i % stack_words_per_line) == 0))
			printk("\n  ");
		/*
		 * gva ==> hva
		 */
		ret = stack_addr_gva2hva(stack_gva, &stack_hva);
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
		stack_gva = hva_add(stack_gva, sizeof(unsigned long));
	}

	if (i == 0)
		printk("Stack empty.");

	printk("\n");
}


static void show_registers(const struct lcd_arch_regs *regs)

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
		regs->r15, regs->cr0, regs->cr4);
	printk("cr3: %016lx   cr2: %016lx\n", regs->cr3, regs->cr2);
	printk("ds: %04x   es: %04x   fs: %04x   gs: %04x   "
		"ss: %04x   cs: %04x\n",
		regs->ds, regs->es, regs->fs,
		regs->gs, regs->ss, regs->cs);
	printk("tr: %04x  tr base: %016lx   tr limit: %08x\n",
		regs->tr, regs->tr_base, regs->tr_limit);
	printk("gdtr base: %016lx   gdtr limit: %08x\n",
		regs->gdtr_base, regs->gdtr_limit);
	printk("idtr base: %016lx   idtr limit: %08x\n",
		regs->idtr_base, regs->idtr_limit);
}

/**
 * Read some regs that aren't saved after every exit.
 */
static void update_regs(struct lcd_arch *lcd)
{
	lcd->regs.rip = vmcs_readl(GUEST_RIP);
	lcd->regs.rsp = vmcs_readl(GUEST_RSP);
	lcd->regs.flags = vmcs_readl(GUEST_RFLAGS);

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
	show_stack(lcd);
	show_trace(lcd);

	printk(KERN_ERR "---- End LCD Arch Dump ----\n");
}

void lcd_arch_dump_lcd(struct lcd_arch *lcd)
{
	vmx_get_cpu(lcd);
	__lcd_arch_dump_lcd(lcd);
	vmx_put_cpu(lcd);
}
