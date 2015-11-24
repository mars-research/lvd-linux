/* Part of a simple module loop */
#include "module.h"

extern int from_loop1;

int from_loop2;

EXPORT_SYMBOL(from_loop2);

static void foo(void)
{
	from_loop2 = from_loop1;
}
