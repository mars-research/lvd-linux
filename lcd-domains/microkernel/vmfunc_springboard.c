#include <asm/asm-offsets.h>
#include <asm/current.h>
#include <asm/lcd_domains/libvmfunc.h>

__asm__(
		".text	\n\t"
		".align 16	\n\t"
		".globl vmfunc_kernel_springboard	\n\t"
		".extern vmfunc_state_page	\n\t"
		".extern lcd_stack_off		\n\t"
		"vmfunc_kernel_springboard:	\n\t"

		/* restore kernel_stack from vmfunc_state_page */
		"  mov " __stringify(VMFUNC_kernel_esp) " + vmfunc_state_page, %r13	\n\t"
		/* TODO: restore %gs */
		"  mov %gs:current_task, %rax	\n\t"
		"  mov lcd_stack_off, %rbx	\n\t"
		/* save esp_lcd */
		"  mov %rsp, (%rax, %rbx)	\n\t"
		/* populate esp_kernel */
		"  mov %r13, %rsp		\n\t"

		/* set entered_lcd = 0 in vmfunc_state_page */
		"  movq $0x0, " __stringify(VMFUNC_entered_lcd) " + vmfunc_state_page	\n\t"

#ifdef CONFIG_LVD_DISABLE_IRQS
		/* we are in trusted domain, re-enable irqs */
		"  sti				\n\t"
#endif
		/* stack is populated. we are good to go */
		"  call vmfunc_dispatch		\n\t"

		/* we are ready to jump back to the caller */
		/* save kernel stack */
		/* save rsp to the vmfunc_state_page */
		"  mov %rsp, " __stringify(VMFUNC_kernel_esp) " + vmfunc_state_page	\n\t"

#ifdef CONFIG_LVD_DISABLE_IRQS
		/* disable irqs, we jump back to untrusted domain */
		"  cli			\n\t"
#endif

#ifdef CONFIG_DEFEAT_LAZY_TLB
		/*
		 * vmfunc_dispatch populates all the return values in these
		 * regs. We are making a C call here, let's save and restore
		 */
		"  push " _REG0 " \n\t"
		"  push " _REG1 " \n\t"
		"  push " _REG2 " \n\t"
		"  push " _REG3 " \n\t"
		"  push " _REG4 " \n\t"
		"  push " _REG5 " \n\t"
		"  push " _REG6 " \n\t"
		"  push " _REG7 " \n\t"

		/* we need a valid stack here */
		"  call remap_cr3	\n\t"

		"  pop " _REG7 " \n\t"
		"  pop " _REG6 " \n\t"
		"  pop " _REG5 " \n\t"
		"  pop " _REG4 " \n\t"
		"  pop " _REG3 " \n\t"
		"  pop " _REG2 " \n\t"
		"  pop " _REG1 " \n\t"
		"  pop " _REG0 " \n\t"
#endif

		/* set entered_lcd = 1 in vmfunc_state_page */
		"  movq $0x1, " __stringify(VMFUNC_entered_lcd) " + vmfunc_state_page	\n\t"
		/* restore stack for vmfunc domain */
		"  mov %gs:current_task, %rax	\n\t"
		"  mov lcd_stack_off, %rbx	\n\t"
		"  mov (%rax, %rbx), %rsp	\n\t"

		/* LCD STACK RESTORED! */

		/* zero registers as we go to untrusted domain */
		"  xor %rbx, %rbx \n\t"
		"  xor %r13, %r13 \n\t"
		"  xor %r14, %r14 \n\t"
		"  xor %r15, %r15 \n\t"
		"  xor %rbp, %rbp \n\t"

		/* This is the return path */
		"  mov $0, %rax		\n\t"
		"  mov $1, %rcx		\n\t"
		"  jmp vmfunc_springboard_return	\n\t"
	);
