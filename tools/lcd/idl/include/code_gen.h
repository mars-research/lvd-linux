#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "lcd_ast.h"
#include "ccst.h"

#define CONTAINER_OF "container_of"

// server.cpp
CCSTFile* generate_server_source(Module *m); // todo complete
CCSTFile* generate_server_header(Module *m); // todo complete?

CCSTCompoundStatement* callee_body(Rpc *r); // todo complete  2 functions in the file.....

CCSTDeclaration* callee_declaration(Rpc *r); // todo complete

CCSTCompoundStatement* dispatch_loop_body(std::vector<Rpc*> rpcs); //todo complete... revamp
CCSTDeclaration* dispatch_function_declaration(); // todo complete

// client.cpp

CCSTFile* generate_client_header(Module *m); // todo empty, maybe unnecessary?
CCSTFile* generate_client_source(Module *m); // todo complete

CCSTCompoundStatement* caller_body(Rpc *r); // todo complete

std::vector<CCSTStatement*> marshal_in_parameters(std::vector<Parameter*> params); // complete

std::vector<CCSTStatement*> marshal_parameters(std::vector<Parameter*> params); // todo i think this belongs in a different file
std::vector<CCSTStatement*> unmarshal_implicit_return(std::vector<Parameter*> implicit_returns); // todo does this belong in a different file?
CCSTStatement* unmarshal_explicit_return(Marshal_type *return_info); // todo does this belong in a different file?

const char* struct_name(ProjectionType *pt); // complete
// client seems to be missing important functions.



// helper.cpp
CCSTCompoundStatement* alloc_init_containers_driver(ProjectionType *pt, LexicalScope *ls, const char* side); // complete
CCSTCompoundStatement* alloc_init_containers(ProjectionType *pf, LexicalScope *ls, const char* side); // complete
bool alloc_caller(Variable *v, const char* side); // complete
CCSTStructUnionSpecifier* struct_declaration(ProjectionType *pt); // complete
CCSTDeclaration* declare_and_initialize_container_struct(Variable *v); // complete
std::vector<CCSTAssignExpr*> container_of_args(CCSTPostFixExpr *struct_pointer, const char *type_name, const char *field_name); // complete

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
std::vector<CCSTDecSpecifier*> int_type(); // complete

// name creators
const char* parameter_name(const char *name); // complete
const char* container_name(const char *name); // complete
const char* enum_name(const char *name); // todo totally incomplete
const char* string_to_upper(const char *str); //???????? const or not? this function seems retarded.

CCSTPointer* pointer(int p_count); // complete

std::vector<CCSTDecSpecifier*> struct_type(const char *type_name); // complete
std::vector<CCSTDecSpecifier*> type2(Type *t); // complete?
std::vector<CCSTSpecifierQual*> type(Type *t); // complete?
CCSTTypeName* type_cast(Type *t); //??????
std::vector<CCSTSpecifierQual*> integer_type_cast(IntegerType *it); /// ????????


// is this in the correct file?
CCSTCompoundStatement* interface_init_function_body(std::vector<GlobalVariable*> globals); // complete


CCSTFuncDef* function_definition(CCSTDeclaration *function_declaration, CCSTCompoundStatement *body); // complete
CCSTDeclaration* function_declaration(Rpc *r); // complete

CCSTParamTypeList* parameter_list(std::vector<Parameter*> params); // complete


// needed functions.
// creates a function declaration from a return type, a name and parameters

// add marshal and unmarshal cpp files to this


// unmarshal.cpp
CCSTStatement* unmarshal_variable(Variable *v); // almost complete

// marshal.cpp
CCSTStatement* marshal_variable(Variable *v); // empty


#endif
