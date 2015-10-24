#include "lcd_ast.h"

/* global variable */

GlobalVariable::GlobalVariable(Type *type, const char *id)
{
  this->type_ = type;
  this->id_ = id;
}

Type* GlobalVariable::type()
{
  return this->type_;
}

const char* GlobalVariable::identifier()
{
  return this->id_;
}

// probably more functions needed

/* end */

/* parameter */

Parameter::Parameter()
{
  this->type_ = 0x0;
  this->name_ = "";
}

Parameter::Parameter(Type* type, const char* name)
{
  this->type_ = type;
  this->name_ = name;
}

/*
Marshal_type* Parameter::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}
*/

const char* Parameter::identifier()
{
  return this->name_;
}

Type* Parameter::type()
{
  return this->type_;
}

Rpc* Parameter::scope()
{
  return this->function_;
}

void Parameter::set_accessor(Variable *v)
{
  this->accessor_ = v;
}

Variable* Parameter::accessor()
{
  return this->accessor_;
}

void Parameter::set_marshal_info(Marshal_type* mt)
{
  this->marshal_info_ = mt;
}

Marshal_type* Parameter::marshal_info()
{
  return this->marshal_info_;
}

/* end */

/* Return Variable */


ReturnVariable::ReturnVariable()
{

}

ReturnVariable::ReturnVariable(Type *return_type)
{
  this->type_ = return_type;
  this->name_ = "";
}

void ReturnVariable::set_marshal_info(Marshal_type *mt)
{
  this->marshal_info_ = mt;
}

Marshal_type* ReturnVariable::marshal_info()
{
  return this->marshal_info_;
}

const char* ReturnVariable::identifier()
{
  return this->name_;
}

Type* ReturnVariable::type()
{
  return this->type_;
}

void ReturnVariable::set_accessor(Variable *v)
{
  this->accessor_ = v;
}

Rpc* ReturnVariable::scope()
{
  return this->function_;
}

Variable* ReturnVariable::accessor()
{
  return this->accessor_;
}

/* end */

/* implicit return variable */


ImplicitReturnVariable::ImplicitReturnVariable(Parameter *p)
{
  this->p_ = p;
}

void ImplicitReturnVariable::set_marshal_info(Marshal_type *mt)
{
  this->marshal_info_ = mt;
}

Marshal_type* ImplicitReturnVariable::marshal_info()
{
  return this->marshal_info_;
}

void ImplicitReturnVariable::set_accessor(Variable *v)
{
  this->accessor_ = v;
}

Type* ImplicitReturnVariable::type()
{
  return this->p_->type();
}

const char* ImplicitReturnVariable::identifier()
{
  return this->p_->identifier();
}

Rpc* ImplicitReturnVariable::scope()
{
  return this->function_;
}

Variable* ImplicitReturnVariable::accessor()
{
  return this->accessor_;
}

/* end */

/* projection field */

ProjectionField::ProjectionField(Type* field_type, const char* field_name)
{
  this->in_ = false; 
  this->out_ = false;
  this->alloc_ = false;
  this->alloc_callee_ = false;
  this->alloc_caller_ = false;
  this->alloc_callee_caller_ = false;
  this->field_type_ = field_type; 
  this->field_name_ = field_name;
}

/*
Marshal_type* ProjectionField::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}
*/

Type* ProjectionField::type()
{
  return this->field_type_;
}

Rpc* ProjectionField::scope()
{
  return this->accessor_->scope();
}

const char* ProjectionField::identifier()
{
  return this->field_name_;
}

void ProjectionField::set_accessor(Variable *v)
{
  this->accessor_ = v;
}

Variable* ProjectionField::accessor()
{
  return this->accessor_;
}

void ProjectionField::set_in(bool b)
{
  this->in_ = b;
}

void ProjectionField::set_out(bool b)
{
  this->out_ = b;
}

void ProjectionField::set_alloc(bool b)
{
  this->alloc_ = b;
}

void ProjectionField::set_alloc_callee(bool b)
{
  this->alloc_callee_ = b;
}

void ProjectionField::set_alloc_caller(bool b)
{
  this->alloc_caller_ = b;
}

void ProjectionField::set_alloc_callee_caller(bool b)
{
  this->alloc_callee_caller_ = b;
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

bool ProjectionField::alloc_callee()
{
  return this->alloc_callee_;
}

bool ProjectionField::alloc_caller()
{
  return this->alloc_caller_;
}

bool ProjectionField::alloc_callee_caller()
{
  return this->alloc_callee_caller_;
}

/* end */

