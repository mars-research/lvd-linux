/*
 * Copyright (C) 2023 Yinggang
 *
 * This file is released under the GPL.
 */
#include <linux/module.h>
#include <linux/init.h>

// extern int pks_tables_init(void);
// extern int init_pks_dmap_tables(void);
// extern void init_private_stacks(int pkey);

static int __init pks_monitor_init(void)
{
	// int r1 = pks_tables_init();
	// int r2 = init_pks_dmap_tables();
// #ifdef CONFIG_PKS_STACK
//     init_private_stacks(2);
// #endif

// 	return r1 + r2;
	pr_info("pks_monitor_init!\n");
	return 0;
}

static void __exit pks_monitor_exit(void)
{
	pr_info("pks_monitor_exit!\n");
}

module_init(pks_monitor_init)
module_exit(pks_monitor_exit)

MODULE_AUTHOR("Yinggang");
MODULE_DESCRIPTION("PKS-protected tables");
MODULE_LICENSE("GPL");
