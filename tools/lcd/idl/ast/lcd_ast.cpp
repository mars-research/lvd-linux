#include "lcd_ast.h"
#include "utils.h"
#include <stdio.h>

Rpc::Rpc(ReturnVariable *return_value, const char* name, std::vector<Parameter* > parameters, LexicalScope *current_scope)
{
  this->explicit_return_ = return_value;
  this->name_ = name;
  this->parameters_ = parameters;
  this->symbol_table_ = new SymbolTable();
  this->current_scope_ = current_scope;
  this->tag_ = 0;
  
  for(std::vector<Parameter*>::iterator it = parameters.begin(); it != parameters.end(); it ++)
    {
      Parameter *p = (Parameter*) *it;
      this->symbol_table_->insert(p->identifier());
    }
}

unsigned int Rpc::tag()
{
  return this->tag_;
}

void Rpc::set_tag(unsigned int t)
{
  this->tag_ = t;
}

std::vector<Variable*> Rpc::marshal_projection_parameters(ProjectionType *pt, const char* direction)
{
  std::vector<Variable*> marshal_parameters;

  std::vector<ProjectionField*> fields = pt->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    ProjectionField *pf = *it;
    if( (strcmp(direction, "in") == 0 && pf->in())
	|| (strcmp(direction, "out") == 0 && pf->out())
	|| (strcmp(direction, "inout") == 0 && pf->in() && pf->out())
	|| strcmp(direction, "") == 0) {

      if (pf->type()->num() == 4 || pf->type()->num() == 9) {
	ProjectionType *pt_tmp = dynamic_cast<ProjectionType*>(pf->type());
	Assert(pt_tmp != 0x0, "Error: dynamic cast to Projection type failed.\n");
	std::vector<Variable*> tmp_params = marshal_projection_parameters(pt_tmp, direction);
	marshal_parameters.insert(marshal_parameters.end(), tmp_params.begin(), tmp_params.end());
      } else {
	marshal_parameters.push_back(pf);
      }

      if(pf->container() != 0x0) {
	Variable *con = pf->container();
	if( (strcmp(direction, "in") == 0 && con->in())
	    || (strcmp(direction, "out") == 0 && con->out())
	    || (strcmp(direction, "inout") == 0 && con->in() && pf->out())
	    || strcmp(direction, "") == 0) {
	  
	  if (con->type()->num() == 4 || con->type()->num() == 9) {
	    ProjectionType *pt_tmp = dynamic_cast<ProjectionType*>(con->type());
	    Assert(pt_tmp != 0x0, "Error: dynamic cast to Projection type failed.\n");
	    std::vector<Variable*> tmp_params = marshal_projection_parameters(pt_tmp, direction);
	    marshal_parameters.insert(marshal_parameters.end(), tmp_params.begin(), tmp_params.end());
	  } else {
	    marshal_parameters.push_back(con);
	  }
	}
      }
    }
  }
  return marshal_parameters;
}

void Rpc::create_container_variables()
{
  // for each parameter that is a pointer, need to create a container variable
  printf("in create container variables for %s\n", this->name_);
  for(std::vector<Parameter*>::iterator it = this->parameters_.begin(); it != this->parameters_.end(); it ++) {
    Parameter *p = *it;
    p->create_container_variable(this->current_scope());
  }
}

void Rpc::set_function_pointer_defined(bool b)
{
  this->function_pointer_defined_ = b;
}

void Rpc::set_hidden_args(std::vector<Parameter*> hidden_args)
{
  this->hidden_args_ = hidden_args;
}

bool Rpc::function_pointer_defined()
{
  return this->function_pointer_defined_;
}

ReturnVariable* Rpc::return_variable()
{
  return this->explicit_return_;
}

const char* Rpc::name()
{
  return name_;
}

/*
Marshal_type* Rpc::accept(MarshalVisitor* worker, Registers *data)
{
  return worker->visit(this, data);
}
*/

LexicalScope* Rpc::current_scope()
{
  return this->current_scope_;
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
  return parameters_;
}

