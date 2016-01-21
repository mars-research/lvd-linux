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

CCSTCompoundStatement* caller_body(Rpc *r)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  // allocate necessary container shit
  
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

	// lookup container struct
	int err;
	const char* container_name_ = container_name(pt->name());
	Type *container_tmp = r->current_scope()->lookup(container_name_, &err);
	printf("looked up container %s for function %s. pointer for regular projection %p\n", container_name_, r->name(), r->current_scope()->lookup(pt->name(), &err)); 
	Assert(container_tmp != 0x0, "Error: could not find container in environment\n");
	ProjectionType *container = dynamic_cast<ProjectionType*>(container_tmp);
	Assert(container != 0x0, "Error: dynamic cast to Projection type failed!\n");
	
	// declare instance of container
	std::vector<CCSTInitDeclarator*> decs;
	decs.push_back(new CCSTDeclarator(new CCSTPointer(), new CCSTDirectDecId(container_name_)));
	CCSTDeclaration *container_declaration = new CCSTDeclaration(type2(container), decs);
	// finish declaring instance of container 
	
	declarations.push_back(container_declaration);
	
	// 	file_container = kzalloc(sizeof(*file_container), GFP_KERNEL);
	std::vector<CCSTAssignExpr*> kzalloc_args;
	kzalloc_args.push_back(new CCSTUnaryExprSizeOf(new CCSTUnaryExprCastExpr(new CCSTUnaryOp(unary_mult_t), new CCSTPrimaryExprId(container_name_))));
	kzalloc_args.push_back(new CCSTEnumConst("GFP_KERNEL"));
	

	statements.push_back(new CCSTAssignExpr(new CCSTPrimaryExprId(container_name_), equals(), function_call("kzalloc", kzalloc_args)));

	// if null
	// LIBLCD_ERR("kzalloc");
	//	lcd_exit(-1); /* abort */
	std::vector<CCSTDeclaration*> if_body_declarations;
	std::vector<CCSTStatement*> if_body_statements;
	
	std::vector<CCSTAssignExpr*> liblcd_err_args;
	liblcd_err_args.push_back(new CCSTString("kzalloc"));
	if_body_statements.push_back(function_call("LIBLCD_ERR", liblcd_err_args));
	std::vector<CCSTAssignExpr*> lcd_exit_args;
	lcd_exit_args.push_back(new CCSTInteger(-1));
	if_body_statements.push_back(function_call("lcd_exit", lcd_exit_args));
	CCSTCompoundStatement *if_body = new CCSTCompoundStatement(if_body_declarations, if_body_statements);
	statements.push_back(new CCSTIfStatement(new CCSTUnaryExprCastExpr(Not(), new CCSTPrimaryExprId(container_name_))
						 , if_body));


      
	// insert into dstore
      // do error checking
      }
    }


  /* end of this code, could go in own function maybe?*/

  /*
  std::vector<CCSTStatement*> statements_tmp = marshal_in_parameters(r->parameters());
  
  statements.insert(statements.end(), statements_tmp.begin(), statements_tmp.end());

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





