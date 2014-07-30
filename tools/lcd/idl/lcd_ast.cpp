#include "lcd_ast.h"

PrimitiveType::PrimitiveType(TypeModifier type_mod, PrimType primitive)
{
  this->type_modifier_ = type_mod;
  this->primitive_ = primitive;
}

ProjectionType::ProjectionType(char * type_name, bool pointer)
{
  this->type_name_ = type_name;
  this->pointer_ = pointer;
}

Scope::Scope(char* verbatim, std::vector<Rpc* >* rpc_definitions, std::vector<Message* >* message_definitions,
	     std::map<char* , Projection* >* projection_definitions)
{
  this->verbatim_ = verbatim;
  this->rpc_definitions_ = rpc_definitions;
  this->message_definitions_ = message_definitions;
  this->projection_definitions_ = projection_definitions;
}

Rpc::Rpc(Type* return_type, char * name, std::vector<Parameter* >* parameters)
{
  this->return_type_ = return_type;
  this->name_ = name;
  this->parameters_ = parameters;
}

Projection::Projection(char * name, char* true_type, std::vector<ProjectionField* >* fields)
{
  this->name_ = name;
  this->true_type_ = true_type;
  this->fields_ = fields;
}

ProjectionField::ProjectionField(bool in, bool out, bool alloc, bool bind, Type* field_type, char* field_name)
{
  this->in_ = in;
  this->out_ = out;
  this->alloc_ = alloc;
  this->bind_ = bind;
  this->field_type_ = field_type;
  this->field_name_ = field_name;
}

Parameter::Parameter(Type* type, char* name)
{
  this->type_ = type;
  this->name_ = name;
}

MessageField::MessageField(Type* field_type, char * field_name)
{
  this->field_type_ = field_type;
  this->field_name_ = field_name;
}

Message::Message(char * name, std::vector<MessageField* >* fields)
{
  this->name_ = name;
  this->fields_ = fields;
}




