#include <lcd_config/pre_hook.h>

#include <linux/compiler.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <lcd_domains/microkernel.h>
#include <liblcd/liblcd.h>
#include <linux/stringify.h>

#include <lcd_config/post_hook.h>

#define VMFUNC_WRAPPER_SECTION		".vmfuncwrapper.text"
#define __vmfunc_wrapper __attribute__((section(VMFUNC_WRAPPER_SECTION)))

/* this is the only function Intel VT-x support */
#define VM_FUNCTION	0

int handle_vmfunc_syscall(struct fipc_message *msg);
int handle_vmfunc_syncipc(struct fipc_message *msg);

#ifndef LCD_ISOLATE
int handle_vmfunc_syncipc(struct fipc_message *msg)
{
	return 0;
}
#endif
/* Every module (KLCD, LCD should define their own handle_rpc_calls() to handle
 * RPC calls for that domain
 */
int
__weak
handle_rpc_calls(struct fipc_message *msg);
#if 0
{
	printk("weak alias for %s! If you want to handle RPCs"
		" please override the function in your module\n", __func__);
	return 0;
};
EXPORT_SYMBOL(handle_rpc_calls);
#endif
#ifndef LCD_ISOLATE
extern rpc_handler_t klcd_handler;
#endif
int noinline
__vmfunc_dispatch(struct fipc_message *msg)
{
	int ret = 0;

	switch(msg->vmfunc_id) {

#ifndef LCD_ISOLATE
	case VMFUNC_SYNC_IPC:
		handle_vmfunc_syncipc(msg);
		break;

	case VMFUNC_LCD_SYSCALL:
		ret = handle_vmfunc_syscall(msg);
		break;
#endif
	case VMFUNC_RPC_CALL:
#ifdef LCD_ISOLATE
		if (handle_rpc_calls)
			ret = handle_rpc_calls(msg);
#else
		if (klcd_handler)
			ret = klcd_handler(msg);
#endif
		else {
			printk("rpc handler doesn't exist\n");
		}
		break;
	default:
#if 0
		printk("%s, no handler for unknown call type %d\n",
				__func__, msg->vmfunc_id);

#endif
		fipc_set_reg1(msg, 0xdad);
		fipc_set_reg2(msg, 0xadd);
		fipc_set_reg3(msg, 0xcad);
		break;
	}

	return ret;
}

/* this function is called from vmfunc layer with arguments on the registers
 * (per system V abi)
 */
void
vmfunc_dispatch(unsigned long reg0, //rdi
		unsigned long reg1, //rsi
		unsigned long reg2, //rdx
		/* rcx is clobbered. we cannot use it */
		unsigned long reg3, //r8
		unsigned long reg4, //r9
		unsigned long reg5, //r10
		unsigned long reg6, //r11
		unsigned long reg7); //r12
__asm__ (
	"	.align 16	\n\t"
	"	.type vmfunc_dispatch,  @function \n\t"
	"	.globl vmfunc_dispatch	\n\t"
	"vmfunc_dispatch:		\n\t"
	/* construct request fipc_message */
	"  push %r12	\n\t"
	"  push %r11	\n\t"
	"  push %r10	\n\t"
	"  push %r9	\n\t"
	"  push %r8	\n\t"
	"  push %rdx	\n\t"
	"  push %rsi	\n\t"
	"  push %rdi	\n\t"
	/* make a pointer to this fipc_message struct and place it
	 * as first argument */
	"  mov %rsp, %rdi		\n\t"
	"  call __vmfunc_dispatch	\n\t"
	/* populate registers for response fipc_message */
	"  pop %rdi	\n\t"
	"  pop %rsi	\n\t"
	"  pop %rdx	\n\t"
	"  pop %r8	\n\t"
	"  pop %r9	\n\t"
	"  pop %r10	\n\t"
	"  pop %r11	\n\t"
	"  pop %r12	\n\t"
	/* stack pointer is restored */
	"  ret			\n\t"
	);


int noinline
__vmfunc_wrapper
vmfunc_wrapper(struct fipc_message *request)
{
	vmfunc_springboard_entry(request);
	return 0;
}

EXPORT_SYMBOL(vmfunc_wrapper);

/*
 * vmfunc.trampoline.text functions
 * This section contains the code pages that are shared between the microkernel
 * and all LCDs
 */
#define VMFUNC_TRAMPOLINE_SECTION		".vmfunc.trampoline.text"
#define __vmfunc_trampoline	__attribute__((section(VMFUNC_TRAMPOLINE_SECTION)))

