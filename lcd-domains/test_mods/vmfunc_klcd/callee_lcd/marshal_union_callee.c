#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include "marshal_callee.h"

#include <lcd_config/post_hook.h>

void unmarshal_int(union acpi_object *obj, uint64_t *regs) {
	printk("%s, value: %llu\n", __func__, obj->integer.value);
}

void unmarshal_string(union acpi_object *obj, uint64_t *regs) {
	int i = 0, len;
	len = obj->string.length = regs[i++];
	memcpy(obj->string.pointer, &regs[i++], len);
}

void unmarshal_buffer(union acpi_object *obj, uint64_t *regs) {
	printk("%s, len: %d | ptr:%s\n", __func__, obj->buffer.length, obj->buffer.pointer);
}

void unmarshal_package(union acpi_object *obj, uint64_t *regs) {
	int i;
	union acpi_object *elements = obj->package.elements;
	printk("%s, count: %d | elements: %p\n", __func__, obj->package.count, obj->package.elements);
	for (i = 0; i < obj->package.count; i++) {
		union acpi_object *e = &elements[i];
		resolve_acpi_object_type_callee(e, regs);
	}
}

void unmarshal_reference(union acpi_object *obj, uint64_t *regs) {
	printk("Found reference pointer type!\n");
	printk("%s, actual_type: %02x handle:%p\n", __func__, obj->reference.actual_type, obj->reference.handle);
}

void unmarshal_power_resource(union acpi_object *obj, uint64_t *regs) {
	printk("%s, sys_lvl: %u ord:%u\n", __func__, obj->power_resource.system_level, obj->power_resource.resource_order);
}


