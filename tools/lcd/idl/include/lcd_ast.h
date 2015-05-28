#ifndef LCD_AST_H
#define LCD_AST_H

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <stdio.h>
#include "marshal_op.h"


enum PrimType {pt_char_t, pt_short_t, pt_int_t, pt_long_t, pt_longlong_t, pt_capability_t};

class ASTVisitor;

class M_rpc;
class M_type;

class Base
{
};

class Type : public Base
{
  M_type* marshal_info_;
 public:
  virtual void accept(ASTVisitor *worker) = 0;
  virtual int num() = 0;
  // virtual ~Type(){printf("type destructor\n");}
};

class Scope : public Base
{
 public:
  virtual Type* lookup_symbol(const char* sym, int* err) =0;
  virtual int insert_symbol(const char* sym, Type* type) =0;
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
};

class FileScope : public Scope
{
  RootScope* root_;
  std::map<std::string, Type*> *types_;
 public:
  FileScope(RootScope* root);
  virtual Type* lookup_symbol(const char* sym, int* err);
  virtual int insert_symbol(const char* type, Type* t);
};

class Typedef : public Type
{
  Type* type_;
  const char* alias_;
  char* marshal_info_;

 public:
  Typedef(const char* alias, Type* type);
  virtual void accept(ASTVisitor *worker);
  Type* type() { return this->type_; }
  const char* alias();
  virtual int num() {return 1;}
  // virtual void marshal();
};

class VoidType : public Type
{
 public:
  VoidType();
  virtual void accept(ASTVisitor *worker);
  virtual int num() {return 5;}
};

class IntegerType : public Type
{
  bool unsigned_;
  PrimType type_;
  int size_;
  
 public:
  IntegerType(PrimType type, bool un, int size);
  virtual void accept(ASTVisitor *worker);
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
  virtual void accept(ASTVisitor *worker);
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
  bool in();
  bool out();
  bool alloc();
  bool bind();
  virtual void accept(ASTVisitor *worker);
};

class ProjectionType : public Type // complex type
{
  const char* id_; 
  const char* real_type_;
  std::vector<ProjectionField*> fields_;

 public:
  ProjectionType(const char* id, const char* real_type, std::vector<ProjectionField*> fields);
  virtual void accept(ASTVisitor *worker);
  const char* id();
  const char* real_type();
  std::vector<ProjectionField*> fields() { return this->fields_; }
  virtual int num() {return 4;}
  ~ProjectionType(){printf("projection type destructor\n");}
};

class Parameter : public Base
{
  Type* type_;
  const char* name_;
  std::vector<int> registers_;
  
 public:
  Parameter(Type* type, const char* name);
  ~Parameter();
  virtual void accept(ASTVisitor *worker);
  Type* type() { return this->type_; }
  std::vector<int> get_registers() { return this->registers_; }
  void set_registers(std::vector<int> r) { this->registers_ = r; }
  const char* name();
};

class Rpc : public Base
{
  const char* enum_name_;
  M_rpc* marshal_info_;
  
  /* special case */
  Type* ret_type_;
  int ret_register_;
  /* -------------- */

  char* name_;
  std::vector<Parameter* > params_;

 public:
  Rpc(Type* return_type, char* name, std::vector<Parameter* > parameters);
  char* name();
  Type* return_type();
  int ret_register() { return this->ret_register_; }
  void set_ret_register(int r) { this->ret_register_ = r; }
  std::vector<Parameter*> parameters();
  virtual void accept(ASTVisitor *worker);
  M_rpc* marshal_info() { return this->marshal_info_; }
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
  virtual void accept(ASTVisitor *worker);
  std::vector<Rpc*> rpc_defs();
  
};

class ASTVisitor
{
 public:
  virtual void visit(File *file) = 0;
  //  virtual void visit(Message *message) = 0;
  // virtual void visit(MessageField *message_field) = 0;
  virtual void visit(ProjectionField *proj_field) = 0;
  virtual void visit(Rpc *rpc) = 0;
  virtual void visit(Parameter *parameter) = 0;
  virtual void visit(Typedef *type_def) = 0;
  virtual void visit(ProjectionType *proj_type) = 0;
  virtual void visit(PointerType *pointer_type) = 0;
  virtual void visit(IntegerType *integer_type) = 0;
  virtual void visit(VoidType *vt) = 0;
};

#endif
