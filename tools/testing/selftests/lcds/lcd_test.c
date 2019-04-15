#define _GNU_SOURCE
#include <linux/membarrier.h>
#include <syscall.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "../kselftest.h"

static inline unsigned long rdtsc(void)
{
	unsigned long stamp;
	/*
	 * Assumes x86
	 *
	 * rdtsc returns current cycle counter on cpu; 
	 * low 32 bits in %rax, high 32 bits in %rdx.
	 *
	 * Note: We use rdtsc to start the stopwatch because it won't
	 * wait for prior instructions to complete (that we don't care
	 * about). It is not exact - meaning that instructions after
	 * it in program order may start executing before the read
	 * is completed (so we may slightly underestimate the time to
	 * execute the intervening instructions). But also note that
	 * the two subsequent move instructions are also counted against
	 * us (overestimate).
	 */
	asm volatile(
		"rdtsc\n\t"
		"shl $32, %%rdx\n\t"
		"or %%rdx, %%rax\n\t" 
		: "=a" (stamp)
		:
		: "rdx");
	return stamp;
}

static inline unsigned long rdtscp(void)
{
	unsigned long stamp;
	/*
	 * Assumes x86
	 *
	 * Unlike start_stopwatch, we want to wait until all prior
	 * instructions are done, so we use rdtscp. (We don't care
	 * about the tsc aux value.)
	 */
	asm volatile(
		"rdtscp\n\t"
		"shl $32, %%rdx\n\t"
		"or %%rdx, %%rax\n\t" 
		: "=a" (stamp)
		:
		: "rdx", "rcx");
	return stamp;
}


unsigned long num_iterations = 1000000;

static int sys_lcd_null(void)
{
	return syscall(__NR_lcd_null);
}

int main(int argc, char **argv)
{
	unsigned long start = rdtsc(), end;

	for(int i = 0; i < num_iterations; i++) {
			
		//asm volatile("int $0xf3");
		sys_lcd_null(); 

	}
	end = rdtsc();
	printf("%lu iterations of lcd_null() call took %lu cycles (avg: %lu cycles)\n",
		num_iterations, end - start, (end - start) / num_iterations);

	return ksft_exit_pass();
}
