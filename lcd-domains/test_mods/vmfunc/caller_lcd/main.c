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

static int __noreturn caller_main(void)
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
		transaction_id++;
		vmfunc(0, 0);
	}
	end = lcd_RDTSC_STOP();

	printk("%s, vmfunc (%p) took %llu cycles (num_transactions = %d) | vmfunc_load_addr %p | size %zx\n", __func__,
			vmfunc, (end - start) / TRANSACTIONS, TRANSACTIONS, vmfunc_load_addr, vmfunc_page_size);
	/*
	 * Done; just exit (everything will be torn down when we die)
	 */
	goto out;
	       
out:
	lcd_exit(ret);
}

static int vmfunc_caller_init(void)
{
	int ret;

	LCD_MAIN({

			ret = caller_main();

		});

	return ret;
}

static void __exit vmfunc_caller_exit(void)
{
	return;
}

module_init(vmfunc_caller_init);
module_exit(vmfunc_caller_exit);
MODULE_LICENSE("GPL");
