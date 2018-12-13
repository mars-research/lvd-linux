#include <lcd_config/pre_hook.h>

#include <linux/compiler.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>

#include <lcd_config/post_hook.h>

#define __vmfunc_syscalls	__attribute__((section(".vmfunc.syscalls")))


static int func = 0;
static int host_ept = 0;
static int lcds_ept = 1;

#define REG_RSP		"%%rsp"

void *syscall_stack;

int vmfunc_syscall_init(void)
{
	/* do this only for klcd case as syscalls are originated only from
	 * LCDs.  When LCDs call vmfunc_syscall, the code will save the
	 * existing stack pointer to syscall_stack variable and restore it on
	 * the way back. On the host kernel, we need a stack to operate on.
	 * Let's allocate a page for it.
	 */
#ifndef LCD_ISOLATE
	/* allocate a single page for stack */
	syscall_stack = vmalloc(PAGE_SIZE);
	if (!syscall_stack) {
		LCD_ERR("failed to alloc mem for syscall stack");
		return -ENOMEM;
	}
#endif
}

int
__vmfunc_syscalls
vmfunc_syscall(unsigned long id)
{
	long ret;
	asm volatile(
		"mov %[id], %%rdi \n\t"
		"mov %[func], %%eax \n\t"
		"mov %[hept], %%ecx \n\t"
		/* save RSP */
		"mov " REG_RSP ", %[g_stack] \n\t"
		"vmfunc \n\t"
		/* restore RSP */
		"mov %[g_stack], " REG_RSP " \n\t"
		"call syscall_dispatch \n\t"
		/* return value is in rax */
		"mov %%rax, %%rdi \n\t"
		"mov $0, %%eax \n\t"
		"mov %[lept], %%ecx \n\t"
		/* we are back after invoking the syscall, no need to save stack */
		"vmfunc \n\t"
		/* restore RSP */
		"mov %[g_stack], " REG_RSP " \n\t"
		: : [id]"r"(id),
		[func]"i"(func),
		[hept]"i"(host_ept),
		[lept]"i"(lcds_ept),
		[g_stack]"m"(syscall_stack)
		:
		/* add this to clobbered list and let the compiler generate push and pop
		 * sequences in prologue and epilogue automatically
		 */
		"rbx", "r12", "r13", "r14",
		"r15", "memory", "cc");
}
