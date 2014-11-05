#ifndef LCD_AST_H
#define LCD_AST_H

#include <vector>
#include <map>

enum PrimType { kChar = 1, kShort, kInt, kLong, kLongLong, kCapability};

class ASTVisitor;

class Base
{
  //M_info info;
};

class Type : public Base
{
 public:
  virtual void accept(ASTVisitor *worker) = 0;
  //  virtual void marshal() = 0;
  // virtual int size() = 0;
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
  std::map<const char*,Type*>* types_;
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
  std::map<const char*, Type*> *types_;
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
  // virtual void marshal();
};

class IntegerType : public Type
{
  bool unsigned_;
  const char* type_;
  int size_;
  
 public:
  IntegerType(const char* type, bool un, int size);
  virtual void accept(ASTVisitor *worker);
  // virtual void marshal();
};

class CapabilityType : public IntegerType
{
  // is this needed?
};

class PointerType : public Type
{
  Type* type_;
 public:
  PointerType(Type* type);
  virtual void accept(ASTVisitor *worker);
  // virtual void marshal();
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
  bool in();
  bool out();
  bool alloc();
  bool bind();
  void accept(ASTVisitor *worker);
};

class ProjectionType : public Type // complex type
{
  const char* id_; 
  const char* real_type_;
  std::vector<ProjectionField*>* fields_;

 public:
  ProjectionType(const char* id, const char* real_type, std::vector<ProjectionField*>* fields);
  void accept(ASTVisitor *worker);
  // virtual void marshal();
};

class Parameter : public Base
{
  Type* type_;
  const char* name_;
  //M_info m;
 public:
  Parameter(Type* type, const char* name);
  ~Parameter();
  void accept(ASTVisitor *worker);
};

class Rpc : public Base
{
  Type* ret_type_;
  const char* name_;
  std::vector<Parameter* >* params_;

 public:
  Rpc(Type* return_type, const char* name, std::vector<Parameter* >* parameters);
  const char* name();
  Type* return_type();
  std::vector<Parameter*>* parameters();
  void accept(ASTVisitor *worker);
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
  std::vector<Rpc *>* rpc_defs_;
  // std::vector<Message *>* message_defs_;
  
 public:
  File(const char* verbatim, FileScope* fs, std::vector<Rpc* >* rpc_definitions);
  void accept(ASTVisitor *worker);
  
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
};

#endif