void Rpc::prepare_marshal()
{
  // TODO: accoutn for hidden args
  std::vector<Variable*> in_params;
  std::vector<Variable*> out_params;
  std::vector<Variable*> in_out_params;

  std::vector<Parameter*> all_params = this->parameters_;
  if(this->function_pointer_defined()) {
    all_params.insert(all_params.end(), this->hidden_args_.begin(), this->hidden_args_.end());
  }

  // sort our parameters
  for(std::vector<Parameter*>::iterator it = all_params.begin(); it != all_params.end(); it ++) {
    Parameter *p = *it;
    printf("parameter we are going to marshal is %s\n", p->identifier());
    if(p->in() && !p->out()) {
      in_params.push_back(p);
      if(p->type()->num() == 4 || p->type()->num() == 9) {
	ProjectionType *pt = dynamic_cast<ProjectionType*>(p->type());
	Assert(pt != 0x0, "Error: dynamic cast to projection failed\n");
	std::vector<Variable*> tmp = this->marshal_projection_parameters(pt, "in");
	in_params.insert(in_params.end(), tmp.begin(), tmp.end());
      }
    } else if (!p->in() && p->out()) {
      out_params.push_back(p);
      if(p->type()->num() == 4 || p->type()->num() == 9) {
	ProjectionType *pt = dynamic_cast<ProjectionType*>(p->type());
	Assert(pt != 0x0, "Error: dynamic cast to projection failed\n");
	std::vector<Variable*> tmp = this->marshal_projection_parameters(pt, "out");
	out_params.insert(out_params.end(), tmp.begin(), tmp.end());
      }
    } else if (p->in() && p->out()) {
      in_out_params.push_back(p);
      if(p->type()->num() == 4 || p->type()->num() == 9) {
	ProjectionType *pt = dynamic_cast<ProjectionType*>(p->type());
	Assert(pt != 0x0, "Error: dynamic cast to projection failed\n");
	// in 
	std::vector<Variable*> tmp = this->marshal_projection_parameters(pt, "in");
	in_params.insert(in_params.end(), tmp.begin(), tmp.end());

	// out
	std::vector<Variable*> tmp2 = this->marshal_projection_parameters(pt, "out");
	out_params.insert(out_params.end(), tmp2.begin(), tmp2.end());

	// in out
	std::vector<Variable*> tmp3 = this->marshal_projection_parameters(pt, "inout");
	in_out_params.insert(in_out_params.end(), tmp3.begin(), tmp3.end());
      }
      
    }

    // have to do it for container too!!!
    if(p->container() != 0x0) {
      Variable *container = p->container();
      printf("parameter container we are going to marshal is %s\n", p->container()->identifier());
      
      if(container->in() && !container->out()) {
	in_params.push_back(container);
	if(container->type()->num() == 4 || container->type()->num() == 9) {
	  ProjectionType *pt = dynamic_cast<ProjectionType*>(container->type());
	  Assert(pt != 0x0, "Error: dynamic cast to projection failed\n");
	  std::vector<Variable*> tmp = this->marshal_projection_parameters(pt, "in");
	  in_params.insert(in_params.end(), tmp.begin(), tmp.end());
	}
      } else if (!container->in() && container->out()) {
	out_params.push_back(container);
	if(container->type()->num() == 4 || container->type()->num() == 9) {
	  ProjectionType *pt = dynamic_cast<ProjectionType*>(container->type());
	  Assert(pt != 0x0, "Error: dynamic cast to projection failed\n");
	  std::vector<Variable*> tmp = this->marshal_projection_parameters(pt, "out");
	  out_params.insert(out_params.end(), tmp.begin(), tmp.end());
	}
      } else if (container->in() && container->out()) {
	in_out_params.push_back(container);
	if(container->type()->num() == 4 || container->type()->num() == 9) {
	  ProjectionType *pt = dynamic_cast<ProjectionType*>(container->type());
	  Assert(pt != 0x0, "Error: dynamic cast to projection failed\n");
	  // in 
	  std::vector<Variable*> tmp = this->marshal_projection_parameters(pt, "in");
	  in_params.insert(in_params.end(), tmp.begin(), tmp.end());

	  // out
	  std::vector<Variable*> tmp2 = this->marshal_projection_parameters(pt, "out");
	  out_params.insert(out_params.end(), tmp2.begin(), tmp2.end());

	  // in out
	  std::vector<Variable*> tmp3 = this->marshal_projection_parameters(pt, "inout");
	  in_out_params.insert(in_out_params.end(), tmp3.begin(), tmp3.end());
	}
      
      }
    }
  }
  

  // assign register(s) to return value
  out_params.push_back(this->explicit_return_);
  if(this->explicit_return_->type()->num() == 4 || this->explicit_return_->type()->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(this->explicit_return_->type());
    Assert(pt != 0x0, "Error: dynamic cast to projection failed\n");
    // in 
    std::vector<Variable*> tmp = this->marshal_projection_parameters(pt, ""); // everything
    out_params.insert(out_params.end(), tmp.begin(), tmp.end());
  }

  
  /* TODO: make sure register 0 is free for function tag */

  // marshal prepare the in parameters
  Registers *in_reg = new Registers();
  int arr[1];
  arr[0] = 1;
  in_reg->init(arr, 1, 0x0, 0);

  MarshalPrepareVisitor *in_marshal_worker = new MarshalPrepareVisitor(in_reg);
  
  for(std::vector<Variable*>::iterator it = in_params.begin(); it != in_params.end(); it ++) {
    Variable* v = *it;
    v->prepare_marshal(in_marshal_worker);
  }


  // marshal prepare the out parameters
  Registers *out_reg = new Registers();
  out_reg->init(arr, 1, 0x0, 0);

  MarshalPrepareVisitor *out_marshal_worker = new MarshalPrepareVisitor(out_reg);

  for(std::vector<Variable*>::iterator it = out_params.begin(); it != out_params.end(); it ++) {
    Variable *v = *it;
    v->prepare_marshal(out_marshal_worker);
  }

  // marshal prepare for the in/out params.  meaning they need only 1 register for both ways
  // need to get the set union of in_marshal_worker's registers and out_marshal_worker's registers
  Registers *in_out_regs = new Registers();
  in_out_regs->init(in_reg, out_reg);

  MarshalPrepareVisitor *in_out_marshal_worker = new MarshalPrepareVisitor(in_out_regs);

  for(std::vector<Variable*>::iterator it = in_out_params.begin(); it != in_out_params.end(); it ++) {
    Variable *v = *it;
    v->prepare_marshal(in_out_marshal_worker);
  }

  // done
}

