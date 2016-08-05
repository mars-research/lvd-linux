#include "lcd_ast.h"
#include "code_gen.h"

/* global variable */

GlobalVariable::GlobalVariable(Type *type, const char *id, int pointer_count)
{
  this->type_ = type;
  this->id_ = id;
  this->pointer_count_ = pointer_count;
  this->container_ = 0x0;
  this->accessor_ = 0x0;
}

GlobalVariable::GlobalVariable(const GlobalVariable& other)
{
  // copy type
  this->type_ = other.type_->clone();

  this->accessor_ = other.accessor_;
  
  // copy id
  char* id_copy = (char*) malloc(sizeof(char)*(strlen(other.id_)+1));
  strcpy(id_copy, other.id_);
  this->id_ = id_copy;
  // copy marshal info
  if (other.marshal_info_ != 0x0) {
    this->marshal_info_ = other.marshal_info_->clone(); // copy for real?
  } else {
    this->marshal_info_ = 0x0;
  }
  
  // copy container
  if (other.container_ != 0x0) {
    this->container_ = other.container_->clone();
  } else {
    this->container_ = 0x0;
  }
}

void GlobalVariable::prepare_marshal(MarshalPrepareVisitor *worker)
{
  if (this->container_ != 0x0) {
    this->container_->prepare_marshal(worker);
  }

  this->marshal_info_ = this->type_->accept(worker);
}

Type* GlobalVariable::type()
{
  return this->type_;
}

const char* GlobalVariable::identifier()
{
  return this->id_;
}

void GlobalVariable::set_identifier(const char* id)
{
  this->id_ = id;
}

void GlobalVariable::set_accessor(Variable *v)
{
  this->accessor_ = v;
  
  if(this->container_ != 0x0) {
    this->container_->set_accessor(0x0);
  }

  if(this->type_->num() == 4 || this->type_->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(this->type_);
    Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");

    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      pf->set_accessor(this);
    }
  }
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

void GlobalVariable::set_pointer_count(int pcount)
{
  this->pointer_count_ = pcount;
}

void GlobalVariable::resolve_types(LexicalScope *ls)
{
  // need to rewrite to account for initializetype
  Type *last = this->type_;
  Type *tmp = this->type_;
  
  if(this->type_->num() == 10) {
    Type *tmp = this->type_;
    InitializeType *it = 0x0;
    while(tmp->num() == 10) {
      it = dynamic_cast<InitializeType*>(tmp);
      Assert(it != 0x0, "Error: dynamic cast to initialize type failed\n");
      tmp = it->type_;
    }
    
    if(tmp->num() != 8) {
      return;
    }

    int err;
    Type *t = ls->lookup(tmp->name(), &err);
    if(t == 0x0) {
      printf("Error: could not resolve type %s\n", tmp->name());
      return;
    }

    it->type_ = t;
    return;

  } else {

    // check if unresolved
    if(this->type_->num() != 8) {
      return;
    }
    
    int err;
    Type *t = ls->lookup(this->type_->name(), &err);
    if(t == 0x0) {
      printf("Error: could not resolve type %s\n", this->type_->name());
      return;
    } 
    
    // and set
    this->type_ = t;
    return;
  }
}

void GlobalVariable::initialize_type()
{
  if ( this->type_->num() == 10 ) {
    InitializeType *it = dynamic_cast<InitializeType*>(this->type_);
    Assert(it != 0x0, "Error: dynamic cast to Initialize type failed\n");
    
    it->initialize();
    this->type_ = it->type_;
  } else if (this->type_->num() == 4 || this->type_->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(this->type_);
    Assert(pt != 0x0, "Error: dynamic cast to Projection type failed\n");

    pt->initialize_type();
  }
}

