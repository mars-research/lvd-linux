#include "ccst.h"
#include "code_gen.h"

CCSTDeclaration* dispatch_function_declaration()
{
  std::vector<CCSTDecSpecifier*>specifier;
  specifier.push_back(new CCSTSimpleTypeSpecifier(void_t));
  
  std::vector<CCSTInitDeclarator*> decs;
  
  std::vector<CCSTParamDeclaration*> empty;
  decs.push_back(new CCSTDeclarator(0x0
				    , new CCSTDirectDecParamTypeList( new CCSTDirectDecId("dispatch_loop")
								      , new CCSTParamList(empty))));
  return new CCSTDeclaration(specifier, decs);
}

CCSTCompoundStatement* dispatch_loop_body(std::vector<Rpc*> rps)
{  
  
  std::vector<CCSTDeclaration*> decs_in_body;
  // int ret; 
  std::vector<CCSTDecSpecifier*> s;// = new std::vector<CCSTDecSpecifier*>();
  std::vector<CCSTInitDeclarator*> d;// = new std::vector<CCSTInitDeclarator*>();
  s.push_back(new CCSTSimpleTypeSpecifier(int_t));
  d.push_back(new CCSTDeclarator(0x0, new CCSTDirectDecId("ret")));
  decs_in_body.push_back(new CCSTDeclaration(s,d));
  // Declare a variable of type int with name ret
  
  
  /* body statement, switch*/


  /*  lcd_recv(manufacturer_interface_cap) */
  /* check ret value, begin */
  std::vector<CCSTAssignExpr*> args;
  args.push_back( new CCSTPrimaryExprId("manufacturer_interface_cap"));
  CCSTAssignExpr* init_ret = new CCSTAssignExpr(new CCSTPrimaryExprId("ret")
						 , new CCSTAssignOp(equal_t)
						 , new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId("lcd_recv")
									       ,args));
  /* check ret value, end */

  /* if ret begin */
  CCSTIfStatement* if_stmt = new CCSTIfStatement(new CCSTPrimaryExprId("ret")
						 , new CCSTGoto("out"));
  /* if ret end */


  
  /* switch statement begin */
  std::vector<CCSTStatement*> cases;
  for(std::vector<Rpc*>::iterator it = rps.begin(); it != rps.end(); it ++)
    {
      Rpc* r_tmp = (Rpc*) *it;
      
      std::vector<CCSTDeclaration*> case_dec_empty;

      std::vector<CCSTStatement*> case_body_stmts;
      std::vector<CCSTAssignExpr*> msg_args;
      msg_args.push_back(new CCSTString("FILL IN MESSAGE"));
      case_body_stmts.push_back(new CCSTExprStatement( new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId("LCD_MSG")
										   , msg_args)));

      // make call to callee. 
      std::vector<CCSTAssignExpr*> args_empty;
      case_body_stmts.push_back(new CCSTExprStatement( new CCSTAssignExpr(new CCSTPrimaryExprId("ret")
						   , new CCSTAssignOp(equal_t)
						   , new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId(r_tmp->callee_name())
										 , args_empty))));

      case_body_stmts.push_back(new CCSTBreak());
      CCSTCompoundStatement* case_body = new CCSTCompoundStatement(case_dec_empty
								   , case_body_stmts);
      CCSTCaseStatement* tmp_case =  new CCSTCaseStatement(new CCSTPrimaryExprId(r_tmp->enum_name())
							   , case_body);
      cases.push_back(tmp_case);
    }
  /* adding a default case */

  std::vector<CCSTDeclaration*> default_dec_empty;
  std::vector<CCSTStatement*> default_stmts;
  std::vector<CCSTAssignExpr*> lcd_msg_args;
  lcd_msg_args.push_back(new CCSTString("Error unknown function"));
  default_stmts.push_back(new CCSTExprStatement( new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId("LCD_MSG")
									     , lcd_msg_args)));
  default_stmts.push_back(new CCSTGoto("out"));
  CCSTCompoundStatement* default_body =  new CCSTCompoundStatement(default_dec_empty
								   , default_stmts);
  CCSTDefaultLabelStatement* default_case = new CCSTDefaultLabelStatement( new CCSTExprStatement( new CCSTString("finish") ));
  cases.push_back(default_case);
  /* end of adding default case */

  std::vector<CCSTDeclaration*> switch_dec_empty;
  CCSTCompoundStatement* switch_body = new CCSTCompoundStatement(switch_dec_empty, cases);
  CCSTSwitchStatement* dispatch = new CCSTSwitchStatement(new CCSTPrimaryExprId("ret")
							  , switch_body);
  /* switch statement end */

  /* error checking if begin */
  CCSTIfStatement* error_if = new CCSTIfStatement(new CCSTPrimaryExprId("ret")
						  , new CCSTGoto("out"));
  /* error checking if end */

    /* for loop begin */
  std::vector<CCSTDeclaration*> for_declarations_empty;
  std::vector<CCSTStatement*> for_body_statements;
  for_body_statements.push_back(new CCSTExprStatement( init_ret));
  for_body_statements.push_back(if_stmt);
  for_body_statements.push_back(dispatch);
  for_body_statements.push_back(error_if);
  CCSTCompoundStatement *for_body = new CCSTCompoundStatement(for_declarations_empty, for_body_statements);
  CCSTForLoop* for_loop = new CCSTForLoop(0x0, 0x0, 0x0, for_body); 

  /* for loop end */

  /* labeled statement, out, begin */
  CCSTPlainLabelStatement* out_label = new CCSTPlainLabelStatement("out", new CCSTReturn());
  // doesn't return anything;
  /* labeled statement, out , end */
  
  /* put body together */
  std::vector<CCSTStatement*> body_statements;
  body_statements.push_back(for_loop);
  body_statements.push_back(out_label);
  return new CCSTCompoundStatement(decs_in_body, body_statements);
  /* body complete */
}

