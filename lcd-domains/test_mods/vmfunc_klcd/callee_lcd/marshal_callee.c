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

extern unsigned long volatile jiffies;

int marshal_none_callee(struct fipc_message *msg) {
	return 0;
}

int marshal_int_callee(struct fipc_message *msg) {

	int foo = fipc_get_reg0(msg);
	fipc_set_reg0(msg, foo + 1);
	return 0;
}

int *array;

int marshal_array_callee(struct fipc_message *msg) {
	int j;
	struct ext_registers *this_reg_page = get_register_page(smp_processor_id());
	uint64_t array_sz = fipc_get_reg0(msg);

	if (!array) {
		printk("%s, allocating an array of size %llu", __func__, array_sz);

		array = kmalloc(array_sz * sizeof(int), GFP_KERNEL);

		if (!array) {
			printk("array allocation failed\n");
			goto exit;
		}
	}

	for (j = 0; j < array_sz; j++) {
		array[j] = this_reg_page->regs[j];
	}

	fipc_set_reg0(msg, 0);
	return 0;
exit:
	fipc_set_reg0(msg, 0xbad);
	return 0;
}

char *string;

int marshal_string_callee(struct fipc_message *msg) {
	struct ext_registers *this_reg_page = get_register_page(smp_processor_id());
	uint64_t str_sz = fipc_get_reg0(msg) + 1; // to accomodate null character

	if (!string) {
		printk("%s, allocating a string of size %llu", __func__, str_sz);
		string = kmalloc(str_sz, GFP_KERNEL);

		if (!string) {
			printk("string allocation failed\n");
			goto exit;
		}
	}

	memcpy(string, &this_reg_page->regs[0], str_sz);

	fipc_set_reg0(msg, 0);
	return 0;
exit:
	fipc_set_reg0(msg, 0xbad);
	return 0;
}

void *ptr;
int marshal_voidptr_callee(struct fipc_message *msg) {
	struct ext_registers *this_reg_page = get_register_page(smp_processor_id());
	uint64_t data_sz = fipc_get_reg0(msg);

	if (!ptr) {
		printk("%s, allocating a buffer of size %llu", __func__, data_sz);
		ptr = kmalloc(data_sz, GFP_KERNEL);

		if (!ptr) {
			printk("allocation failed\n");
			goto exit;
		}
	}

	memcpy(ptr, &this_reg_page->regs[0], data_sz);

	fipc_set_reg0(msg, 0);
	return 0;
exit:
	fipc_set_reg0(msg, 0xbad);
	return 0;
}


union acpi_object *obj;

int marshal_union_callee(struct fipc_message *msg) {
	struct ext_registers *this_reg_page = get_register_page(smp_processor_id());
	uint64_t *regs = &this_reg_page->regs[0];
	int i = 0;

	if (!obj) {
		obj = kzalloc(sizeof(union acpi_object), GFP_KERNEL);

		if (!obj) {
			printk("allocation failed\n");
			goto exit;
		}
	}

	obj->type = regs[i++];

	resolve_acpi_object_type_callee(obj, &regs[1]);

	return 0;
exit:
	fipc_set_reg0(msg, 0xbad);
	return 0;
}


unsigned long get_jiffies(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;

	unsigned long ret = 0;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	async_msg_set_fn_type(msg, RPC_ID_get_jiffies);

	ret = fipc_get_reg0(msg);

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}