void Rpc::resolve_types()
{  
  // marshal prepare for parameters as long as they are in or out
  for(std::vector<Parameter*>::iterator it = this->parameters_.begin(); it != this->parameters_.end(); it ++) {
    Parameter* p = *it;
    p->resolve_types(this->current_scope_);
  }
  
  // marshal prepare for return value
  this->explicit_return_->resolve_types(this->current_scope_);
}

void Rpc::copy_types()
{
  // copy parameters.
  for(std::vector<Parameter*>::iterator it = this->parameters_.begin(); it != this->parameters_.end(); it ++) {
    Parameter *p = *it;
    p->type_ = p->type_->clone();
    if (p->container_ != 0x0) {
      p->container_ = p->container_->clone();
    }
  }
  
  // copy return type
  this->explicit_return_->type_ = this->explicit_return_->type_->clone();
}

void Rpc::set_accessors()
{
  // return variable
  this->explicit_return_->set_accessor(0x0);

  // parameters
  for(std::vector<Parameter*>::iterator it = this->parameters_.begin(); it != this->parameters_.end(); it ++) {
    Parameter *p = *it;
    p->set_accessor(0x0);
  }
}

void Rpc::set_copy_container_accessors()
{
  // return variable
  this->explicit_return_->set_accessor(0x0);

  // parameters
  for(std::vector<Parameter*>::iterator it = this->parameters_.begin(); it != this->parameters_.end(); it ++) {
    Parameter *p = *it;
    p->set_accessor(0x0);
  }
}

void Rpc::initialize_types()
{
  // parameters
  for(std::vector<Parameter*>::iterator it = this->parameters_.begin(); it != this->parameters_.end(); it ++) {
    Parameter *p = *it;
    p->initialize_type();
  }

  // return variable
  this->explicit_return_->initialize_type();
}