/* body for a callee function
 * does unmarshaling, real function call, etc
 */
CCSTCompoundStatement* callee_body(Rpc *r)
{
  // unmarshal parameters based on marshal data.
  // which says where params are stored.
  
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  
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

  return new CCSTCompoundStatement(declarations, statements);
}


CCSTFile* generate_server_header(Module *file)
{
  // #ifndef
  // #define
  // #endif
  // enum-specifier: enum id 
  std::vector<CCSTExDeclaration*> definitions; // = new std::vector<CCSTExDeclaration*>();
  // check if there are rpcs
  if(!file->rpc_definitions().empty())
    {
      printf("rpc not empty\n");
      definitions.push_back(construct_enum(file));
      // function callee function declarations
      std::vector<Rpc*> rpcs = file->rpc_definitions();
      for(std::vector<Rpc*>::iterator it = rpcs.begin(); it != rpcs.end(); it ++)
	{
	  definitions.push_back(callee_declaration((Rpc*) *it));
	}
    }
  definitions.push_back(dispatch_function_declaration());
  CCSTFile *c_file = new CCSTFile(definitions);
  return c_file;
}

CCSTDeclaration* callee_declaration(Rpc* r)
{
  std::vector<CCSTDecSpecifier*> specifier; // = new std::vector<CCSTDecSpecifier*>();
  specifier.push_back(new CCSTSimpleTypeSpecifier(int_t));
  char * callee_name = (char*) malloc((strlen(r->name())+strlen("_callee")+1)*sizeof(char));
  sprintf(callee_name, "%s%s", r->name(), "_callee");
  CCSTDirectDecId* id = new CCSTDirectDecId(callee_name);
  
  std::vector<CCSTDecSpecifier*> s;
  s.push_back(new  CCSTSimpleTypeSpecifier(void_t));
  CCSTParamDeclaration *parameter = new CCSTParamDeclaration(s);
  
  std::vector<CCSTParamDeclaration*> p_decs; // = new std::vector<CCSTParamDeclaration*>();
  p_decs.push_back(parameter);
  CCSTParamList *param_list = new CCSTParamList(p_decs);

  CCSTDirectDecParamTypeList *params = new CCSTDirectDecParamTypeList(id, param_list); 
    
  CCSTDeclarator* declarator = new CCSTDeclarator(NULL, params);
  std::vector<CCSTInitDeclarator*> init_declarator; // = new std::vector<CCSTInitDeclarator*>();
  init_declarator.push_back(declarator);
  CCSTDeclaration *func_declaration = new CCSTDeclaration(specifier, init_declarator);
  
  return func_declaration;
}



/* ServerCCSTSourceVisitor */


/*
 * generates the source file for the provided module/interface
 */
