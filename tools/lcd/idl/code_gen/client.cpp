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
  std::vector<GlobalVariable*> globals = f->globals(); // if you odn't do this you get use after free heap issues

  for(std::vector<GlobalVariable*>::iterator it = globals.begin(); it != globals.end(); it ++) {
    GlobalVariable *gv = (GlobalVariable*) *it;
    definitions.push_back(declare_static_variable(gv));
  }

  // create initialization function
  

  // define container structs
  


  // functions
  std::vector<Rpc*> rpcs = f->rpc_definitions();
  for(std::vector<Rpc*>::iterator it = rpcs.begin(); it != rpcs.end(); it ++) {
    Rpc *r = (Rpc*) *it;
    definitions.push_back(function_definition(function_declaration(r)
						     ,caller_body(r)));
  }
  
  return new CCSTFile(definitions);
}

CCSTCompoundStatement* function_pointer_caller_body(Rpc *f)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;

  std::vector<Parameter*> params = f->parameters();

  // allocate container structs if necessary
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++) {
    Parameter *p = *it;
    
    if(p->type()->num() == 4 && p->alloc_caller()) {
      ProjectionType *pt = dynamic_cast<ProjectionType*>(p->type());
      Assert(pt != 0x0, "Error: dynamic cast to Projection type failed!\n");
      statements.push_back(alloc_init_containers_driver(p, pt, f->current_scope(), "caller"));
    }
  }

  // allocate trampolines

  // marshal parameters

  

}

CCSTCompoundStatement* caller_body(Rpc *r)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  // allocate necessary container things
  
  /* code that loops through parameters and allocates/initializes whatever necessary before marshalling*/
  
  // loop through params, declare a tmp and pull out marshal value
  std::vector<Parameter*> params = r->parameters();

  // for each parameter that is ia projection -- & is alloc
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++)
    {
      Parameter *p = *it;
      if(p->type()->num() == 4 && p->alloc_caller()) {
	ProjectionType *pt = dynamic_cast<ProjectionType*>(p->type());
	Assert(pt != 0x0, "Error: dynamic cast to Projection type failed!\n");
	statements.push_back(alloc_init_containers_driver(p, pt, r->current_scope(), "caller"));
      }
    }
  
  // marshal_parameters
  std::vector<Variable*> marshal_params = r->marshal_parameters;
  printf("going to loop through marshal params\n");
  for(std::vector<Variable*>::iterator it = marshal_params.begin(); it != marshal_params.end(); it ++) {
    Variable *v = *it;
    printf("calling marshal variable for var %s\n", v->identifier());
    statements.push_back(marshal_variable(v));    
  }

  // marshal_variable(variable);

  return new CCSTCompoundStatement(declarations, statements);
  
}



// marshals all of the parameters that are declared as "in"
// just calls marshal_variable
std::vector<CCSTStatement*> marshal_in_parameters(std::vector<Parameter*> params)
{
  std::vector<CCSTStatement*> statements;
  
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++) {
    Parameter *p = (Parameter*) *it;
    if(p->in()) {
      statements.push_back(marshal_variable(p));
    }
  }
  
  return statements;
}

const char* struct_name(ProjectionType *pt)
{
  const char *struct_name = pt->real_type();
  int len = strlen(struct_name);
  int len2 = strlen("struct ");
  char *new_str = (char*) malloc(sizeof(char)*(len+len2+1));
  
  std::ostringstream total;
  total << "struct " << struct_name;
  strncpy(new_str, total.str().c_str(), len+len2+1);
  return new_str;
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





