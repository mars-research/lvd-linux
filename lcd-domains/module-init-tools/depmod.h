/*
 * depmod.h - prototypes for the depmod utility
 */

#ifndef MODINITTOOLS_DEPMOD_H
#define MODINITTOOLS_DEPMOD_H
#include "list.h"
#include "elfops.h"

struct module;

/* This is not the same as the module struct in the kernel built .ko files */
struct module
{
	/* Next module in list of all modules */
	struct module *next;

	/* Dependencies: filled in by ops->calculate_deps() */
	unsigned int num_deps;
	struct module **deps;

	/* Set while we are traversing dependencies */
	struct list_head dep_list;

	/* Line number in modules.order (or INDEX_PRIORITY_MIN) */
	unsigned int order;

	/* Tables extracted from module by ops->fetch_tables(). */
	struct module_tables tables;

	/* Module operations, endian conversion required, etc. */
	struct elf_file *file;

	char *basename; /* points into pathname */
	char pathname[0];
};

#endif /* MODINITTOOLS_DEPMOD_H */
