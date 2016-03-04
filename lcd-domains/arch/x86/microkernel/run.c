/*
 * run.c
 *
 * Code for running and handling VM exits for LCDs
 *
 * Copyright: University of Utah
 */

#include <lcd_domains/types.h>
#include <asm/lcd_domains/types.h>
#include <asm/lcd_domains/microkernel.h>

DEFINE_PER_CPU(struct lcd_arch *, local_lcd_arch);

static void dump_lcd_arch(struct lcd_arch *lcd)
{
	unsigned long flags;

	vmx_get_cpu(lcd);
	lcd->regs.rip = vmcs_readl(GUEST_RIP);
	lcd->regs.rsp = vmcs_readl(GUEST_RSP);
	flags = vmcs_readl(GUEST_RFLAGS);
	vmx_put_cpu(lcd);

	printk(KERN_ERR "---- Begin LCD Arch Dump ----\n");
	printk(KERN_ERR "CPU %d VPID %d\n", lcd->cpu, lcd->vpid);

	/*
	printk(KERN_ERR "RIP 0x%016llx RFLAGS 0x%08lx\n",
	       lcd->regs[LCD_ARCH_REGS_RIP], flags);
	printk(KERN_ERR "RAX 0x%016llx RCX 0x%016llx\n",
			lcd->regs[LCD_ARCH_REGS_RAX], 
		lcd->regs[LCD_ARCH_REGS_RCX]);
	printk(KERN_ERR "RDX 0x%016llx RBX 0x%016llx\n",
			lcd->regs[LCD_ARCH_REGS_RDX], 
		lcd->regs[LCD_ARCH_REGS_RBX]);
	printk(KERN_ERR "RSP 0x%016llx RBP 0x%016llx\n",
			lcd->regs[LCD_ARCH_REGS_RSP], 
		lcd->regs[LCD_ARCH_REGS_RBP]);
	printk(KERN_ERR "RSI 0x%016llx RDI 0x%016llx\n",
			lcd->regs[LCD_ARCH_REGS_RSI], 
		lcd->regs[LCD_ARCH_REGS_RDI]);
	printk(KERN_ERR "R8  0x%016llx R9  0x%016llx\n",
			lcd->regs[LCD_ARCH_REGS_R8], 
		lcd->regs[LCD_ARCH_REGS_R9]);
	printk(KERN_ERR "R10 0x%016llx R11 0x%016llx\n",
			lcd->regs[LCD_ARCH_REGS_R10], 
		lcd->regs[LCD_ARCH_REGS_R11]);
	printk(KERN_ERR "R12 0x%016llx R13 0x%016llx\n",
			lcd->regs[LCD_ARCH_REGS_R12], 
		lcd->regs[LCD_ARCH_REGS_R13]);
	printk(KERN_ERR "R14 0x%016llx R15 0x%016llx\n",
			lcd->regs[LCD_ARCH_REGS_R14], 
		lcd->regs[LCD_ARCH_REGS_R15]);
	*/

//	lcd_show_execution_state(lcd, &lcd->regs);
	/* printk(KERN_ERR "Dumping Stack Contents...\n"); */
	/* sp = (unsigned long *) vcpu->regs[VCPU_REGS_RSP]; */
	/* for (i = 0; i < STACK_DEPTH; i++) */
	/* 	if (get_user(val, &sp[i])) */
	/* 		printk(KERN_INFO "vmx: RSP%+-3ld ?\n", */
	/* 			i * sizeof(long)); */
	/* 	else */
	/* 		printk(KERN_INFO "vmx: RSP%+-3ld 0x%016lx\n", */
	/* 			i * sizeof(long), val); */

	printk(KERN_ERR "---- End LCD Arch Dump ----\n");
}

static inline int vmx_exit_intr(struct lcd_arch *lcd_arch)
{
	return (lcd_arch->exit_reason == EXIT_REASON_EXTERNAL_INTERRUPT) ||
		(lcd_arch->exit_reason == EXIT_REASON_EXCEPTION_NMI);
}

static inline int vmx_exit_intr_info_valid(struct lcd_arch *lcd_arch)
{
	return lcd_arch->exit_intr_info & INTR_INFO_VALID_MASK;
}

static inline unsigned int vmx_exit_intr_type(struct lcd_arch *lcd_arch)
{
	return lcd_arch->exit_intr_info & INTR_INFO_INTR_TYPE_MASK;
}

static inline unsigned int vmx_exit_intr_vector(struct lcd_arch *lcd_arch)
{
	return lcd_arch->exit_intr_info & INTR_INFO_VECTOR_MASK;
}