void GlobalVariable::create_container_variable(LexicalScope *ls)
{
  if(this->pointer_count() <= 0 || (this->type_->num() != 4 && this->type_->num() != 9 && this->type_->num() != 10)) {
    return;
  }
  Type *tmp = this->type_;

  if(this->type_->num() == 10) { // initialize type
    while(tmp->num() == 10) {
      InitializeType *it = dynamic_cast<InitializeType*>(tmp);
      Assert(it != 0x0, "Error: dynamic cast to initialize type failed\n");

      tmp = it->type_;
    }

    if(tmp->num() != 4 && tmp->num() != 9) {
      return;
    }
  }
  
  // lookup in scope the container for its type. 
  int err;
  Type *container_t = ls->lookup(container_name(tmp->name()), &err);
  Assert(container_t != 0x0, "Error: could not find container in scope\n");

  ProjectionType *container = dynamic_cast<ProjectionType*>(container_t);
  Assert(container != 0x0, "Error: could not dynamically cast to projection\n");

  // really need to make variable non abstract and get rid of unnecessary variables.

  const char* name = container_name(append_strings("_", construct_list_vars(this)));

  Parameter *container_var = new Parameter(container, name, 1);
  container_var->set_in(this->in());
  container_var->set_out(this->out());

  // save. 
  this->container_ = container_var;
  
  ProjectionType *pt = dynamic_cast<ProjectionType*>(tmp);
  Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
  
  std::vector<ProjectionField*> fields = pt->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    ProjectionField *pf = *it;
    pf->create_container_variable(ls);
  }
}

void GlobalVariable::set_in(bool b)
{
  printf("this operation is not allowed\n");
}

void GlobalVariable::set_out(bool b)
{
  printf("this operation is not allowed\n");
}

void GlobalVariable::set_alloc_caller(bool b)
{
  printf("this operation is not allowed\n");
}

void GlobalVariable::set_alloc_callee(bool b)
{
  printf("this operation is not allowed\n");
}

void GlobalVariable::set_dealloc_caller(bool b)
{
  printf("this operation is not allowed\n");
}

void GlobalVariable::set_dealloc_callee(bool b)
{
  printf("this operation is not allowed\n");
}

bool GlobalVariable::in()
{
  printf("this operation is not allowed\n");
  return false;
}

bool GlobalVariable::out()
{
  printf("this operation is not allowed\n");
  return false;
}

bool GlobalVariable::alloc_caller()
{
  printf("this operation is not allowed\n");
  return false;
}
bool GlobalVariable::alloc_callee()
{
  printf("this operation is not allowed\n");
  return false;
}

bool GlobalVariable::dealloc_caller()
{
  printf("this operation is not allowed\n");
  return false;
}

bool GlobalVariable::dealloc_callee()
{
  printf("this operation is not allowed\n");
  return false;
}

Variable* GlobalVariable::container()
{
  return this->container_;
}

void GlobalVariable::set_bind_caller(bool b)
{
  printf("this operation is not allowed\n");
}

void GlobalVariable::set_bind_callee(bool b)
{
  printf("this operation is not allowed\n");
}

bool GlobalVariable::bind_caller()
{
  printf("this operation is not allowed\n");
  return false;
}

bool GlobalVariable::bind_callee()
{
  printf("this operation is not allowed\n");
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
  this->alloc_callee_ = false;
  this->alloc_caller_ = false;
  this->dealloc_callee_ = false;
  this->dealloc_caller_ = false;
  this->container_ = 0x0;
}

Parameter::Parameter(Type* type, const char* name, int pointer_count)
{
  this->type_ = type;
  this->name_ = name;
  this->pointer_count_ = pointer_count;
  this->in_ = false;
  this->out_ = false;
  this->alloc_callee_ = false;
  this->alloc_caller_ = false;
  this->dealloc_callee_ = false;
  this->dealloc_caller_ = false;
}

Parameter::Parameter(const Parameter& other)
{
  this->type_ = other.type_->clone();

  // clone name
  char* name_copy = (char*) malloc(sizeof(char)*(strlen(other.name_)+1));
  strcpy(name_copy, other.name_);
  this->name_ = name_copy;
  this->pointer_count_ = other.pointer_count_;
  this->in_ = other.in_;
  this->out_ = other.out_;
  this->alloc_callee_ = other.alloc_callee_;
  this->alloc_caller_ = other.alloc_caller_;
  this->dealloc_callee_ = other.dealloc_callee_;
  this->dealloc_caller_ = other.dealloc_caller_;

  if(other.container_ != 0x0) {
    this->container_ = other.container_->clone();
  } else {
    this->container_ = 0x0;
  }
}

