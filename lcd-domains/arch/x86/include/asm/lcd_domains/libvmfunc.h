#ifndef LIB_VMFUNC_H
#define LIB_VMFUNC_H

/* to use struct fipc_message */
#include <libfipc.h>
/* vmfunc reserved register (32-bit)
 * eax = 0 (select vmfunctions, 0 - eptp switching)
 * ecx = i (i - index of the eptp to switch to. 0 < i < 512)
 */
/* arch/x86/include/asm/vmx.h has macros REG_XXX defined.
 * prefix it with an '_' to avoid cluttering
 */
#define _REG_EAX		"%%eax"
#define _REG_ECX		"%%ecx"


/* other 64-bit regs */
#define _REG_RAX		"%%rax"
#define _REG_RBX		"%%rbx"
#define _REG_RCX		"%%rcx"
#define _REG_RDX		"%%rdx"
#define _REG_RSI		"%%rsi"
#define _REG_RDI		"%%rdi"
#define _REG_R8		"%%r8"
#define _REG_R9		"%%r9"
#define _REG_R10		"%%r10"
#define _REG_R11		"%%r11"
#define _REG_R12		"%%r12"
#define _REG_R13		"%%r13"
#define _REG_R14		"%%r14"
#define _REG_R15		"%%r15"
#define _REG_RSP		"%%rsp"

#define REG0	_REG_RDI
#define REG1	_REG_RSI
#define REG2	_REG_RDX
/* well, we have to break the ABI here.
 * According to ABI, RCX is used as a register to pass arguments.
 * VMFUNC instruction uses ECX to pass eptp_list index.
 */
//#define REG3	_REG_RCX
#define REG3	_REG_R8
#define REG4	_REG_R9
#define REG5	_REG_R10
#define REG6	_REG_R11
#define REG7	_REG_R12

typedef enum {
	VMFUNC_SYNC_IPC = 0x1,
	VMFUNC_LCD_SYSCALL = 0x2,
} vmfunc_id;

int handle_vmfunc_syscall(struct fipc_message *msg);
int handle_vmfunc_syncipc(struct fipc_message *msg);

#endif /* LIB_VMFUNC_H */
