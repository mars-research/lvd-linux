#ifndef __MARSHAL_CALLEE_H_
#define __MARSHAL_CALLEE_H_

#include <libfipc.h>
#include "../rpc.h"

int marshal_int_callee(struct fipc_message *msg);
int marshal_array_callee(struct fipc_message *msg);
int marshal_string_callee(struct fipc_message *msg);
int marshal_voidptr_callee(struct fipc_message *msg);

void unmarshal_int(union acpi_object *obj, uint64_t *regs);
void unmarshal_string(union acpi_object *obj, uint64_t *regs);
void unmarshal_buffer(union acpi_object *obj, uint64_t *regs);
void unmarshal_package(union acpi_object *obj, uint64_t *regs);
void unmarshal_reference(union acpi_object *obj, uint64_t *regs);
void unmarshal_power_resource(union acpi_object *obj, uint64_t *regs);

#define resolve_acpi_object_type_callee(obj, regs)			\
	do { 								\
		switch (obj->type) {					\
		case ACPI_TYPE_INTEGER:					\
			unmarshal_int(obj, regs);			\
			break;						\
		case ACPI_TYPE_STRING:					\
			unmarshal_string(obj, regs);			\
			break;						\
		case ACPI_TYPE_BUFFER:					\
			unmarshal_buffer(obj, regs);			\
			break;						\
		case ACPI_TYPE_PACKAGE:					\
			unmarshal_package(obj, regs);			\
			break;						\
		case ACPI_TYPE_POWER:					\
			unmarshal_power_resource(obj, regs);		\
			break;						\
		default:						\
			printk("Unknown type: %02x\n", obj->type);	\
			break;						\
		}							\
	} while (0)

#endif /* __MARSHAL_CALLEE_H_ */
