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
  virtual int num() = 0;
  // virtual ~Type(){printf("type destructor\n");}
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
  Type* type() { return this->type_; }
  const char* alias();
  virtual int num() {return 1;}
  // virtual void marshal();
};

class VoidType : public Type
{
 public:
  VoidType();
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  virtual int num() {return 5;}
};

class IntegerType : public Type
{
  bool unsigned_;
  PrimType type_;
  int size_;
  
 public:
  IntegerType(PrimType type, bool un, int size);
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  PrimType int_type();
  bool is_unsigned();
  virtual int num() {return 2;}
  ~IntegerType(){printf("inttype destructor\n");}
};
/*
class CapabilityType : public IntegerType
{
  // is this needed?
};
*/
class PointerType : public Type
{
  Type* type_;
 public:
  PointerType(Type* type);
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  Type* type() { return this->type_; }
  virtual int num() {return 3;}
  ~PointerType(){printf("pointer type destructor\n");}
};

class ProjectionField : public Base
{
  bool in_;
  bool out_;
  bool alloc_;
  bool bind_;
  Type* field_type_;
  const char* field_name_;
  
 public:
  ProjectionField(bool in, bool out, bool alloc, bool bind, Type* field_type, const char* field_name);
  ~ProjectionField(); 
  Type* type() { return this->field_type_; }
  const char* field_name() { return this->field_name_; }
  bool in();
  bool out();
  bool alloc();
  bool bind();
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
};

class ProjectionType : public Type // complex type
{
  const char* id_; 
  const char* real_type_;
  std::vector<ProjectionField*> fields_;

 public:
  ProjectionType(const char* id, const char* real_type, std::vector<ProjectionField*> fields);
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  const char* id();
  const char* real_type();
  std::vector<ProjectionField*> fields() { return this->fields_; }
  virtual int num() { printf("calling projectiontype num\n"); return 4;}
  ~ProjectionType(){printf("projection type destructor\n");}
};

class Parameter : public Base
{
  Type* type_;
  const char* name_;
  Marshal_type *marshal_info_;
  
 public:
  Parameter(Type* type, const char* name);
  ~Parameter();
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);
  Type* type() { return this->type_; }
  void set_marshal_info(Marshal_type* mt) { this->marshal_info_ = mt; }
  Marshal_type* get_marshal_info() { return this->marshal_info_; }
  const char* name();
};

class Rpc : public Base
{
  const char* enum_name_;

  /* special case */
  Type* explicit_ret_type_;
  std::vector<Type*> implicit_ret_types_; // can "Return" struct fields
  Marshal_type *explicit_ret_marshal_info_;
  std::vector<Marshal_type*> implicit_ret_marshal_info_;
  /* -------------- */

  char* name_;
  std::vector<Parameter* > params_;

 public:
  Rpc(Type* return_type, char* name, std::vector<Parameter* > parameters);
  char* name();
  Type* explicit_return_type() { return this->explicit_ret_type_; }
  std::vector<Type*> implicit_return_types() { return this->implicit_ret_types_; }
  std::vector<Parameter*> parameters();
  std::vector<Marshal_type*> implicit_ret_marshal_info() { return this->implicit_ret_marshal_info_; }
  void set_implicit_ret_marshal_info(std::vector<Marshal_type*> mt) { this->implicit_ret_marshal_info_ = mt; }
  Marshal_type* explicit_ret_marshal_info() { return this->explicit_ret_marshal_info_; }
  void set_explicit_ret_marshal_info(Marshal_type *mt) { this->explicit_ret_marshal_info_ = mt; }
  virtual Marshal_type* accept(MarshalVisitor *worker, Registers *data);

  const char* enum_name();
  const char* callee_name();
};



/*
class MessageField
{
  Type* field_type_;
  char* field_name_;

 public:
  MessageField(Type* field_type, char* field_name) {
    this->field_type_ = field_type;
    this->field_name_ = field_name; }
  void accept(ASTVisitor *worker) { worker->visit(this); }
};

class Message : public  Definition
{
  char * name_;
  std::vector<MessageField* >* fields_;

 public:
  Message(char * name, std::vector<MessageField* >* fields) {
    this->name_ = name;
    this->fields_ = fields; }
  DefinitionType get_definition_type() { return kMessage; }
  void accept(ASTVisitor *worker) { worker->visit(this); }
};
*/


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

/*
template<class T, class T2>
  class ASTVisitor
{
 public:
  virtual T* visit(File *file, T2 *data) = 0;
  //  virtual T visit(Message *message) = 0;
  // virtual T visit(MessageField *message_field) = 0;
  virtual T* visit(ProjectionField *proj_field, T2 *data) = 0;
  virtual T* visit(Rpc *rpc, T2 *data) = 0;
  virtual T* visit(Parameter *parameter, T2 *data) = 0;
  virtual T* visit(Typedef *type_def, T2 *data) = 0;
  virtual T* visit(ProjectionType *proj_type, T2 *data) = 0;
  virtual T* visit(PointerType *pointer_type, T2 *data) = 0;
  virtual T* visit(IntegerType *integer_type, T2 *data) = 0;
  virtual T* visit(VoidType *vt, T2 *data) = 0;
};
*/
#endif
