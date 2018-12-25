#include <lcd_config/pre_hook.h>

#include <linux/compiler.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <lcd_domains/microkernel.h>

#include <lcd_config/post_hook.h>

#define VMFUNC_CALL_SECTION		".vmfunc.call"
#define __vmfunc	__attribute__((section(VMFUNC_CALL_SECTION)))

#define VMFUNC_WRAPPER_SECTION		".vmfuncwrapper.text"
#define __vmfunc_wrapper __attribute__((section(VMFUNC_WRAPPER_SECTION)))

int handle_vmfunc_syscall(struct fipc_message *msg);
int handle_vmfunc_syncipc(struct fipc_message *msg);

static int func = 0;
static int host_ept = 0;
static int lcds_ept = 1;

void *_stack;


/* include/linux/stringify.h */
#define __stringify_1(x...)	#x
#define __stringify(x...)	__stringify_1(x)

int vmfunc_call_init(void)
{
	/* do this only for klcd case as syscalls are originated only from
	 * LCDs.  When LCDs call vmfunc_syscall, the code will save the
	 * existing stack pointer to syscall_stack variable and restore it on
	 * the way back. On the host kernel, we need a stack to operate on.
	 * Let's allocate a page for it.
	 */
#ifndef LCD_ISOLATE
	/* allocate a single page for stack */
	_stack = vmalloc(2 * PAGE_SIZE);
	if (!_stack) {
		LCD_ERR("failed to alloc mem for syscall stack");
		return -ENOMEM;
	}
#endif
	return 0;
}

void vmfunc_call_exit(void)
{
#ifndef LCD_ISOLATE
	if (_stack)
		vfree(_stack);
#endif
}

int handle_vmfunc_syncipc(struct fipc_message *msg)
{
	return 0;
}

//extern int noinline
//__vmfunc_dispatch(struct fipc_message *msg);

// Every module (KLCD, LCD should define their own handle_rpc_calls() to handle
// RPC calls for that domain
extern int handle_rpc_calls(struct fipc_message *);

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
	default:
		ret = handle_rpc_calls(msg);
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
		unsigned long reg6); //r11
__asm__ (
	"	.section " VMFUNC_CALL_SECTION ", \"ax\"	\n\t"
	"	.align 16	\n\t"
	"	.type vmfunc_dispatch,  @function \n\t"
	"vmfunc_dispatch:		\n\t"
	/* construct fipc_message */
	"  sub $64, %rsp		\n\t"
	"  mov %rdi, 0(%rsp)	\n\t"
	"  mov %rsi, 8(%rsp)	\n\t"
	"  mov %rdx, 16(%rsp)	\n\t"
	"  mov %r8, 24(%rsp)	\n\t"
	"  mov %r9, 32(%rsp)	\n\t"
	"  mov %r10, 40(%rsp)	\n\t"
	"  mov %r11, 48(%rsp)	\n\t"
	"  mov %r12, 56(%rsp)	\n\t"
	"  mov %rsp, %rdi		\n\t"
	"  call __vmfunc_dispatch	\n\t"
	"  add $64, %rsp		\n\t"
	"  ret			\n\t"
	);


/* functions with vmfunc instructions should be on the same page on both
 * host Linux and LCDs. Place it in a separate section
 */
int
__vmfunc
vmfunc_call(int domain, struct fipc_message *msg)
{
	long ret = 0;
	asm volatile(
		"mov %[func], " _REG_EAX " \n\t"
		"mov %[hept], "	_REG_ECX " \n\t"
		/* populate registers as per calling convention */
		"mov %[id]," REG0 " \n\t"
		"mov %[reg0]," REG1 " \n\t"
		"mov %[reg1]," REG2 " \n\t"
		"mov %[reg2]," REG3 " \n\t"
		"mov %[reg3]," REG4 " \n\t"
		"mov %[reg4]," REG5 " \n\t"
		"mov %[reg5]," REG6 " \n\t"
		"mov %[reg6]," REG7 " \n\t"

		/* save RSP */
		"mov " _REG_RSP ", %[g_stack] \n\t"
		"vmfunc \n\t"
		/* restore RSP */
		"mov %[g_stack], " _REG_RSP " \n\t"
		"call vmfunc_dispatch \n\t"
		/* return value is in rax */
		"mov " _REG_RAX "," REG0  " \n\t"
		"mov $0, " _REG_EAX " \n\t"
		"mov %[lept], " _REG_ECX " \n\t"
		/* we are back after invoking the syscall, no need to save stack */
		"vmfunc \n\t"
		/* restore RSP */
		"mov %[g_stack], " _REG_RSP " \n\t"
		: : [func]"i"(func),
		[hept]"i"(host_ept),
		[lept]"i"(lcds_ept),
		[g_stack]"m"(_stack),
		[id]"m"(msg->id),
		[reg0]"m"(msg->regs[0]),
		[reg1]"m"(msg->regs[1]),
		[reg2]"m"(msg->regs[2]),
		[reg3]"m"(msg->regs[3]),
		[reg4]"m"(msg->regs[4]),
		[reg5]"m"(msg->regs[5]),
		[reg6]"m"(msg->regs[6])
		:
		/* add this to clobbered list and let the compiler generate push and pop
		 * sequences in prologue and epilogue automatically
		 */
		"rbx", "r12", "r13", "r14",
		"r15", "memory", "cc");
	return ret;
}
EXPORT_SYMBOL(vmfunc_call);

int noinline
__vmfunc_wrapper
vmfunc_wrapper(struct fipc_message *request)
{
	static int once = 0;

	if (!once) {
		printk("%s, vmfunc (%p)\n", __func__, vmfunc_call);
		print_hex_dump(KERN_DEBUG, "vmfunc: ", DUMP_PREFIX_ADDRESS,
			       16, 1, vmfunc_call, 0x100, false);
		once = 1;
	}
	vmfunc_call(OTHER_DOMAIN, request);
	return 0;
}
EXPORT_SYMBOL(vmfunc_wrapper);
