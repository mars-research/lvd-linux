#include "lcd_ast.h"

const char* new_name(const char* name, const char* suffix)
{
  int length = strlen(name);
  int length2 = strlen(suffix);
  char *new_str = (char*) malloc(sizeof(char)*(length+length2+1));
  
  std::ostringstream total;
  total << name << suffix;
  strncpy(new_str, total.str().c_str(), length+length2+1);
  return new_str;
}

/* function pointer type*/
const char* container_name(const char* name)
{
  return new_name(name, "_container");
}

const char* hidden_args_name(const char* name)
{
  return new_name(name, "_hidden_args");
}

Function::Function(const char *id, ReturnVariable *return_var, std::vector<Parameter*> parameters, LexicalScope *ls)
{
  this->identifier_  = id;
  this->return_var_ = return_var;
  this->parameters_  = parameters;
  this->current_scope_ = ls;
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

void Function::create_trampoline_structs(LexicalScope *ls)
{
  return;
}

Rpc* Function::to_rpc(ProjectionType *pt)
{
  // adding extra parameters here. but maybe depending on needs this could be done at parse time
  // and these extra parameters can be added to the Function.
  std::vector<Parameter*> new_parameters;
  new_parameters.insert(new_parameters.end(), this->parameters_.begin(), this->parameters_.end());
  int err;
  Type *dstore = this->current_scope_->lookup("dstore", &err);
  if(dstore == 0x0) {
    printf("Error: dstore is not in scope\n");
  }
  new_parameters.push_back(new Parameter(dstore, "dstore", 1));

  const char* c_name = container_name(pt->name());
  Type *container = this->current_scope_->lookup(c_name, &err);
  if(container == 0x0) {
    printf("Error: container is not in scope\n");
  }

  new_parameters.push_back(new Parameter(container, c_name, 1));

  Rpc* tmp = new Rpc(this->return_var_, this->identifier_, new_parameters, this->current_scope_);
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

void UnresolvedType::create_trampoline_structs(LexicalScope *ls)
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

void Channel::create_trampoline_structs(LexicalScope *ls)
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

void Typedef::create_trampoline_structs(LexicalScope *ls)
{
  return; //todo?
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

void VoidType::create_trampoline_structs(LexicalScope *ls)
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

void IntegerType::create_trampoline_structs(LexicalScope *ls)
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

void ProjectionType::create_trampoline_structs(LexicalScope *ls)
{
  for(std::vector<ProjectionField*>::iterator it = this->fields_.begin(); it != this->fields_.end(); it ++) {
    ProjectionField *pf = (ProjectionField*) *it;
    if (pf->type()->num() == 7) { // function pointer
      Function *f = dynamic_cast<Function*>(pf->type());
      Assert(f != 0x0, "Error: dynamic cast to function type failed!\n");
      
      std::vector<ProjectionField*> trampoline_fields;
      int err;
      trampoline_fields.push_back(new ProjectionField(ls->lookup(container_name(this->name()), &err)
						      ,container_name(this->name()), 1)); // container field
      trampoline_fields.push_back(new ProjectionField(ls->lookup("dstore", &err), "dstore", 1)); // dstore field
      trampoline_fields.push_back(new ProjectionField(ls->lookup("lcd_trampoline_handle", &err), "t_handle", 1)); // lcd_trampoline handle field

      const char* trampoline_struct_name = hidden_args_name(f->name());
      ls->insert(trampoline_struct_name, new ProjectionType(trampoline_struct_name, trampoline_struct_name, trampoline_fields));
    }
  }
}

ProjectionField* ProjectionType::get_field(const char *field_name)
{
  for(std::vector<ProjectionField*>::iterator it = this->fields_.begin(); it != this->fields_.end(); it ++) {
    ProjectionField *pf = *it;
    if (strcmp(field_name, pf->identifier()) == 0) {
      return pf;
    }
  }
  return 0x0;
}

/* end */