/**
 * Skips to next instruction in lcd. This should only be
 * called when a vm exit occurs that sets the exit
 * instruction length. See Intel SDM V3 27.2.4.
 */
static void vmx_step_instruction(struct lcd_arch *lcd_arch)
{
	vmcs_writel(GUEST_RIP, vmcs_readl(GUEST_RIP) +
		vmcs_read32(VM_EXIT_INSTRUCTION_LEN));
}

/**
 * Processes a vmcall. For now, the only reason the lcd_arch
 * would execute a vmcall is for a syscall to the hypervisor.
 */
static int vmx_handle_vmcall(struct lcd_arch *lcd_arch)
{
	/*
	 * Advance the pc
	 */
	vmx_step_instruction(lcd_arch);
	return LCD_ARCH_STATUS_SYSCALL;
}

/**
 * Simple processing of ept violation (e.g., violation due to
 * an access to unmapped memory).
 */
static int vmx_handle_ept(struct lcd_arch *lcd_arch)
{
	/*
	 * Intel SDM V3 27.2.1 - 27.2.2
	 */
	LCD_ERR("\n");
	printk("   ept violation: \n");
	printk("        guest physical addr: 0x%lx\n",
		vmcs_readl(GUEST_PHYSICAL_ADDRESS));
	if (lcd_arch->exit_qualification & (1UL << 7))
		printk("        linear addr: 0x%lx\n",
			vmcs_readl(GUEST_LINEAR_ADDRESS));
	else
		printk("        linear address not available");
	printk("        faulting %%rip: 0x%lx\n",
		vmcs_readl(GUEST_RIP));
	if (lcd_arch->exit_qualification & (1UL << 0))
		printk("        attempted read\n");
	if (lcd_arch->exit_qualification & (1UL << 1))
		printk("        attempted write\n");
	if (lcd_arch->exit_qualification & (1UL << 2))
		printk("        attempted instruction fetch\n");
	if (lcd_arch->exit_qualification & (1UL << 3))
		printk("        ept says this page is readable\n");
	if (lcd_arch->exit_qualification & (1UL << 4))
		printk("        ept says this page is writeable\n");
	if (lcd_arch->exit_qualification & (1UL << 5))
		printk("        ept says this page is executable\n");
	if (lcd_arch->exit_qualification & (1UL << 8))
		printk("        guest virtual page walk succeeded, but the ept translation failed\n\n");
	else
		printk("        violation occurred during guest virtual page walk (before reaching the final guest physical address)\n\n");

	dump_lcd_arch(lcd_arch);

	return LCD_ARCH_STATUS_EPT_FAULT;
}

/**
 * Checking and emulation of control register accessing.
 * All accesses to %cr0, %cr4, and %cr8 lead to vm exits, and are
 * not currently allowed. Loads / stores to %cr3 lead to
 * vm exits (required for emulab machines) and are emulated.
 */
