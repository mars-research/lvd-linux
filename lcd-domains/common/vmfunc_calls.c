#include <lcd_config/pre_hook.h>

#include <linux/compiler.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <lcd_domains/microkernel.h>
#include <liblcd/liblcd.h>
#include <linux/stringify.h>

#include <lcd_config/post_hook.h>

#define VMFUNC_TEXT_SECTION		".vmfunc.text"
#define __vmfunc	__attribute__((section(VMFUNC_TEXT_SECTION)))

#define VMFUNC_WRAPPER_SECTION		".vmfuncwrapper.text"
#define __vmfunc_wrapper __attribute__((section(VMFUNC_WRAPPER_SECTION)))

int handle_vmfunc_syscall(struct fipc_message *msg);
int handle_vmfunc_syncipc(struct fipc_message *msg);

/* this is the only function Intel VT-x support */
#define VM_FUNCTION	0

/* percpu stack pages and stack pointers */
static void **stack_pages;
static void **stack_ptrs;

/* percpu response buffers */
struct fipc_message **responses;

/* exported by the microkernel. We trust that it's sane */
extern void *cpuid_page;

extern int __vmfunc_page_size;
extern int __vmfunc_load_addr;

int
vmfunc_init(void *stack_page)
{
	/* TODO: this num_online_cpus macro accesses __cpu_online_mask
	 * Make sure we map this page on LCDs EPT
	 */
	stack_pages = kmalloc(sizeof(void*) * num_online_cpus(), GFP_KERNEL);
	stack_ptrs = kmalloc(sizeof(void*) * num_online_cpus(), GFP_KERNEL);
	stack_pages[0] = stack_page;
	stack_ptrs[0] = stack_pages[0];
#if 0
	int i, j;
	for (i = 0; i < num_online_cpus(); i++) {
		stack_pages[i] = kmalloc(PAGE_SIZE * 2, GFP_KERNEL);
		/* store stack pointers */
		stack_ptrs[i] = stack_pages[i];
		if (!stack_pages[i])
			goto clean;
	}

	responses = kmalloc(sizeof(struct fipc_message*) * num_online_cpus(), GFP_KERNEL);
	for (j = 0; j < num_online_cpus(); j++) {
		responses[j] = kmalloc(sizeof(struct fipc_message), GFP_KERNEL);
		if (!responses[j])
			goto clean2;
	}

	return 0;

clean2:
	for (; j >=0; j--)
		kfree(responses[j]);
clean:
	for (; i >=0; i--)
		kfree(stack_pages[i]);

	return -ENOMEM;
#endif
	return 0;
}
EXPORT_SYMBOL(vmfunc_init);

void
vmfunc_exit(void)
{
	int i = 0;

	for (; i < num_online_cpus(); i++)
		kfree(stack_pages[i]);

	for (; i < num_online_cpus(); i++)
		kfree(responses[i]);
}

int handle_vmfunc_syncipc(struct fipc_message *msg)
{
	return 0;
}


/* Every module (KLCD, LCD should define their own handle_rpc_calls() to handle
 * RPC calls for that domain
 */
int
__weak
handle_rpc_calls(struct fipc_message *msg) {
	printk("weak alias for %s! If you want to handle RPCs"
		" please override the function in your module\n", __func__);
	return 0;
};
EXPORT_SYMBOL(handle_rpc_calls);

/*
 * test1: Switch to domain 1 and back
 * Required: CR3 remapping
 * Result: Passed
 */
void noinline
__vmfunc
vmfunc_call_empty_switch(unsigned int ept, //rdi
			struct fipc_message *msg) // rsi
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

