/* A depends on B, C and D.  B depends on E.  C depends on B and E.  D
   depends on B. */
#include "module.h"

int e;

EXPORT_SYMBOL(e);

