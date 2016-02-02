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

CCSTStatement* helper(Variable *p)
{
  
  Type *t = p->type();
  switch (t->num()) {
    
  case 1: { // typedef
    break;
  }
  case 2: { // integer
    break;
  }
  case 4: { // projection
    std::vector<CCSTDeclaration*> declarations;
    std::vector<CCSTStatement*> statements;
    
    ProjectionType *pt = dynamic_cast<ProjectionType*>(t);
    Assert(pt != 0x0, "Error: dynamic cast failed\n");
    if(!p->alloc_caller()) {
      declarations.push_back(declare_and_initialize_container_struct(p));
    } else {
      //todo
      printf("todo\n");
      // what does this mean
    }
    // call for fields of struct
    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      statements.push_back(helper(pf));
    }
    return new CCSTCompoundStatement(declarations, statements);
  }
  case 5: { // void 
    break;
  }
  default: {
    printf("Must be a function pointer\n");
  }
  }
  
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
      statements.push_back(alloc_init_containers_driver(pt, f->current_scope(), "caller"));
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
	statements.push_back(alloc_init_containers_driver(pt, r->current_scope(), "caller"));
      }
    }

  // marshal parameters.

  // if function pointer pass reference to container
  // which will be a parameter.... so don't need to check... thank god
  /*
  std::vector<Variable*> marshal_params = r->marshal_parameters;
  for(std::vector<Variable*>::iterator it = marshal_params.begin(); it != marshal_params.end(); it ++) {
    Variable *mp = *it;
    if (mp->type()->num() == 4) { // is a projection and we need to pass a reference.
      ProjectionType *pt = dynamic_cast<ProjectionType*>(mp->type());
      Assert(pt != 0x0, "Error: dynamic cast to projection failed!\n");
      
      std::vector<ProjectionField*> pt_fields = pt->fields();
      for(std::vector<ProjectionField*>::iterator it2 = pt_fields.begin(); it2 != pt_fields.end(); it2 ++) {
	ProjectionField *pf = *it2;
	if (pf->in()) { // marshal this field
	  
	  std::vector<CCSTAssignExpr*> register_args;
	  register_args.push_back(access(pf)); // pass field marked in.
	  statements.push_back(function_call(access_register_mapping(pf->marshal_info()->get_register())
					     , register_args));
	}
      }
    } else {
      std::vector<CCSTAssignExpr*> register_args;
      register_args.push_back(access(mp));
      statements.push_back(function_call(access_register_mapping(mp->marshal_info()->get_register())
								 , register_args));
    }
  }
  */

  return new CCSTCompoundStatement(declarations, statements);
  
}

// declares and initializes an instance of the container struct for the struct
// that this variable is a type of. uses the container structs name as the name of the variable
CCSTDeclaration* declare_and_initialize_container_struct(Variable *v)
{
  if(v->type()->num() != 4) {
    Assert(1 == 0, "error cannot declare a construct struct for a non struct\n");
  } 
  ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
  Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
  
  std::vector<CCSTDecSpecifier*> specifier = struct_type(container_name(v->type()->name()));
  
  std::vector<CCSTInitDeclarator*> decs;
  
  decs.push_back(new CCSTInitDeclarator( new CCSTDeclarator( new CCSTPointer()
							     , new CCSTDirectDecId(container_name(v->type()->name())))
					 , new CCSTInitializer( new CCSTPostFixExprAssnExpr( function_name(CONTAINER_OF), container_of_args( access(v), struct_name(pt), pt->real_type())))));

  CCSTDeclaration *declare_init_container = new CCSTDeclaration(specifier, decs);
  return declare_init_container;
}


std::vector<CCSTAssignExpr*> container_of_args(CCSTPostFixExpr *struct_pointer, const char* type_name, const char* field_name)
{
  std::vector<CCSTAssignExpr*> args;
  args.push_back( struct_pointer);
  args.push_back( new CCSTPrimaryExprId(type_name));
  args.push_back( new CCSTPrimaryExprId(field_name));
  return args;
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





