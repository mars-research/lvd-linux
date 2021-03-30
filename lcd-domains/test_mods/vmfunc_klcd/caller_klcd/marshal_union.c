#include <lcd_config/pre_hook.h>

#include "../rdtsc_helper.h"
#include "../rpc.h"
#include <liblcd/boot_info.h>
#include "marshal_caller.h"

#include <lcd_config/post_hook.h>

#define resolve_acpi_object_type(obj, regs)				\
	do { 								\
		switch (obj->type) {					\
		case ACPI_TYPE_INTEGER:					\
			marshal_int(obj, regs);				\
			break;						\
		case ACPI_TYPE_STRING:					\
			marshal_string(obj, regs);			\
			break;						\
		case ACPI_TYPE_BUFFER:					\
			marshal_buffer(obj, regs);			\
			break;						\
		case ACPI_TYPE_PACKAGE:					\
			marshal_package(obj, regs);			\
			break;						\
		case ACPI_TYPE_POWER:					\
			marshal_power_resource(obj, regs);		\
			break;						\
		default:						\
			printk("Unknown type: %02x\n", obj->type);	\
			break;						\
		}							\
	} while (0)



void marshal_int(union acpi_object *obj, uint64_t *regs) {
	printk("%s, value: %llu\n", __func__, obj->integer.value);
}

void marshal_string(union acpi_object *obj, uint64_t *regs) {
	int i = 0, len;
	regs[i++] = obj->type;
	regs[i++] = len = strlen(obj->string.pointer);
	memcpy(&regs[i++], obj->string.pointer, len);
}

void marshal_buffer(union acpi_object *obj, uint64_t *regs) {
	printk("%s, len: %d | ptr:%s\n", __func__, obj->buffer.length, obj->buffer.pointer);
}

void marshal_package(union acpi_object *obj, uint64_t *regs) {
	int i;
	union acpi_object *elements = obj->package.elements;
	printk("%s, count: %d | elements: %p\n", __func__, obj->package.count, obj->package.elements);
	for (i = 0; i < obj->package.count; i++) {
		union acpi_object *e = &elements[i];
		resolve_acpi_object_type(e, regs);
	}
}

void marshal_reference(union acpi_object *obj, uint64_t *regs) {
	printk("Found reference pointer type!\n");
	printk("%s, actual_type: %02x handle:%p\n", __func__, obj->reference.actual_type, obj->reference.handle);
}

void marshal_power_resource(union acpi_object *obj, uint64_t *regs) {
	printk("%s, sys_lvl: %u ord:%u\n", __func__, obj->power_resource.system_level, obj->power_resource.resource_order);
}

int test_marshal_union(void)
{
	int i;
	uint64_t start, end;
	union acpi_object *obj = (union acpi_object*) kzalloc(sizeof(union acpi_object), GFP_KERNEL);

	obj->type = ACPI_TYPE_STRING;
	obj->string.length = 32;
	obj->string.pointer = kmalloc(32 + 1, GFP_KERNEL);

	memset(obj->string.pointer, 'b', 32);
	obj->string.pointer[32] = '\0';
	
	start = rdtsc();

	for (i = 0; i < NUM_ITERATIONS; i++) {
		struct fipc_message m;
		struct fipc_message *msg = &m;
		struct ext_registers *this_reg_page = get_register_page(smp_processor_id());
		uint64_t *regs = &this_reg_page->regs[0];

		INIT_FIPC_MSG(msg);

		async_msg_set_fn_type(msg, MARSHAL_UNION);

		resolve_acpi_object_type(obj, regs);

		vmfunc_klcd_wrapper(msg, OTHER_DOMAIN);
	}

	end = rdtsc();

	printk("%s: %d iterations of marshal union took %llu cycles (avg: %llu cycles)\n",
				__func__, NUM_ITERATIONS, end - start, (end - start) / NUM_ITERATIONS);
	return end - start;
}
