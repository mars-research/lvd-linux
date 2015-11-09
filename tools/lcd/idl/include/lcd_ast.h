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

class CCSTStatement;
class CCSTTypeName;
class Marshal_type;
class MarshalVisitor;
class TypeNameVisitor;
class AllocateTypeVisitor;
class Variable;
class Type;
class Parameter;
class Rpc;

enum PrimType {pt_char_t, pt_short_t, pt_int_t, pt_long_t, pt_longlong_t, pt_capability_t};
enum type_k {};

template<class T, class T2>
class ASTVisitor;

class Base
{			  
 public:
  // virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data) = 0;
};

class LexicalScope : public Base
{
  LexicalScope *outer_scope_;
  std::map<std::string, Type*> type_definitions_;
  std::map<std::pair<std::string, std::vector<Parameter*> >, Rpc*> rpc_definitions_; // rpc or function pointer
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
};

class GlobalScope : public LexicalScope
{
  static GlobalScope *instance_;
  LexicalScope *outer_scope_;
  std::map<std::string, Type*> types_definitions_;
  std::map<std::pair<std::string, std::vector<Parameter*> >, Rpc*> rpc_definitions_; // rpc or function pointer
  std::vector<LexicalScope*> inner_scopes_;

 public:
  GlobalScope();
  virtual std::vector<Rpc*> rpc_in_scope();
  virtual bool insert(Rpc *r);
  virtual Type* lookup(const char *symbol, int *err);
  virtual bool insert(const char *symbol, Type *type);
  virtual bool contains(const char *symbol);
  virtual void set_outer_scope(LexicalScope *ls);
  virtual void add_inner_scope(LexicalScope *ls);
  virtual void add_inner_scopes(std::vector<LexicalScope*> scopes);
  virtual std::map<std::string, Type*> type_definitions();
  virtual std::vector<LexicalScope*> inner_scopes();
  static GlobalScope* instance();
};

class Type : public Base
{
 public:
  // virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data) = 0;
  virtual CCSTTypeName* accept(TypeNameVisitor *worker) = 0;
  virtual CCSTStatement* accept(AllocateTypeVisitor *worker, Variable *v) = 0;
  virtual int num() = 0;
  virtual const char* name() = 0;
};

class Variable : public Base
{
 public:
  virtual Type* type() = 0;
  virtual const char* identifier() = 0;
  virtual void set_accessor(Variable *v) = 0;
  virtual Variable* accessor() = 0;
  virtual Marshal_type* marshal_info() = 0;
  virtual int pointer_count();

  virtual void set_in(bool b) = 0;
  virtual void set_out(bool b) = 0;
  virtual void set_alloc(bool b) = 0;
  virtual void set_alloc_caller(bool b) = 0;
  virtual void set_alloc_callee(bool b) = 0;
  virtual void set_alloc_callee_caller(bool b) = 0;

  virtual bool in() = 0;
  virtual bool out() = 0;
  virtual bool alloc() = 0;
  virtual bool alloc_caller() = 0;
  virtual bool alloc_callee() = 0;
  virtual bool alloc_callee_caller() = 0;
  
};

class GlobalVariable : public Variable
{
  Type *type_;
  const char *id_;
  int pointer_count_;
 public:
  GlobalVariable(Type *type, const char *id, int pointer_count);
  virtual Type* type();
  virtual const char* identifier();
  virtual void set_accessor(Variable *v);
  virtual Variable* accessor();
  virtual Marshal_type* marshal_info();
  virtual int pointer_count();

  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_alloc_callee_caller(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool alloc_callee_caller();

};

class Parameter : public Variable
{
  bool in_;
  bool out_;
  bool alloc_;
  bool alloc_callee_;
  bool alloc_caller_;
  bool alloc_callee_caller_;

  Type* type_;
  const char* name_;
  Marshal_type *marshal_info_;
  Variable *accessor_;
  int pointer_count_;

 public:
  Parameter();
  Parameter(Type* type, const char* name, int pointer_count);
  ~Parameter();
  // virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual Type* type();
  void set_marshal_info(Marshal_type* mt); // ??????????????????????????????
  Marshal_type* marshal_info(); //???????????????????????????????????
  virtual const char* identifier();
  virtual void set_accessor(Variable *v);
  virtual Variable* accessor();
  virtual int pointer_count();
  
  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_alloc_callee_caller(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool alloc_callee_caller();
};


// a parameter without a name
class FPParameter : public Parameter
{
  Type *type_;
  int pointer_count_;
 public:
  FPParameter(Type *type, int pointer_count);
  virtual Type* type();
  virtual const char* identifier();
  virtual int pointer_count();
};

class FunctionPointer : public Type
{
  const char *identifier_;
  Type *return_type_;
  std::vector<FPParameter*> parameters_;

 public:
  FunctionPointer(const char *id, Type *return_type, std::vector<FPParameter*> parameters);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(AllocateTypeVisitor *worker, Variable *v);
  virtual int num();
  virtual const char* name();
};
 
class Typedef : public Type
{
  Type* type_;
  const char* alias_;
  char* marshal_info_;

 public:
  Typedef(const char* alias, Type* type);
  // virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(AllocateTypeVisitor *worker, Variable *v);
  virtual const char* name();
  Type* type();
  const char* alias();
  virtual int num();
  // virtual void marshal();
};

class VoidType : public Type
{
 public:
  VoidType();
  // virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(AllocateTypeVisitor *worker, Variable *v);
  virtual const char* name();
  virtual int num();
};

class IntegerType : public Type
{
  bool unsigned_;
  PrimType type_;
  int size_;
  
