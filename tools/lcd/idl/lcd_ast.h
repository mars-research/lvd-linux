#ifndef LCD_AST_H
#define LCD_AST_H

#include <vector>
#include <map>

enum PrimType { kChar = 1, kShort, kInt, kLong, kLongLong, kCapability};

class Base
{
  
};

class Scope : public Base
{

};
 
class Type : public Base
{
 public:
  virtual void accept(ASTVisitor *worker) = 0;
  virtual void marshal() = 0;
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
  PrimitiveType(TypeModifier type_mod, PrimType primitive) 
  { this->type_modifier_ = type_mod; this->primitive_ = primitive; }
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

class Typedef : public Declaration
{
  char* real_type_; // how should this be represented?
  Type* marshal_type_; // should this be a supported type?
  char* name_;

 public:
  Typedef(char* real_type, Type* marshal_type, char* typedef_name) {
    this->real_type_ = real_type;
    this->marshal_type_ = marshal_type;
    this->name_ = typedef_name; }
  char* name() { return name_; }
  DefinitionType get_definition_type(){ return kTypedef; }
  void accept(ASTVisitor *worker) { worker->visit(this); }
};

class Parameter : public Base
{
  Type* type_;
  char* name_;

 public:
  Parameter(Type* type, char* name) 
    { this->type_ = type; this->name_ = name; }
  ~Parameter();
  void accept(ASTVisitor *worker) { worker->visit(this); }
};

class Rpc : public Base
{
  Type* return_type_;
  char* name_;
  std::vector<Parameter* >* parameters_;

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
  std::vector<Rpc *>* rpc_definitions_;
  std::vector<Message *>* message_definitions_;
  std::map<char *, Definition *>* symbol_table_;
  // believe it is only necessary to store projections in "env" since functions can't be referenced in other functions
  
 public:
  File(char* verbatim, std::vector<Rpc* >* rpc_definitions, std::vector<Message* >* message_definitions,
	std::map<char* , Definition* >* symbol_table) {
    this->verbatim_ = verbatim;
    this->rpc_definitions_ = rpc_definitions;
    this->message_definitions_ = message_definitions;
    this->symbol_table_ = symbol_table; }
  void accept(ASTVisitor *worker) { worker->visit(this); }
  
};



#endif
