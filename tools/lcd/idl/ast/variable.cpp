#include "lcd_ast.h"

/* global variable */

GlobalVariable::GlobalVariable(Type *type, const char *id, int pointer_count)
{
  this->type_ = type;
  this->id_ = id;
  this->pointer_count_ = pointer_count;
}

void GlobalVariable::prepare_marshal(MarshalPrepareVisitor *worker)
{
  printf("global variable prepare marshal todo\n");
}

Type* GlobalVariable::type()
{
  return this->type_;
}

const char* GlobalVariable::identifier()
{
  return this->id_;
}

void GlobalVariable::set_accessor(Variable *v)
{
  printf("Error this operation not allowed\n");
}

Variable* GlobalVariable::accessor()
{
  printf("Error this operation is not allowed\n");
  return 0x0;
}

void GlobalVariable::set_marshal_info(Marshal_type *mt)
{
  this->marshal_info_ = mt;
}

Marshal_type* GlobalVariable::marshal_info()
{
  return this->marshal_info_;
}

int GlobalVariable::pointer_count()
{
  return this->pointer_count_;
}

void GlobalVariable::set_in(bool b)
{
  printf("this operation is now allowed\n");
}

void GlobalVariable::set_out(bool b)
{
  printf("this operation is now allowed\n");
}

void GlobalVariable::set_alloc(bool b)
{
  printf("this operation is now allowed\n");
}

void GlobalVariable::set_alloc_caller(bool b)
{
  printf("this operation is now allowed\n");
}

void GlobalVariable::set_alloc_callee(bool b)
{
  printf("this operation is now allowed\n");
}

void GlobalVariable::set_alloc_callee_caller(bool b)
{
  printf("this operation is now allowed\n");
}

bool GlobalVariable::in()
{
  printf("this operation is now allowed\n");
  return false;
}

bool GlobalVariable::out()
{
  printf("this operation is now allowed\n");
  return false;
}
bool GlobalVariable::alloc()
{
  printf("this operation is now allowed\n");
  return false;
}
bool GlobalVariable::alloc_caller()
{
  printf("this operation is now allowed\n");
  return false;
}
bool GlobalVariable::alloc_callee()
{
  printf("this operation is now allowed\n");
  return false;
}
bool GlobalVariable::alloc_callee_caller()
{
  printf("this operation is now allowed\n");
  return false;
}

// probably more functions needed

/* end */

/* parameter */

Parameter::Parameter()
{
  this->type_ = 0x0;
  this->name_ = "";
  this->pointer_count_ = 0;
  this->in_ = false;
  this->out_ = false;
  this->alloc_ = false;
  this->alloc_callee_ = false;
  this->alloc_caller_ = false;
  this->alloc_callee_caller_ = false;
}

Parameter::Parameter(Type* type, const char* name, int pointer_count)
{
  this->type_ = type;
  this->name_ = name;
  this->pointer_count_ = pointer_count;
  this->in_ = false;
  this->out_ = false;
  this->alloc_ = false;
  this->alloc_callee_ = false;
  this->alloc_caller_ = false;
  this->alloc_callee_caller_ = false;
}

void Parameter::prepare_marshal(MarshalPrepareVisitor *worker)
{
  this->marshal_info_ = this->type_->accept(worker);
}

const char* Parameter::identifier()
{
  return this->name_;
}

Type* Parameter::type()
{
  return this->type_;
}

