#ifndef _MODULE_H
#define _MODULE_H

struct kernel_symbol
{
	unsigned long value;
	const char *name;
};

#define EXPORT_SYMBOL(sym)					\
	const char __ksymtab_string_##sym[]			\
	__attribute__((section("__ksymtab_strings"))) = #sym;	\
	const struct kernel_symbol __ksymtab_##sym		\
	__attribute__((section("__ksymtab")))			\
	= { (unsigned long)&sym, __ksymtab_string_##sym }

#endif
