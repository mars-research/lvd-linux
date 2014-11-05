#include "../include/lcd_ast.h"

Typedef::Typedef(const char* alias, Type* type)
{
  this->alias_ = alias;
  this->type_ = type; // need to allocate?
}

void Typedef::accept(ASTVisitor* worker)
{
  worker->visit(this);
}

IntegerType::IntegerType(const char* type, bool un, int size)
{
  this->type_ = type;
  this->unsigned_ = un;
  this->size_ = size;
}

void IntegerType::accept(ASTVisitor* worker)
{
  worker->visit(this);
}

PointerType::PointerType(Type* type)
{
  this->type_ = type;
}

void PointerType::accept(ASTVisitor* worker)
{
  worker->visit(this);
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

void ProjectionField::accept(ASTVisitor* worker)
{
  worker->visit(this);
}

ProjectionType::ProjectionType(const char* id, const char* real_type, std::vector<ProjectionField*>* fields)
{
  this->id_ = id; this->real_type_ = real_type; this->fields_ = fields;
}

void ProjectionType::accept(ASTVisitor* worker)
{
  worker->visit(this);
}

Parameter::Parameter(Type* type, const char* name)
{
  this->type_ = type;
  this->name_ = name;
}

void Parameter::accept(ASTVisitor* worker)
{
  worker->visit(this);
}

Rpc::Rpc(Type* return_type, const char* name, std::vector<Parameter* >* parameters)
{
  this->ret_type_ = return_type;
  this->name_ = name;
  this->params_ = parameters;
}

const char* Rpc::name()
{
  return name_;
}

Type* Rpc::return_type()
{
  return ret_type_;
}

void Rpc::accept(ASTVisitor* worker)
{
  worker->visit(this);
}

std::vector<Parameter*>* Rpc::parameters()
{
  return params_;
}

File::File(const char* verbatim, FileScope* fs, std::vector<Rpc* >* rpc_definitions)
{
  this->verbatim_ = verbatim;
  this->scope_ = fs;
  this->rpc_defs_ = rpc_definitions;
}

void File::accept(ASTVisitor* worker)
{
  worker->visit(this);
}