void Parameter::create_container_variable(LexicalScope *ls)
{
  if(this->pointer_count() <= 0 || (this->type_->num() != 4 && this->type_->num() != 9 && this->type_->num() != 10) || (!this->bind_caller() && !this->bind_callee() && !this->alloc_caller() && !this->alloc_callee())) {
    return;
  }
  Type *tmp = this->type_;

  if(this->type_->num() == 10) { // initialize type
    while(tmp->num() == 10) {
      InitializeType *it = dynamic_cast<InitializeType*>(tmp);
      Assert(it != 0x0, "Error: dynamic cast to initialize type failed\n");

      tmp = it->type_;
    }

    if(tmp->num() != 4 && tmp->num() != 9) {
      return;
    }
  }
  
  // lookup in scope the container for its type. 
  int err;
  Type *container_t = ls->lookup(container_name(tmp->name()), &err);
  printf("Just looked up container %s\n", container_name(tmp->name()));
  Assert(container_t != 0x0, "Error: could not find container in scope\n");

  ProjectionType *container = dynamic_cast<ProjectionType*>(container_t);
  Assert(container != 0x0, "Error: could not dynamically cast to projection\n");

  // really need to make variable non abstract and get rid of unnecessary variables.

  const char* name = container_name(append_strings("_", construct_list_vars(this)));

  Parameter *container_var = new Parameter(container, name, 1);
  container_var->set_in(this->in());
  container_var->set_out(this->out());

  // save. 
  this->container_ = container_var;

  // recurse

  ProjectionType *pt = dynamic_cast<ProjectionType*>(tmp);
  Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
  
  std::vector<ProjectionField*> fields = pt->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    ProjectionField *pf = *it;
    pf->create_container_variable(ls);
  }
}

Variable* Parameter::container()
{
  return this->container_;
}

void Parameter::prepare_marshal(MarshalPrepareVisitor *worker)
{
  // if 
  if (this->container_ != 0x0) {
    this->container_->prepare_marshal(worker);
  }
  
  this->marshal_info_ = this->type_->accept(worker);
}

const char* Parameter::identifier()
{
  return this->name_;
}

void Parameter::set_identifier(const char* id)
{
  this->name_ = id;
}

Type* Parameter::type()
{
  return this->type_;
}

int Parameter::pointer_count()
{
  return this->pointer_count_;
}

void Parameter::set_pointer_count(int pcount)
{
  this->pointer_count_ = pcount;
}

void Parameter::set_accessor(Variable *v)
{
  this->accessor_ = v;
  if(this->container_ != 0x0) {
    this->container_->set_accessor(0x0);
  }
  
  if(this->type_->num() == 4 || this->type_->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(this->type_);
    Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
    
    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      pf->set_accessor(this);
    }
  }
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

void Parameter::resolve_types(LexicalScope *ls)
{
   // need to rewrite to account for initializetype
  Type *last = this->type_;
  Type *tmp = this->type_;
  
  if(this->type_->num() == 10) {
    Type *tmp = this->type_;
    InitializeType *it = 0x0;
    while(tmp->num() == 10) {
      it = dynamic_cast<InitializeType*>(tmp);
      Assert(it != 0x0, "Error: dynamic cast to initialize type failed\n");
      tmp = it->type_;
    }
    
    if(tmp->num() != 8) {
      return;
    }

    int err;
    Type *t = ls->lookup(tmp->name(), &err);
    if(t == 0x0) {
      printf("Error: could not resolve type %s\n", tmp->name());
      return;
    }

    it->type_ = t;
    return;

  } else {

    // check if unresolved
    if(this->type_->num() != 8) {
      return;
    }
    
    int err;
    Type *t = ls->lookup(this->type_->name(), &err);
    if(t == 0x0) {
      printf("Error: could not resolve type %s\n", this->type_->name());
      return;
    } 
    
    // and set
    this->type_ = t;
    return;
  }
}

