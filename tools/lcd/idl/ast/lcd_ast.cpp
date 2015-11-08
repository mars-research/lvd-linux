#include "lcd_ast.h"
#include <stdio.h>

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

Module::Module(std::vector<Rpc*> rpc_definitions, std::vector<GlobalVariable*> globals, LexicalScope *ls)
{
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
