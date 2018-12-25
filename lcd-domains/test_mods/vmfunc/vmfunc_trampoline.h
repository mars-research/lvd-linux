#ifndef VMFUNC_TRAMPOLINE_H
#define VMFUNC_TRAMPOLINE_H

//#include <lcd_config/pre_hook.h>

//#include <lcd_config/post_hook.h>

#define VMX_VMFUNC ".byte 0x0f,0x01,0xd4"

#define VMFUNC_LINKAGE	  __attribute__((section(".vmfunc.text")))

/* Linker variables */
extern int __vmfunc_page_size;
extern int __vmfunc_load_addr;

/* extract data from linker variables */
size_t vmfunc_page_size = (size_t)&__vmfunc_page_size;
unsigned long* vmfunc_load_addr = (unsigned long*) &__vmfunc_load_addr;

static void noinline
VMFUNC_LINKAGE
vmfunc(unsigned int nr, unsigned int ept)
{
	asm volatile(VMX_VMFUNC
		     :
		     : "a"(nr), "c"(ept)
		     : "memory");
}

#endif /* VMFUNC_TRAMPOLINE_H */
