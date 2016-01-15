#include "lcd_ast.h"

/* function pointer type*/
const char* container_name(const char* name)
{
  int length = strlen(name);
  int length2 = strlen("_container");
  char *new_str = (char*) malloc(sizeof(char)*(length+length2+1));
  
  std::ostringstream total;
  total << name << "_container";
  strncpy(new_str, total.str().c_str(), length+length2+1);
  return new_str;
}

Function::Function(const char *id, ReturnVariable *return_var, std::vector<Parameter*> parameters)
{
  this->identifier_  = id;
  this->return_var_ = return_var;
  this->parameters_  = parameters;
}

Marshal_type* Function::accept(MarshalPrepareVisitor *worker)
{
  return worker->visit(this);
}

CCSTTypeName* Function::accept(TypeNameVisitor *worker)
{
  return worker->visit(this);
}

CCSTStatement* Function::accept(TypeVisitor *worker, Variable *v)
{
  return worker->visit(this, v);
}

int Function::num()
{
  printf("num todo for function pointer\n");
  return 7;
}

const char* Function::name()
{
  return this->identifier_;
}

void Function::resolve_types(LexicalScope *ls)
{
  return;
}

Rpc* Function::to_rpc(LexicalScope *ls, ProjectionType *pt)
{
  // adding extra parameters here. but maybe depending on needs this could be done at parse time
  // and these extra parameters can be added to the Function.
  std::vector<Parameter*> new_parameters;
  new_parameters.insert(new_parameters.end(), this->parameters_.begin(), this->parameters_.end());
  int err;
  Type *dstore = ls->lookup("dstore", &err);
  if(dstore == 0x0) {
    printf("Error: dstore is not in scope\n");
  }
  new_parameters.push_back(new Parameter(dstore, "dstore", 1));

  const char* c_name = container_name(pt->name());
  Type *container = ls->lookup(c_name, &err);
  if(container == 0x0) {
    printf("Error: container is not in scope\n");
  }

  new_parameters.push_back(new Parameter(container, c_name, 1));

  Rpc* tmp = new Rpc(this->return_var_, this->identifier_, new_parameters, ls);
  tmp->set_function_pointer_defined(true);
  return tmp;
}

/* end */

UnresolvedType::UnresolvedType(const char *name)
{
  this->type_name_ = name;
}

Marshal_type* UnresolvedType::accept(MarshalPrepareVisitor *worker)
{
  // todo
}

CCSTTypeName* UnresolvedType::accept(TypeNameVisitor *worker)
{
  // todo
}

CCSTStatement* UnresolvedType::accept(TypeVisitor *worker, Variable *v)
{
  // todo
}

int UnresolvedType::num()
{
  return 8;
}

const char* UnresolvedType::name()
{
  return this->type_name_;
}

void UnresolvedType::resolve_types(LexicalScope *ls)
{
  return;
}


Channel::Channel()
{
}

Marshal_type* Channel::accept(MarshalPrepareVisitor *worker)
{
  return worker->visit(this);
}
CCSTTypeName* Channel::accept(TypeNameVisitor *worker)
{
  return worker->visit(this);
}
CCSTStatement* Channel::accept(TypeVisitor *worker, Variable *v)
{
  return worker->visit(this, v);
}
const char* Channel::name()
{
  return "channel";
}
int Channel::num()
{
  return 6;
}

void Channel::resolve_types(LexicalScope *ls)
{
  return;
}

/* typedef type */


Typedef::Typedef(const char* id, const char* alias, Type* type)
{
  this->identifier_ = id;
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
  return this->identifier_;
}

void Typedef::resolve_types(LexicalScope *ls)
{
  return;
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

void VoidType::resolve_types(LexicalScope *ls)
{
  return;
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

void IntegerType::resolve_types(LexicalScope *ls)
{
  return;
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

void ProjectionType::resolve_types(LexicalScope *ls)
{
  for(std::vector<ProjectionField*>::iterator it = this->fields_.begin(); it != this->fields_.end(); it ++) {
    ProjectionField *pf = (ProjectionField*) *it;
    pf->resolve_types(ls);
  }
}

/* end */

