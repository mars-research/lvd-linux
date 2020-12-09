#ifndef _MARSHAL_CALLER_H_
#define _MARSHAL_CALLER_H_

void marshal_int(union acpi_object *obj, uint64_t *regs);
void marshal_string(union acpi_object *obj, uint64_t *regs);
void marshal_buffer(union acpi_object *obj, uint64_t *regs);
void marshal_package(union acpi_object *obj, uint64_t *regs);
void marshal_reference(union acpi_object *obj, uint64_t *regs);
void marshal_power_resource(union acpi_object *obj, uint64_t *regs);

uint64_t test_marshal_empty(void);
uint64_t test_marshal_int(void);
uint64_t test_marshal_array(size_t arr_sz);
uint64_t test_marshal_string(size_t str_sz);
uint64_t test_marshal_voidptr(size_t data_sz);
int test_marshal_union(void);



#endif	/* _MARSHAL_CALLER_H_ */
