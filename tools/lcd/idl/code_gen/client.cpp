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

  // for each parameter that is ia projection
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++)
    {
      Parameter *p = *it;
      if(p->type()->num() == 4) {
	ProjectionType *pt = dynamic_cast<ProjectionType*>(p->type());
	Assert(pt != 0x0, "Error: dynamic cast to Projection type failed!\n");
	statements.push_back(declare_and_initialize_container_struct(p, pt, r->current_scope(), "caller"));
      }
    }

  /* TODO: projection channel allocation */
  /* TODO: what about function pointers */
  
  // marshal_parameters
  std::vector<Variable*> marshal_params = r->marshal_parameters;
  for(std::vector<Variable*>::iterator it = marshal_params.begin(); it != marshal_params.end(); it ++) {
    Variable *v = *it;
    statements.push_back(marshal_variable(v));    
  }

  /* TODO: make remote call using appropriate channel */
  
  /* unmarshal appropriate parameters and return value */
  
  /* Only thing that might need to be allocated is the return value */

  /* TODO: possibly allocate return value */
  const char* ret_var_name = "ret_val";

  std::vector<Variable*> unmarshal_params = r->unmarshal_parameters;
  for(std::vector<Variable*>::iterator it = unmarshal_params.begin(); it != unmarshal_params.end(); it ++) {
    Variable *v = *it;
    statements.push_back(unmarshal_variable(v));
  }

  /* unmarshal explicit return value */
  if(r->return_variable()->type()->num() != 5) { // check if not void.
    /* 
     * This adds some complexity because in this case we DO NOT have a list of things that need 
     * to be unmarshalled like for the params.  
     * Could just add this to the list of things to unmarshal and treat as special at that time.
     * Make sure to set up special accessor that is the one we will allocate here.
     */

  }

  /* TODO:  clear capability registers? */

  /* return value to caller */
  if(r->return_variable()->type()->num() != 5) {
    statements.push_back(new CCSTReturn(new CCSTPrimaryExprId(ret_var_name)));
  } else {
    statements.push_back(new CCSTReturn());
  }

  return new CCSTCompoundStatement(declarations, statements);
  
}





