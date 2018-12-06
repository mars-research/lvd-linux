#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include <linux/module.h>
#include <linux/compiler.h>

#include "vmfunc_trampoline.h"

#include <lcd_config/post_hook.h>


#define VMX_VMFUNC ".byte 0x0f,0x01,0xd4"

#define VMFUNC_LINKAGE	  __attribute__((section(".vmfunc.text")))

void noinline
VMFUNC_LINKAGE
vmfunc(unsigned int nr, unsigned int ept)
{
	asm volatile(VMX_VMFUNC
		     :
		     : "a"(nr), "c"(ept)
		     : "memory");
}

extern int __vmfunc_page_size;
extern int __vmfunc_load_addr;

size_t vmfunc_page_size = (size_t)&__vmfunc_page_size;
unsigned long* vmfunc_load_addr = (unsigned long*) &__vmfunc_load_addr;