CCSTFile* generate_server_source(Module *m)
{
  std::vector<CCSTExDeclaration*> definitions;

  // includes. todo

  // need to print containers. but scopes container possibly duplicate projections definitions.

  // this feels mildy "hacky"
  // for each projection look up its container int the environment and print its declaration
  // if no container int the environment, don't print
  std::map<std::string, Type*> module_types = m->module_scope()->all_type_definitions();
  for(std::map<std::string, Type*>::iterator it = module_types.begin(); it != module_types.end(); it ++) {
    Type *t = (Type*) it->second;

    if(t->num() == 4) {
      ProjectionType *pt = dynamic_cast<ProjectionType*>(t);
      Assert(pt != 0x0, "Error: dynamic cast to projection type failed!\n");
      
      if(module_types.find(container_name(pt->name())) != module_types.end()) { // found the container
	
	ProjectionType *pt_container = dynamic_cast<ProjectionType*>(module_types[container_name(pt->name())]);
	Assert(pt_container != 0x0, "Error: dynamic cast to projection type failed\n");
	
	std::vector<CCSTDecSpecifier*> specifier;
	specifier.push_back(struct_declaration(pt_container));
	std::vector<CCSTInitDeclarator*> empty;
	definitions.push_back(new CCSTDeclaration(specifier, empty));
      }
    }
  }
  
  // print trampoline structs
  std::vector<Rpc*> rpcs = m->rpc_definitions();
  for(std::vector<Rpc*>::iterator it = rpcs.begin(); it != rpcs.end(); it ++) {
    Rpc *r = *it;
    if (r->function_pointer_defined()) {
      int err;
      Type *t = r->current_scope()->lookup(hidden_args_name(r->name()), &err);
      Assert(t != 0x0, "Error: failure looking up type\n");
      ProjectionType *pt = dynamic_cast<ProjectionType*>(t);
      Assert(t != 0x0, "Error: dynamic cast to projection type failed!\n");

      std::vector<CCSTDecSpecifier*> specifier;
      specifier.push_back(struct_declaration(pt));
      std::vector<CCSTInitDeclarator*> empty;
      definitions.push_back(new CCSTDeclaration(specifier, empty));
    }
  }
  

  // globals.
  std::vector<GlobalVariable*> globals = m->globals();
  for(std::vector<GlobalVariable*>::iterator it = globals.begin(); it != globals.end(); it ++) {
    GlobalVariable *gv = *it;
    definitions.push_back(declare_static_variable(gv));
  }
  // type definitions
  // container structs


  // also typedefs.
  /*
  std::map<std::string, Type*> module_type_definitions = m->module_scope()->type_definitions();
  
  for(std::map<std::string, Type*>::iterator it = module_type_definitions.begin(); it != module_type_definitions.end(); it ++) {
    Type* t = it->second;
    switch (t->num()) {
    case 1: // typedef
      {
	/*
	  definitions.push_back(typedef_declaration(t));
	
	break;
      }
    case 4: // projection
      {
	// note this is not "real type"
	// change from true to actual.
	ProjectionType *pt = dynamic_cast<ProjectionType*>(t);
	if(t == 0x0) {
	  printf("Error line 289: dynamic cast failed!\n");
	}
	definitions.push_back(container_struct_definition(container_name(t->name()), pt, true));
	break;
      }
    default:
      {
	break;
      }
    }
  }

  // init function
  
  
  // CCSTCompoundStatement* init_function_body = interface_init_function_body(globals_to_declare);
  // definitions.push_back(function_definitions(declaration, init_function_body));
  // functions

  // dispatch
  /*
  create_function_definition(create_function_declaration()
			     ,create_dispatch_loop_body(file->rpc_defs()));
  */

  std::vector<Rpc*> rps = m->rpc_definitions();
  for(std::vector<Rpc*>::iterator it = rps.begin(); it != rps.end(); it ++)
     {
       Rpc* r_tmp = (Rpc*) *it;
       if(r_tmp->function_pointer_defined()) {
	 definitions.push_back( function_definition(function_declaration(r_tmp)
						    ,caller_body(r_tmp)));
       } else {
	 definitions.push_back( function_definition(callee_declaration(r_tmp)
						    ,callee_body(r_tmp)));
       }
     }
   
  definitions.push_back( function_definition(dispatch_function_declaration()
						   , dispatch_loop_body(rps)));
   CCSTFile *c_file = new CCSTFile(definitions);
   printf("in server source gen\n");
   return c_file;
}


// one

/*
CCSTCompoundStatement* create_callee_body(Rpc *r)
{
  std::vector<Parameter*> parameters = r->parameters();
  for(std::vector<Parameter*>::iterator it = parameters.begin(); it != parameters.end(); it ++) {
    Parameter *p = (Parameter*) *it;
    if(p->alloc()) {
      AllocateVariableVisitor *worker = new AllocateVariableVisitor();
      p->type()->accept(worker); // allocates space if needed. 
    } else { // if not alloc must be bind?
      // grab from some function. 
    }

    UnmarshalVariableVisitor *worker = new UnmarshalVariableVisitor();
    p->accept(worker);
    
  }

  // make real call.
  
  // for each implicit return, marshal

  // marshal explicit return
  
  } */
