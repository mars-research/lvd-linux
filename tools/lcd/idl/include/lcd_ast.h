#ifndef LCD_AST_H
#define LCD_AST_H

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <stdio.h>
#include "marshal_op.h"
#include "symbol_table.h"
#include "ccst.h"

class MarshalPrepareVisitor;
class CCSTStatement;
class CCSTTypeName;
class Marshal_type;
class TypeVisitor;
class MarshalVisitor;
class TypeNameVisitor;
class AllocateTypeVisitor;
class Variable;
class Type;
class Parameter;
class Rpc;
class ProjectionType;

enum PrimType {pt_char_t, pt_short_t, pt_int_t, pt_long_t, pt_longlong_t, pt_capability_t};
enum type_k {};

template<class T, class T2>
class ASTVisitor;

const char* new_name(const char* name, const char* suffix);
const char* container_name(const char* name);
const char* hidden_args_name(const char* name);

class Base
{			  
 public:
};

class LexicalScope : public Base
{
  LexicalScope *outer_scope_;
  std::map<std::string, Type*> type_definitions_;
  std::map<std::pair<std::string, std::vector<Parameter*> >, Rpc*> rpc_definitions_; // rpc or function pointer. why do we keep this? 
  std::vector<LexicalScope*> inner_scopes_;
 public:
  LexicalScope();
  LexicalScope(LexicalScope *outer_scope);
  virtual std::vector<Rpc*> rpc_in_scope();
  virtual bool insert(Rpc *r);
  virtual Type* lookup(const char *sym, int* err);
  virtual bool insert(const char *sym, Type* type);
  virtual bool contains(const char *symbol);
  virtual void set_outer_scope(LexicalScope *ls);
  virtual void add_inner_scope(LexicalScope *ls);
  virtual void add_inner_scopes(std::vector<LexicalScope*> scopes);
  virtual std::map<std::string, Type*> type_definitions();
  virtual std::vector<LexicalScope*> inner_scopes();
  virtual LexicalScope* outer_scope();
  virtual void resolve_types();
  virtual void create_trampoline_structs();
  virtual std::vector<Rpc*> function_pointer_to_rpc();
  virtual std::map<std::string, Type*> all_type_definitions();
  virtual std::map<std::string, Type*> all_types_outer();
  virtual std::map<std::string, Type*> all_types_inner();
};

class GlobalScope : public LexicalScope
{
  static GlobalScope *instance_;
  LexicalScope *outer_scope_;
  std::map<std::string, Type*> type_definitions_;
  std::map<std::pair<std::string, std::vector<Parameter*> >, Rpc*> rpc_definitions_; // rpc or function pointer
  std::vector<LexicalScope*> inner_scopes_;
 public:
  GlobalScope();
  virtual std::vector<Rpc*> rpc_in_scope();
  virtual bool insert(Rpc *r); // x
  virtual Type* lookup(const char *symbol, int *err); //x
  virtual bool insert(const char *symbol, Type *type); // x
  virtual bool contains(const char *symbol); // x
  virtual void set_outer_scope(LexicalScope *ls); // x
  virtual void add_inner_scope(LexicalScope *ls); // x
  virtual void add_inner_scopes(std::vector<LexicalScope*> scopes); // x
  virtual std::map<std::string, Type*> type_definitions(); // x
  virtual std::vector<LexicalScope*> inner_scopes(); // x
  static GlobalScope* instance(); // x
  virtual LexicalScope* outer_scope();
  virtual void resolve_types();
  virtual void create_trampoline_structs();
  virtual std::vector<Rpc*> function_pointer_to_rpc();
  virtual std::map<std::string, Type*> all_type_definitions();
  virtual std::map<std::string, Type*> all_types_outer();
  virtual std::map<std::string, Type*> all_types_inner();
};

class Type : public Base
{
 public:
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker) = 0;
  virtual CCSTTypeName* accept(TypeNameVisitor *worker) = 0;
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v) = 0;
  virtual int num() = 0;
  virtual const char* name() = 0;
  virtual void resolve_types(LexicalScope *ls) = 0;
  virtual void create_trampoline_structs(LexicalScope *ls) = 0;
};

class UnresolvedType : public Type
{
  const char *type_name_;
 public:
  UnresolvedType(const char* type_name);
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker); // need to add unresolved type to these visitors.
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual int num();
  virtual const char* name();
  virtual void resolve_types(LexicalScope *ls);
  virtual void create_trampoline_structs(LexicalScope *ls);
};