static int vmx_handle_control_reg(struct lcd_arch *lcd_arch)
{
	int control_reg;
	int access_type;
	int general_reg;
	/*
	 * Intel SDM V3 27.2.1
	 */
	
	/*
	 * Determine the control and general purpose registers involved, 
	 * and the access type.
	 */
	control_reg = lcd_arch->exit_qualification & CONTROL_REG_ACCESS_NUM;
	access_type = lcd_arch->exit_qualification & CONTROL_REG_ACCESS_TYPE;
	general_reg = lcd_arch->exit_qualification & CONTROL_REG_ACCESS_REG;

	switch (control_reg) {
	case 3:
		/*
		 * %cr3
		 */
		if (access_type == 0) {
			/*
			 * Move to
			 */
			switch (general_reg){
				case 0: vmcs_writel(GUEST_CR3, lcd_arch->regs.rax); break;
				case 1: vmcs_writel(GUEST_CR3, lcd_arch->regs.rcx); break;
				case 2: vmcs_writel(GUEST_CR3, lcd_arch->regs.rdx); break;
				case 3: vmcs_writel(GUEST_CR3, lcd_arch->regs.rbx); break;
				case 4: vmcs_writel(GUEST_CR3, lcd_arch->regs.rsp); break;
				case 5: vmcs_writel(GUEST_CR3, lcd_arch->regs.rbp); break;
				case 6: vmcs_writel(GUEST_CR3, lcd_arch->regs.rsi); break;
				case 7: vmcs_writel(GUEST_CR3, lcd_arch->regs.rdi); break;
				case 8: vmcs_writel(GUEST_CR3, lcd_arch->regs.r8); break;
				case 9: vmcs_writel(GUEST_CR3, lcd_arch->regs.r9); break;
				case 10: vmcs_writel(GUEST_CR3, lcd_arch->regs.r10); break;
				case 11: vmcs_writel(GUEST_CR3, lcd_arch->regs.r11); break;
				case 12: vmcs_writel(GUEST_CR3, lcd_arch->regs.r12); break;
				case 13: vmcs_writel(GUEST_CR3, lcd_arch->regs.r13); break;
				case 14: vmcs_writel(GUEST_CR3, lcd_arch->regs.r14); break;
				case 15: vmcs_writel(GUEST_CR3, lcd_arch->regs.r15); break;
				default: LCD_ERR("Unknown general register in CR3 access:%d", general_reg);
			}
		} else {
			/*
			 * Move from
			 */
			switch (general_reg){
				case 0: lcd_arch->regs.rax = vmcs_readl(GUEST_CR3); break;
				case 1: lcd_arch->regs.rcx = vmcs_readl(GUEST_CR3); break;
				case 2: lcd_arch->regs.rdx = vmcs_readl(GUEST_CR3); break;
				case 3: lcd_arch->regs.rbx = vmcs_readl(GUEST_CR3); break;
				case 4: lcd_arch->regs.rsp = vmcs_readl(GUEST_CR3); break;
				case 5: lcd_arch->regs.rbp = vmcs_readl(GUEST_CR3); break;
				case 6: lcd_arch->regs.rsi = vmcs_readl(GUEST_CR3); break;
				case 7: lcd_arch->regs.rdi = vmcs_readl(GUEST_CR3); break;
				case 8: lcd_arch->regs.r8 = vmcs_readl(GUEST_CR3); break;
				case 9: lcd_arch->regs.r9 = vmcs_readl(GUEST_CR3); break;
				case 10: lcd_arch->regs.r10 = vmcs_readl(GUEST_CR3); break;
				case 11: lcd_arch->regs.r11 = vmcs_readl(GUEST_CR3); break;
				case 12: lcd_arch->regs.r12 = vmcs_readl(GUEST_CR3); break;
				case 13: lcd_arch->regs.r13 = vmcs_readl(GUEST_CR3); break;
				case 14: lcd_arch->regs.r14 = vmcs_readl(GUEST_CR3); break;
				case 15: lcd_arch->regs.r15 = vmcs_readl(GUEST_CR3); break;

				default: LCD_ERR("Unknown general register in CR3 access:%d", general_reg);
			}

		}
		/*
		 * Step past instruction that caused exit
		 */
		vmx_step_instruction(lcd_arch);
		return LCD_ARCH_STATUS_CR3_ACCESS;
	default:
		LCD_ERR("attempted access to protected cr");
		return -EIO;
	}
}

static int vmx_handle_ext_intr(struct lcd_arch *lcd_arch)
{
	/*
	 * We don't try to emulate hardware interrupt handling anymore. I
	 * don't think the code in KVM is actually used anymore, because they
	 * don't set the VM_EXIT_ACK_INTR_ON_EXIT vm exit control for
	 * non-nested vm's. (So the interrupt emulation code is always skipped
	 * and just re-enables interrupts.)
	 *
	 * This is what Dune does too - they don't try to emulate.
	 *
	 * I made this choice when I realized our emulation code (and KVM's
	 * code) is probably not switching to the per-cpu interrupt stack;
	 * it's calling the interrupt handler with the same stack = bad idea
	 * on x86_64! Could lead to stack overflows, or using the wrong
	 * stack for certain exceptions/interrupts.
	 * 
	 * See:
	 *
	 * https://www.kernel.org/doc/Documentation/x86/x86_64/kernel-stacks
	 *
	 * In order to correctly emulate, we would need to look up the correct
	 * IST entry in the cpu's tss, and switch to the correct stack. But
	 * I have a feeling that wouldn't be reliable ...
	 *
	 * So ... this code will probably never be called for now - because I
	 * turned off ack interrupts on vm exit, and hence
	 * the interrupt info will be invalid in the case of an external
	 * interrupt, and we check that before calling into here.
	 */
	return LCD_ARCH_STATUS_EXT_INTR;
}

