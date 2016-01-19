#include "lcd_ast.h"
#include <stdio.h>


Rpc::Rpc(ReturnVariable *return_value, const char* name, std::vector<Parameter* > parameters, LexicalScope *current_scope)
{
  this->explicit_return_ = return_value;
  this->name_ = name;
  this->parameters_ = parameters;
  this->symbol_table_ = new SymbolTable();
  this->current_scope_ = current_scope;
  
  for(std::vector<Parameter*>::iterator it = parameters.begin(); it != parameters.end(); it ++)
    {
      Parameter *p = (Parameter*) *it;
      this->symbol_table_->insert(p->identifier());
    }
}

void Rpc::set_function_pointer_defined(bool b)
{
  this->function_pointer_defined_ = b;
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
  MarshalPrepareVisitor *worker = new MarshalPrepareVisitor(new Registers());
  
  // marshal prepare for parameters as long as they are in or out
  for(std::vector<Parameter*>::iterator it = this->parameters_.begin(); it != this->parameters_.end(); it ++) {
    Parameter* p = *it;
    if(p->in() || p->out()) {
      p->prepare_marshal(worker);
    }
  }
  
  // marshal prepare for return value
  this->explicit_return_->prepare_marshal(worker);
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

void Rpc::create_trampoline_structs()
{
  for(std::vector<Parameter*>::iterator it = this->parameters_.begin(); it != this->parameters_.end(); it ++) {
    Parameter* p = *it;
    if (p->type()->num() == 7) {
      Function *f = dynamic_cast<Function*>(p->type());
      Assert(f != 0x0, "Error: dynamic cast to function type failed!\n");

      std::vector<ProjectionField*> trampoline_fields;
      int err;
      trampoline_fields.push_back(new ProjectionField(this->current_scope_->lookup("dstore", &err), "dstore", 1)); // dstore field
      trampoline_fields.push_back(new ProjectionField(this->current_scope_->lookup("lcd_trampoline_handle", &err), "t_handle", 1)); // lcd_trampoline handle field
      
      const char* trampoline_struct_name = hidden_args_name(f->name());
      this->current_scope_->insert(trampoline_struct_name, new ProjectionType(trampoline_struct_name, trampoline_struct_name, trampoline_fields));
    }
  }
}

Module::Module(const char *id, std::vector<Rpc*> rpc_definitions, std::vector<GlobalVariable*> globals, LexicalScope *ls)
{
  this->module_name_ = id;
  this->module_scope_ = ls;
  this->rpc_definitions_ = rpc_definitions;
  this->globals_ = globals;
}

std::vector<Rpc*> Module::rpc_definitions()
{
  return this->rpc_definitions_;
}

std::vector<GlobalVariable*> Module::globals()
{
  return this->globals_;
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

const char* Module::identifier()
{
  return this->module_name_;
}

Project::Project(LexicalScope *scope, std::vector<Module*> modules, std::vector<Include*> includes)
{
  this->project_scope_ = scope;
  this->project_modules_ = modules;
  this->project_includes_ = includes;
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

std::vector<Module*> Project::modules()
{
  return this->project_modules_;
}

Include::Include(bool relative, const char *path)
{
  this->relative_ = relative;
  this->path_ = path;
}

