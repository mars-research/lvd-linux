#ifndef LCD_AST_H
#define LCD_AST_H

#include <vector>
#include <map>

enum PrimType { kChar = 1, kShort, kInt, kLong, kLongLong, kCapability};

class Base
{
  M_info info;
};

class Scope : public Base
{
  // add built in types here, so size can be evaluated before parse
 private:
  map<int,Type> types_;
  void init_types();
 public:
  Scope();
  lookup_symbol(char* sym);
};
 
class Type : public Base
{
 public:
  virtual void accept(ASTVisitor *worker) = 0;
  virtual void marshal() = 0;
  virtual int size() = 0;
};

class Typedef : public Type
{
  Type* type_;
  char* alias_;
  char* marshal_info_;

 public:

  virtual void accept(ASTVisitor *worker);
  virtual void marshal();
}

class IntegerType : public Type
{
  bool unsigned_;
  PrimType type_;
  int size_:
  
 public:
    IntegerType(PrimType type, bool unsigned, int size) 
  { this->unsigned_ = unsigned; this->type_ = type; this->size_ = size;}
  virtual void accept(ASTVisitor *worker) { worker->visit(this); }
  virtual void marshal();
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
  virtual void accept(ASTVisitor *worker) { worker->visit(this); }
  virtual void marshal();
};

class ProjectionType : public Type // complex type
{
  char* id_; 
  char* real_type_;
  std::vector<ProjectionField*>* fields_;

 public:
  ProjectionType(char * id, char* real_type, std::vector<ProjectionField*>* fields) 
    { this->id_ = id; this->real_type_ = real_type; this->fields_ = fields; }
  void accept(ASTVisitor *worker) { worker->visit(this); }
  virtual void marshal();
};

class Parameter : public Base
{
  Type* type_;
  char* name_;
  M_info m;
 public:
  Parameter(Type* type, char* name) 
    { this->type_ = type; this->name_ = name; m = new M_info(); }
  ~Parameter();
  void accept(ASTVisitor *worker) { worker->visit(this); }
};

class Rpc : public Base
{
  Type* ret_type_;
  char* name_;
  std::vector<Parameter* >* params_;

 public:
  Rpc(Type* return_type, char* name, std::vector<Parameter* >* parameters) {
    this->return_type_ = return_type;
    this->name_ = name;
    this->parameters_ = parameters; }
  DefinitionType get_definition_type(){ return kRpc; }
  char* name() { return name_; }
  Type* return_type() { return return_type_; }
  std::vector<Parameter*>* parameters() { return parameters_; }
  void accept(ASTVisitor *worker) { worker->visit(this); }
};

class ProjectionField : public Base
{
  bool in_;
  bool out_;
  bool alloc_;
  bool bind_;
  Type* field_type_;
  char* field_name_;
  
 public:
  ProjectionField(bool in, bool out, bool alloc, bool bind, Type* field_type, char* field_name)
    { this->in_ = in; this->out_ = out; this->alloc_ = alloc; this->bind_ = bind; this->field_type_ = field_type;
      this->field_name_ = field_name; }
  ~ProjectionField();
  bool in() { return in_; }
  bool out() { return out_; }
  bool alloc() { return alloc_; }
  bool bind() { return bind_; }
  void accept(ASTVisitor *worker) { worker->visit(this); }
  virtual void marshal();   // should this be connected to type needs to be marshalled too?
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


class File : public Scope
{
  char* verbatim_;
  Root_scope * root_scope_;
  std::vector<Rpc *>* rpc_defs_;
  std::vector<Message *>* message_defs_;
  
 public:
  File(char* verbatim, Root_scope* rs, std::vector<Rpc* >* rpc_definitions, std::vector<Message* >* message_definitions) {
    this->verbatim_ = verbatim;
    this->root_scope_ = rs;
    this->rpc_definitions_ = rpc_definitions;
    this->message_definitions_ = message_definitions;
  }
  void accept(ASTVisitor *worker) { worker->visit(this); }
  
};



#endif