void Parameter::initialize_type()
{
  if ( this->type_->num() == 10 ) {
    InitializeType *it = dynamic_cast<InitializeType*>(this->type_);
    Assert(it != 0x0, "Error: dynamic cast to Initialize type failed\n");
    
    it->initialize();
    this->type_ = it->type_;
  } else if (this->type_->num() == 4 || this->type_->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(this->type_);
    Assert(pt != 0x0, "Error: dynamic cast to Projection type failed\n");

    pt->initialize_type();
  }
}

void Parameter::set_in(bool b)
{
  this->in_ = b;
}

void Parameter::set_out(bool b)
{
  this->out_ = b;
}

void Parameter::set_alloc_caller(bool b)
{
  this->alloc_caller_ = b;
}

void Parameter::set_alloc_callee(bool b)
{
  this->alloc_callee_ = b;
}

void Parameter::set_dealloc_caller(bool b)
{
  this->dealloc_caller_ = b;
}

void Parameter::set_dealloc_callee(bool b)
{
  this->dealloc_callee_ = b;
}

bool Parameter::in()
{
  return this->in_;
}

bool Parameter::out()
{
  return this->out_;
}

bool Parameter::alloc_caller()
{
  return this->alloc_caller_;
}
bool Parameter::alloc_callee()
{
  return this->alloc_callee_;
}

bool Parameter::dealloc_caller()
{
  return this->dealloc_caller_;
}

bool Parameter::dealloc_callee()
{
  return this->dealloc_callee_;
}

void Parameter::set_bind_caller(bool b)
{
  this->bind_caller_ = b;
}

void Parameter::set_bind_callee(bool b)
{
  this->bind_callee_ = b;
}

bool Parameter::bind_caller()
{
  return this->bind_caller_;
}

bool Parameter::bind_callee()
{
  return this->bind_callee_;
}

/* end */

/* Return Variable */


ReturnVariable::ReturnVariable()
{

}

ReturnVariable::ReturnVariable(Type *return_type, int pointer_count, const char* id)
{
  this->type_ = return_type;
  this->name_ = id;
  this->pointer_count_ = pointer_count;
  this->container_ = 0x0;
}

ReturnVariable::ReturnVariable(const ReturnVariable& other)
{
  // copy name
  char* name_copy = (char*) malloc(sizeof(char)*(strlen(other.name_)+1));
  strcpy(name_copy, other.name_);
  this->name_ = name_copy;

  // copy type
  this->type_ = other.type_->clone();

  // copy marshal info
  if(other.marshal_info_ != 0x0) {
    this->marshal_info_ = other.marshal_info_->clone();
  } else {
    this->marshal_info_ = 0x0;
  }

  // copy accessor
  this->accessor_ = other.accessor_;

  this->pointer_count_ = other.pointer_count_;

  if(other.container_ != 0x0) {
    this->container_ = other.container_->clone();
  } else {
    this->container_ = 0x0;
  }
  
}

void ReturnVariable::create_container_variable(LexicalScope *ls)
{
    if(this->pointer_count() <= 0 || (this->type_->num() != 4 && this->type_->num() != 9 && this->type_->num() != 10)) {
    return;
  }
  Type *tmp = this->type_;

  if(this->type_->num() == 10) { // initialize type

    while(tmp->num() == 10) {
      InitializeType *it = dynamic_cast<InitializeType*>(tmp);
      Assert(it != 0x0, "Error: dynamic cast to initialize type failed\n");

      tmp = it->type_;
    }

    if(tmp->num() != 4 && tmp->num() != 9) {
      return;
    }
  }

  // lookup in scope the container for its type. 
  int err;
  Type *container_t = ls->lookup(container_name(tmp->name()), &err);
  Assert(container_t != 0x0, "Error: could not find container in scope\n");

  ProjectionType *container = dynamic_cast<ProjectionType*>(container_t);
  Assert(container != 0x0, "Error: could not dynamically cast to projection\n");

  // really need to make variable non abstract and get rid of unnecessary variables.

  const char* name = container_name(append_strings("_", construct_list_vars(this)));

  ReturnVariable *container_var = new ReturnVariable(container, 1, "");
  container_var->set_in(this->in());
  container_var->set_out(this->out());
  
  // save. 
  this->container_ = container_var;

  ProjectionType *pt = dynamic_cast<ProjectionType*>(tmp);
  Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
  
  std::vector<ProjectionField*> fields = pt->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    ProjectionField *pf = *it;
    pf->create_container_variable(ls);
  }
}

