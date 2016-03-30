#ifndef UTILS_H
#define UTILS_H

const char* new_name(const char* name, const char* suffix);
const char* struct_name(const char* name);
const char* lookup_name(const char* name);
const char* exit_name(const char* name);
const char* init_name(const char* name);
const char* container_name(const char* name);
const char* insert_name(const char* name);
const char* cap_init_name(const char* name);
const char* cap_create_name(const char* name);
const char* cap_exit_name(const char* name);
const char* cspace_name(const char* name);
const char* group_name(const char* name);
const char* glue_name(const char* name);
const char* hidden_args_name(const char* name);
const char* parameter_name(const char* name);
const char* trampoline_func_name(const char* name);
const char* string_to_upper(const char* str);
const char* append_strings(const char* delimiter, std::vector<const char*> strs);
bool dealloc_caller(Variable *v, const char* side);
bool dealloc_callee(Variable *v, const char* side);
bool in(Variable *v, const char* side);
bool out(Variable *v, const char* side);


#endif
