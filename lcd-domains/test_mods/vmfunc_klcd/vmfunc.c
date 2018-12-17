#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include <linux/module.h>
#include <linux/compiler.h>
#include <asm/lcd_domains/libvmfunc.h>

#include <libfipc.h>
#include "vmfunc_trampoline.h"
#include "rpc.h"

#include <lcd_config/post_hook.h>

#define VMFUNC_TEXT_SECTION		".vmfunc.text"
#define __vmfunc_page	  __attribute__((section(VMFUNC_TEXT_SECTION)))

/* Linker variables */
extern int __vmfunc_page_size;
extern int __vmfunc_load_addr;

/* extract data from linker variables */
size_t vmfunc_page_size = (size_t)&__vmfunc_page_size;
unsigned long* vmfunc_load_addr = (unsigned long*) &__vmfunc_load_addr;

void *stack_pages[NR_CPUS];
void *stack_ptrs[NR_CPUS];

void *g_stack = NULL;
/* this is the only function Intel VT-x support */
static int func = 0;
extern int noinline null_invocation(void);

int
vmfunc_init(void)
{
	int i = 0;
	for (i = 0; i < NR_CPUS; i++) {
		stack_pages[i] = lcd_vmalloc(PAGE_SIZE);
		/* store stack pointers */
		stack_ptrs[i] = stack_pages[i];
		if (!stack_pages[i])
			goto clean;
	}
	return 0;
clean:
	for (; i >=0; i--) {
		lcd_vfree(stack_pages[i]);
	}
	return -ENOMEM;
}

void
vmfunc_exit(void)
{
	int i = 0;
	for (; i < NR_CPUS; i++) {
		lcd_vfree(stack_pages[i]);
	}
}
/* this function is called from vmfunc layer with arguments on the registers
 * (per system V abi)
 */
void
dispatch_loop(unsigned long reg0, //rdi
		unsigned long reg1, //rsi
		unsigned long reg2, //rdx
		/* rcx is clobbered. we cannot use it */
		unsigned long reg3, //r8
		unsigned long reg4, //r9
		unsigned long reg5, //r10
		unsigned long reg6, //r11
		unsigned long reg7); //r12
__asm__ (
	"	.section " VMFUNC_TEXT_SECTION ", \"ax\"	\n\t"
	"	.align 16	\n\t"
	"	.type dispatch_loop,  @function \n\t"
	"dispatch_loop:		\n\t"
	/* construct fipc_message */
	"  push %rdi	\n\t"
	"  push %rsi	\n\t"
	"  push %rdx	\n\t"
	"  push %r8	\n\t"
	"  push %r9	\n\t"
	"  push %r10	\n\t"
	"  push %r11	\n\t"
	"  push %r12	\n\t"
	/* make a pointer to this fipc_message struct and place it
	 * as first argument */
	"  mov %rsp, %rdi		\n\t"
	"  call __dispatch_loop	\n\t"
	/* restore stack pointer */
	"  add $64, %rsp		\n\t"
	"  ret			\n\t"
	);

int noinline
__vmfunc_page
__dispatch_loop(struct fipc_message *msg)
{
	int ret = 0;

	switch(msg->regs[0]) {
	case NULL_INVOCATION:
		ret = null_invocation();
		break;
	default:
		break;
	}

	return ret;
}


void
__vmfunc_page
vmfunc_call(unsigned int ept, //rdi
		struct fipc_message *msg) //rsi
{
	asm volatile(
		/* function prologue pushes callee-saved registers onto the
		 * stack as we have those registers clobbered */
		/* populate eax,ecx for vmfunc */
		"mov %[func], " _REG_EAX " \n\t"
		"mov %[ept], " _REG_ECX " \n\t"
		/* populate registers as per calling convention */
		"mov %[reg0]," REG0 " \n\t"
		"mov %[reg1]," REG1 " \n\t"
		"mov %[reg2]," REG2 " \n\t"
		"mov %[reg3]," REG3 " \n\t"
		"mov %[reg4]," REG4 " \n\t"
		"mov %[reg5]," REG5 " \n\t"
		"mov %[reg6]," REG6 " \n\t"
		"mov %[reg7]," REG7 " \n\t"
		/* zero callee saved registers */
		"xor " _REG_RBX "," _REG_RBX " \n\t"
		"xor " _REG_R12 "," _REG_R12 " \n\t"
		"xor " _REG_R13 "," _REG_R13 " \n\t"
		"xor " _REG_R14 "," _REG_R14 " \n\t"
		"xor " _REG_R15 "," _REG_R15 " \n\t"
		/* save RSP */
		"mov " _REG_RSP ", %[g_stack] \n\t"
		/* call vmfunc */
		"vmfunc \n\t"
		/* restore RSP */
		"mov %[g_stack], " _REG_RSP " \n\t"
		"call dispatch_loop \n\t"
		/* when the call returns RSP should be the same as above */
		/* response message is on REG0..REG6 registers */
		/* if so, do we need to save again? */
		/* save RSP */
		"mov " _REG_RSP ", %[g_stack] \n\t"
		/* we do not need any registers to be saved as we are
		 * returning to caller
		 */
		/* eax is used by vmfunc. so move the return value to rdi */
		"mov " _REG_RAX ", " REG0 " \n\t"
		"mov 0, " _REG_EAX " \n\t"
		/* OTHER_DOMAIN is set via cflags */
		"mov " __stringify(OTHER_DOMAIN) ", " _REG_ECX " \n\t"
		"vmfunc			\n\t"
		/* restore RSP */
		"mov %[g_stack], " _REG_RSP " \n\t"
		/* after restoring the callee-saved registers are popped off
		 * the current stack and the calle returns
		 */
		: : [func]"i"(func),
		[ept]"r"(ept),
		[reg0]"m"(msg->id),
		[reg1]"m"(msg->regs[0]),
		[reg2]"m"(msg->regs[1]),
		[reg3]"m"(msg->regs[2]),
		[reg4]"m"(msg->regs[3]),
		[reg5]"m"(msg->regs[4]),
		[reg6]"m"(msg->regs[5]),
		[reg7]"m"(msg->regs[6]),
		[g_stack]"m"(g_stack)
		:
		/* add this to clobbered list and let the compiler generate push and pop
		 * sequences in prologue and epilogue automatically
		 */
		"rbx", "r12", "r13", "r14",
		"r15", "memory", "cc");
}