Variable* ReturnVariable::container()
{
  return this->container_;
}

void ReturnVariable::prepare_marshal(MarshalPrepareVisitor *worker)
{
  if (this->container_ != 0x0) {
    this->container_->prepare_marshal(worker);
  }
  
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

void ReturnVariable::set_identifier(const char* id)
{
  this->name_ = id;
}

Type* ReturnVariable::type()
{
  return this->type_;
}

void ReturnVariable::set_accessor(Variable *v)
{
  this->accessor_ = v;

  if(this->container_ != 0x0) {
    this->container_->set_accessor(0x0);
  }

  if(this->type_->num() == 4 || this->type_->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(this->type_);
    Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
    
    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      pf->set_accessor(this);
    }
  }
}

int ReturnVariable::pointer_count()
{
  return this->pointer_count_;
}

void ReturnVariable::set_pointer_count(int pcount)
{
  this->pointer_count_ = pcount;
}

void ReturnVariable::resolve_types(LexicalScope *ls)
{
 // need to rewrite to account for initializetype
  Type *last = this->type_;
  Type *tmp = this->type_;
  
  if(this->type_->num() == 10) {
    Type *tmp = this->type_;
    InitializeType *it = 0x0;
    while(tmp->num() == 10) {
      it = dynamic_cast<InitializeType*>(tmp);
      Assert(it != 0x0, "Error: dynamic cast to initialize type failed\n");
      tmp = it->type_;
    }
    
    if(tmp->num() != 8) {
      return;
    }

    int err;
    Type *t = ls->lookup(tmp->name(), &err);
    if(t == 0x0) {
      printf("Error: could not resolve type %s\n", tmp->name());
      return;
    }

    it->type_ = t;
    return;

  } else {

    // check if unresolved
    if(this->type_->num() != 8) {
      return;
    }
    
    int err;
    Type *t = ls->lookup(this->type_->name(), &err);
    if(t == 0x0) {
      printf("Error: could not resolve type %s\n", this->type_->name());
      return;
    } 
    
    // and set
    this->type_ = t;
    return;
  }
}

