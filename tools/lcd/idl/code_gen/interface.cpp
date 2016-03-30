#include "ccst.h"
#include "code_gen.h"

/*
 * code related to interface initialization and tear-down.
 */


/* TODO
 * 1. Always returns an int.
 * 2. name is glue_interfacename_init
 * 3. Params TODO. What params are always required, what is taken from IDL
 * for params, user specifies in IDL how many channels and their names
 * for each channel a cspace is declared and created with name chnlname_cspace.
 * Params are cptr_t for each channel, with specified name, 
 * There is an extra param not mentioned in idl "lcd_sync_channel_group". 
 */
CCSTDeclaration* interface_init_function_declaration(Module *m)
{
  /* returns an int */
  std::vector<CCSTDecSpecifier*> specifier;
  specifier.push_back(new CCSTSimpleTypeSpecifier(int_t));
  
  CCSTDirectDecId *name = new CCSTDirectDecId(glue_name(init_name(m->identifier())));
  /* init function parameters: the channels specified, and a */
  std::vector<GlobalVariable*> channels = m->channels();
  std::vector<CCSTParamDeclaration*> param_decs;
  for(std::vector<GlobalVariable*>::iterator it = channels.begin(); it != channels.end(); it ++) {
    GlobalVariable *p = (GlobalVariable*) *it;
    param_decs.push_back(new CCSTParamDeclaration(type2(p->type()), new CCSTDeclarator(pointer(p->pointer_count()), new CCSTDirectDecId(p->identifier()))));
  }
  // TODO: add the channels group thing
  GlobalVariable *extra_param = m->channel_group;
  param_decs.push_back(new CCSTParamDeclaration(type2(extra_param->type()), new CCSTDeclarator(pointer(extra_param->pointer_count()), new CCSTDirectDecId(extra_param->identifier()))));
  
  CCSTParamTypeList *param_list = new CCSTParamList(param_decs);
  /* end making params*/								   
  CCSTDirectDecParamTypeList *declaration = new CCSTDirectDecParamTypeList(name, param_list);

  std::vector<CCSTInitDeclarator*> func;
  func.push_back(new CCSTDeclarator(0x0, declaration));
  
  return new CCSTDeclaration(specifier, func);
}

/* TODO
 * takes the vector of global variables with also provides the parameters to the function.
 * 1. What about the generalization of this?
 */
CCSTCompoundStatement* interface_init_function_body(Module *m)
{
  // set each global variable to a parameter.
  std::vector<CCSTDeclaration*> body_declarations;
  std::vector<CCSTStatement*> body_statements;


  // declare ret
  std::vector<CCSTInitDeclarator*> decs;
  decs.push_back(new CCSTDeclarator(pointer(0)
				    , new CCSTDirectDecId("ret")));
  body_declarations.push_back(new CCSTDeclaration(int_type(), decs)); // TODO: add int ret;

  std::vector<GlobalVariable*> channels = m->channels();
  
  // set globals, which were already declared.
  for(std::vector<GlobalVariable*>::iterator it = channels.begin(); it != channels.end(); it ++) {
    GlobalVariable *g = *it;
    body_statements.push_back(new CCSTAssignExpr(new CCSTPrimaryExprId(g->identifier())
						 , equals()
						 , new CCSTPrimaryExprId(g->identifier())));
  }

  // set lcd_sync_channel_group 
  body_statements.push_back(new CCSTAssignExpr(new CCSTPrimaryExprId(m->channel_group->identifier())
					       , equals()
					       , new CCSTPrimaryExprId(m->channel_group->identifier())));

  // init cap code..
  std::vector<CCSTAssignExpr*> cap_init_args_empty;
  body_statements.push_back(new CCSTAssignExpr(new CCSTPrimaryExprId("ret")
					       , equals()
					       , function_call(cap_init_name(m->identifier())
							       , cap_init_args_empty)));
  // do error checking
  body_statements.push_back(if_cond_fail_goto(new CCSTUnaryExprCastExpr(Not(), new CCSTPrimaryExprId("ret"))
					      , "cap init", "fail1"));

  // initialize data stores.
  std::vector<GlobalVariable*> cspaces = m->cspaces_;
  for(std::vector<GlobalVariable*>::iterator it = cspaces.begin(); it != cspaces.end(); it ++) {
    GlobalVariable *gv = *it;
    
    std::vector<CCSTAssignExpr*> cap_create_args;
    cap_create_args.push_back(new CCSTUnaryExprCastExpr(reference()
							, new CCSTPrimaryExprId( gv->identifier())));
    body_statements.push_back(new CCSTAssignExpr(new CCSTPrimaryExprId("ret")
						 , equals()
						 , function_call(cap_create_name(m->identifier())
								 , cap_create_args)));
    
    // do error checking
    body_statements.push_back(if_cond_fail_goto(new CCSTUnaryExprCastExpr(Not(), new CCSTPrimaryExprId("ret"))
						, "cap create", "fail2"));
  }
  

  body_statements.push_back(new CCSTReturn(new CCSTInteger(0)));

  // failures
  std::vector<CCSTAssignExpr*> cap_exit_args_empty;
  body_statements.push_back(new CCSTPlainLabelStatement("fail2"
						       , function_call(cap_exit_name(m->identifier())
								       , cap_exit_args_empty)));

  body_statements.push_back(new CCSTPlainLabelStatement("fail1"
						       , new CCSTReturn(new CCSTPrimaryExprId("ret"))));
  return new CCSTCompoundStatement(body_declarations, body_statements);
}

/*
 * 
 */
CCSTDeclaration* interface_exit_function_declaration(Module *m)
{
  /* returns void */
  std::vector<CCSTDecSpecifier*> specifier;
  specifier.push_back(new CCSTSimpleTypeSpecifier(void_t));

  CCSTDirectDecId *name = new CCSTDirectDecId(exit_name(m->identifier()));
  CCSTParamList *param_list = new CCSTParamList(); // empty
  
  CCSTDirectDecParamTypeList *declaration = new CCSTDirectDecParamTypeList(name, param_list);

  std::vector<CCSTInitDeclarator*> func;
  func.push_back(new CCSTDeclarator(0x0, declaration));

  return new CCSTDeclaration(specifier, func);
}

/*
 * TODO
 * 1. Need to know what to tear-down.
 */
CCSTCompoundStatement* interface_exit_function_body()
{
  /* 
     void glue_vfs_exit(void)
{
	lcd_dstore_destroy(minix_dstore);

	/* We may as well remove the channel from the loop, though
	 * it doesn't matter in this simple example. (In general, we
	 * probably should.) 
	loop_rm_channel(loop_ctx, &vfs_channel);
}
   */

  return 0x0;
}