int Parameter::pointer_count()
{
  return this->pointer_count_;
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

void Parameter::set_in(bool b)
{
  this->in_ = b;
}

void Parameter::set_out(bool b)
{
  this->out_ = b;
}

void Parameter::set_alloc(bool b)
{
  this->alloc_ = b;
}

void Parameter::set_alloc_caller(bool b)
{
  this->alloc_caller_ = b;
}

void Parameter::set_alloc_callee(bool b)
{
  this->alloc_callee_ = b;
}

void Parameter::set_alloc_callee_caller(bool b)
{
  this->alloc_callee_caller_ = b;
}

bool Parameter::in()
{
  return this->in_;
}

bool Parameter::out()
{
  return this->out_;
}

bool Parameter::alloc()
{
  return this->alloc_;
}

bool Parameter::alloc_caller()
{
  return this->alloc_caller_;
}
bool Parameter::alloc_callee()
{
  return this->alloc_callee_;
}

bool Parameter::alloc_callee_caller()
{
  return this->alloc_callee_caller_;
}

/* end */

/* Return Variable */


ReturnVariable::ReturnVariable()
{

}

ReturnVariable::ReturnVariable(Type *return_type, int pointer_count)
{
  this->type_ = return_type;
  this->name_ = "";
  this->pointer_count_ = pointer_count;
}

void ReturnVariable::prepare_marshal(MarshalPrepareVisitor *worker)
{
  this->marshal_info_ = this->type_->accept(worker);
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

int ReturnVariable::pointer_count()
{
  return this->pointer_count_;
}

Variable* ReturnVariable::accessor()
{
  return this->accessor_;
}

void ReturnVariable::set_in(bool b)
{
  printf("error this operation not allowed\n");
}

void ReturnVariable::set_out(bool b)
{
  printf("error this operation not allowed\n");
}

void ReturnVariable::set_alloc(bool b)
{
  printf("error this operation not allowed\n");
}

void ReturnVariable::set_alloc_caller(bool b)
{
  printf("error this operation not allowed\n");
}

void ReturnVariable::set_alloc_callee(bool b)
{
  printf("error this operation not allowed\n");
}

void ReturnVariable::set_alloc_callee_caller(bool b)
{
  printf("error this operation not allowed\n");
}

bool ReturnVariable::in()
{
  printf("error this operation not allowed\n");
  return false;
}

bool ReturnVariable::out()
{
  printf("error this operation not allowed\n");
  return false;
}

bool ReturnVariable::alloc()
{
  printf("error this operation not allowed\n");
  return false;
}

bool ReturnVariable::alloc_caller()
{
  printf("error this operation not allowed\n");
  return false;
}
bool ReturnVariable::alloc_callee()
{
  printf("error this operation not allowed\n");
  return false;
}

bool ReturnVariable::alloc_callee_caller()
{
  printf("error this operation not allowed\n");
  return false;
}

/* end */

/* projection field */

ProjectionField::ProjectionField(Type* field_type, const char* field_name, int pointer_count)
{
  this->in_ = false; 
  this->out_ = false;
  this->alloc_ = false;
  this->alloc_callee_ = false;
  this->alloc_caller_ = false;
  this->alloc_callee_caller_ = false;
  this->field_type_ = field_type; 
  this->field_name_ = field_name;
  this->pointer_count_ = pointer_count;
}

void ProjectionField::prepare_marshal(MarshalPrepareVisitor *worker)
{
  this->marshal_info_ = this->field_type_->accept(worker);
}

Type* ProjectionField::type()
{
  return this->field_type_;
}

int ProjectionField::pointer_count()
{
  return this->pointer_count_;
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

void ProjectionField::set_marshal_info(Marshal_type *mt)
{
  this->marshal_info_ = mt;
}

Marshal_type* ProjectionField::marshal_info()
{
  return this->marshal_info_;
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

FPParameter::FPParameter(Type *type, int pointer_count)
{
  this->type_ = type;
  this->pointer_count_ = pointer_count;
}

void FPParameter::prepare_marshal(MarshalPrepareVisitor *worker)
{
  this->marshal_info_ = this->type_->accept(worker);
}

Type* FPParameter::type()
{
  return this->type_;
}

int FPParameter::pointer_count()
{
  return this->pointer_count_;
}

const char* FPParameter::identifier()
{
  Assert(1 == 0, "Error: operation not allowed on function pointer parameter\n");
}

void FPParameter::set_marshal_info(Marshal_type *mt)
{
  this->marshal_info_ = mt;
}

Marshal_type* FPParameter::marshal_info()
{
  return this->marshal_info_;
}

void FPParameter::set_in(bool b)
{
  printf("this operation is now allowed\n");
}

void FPParameter::set_out(bool b)
{
  printf("this operation is now allowed\n");
}

void FPParameter::set_alloc(bool b)
{
  printf("this operation is now allowed\n");
}

void FPParameter::set_alloc_caller(bool b)
{
  printf("this operation is now allowed\n");
}

void FPParameter::set_alloc_callee(bool b)
{
  printf("this operation is now allowed\n");
}

void FPParameter::set_alloc_callee_caller(bool b)
{
  printf("this operation is now allowed\n");
}

bool FPParameter::in()
{
  printf("this operation is now allowed\n");
  return false;
}

bool FPParameter::out()
{
  printf("this operation is now allowed\n");
  return false;
}
bool FPParameter::alloc()
{
  printf("this operation is now allowed\n");
  return false;
}
bool FPParameter::alloc_caller()
{
  printf("this operation is now allowed\n");
  return false;
}
bool FPParameter::alloc_callee()
{
  printf("this operation is now allowed\n");
  return false;
}
bool FPParameter::alloc_callee_caller()
{
  printf("this operation is now allowed\n");
  return false;
}

/* end */

