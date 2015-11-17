#include "ccst.h"
#include "code_gen.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

CCSTFile* generate_client_header(Module* f)
{
  
}

CCSTFile* generate_client_source(Module* f)
{ 
  std::vector<CCSTExDeclaration*> definitions;

  // declare globals
  for(std::vector<GlobalVariable*>::iterator it = f->globals().begin(); it != f->globals().end(); it ++) {
    GlobalVariable *gv = (GlobalVariable*) *it;
    definitions.push_back(declare_static_variable(gv));
  }

  // create initialization function

  
  std::vector<Rpc*> rpcs = f->rpc_definitions();
  for(std::vector<Rpc*>::iterator it = rpcs.begin(); it != rpcs.end(); it ++) {
    Rpc *r = (Rpc*) *it;
    definitions.push_back(function_definition(function_declaration(r)
						     ,caller_body(r)));
  }
  
  return new CCSTFile(definitions);
}

CCSTCompoundStatement* caller_body(Rpc *r)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements = marshal_parameters(r->parameters());

  // implicit returns

  // replace with parameter loop
  //std::vector<CCSTStatement*> uirs = unmarshal_implicit_return(r->implicit_ret_marshal_info());
  //statements.insert(statements.end(), uirs.begin(), uirs.end());
  
  ReturnVariable *rv = r->return_variable();

  if(rv->type()->num() != 5) { // not void
    // Marshal_type *ret_info = r->explicit_ret_marshal_info();
    
    
  }
  else {
    statements.push_back(new CCSTReturn());
  }

  return new CCSTCompoundStatement(declarations, statements);
  
}

std::vector<CCSTStatement*> marshal_parameters(std::vector<Parameter*> params)
{
  std::vector<CCSTStatement*> statements;
  
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++) {
    Parameter *p = (Parameter*) *it;
    statements.push_back(marshal_variable(p));
  }
  
  return statements;
}

std::vector<CCSTStatement*> unmarshal_implicit_return(std::vector<Parameter*> implicit_returns)
{
  /*
  std::vector<CCSTStatement*> statements;
  for(std::vector<Parameter*>::iterator it = implicit_returns.begin(); it != implicit_returns.end(); it ++) {
    Parameter *p = *it;
    UnmarshalTypeVisitor *visitor = new UnmarshalTypeVisitor();
    statements.push_back(mt->accept(visitor));  
  }
  
  return statements;
  */
}

/*
CCSTStatement* unmarshal_explicit_return(Marshal_type* return_info)
{
  CCSTPointer *p = 0x0;
  if(r->explicit_return_type()->num() == 3) {
    p = new CCSTPointer();
  }
  std::vector<CCSTDecSpecifier*> ret_type = type(r->explicit_return_type());
  
  
  UnmarshalTypeVisitor *visitor = new UnmarshalTypeVisitor();

  std::vector<CCSTDeclaration*> d; std::vector<CCSTStatement*> s;
  s.push_back(ret_info->accept(visitor));
  s.push_back( new CCSTReturn(new CCSTPrimaryExprId("internal_ret")));
}
*/





