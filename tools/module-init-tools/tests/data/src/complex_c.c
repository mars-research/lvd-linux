/* A depends on B, C and D.  B depends on E.  C depends on B and E.  D
   depends on B. */
#include "module.h"

int c;
extern int b, e;

static void foo(void)
{
	c = b = e = 0;
}


EXPORT_SYMBOL(c);

