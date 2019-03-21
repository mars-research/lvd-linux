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
#include "../vmfunc_trampoline.h"
#include <linux/module.h>
#include <asm/lcd_domains/libvmfunc.h>

#include <lcd_config/post_hook.h>

extern int vmfunc_wrapper(struct fipc_message *req);
extern int vmfunc_init(void*);
unsigned long long stack;
unsigned long long eip;
extern void *lcd_stack;
extern unsigned long *vmfunc_load_addr;
extern size_t vmfunc_page_size;

void run_vmfunc_tests(void)
{
	struct fipc_message msg = {0};

	/* test1: empty switch */
	vmfunc_klcd_test_wrapper(&msg, OTHER_DOMAIN, VMFUNC_TEST_EMPTY_SWITCH);

	/* test2: dummy unhandled call type */
	vmfunc_klcd_test_wrapper(&msg, OTHER_DOMAIN, VMFUNC_TEST_DUMMY_CALL);
	printk("VMFUNC_TEST_EMPTY_SWITCH: Value from other domain %lx\n", fipc_get_reg1(&msg));

	/* test3: RPC call, null_invocation */
	msg.rpc_id = NULL_INVOCATION;
	vmfunc_klcd_test_wrapper(&msg, OTHER_DOMAIN, VMFUNC_TEST_RPC_CALL);
	printk("VMFUNC_TEST_RPC_CALL: Value from other domain %lx\n", fipc_get_reg1(&msg));
}

static int caller_main(void)
{
	int ret = 0;
#if 0
	unsigned int transaction_id = 0;
	u64 start, end;
#endif
	struct fipc_message dummy = {0};
	/*
	 * only 3 types of calls are handled by the dispatch loop.  Set this to
	 * 0x4 to cause a vmfunc switch to goto the default case and return.
	 */
	dummy.vmfunc_id = 0x4;
	dummy.rpc_id = NULL_INVOCATION;	
	fipc_set_reg1(&dummy, 0xabcd);
	/*
	 * Boot
	 */
	printk("%s entered,  lcd_stack %p\n", __func__, lcd_stack);
	vmfunc_init(lcd_stack);
	
	run_vmfunc_tests();
#if 0
	ret = lcd_enter();
	if (ret)
		goto out;
	start = lcd_RDTSC_START();
	while (transaction_id < TRANSACTIONS) {
		if (transaction_id == 0)
#endif
	vmfunc_klcd_wrapper(&dummy, OTHER_DOMAIN);

#if 0
		transaction_id++;
	}
	end = lcd_RDTSC_STOP();
#endif
	printk("Value from other domain %lx\n", fipc_get_reg1(&dummy));

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