void Rpc::create_trampoline_structs()
{
  for(std::vector<Parameter*>::iterator it = this->parameters_.begin(); it != this->parameters_.end(); it ++) {
    Parameter* p = *it;
    if (p->type()->num() == 7) {
      Function *f = dynamic_cast<Function*>(p->type());
      Assert(f != 0x0, "Error: dynamic cast to function type failed!\n");

      std::vector<ProjectionField*> trampoline_fields;
      int err;
      trampoline_fields.push_back(new ProjectionField(this->current_scope_->lookup("cspace", &err), "cspace", 1)); // dstore field
      trampoline_fields.push_back(new ProjectionField(this->current_scope_->lookup("lcd_trampoline_handle", &err), "t_handle", 1)); // lcd_trampoline handle field
      
      const char* trampoline_struct_name = hidden_args_name(f->name());
      this->current_scope_->insert(trampoline_struct_name, new ProjectionType(trampoline_struct_name, trampoline_struct_name, trampoline_fields));
    }
  }
}

Module::Module(const char *id, std::vector<Rpc*> rpc_definitions, std::vector<GlobalVariable*> channels, LexicalScope *ls)
{
  this->module_name_ = id;
  this->module_scope_ = ls;
  this->rpc_definitions_ = rpc_definitions;
  this->channels_ = channels;

  int err;
  Type *cspace = this->module_scope_->lookup("glue_cspace", &err);
  if(!cspace) {
    cspace = new UnresolvedType("glue_cspace");
  }
  // create cspaces.
  for(std::vector<GlobalVariable*>::iterator it = this->channels_.begin(); it != this->channels_.end(); it ++) {
    GlobalVariable *gv = *it;
    this->cspaces_.push_back(new GlobalVariable(cspace, cspace_name(gv->identifier()), 1));
  }
  
  // create channel group
  Type *group = this->module_scope_->lookup("lcd_sync_channel_group", &err);
  if(!group) {
    group = new UnresolvedType("lcd_sync_channel_group");
  }

  this->channel_group = new GlobalVariable(group, group_name(this->identifier()), 1);
}

std::vector<Rpc*> Module::rpc_definitions()
{
  return this->rpc_definitions_;
}

std::vector<GlobalVariable*> Module::channels()
{
  return this->channels_;
}

LexicalScope* Module::module_scope()
{
  return this->module_scope_;
}

void Module::prepare_marshal()
{
  for(std::vector<Rpc*>::iterator it = this->rpc_definitions_.begin(); it != this->rpc_definitions_.end(); it ++) {
    Rpc *r = *it;
    r->prepare_marshal();
  }
}

void Module::resolve_types()
{
  // need to resolve types in projections.
  this->module_scope_->resolve_types();
  
  for(std::vector<Rpc*>::iterator it = this->rpc_definitions_.begin(); it != this->rpc_definitions_.end(); it ++) {
    Rpc *r = *it;
    r->resolve_types();
  }
}

void Module::copy_types()
{
  for(std::vector<Rpc*>::iterator it = this->rpc_definitions_.begin(); it != this->rpc_definitions_.end(); it ++) {
    Rpc *r = *it;
    r->copy_types();
  }
}

void Module::set_accessors()
{
  for(std::vector<Rpc*>::iterator it = this->rpc_definitions_.begin(); it != this->rpc_definitions_.end(); it ++) {
    Rpc *r = *it;
    r->set_accessors();
  }
}

void Module::initialize_types()
{
  for(std::vector<Rpc*>::iterator it = this->rpc_definitions_.begin(); it != this->rpc_definitions_.end(); it ++) {
    Rpc *r = *it;
    r->initialize_types();
  }
}

void Module::function_pointer_to_rpc()
{
  std::vector<Rpc*> rpcs = this->module_scope()->function_pointer_to_rpc();
  this->rpc_definitions_.insert(this->rpc_definitions_.end(), rpcs.begin(), rpcs.end());
}

