#include <asm/lcd_domains/libvmfunc.h>
#include <linux/sched.h>
#include <asm/current.h>

extern void *cpuid_page;

void
vmfunc_trampoline_entry(struct fipc_message *msg); //rdi
__asm__ (
	"	.text		\n\t"
	"	.align 16	\n\t"
	"	.globl vmfunc_trampoline_entry	\n\t"
	"	.type vmfunc_trampoline_entry, @function  \n\t"
	"	.extern cpuid_page	\n\t"
	"	.extern lcd_stack_off	\n\t"
	"vmfunc_trampoline_entry:	\n\t"
	/*
	 * push all callee-saved regs according to ABI
	 */
	"  push %rbp	\n\t"
	"  push %rbx	\n\t"
	"  push %r12	\n\t"
	"  push %r13	\n\t"
	"  push %r14	\n\t"
	"  push %r15	\n\t"

	/*
	 * push msg pointer onto the stack. this is needed for
	 * constructing response buffer when the call returns
	 */
	"  push %rdi	\n\t"
	/*
	 * mov rdi to rax as rdi carries the function arguments to the
	 * next call
	 */
	"  mov %rdi, %rax \n\t"

	/* populate registers as per calling convention */
	"  mov (%rax)," _REG0 " \n\t"
	"  mov 8(%rax)," _REG1 " \n\t"
	"  mov 16(%rax)," _REG2 " \n\t"
	"  mov 24(%rax)," _REG3 " \n\t"
	"  mov 32(%rax)," _REG4 " \n\t"
	"  mov 40(%rax)," _REG5 " \n\t"
	"  mov 48(%rax)," _REG6 " \n\t"
	"  mov 56(%rax)," _REG7 " \n\t"

	/* get cpuid page buffer */
	"  mov cpuid_page, %rax		\n\t"
	/* save rsp to the cpuid page at offset 8*/
	"  mov %rsp, 8(%rax)		\n\t"
	/* get current pointer */
	"  mov %gs:current_task, %r13	\n\t"
	/* get lcd_stack offset */
	"  mov lcd_stack_off, %r14	\n\t"
	/* populate LCD stack */
	"  mov (%r13, %r14), %rsp		\n\t"

	/* populate eax, ecx for vmfunc */
	"  mov $0x0, %eax \n\t"
	"  mov $0x1, %ecx \n\t"

	/* zero callee saved registers */
	"  xor %rbx, %rbx	\n\t"
	"  xor %r13, %r13	\n\t"
	"  xor %r14, %r14	\n\t"
	"  xor %r15, %r15	\n\t"
	"  xor %rbp, %rbp	\n\t"

	/* jumps to the real trampoline */
	"  jmp vmfunc_trampoline	\n\t"
	/* vmfunc_trampoline switches vmfunc and back,
	 * * calls vmfunc_trampoline_out
	 * 	+ restores stack
	 * 	+ construct response message
	 * 	+ return to the caller of this function
	 */
	);

__asm__ (
	"	.text		\n\t"
	"	.align 16	\n\t"
	"	.globl vmfunc_trampoline_out \n\t"
	"	.extern cpuid_page	\n\t"
	"vmfunc_trampoline_out:		\n\t"
	/* restore stack */
	"  mov cpuid_page, %rbx	\n\t"
	"  mov 8(%rbx), %rsp	\n\t"
	/* stack pointer is restored, let's get our msg buffer */
	"  pop %r13 \n\t"
	/* construct response fipc_message from registers */
	"  mov " _REG0 ", 0(%r13) \n\t"
	"  mov " _REG1 ", 8(%r13) \n\t"
	"  mov " _REG2 ", 16(%r13) \n\t"
	"  mov " _REG3 ", 24(%r13) \n\t"
	"  mov " _REG4 ", 32(%r13) \n\t"
	"  mov " _REG5 ", 40(%r13) \n\t"
	"  mov " _REG6 ", 48(%r13) \n\t"
	"  mov " _REG7 ", 56(%r13) \n\t"

	/* everything is set, pop all regs */
	"  pop %r15	\n\t"
	"  pop %r14	\n\t"
	"  pop %r13	\n\t"
	"  pop %r12	\n\t"
	"  pop %rbx	\n\t"
	"  pop %rbp	\n\t"
	/* return to caller */
	"  ret		\n\t"
	);

