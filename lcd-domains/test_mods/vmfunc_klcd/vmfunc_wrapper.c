#include <linux/compiler.h>
#include "vmfunc_trampoline.h"
#include <libfipc.h>

#define VMFUNC_WRAPPER_LINKAGE	  __attribute__((section(".vmfuncwrapper.text")))
/* vmfunc reserved register
 * eax = 0 (select vmfunctions, 0 - eptp switching)
 * ecx = i (i - index of the eptp to switch to. 0 < i < 512)
 */

#define REG_RAX		"rax"
#define REG_RBX		"rbx"
#define REG_RCX		"rcx"
#define REG_RDX		"rdx"
#define REG_RSI		"rsi"
#define REG_RDI		"rdi"
#define REG_R8		"r8"
#define REG_R9		"r9"
#define REG_R10		"r10"
#define REG_R11		"r11"

#define REG1	REG_RDI
#define REG2	REG_RSI
#define REG3	REG_RDX
#define REG4	REG_RCX
#define REG5	REG_R8
#define REG6	REG_R9
#define REG7	REG_R10
#define REG8	REG_R11


int noinline
VMFUNC_WRAPPER_LINKAGE
vmfunc_wrapper(struct fipc_message *request)
{
	static int once = 0;
	if (!once) {
		printk("%s, vmfunc (%p)\n", __func__, vmfunc);
		print_hex_dump(KERN_DEBUG, "vmfunc: ", DUMP_PREFIX_ADDRESS,
			       16, 1, vmfunc, 0x100, false);
		once = 1;
	}
	vmfunc(0, 0);
	return 0;
}
