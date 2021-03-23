#include <lcd_config/pre_hook.h>

#include "../rdtsc_helper.h"
#include "../rpc.h"
#include <liblcd/boot_info.h>

#include <lcd_config/post_hook.h>

uint64_t test_marshal_empty(void) {
	int i;
	uint64_t start, end;

	start = rdtsc();

	for (i = 0; i < NUM_ITERATIONS; i++) {
		struct fipc_message m;
		struct fipc_message *msg = &m;
		INIT_FIPC_MSG(msg);

		async_msg_set_fn_type(msg, MARSHAL_INT);
		fipc_set_reg0(msg, 0xdeadbeef);

		vmfunc_klcd_wrapper(msg, OTHER_DOMAIN);
	}

	end = rdtsc();

	printk("%s: %d iterations of marshal int took %llu cycles (avg: %llu cycles)\n",
				__func__, NUM_ITERATIONS, end - start, (end - start) / NUM_ITERATIONS);
	return end - start;
}

uint64_t test_marshal_int(void) {
	int i;
	uint64_t start, end;

	start = rdtsc();

	for (i = 0; i < NUM_ITERATIONS; i++) {
		struct fipc_message m;
		struct fipc_message *msg = &m;
		INIT_FIPC_MSG(msg);

		async_msg_set_fn_type(msg, MARSHAL_INT);
		fipc_set_reg0(msg, 0xdeadbeef);

		vmfunc_klcd_wrapper(msg, OTHER_DOMAIN);
	}

	end = rdtsc();

	printk("%s: %d iterations of marshal int took %llu cycles (avg: %llu cycles)\n",
				__func__, NUM_ITERATIONS, end - start, (end - start) / NUM_ITERATIONS);
	return end - start;
}

uint64_t test_marshal_array(size_t arr_sz) {
	int i, j;
	int *array;
	uint64_t start, end;

	array = kmalloc(arr_sz * sizeof(int), GFP_KERNEL);

	if (!array) {
		printk("array allocation failed\n");
		goto exit;
	}

	start = rdtsc();

	for (i = 0; i < NUM_ITERATIONS; i++) {
		struct fipc_message m;
		struct fipc_message *msg = &m;
		struct ext_registers *this_reg_page = get_register_page(smp_processor_id());

		INIT_FIPC_MSG(msg);

		async_msg_set_fn_type(msg, MARSHAL_ARRAY);
		// array of 32 integers
		fipc_set_reg0(msg, arr_sz);

		for (j = 0; j < arr_sz; j++) {
			this_reg_page->regs[j] = array[j];
		}

		vmfunc_klcd_wrapper(msg, OTHER_DOMAIN);
		if (fipc_get_reg0(msg) == 0xbad) {
			break;
		}
	}

	end = rdtsc();

	printk("%s: %d iterations of marshal array[%zu] took %llu cycles (avg: %llu cycles)\n",
				__func__, i, arr_sz, end - start, (end - start) / i);

	return end - start;
exit:
	return -1;
}

uint64_t test_marshal_string(size_t str_sz) {
	int i;
	char *string;
	uint64_t start, end;

	string = kmalloc(str_sz, GFP_KERNEL);

	if (!string) {
		printk("string allocation failed\n");
		goto exit;
	}
	memset(string, 'a', str_sz);
	string[str_sz - 1] = '\0';

	start = rdtsc();

	for (i = 0; i < NUM_ITERATIONS; i++) {
		struct fipc_message m;
		struct fipc_message *msg = &m;
		struct ext_registers *this_reg_page = get_register_page(smp_processor_id());

		INIT_FIPC_MSG(msg);

		async_msg_set_fn_type(msg, MARSHAL_ARRAY);
		// string
		fipc_set_reg0(msg, strlen(string));

		memcpy(&this_reg_page->regs[0], string, strlen(string));

		vmfunc_klcd_wrapper(msg, OTHER_DOMAIN);

		if (fipc_get_reg0(msg) == 0xbad) {
			break;
		}
	}

	end = rdtsc();

	printk("%s: %d iterations of marshal string (len: %zu) took %llu cycles (avg: %llu cycles)\n",
				__func__, i, strlen(string), end - start, (end - start) / i);

	return end - start;
exit:
	return -1;
}

uint64_t test_marshal_voidptr(size_t data_sz) {
	int i;
	void *p;
	uint64_t start, end;

	p = kmalloc(data_sz, GFP_KERNEL);

	if (!p) {
		printk("allocation failed\n");
		goto exit;
	}

	start = rdtsc();

	for (i = 0; i < NUM_ITERATIONS; i++) {
		struct fipc_message m;
		struct fipc_message *msg = &m;
		struct ext_registers *this_reg_page = get_register_page(smp_processor_id());

		INIT_FIPC_MSG(msg);

		async_msg_set_fn_type(msg, MARSHAL_VOIDP);
		
		fipc_set_reg0(msg, data_sz);

		memcpy(&this_reg_page->regs[0], p, data_sz);

		vmfunc_klcd_wrapper(msg, OTHER_DOMAIN);

		if (fipc_get_reg0(msg) == 0xbad) {
			break;
		}
	}

	end = rdtsc();

	printk("%s: %d iterations of marshal void ptr (sz=%zu) took %llu cycles (avg: %llu cycles)\n",
				__func__, i, data_sz, end - start, (end - start) / i);

	return end - start;
exit:
	return -1;
}
