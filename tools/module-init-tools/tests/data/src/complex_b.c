/* A depends on B, C and D.  B depends on E.  C depends on B and E.  D
   depends on B. */
#include "module.h"

int b;
extern int e;

static void foo(void)
{
	b = e = 0;
}


EXPORT_SYMBOL(b);