static int vmx_handle_hard_exception(struct lcd_arch *lcd_arch)
{
	unsigned int vector;
	/*
	 * Page fault, invalid opcode, gp exception, ...
	 *
	 * We error out on all except page faults.
	 */
	vector = vmx_exit_intr_vector(lcd_arch);
	switch (vector) {
	case 14: // Page fault
		/*
		 * Guest virtual page fault
		 *
		 * TODO: We will have microkernel handle it.
		 */
		LCD_ERR("page fault: faulting gv address = 0x%lx, faulting instruction address = 0x%lx",
			lcd_arch->exit_qualification,
			vmcs_readl(GUEST_RIP));
		return LCD_ARCH_STATUS_PAGE_FAULT;
	case 18: // Machine Check
		/*
		 * This could be a serious problem, so we single it out from
		 * the others.
		 */
		LCD_ERR("got a machine check inside vm!");
		return -EIO;
	default:
		LCD_ERR("unhandled exception: vector = %x, info = %x, instruction addr = 0x%lx",
			vector, lcd_arch->exit_intr_info,
			vmcs_readl(GUEST_RIP));
		return -EIO;
	}
}

static int vmx_handle_soft_exception(struct lcd_arch *lcd_arch)
{
	/*
	 * Software exception
	 *
	 * div by zero, overflow, ...
	 *
	 * For now, we error out.
	 */
	LCD_ERR("unhandled exception, vector: %d\n",
		vmx_exit_intr_vector(lcd_arch));
	return -EIO;
}

static int vmx_handle_nmi(struct lcd_arch *lcd_arch)
{
	/*
	 * This is how KVM does it. We just fire another nmi manually. This
	 * is allowed by the architecture, but it won't trigger some of the
	 * nmi hardware (see Intel SDM V3 6.3.3).
	 *
	 * Note that getting nmi's is not necessarily bad. They are set up
	 * by the nmi watchdog, and will inevitably fire sometimes while we are
	 * inside the vm. Improper handling of them can lead to nasty
	 * lockups, however ...
	 */
	asm("int $2;");
	return LCD_ARCH_STATUS_EXT_INTR;
}

static int vmx_handle_exception_interrupt(struct lcd_arch *lcd_arch)
{
	int ret;
	int type;
	/*
	 * Check if we exited due to an exception/interrupt, and if the
	 * interrupt info is valid. The info will be invalid if we turned
	 * off the VM_EXIT_ACK_INTR_ON_EXIT vm exit control, for example.
	 */
	if (!vmx_exit_intr(lcd_arch))
		return 0;
	if (!vmx_exit_intr_info_valid(lcd_arch))
		return LCD_ARCH_STATUS_EXT_INTR;

	type = vmx_exit_intr_type(lcd_arch);
	switch (type) {
	case INTR_TYPE_NMI_INTR:
		ret = vmx_handle_nmi(lcd_arch);
		break;
	case INTR_TYPE_SOFT_EXCEPTION:
		ret = vmx_handle_soft_exception(lcd_arch);
		break;
	case INTR_TYPE_HARD_EXCEPTION:
		ret = vmx_handle_hard_exception(lcd_arch);
		break;
	case INTR_TYPE_EXT_INTR:
		ret = vmx_handle_ext_intr(lcd_arch);
		break;
	default:
		LCD_ERR("unhandled interrupt type %d", type);
		ret = -EIO;
		break;
	}
	return ret;
}

static int vmx_handle_other_exits(struct lcd_arch *lcd_arch)
{
	int exit_reason;
	int ret;

	exit_reason = lcd_arch->exit_reason;

	switch (exit_reason) {
	case EXIT_REASON_VMCALL:
		ret = vmx_handle_vmcall(lcd_arch);
		break;
	case EXIT_REASON_EPT_MISCONFIG:
	case EXIT_REASON_EPT_VIOLATION:
		ret = vmx_handle_ept(lcd_arch);
		break;
	case EXIT_REASON_CR_ACCESS:
		ret = vmx_handle_control_reg(lcd_arch);
		break;
	default:
		LCD_ERR("unhandled exit reason: %x\n",
			exit_reason);
		ret = -EIO;
		break;
	}
	return ret;
}

/* VMX RUN -------------------------------------------------- */

/**
 * Low-level vmx launch / resume to enter non-root mode on cpu with
 * the current vmcs.
 */