void ReturnVariable::initialize_type()
{
  if ( this->type_->num() == 10 ) {
    InitializeType *it = dynamic_cast<InitializeType*>(this->type_);
    Assert(it != 0x0, "Error: dynamic cast to Initialize type failed\n");
    
    it->initialize();
    this->type_ = it->type_;
  } else if (this->type_->num() == 4 || this->type_->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(this->type_);
    Assert(pt != 0x0, "Error: dynamic cast to Projection type failed\n");

    pt->initialize_type();
  }
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

void ReturnVariable::set_alloc_caller(bool b)
{
  printf("error this operation not allowed\n");
}

void ReturnVariable::set_alloc_callee(bool b)
{
  printf("error this operation not allowed\n");
}

void ReturnVariable::set_dealloc_caller(bool b)
{
  printf("error this operation not allowed\n");
}

void ReturnVariable::set_dealloc_callee(bool b)
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

bool ReturnVariable::dealloc_caller()
{
  printf("error this operation not allowed\n");
  return false;
}

bool ReturnVariable::dealloc_callee()
{
  printf("error this operation not allowed\n");
  return false;
}

void ReturnVariable::set_bind_caller(bool b)
{
  printf("this operation is not allowed\n");
}

void ReturnVariable::set_bind_callee(bool b)
{
  printf("this operation is not allowed\n");
}

bool ReturnVariable::bind_caller()
{
  printf("this operation is not allowed\n");
  return false;
}

bool ReturnVariable::bind_callee()
{
  printf("this operation is not allowed\n");
  return false;
}

/* end */

/* projection field */

ProjectionField::ProjectionField(Type* field_type, const char* field_name, int pointer_count)
{
  this->in_ = false; 
  this->out_ = false;
  this->alloc_callee_ = false;
  this->alloc_caller_ = false;
  this->dealloc_callee_ = false;
  this->dealloc_caller_ = false;
  this->type_ = field_type; 
  this->field_name_ = field_name;
  this->pointer_count_ = pointer_count;
  this->container_ = 0x0;
}

ProjectionField::ProjectionField(const ProjectionField& other)
{
  this->in_ = other.in_;
  this->out_ = other.out_;
  this->alloc_callee_ = other.alloc_callee_;
  this->alloc_caller_ = other.alloc_caller_;
  this->dealloc_callee_ = other.dealloc_callee_;
  this->dealloc_caller_ = other.dealloc_caller_;
  // copy Type
  this->type_ = other.type_->clone();
  // copy field name
  char* field_name_copy = (char*) malloc(sizeof(char)*(strlen(other.field_name_)+1));
  strcpy(field_name_copy, other.field_name_);
  this->field_name_ = field_name_copy;

  this->pointer_count_ = other.pointer_count_;

  // copy marshal info
  if(other.marshal_info_ != 0x0) {
    this->marshal_info_ = other.marshal_info_->clone();
  } else {
    this->marshal_info_ = 0x0;
  }
  // copy container;
  if(other.container_ != 0x0) {
    this->container_ = other.container_->clone();
  } else {
    this->container_ = 0x0;
  }

  // copy accessor
  this->accessor_ = other.accessor_;
}

void ProjectionField::create_container_variable(LexicalScope *ls)
{
  if(this->pointer_count() <= 0 || (this->type_->num() != 4 && this->type_->num() != 9 && this->type_->num() != 10) || (!this->bind_caller() && !this->bind_callee() && !this->alloc_caller() && !this->alloc_callee())) {
    return;
  }
  Type *tmp = this->type_;
  
  if(this->type_->num() == 10) { // initialize type
    while(tmp->num() == 10) {
      InitializeType *it = dynamic_cast<InitializeType*>(tmp);
      Assert(it != 0x0, "Error: dynamic cast to initialize type failed\n");
      
      tmp = it->type_;
    }

    if(tmp->num() != 4 && tmp->num() != 9) {
      return;
    }
  }

  // lookup in scope the container for its type. 
  int err;
  Type *container_t = ls->lookup(container_name(tmp->name()), &err);
  printf("looking up container %s in pf\n", container_name(tmp->name()));
  Assert(container_t != 0x0, "Error: could not find container in scope\n");

  ProjectionType *container = dynamic_cast<ProjectionType*>(container_t);
  Assert(container != 0x0, "Error: could not dynamically cast to projection\n");

  // really need to make variable non abstract and get rid of unnecessary variables.

  const char* name = container_name(append_strings("_", construct_list_vars(this)));

  ProjectionField *container_var = new ProjectionField(container, name, 1);
  container_var->set_in(this->in());
  container_var->set_out(this->out());

  // save. 
  this->container_ = container_var;

  ProjectionType *pt = dynamic_cast<ProjectionType*>(tmp);
  Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
  
  std::vector<ProjectionField*> fields = pt->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    ProjectionField *pf = *it;
    pf->create_container_variable(ls);
  }
}

Variable* ProjectionField::container()
{
  return this->container_;
}

void ProjectionField::prepare_marshal(MarshalPrepareVisitor *worker)
{
  if (this->container_ != 0x0) {
    this->container_->prepare_marshal(worker);
  }

  this->marshal_info_ = this->type_->accept(worker);
}

Type* ProjectionField::type()
{
  return this->type_;
}

int ProjectionField::pointer_count()
{
  return this->pointer_count_;
}

void ProjectionField::set_pointer_count(int pcount)
{
  this->pointer_count_ = pcount;
}

const char* ProjectionField::identifier()
{
  return this->field_name_;
}

void ProjectionField::set_identifier(const char* id)
{
  this->field_name_ = id;
}

void ProjectionField::set_accessor(Variable *v)
{
  this->accessor_ = v;

  if (this->container_ != 0x0) {
    this->container_->set_accessor(0x0);
  }
  
  if(this->type_->num() == 4 || this->type_->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(this->type_);
    Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
    
    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      pf->set_accessor(this);
    }
  }
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