void Module::create_trampoline_structs()
{
  this->module_scope_->create_trampoline_structs();
  // loop through rpc definitions
  // todo
  
  for(std::vector<Rpc*>::iterator it = this->rpc_definitions_.begin(); it != this->rpc_definitions_.end(); it ++) {
    Rpc *r = *it;
    r->create_trampoline_structs();
  }
}

void Module::generate_function_tags(Project *p)
{
  for(std::vector<Rpc*>::iterator it = this->rpc_definitions_.begin(); it != this->rpc_definitions_.end(); it ++) {
    Rpc *r = *it;
    
    r->set_tag(p->get_next_tag());
  }
}

void Module::create_container_variables()
{
  for(std::vector<Rpc*>::iterator it = this->rpc_definitions_.begin(); it != this->rpc_definitions_.end(); it ++) {
    Rpc *r = *it;
    r->create_container_variables();
  }
}

void Module::set_copy_container_accessors()
{
  for(std::vector<Rpc*>::iterator it = this->rpc_definitions_.begin(); it != this->rpc_definitions_.end(); it ++) {
    Rpc *r = *it;
    r->set_copy_container_accessors();
  }
}

const char* Module::identifier()
{
  return this->module_name_;
}

Project::Project(LexicalScope *scope, std::vector<Module*> modules, std::vector<Include*> includes)
{
  this->project_scope_ = scope;
  this->project_modules_ = modules;
  this->project_includes_ = includes;
  this->last_tag_ = 0;
}

void Project::prepare_marshal()
{
  for(std::vector<Module*>::iterator it = this->project_modules_.begin(); it != this->project_modules_.end(); it ++) {
    Module *m = *it;
    m->prepare_marshal();
  }
}

void Project::resolve_types()
{
  this->project_scope_->resolve_types();
  
  for(std::vector<Module*>::iterator it = this->project_modules_.begin(); it != this->project_modules_.end(); it ++) {
    Module *m = *it;
    m->resolve_types();
  }
}

void Project::copy_types()
{
  for(std::vector<Module*>::iterator it = this->project_modules_.begin(); it != this->project_modules_.end(); it ++) {
    Module *m = *it;
    m->copy_types();
  }
}

void Project::set_accessors()
{
  for(std::vector<Module*>::iterator it = this->project_modules_.begin(); it != this->project_modules_.end(); it ++) {
    Module *m = *it;
    m->set_accessors();
  }
}

void Project::function_pointer_to_rpc()
{
  // right now project doesnt have free rpcs.

  for(std::vector<Module*>::iterator it = this->project_modules_.begin(); it != this->project_modules_.end(); it ++) {
    Module *m = *it;
    m->function_pointer_to_rpc();
  }
}

void Project::create_trampoline_structs()
{
  // todo

  for(std::vector<Module*>::iterator it = this->project_modules_.begin(); it != this->project_modules_.end(); it ++) {
    Module *m = *it;
    m->create_trampoline_structs();
  }
}

void Project::generate_function_tags()
{
  for(std::vector<Module*>::iterator it = this->project_modules_.begin(); it != this->project_modules_.end(); it ++) {
    Module *m = *it;
    m->generate_function_tags(this);
  }
}

void Project::create_container_variables()
{
  for(std::vector<Module*>::iterator it = this->project_modules_.begin(); it != this->project_modules_.end(); it ++) {
    Module *m = *it;
    m->create_container_variables();
  }
}

std::vector<Module*> Project::modules()
{
  return this->project_modules_;
}

unsigned int Project::get_next_tag()
{
  this->last_tag_ += 1;
  return this->last_tag_;
}

void Project::initialize_types()
{
  for(std::vector<Module*>::iterator it = this->project_modules_.begin(); it != this->project_modules_.end(); it ++) {
    Module *m = *it;
    m->initialize_types();
  }
}

void Project::set_copy_container_accessors()
{
  for(std::vector<Module*>::iterator it = this->project_modules_.begin(); it != this->project_modules_.end(); it ++) {
    Module *m = *it;
    m->set_copy_container_accessors();
  }
}

Include::Include(bool relative, const char *path)
{
  this->relative_ = relative;
  this->path_ = path;
}