__asm__ (
	"	.section " VMFUNC_TRAMPOLINE_SECTION ", \"ax\"      \n\t"
	"	.align 16	\n\t"
	"	.globl vmfunc_trampoline \n\t"
	"	.weak vmfunc_trampoline_out \n\t"
	"vmfunc_trampoline:		\n\t"
	/* vmfunc to domain 1 */
	"  vmfunc 		\n\t"
	/* abort if ecx != 1 */
	"  cmp $0x1, %ecx	\n\t"
	"  jne abort		\n\t"
	/* stack is populated. we are good to go */
	"  call vmfunc_dispatch \n\t"
	/* This is the return path */
	"  mov $0, %eax		\n\t"
	"  mov $0, %ecx		\n\t"
	/* vmfunc to return to domain 0 */
	"  vmfunc 		\n\t"
	/* abort if ecx != 0 */
	"  cmp $0x0, %ecx	\n\t"
	"  jne abort		\n\t"
	"  jmp vmfunc_trampoline_out	\n\t"
	"abort:		\n\t"
	"vmfunc_trampoline_out:		\n\t"
	"  mov $0xabcd, %eax	\n\t"
	"  cpuid		\n\t"
	);

void noinline
__vmfunc_trampoline
vmfunc_call_empty_switch(void)
{
	asm volatile(
		/* populate eax, ecx */
		"mov $0, %eax \n\t"
		"mov $1, %ecx \n\t"
		/* switch to domain 1 */
		"vmfunc 	\n\t"
		/* zero ecx */
		"xor %ecx, %ecx \n\t"
		/* switch back to domain 0 */
		"vmfunc 	\n\t"
		);
}

#define VMFUNC_SBOARD_SECTION		".vmfunc.springboard.text"
#define __vmfunc_springboard	__attribute__((section(VMFUNC_SBOARD_SECTION)))
/*
 * functions with vmfunc instructions should be on the same page on both
 * host Linux and LCDs. Place it in a separate section to achieve this.
 * This is a call from an untrusted domain to the kernel, which is trusted.
 */
void
__vmfunc_springboard
vmfunc_springboard_entry(struct fipc_message *msg /* rdi */);

__asm__ (
	"	.section " VMFUNC_SBOARD_SECTION ", \"ax\" \n\t"
	"	.align 16	\n\t"
	"	.globl vmfunc_springboard_entry	\n\t"
	"	.type vmfunc_springboard_entry, @function  \n\t"
	"	.weak vmfunc_kernel_springboard	\n\t"
	"vmfunc_springboard_entry:	\n\t"
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

	/*
	 * populate eax, ecx for vmfunc
	 * This code always jumps to the trusted domain (ecx=0)
	 */
	"  xor %rax, %rax	\n\t"
	"  mov $0, %ecx \n\t"
	/* do vmfunc */
	"  vmfunc \n\t"

	/* abort if ecx != 0 */
	"  cmp $0, %ecx	\n\t"
	"  jne abort_sb	\n\t"

	/* transition to kernel springboard handler */
	"  jmp vmfunc_kernel_springboard	\n\t"
	);


__asm__(
	"	.section " VMFUNC_SBOARD_SECTION ", \"ax\" \n\t"
	"	.align 16	\n\t"
	"	.globl vmfunc_springboard_return	\n\t"
	"vmfunc_springboard_return:	\n\t"
	/* do a vmfunc back */
	"  vmfunc			\n\t"
	"  cmp $1, %ecx		\n\t"
	"  jne abort_sb		\n\t"
	/* stack pointer is restored, let's get our msg buffer */
	"  pop " _REG_R13 	" \n\t"
	/* construct response fipc_message from registers */
	"  mov " _REG0 ", 0(" _REG_R13 ") \n\t"
	"  mov " _REG1 ", 8(" _REG_R13 ") \n\t"
	"  mov " _REG2 ", 16(" _REG_R13 ") \n\t"
	"  mov " _REG3 ", 24(" _REG_R13 ") \n\t"
	"  mov " _REG4 ", 32(" _REG_R13 ") \n\t"
	"  mov " _REG5 ", 40(" _REG_R13 ") \n\t"
	"  mov " _REG6 ", 48(" _REG_R13 ") \n\t"
	"  mov " _REG7 ", 56(" _REG_R13 ") \n\t"
	/*
	 * after restoring the callee-saved registers are popped off
	 * the current stack and the callee returns
	 */
	"  pop %r15	\n\t"
	"  pop %r14	\n\t"
	"  pop %r13	\n\t"
	"  pop %r12	\n\t"
	"  pop %rbx	\n\t"
	"  pop %rbp	\n\t"
	"  ret		\n\t"
	"abort_sb:		\n\t"
	"  mov $0xabcd, %eax	\n\t"
	"  cpuid		\n\t"
	);
