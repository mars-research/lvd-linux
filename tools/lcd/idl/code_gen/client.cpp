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
  std::vector<GlobalVariable*> globals = f->channels(); // if you odn't do this you get use after free heap issues

  for(std::vector<GlobalVariable*>::iterator it = globals.begin(); it != globals.end(); it ++) {
    GlobalVariable *gv = (GlobalVariable*) *it;
    definitions.push_back(declare_static_variable(gv));
  }

  // declare cspaces
  std::vector<GlobalVariable*> cspaces = f->cspaces_;
  for(std::vector<GlobalVariable*>::iterator it = cspaces.begin(); it != cspaces.end(); it ++) {
    GlobalVariable *gv = *it;
    definitions.push_back(declare_static_variable(gv));
  }

  // declare channel group
  definitions.push_back(declare_static_variable(f->channel_group));

  // create initialization function
  definitions.push_back(function_definition(interface_init_function_declaration(f)
					    , caller_interface_init_function_body(f)));
  
  // create exit function
  definitions.push_back(function_definition(interface_exit_function_declaration(f)
					    , caller_interface_exit_function_body(f)));

  // define container structs
  


  // functions
  std::vector<Rpc*> rpcs = f->rpc_definitions();
  for(std::vector<Rpc*>::iterator it = rpcs.begin(); it != rpcs.end(); it ++) {
    Rpc *r_tmp = (Rpc*) *it;
    if(r_tmp->function_pointer_defined()) {
      //    definitions.push_back(function_definition(callee_declaration(r_tmp)
      //					, callee_body(r_tmp)));
    } else {
      definitions.push_back(function_definition(function_declaration(r_tmp)
						,caller_body(r_tmp, f)));
    }
  }
  
  return new CCSTFile(definitions);
}

std::vector<CCSTDeclaration*> declare_containers(Variable *v)
{
  std::vector<CCSTDeclaration*> declarations;
  
  if(v->container() != 0x0) {
    declarations.push_back(declare_variable(v->container()));
  }

  if(v->type()->num() == 4) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
    Assert(pt != 0x0, "Error: dynamic cast to projection type failed.\n");

    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      if(pf->container() != 0x0) {
	std::vector<CCSTDeclaration*> tmp = declare_containers(pf);
	declarations.insert(declarations.end(), tmp.begin(), tmp.end());
      }
    }
  }
  
  return declarations;
}

/*
 * use module to get things like channels and cspaces.
 * or add channel and cspace as a field to an rpc....
 * that way each rpc can have its own channel or something....
 */
CCSTCompoundStatement* caller_body(Rpc *r, Module *m)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  // allocate necessary container things
  
  /* code that loops through parameters and allocates/initializes whatever necessary before marshalling*/
  
  // loop through params, declare a tmp and pull out marshal value
  std::vector<Parameter*> params = r->parameters();

  // for every parameter that has a container. declare containers. then alloc or container of
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++)
    {
      Parameter *p = *it;
      if(p->container() != 0x0) {
	// declare containers
	std::vector<CCSTDeclaration*> tmp = declare_containers(p);
	declarations.insert(declarations.end(), tmp.begin(), tmp.end());

	statements.push_back(alloc_link_container_caller(p));

      }
    }

  /* TODO: projection channel allocation */
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++) {
    Parameter *p = *it;
    
    if(p->type_->num() == 4 || p->type_->num() == 9) { // if a projection
      ProjectionType *pt = dynamic_cast<ProjectionType*>(p->type_);
      Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
      std::vector<CCSTStatement*> tmp_statements = caller_allocate_channels(pt);
      statements.insert(statements.end(), tmp_statements.begin(), tmp_statements.end());
    }
  }

  // projection channel initialization
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++) {
    Parameter *p = *it;
    
    if(p->type_->num() == 4 || p->type_->num() == 9) {
      ProjectionType *pt = dynamic_cast<ProjectionType*>(p->type_);
      Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
      std::vector<CCSTStatement*> tmp_statements = caller_initialize_channels(pt);
      statements.insert(statements.end(), tmp_statements.begin(), tmp_statements.end());
    }
  }

  
  // 

  /* TODO: what about function pointers */
  
  /* marshal parameters */

  std::vector<Parameter*> parameters = r->parameters();
  for(std::vector<Parameter*>::iterator it = parameters.begin(); it != parameters.end(); it ++) {
    Parameter *p = *it;
    statements.push_back(marshal_variable(p, "in"));    
  }

  /* marshal function tag */
  statements.push_back(marshal(new CCSTInteger(r->tag()), 0));

  /* make remote call using appropriate channel */

  std::vector<CCSTInitDeclarator*> ret_err;
  ret_err.push_back(new CCSTDeclarator(0x0, new CCSTDirectDecId("ret_err")));
  declarations.push_back(new CCSTDeclaration(int_type(), ret_err));

  std::vector<CCSTAssignExpr*> lcd_sync_call_args;
  lcd_sync_call_args.push_back(new CCSTPrimaryExprId(m->channels().at(0)->identifier())); // first channel
  statements.push_back( new CCSTAssignExpr( new CCSTPrimaryExprId("ret_err"), equals(), function_call("lcd_sync_call", lcd_sync_call_args)));

  statements.push_back(if_cond_fail(new CCSTPrimaryExprId("ret_err"), "lcd_sync_call"));
  
  /* unmarshal appropriate parameters and return value */

  for(std::vector<Parameter*>::iterator it = parameters.begin(); it != parameters.end(); it ++) {
    Parameter *p = *it;
    if(p->type()->num() != 5) {
      printf("calling unmarshal for %s\n", p->identifier());
      statements.push_back(unmarshal_variable(p, "out"));
    }
  }

  /* TODO:  clear capability registers? */

  /* return value to caller */
  if(r->return_variable()->type()->num() != 5) {
    // TODO: declare return variable
    statements.push_back(unmarshal_variable(r->return_variable(), ""));
    statements.push_back(new CCSTReturn(new CCSTPrimaryExprId(r->return_variable()->identifier())));
  } else {
    statements.push_back(new CCSTReturn());
  }
  
  return new CCSTCompoundStatement(declarations, statements);  
}