void ProjectionField::resolve_types(LexicalScope *ls)
{
 // need to rewrite to account for initializetype
  Type *last = this->type_;
  Type *tmp = this->type_;
  
  if(this->type_->num() == 10) {
    Type *tmp = this->type_;
    InitializeType *it = 0x0;
    while(tmp->num() == 10) {
      it = dynamic_cast<InitializeType*>(tmp);
      Assert(it != 0x0, "Error: dynamic cast to initialize type failed\n");
      tmp = it->type_;
    }
    
    if(tmp->num() != 8) {
      return;
    }

    int err;
    Type *t = ls->lookup(tmp->name(), &err);
    if(t == 0x0) {
      printf("Error: could not resolve type %s\n", tmp->name());
      return;
    }

    it->type_ = t;
    return;

  } else {

    // check if unresolved
    if(this->type_->num() != 8) {
      return;
    }
    
    int err;
    Type *t = ls->lookup(this->type_->name(), &err);
    if(t == 0x0) {
      printf("Error: could not resolve type %s\n", this->type_->name());
      return;
    } 
    
    // and set
    this->type_ = t;
    return;
  }
}

void ProjectionField::initialize_type()
{
  if ( this->type_->num() == 10 ) {
    InitializeType *it = dynamic_cast<InitializeType*>(this->type_);
    Assert(it != 0x0, "Error: dynamic cast to Initialize type failed\n");
    
    it->initialize();
    this->type_ = it->type_;
  } else if (this->type_->num() == 4 || this->type_->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(this->type_);
    Assert(pt != 0x0, "Error: dynamic cast to Projection type failed\n");

    pt->initialize_type();
  }
}

void ProjectionField::set_in(bool b)
{
  this->in_ = b;
}

void ProjectionField::set_out(bool b)
{
  this->out_ = b;
}

void ProjectionField::set_alloc_callee(bool b)
{
  this->alloc_callee_ = b;
}

void ProjectionField::set_alloc_caller(bool b)
{
  this->alloc_caller_ = b;
}

void ProjectionField::set_dealloc_caller(bool b)
{
  this->dealloc_caller_ = b;
}

void ProjectionField::set_dealloc_callee(bool b)
{
  this->dealloc_callee_ = b;
}

bool ProjectionField::in()
{
  return this->in_;
}

bool ProjectionField::out()
{
  return this->out_;
}

bool ProjectionField::alloc_callee()
{
  return this->alloc_callee_;
}

bool ProjectionField::alloc_caller()
{
  return this->alloc_caller_;
}

bool ProjectionField::dealloc_caller()
{
  return this->dealloc_caller_;
}

bool ProjectionField::dealloc_callee()
{
  return this->dealloc_callee_;
}

void ProjectionField::set_bind_caller(bool b)
{
  this->bind_caller_ = b;
}

void ProjectionField::set_bind_callee(bool b)
{
  this->bind_callee_ = b;
}

bool ProjectionField::bind_caller()
{
  return this->bind_caller_;
}

bool ProjectionField::bind_callee()
{
  return this->bind_callee_;
}

FPParameter::FPParameter(Type *type, int pointer_count)
{
  this->type_ = type;
  this->pointer_count_ = pointer_count;
  this->container_ = 0x0;
}

FPParameter::FPParameter(const FPParameter& other)
{
  // copy type
  this->type_ = other.type_->clone();

  // copy container
  this->container_ = other.container_->clone();

  // copy marshal info
  if (other.marshal_info_ != 0x0) {
    this->marshal_info_ = other.marshal_info_->clone();
  } else {
    this->marshal_info_ = 0x0;
  }

  this->pointer_count_ = other.pointer_count_;
}

