#ifndef LCD_AST_H
#define LCD_AST_H

#include <vector>
#include <map>

enum DefinitionType {kRpc = 1, kModule, kMessage, kProjection, kTypedef};
enum Types {kProjectionType, kPrimType, kUnresolvedType }
enum PrimType { kChar = 1, kShort, kInt, kLong, kLongLong, kCapability};
enum TypeModifier {kUnsigned = 1, kNone};

class Definition
{
 public:
  virtual DefinitionType get_definition_type(void) = 0;
  virtual void accept(ASTVisitor *worker) = 0;
  // virtual ~Definition() {};

};

class Type
{
 public:
  virtual ~Type() {}
  virtual DerivedType DerivedClassType(void) = 0;
  virtual void accept(ASTVisitor *worker) = 0;
  char* GetType();
};

class UnresolvedType : public Type
{
  char* name_;

 public:
  UnresolvedType(char* name) { this->name_ = name; }
  DerivedType DerivedClassType() { return kUnresolvedType; }
  char * name() { return name_; }
  void accept(ASTVisitor *worker) { worker->visit(this); }
}

class PrimitiveType : public Type
{
  TypeModifier type_modifier_;
  PrimType primitive_;
  
 public:
  PrimitiveType(TypeModifier type_mod, PrimType primitive) {
    this->type_modifier_ = type_mod;
    this->primitive_ = primitive; }
  TypeModifier type_modifier() {return type_modifier_; }
  PrimType primitive() {return primitive_; }
  char * GetType();
  DerivedType DerivedClassType() { return kPrimType; }
  void accept(ASTVisitor *worker) { worker->visit(this); }
  
  // maybe for consistency this shouldnt have getfullname
};

class ProjectionType : public Type // complex type
{
  char* type_name_; 
  bool pointer_;

 public:
  ProjectionType(char * type_name, bool pointer) {
    this->type_name_ = type_name;
    this->pointer_ = pointer; }
  char* type_name() {return type_name_; }
  bool pointer() { return pointer_; }
  DerivedType DerivedClassType() { return kProjectionType; }
  void accept(ASTVisitor *worker) { worker->visit(this); }
  // get full name function? class isn't aware of parent need to pass env
};

class Typedef : public Definition
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

class Parameter
{
  Type* type_;
  char* name_;

 public:
  Parameter(Type* type, char* name) {
    this->type_ = type;
    this->name_ = name; }
  ~Parameter();
  void accept(ASTVisitor *worker) { worker->visit(this); }
};

class Rpc : public Definition
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

class ProjectionField
{
  bool in_;
  bool out_;
  bool alloc_;
  bool bind_;
  Type* field_type_;
  char* field_name_;
  
 public:
  ProjectionField(bool in, bool out, bool alloc, bool bind, Type* field_type, char* field_name);
  ~ProjectionField();
  bool in() { return in_; }
  bool out() { return out_; }
  bool alloc() { return alloc_; }
  bool bind() { return bind_; }
  void accept(ASTVisitor *worker) { worker->visit(this); }
};

class Projection : public Definition
{
  char* name_;
  char* true_type_; // struct isn't allowed anywhere but here so just use char *
  std::vector<ProjectionField* >* fields_;

 public:
  Projection(char* name, char* true_type, std::vector<ProjectionField* >* fields) {
    this->name_ = name;
    this->true_type_ = true_type;
    this->fields_ = fields; }
  char* name() const { return name_; }
  DefinitionType get_definition_type(){return kProjection;}
  void accept(ASTVisitor *worker) { worker->visit(this); }
  
};

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

class Message : public Definition
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



class Scope 
{
  char* verbatim_;
  std::vector<Rpc *>* rpc_definitions_;
  std::vector<Message *>* message_definitions_;
  std::map<char *, Definition *>* symbol_table_;
  // believe it is only necessary to store projections in "env" since functions can't be referenced in other functions
  
 public:
  Scope(char* verbatim, std::vector<Rpc* >* rpc_definitions, std::vector<Message* >* message_definitions,
	std::map<char* , Definition* >* symbol_table) {
    this->verbatim_ = verbatim;
    this->rpc_definitions_ = rpc_definitions;
    this->message_definitions_ = message_definitions;
    this->symbol_table_ = symbol_table; }
  void accept(ASTVisitor *worker) { worker->visit(this); }
  
};



#endif
