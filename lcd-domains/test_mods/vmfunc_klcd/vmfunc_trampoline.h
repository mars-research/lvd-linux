#pragma once

#ifndef VMFUNC_TRAMPOLINE_H
#define VMFUNC_TRAMPOLINE_H

#include <linux/types.h>
#include <libfipc.h>

/* Linker variables */
extern int __vmfunc_page_size;
extern int __vmfunc_load_addr;

/* extract data from linker variables */
size_t vmfunc_page_size = (size_t)&__vmfunc_page_size;
unsigned long* vmfunc_load_addr = (unsigned long*) &__vmfunc_load_addr;

#endif /* VMFUNC_TRAMPOLINE_H */
