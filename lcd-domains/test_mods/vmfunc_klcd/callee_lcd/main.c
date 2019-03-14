/*
 * callee.c
 *
 * Code for the "callee side" of the channel
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <linux/slab.h>
#include <liblcd/liblcd.h>
#include "../rpc.h"
#include "../rdtsc_helper.h"
#include <linux/module.h>

#include <lcd_config/post_hook.h>

extern int vmfunc_wrapper(struct fipc_message *req);
extern int vmfunc_call(struct fipc_message *msg);

int callee(struct fipc_message *msg2)
{
	int ret = 0;
	unsigned long transaction_id = 0;
	u64 start, end;
	struct fipc_message msg;

	start = lcd_RDTSC_START();
	for (transaction_id = 0; 
	     transaction_id < TRANSACTIONS; 
	     transaction_id++) {
		vmfunc_wrapper(&msg);
	}

	end = lcd_RDTSC_STOP();

//	printk("%s, vmfunc took %llu cycles (num_transactions = %d)\n",
//			__func__,
//			(end - start) / TRANSACTIONS,
//			TRANSACTIONS);
	return ret;
}

static int __noreturn callee_main(void)
{
	int ret;
	/*
	 * Boot LCD
	 */
	ret = lcd_enter();
	if (ret)
		goto out;

	ret = callee(NULL);
	if (ret)
		goto out;
	/*
	 * Done (everything is just torn down when we die; we could write
	 * tear down code, but there's no point, and just adds irrelevant
	 * complexity)
	 */
	goto out;

out:
	lcd_exit(ret);
}

static int vmfunc_callee_init(void)
{
	int ret = 0;

	LCD_MAIN({

			ret = callee_main();

		});

	return ret;
}

static void __exit vmfunc_callee_exit(void)
{
	return;
}

module_init(vmfunc_callee_init);
module_exit(vmfunc_callee_exit);
MODULE_LICENSE("GPL");
MODULE_INFO(livepatch, "Y");
