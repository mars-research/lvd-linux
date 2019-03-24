#ifndef VMFUNC_TRAMPOLINE_H
#define VMFUNC_TRAMPOLINE_H

#include <linux/types.h>
#include <libfipc.h>
#include <asm/lcd_domains/libvmfunc.h>

/* extract data from linker variables */
size_t vmfunc_sboard_page_size = (size_t)&__vmfunc_sboard_page_size;
unsigned long* vmfunc_sboard_load_addr = (unsigned long*) &__vmfunc_sboard_load_addr;

/* extract data from linker variables */
size_t vmfunc_trampoline_page_size = (size_t)&__vmfunc_trampoline_page_size;
unsigned long* vmfunc_trampoline_load_addr = (unsigned long*) &__vmfunc_trampoline_load_addr;

#endif /* VMFUNC_TRAMPOLINE_H */
