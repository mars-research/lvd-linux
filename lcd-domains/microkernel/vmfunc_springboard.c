#include <asm/current.h>

__asm__(
		".text	\n\t"
		".align 16	\n\t"
		".globl vmfunc_kernel_springboard	\n\t"
		".extern cpuid_page		\n\t"
		".extern lcd_stack_off		\n\t"
		"vmfunc_kernel_springboard:	\n\t"
		"  mov cpuid_page, %rax		\n\t"
		/* place esp_kernel in r13 */
		"  mov 8(%rax), %r13		\n\t"
		/* TODO: restore %gs */
		"  mov %gs:current_task, %rax	\n\t"
		"  mov lcd_stack_off, %rbx	\n\t"
		/* save esp_lcd */
		"  mov %rsp, (%rax, %rbx)	\n\t"
		/* populate esp_kernel */
		"  mov %r13, %rsp		\n\t"

#ifdef CONFIG_LVD_DISABLE_IRQS
		/* we are in trusted domain, re-enable irqs */
		"  sti				\n\t"
#endif
		/* stack is populated. we are good to go */
		"  call vmfunc_dispatch		\n\t"

		/* we are ready to jump back to the caller */
		/* save kernel stack */
		/* get cpuid page buffer */
		"  mov cpuid_page, %rax		\n\t"
		/* save esp_kernel in the cpuid page */
		"  mov %rsp, 8(%rax)		\n\t"

#ifdef CONFIG_LVD_DISABLE_IRQS
		/* disable irqs, we jump back to untrusted domain */
		"  cli			\n\t"
#endif

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
