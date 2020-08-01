/*
 * caller.c
 *
 * The "caller side" of the channel
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <linux/slab.h>
#include <liblcd/liblcd.h>
#include "../rpc.h"
#include "../rdtsc_helper.h"
#include <linux/module.h>
#include <linux/kallsyms.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <asm/processor.h>
#include <asm/desc.h>
#include <asm/lcd_domains/bflank.h>

#include <lcd_config/post_hook.h>

extern int vmfunc_wrapper(struct fipc_message *req);
extern int vmfunc_init(void *stack_page, rpc_handler_t rpc_handler,
		rpc_handler_t sync_handler);
unsigned long long stack;
unsigned long long eip;
extern void *lcd_stack;

int handle_rpc_calls_klcd(struct fipc_message *msg);

DECLARE_PER_CPU_PAGE_ALIGNED(char, exception_stacks
        [(N_EXCEPTION_STACKS - 1) * EXCEPTION_STKSZ + DEBUG_STKSZ]);

void run_vmfunc_tests(void)
{
	struct fipc_message msg;
	int cpu;
	unsigned char idt_ptr[10];
	unsigned long idt_base;

	asm volatile("sidt %[idt_ptr]"
			:[idt_ptr]"=m"(idt_ptr));

	idt_base = *((unsigned long *)&idt_ptr[2]);

	INIT_FIPC_MSG(&msg);

	/* test1: empty switch */
	vmfunc_klcd_test_wrapper(&msg, OTHER_DOMAIN, VMFUNC_TEST_EMPTY_SWITCH);
	printk("%s: VMFUNC_TEST_EMPTY_SWITCH: Passed\n", __func__);

	/* test2: dummy unhandled call type */
	vmfunc_klcd_test_wrapper(&msg, OTHER_DOMAIN, VMFUNC_TEST_DUMMY_CALL);

	printk("%s, lcd->stack %p\n", __func__, current->lcd_stack);
	printk("%s: VMFUNC_TEST_DUMMY_CALL: Passed\n\tValue from other domain r1: %lx, r2:%lx, r3:%lx\n",
			__func__,
			fipc_get_reg1(&msg),
			fipc_get_reg2(&msg),
			fipc_get_reg3(&msg)
			);

	printk("%s, lcd->stack %p\n", __func__, current->lcd_stack);
	/* test3: RPC call, null_invocation */
	msg.rpc_id = NULL_INVOCATION;
	vmfunc_klcd_test_wrapper(&msg, OTHER_DOMAIN, VMFUNC_TEST_RPC_CALL);
	printk("%s: VMFUNC_TEST_RPC_CALL: Passed!\n\tValue from other domain %lx\n",
				__func__, fipc_get_reg1(&msg));


	/* test3a: RPC call microbenchmark */
	memset(&msg, 0x0, sizeof(msg));
	msg.rpc_id = MARSHAL_ONE;
	fipc_set_reg0(&msg, (u64) 0xabcd);

	vmfunc_klcd_test_wrapper(&msg, OTHER_DOMAIN, VMFUNC_TEST_RPC_CALL_BENCHMARK);

	/* test4: RPC call and get a call back */
	//for_each_online_cpu(cpu) {
	cpu = smp_processor_id();
	{
		//char *ist_stack = &per_cpu(exception_stacks, cpu);
		struct tss_struct *tss = &per_cpu(cpu_tss, cpu);
		char *lvd_stack = (char*) tss->x86_tss.ist[IRQ_LVD_STACK - 1];
		unsigned int * except_stack_sz = (unsigned int*) kallsyms_lookup_name("exception_stack_sizes");

		/* test4: RPC call and get a call back */
		memset(&msg, 0x0, sizeof(msg));
		msg.rpc_id = FOO;
		fipc_set_reg0(&msg, (u64) idt_base);
		fipc_set_reg1(&msg, (u64) tss);
		fipc_set_reg2(&msg, (u64) &per_cpu(gdt_page, cpu));
		fipc_set_reg3(&msg, (u64) lvd_stack);
		fipc_set_reg4(&msg, (u64) except_stack_sz[IRQ_LVD_STACK - 1]);
		fipc_set_reg5(&msg, cpu);

		vmfunc_klcd_test_wrapper(&msg, OTHER_DOMAIN, VMFUNC_TEST_RPC_CALL);
	}

	printk("%s: VMFUNC_TEST_RPC_CALLBACK: Value from other domain "
			"r1: %lx, r2: %lx, r3: %lx, r4: %lx, r5: %lx, r6: %lx\n",
			__func__,
			fipc_get_reg1(&msg),
			fipc_get_reg2(&msg),
			fipc_get_reg3(&msg),
			fipc_get_reg4(&msg),
			fipc_get_reg5(&msg),
			fipc_get_reg6(&msg)
			);

	INIT_FIPC_MSG(&msg);
	{
		struct ext_registers *this_reg_page = get_register_page(smp_processor_id());
		int i;
		printk("%s, register page %p\n", __func__, this_reg_page);
		for (i = 0; i < PAGE_SIZE/64; i++) {
			this_reg_page->regs[i] = 0xabcd + i;
		}
		msg.rpc_id = MODULE_INIT;
		printk("%s, before MODULE_INIT lcd->stack %p\n", __func__, current->lcd_stack);
		vmfunc_klcd_test_wrapper(&msg, OTHER_DOMAIN, VMFUNC_TEST_RPC_CALL);
		printk("%s: VMFUNC_TEST_RPC_CALL: MODULE_INIT Passed!\n",
					__func__);
	}

	msg.rpc_id = NULL_INVOCATION;
	vmfunc_klcd_test_wrapper(&msg, OTHER_DOMAIN, VMFUNC_TEST_RPC_CALL);
	printk("%s: VMFUNC_TEST_RPC_CALL: Passed!\n\tValue from other domain %lx\n",
				__func__, fipc_get_reg1(&msg));

	/* test3a: RPC call microbenchmark */
	memset(&msg, 0x0, sizeof(msg));
	msg.rpc_id = MARSHAL_ONE;
	fipc_set_reg0(&msg, (u64) 0xabcd);

	vmfunc_klcd_test_wrapper(&msg, OTHER_DOMAIN, VMFUNC_TEST_RPC_CALL);
}

