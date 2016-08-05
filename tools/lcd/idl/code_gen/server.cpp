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
CCSTCompoundStatement* callee_body(Rpc *r, Module *m)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  // allocate necessary container things
  
  /* code that loops through parameters and allocates/initializes whatever necessary before marshalling*/
  
  // loop through params, declare a tmp and pull out marshal value
  std::vector<Parameter*> params = r->parameters();
  
  // declare containers and variables
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++)
    {
      Parameter *p = *it;
      std::vector<CCSTDeclaration*> tmp_decs = declare_variables_callee(p);
      declarations.insert(declarations.end(), tmp_decs.begin(), tmp_decs.end());
    }

  if(r->function_pointer_defined()) {
    std::vector<Parameter*> hidden_args = r->hidden_args_;
    for(std::vector<Parameter*>::iterator it = hidden_args.begin(); it != hidden_args.end(); it ++) {
      Parameter *p = *it;
      // declare hiiden args.
    }
  }

  // TODO: unmarshal channel refs;

  // allocate/initiliaze and link these
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++)
    {
      Parameter *p = *it; // allocs and lookups but doesn't link yet.
      printf("Parameter is %s\n", p->identifier());
      statements.push_back(allocate_and_link_containers_callee(p
							       , m->cspaces_.at(0)->identifier()));
    }

  // allocate things which are not containers
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++)
    {
      Parameter *p = *it; // allocs and lookups but doesn't link yet.
      statements.push_back(allocate_non_container_variables(p));
    }

  /* As it stands now, only need to allocate hidden args structures if the 
   * parameter is alloc callee and contains function pointers
   */

  // declare hidden args structures;
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++) {
    Parameter *p = *it;
    if( p->type()->num() == 4 || p->type()->num() == 9) {
      ProjectionType *pt = dynamic_cast<ProjectionType*>(p->type());
      Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
      
      std::vector<CCSTDeclaration*> tmp_decs = declare_hidden_args_structures(pt, r->current_scope());
      declarations.insert(declarations.end(), tmp_decs.begin(), tmp_decs.end());
      
      // allocate hidden args structures
      Variable* cspace;
      if(p->type()->num() == 9) {
	ProjectionConstructorType *pct = dynamic_cast<ProjectionConstructorType*>(p->type());
	Assert(pct != 0x0, "Error: dynamic cast to projection constructor failed\n");

	cspace = pct->channel_params_.at(0).second;
      } else if (pt->channels_.size() > 0) {
	cspace = pt->channels_.at(0);
      } else {
	cspace = m->cspaces_.at(0);
      }
      
      if(p->alloc_callee()) {
	statements.push_back(alloc_init_hidden_args_struct(pt, p, r->current_scope(), m->cspaces_.at(0)));
      }
    }
  }

  // unmarshal rest of parameters. rest means not a container reference.
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++) {
    Parameter *p = *it;
    if(p->in()) {
      std::vector<CCSTStatement*> tmp_stmts = unmarshal_variable_callee(p);
      statements.insert(statements.end(), tmp_stmts.begin(), tmp_stmts.end());
    }
  }

  
  /* build up real call params */
  std::vector<CCSTAssignExpr*> real_call_params;
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++) {
    Parameter *p = *it;
    if(p->container() != 0x0) {
      ProjectionType *p_container_type = dynamic_cast<ProjectionType*>(p->container()->type());
      Assert(p_container_type != 0x0, "Error: dynamic cast to projection type failed\n");
      ProjectionField *p_container_real_field = p_container_type->get_field(p->type()->name());
      Assert(p_container_real_field != 0x0, "Error: could not find field in structure\n");
      real_call_params.push_back(access(p_container_real_field));
    } else {
      real_call_params.push_back(access(p));
    }
   
  }

  // declare return variable
  if(r->return_variable()->type()->num() != 5) {
    std::vector<CCSTDeclaration*> tmp_decs = declare_variables_callee(r->return_variable());
    declarations.insert(declarations.end(), tmp_decs.begin(), tmp_decs.end());
  }

  // allocate return variable // return value cannot have a container?
  statements.push_back(allocate_and_link_containers_callee(r->return_variable()
							   , m->cspaces_.at(0)->identifier()));
  statements.push_back(allocate_non_container_variables(r->return_variable()));
  
  // real call
  if(r->return_variable()->type()->num() == 5) {
    statements.push_back(new CCSTExprStatement(function_call(r->name()
							     , real_call_params)));
  } else {
    statements.push_back(new CCSTExprStatement(new CCSTAssignExpr(access(r->return_variable())
								  , equals()
								  , function_call(r->name()
										  , real_call_params))));
  }

  /* dealloc containers and contents of containers if necessary */
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++) {
    Parameter *v = *it;
    std::vector<CCSTStatement*> tmp_stmts = dealloc_containers_callee(v, m->cspaces_.at(0)->identifier(), r->current_scope());
    statements.insert(statements.end(), tmp_stmts.begin(), tmp_stmts.end());
  }

  /* marshal return params and val */
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++) {
    Parameter *v = *it;
    if(v->out()) {
      std::vector<CCSTStatement*> tmp_stmts = marshal_variable_callee(v);
      statements.insert(statements.end(), tmp_stmts.begin(), tmp_stmts.end());
    }
  }

  // marshal return val;
  if(r->return_variable()->type()->num() != 5) {
    std::vector<CCSTStatement*> tmp_stmts = marshal_variable_no_check(r->return_variable());
    statements.insert(statements.end(), tmp_stmts.begin(), tmp_stmts.end());
  }

  /* make IPC return call */
  // err = lcd_sync_reply();
  // if (err) { ...}
  std::vector<CCSTAssignExpr*> lcd_sync_reply_args_empty;
  statements.push_back(new CCSTExprStatement( new CCSTAssignExpr(new CCSTPrimaryExprId("err")
								 , equals()
								 , function_call("lcd_sync_reply"
										 , lcd_sync_reply_args_empty))));
  statements.push_back(if_cond_fail(new CCSTPrimaryExprId("err")
				    , "lcd_sync_reply"));

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

    if(t->num() == 4 || t->num() == 9) {
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
  std::vector<GlobalVariable*> globals = m->channels();
  for(std::vector<GlobalVariable*>::iterator it = globals.begin(); it != globals.end(); it ++) {
    GlobalVariable *gv = *it;
    definitions.push_back(declare_static_variable(gv));
  }

  std::vector<Rpc*> rps = m->rpc_definitions();
  for(std::vector<Rpc*>::iterator it = rps.begin(); it != rps.end(); it ++)
     {
       Rpc* r_tmp = (Rpc*) *it;
       if(r_tmp->function_pointer_defined()) {
	 printf("doing function pointer def\n");
	 definitions.push_back( function_definition(function_pointer_function_declaration(r_tmp)
						    ,caller_body(r_tmp, m)));
	 
	 definitions.push_back( function_definition(trampoline_function_declaration(r_tmp)
						    , trampoline_function_body(r_tmp)));
       } else {
	 printf("doing callee_declaration\n");
	 //	 definitions.push_back( function_definition(callee_declaration(r_tmp)
	 //					    ,callee_body(r_tmp, m)));
       }
     }
   
  //  definitions.push_back( function_definition(dispatch_function_declaration()
  //						   , dispatch_loop_body(rps)));
   CCSTFile *c_file = new CCSTFile(definitions);
   printf("in server source gen\n");
   return c_file;
}