static void __noclone vmx_enter(struct lcd_arch *lcd_arch)
{
	asm(
		/* 
		 * Store host registers on stack (all other regs are
		 * clobbered)
		 */
		"push %%rdx \n\t"
		"push %%rbp \n\t"
		"push %%rcx \n\t" /* placeholder for guest %rcx (see below) */
		"push %%rcx \n\t" /* copy of lcd_arch vcpu pointer */
		/*
		 * Remember the current %rsp *and* store it
		 * in the vmcs.
		 * (je 1f jumps forward to numeric label 1)
		 */
		"cmp %%rsp, %c[host_rsp](%0) \n\t"
		"je 1f \n\t"
		"mov %%rsp, %c[host_rsp](%0) \n\t"
		"mov $%c[host_rsp_field], %%rdx \n\t"
		ASM_VMX_VMWRITE_RSP_RDX "\n\t"
		"1: \n\t"
		/*
		 * Re-load %cr2 (page fault addr) if changed
		 */
		"mov %c[cr2](%0), %%rax \n\t"
		"mov %%cr2, %%rdx \n\t"
		"cmp %%rax, %%rdx \n\t"
		"je 2f \n\t"
		"mov %%rax, %%cr2 \n\t"
		"2: \n\t"
		/*
		 * Check if vmlaunch or vmresume is needed
		 */
		"cmpl $0, %c[launched](%0) \n\t"
		/*
		 * Load guest registers.  Don't clobber flags.
		 */
		"mov %c[rax](%0), %%rax \n\t"
		"mov %c[rbx](%0), %%rbx \n\t"
		"mov %c[rdx](%0), %%rdx \n\t"
		"mov %c[rsi](%0), %%rsi \n\t"
		"mov %c[rdi](%0), %%rdi \n\t"
		"mov %c[rbp](%0), %%rbp \n\t"
		"mov %c[r8](%0),  %%r8  \n\t"
		"mov %c[r9](%0),  %%r9  \n\t"
		"mov %c[r10](%0), %%r10 \n\t"
		"mov %c[r11](%0), %%r11 \n\t"
		"mov %c[r12](%0), %%r12 \n\t"
		"mov %c[r13](%0), %%r13 \n\t"
		"mov %c[r14](%0), %%r14 \n\t"
		"mov %c[r15](%0), %%r15 \n\t"
		"mov %c[rcx](%0), %%rcx \n\t" /* kills %0 (host %rcx) */
		/*
		 * Enter guest mode
		 */
		"jne 3f \n\t"
		ASM_VMX_VMLAUNCH "\n\t"
		"jmp vmx_return_lbl \n\t"
		"3: " ASM_VMX_VMRESUME "\n\t"
		"vmx_return_lbl: "

		/*
		 * Save guest registers, load host registers, keep flags
		 */

		/*
		 * Store guest %rcx at rsp + wordsize (placeholder)
		 */
		"mov %0, %c[wordsize](%%rsp) \n\t"
		/*
		 * Pop lcd_arch vcpu pointer into %rcx (%0) 
		 * (host %rcx is at bottom of stack)
		 */
		"pop %0 \n\t"
		"mov %%rax, %c[rax](%0) \n\t"
		"mov %%rbx, %c[rbx](%0) \n\t"
		
		/*
		 * Pop guest %rcx that was stored above
		 */
		"popq %c[rcx](%0) \n\t"

		/*
		 * Store remaining regs
		 */
		"mov %%rdx, %c[rdx](%0) \n\t"
		"mov %%rsi, %c[rsi](%0) \n\t"
		"mov %%rdi, %c[rdi](%0) \n\t"
		"mov %%rbp, %c[rbp](%0) \n\t"
		"mov %%r8,  %c[r8](%0) \n\t"
		"mov %%r9,  %c[r9](%0) \n\t"
		"mov %%r10, %c[r10](%0) \n\t"
		"mov %%r11, %c[r11](%0) \n\t"
		"mov %%r12, %c[r12](%0) \n\t"
		"mov %%r13, %c[r13](%0) \n\t"
		"mov %%r14, %c[r14](%0) \n\t"
		"mov %%r15, %c[r15](%0) \n\t"
		"mov %%rax, %%r10 \n\t"
		"mov %%rdx, %%r11 \n\t"

		"mov %%cr2, %%rax   \n\t"
		"mov %%rax, %c[cr2](%0) \n\t"

		"pop  %%rbp \n\t"
		"pop  %%rdx \n\t"
		
		/*
		 * Read flags
		 */
		"setbe %c[fail](%0) \n\t"

		/*
		 * Return %rip info (needed to store in HOST_RIP
		 * field in vmcs)
		 */
		".pushsection .rodata \n\t"
		".global vmx_return \n\t"
		"vmx_return: " _ASM_PTR " vmx_return_lbl \n\t"
		".popsection"
		: : "c"(lcd_arch),
		  [host_rsp_field]"i"(HOST_RSP),
		  [launched]"i"(offsetof(struct lcd_arch, launched)),
		  [fail]"i"(offsetof(struct lcd_arch, fail)),
		  [host_rsp]"i"(offsetof(struct lcd_arch, host_rsp)),
		  [rax]"i"(offsetof(struct lcd_arch, 
					  regs.rax)),
		  [rbx]"i"(offsetof(struct lcd_arch, 
					  regs.rbx)),
		  [rcx]"i"(offsetof(struct lcd_arch, 
					  regs.rcx)),
		  [rdx]"i"(offsetof(struct lcd_arch, 
					  regs.rdx)),
		  [rsi]"i"(offsetof(struct lcd_arch, 
					  regs.rsi)),
		  [rdi]"i"(offsetof(struct lcd_arch, 
					  regs.rdi)),
		  [rbp]"i"(offsetof(struct lcd_arch, 
					  regs.rbp)),
		  [r8]"i"(offsetof(struct lcd_arch, 
					  regs.r8)),
		  [r9]"i"(offsetof(struct lcd_arch, 
					  regs.r9)),
		  [r10]"i"(offsetof(struct lcd_arch, 
					  regs.r10)),
		  [r11]"i"(offsetof(struct lcd_arch, 
					  regs.r11)),
		  [r12]"i"(offsetof(struct lcd_arch, 
					  regs.r12)),
		  [r13]"i"(offsetof(struct lcd_arch, 
					  regs.r13)),
		  [r14]"i"(offsetof(struct lcd_arch, 
					  regs.r14)),
		  [r15]"i"(offsetof(struct lcd_arch, 
					  regs.r15)),
		  [cr2]"i"(offsetof(struct lcd_arch, regs.cr2)),
		  [wordsize]"i"(sizeof(ulong))
		: "cc", "memory"
		  , "rax", "rdx", "rbx", "rdi", "rsi"
		  , "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
		);

	lcd_arch->launched = 1;

	lcd_arch->regs.rip = vmcs_readl(GUEST_RIP);
	lcd_arch->regs.rsp = vmcs_readl(GUEST_RSP);

	if (unlikely(lcd_arch->fail)) {
		/*
		 * See Intel SDM V3 30.4 for error codes
		 */
		LCD_ERR("failure detected (err %x)\n",
			vmcs_read32(VM_INSTRUCTION_ERROR));
		lcd_arch->exit_reason = VMX_EXIT_REASONS_FAILED_VMENTRY;
		return;
	}

	lcd_arch->exit_reason = vmcs_read32(VM_EXIT_REASON);
	lcd_arch->exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
	lcd_arch->idt_vectoring_info = vmcs_read32(IDT_VECTORING_INFO_FIELD);
	lcd_arch->error_code = vmcs_read32(IDT_VECTORING_ERROR_CODE);
	lcd_arch->exit_intr_info = vmcs_read32(VM_EXIT_INTR_INFO);
	lcd_arch->vec_no = lcd_arch->exit_intr_info & INTR_INFO_VECTOR_MASK;

	return;
}

