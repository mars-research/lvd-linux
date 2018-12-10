#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include <linux/module.h>
#include <linux/compiler.h>

#include <libfipc.h>
#include "vmfunc_trampoline.h"
#include "rpc.h"

#include <lcd_config/post_hook.h>


#define VMX_VMFUNC ".byte 0x0f,0x01,0xd4"

#define VMFUNC_LINKAGE	  __attribute__((section(".vmfunc.text")))

/* vmfunc reserved register
 * eax = 0 (select vmfunctions, 0 - eptp switching)
 * ecx = i (i - index of the eptp to switch to. 0 < i < 512)
 */

#define REG_EAX		"%%eax"
#define REG_ECX		"%%ecx"
#define REG_RAX		"%%rax"
#define REG_RBX		"%%rbx"
#define REG_RCX		"%%rcx"
#define REG_RDX		"%%rdx"
#define REG_RSI		"%%rsi"
#define REG_RDI		"%%rdi"
#define REG_R8		"%%r8"
#define REG_R9		"%%r9"
#define REG_R10		"%%r10"
#define REG_R11		"%%r11"
#define REG_R12		"%%r12"
#define REG_R13		"%%r13"
#define REG_R14		"%%r14"
#define REG_R15		"%%r15"
#define REG_RSP		"%%rsp"

#define REG0	REG_RDI
#define REG1	REG_RSI
#define REG2	REG_RDX
/* well, we have to break the ABI here.
 * According to ABI, RCX is used as a register to pass arguments.
 * VMFUNC instruction uses ECX to pass eptp_list index.
 */
//#define REG3	REG_RCX
#define REG3	REG_R8
#define REG4	REG_R9
#define REG5	REG_R10
#define REG6	REG_R11
#define REG7	REG_R12

extern int __vmfunc_page_size;
extern int __vmfunc_load_addr;

void *g_stack = NULL;

size_t vmfunc_page_size = (size_t)&__vmfunc_page_size;
unsigned long* vmfunc_load_addr = (unsigned long*) &__vmfunc_load_addr;

extern int noinline null_invocation(void);

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
		unsigned long reg6); //r11
__asm__ (
	"	.section .vmfunc.text, \"ax\"	\n\t"
	"	.align 16	\n\t"
	"	.type dispatch_loop,  @function \n\t"
	"dispatch_loop:		\n\t"
	/* construct fipc_message */
	"  sub $64, %rsp		\n\t"
	"  mov %rdi, 8(%rsp)	\n\t"
	"  mov %rsi, 16(%rsp)	\n\t"
	"  mov %rdx, 24(%rsp)	\n\t"
	"  mov %r8, 32(%rsp)	\n\t"
	"  mov %r9, 40(%rsp)	\n\t"
	"  mov %r10, 48(%rsp)	\n\t"
	"  mov %r11, 56(%rsp)	\n\t"
	"  mov %rsp, %rdi		\n\t"
	"  call __dispatch_loop	\n\t"
	"  add $64, %rsp		\n\t"
	"  ret			\n\t"
	);

int noinline
VMFUNC_LINKAGE
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
VMFUNC_LINKAGE
vmfunc_call(unsigned int func, //rdi
		unsigned int ept, //rsi
		struct fipc_message *msg) //rdx
{
	asm volatile(
		/* function prologue pushes callee-saved registers onto the
		 * stack as we have those registers clobbered */
		/* populate eax,ecx for vmfunc */
		"mov %[func], " REG_EAX " \n\t"
		"mov %[ept], " REG_ECX " \n\t"
		/* populate registers as per calling convention */
		"mov %[reg0]," REG0 " \n\t"
		"mov %[reg1]," REG1 " \n\t"
		"mov %[reg2]," REG2 " \n\t"
		"mov %[reg3]," REG3 " \n\t"
		"mov %[reg4]," REG4 " \n\t"
		"mov %[reg5]," REG5 " \n\t"
		"mov %[reg6]," REG6 " \n\t"
		/* zero callee saved registers */
		"xor " REG_RBX "," REG_RBX " \n\t"
		"xor " REG_R12 "," REG_R12 " \n\t"
		"xor " REG_R13 "," REG_R13 " \n\t"
		"xor " REG_R14 "," REG_R14 " \n\t"
		"xor " REG_R15 "," REG_R15 " \n\t"
		/* save RSP */
		"mov " REG_RSP ", %[g_stack] \n\t"
		/* call vmfunc */
		"vmfunc \n\t"
		/* restore RSP */
		"mov %[g_stack], " REG_RSP " \n\t"
		"call dispatch_loop \n\t"
		/* when the call returns RSP should be the same as above */
		/* response message is on REG0..REG6 registers */
		/* if so, do we need to save again? */
		/* save RSP */
		"mov " REG_RSP ", %[g_stack] \n\t"
		/* we do not need any registers to be saved as we are
		 * returning to caller
		 */
		/* eax is used by vmfunc. so move the return value to rdi */
		"mov " REG_RAX ", " REG0 " \n\t"
		"mov 0, " REG_EAX " \n\t"
		/* OTHER_DOMAIN is set via cflags */
		"mov " __stringify(OTHER_DOMAIN) ", " REG_ECX " \n\t"
		"vmfunc			\n\t"
		/* restore RSP */
		"mov %[g_stack], " REG_RSP " \n\t"
		/* after restoring the callee-saved registers are popped off
		 * the current stack and the calle returns
		 */
		: : [func]"r"(func),
		[ept]"r"(ept),
		[reg0]"m"(msg->regs[0]),
		[reg1]"m"(msg->regs[1]),
		[reg2]"m"(msg->regs[2]),
		[reg3]"m"(msg->regs[3]),
		[reg4]"m"(msg->regs[4]),
		[reg5]"m"(msg->regs[5]),
		[reg6]"m"(msg->regs[6]),
		[g_stack]"m"(g_stack)
		:
		/* add this to clobbered list and let the compiler generate push and pop
		 * sequences in prologue and epilogue automatically
		 */
		"rbx", "r12", "r13", "r14",
		"r15", "memory", "cc");
}
