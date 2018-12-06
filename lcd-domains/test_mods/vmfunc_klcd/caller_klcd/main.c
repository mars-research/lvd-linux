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
#include "../vmfunc_trampoline.h"

#include <lcd_config/post_hook.h>

extern int vmfunc_wrapper(struct fipc_message *req);
unsigned long long stack;
unsigned long long eip;

static int caller_main(void)
{
	int ret;
	unsigned int transaction_id = 0;
	u64 start, end;
	/*
	 * Boot
	 */
	ret = lcd_enter();
	if (ret)
		goto out;

	start = lcd_RDTSC_START();
	while (transaction_id < TRANSACTIONS) {
#ifdef HOST_LINUX_IN_VM
		if (transaction_id == 0)
			vmfunc_wrapper(NULL);
#endif
		transaction_id++;
	}
	end = lcd_RDTSC_STOP();

	printk("%s, vmfunc (%p) took %llu cycles (num_transactions = %d) | vmfunc_load_addr %p | size %zx\n", __func__,
			vmfunc, (end - start) / TRANSACTIONS, TRANSACTIONS, vmfunc_load_addr, vmfunc_page_size);

	print_hex_dump(KERN_DEBUG, "vmfuncwrapper: ", DUMP_PREFIX_ADDRESS,
		       32, 1, vmfunc_wrapper, 0x100, false);
	/*
	 * Done; just exit (everything will be torn down when we die)
	 */

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