 public:
  IntegerType(PrimType type, bool un, int size);
  // virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(AllocateTypeVisitor *worker, Variable *v);
  virtual const char* name();
  PrimType int_type();
  bool is_unsigned();
  virtual int num();
  ~IntegerType(){printf("inttype destructor\n");}
};

class ProjectionField : public Variable //?
{
  bool in_;
  bool out_;
  bool alloc_;
  bool alloc_callee_;
  bool alloc_caller_;
  bool alloc_callee_caller_;

  Type* field_type_;
  const char* field_name_;
  Variable *accessor_; // 
  int pointer_count_;

 public:
  ProjectionField(Type* field_type, const char* field_name, int pointer_count);
  ~ProjectionField(); 
  // virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual Type* type();
  virtual const char* identifier();
  virtual void set_accessor(Variable *v);
  virtual Variable* accessor();
  virtual Marshal_type* marshal_info();
  virtual int pointer_count();

  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_alloc_callee_caller(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool alloc_callee_caller();
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
  // virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(AllocateTypeVisitor *worker, Variable *v);
  virtual const char* name();
  const char* real_type();
  std::vector<ProjectionField*> fields();
  virtual int num();
  ~ProjectionType(){printf("projection type destructor\n");}
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
  void set_marshal_info(Marshal_type *mt);
  Marshal_type* marshal_info();

  virtual const char* identifier();
  virtual Type* type();
  virtual void set_accessor(Variable *v);
  virtual Variable* accessor();
  virtual int pointer_count();

  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_alloc_callee_caller(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool alloc_callee_caller();
};

class ImplicitReturnVariable : public ReturnVariable
{
  Parameter* p_;
  Marshal_type *marshal_info_;
  Variable *accessor_;

 public:
  ImplicitReturnVariable(Parameter *p);
  void set_marshal_info(Marshal_type *mt);
  Marshal_type* marshal_info();

  virtual void set_accessor(Variable *v);
  virtual Type* type();
  virtual const char* identifier();
  virtual Variable* accessor();
  virutal int pointer_count();

  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_alloc_callee_caller(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool alloc_callee_caller();
};

class Rpc : public Base
{
  const char* enum_name_;
  SymbolTable *symbol_table_;
  ReturnVariable *explicit_return_;
  std::vector<ImplicitReturnVariable*> implicit_returns_;
  /* -------------- */

  char* name_;
  std::vector<Parameter* > parameters_;
  std::vector<Parameter* > marshal_parameters_;
  void set_implicit_returns();
 public:
  Rpc(ReturnVariable *return_value, char* name, std::vector<Parameter* > parameters);
  char* name();
  const char* enum_name();
  const char* callee_name();
  std::vector<Parameter*> parameters();
  // virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  ReturnVariable *return_value();
  SymbolTable* symbol_table();
};

class Module : public Base
{
  // const char *verbatim_;
  LexicalScope *module_scope_;
  std::vector<GlobalVariable*> globals_;
  std::vector<Rpc*> rpc_definitions_;
 public:
  Module(std::vector<Rpc*> rpc_definitions, std::vector<GlobalVariable*> globals, LexicalScope *ls);
  std::vector<Rpc*> rpc_definitions();  
  std::vector<GlobalVariable*> globals();
  LexicalScope *module_scope();
};

class Project : public Base
{
  LexicalScope *project_scope_;
  std::vector<Module*> project_modules_;
  
 public:
  Project(LexicalScope scope, std::vector<Module*> modules);
};

class TypeNameVisitor // generates CCSTTypeName for each type.
{
 public:
  CCSTTypeName* visit(Typedef *td);
  CCSTTypeName* visit(VoidType *vt);
  CCSTTypeName* visit(IntegerType *it);
  CCSTTypeName* visit(ProjectionType *pt);
  CCSTTypeName* visit(FunctionPointer *fp);
};

class AllocateTypeVisitor 
{
  Type* first_non_pointer(PointerType *pt);
  int count_pointers(PointerType *pt);
  bool pointer_to_struct(Type *t);
  CCSTStatement* allocation_helper(IntegerType *it, Variable *v, int pointer_count);
  CCSTStatement* allocation_helper(VoidType *vt, Variable *v, int pointer_count);
  CCSTStatement* allocation_helper(Typedef *vt, Variable *v, int pointer_count);
  CCSTStatement* allocation_helper(ProjectionType *vt, Variable *v, int pointer_count);
 public:
  AllocateTypeVisitor();
  CCSTStatement* visit(Typedef *td, Variable *v);
  CCSTStatement* visit(VoidType *vt, Variable *v);
  CCSTStatement* visit(IntegerType *it, Variable *v);
  CCSTStatement* visit(ProjectionType *pt, Variable *v);
  CCSTStatement* visit(FunctionPointer *fp, Variable *v);
};

class AllocateVariableVisitor 
{
 public:
  AllocateVariableVisitor();
  CCSTStatement* visit(ProjectionField *pf);
  CCSTStatement* visit(Parameter *p);
  CCSTStatement* visit(ReturnVariable *rv);
  CCSTStatement* visit(ImplicitReturnVariable *irv);
};

class AllocateRegisterVisitor
{
 public:
  AllocateRegisterVisitor();
  CCSTStatement* visit(ProjectionField *pf);
  CCSTStatement* visit(Parameter *p);
  CCSTStatement* visit(ReturnVariable *rv);
  CCSTStatement* visit(ImplicitReturnVariable *irv);
};

#endif