class Variable : public Base
{
 public:
  virtual Type* type() = 0;
  virtual const char* identifier() = 0;
  virtual void set_accessor(Variable *v) = 0;
  virtual Variable* accessor() = 0;
  virtual void set_marshal_info(Marshal_type *mt) = 0;
  virtual Marshal_type* marshal_info() = 0;
  virtual int pointer_count() = 0;
  virtual void prepare_marshal(MarshalPrepareVisitor *worker) = 0;
  virtual void resolve_types(LexicalScope *ls) = 0;

  virtual void set_in(bool b) = 0;
  virtual void set_out(bool b) = 0;
  virtual void set_alloc_caller(bool b) = 0;
  virtual void set_alloc_callee(bool b) = 0;
  virtual void set_dealloc_caller(bool b) = 0;
  virtual void set_dealloc_callee(bool b) = 0;

  virtual bool in() = 0;
  virtual bool out() = 0;
  virtual bool alloc_caller() = 0;
  virtual bool alloc_callee() = 0;
  virtual bool dealloc_caller() = 0;
  virtual bool dealloc_callee() = 0;
};

class GlobalVariable : public Variable
{
  Type *type_;
  const char *id_;
  int pointer_count_;
  Marshal_type *marshal_info_;

 public:
  GlobalVariable(Type *type, const char *id, int pointer_count);
  virtual void prepare_marshal(MarshalPrepareVisitor *worker);
  virtual void resolve_types(LexicalScope *ls);
  virtual Type* type();
  virtual const char* identifier();
  virtual void set_accessor(Variable *v);
  virtual Variable* accessor();
  virtual void set_marshal_info(Marshal_type *mt);
  virtual Marshal_type* marshal_info();
  virtual int pointer_count();
  

  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_dealloc_caller(bool b);
  virtual void set_dealloc_callee(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool dealloc_caller();
  virtual bool dealloc_callee();

};

class Parameter : public Variable
{
  bool in_;
  bool out_;
  bool alloc_callee_;
  bool alloc_caller_;
  bool dealloc_callee_;
  bool dealloc_caller_;

  Type* type_;
  const char* name_;
  Marshal_type *marshal_info_;
  Variable *accessor_;
  int pointer_count_;

 public:
  Parameter();
  Parameter(Type* type, const char* name, int pointer_count);
  ~Parameter();
  virtual void prepare_marshal(MarshalPrepareVisitor *worker);
  virtual void resolve_types(LexicalScope *ls);
  virtual Type* type();
  virtual void set_marshal_info(Marshal_type* mt);
  virtual Marshal_type* marshal_info(); 
  virtual const char* identifier();
  virtual void set_accessor(Variable *v);
  virtual Variable* accessor();
  virtual int pointer_count();
  
  
  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_dealloc_caller(bool b);
  virtual void set_dealloc_callee(bool b);  

  virtual bool in();
  virtual bool out();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool dealloc_caller();
  virtual bool dealloc_callee();
  
};


// a parameter without a name
// add to variables.cpp
class FPParameter : public Parameter
{
  Type *type_;
  int pointer_count_;
  Marshal_type *marshal_info_;
 public:
  FPParameter(Type *type, int pointer_count);
  virtual Type* type();
  virtual const char* identifier();
  virtual int pointer_count();
  virtual void set_marshal_info(Marshal_type *mt);
  virtual Marshal_type* marshal_info();
  virtual void prepare_marshal(MarshalPrepareVisitor *worker);
  virtual void resolve_types(LexicalScope *ls);


  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_dealloc_caller(bool b);
  virtual void set_dealloc_callee(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool dealloc_caller();
  virtual bool dealloc_callee();
};

class ReturnVariable : public Variable
{
  const char* name_; // to be decided by a name space or something
  Type* type_;
  Marshal_type *marshal_info_;
  Variable* accessor_;
  int pointer_count_;
  
 public:
  ReturnVariable();
  ReturnVariable(Type* return_type, int pointer_count);
  virtual void set_marshal_info(Marshal_type *mt);
  virtual Marshal_type* marshal_info();
  virtual void prepare_marshal(MarshalPrepareVisitor *worker);
  virtual void resolve_types(LexicalScope *ls);
  virtual const char* identifier();
  virtual Type* type();
  virtual void set_accessor(Variable *v);
  virtual Variable* accessor();
  virtual int pointer_count();
  

  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_dealloc_caller(bool b);
  virtual void set_dealloc_callee(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool dealloc_caller();
  virtual bool dealloc_callee();
};

class Function : public Type
{
  const char *identifier_;
  ReturnVariable *return_var_;
  std::vector<Parameter*> parameters_;
  LexicalScope *current_scope_;

