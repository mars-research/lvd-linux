#include "lcd_ast.h"
#include <stdio.h>

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

ProjectionField::ProjectionField(bool in, bool out, bool alloc, bool bind, Type* field_type, const char* field_name)
{
  this->in_ = in; 
  this->out_ = out;  
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

bool ProjectionField::in()
{
  return this->in_;
}

bool ProjectionField::out()
{
  return this->out_;
}

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

FPParameter::FPParameter(Type *type)
{
  this->type_ = type;
}

Type* FPParameter::type()
{
  return this->type_;
}

const char* FPParameter::identifier()
{
  Assert(1 == 0, "Error: operation not allowed on function pointer parameter\n");
}

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

Rpc::Rpc(ReturnVariable *return_value, char* name, std::vector<Parameter* > parameters)
{
  this->explicit_return_ = return_value;
  this->name_ = name;
  this->parameters_ = parameters;
  this->set_implicit_returns();
  this->symbol_table_ = new SymbolTable();
  
  for(std::vector<Parameter*>::iterator it = parameters.begin(); it != parameters.end(); it ++)
    {
      Parameter *p = (Parameter*) *it;
      this->symbol_table_->insert(p->identifier());
    }
}

ReturnVariable* Rpc::return_value()
{
  return this->explicit_return_;
}

void Rpc::set_implicit_returns()
{
  
  std::vector<ImplicitReturnVariable*> implicit_returns;

  for(std::vector<Parameter*>::iterator it = this->parameters_.begin(); it != this->parameters_.end(); it ++)
    {
      Parameter *p = (Parameter*) *it;
      
      if(p->type()->num() == 3) // Pointer //make an enum......
	{
	  implicit_returns.push_back(new ImplicitReturnVariable(p));
	}
    }
  this->implicit_returns_ = implicit_returns;
}

 char* Rpc::name()
{
  return name_;
}

/*
Marshal_type* Rpc::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}
*/

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
  return parameters_;
}

