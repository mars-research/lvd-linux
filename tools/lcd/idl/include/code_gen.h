#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "lcd_ast.h"
#include "ccst.h"
#include "utils.h"

#define CONTAINER_OF "container_of"

// server.cpp
CCSTFile* generate_server_source(Module *m); // todo complete
CCSTFile* generate_server_header(Module *m); // todo complete?

CCSTCompoundStatement* callee_body(Rpc *r, Module *m); // todo complete  2 functions in the file.....

CCSTDeclaration* callee_declaration(Rpc *r); // todo complete

CCSTCompoundStatement* dispatch_loop_body(std::vector<Rpc*> rpcs); //todo complete... revamp
CCSTDeclaration* dispatch_function_declaration(); // todo complete

// client.cpp

CCSTFile* generate_client_header(Module *m); // todo empty, maybe unnecessary?
CCSTFile* generate_client_source(Module *m); // todo complete

CCSTCompoundStatement* caller_body(Rpc *r, Module *m); // todo complete

std::vector<CCSTStatement*> marshal_in_parameters(std::vector<Parameter*> params); // complete

std::vector<CCSTStatement*> marshal_parameters(std::vector<Parameter*> params); // todo i think this belongs in a different file
std::vector<CCSTStatement*> unmarshal_implicit_return(std::vector<Parameter*> implicit_returns); // todo does this belong in a different file?
CCSTStatement* unmarshal_explicit_return(Marshal_type *return_info); // todo does this belong in a different file?

const char* struct_name(ProjectionType *pt); // complete
// client seems to be missing important functions.



// helper.cpp
std::vector<const char*> construct_list_vars(Variable *v); // complete
CCSTUnaryOp* reference();
char* type_number_to_name(int num);
CCSTDeclaration* struct_pointer_declaration(const char* struct_name, const char* var_name, LexicalScope *ls); // complete
CCSTStatement* kzalloc_structure(const char*struct_name, const char* var_name); // complete
CCSTCompoundStatement* alloc_init_containers_driver(Variable *v, ProjectionType *pt, LexicalScope *ls, const char* side); // complete
CCSTCompoundStatement* declare_and_initialize_container_struct(Variable *v, ProjectionType *pt, LexicalScope *ls, const char* side); // complete
CCSTCompoundStatement* alloc_init_containers(Variable *v, ProjectionType *pt, LexicalScope *ls, const char* side); // complete


bool contains_function_pointer(ProjectionType *pt); //complete
bool alloc_callee(Variable *v, const char* side); // complete 
bool alloc_caller(Variable *v, const char* side); // complete
CCSTStructUnionSpecifier* struct_declaration(ProjectionType *pt); // complete
CCSTDeclaration* declare_and_initialize_container_struct(Variable *v); // complete



CCSTDeclaration* typedef_declaration(Typedef *t); // todo. totally not done
CCSTAssignOp* equals(); // complete
CCSTDeclaration* declare_static_variable(Variable *v); // complete

CCSTExDeclaration* construct_enum(Module *m); // todo?
CCSTEnumeratorList* construct_enumlist(std::vector<Rpc*> rpcs); // ???????

CCSTUnaryOp* indirection(); // complete
CCSTUnaryExprCastExpr* dereference(CCSTCastExpr *to_deref); //complete
CCSTPostFixExpr* access(Variable *v); // complete to test.
CCSTPrimaryExprId* function_name(const char *func_name); // complete
CCSTPostFixExprAssnExpr* function_call(const char* func_name, std::vector<CCSTAssignExpr*> args); // complete
CCSTUnaryOp* Not();
CCSTIfStatement* if_cond_fail(CCSTExpression *cond, const char *err_msg); // complete
CCSTIfStatement* if_cond_fail_goto(CCSTExpression *cond, const char* err_msg, const char* goto_label); // complete
std::vector<CCSTDecSpecifier*> int_type(); // complete

CCSTPointer* pointer(int p_count); // complete

std::vector<CCSTDecSpecifier*> struct_type(const char *type_name); // complete
std::vector<CCSTDecSpecifier*> type2(Type *t); // complete?
std::vector<CCSTSpecifierQual*> type(Type *t); // complete?
CCSTTypeName* type_cast(Type *t); //??????
std::vector<CCSTSpecifierQual*> integer_type_cast(IntegerType *it); /// ????????