 public:
  Function(const char *id, ReturnVariable *return_var, std::vector<Parameter*> parameters, LexicalScope *ls);
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual int num();
  virtual const char* name();
  virtual void resolve_types(LexicalScope *ls);
  Rpc* to_rpc(ProjectionType *pt);
  virtual void create_trampoline_structs(LexicalScope *ls);
};
 
class Typedef : public Type
{
  Type* type_;
  const char* alias_;
  char* marshal_info_;
  const char* identifier_;

 public:
  Typedef(const char* id, const char* alias, Type* type);
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual const char* name();
  Type* type();
  const char* alias();
  virtual int num();
  virtual void resolve_types(LexicalScope *ls);
  // virtual void marshal();
  virtual void create_trampoline_structs(LexicalScope *ls);
};

class Channel : public Type 
{
 public:
  Channel();
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual const char* name();
  virtual int num();
  virtual void resolve_types(LexicalScope *ls);
  virtual void create_trampoline_structs(LexicalScope *ls);
};

class VoidType : public Type
{
 public:
  VoidType();
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual const char* name();
  virtual int num();
  virtual void resolve_types(LexicalScope *ls);
  virtual void create_trampoline_structs(LexicalScope *ls);
};

class IntegerType : public Type
{
  bool unsigned_;
  PrimType type_;
  int size_;
  
 public:
  IntegerType(PrimType type, bool un, int size);
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual const char* name();
  PrimType int_type();
  bool is_unsigned();
  virtual int num();
  virtual void resolve_types(LexicalScope *ls);
  ~IntegerType(){printf("inttype destructor\n");}
  virtual void create_trampoline_structs(LexicalScope *ls);
};

class ProjectionField : public Variable //?
{
  bool in_;
  bool out_;
  bool alloc_callee_;
  bool alloc_caller_;
  bool dealloc_callee_;
  bool dealloc_caller_;

  Type* field_type_;
  const char* field_name_;
  Variable *accessor_; // 
  int pointer_count_;
  Marshal_type *marshal_info_;

 public:
  ProjectionField(Type* field_type, const char* field_name, int pointer_count);
  ~ProjectionField(); 
  virtual Type* type();
  virtual const char* identifier();
  virtual void set_accessor(Variable *v);
  virtual Variable* accessor();
  virtual void set_marshal_info(Marshal_type *mt); // add to .cpp file
  virtual Marshal_type* marshal_info(); // make sure all variables have
  virtual void prepare_marshal(MarshalPrepareVisitor *worker);
  virtual void resolve_types(LexicalScope *ls);
  virtual int pointer_count();
  


  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_dealloc_caller(bool b);
  virtual void set_dealloc_callee(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool dealloc_caller();
  virtual bool dealloc_callee();

};

class ProjectionType : public Type // complex type
{
  
  const char* id_; 
  const char* real_type_;
  std::vector<ProjectionField*> fields_; 
  std::vector<GlobalVariable*> init_variables_;
 public:
  ProjectionType(const char* id, const char* real_type, std::vector<ProjectionField*> fields, std::vector<GlobalVariable*> init_variables);
  ProjectionType(const char* id, const char* real_type, std::vector<ProjectionField*> fields);
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual const char* name();
  const char* real_type();
  std::vector<ProjectionField*> fields();
  virtual int num();
  virtual void resolve_types(LexicalScope *ls);
  ~ProjectionType(){printf("projection type destructor\n");}
  virtual void create_trampoline_structs(LexicalScope *ls);
};

class Rpc : public Base
{
  const char* enum_name_;
  SymbolTable *symbol_table_;
  ReturnVariable *explicit_return_;
  LexicalScope *current_scope_;
  /* -------------- */

