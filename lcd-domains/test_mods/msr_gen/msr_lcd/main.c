/*
 * main.c - runs when dummy lcd boots
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <asm/lcd_domains/libvmfunc.h>

#include "../common.h"

#include <lcd_config/post_hook.h>

struct thc_channel *msr_async;
struct glue_cspace *msr_cspace;
cptr_t msr_sync_endpoint;

int msr_init(void);
void msr_exit(void);

int handle_rpc_calls(struct fipc_message* msg)
{
	const size_t id = msg->rpc_id;
	if (!try_dispatch(id, msg, get_register_page(smp_processor_id()))) {
		glue_user_panic("Couldn't dispatch on LCD side");
	}

	return 0;
}

static int module_lcd_init(void)
{
	int ret = 0;

	printk("LCD enter: current:%p \n", current);
	ret = lcd_enter();
	if (ret)
		goto fail1;

	glue_user_init();

	ret = msr_init();

	return ret;
fail1:
	lcd_exit(ret);
}

int __module_lcd_init(void)
{
	return module_lcd_init();
}

static void module_lcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);

	msr_exit();

	lvd_exit(0);
	return;
}

void __module_lcd_exit(void)
{
	module_lcd_exit();
}

module_init(__module_lcd_init);
module_exit(__module_lcd_exit);
MODULE_LICENSE("GPL");
MODULE_INFO(livepatch, "Y");

/* extract data from linker variables */
size_t vmfunc_sboard_page_size = (size_t)&__vmfunc_sboard_page_size;
unsigned long* vmfunc_sboard_load_addr = (unsigned long*) &__vmfunc_sboard_load_addr;

/* extract data from linker variables */
size_t vmfunc_trampoline_page_size = (size_t)&__vmfunc_trampoline_page_size;
unsigned long* vmfunc_trampoline_load_addr = (unsigned long*) &__vmfunc_trampoline_load_addr;

EXPORT_SYMBOL(vmfunc_sboard_load_addr);
EXPORT_SYMBOL(vmfunc_trampoline_load_addr);

