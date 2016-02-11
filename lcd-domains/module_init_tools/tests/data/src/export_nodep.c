/* A module exporting two symbols, and requiring none. */
#include "module.h"

int exported1, exported2;

EXPORT_SYMBOL(exported1);
EXPORT_SYMBOL(exported2);
