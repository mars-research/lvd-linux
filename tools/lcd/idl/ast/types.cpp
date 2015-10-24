#include "lcd_ast.h"

/* function pointer type*/

FunctionPointer::FunctionPointer(const char *id, Type *return_type, std::vector<FPParameter*> parameters)
{
  this->identifier_  = id;
  this->return_type_ = return_type;
  this->parameters_  = parameters;
}

CCSTTypeName* FunctionPointer::accept(TypeNameVisitor *worker)
{
  return worker->visit(this);
}

CCSTStatement* FunctionPointer::accept(AllocateTypeVisitor *worker, Variable *v)
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
/*
Marshal_type* Typedef::accept(MarshalVisitor *worker, Registers *data)
{
  return worker->visit(this, data);
}
*/

CCSTStatement* Typedef::accept(AllocateTypeVisitor *worker, Variable *v)
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
/*
Marshal_type* VoidType::accept(MarshalVisitor *worker, Registers *data)
{
  return worker->visit(this, data);
}
*/

CCSTStatement* VoidType::accept(AllocateTypeVisitor *worker, Variable *v)
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

/*
Marshal_type* IntegerType::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}
*/

CCSTStatement* IntegerType::accept(AllocateTypeVisitor *worker, Variable *v)
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

/* pointer type */

PointerType::PointerType(Type* type)
{
  this->type_ = type;
}

int PointerType::num()
{
  return 3;
}

Type* PointerType::type()
{
  return this->type_;
}

/*
Marshal_type* PointerType::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}
*/

CCSTStatement* PointerType::accept(AllocateTypeVisitor *worker, Variable *v)
{
  return worker->visit(this, v);
}

const char* PointerType::name()
{
  return this->type_->name();
}

/* end */

/* projection type */

ProjectionType::ProjectionType(const char* id, const char* real_type, std::vector<ProjectionField*> fields)
{
  this->id_ = id; this->real_type_ = real_type; this->fields_ = fields;
}

/*
Marshal_type* ProjectionType::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}
*/

CCSTStatement* ProjectionType::accept(AllocateTypeVisitor *worker, Variable *v)
{
  return worker->visit(this, v);
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

