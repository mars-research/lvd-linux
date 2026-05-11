/*
 * Copyright (C) 2023 Yinggang
 *
 * This file is released under the GPL.
 */
// #include <sys/syscall.h>
#include <linux/module.h>
#include <linux/init.h>

static int __init wrpkrs_init(void)
{
	uint64_t start, end;
	unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
	int i;

	for (i=0; i<1000; i++) {

	asm volatile("cpuid\n\t"
				"rdtsc\n\t"
				"mov %%edx, %0\n\t"
				"mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
				"%rax", "%rbx", "%rcx", "%rdx");

	
	asm volatile ("mov $0x6e1, %ecx\n\t"
				"mov $0, %eax\n\t"
				"mov $0, %edx\n\t"
				"wrmsr");
	// syscall(SYS_getpid);

	asm volatile("rdtscp\n\t"
				"mov %%edx, %0\n\t"
				"mov %%eax, %1\n\t"
				"cpuid\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
				"%rax", "%rbx", "%rcx", "%rdx");
	start = ( ((uint64_t)cycles_high << 32) | cycles_low );
	end = ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );
	printk(KERN_INFO "start:%llu, end:%llu, cycles:%llu\n", start, end, end-start);

	}

	return 0;
}

static void __exit wrpkrs_exit(void)
{
	pr_info("wrpkrs_exit!\n");
}

module_init(wrpkrs_init)
module_exit(wrpkrs_exit)

MODULE_AUTHOR("Yinggang");
MODULE_DESCRIPTION("Test wrpkrs cycles");
MODULE_LICENSE("GPL");
