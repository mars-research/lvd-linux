#include "lcd_ast.h"

/* function pointer type*/

FunctionPointer::FunctionPointer(const char *id, ReturnVariable *return_var, std::vector<FPParameter*> parameters)
{
  this->identifier_  = id;
  this->return_var_ = return_var;
  this->parameters_  = parameters;
}

Marshal_type* FunctionPointer::accept(MarshalPrepareVisitor *worker)
{
  return worker->visit(this);
}

CCSTTypeName* FunctionPointer::accept(TypeNameVisitor *worker)
{
  return worker->visit(this);
}

CCSTStatement* FunctionPointer::accept(TypeVisitor *worker, Variable *v)
{
  return worker->visit(this, v);
}

int FunctionPointer::num()
{
  printf("num todo for function pointer\n");
  return -2;
}

const char* FunctionPointer::name()
{
  return this->identifier_;
}

/* end */

/* typedef type */


Typedef::Typedef(const char* alias, Type* type)
{
  this->alias_ = alias;
  this->type_ = type; // need to allocate?
}

Marshal_type* Typedef::accept(MarshalPrepareVisitor *worker)
{
  return worker->visit(this);
}

CCSTTypeName* Typedef::accept(TypeNameVisitor *worker)
{
  return worker->visit(this);
}

CCSTStatement* Typedef::accept(TypeVisitor *worker, Variable *v)
{
  return worker->visit(this, v);
}

const char* Typedef::alias()
{
  return this->alias_;
}

Type* Typedef::type() 
{
  return this->type_;
}

int Typedef::num()
{
  return 1;
}

const char* Typedef::name()
{
  return this->alias_;
}

/* end */

/* void type */

VoidType::VoidType()
{
}

Marshal_type* VoidType::accept(MarshalPrepareVisitor *worker)
{
  return worker->visit(this);
}

CCSTTypeName* VoidType::accept(TypeNameVisitor *worker)
{
  return worker->visit(this);
}

CCSTStatement* VoidType::accept(TypeVisitor *worker, Variable *v)
{
  return worker->visit(this, v);
}

int VoidType::num()
{
  return 5;
}

const char* VoidType::name()
{
  return "void";
}

/* end */

/* integer type */

IntegerType::IntegerType(PrimType type, bool un, int size)
{
  this->type_ = type;
  this->unsigned_ = un;
  this->size_ = size;
}

Marshal_type* IntegerType::accept(MarshalPrepareVisitor *worker)
{
  return worker->visit(this);
}

CCSTTypeName* IntegerType::accept(TypeNameVisitor *worker)
{
  return worker->visit(this);
}

CCSTStatement* IntegerType::accept(TypeVisitor *worker, Variable *v)
{
  return worker->visit(this, v);
}

PrimType IntegerType::int_type()
{
  return this->type_;
}

bool IntegerType::is_unsigned()
{
  return unsigned_;
}

int IntegerType::num()
{
  return 2;
}

const char* IntegerType::name()
{
  printf("todo integer type name function.\n");
  return "";
}

/* end */

/* projection type */

ProjectionType::ProjectionType(const char* id, const char* real_type, std::vector<ProjectionField*> fields)
{
  this->id_ = id; this->real_type_ = real_type; this->fields_ = fields;
}

ProjectionType::ProjectionType(const char* id, const char* real_type, std::vector<ProjectionField*> fields, std::vector<GlobalVariable*> init_vars)
{
  this->id_ = id; this->real_type_ = real_type; this->fields_ = fields;
  this->init_variables_ = init_vars;
}

Marshal_type* ProjectionType::accept(MarshalPrepareVisitor *worker)
{
  return worker->visit(this);
}

CCSTTypeName* ProjectionType::accept(TypeNameVisitor *worker)
{
  return worker->visit(this);
}

CCSTStatement* ProjectionType::accept(TypeVisitor *worker, Variable *v)
{
  return worker->visit(this, v);
}

// may never be used
const char* ProjectionType::real_type()
{
  return this->real_type_;
}

std::vector<ProjectionField*> ProjectionType::fields()
{
  return this->fields_;
}

int ProjectionType::num()
{
  return 4;
}

const char* ProjectionType::name()
{
  return this->id_;
}

/* end */

