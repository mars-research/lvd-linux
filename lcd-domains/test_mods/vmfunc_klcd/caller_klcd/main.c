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

static int caller_main(void)
{
	int ret = 0;
//	unsigned int transaction_id = 0;
	u64 start, end;
	struct fipc_message dummy;
	dummy.vmfunc_id = VMFUNC_RPC_CALL;
	dummy.rpc_id = NULL_INVOCATION;	
	fipc_set_reg1(&dummy, 0xabcd);
	/*
	 * Boot
	 */
	printk("%s entered\n", __func__);
	//vmfunc_init(lcd_stack);
	//ret = lcd_enter();
	if (ret)
		goto out;

	start = lcd_RDTSC_START();
//	while (transaction_id < TRANSACTIONS) {
//		if (transaction_id == 0)
//			vmfunc_wrapper(&dummy);
//
//		transaction_id++;
//	}
	asm volatile ("xor %ecx, %ecx \n\t"
			"xor %eax, %eax \n\t"
			"vmfunc \n\t");
	end = lcd_RDTSC_STOP();

	printk("Value from other domain %lx\n", fipc_get_reg1(&dummy));

	printk("%s, vmfunc (%p) took %llu cycles (num_transactions = %d) | vmfunc_load_addr %p | size %zx\n", __func__,
			vmfunc_call, (end - start) / TRANSACTIONS, TRANSACTIONS, vmfunc_load_addr, vmfunc_page_size);

	print_hex_dump(KERN_DEBUG, "vmfuncwrapper: ", DUMP_PREFIX_ADDRESS,
		       32, 1, vmfunc_wrapper, 0x100, false);
	/*
	 * Done; just exit (everything will be torn down when we die)
	 */

	return ret;
out:
	lcd_exit(ret);
	return ret;
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
