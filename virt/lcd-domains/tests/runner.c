/**
 * runner.c - Runs the regression tests.
 */

#include <linux/kernel.h>
#include <linux/module.h>

void cap_tests(void);
void ipc_tests(void);
void mem_tests(void);
void cptr_tests(void);
void enterexit_tests(void);
void arch_tests(void);
void dstore_tests(void);

static int __init runner_init(void)
{
	cap_tests();
	ipc_tests();
	mem_tests();
	cptr_tests();
	enterexit_tests();
	arch_tests();
	dstore_tests();
	return 0;
}

static void __exit runner_exit(void)
{
	/* Do nothing */
	return;
}

/* EXPORTS ---------------------------------------- */

module_init(runner_init);
module_exit(runner_exit);


