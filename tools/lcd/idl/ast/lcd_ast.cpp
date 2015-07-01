#include "lcd_ast.h"
#include <stdio.h>

Typedef::Typedef(const char* alias, Type* type)
{
  this->alias_ = alias;
  this->type_ = type; // need to allocate?
}

Marshal_type* Typedef::accept(MarshalVisitor *worker, Registers *data)
{
  return worker->visit(this, data);
}

const char* Typedef::alias()
{
  return this->alias_;
}

VoidType::VoidType()
{
}

Marshal_type* VoidType::accept(MarshalVisitor *worker, Registers *data)
{
  return worker->visit(this, data);
}

IntegerType::IntegerType(PrimType type, bool un, int size)
{
  this->type_ = type;
  this->unsigned_ = un;
  this->size_ = size;
}

Marshal_type* IntegerType::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}

PrimType IntegerType::int_type()
{
  return this->type_;
}

bool IntegerType::is_unsigned()
{
  return unsigned_;
}

PointerType::PointerType(Type* type)
{
  this->type_ = type;
}

Marshal_type* PointerType::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}

ProjectionField::ProjectionField(bool in, bool out, bool alloc, bool bind, Type* field_type, const char* field_name)
{
  this->in_ = in; this->out_ = out; this->alloc_ = alloc; this->bind_ = bind; this->field_type_ = field_type; this->field_name_ = field_name;
}

bool ProjectionField::in()
{
  return this->in_;
}

bool ProjectionField::out()
{
  return this->out_;
}

bool ProjectionField::alloc()
{
  return this->alloc_;
}

bool ProjectionField::bind()
{
  return this->bind_;
}

Marshal_type* ProjectionField::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}

ProjectionType::ProjectionType(const char* id, const char* real_type, std::vector<ProjectionField*> fields)
{
  this->id_ = id; this->real_type_ = real_type; this->fields_ = fields;
}

Marshal_type* ProjectionType::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}

const char* ProjectionType::id()
{
  return this->id_;
}

// may never be used
const char* ProjectionType::real_type()
{
  return this->real_type_;
}

Parameter::Parameter(Type* type, const char* name)
{
  this->type_ = type;
  this->name_ = name;
}

Marshal_type* Parameter::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}

const char* Parameter::name()
{
  return this->name_;
}



Rpc::Rpc(Type* return_type, char* name, std::vector<Parameter* > parameters)
{
  this->explicit_ret_type_ = return_type;
  this->name_ = name;
  this->params_ = parameters;
}

 char* Rpc::name()
{
  return name_;
}

Marshal_type* Rpc::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}

const char* Rpc::callee_name()
{
  char * callee_name = (char*) malloc((strlen(this->name_)+strlen("_callee")+1)*sizeof(char));
  sprintf(callee_name, "%s%s", this->name_, "_callee");
  return callee_name;
}

const char* Rpc::enum_name()
{					       
  char * callee_name = (char*) malloc((strlen(this->name_)+strlen("_enum")+1)*sizeof(char));
  sprintf(callee_name, "%s%s", this->name_, "_enum");
  return callee_name;
}

std::vector<Parameter*> Rpc::parameters()
{
  return params_;
}

File::File(const char* verbatim, FileScope* fs, std::vector<Rpc* > rpc_definitions)
{
  this->verbatim_ = verbatim;
  this->scope_ = fs;
  this->rpc_defs_ = rpc_definitions;
}

Marshal_type* File::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}

std::vector<Rpc*> File::rpc_defs()
{
  return this->rpc_defs_;
}
