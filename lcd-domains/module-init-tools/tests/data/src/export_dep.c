/* A module exporting a symbol, and requiring a symbol */
#include "module.h"

extern int exported1;
int exported3;

EXPORT_SYMBOL(exported3);

static void foo(void)
{
	exported3 = exported1;
}
