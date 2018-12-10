#include <linux/compiler.h>
#include "vmfunc_trampoline.h"
#include <libfipc.h>

#define VMFUNC_WRAPPER_LINKAGE	  __attribute__((section(".vmfuncwrapper.text")))

/* include/linux/stringify.h */
#define __stringify_1(x...)	#x
#define __stringify(x...)	__stringify_1(x)

int noinline
VMFUNC_WRAPPER_LINKAGE
vmfunc_wrapper(struct fipc_message *request)
{
	static int once = 0;

	if (!once) {
		printk("%s, vmfunc (%p)\n", __func__, vmfunc_call);
		print_hex_dump(KERN_DEBUG, "vmfunc: ", DUMP_PREFIX_ADDRESS,
			       16, 1, vmfunc_call, 0x100, false);
		once = 1;
	}
	vmfunc_call(0, OTHER_DOMAIN, request);
	return 0;
}