void FPParameter::create_container_variable(LexicalScope *ls)
{
  if(this->pointer_count() <= 0 || (this->type_->num() != 4 && this->type_->num() != 9 && this->type_->num() != 10)) {
    return;
  }
  Type *tmp = this->type_;
  
  if(this->type_->num() == 10) { // initialize type
    while(tmp->num() == 10) {
      InitializeType *it = dynamic_cast<InitializeType*>(tmp);
      Assert(it != 0x0, "Error: dynamic cast to initialize type failed\n");

      tmp = it->type_;
    }

    if(tmp->num() != 4 && tmp->num() != 9) {
      return;
    }
  }

  // lookup in scope the container for its type. 
  int err;
  Type *container_t = ls->lookup(container_name(tmp->name()), &err);
  Assert(container_t != 0x0, "Error: could not find container in scope\n");

  ProjectionType *container = dynamic_cast<ProjectionType*>(container_t);
  Assert(container != 0x0, "Error: could not dynamically cast to projection\n");

  // really need to make variable non abstract and get rid of unnecessary variables.

  const char* name = container_name(append_strings("_", construct_list_vars(this)));

  FPParameter *container_var = new FPParameter(container, 1);
  container_var->set_in(this->in());
  container_var->set_out(this->out());

  // save. 
  this->container_ = container_var;

  ProjectionType *pt = dynamic_cast<ProjectionType*>(tmp);
  Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
  
  std::vector<ProjectionField*> fields = pt->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    ProjectionField *pf = *it;
    pf->create_container_variable(ls);
  }
}

Variable* FPParameter::container()
{
  return this->container_;
}

void FPParameter::prepare_marshal(MarshalPrepareVisitor *worker)
{
  if (this->container_ != 0x0) {
    this->container_->prepare_marshal(worker);
  }
  
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

void FPParameter::set_pointer_count(int pcount)
{
  this->pointer_count_ = pcount;
}

const char* FPParameter::identifier()
{
  Assert(1 == 0, "Error: operation not allowed on function pointer parameter\n");
}

void FPParameter::set_identifier(const char* id)
{
  return;
}

void FPParameter::set_marshal_info(Marshal_type *mt)
{
  this->marshal_info_ = mt;
}

Marshal_type* FPParameter::marshal_info()
{
  return this->marshal_info_;
}

void FPParameter::resolve_types(LexicalScope *ls)
{
 // need to rewrite to account for initializetype
  Type *last = this->type_;
  Type *tmp = this->type_;
  
  if(this->type_->num() == 10) {
    Type *tmp = this->type_;
    InitializeType *it = 0x0;
    while(tmp->num() == 10) {
      it = dynamic_cast<InitializeType*>(tmp);
      Assert(it != 0x0, "Error: dynamic cast to initialize type failed\n");
      tmp = it->type_;
    }
    
    if(tmp->num() != 8) {
      return;
    }

    int err;
    Type *t = ls->lookup(tmp->name(), &err);
    if(t == 0x0) {
      printf("Error: could not resolve type %s\n", tmp->name());
      return;
    }

    it->type_ = t;
    return;

  } else {

    // check if unresolved
    if(this->type_->num() != 8) {
      return;
    }
    
    int err;
    Type *t = ls->lookup(this->type_->name(), &err);
    if(t == 0x0) {
      printf("Error: could not resolve type %s\n", this->type_->name());
      return;
    } 
    
    // and set
    this->type_ = t;
    return;
  }
}

void FPParameter::initialize_type()
{
  if ( this->type_->num() == 10 ) {
    InitializeType *it = dynamic_cast<InitializeType*>(this->type_);
    Assert(it != 0x0, "Error: dynamic cast to Initialize type failed\n");
    
    it->initialize();
    this->type_ = it->type_;
  } else if (this->type_->num() == 4 || this->type_->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(this->type_);
    Assert(pt != 0x0, "Error: dynamic cast to Projection type failed\n");

    pt->initialize_type();
  }
}

void FPParameter::set_in(bool b)
{
  printf("this operation is now allowed\n");
}

void FPParameter::set_out(bool b)
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

void FPParameter::set_dealloc_caller(bool b)
{
  printf("this operation is now allowed\n");
}

void FPParameter::set_dealloc_callee(bool b)
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

bool FPParameter::dealloc_caller()
{
  printf("this operation is now allowed\n");
  return false;
}

bool FPParameter::dealloc_callee()
{
  printf("this operation is now allowed\n");
  return false;
}

void FPParameter::set_bind_caller(bool b)
{
  printf("this operation is not allowed\n");
}

void FPParameter::set_bind_callee(bool b)
{
  printf("this operation is not allowed\n");
}

bool FPParameter::bind_caller()
{
  printf("this operation is not allowed\n");
  return false;
}

bool FPParameter::bind_callee()
{
  printf("this operation is not allowed\n");
  return false;
}

/* end */

