#ifndef VMFUNC_TRAMPOLINE_H
#define VMFUNC_TRAMPOLINE_H

//#include <lcd_config/pre_hook.h>

//#include <lcd_config/post_hook.h>

#define VMX_VMFUNC ".byte 0x0f,0x01,0xd4"

#define VMFUNC_LINKAGE	  __attribute__((section(".vmfunc.text")))

static void noinline
VMFUNC_LINKAGE
vmfunc(unsigned int nr, unsigned int ept)
{
	asm volatile(VMX_VMFUNC
		     :
		     : "a"(nr), "c"(ept)
		     : "memory");
}

extern char __vmfunc_page_size;
extern char __vmfunc_load_addr;

size_t vmfunc_page_size = (size_t)&__vmfunc_page_size;
unsigned char* vmfunc_load_addr = (unsigned char*) &__vmfunc_load_addr;
#if 0
static void *
__attribute__((unused))
VMFUNC_LINKAGE
get_stack(void)
{
	return NULL;
}
#endif
#endif /* VMFUNC_TRAMPOLINE_H */
