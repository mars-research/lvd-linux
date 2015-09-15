#ifndef LCD_AST_H
#define LCD_AST_H

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <stdio.h>
#include "marshal_op.h"
#include "marshal_visitor.h"

class MarshalVisitor;

enum PrimType {pt_char_t, pt_short_t, pt_int_t, pt_long_t, pt_longlong_t, pt_capability_t};
enum type_k {};

template<class T, class T2>
class ASTVisitor;

class Base
{			  
 public:
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data) = 0;
};

class Type : public Base
{
 public:
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data) = 0;
  virtual CCSTTypeName* accept(TypeNameVisitor *worker) = 0;
  virtual int num() = 0;
  
};

class Variable : public Base
{
 public:
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data) = 0;
  virtual Type* type() = 0;
  virtual const char* identifier() = 0;
  virtual void set_accessor(Variable *v) = 0;
  virtual Variable* accessor() = 0;
  virtual Marshal_type* marshal_info() = 0;
  virtual Rpc* scope() = 0;
};

class Scope : public Base
{
 public:
  virtual Type* lookup_symbol(const char* sym, int* err) =0;
  virtual int insert_symbol(const char* sym, Type* type) =0;
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data) = 0;
};
 
class RootScope : public Scope
{
  static RootScope* instance_;
  std::map<std::string,Type*>* types_;
  void init_types();
  RootScope(void);

 public:
  static int test();
  static RootScope* instance();
  virtual Type* lookup_symbol(const char* sym, int* err);
  virtual int insert_symbol(const char* sym, Type* type);
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
};

class FileScope : public Scope
{
  RootScope* root_;
  std::map<std::string, Type*> *types_;
 public:
  FileScope(RootScope* root);
  virtual Type* lookup_symbol(const char* sym, int* err);
  virtual int insert_symbol(const char* type, Type* t);
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
};

class Typedef : public Type
{
  Type* type_;
  const char* alias_;
  char* marshal_info_;

 public:
  Typedef(const char* alias, Type* type);
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  Type* type();
  const char* alias();
  virtual int num();
  // virtual void marshal();
};

class VoidType : public Type
{
 public:
  VoidType();
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual int num();
};

class IntegerType : public Type
{
  bool unsigned_;
  PrimType type_;
  int size_;
  
 public:
  IntegerType(PrimType type, bool un, int size);
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
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
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
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
  ProjectionField(bool in, bool out, bool alloc, bool bind, Type* field_type, const char* field_name, ProjectionType *container_projection);
  ~ProjectionField(); 
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
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

 public:
  ProjectionType(const char* id, const char* real_type, std::vector<ProjectionField*> fields);
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  const char* id();
  const char* real_type();
  std::vector<ProjectionField*> fields();
  virtual int num();
  ~ProjectionType(){printf("projection type destructor\n");}
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
  Parameter(Type* type, const char* name);
  ~Parameter();
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
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

class ReturnVariable : public Variable
{
  const char* name_; // to be decided by a name space or something
  Type* type_;
  Marshal_type *marshal_info_;
  Variable* accessor_;
  Rpc* function_;
  
 public:
  ReturnValue();
  ReturnValue(Type* return_type);
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
  ImplicitReturnValue(Parameter *p);
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
  ReturnValue *explicit_return_;
  std::vector<ImplicitReturnValue*> implicit_returns_;
  /* -------------- */

  char* name_;
  std::vector<Parameter* > parameters_;
  std::vector<Parameter* > marshal_parameters_;
  void set_implicit_returns();
 public:
  Rpc(ReturnValue *return_value, char* name, std::vector<Parameter* > parameters);
  char* name();
  const char* enum_name();
  const char* callee_name();
  std::vector<Parameter*> parameters();
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  ReturnValue *return_value();
  SymbolTable* symbol_table();
};

class File : public Base
{
  const char* verbatim_;
  FileScope * scope_; // has pointer to root scope
  std::vector<Rpc *> rpc_defs_;
  // std::vector<Message *>* message_defs_;
  
 public:
  File(const char* verbatim, FileScope* fs, std::vector<Rpc* > rpc_definitions);
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  std::vector<Rpc*> rpc_defs();
  
};

class TypeNameVisitor // generates CCSTTypeName for each type.
{
 public:
  CCSTTypeName* visit(Typedef *td);
  CCSTTypeName* visit(VoidType *vt);
  CCSTTypeName* visit(IntegerType *it);
  CCSTTypeName* visit(PointerType *pt);
  CCSTTypeName* visit(ProjectionType *pt);
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
  CCSTStatement* visit(Typedef *td);
  CCSTStatement* visit(VoidType *vt);
  CCSTStatement* visit(IntegerType *it);
  CCSTStatement* visit(PointerType *pt);
  CCSTStatement* visit(ProjectionType *pt);
  
  
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

#endif