static int caller_main(void)
{
	int ret = 0;
#if 0
	unsigned int transaction_id = 0;
	u64 start, end;
#endif
//	struct fipc_message dummy = {0};
	/*
	 * only 3 types of calls are handled by the dispatch loop.  Set this to
	 * 0x4 to cause a vmfunc switch to goto the default case and return.
	 */
//	dummy.vmfunc_id = VMFUNC_RPC_CALL;
//	dummy.rpc_id = NULL_INVOCATION;
//	fipc_set_reg1(&dummy, 0xabcd);
	/*
	 * Boot
	 */
	printk("%s entered,  lcd_stack %p\n", __func__, lcd_stack);
	vmfunc_init(lcd_stack, handle_rpc_calls_klcd, NULL);

	run_vmfunc_tests();

	{
		int i = 0; 
		u64 s_exits = bfcall_dump_perf(), e_exits; 
		u64 start = rdtsc(), end;

		void *rsp_ptr;
		int num_iterations = 1000000;

		for(i = 0; i < num_iterations; i++) {
			asm volatile("mov %%rsp, %[rsp_ptr]"
				: [rsp_ptr]"=r"(rsp_ptr));
			//printk("rsp before int 0xf3 %p", rsp_ptr);

			asm volatile("int $0xf3");

			//asm volatile("mov %%rsp, %[rsp_ptr]"
			//		: [rsp_ptr]"=r"(rsp_ptr));
			//printk("rsp before int 0xf3 %p", rsp_ptr);
		}
		end = rdtsc();

		e_exits = bfcall_dump_perf(); 

		printk("caller lcd: %d iterations of int 0xf3 back-to-back took %llu cycles (avg: %llu cycles, total exits:%llu)\n",
				num_iterations, end - start, (end - start) / num_iterations, e_exits - s_exits);

	}
#if 0
	ret = lcd_enter();
	if (ret)
		goto out;
	start = lcd_RDTSC_START();
	while (transaction_id < TRANSACTIONS) {
		if (transaction_id == 0)
#endif
//	vmfunc_klcd_wrapper(&dummy, OTHER_DOMAIN);

#if 0
		transaction_id++;
	}
	end = lcd_RDTSC_STOP();
#endif
//	printk("Value from other domain %lx\n", fipc_get_reg1(&dummy));

#if 0
	printk("%s, vmfunc (%p) took %llu cycles (num_transactions = %d) | vmfunc_load_addr %p | size %zx\n", __func__,
			vmfunc_call, (end - start) / TRANSACTIONS, TRANSACTIONS, vmfunc_load_addr, vmfunc_page_size);

	print_hex_dump(KERN_DEBUG, "vmfuncwrapper: ", DUMP_PREFIX_ADDRESS,
		       32, 1, vmfunc_wrapper, 0x100, false);
#endif
	/*
	 * Done; just exit (everything will be torn down when we die)
	 */

	return ret;
#if 0
out:
	lcd_exit(ret);
	return ret;
#endif
}

static int caller_init(void)
{
	return caller_main();
}

static void __exit caller_exit(void)
{
	return;
}

module_init(caller_init);
module_exit(caller_exit);
MODULE_LICENSE("GPL");