int noinline
__vmfunc_dispatch(struct fipc_message *msg)
{
	int ret = 0;

	switch(msg->vmfunc_id) {
	case VMFUNC_SYNC_IPC:
		handle_vmfunc_syncipc(msg);
		break;

#ifndef LCD_ISOLATE
	case VMFUNC_LCD_SYSCALL:
		ret = handle_vmfunc_syscall(msg);
		break;
#endif
	case VMFUNC_RPC_CALL:
		ret = handle_rpc_calls(msg);
		break;
	default:
#if 0
		printk("%s, no handler for unknown call type %d\n",
				__func__, msg->vmfunc_id);

#endif
		fipc_set_reg1(msg, 0xdad);
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
	"	.section " VMFUNC_TEXT_SECTION ", \"ax\"	\n\t"
	"	.align 16	\n\t"
	"	.type vmfunc_dispatch,  @function \n\t"
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


/* functions with vmfunc instructions should be on the same page on both
 * host Linux and LCDs. Place it in a separate section
 */
void
__vmfunc
vmfunc_call(unsigned int ept, //rdi
		struct fipc_message *msg) //rsi
{
	asm volatile(
		/* function prologue pushes callee-saved registers onto the
		 * stack as we have those registers clobbered */

		/* push msg pointer onto the stack. this is needed for
		 * constructing response buffer when the call returns */
		"push " _REG_RSI " \n\t"
		"push " _REG_RDI " \n\t"
		/*
		 * mov rsi to rax as rsi carries the function arguments to the
		 * next call
		 */
		"mov " _REG_RSI "," _REG_RAX " \n\t"

		/* populate registers as per calling convention */
		"mov (" _REG_RAX ") ," REG0 " \n\t"
		"mov 8(" _REG_RAX ") ," REG1 " \n\t"
		"mov 16(" _REG_RAX ") ," REG2 " \n\t"
		"mov 24(" _REG_RAX ") ," REG3 " \n\t"
		"mov 32(" _REG_RAX ") ," REG4 " \n\t"
		"mov 40(" _REG_RAX ") ," REG5 " \n\t"
		"mov 48(" _REG_RAX ") ," REG6 " \n\t"
		"mov 56(" _REG_RAX ") ," REG7 " \n\t"

		/* get cpuid page buffer */
		"mov %[cpuid], " _REG_RBX " \n\t"
		/* get cpu number from cpuid page */
		"mov (" _REG_RBX "), " _REG_RAX " \n\t"
		/* get base address of stack pointer array */
		"mov %[stack_ptrs], " _REG_R13 " \n\t"
		/* pop back the ept value we pushed earlier */
		"pop " _REG_RCX " \n\t"
		/* save rsp to the cpuid page at offset 8*/
		"mov " _REG_RSP ", 8(" _REG_RBX ") \n\t"
		/* populate stack for vmfunc domain */
		/* FIXME: This should be from TLS of current */
		"mov (" _REG_R13 ", " _REG_RAX ", 8), " _REG_RSP " \n\t"
		/* populate eax for vmfunc */
		"mov %[func], " _REG_EAX " \n\t"

		/* zero callee saved registers */
		"xor " _REG_RBX "," _REG_RBX " \n\t"
		"xor " _REG_R13 "," _REG_R13 " \n\t"
		"xor " _REG_R14 "," _REG_R14 " \n\t"
		"xor " _REG_R15 "," _REG_R15 " \n\t"
		"xor " _REG_RBP "," _REG_RBP " \n\t"

		/* call vmfunc */
		"vmfunc \n\t"
		/* stack is populated. we are good to go */
		"call vmfunc_dispatch \n\t"
		/* This is the return path */
		"mov $0, " _REG_EAX " \n\t"
		/* OTHER_DOMAIN is set via cflags */
		"mov $" __stringify(OTHER_DOMAIN) ", " _REG_ECX " \n\t"

		/* do a vmfunc back */
		"vmfunc			\n\t"
		/* get cpuid page buffer */
		"mov %[cpuid], " _REG_RBX " \n\t"
		/* get cpu number from cpuid page */
		"mov (" _REG_RBX "), " _REG_RCX " \n\t"
		/* restore KLCD's RSP */
		"mov 8(" _REG_RBX "), " _REG_RSP " \n\t"
		/* stack pointer is restored, let's get our msg buffer */
		"pop " _REG_R13 " \n\t"
		/* construct response fipc_message from registers */
		"mov " REG0 ", 0(" _REG_R13 ") \n\t"
		"mov " REG1 ", 8(" _REG_R13 ") \n\t"
		"mov " REG2 ", 16(" _REG_R13 ") \n\t"
		"mov " REG3 ", 24(" _REG_R13 ") \n\t"
		"mov " REG4 ", 32(" _REG_R13 ") \n\t"
		"mov " REG5 ", 40(" _REG_R13 ") \n\t"
		"mov " REG6 ", 48(" _REG_R13 ") \n\t"
		"mov " REG7 ", 56(" _REG_R13 ") \n\t"
		/* after restoring the callee-saved registers are popped off
		 * the current stack and the callee returns
		 */
		: : [func]"i"(VM_FUNCTION),
		[ept]"r"(ept),
		[msg]"m"(msg),
		[stack_ptrs]"m"(stack_ptrs),
		[cpuid]"m"(cpuid_page),
		[resps]"m"(responses)
		:
		/* add this to clobbered list and let the compiler generate push and pop
		 * sequences in prologue and epilogue automatically
		 */
		"rbx", "r12", "r13", "r14",
		"r15", "memory", "cc");
}
EXPORT_SYMBOL(vmfunc_call);

int noinline
__vmfunc_wrapper
vmfunc_wrapper(struct fipc_message *request)
{
	static int once = 0;

	if (!once) {
		printk("%s, vmfunc (%p), request: %p\n", __func__, vmfunc_call, request);
		if (0)
			print_hex_dump(KERN_DEBUG, "vmfunc: ", DUMP_PREFIX_ADDRESS,
			       16, 1, vmfunc_call, 0x100, false);
		once = 1;
	}
	vmfunc_call(OTHER_DOMAIN, request);
	return 0;
}

int noinline
__vmfunc_wrapper
vmfunc_test_wrapper(struct fipc_message *request, vmfunc_test_t test)
{

	switch (test) {
	case VMFUNC_TEST_EMPTY_SWITCH:
		vmfunc_call_empty_switch(OTHER_DOMAIN, request);
		break;
	case VMFUNC_TEST_DUMMY_CALL:
		/* only upto vmfunc_id 0x3 is handled */
		request->vmfunc_id = 0x4;
		vmfunc_call(OTHER_DOMAIN, request);
		break;
	case VMFUNC_TEST_RPC_CALL:
		request->vmfunc_id = VMFUNC_RPC_CALL;
		vmfunc_call(OTHER_DOMAIN, request);
		break;
	}
	return 0;
}

EXPORT_SYMBOL(vmfunc_wrapper);
EXPORT_SYMBOL(vmfunc_test_wrapper);
EXPORT_SYMBOL(__vmfunc_load_addr);
EXPORT_SYMBOL(__vmfunc_page_size);
