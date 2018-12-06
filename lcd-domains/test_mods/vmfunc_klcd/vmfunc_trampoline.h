#ifndef VMFUNC_TRAMPOLINE_H
#define VMFUNC_TRAMPOLINE_H

#include <linux/types.h>

#define VMFUNC_LINKAGE	  __attribute__((section(".vmfunc.text")))

void noinline
VMFUNC_LINKAGE
vmfunc(unsigned int nr, unsigned int ept);

extern size_t vmfunc_page_size;
extern unsigned long* vmfunc_load_addr;

#endif /* VMFUNC_TRAMPOLINE_H */
