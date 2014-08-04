/* A depends on B, C and D.  B depends on E.  C depends on B and E.  D
   depends on B. */
#include "module.h"

int d;
extern int b;

static void foo(void)
{
	d = b;
}

EXPORT_SYMBOL(d);