  const char* name_;
  std::vector<Parameter* > parameters_;
  std::vector<Parameter* > marshal_parameters_; // wtf is this
  bool function_pointer_defined_;
 public:
  Rpc(ReturnVariable *return_var, const char* name, std::vector<Parameter* > parameters, LexicalScope *current_scope);
  void set_function_pointer_defined(bool b);
  bool function_pointer_defined();
  const char* name();
  const char* enum_name();
  const char* callee_name();
  std::vector<Parameter*> parameters();
  ReturnVariable *return_variable();
  SymbolTable* symbol_table();
  void prepare_marshal();
  void resolve_types();
  void create_trampoline_structs();
  LexicalScope *current_scope();
};

class Module : public Base
{
  // const char *verbatim_;
  const char* module_name_;
  LexicalScope *module_scope_;
  std::vector<GlobalVariable*> globals_;
  std::vector<Rpc*> rpc_definitions_;
 public:
  Module(const char* id, std::vector<Rpc*> rpc_definitions, std::vector<GlobalVariable*> globals, LexicalScope *ls);
  std::vector<Rpc*> rpc_definitions();  
  std::vector<GlobalVariable*> globals();
  LexicalScope *module_scope();
  void prepare_marshal();
  void resolve_types();
  void function_pointer_to_rpc();
  void create_trampoline_structs();
  const char* identifier();
};

class Include : public Base
{
  bool relative_; // true if "" false for <>
  const char *path_;
 public:
  Include(bool relative, const char* path);
};

class Project : public Base
{
  LexicalScope *project_scope_;
  std::vector<Module*> project_modules_;
  std::vector<Include*> project_includes_;
  
 public:
  Project(LexicalScope *scope, std::vector<Module*> modules, std::vector<Include*> includes);
  void prepare_marshal();
  void resolve_types();
  void function_pointer_to_rpc();
  void create_trampoline_structs();
  std::vector<Module*> modules();
};

class TypeNameVisitor // generates CCSTTypeName for each type.
{
 public:
  CCSTTypeName* visit(Typedef *td);
  CCSTTypeName* visit(VoidType *vt);
  CCSTTypeName* visit(IntegerType *it);
  CCSTTypeName* visit(ProjectionType *pt);
  CCSTTypeName* visit(Function *fp);
  CCSTTypeName* visit(Channel *c);
};

class TypeVisitor
{
 public:
  virtual CCSTStatement* visit(Function *fp, Variable *v) = 0;
  virtual CCSTStatement* visit(Typedef *td, Variable *v) = 0;
  virtual CCSTStatement* visit(VoidType *vt, Variable *v) = 0;
  virtual CCSTStatement* visit(IntegerType *it, Variable *v) = 0;
  virtual CCSTStatement* visit(ProjectionType *pt, Variable *v) = 0;
  virtual CCSTStatement* visit(Channel *c, Variable *v) = 0;
};

class AllocateTypeVisitor : public TypeVisitor    
{
  CCSTStatement* allocation_helper(IntegerType *it, Variable *v, int pointer_count);
  CCSTStatement* allocation_helper(VoidType *vt, Variable *v, int pointer_count);
  CCSTStatement* allocation_helper(Typedef *vt, Variable *v, int pointer_count);
  CCSTStatement* allocation_helper(ProjectionType *vt, Variable *v, int pointer_count);
 public:
  AllocateTypeVisitor();
  virtual CCSTStatement* visit(Function *fp, Variable *v);
  virtual CCSTStatement* visit(Typedef *td, Variable *v);
  virtual CCSTStatement* visit(VoidType *vt, Variable *v);
  virtual CCSTStatement* visit(IntegerType *it, Variable *v);
  virtual CCSTStatement* visit(ProjectionType *pt, Variable *v);
  virtual CCSTStatement* visit(Channel *c, Variable *v);
};

class MarshalTypeVisitor : public TypeVisitor
{
 public:
  MarshalTypeVisitor();
  virtual CCSTStatement* visit(Function *fp, Variable *v);
  virtual CCSTStatement* visit(Typedef *td, Variable *v);
  virtual CCSTStatement* visit(VoidType *vt, Variable *v);
  virtual CCSTStatement* visit(IntegerType *it, Variable *v);
  virtual CCSTStatement* visit(ProjectionType *pt, Variable *v);
  virtual CCSTStatement* visit(Channel *c, Variable *v);
};

class AllocateVariableVisitor 
{
 public:
  AllocateVariableVisitor();
  CCSTStatement* visit(ProjectionField *pf);
  CCSTStatement* visit(Parameter *p);
  CCSTStatement* visit(ReturnVariable *rv);
};

class AllocateRegisterVisitor
{
 public:
  AllocateRegisterVisitor();
  CCSTStatement* visit(ProjectionField *pf);
  CCSTStatement* visit(Parameter *p);
  CCSTStatement* visit(ReturnVariable *rv);
};

#endif
