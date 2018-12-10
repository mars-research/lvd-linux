#ifndef VMFUNC_TRAMPOLINE_H
#define VMFUNC_TRAMPOLINE_H

#include <linux/types.h>
#include <libfipc.h>
#define VMFUNC_LINKAGE	  __attribute__((section(".vmfunc.text")))

struct vmfunc_msg;

void noinline
VMFUNC_LINKAGE
vmfunc_call(unsigned int nr, unsigned int ept, struct fipc_message *reg);

extern size_t vmfunc_page_size;
extern unsigned long* vmfunc_load_addr;

struct vmfunc_msg {
	unsigned int nr;
	unsigned int ept;
	struct fipc_message *msg;
};
#endif /* VMFUNC_TRAMPOLINE_H */