// is this in the correct file?
CCSTCompoundStatement* caller_interface_init_function_body(Module *m); // complete
CCSTCompoundStatement* callee_interface_init_function_body(Module *m); // complete
CCSTDeclaration* interface_init_function_declaration(Module *m); // complete
CCSTCompoundStatement* caller_interface_exit_function_body(Module *m); // complete
CCSTCompoundStatement* callee_interface_exit_function_body(Module *m); // complete
CCSTDeclaration* interface_exit_function_declaration(Module *m); // complete


CCSTFuncDef* function_definition(CCSTDeclaration *function_declaration, CCSTCompoundStatement *body); // complete
CCSTDeclaration* function_declaration(Rpc *r); // complete
CCSTDeclaration* function_pointer_function_declaration(Rpc *r);

CCSTParamTypeList* parameter_list(std::vector<Parameter*> params); // complete



// needed functions.
// creates a function declaration from a return type, a name and parameters

// add marshal and unmarshal cpp files to this

// unmarshal.cpp
CCSTPostFixExprAssnExpr* unmarshal_variable(Variable *v); 
std::vector<CCSTStatement*> unmarshal_variable_no_check(Variable *v); // complete
std::vector<CCSTStatement*> unmarshal_variable_callee(Variable *v);
std::vector<CCSTStatement*> unmarshal_variable_caller(Variable *v);
std::vector<CCSTStatement*> unmarshal_container_refs_caller(Variable *v);

// marshal.cpp
CCSTStatement* marshal_variable(Variable *v, const char* direction); // complete
std::vector<CCSTStatement*> marshal_variable_callee(Variable *v); // complete
std::vector<CCSTStatement*> marshal_variable_no_check(Variable *v); // complete
CCSTStatement* marshal(CCSTPostFixExpr *v, int reg); // complete

// containers.cpp
std::vector<CCSTAssignExpr*> container_of_args(CCSTPostFixExpr *struct_pointer, const char *type_name, const char *field_name); // complete
std::vector<CCSTStatement*> container_of(Variable *v, const char* cspace); // complete
CCSTCompoundStatement* set_remote_ref(Variable *v); // complete
CCSTCompoundStatement* allocate_and_link_containers_callee(Variable *v, const char* cspace);
CCSTStatement* lookup_variable_container(Variable *v);
CCSTStatement* alloc_variable(Variable *v); // complete
CCSTStatement* allocate_non_container_variables(Variable *v);
CCSTCompoundStatement* alloc_insert_variable_container(Variable *v, const char* cspace);
ProjectionField* get_cptr_field(Variable *v);
ProjectionField* find_field(ProjectionType *pt, const char *field_name);
CCSTCompoundStatement* alloc_link_container_caller(Variable *v, const char* cspace);
std::vector<CCSTStatement*> caller_allocate_channels(ProjectionType *pt);
std::vector<CCSTStatement*> caller_initialize_channels(ProjectionType *pt);
std::vector<CCSTStatement*> dealloc_containers(Variable *v, const char* cspace);
std::vector<CCSTStatement*> dealloc_containers_callee(Variable *v, const char* cspace, LexicalScope *ls);
std::vector<CCSTStatement*> dealloc_containers_caller(Variable *v, const char* cspace, LexicalScope *ls);


// variable_utils.cpp
CCSTDeclaration* declare_variable(Variable *v);
std::vector<CCSTDeclaration*> declare_variables_callee(Variable *v);
std::vector<CCSTDeclaration*> declare_containers_only_callee(Variable *v);






// trampoline.cpp
CCSTCompoundStatement* trampoline_function_body(Rpc *r); // complete.
CCSTDeclaration* trampoline_function_declaration(Rpc *r); // has a bug +.
CCSTCompoundStatement* alloc_init_hidden_args_struct(ProjectionType *pt, Variable *v, LexicalScope *ls, Variable *cspace); // complete
std::vector<CCSTDeclaration*> declare_hidden_args_structures(ProjectionType *pt, LexicalScope *ls); // complete

// end trampoline.cpp

#endif
