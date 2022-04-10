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
#include "../vmfunc_trampoline.h"
#include <linux/module.h>

#include <lcd_config/post_hook.h>

extern int vmfunc_wrapper(struct fipc_message *req);

EXPORT_SYMBOL(vmfunc_sboard_load_addr);
EXPORT_SYMBOL(vmfunc_trampoline_load_addr);

unsigned long loops_per_jiffy;
unsigned long volatile jiffies;

int callee(struct fipc_message *msg2)
{
	int ret = 0;
	unsigned long transaction_id = 0;
	u64 start, end;
	struct fipc_message msg;

	INIT_FIPC_MSG(&msg);

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

extern unsigned long get_jiffies(void);
void __init_globals(void) {

	jiffies = get_jiffies();
}

int callee_main(void)
{
	int ret;
	struct ext_registers *reg_page = get_register_page(0);

	printk("%s, register page %p", __func__, reg_page);
	{
		int i;
		for (i = 0; i < PAGE_SIZE/64; i++) {
			printk("reg_page->regs[%d] = %llx", i, reg_page->regs[i]);
		}
	}
	/*
	 * Boot LCD
	 */
	ret = lcd_enter();
#if 0
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
#endif
	return ret;
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
