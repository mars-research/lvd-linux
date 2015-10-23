#ifndef LCD_AST_H
#define LCD_AST_H

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <stdio.h>
#include "marshal_op.h"
#include "symbol_table.h"

class MarshalVisitor;
class TypeNameVisitor;
class AllocateTypeVisitor;
class Variable;

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
  virtual bool insert(Rpc *r);
  virtual Type* lookup(const char *symbol, int *err);
  virtual bool insert(const char *symbol, Type *type);
  virtual bool contains(const char *symbol);
  virtual void set_outer_scope(LexicalScope *ls);
  virtual void add_inner_scope(LexicalScope *ls);
  virtual void add_inner_scopes(std::vector<LexicalScope*> scopes);
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
  virtual Rpc* scope() = 0;
};

class GlobalVariable : public Variable
{
 public:
  GlobalVariable(Type *type, const char *id);
  virtual Type* type() = 0;
  virtual const char* identifier() = 0;
};

class Parameter : public Variable
{
  Type* type_;
  const char* name_;
  Marshal_type *marshal_info_;
  bool in_;
  bool out_; 
  bool alloc_;
  bool bind_;
  Variable *accessor_;
  Rpc *function_;

 public:
  Parameter();
  Parameter(Type* type, const char* name);
  ~Parameter();
  // virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual Type* type();
  virtual Rpc* scope();
  void set_marshal_info(Marshal_type* mt); // ??????????????????????????????
  Marshal_type* marshal_info(); //???????????????????????????????????
  virtual const char* identifier();
  virtual void set_accessor(Variable *v);
  virtual bool bind();
  virtual bool alloc();
  virtual bool in();
  virtual bool out();
  virtual Variable* accessor();
};

class FPParameter : public Parameter
{
  Type *type_;
 public:
  FPParameter(Type *type);
  virtual Type* type();
  virtual const char* identifier();
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

class PointerType : public Type
{
  Type* type_;
 public:
  PointerType(Type* type);
  // virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(AllocateTypeVisitor *worker, Variable *v);
  virtual const char* name();
  Type* type();
  virtual int num();
  ~PointerType(){printf("pointer type destructor\n");}
};

class ProjectionField : public Variable //?
{
  bool in_;
  bool out_;
  Type* field_type_;
  const char* field_name_;
  Variable *accessor_; // 

 public:
  ProjectionField(bool in, bool out, bool alloc, bool bind, Type* field_type, const char* field_name);
  ~ProjectionField(); 
  // virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual Type* type();
  virtual Rpc* scope();
  virtual const char* identifier();
  virtual void set_accessor(Variable *v);
  virtual Variable* accessor();
  bool in();
  bool out();
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
  const char* id();
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
  Rpc* function_;
  
 public:
  ReturnVariable();
  ReturnVariable(Type* return_type);
  void set_marshal_info(Marshal_type *mt);
  Marshal_type* marshal_info();

  virtual const char* identifier();
  virtual Type* type();
  virtual void set_accessor(Variable *v);
  virtual Rpc* scope();
  virtual Variable* accessor();
};

class ImplicitReturnVariable : public ReturnVariable
{
  Parameter* p_;
  Marshal_type *marshal_info_;
  Variable *accessor_;
  Rpc* function_;

 public:
  ImplicitReturnVariable(Parameter *p);
  void set_marshal_info(Marshal_type *mt);
  Marshal_type* marshal_info();

  virtual void set_accessor(Variable *v);
  virtual Type* type();
  virtual const char* identifier();
  virtual Rpc* scope();
  virtual Variable* accessor();
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
  std::vector<GlobalVariable*> globals_;
  std::vector<Rpc*> rpc_definitions_;
 public:
  Module(std::vector<Rpc*> rpc_definitions, std::vector<GlobalVariable*> globals);
  std::vector<Rpc*> rpc_definitions();  
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
  CCSTTypeName* visit(PointerType *pt);
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
  CCSTStatement* visit(PointerType *pt, Variable *v);
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
