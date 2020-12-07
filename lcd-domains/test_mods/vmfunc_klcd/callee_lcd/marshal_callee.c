#include <lcd_config/pre_hook.h>

#include <linux/slab.h>
#include <liblcd/liblcd.h>
#include "../rpc.h"
#include "../rdtsc_helper.h"
#include <linux/module.h>
#include <linux/kallsyms.h>
#include <asm/processor.h>
#include <asm/desc.h>
#include <asm/lcd_domains/bflank.h>
#include "marshal_callee.h"

#include <lcd_config/post_hook.h>

int marshal_int_callee(struct fipc_message *msg) {

	int foo = fipc_get_reg0(msg);
	fipc_set_reg0(msg, foo + 1);
	return 0;
}

int *array;

int marshal_array_callee(struct fipc_message *msg) {
	int j;
	struct ext_registers *this_reg_page = get_register_page(smp_processor_id());
	int array_sz = fipc_get_reg0(msg);

	if (!array) {
		array = kmalloc(array_sz * sizeof(int), GFP_KERNEL);

		if (!array) {
			printk("array allocation failed\n");
			goto exit;
		}
	}

	for (j = 0; j < array_sz; j++) {
		array[j] = this_reg_page->regs[j];
	}
exit:
	return 0;
}

char *string;

int marshal_string_callee(struct fipc_message *msg) {
	struct ext_registers *this_reg_page = get_register_page(smp_processor_id());
	int str_sz = fipc_get_reg0(msg) + 1; // to accomodate null character

	if (!string) {
		string = kmalloc(str_sz, GFP_KERNEL);

		if (!string) {
			printk("string allocation failed\n");
			goto exit;
		}
	}

	memcpy(string, &this_reg_page->regs[0], str_sz);
exit:
	return 0;
}

void *ptr;
int marshal_voidptr_callee(struct fipc_message *msg) {
	struct ext_registers *this_reg_page = get_register_page(smp_processor_id());
	size_t data_sz = fipc_get_reg0(msg);

	if (!ptr) {
		ptr = kmalloc(data_sz, GFP_KERNEL);

		if (!ptr) {
			printk("allocation failed\n");
			goto exit;
		}
	}

	memcpy(ptr, &this_reg_page->regs[0], data_sz);
exit:
	return 0;
}
