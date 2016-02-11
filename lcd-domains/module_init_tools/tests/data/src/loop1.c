/* Part of a simple module loop */
#include "module.h"

extern int from_loop2;

int from_loop1;

EXPORT_SYMBOL(from_loop1);

static void foo(void)
{
	from_loop1 = from_loop2;
}
