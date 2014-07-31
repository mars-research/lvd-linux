#ifndef LCD_AST_H
#define LCD_AST_H

#include <vector>
#include <map>

enum DefinitionType {kRpc = 1, kModule, kMessage, kProjection, kTypedef};
enum PrimType { kChar = 1, kShort, kInt, kLong, kLongLong, kCapability};
enum TypeModifier {kUnsigned = 1, kNone};

class Definition
{
 public:
  virtual DefinitionType get_definition_type(void) = 0;
  // virtual ~Definition() {};

};

class Type
{
 public:
  virtual ~Type() {}
};

class PrimitiveType : public Type
{
  TypeModifier type_modifier_;
  PrimType primitive_;
  
 public:
  PrimitiveType(TypeModifier type_mod, PrimType primitive);
  TypeModifier type_modifier() {return type_modifier_; }
  PrimType primitive() {return primitive_; }
  char * GetFullName();
  // maybe for consistency this shouldnt have getfullname
};

class ProjectionType : public Type // complex type
{
  char* type_name_; 
  bool pointer_;

 public:
  ProjectionType(char * type_name, bool pointer);
  char * type_name() {return type_name_; }
  bool pointer() { return pointer_; }
  // get full name function? class isn't aware of parent need to pass env
};

class Typedef : public Definition
{
  char* real_type_; // how should this be represented?
  Type* marshal_type_; // should this be a supported type?
  char* name_;

 public:
  Typedef(char* real_type, Type* marshal_type, char* typedef_name);
  char* name() { return name_; }
  DefinitionType get_definition_type(){ return kTypedef; }
};

class Parameter
{
  Type* type_;
  char* name_;

 public:
  Parameter(Type* type, char* name);
  ~Parameter();
};

class Rpc : public Definition
{
  Type* return_type_;
  char* name_;
  std::vector<Parameter* >* parameters_;

 public:
  Rpc(Type* return_type, char* name, std::vector<Parameter* >* parameters);
  DefinitionType get_definition_type(){ return kRpc; }

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
};

class Projection : public Definition
{
  char* name_;
  char* true_type_; // struct isn't allowed anywhere but here so just use char *
  std::vector<ProjectionField* >* fields_;

 public:
  Projection(char* name, char* true_type, std::vector<ProjectionField* >* fields);
  char* name() const { return name_; }
  DefinitionType get_definition_type(){return kProjection;}
  
};

class MessageField
{
  Type* field_type_;
  char* field_name_;

 public:
  MessageField(Type* field_type, char* field_name);
};

class Message : public Definition
{
  char * name_;
  std::vector<MessageField* >* fields_;

 public:
  Message(char * name, std::vector<MessageField* >* fields);
  DefinitionType get_definition_type() { return kMessage; }
};



class Scope 
{
  char* verbatim_;
  std::vector<Rpc *>* rpc_definitions_;
  std::vector<Message *>* message_definitions_;
  std::map<char *, Definition *>* symbol_table__;
  // believe it is only necessary to store projections in "env" since functions can't be referenced in other functions
  
 public:
  Scope(char* verbatim, std::vector<Rpc* >* rpc_definitions, std::vector<Message* >* message_definitions,
	std::map<char* , Definition* >* symbol_table);
  
};



#endif