int lcd_arch_run(struct lcd_arch *lcd_arch)
{
	int ret;

	/*
	 * Load the lcd and invalidate any cached mappings.
	 *
	 * *preemption disabled*
	 */
	vmx_get_cpu(lcd_arch);

	/*
	 * Interrupts off
	 *
	 * This is important - see Documentation/lcd-domains/vmx.txt.
	 */
	local_irq_disable();

	/*
	 * Enter lcd
	 */
	vmx_enter(lcd_arch);

	/*
	 * Check/handle nmi's, exceptions, and external interrupts *before*
	 * we re-enable interrupts.
	 */
	ret = vmx_handle_exception_interrupt(lcd_arch);
	
	/*
	 * Now turn interrupts back on.
	 */
	local_irq_enable();

	if (ret) {
		/*
		 * We exited due to an exception, nmi, or external interrupt.
		 * All done.
		 */
		goto out;
	}

	/*
	 * Handle all other exit reasons
	 *
	 * Intel SDM V3 Appendix C
	 */
	ret = vmx_handle_other_exits(lcd_arch);

out:
	/*
	 * Preemption enabled
	 */
	vmx_put_cpu(lcd_arch);	

	/*
	 * If there was an error, dump the lcd's state.
	 */
	if (ret < 0)
		dump_lcd_arch(lcd_arch);

	return ret;
}

