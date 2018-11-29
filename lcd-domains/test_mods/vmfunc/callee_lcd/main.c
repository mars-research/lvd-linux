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
#include "../vmfunc_trampoline.h"
#include <linux/module.h>

#include <lcd_config/post_hook.h>

extern void* __vmfunc_load_addr;

/*
 * We use "noinline" because we want the function call to actually
 * happen.
 */

static unsigned long noinline 
null_invocation(void)
{
	return 9;
}

static unsigned long noinline
add_constant(unsigned long trans)
{
	return trans + 50;
}

static unsigned long noinline 
add_nums(unsigned long trans, unsigned long res1)
{
	return trans+res1;
}

int callee(struct fipc_ring_channel *chan)
{
	int ret = 0;
	unsigned long transaction_id = 0;

	for (transaction_id = 0; 
	     transaction_id < TRANSACTIONS; 
	     transaction_id++) {
		vmfunc(0, 0);
		if (!transaction_id)
			printk("%s, vmfunc(%p) | vmfunc_load_addr %p\n", __func__, vmfunc, __vmfunc_load_addr);
		null_invocation();
		add_constant(transaction_id);
		add_nums(transaction_id, 50);
	}

	LIBLCD_MSG("Callee is done");
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
