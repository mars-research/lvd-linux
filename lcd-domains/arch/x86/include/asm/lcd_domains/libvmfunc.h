#ifndef LIB_VMFUNC_H
#define LIB_VMFUNC_H


/* to use struct fipc_message */
#ifndef __ASSEMBLY__
#include <libfipc.h>
#endif /* !__ASSEMBLY__ */

/* vmfunc reserved register (32-bit)
 * eax = 0 (select vmfunctions, 0 - eptp switching)
 * ecx = i (i - index of the eptp to switch to. 0 < i < 512)
 */
/*
 * arch/x86/include/asm/vmx.h has macros REG_XXX defined.
 * prefix it with an '_' to avoid cluttering
 */
/* for pure assembly (.s and .S) */
#define EAX		%eax
#define ECX		%ecx
#define RAX		%rax
#define RBX		%rbx
#define RCX		%rcx
#define RDX		%rdx
#define RSI		%rsi
#define RDI		%rdi
#define R8		%r8
#define R9		%r9
#define R10		%r10
#define R11		%r11
#define R12		%r12
#define R13		%r13
#define R14		%r14
#define R15		%r15
#define RSP		%rsp
#define RBP		%rbp

#define FASM(x)		__stringify(x)
#define EXASM(x)	"%" __stringify(x)

/* for C-style assembly (uses "%rxx" format) */
#define _REG_EAX		FASM(EAX)
#define _REG_ECX		FASM(ECX)
#define _REG_RAX		FASM(RAX)
#define _REG_RBX		FASM(RBX)
#define _REG_RCX		FASM(RCX)
#define _REG_RDX		FASM(RDX)
#define _REG_RSI		FASM(RSI)
#define _REG_RDI		FASM(RDI)
#define _REG_R8		FASM(R8)
#define _REG_R9		FASM(R9)
#define _REG_R10		FASM(R10)
#define _REG_R11		FASM(R11)
#define _REG_R12		FASM(R12)
#define _REG_R13		FASM(R13)
#define _REG_R14		FASM(R14)
#define _REG_R15		FASM(R15)
#define _REG_RSP		FASM(RSP)
#define _REG_RBP		FASM(RBP)

/* for extended assembly (uses "%%rxx" format) */
#define EX_REG_EAX              EXASM(EAX)
#define EX_REG_ECX              EXASM(ECX)
#define EX_REG_RAX              EXASM(RAX)
#define EX_REG_RBX              EXASM(RBX)
#define EX_REG_RCX              EXASM(RCX)
#define EX_REG_RDX              EXASM(RDX)
#define EX_REG_RSI              EXASM(RSI)
#define EX_REG_RDI              EXASM(RDI)
#define EX_REG_R8               EXASM(R8)
#define EX_REG_R9               EXASM(R9)
#define EX_REG_R10              EXASM(R10)
#define EX_REG_R11              EXASM(R11)
#define EX_REG_R12              EXASM(R12)
#define EX_REG_R13              EXASM(R13)
#define EX_REG_R14              EXASM(R14)
#define EX_REG_R15              EXASM(R15)
#define EX_REG_RSP              EXASM(RSP)
#define EX_REG_RBP              EXASM(RBP)

/* well, we have to break the ABI here.
 * According to ABI, RCX is used as a register to pass arguments.
 * VMFUNC instruction uses ECX to pass eptp_list index.
 */
/* pure assembly */
#define REG0	RDI
#define REG1	RSI
#define REG2	RDX
#define REG3	R8
#define REG4	R9
#define REG5	R10
#define REG6	R11
#define REG7	R12

/* C-style assembly */
#define _REG0           FASM(REG0)
#define _REG1           FASM(REG1)
#define _REG2           FASM(REG2)
#define _REG3           FASM(REG3)
#define _REG4           FASM(REG4)
#define _REG5           FASM(REG5)
#define _REG6           FASM(REG6)
#define _REG7           FASM(REG7)

/* extended assembly */
#define EX_REG0         EXASM(REG0)
#define EX_REG1         EXASM(REG1)
#define EX_REG2         EXASM(REG2)
#define EX_REG3         EXASM(REG3)
#define EX_REG4         EXASM(REG4)
#define EX_REG5         EXASM(REG5)
#define EX_REG6         EXASM(REG6)
#define EX_REG7         EXASM(REG7)

#ifndef __ASSEMBLY__
typedef enum {
	VMFUNC_SYNC_IPC = 0x1,
	VMFUNC_LCD_SYSCALL = 0x2,
	VMFUNC_RPC_CALL = 0x3,
} vmfunc_id;

typedef enum {
	VMFUNC_TEST_EMPTY_SWITCH = 0x1,
	VMFUNC_TEST_DUMMY_CALL = 0x2,
	VMFUNC_TEST_RPC_CALL = 0x3,
	VMFUNC_TEST_RPC_CALLBACK = 0x4,
} vmfunc_test_t;

void vmfunc_trampoline_entry(struct fipc_message *msg);

void vmfunc_springboard_entry(struct fipc_message *msg);
int noinline vmfunc_wrapper(struct fipc_message *msg);
int noinline vmfunc_test_wrapper(struct fipc_message *msg, vmfunc_test_t type);
void noinline vmfunc_call_empty_switch(void);
int vmfunc_klcd_wrapper(struct fipc_message *msg, unsigned int ept);
int vmfunc_klcd_test_wrapper(struct fipc_message *msg, unsigned int ept, vmfunc_test_t type);


extern int __vmfunc_trampoline_page_size[], __vmfunc_trampoline_load_addr[];
extern int __vmfunc_sboard_page_size[], __vmfunc_sboard_load_addr[];

#endif /* !__ASSEMBLY__ */
#endif /* LIB_VMFUNC_H */
