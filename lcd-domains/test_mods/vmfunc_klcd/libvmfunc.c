#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include <linux/module.h>
#include <linux/compiler.h>

#include <libfipc.h>
#include "vmfunc_trampoline.h"
#include "rpc.h"

#include <lcd_config/post_hook.h>


#define VMX_VMFUNC ".byte 0x0f,0x01,0xd4"

#define VMFUNC_SECTION_NAME	".vmfunc.text"
#define __vmfunc		__attribute__((section(VMFUNC_SECTION_NAME)))

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

#define REG0	REG_RDI
#define REG1	REG_RSI
#define REG2	REG_RDX
#define REG3	REG_RCX
#define REG4	REG_R8
#define REG5	REG_R9
#define REG6	REG_R10
#define REG7	REG_R11

#if defined(__x86_64__)
#define PUSH_CALLEE_SAVED_REGS()					\
  __asm__ volatile ("" : : : "rbx", "r12", "r13", "r14", "r15",         \
		    "memory", "cc")

#define ZERO_CALLEE_SAVED_REGS()					\
  __asm__ volatile("xor %%rbx, %%rbx\n\t"				\
		  "xor %%r12, %%r12\n\t"				\
		  "xor %%r13, %%r13\n\t"				\
		  "xor %%r14, %%r14\n\t"				\
		  "xor %%r15, %%r15\n\t"				\
		  :::)
#endif

extern int __vmfunc_page_size;
extern int __vmfunc_load_addr;

//struct fipc_message msg;
void *g_stack = NULL;
struct fipc_message g_resp;

size_t vmfunc_page_size = (size_t)&__vmfunc_page_size;
unsigned long* vmfunc_load_addr = (unsigned long*) &__vmfunc_load_addr;

#define CONSTRUCT_FUNC_ARGS(msg)					\
  __asm__ volatile("mov %[reg0]," REG0					\
		  "\n\tmov %[reg1]," REG1				\
		  "\n\tmov %[reg2]," REG2				\
		  "\n\tmov %[reg3]," REG3				\
		  "\n\tmov %[reg4]," REG4				\
		  "\n\tmov %[reg5]," REG5				\
		  "\n\tmov %[reg6]," REG6				\
		  ::							\
		  [reg0]"m"(msg->regs[0]),				\
		  [reg1]"m"(msg->regs[1]),				\
		  [reg2]"m"(msg->regs[2]),				\
		  [reg3]"m"(msg->regs[3]),				\
		  [reg4]"m"(msg->regs[4]),				\
		  [reg5]"m"(msg->regs[5]),				\
		  [reg6]"m"(msg->regs[6])				\
		  :)

#define CONSTRUCT_FIPC_MSG(msg)						\
  __asm__ volatile("mov " REG0 ", %[reg0]"				\
		  "\n\tmov " REG1 ", %[reg1]"				\
		  "\n\tmov " REG2 ", %[reg2]"				\
		  "\n\tmov " REG3 ", %[reg3]"				\
		  "\n\tmov " REG4 ", %[reg4]"				\
		  "\n\tmov " REG5 ", %[reg5]"				\
		  "\n\tmov " REG6 ", %[reg6]"				\
		  :							\
		  [reg0]"=m"(msg->regs[0]),				\
		  [reg1]"=m"(msg->regs[1]),				\
		  [reg2]"=m"(msg->regs[2]),				\
		  [reg3]"=m"(msg->regs[3]),				\
		  [reg4]"=m"(msg->regs[4]),				\
		  [reg5]"=m"(msg->regs[5]),				\
		  [reg6]"=m"(msg->regs[6])				\
		  ::)
#define SAVE_RSP()							\
  __asm__ volatile("mov %%rsp, %[g_stack]"				\
		  : [g_stack]"=m"(g_stack) ::);

#define RESTORE_RSP()							\
  __asm__ volatile("mov %[g_stack], %%rsp"				\
		  :: [g_stack]"m"(g_stack) :);


void __vmfunc_return(int ret);
extern int noinline null_invocation(void);
/* this function is called from vmfunc layer with arguments on the registers
 * (per system V abi)
 */
void noinline
VMFUNC_LINKAGE
dispatch_loop(void) {
	struct fipc_message msg;
	int ret = 0;

	CONSTRUCT_FIPC_MSG((&msg));

	switch(msg.regs[0]) {
	case NULL_INVOCATION:
		ret = null_invocation();
		break;
	default:
		break;
	}

	__vmfunc_return(ret);
	return;
}

void
vmfunc_send_request(int func, //rdi
		int ept, //rsi
		struct fipc_message *msg) //rdx
{
	asm volatile(
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
		: : [func]"m"(func),
		[ept]"m"(ept),
		[reg0]"m"(msg->regs[0]),
		[reg1]"m"(msg->regs[1]),
		[reg2]"m"(msg->regs[2]),
		[reg3]"m"(msg->regs[3]),
		[reg4]"m"(msg->regs[4]),
		[reg5]"m"(msg->regs[5]),
		[reg6]"m"(msg->regs[6]),
		[g_stack]"m"(g_stack)
		:);


	/* arguments are ready on the respective registers. Just invoke dispatch loop */
	//dispatch_loop();
	/* callee-saved regs are popped automatically by the compiler as we
	 * have used it in this function. */
}


void noinline
VMFUNC_LINKAGE
vmfunc_return(struct vmfunc_msg *vmsg)
{
	/* push callee saved regs */
	PUSH_CALLEE_SAVED_REGS();

	/* save nr and ept before we clobber rdi */
	asm volatile("mov %[nr], %%r12d\n\t"
			"mov %[ept], %%r13d\n\t"
			:: [nr]"m"(vmsg->nr)
			,[ept]"m"(vmsg->ept)
			:);

	CONSTRUCT_FUNC_ARGS((vmsg->msg));

	/* restore nr and ept before vmfunc */
	asm volatile(
			"xor %%rax, %%rax\n\t"
			"xor %%rcx, %%rcx\n\t"
			"mov %%r12d, %%eax\n\t"
			"mov %%r13d, %%ecx\n\t"
			::: );

	/* We are ready to vmfunc. Let's wipe callee saved regs */
	ZERO_CALLEE_SAVED_REGS();

	/* we leak RBP, but should be fine for now */
	SAVE_RSP();

	asm volatile(VMX_VMFUNC
			::
		     : "memory");

	/* here we return */
	RESTORE_RSP();

	/* construct response */
	CONSTRUCT_FIPC_MSG((&g_resp));

	/* callee-saved are popped up automatically from our restored stack */
	return;
}

void __vmfunc_return(int ret) {
	struct fipc_message msg;
	struct vmfunc_msg vmsg = { 0,  0, &msg };
	msg.regs[0] = ret;
	vmfunc_return(&vmsg);
}
