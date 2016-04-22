#include "ccst.h"
#include "code_gen.h"

/*
 * code related to interface initialization and tear-down.
 */


/*
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

/*
 *
 */
CCSTCompoundStatement* callee_interface_init_function_body(Module *m)
{
  std::vector<CCSTDeclaration*> body_declarations;
  std::vector<CCSTStatement*> body_statements;

  // declare ret
  std::vector<CCSTInitDeclarator*> decs;
  decs.push_back(new CCSTDeclarator(pointer(0)
				    , new CCSTDirectDecId("ret")));
  body_declarations.push_back(new CCSTDeclaration(int_type(), decs)); // TODO: add int ret;

  // register ipc chnanels with dispatch loop
  
  /* int lcd_sync_channel_group_item_init(struct lcd_sync_channel_group_item *c, 
				cptr_t chnl, int expected_cptrs,
				int (*dispatch_fn)(
					struct lcd_sync_channel_group_item *)) */

  /* static struct lcd_sync_channel_group_item vfs_channel;
static int dispatch_vfs_channel(struct lcd_sync_channel_group_item *channel);

int glue_vfs_init(cptr_t vfs_chnl, struct lcd_sync_channel_group *_group)
{
	int ret;

	/* Set up ipc channel 
	ret = lcd_sync_channel_group_item_init(&vfs_channel, vfs_chnl, 1,
					dispatch_vfs_channel);*/

  std::vector<GlobalVariable*> channels = m->channels();
  
  for(std::vector<GlobalVariable*>::iterator it = channels.begin(); it != channels.end(); it ++) {
  
    GlobalVariable *chan = *it;
    std::vector<CCSTAssignExpr*> sync_channel_args;
    sync_channel_args.push_back(new CCSTPrimaryExprId(m->channel_group->identifier())); // &channel_group
    sync_channel_args.push_back(new CCSTPrimaryExprId(chan->identifier())); // channel 
    sync_channel_args.push_back(new CCSTInteger(1)); // 1 , per call? or?
    sync_channel_args.push_back(new CCSTPrimaryExprId("dispatch_todo")); // function poitner. dispatch.
    body_statements.push_back(new CCSTExprStatement( new CCSTAssignExpr(new CCSTPrimaryExprId("ret")
									, equals()
									, function_call("lcd_sync_channel_group_item_init"
											, sync_channel_args))));

    // do error checking
    body_statements.push_back(if_cond_fail_goto(new CCSTUnaryExprCastExpr(Not(), new CCSTPrimaryExprId("ret"))
						, "init channel item", "fail1"));
  }

  // TODO
  
}

/* 
 * takes the vector of global variables with also provides the parameters to the function.
 * 1. What about the generalization of this?
 */
CCSTCompoundStatement* caller_interface_init_function_body(Module *m)
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
    body_statements.push_back(new CCSTExprStatement( new CCSTAssignExpr(new CCSTPrimaryExprId(g->identifier())
									, equals()
									, new CCSTPrimaryExprId(g->identifier()))));
  }

  // set lcd_sync_channel_group 
  body_statements.push_back(new CCSTExprStatement( new CCSTAssignExpr(new CCSTPrimaryExprId(m->channel_group->identifier())
								      , equals()
								      , new CCSTPrimaryExprId(m->channel_group->identifier()))));

  // init cap code..
  std::vector<CCSTAssignExpr*> cap_init_args_empty;
  body_statements.push_back(new CCSTExprStatement( new CCSTAssignExpr(new CCSTPrimaryExprId("ret")
								      , equals()
								      , function_call(cap_init_name(m->identifier())
										      , cap_init_args_empty))));
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
    body_statements.push_back(new CCSTExprStatement( new CCSTAssignExpr(new CCSTPrimaryExprId("ret")
									, equals()
									, function_call(cap_create_name(m->identifier())
											, cap_create_args))));
    
    // do error checking
    body_statements.push_back(if_cond_fail_goto(new CCSTUnaryExprCastExpr(Not(), new CCSTPrimaryExprId("ret"))
						, "cap create", "fail2"));
  }
  

  body_statements.push_back(new CCSTReturn(new CCSTInteger(0)));

  // failures
  std::vector<CCSTAssignExpr*> cap_exit_args_empty;
  body_statements.push_back(new CCSTPlainLabelStatement("fail2"
							, new CCSTExprStatement( function_call(cap_exit_name(m->identifier())
											       , cap_exit_args_empty))));

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

  CCSTDirectDecId *name = new CCSTDirectDecId(glue_name(exit_name(m->identifier())));
  CCSTParamList *param_list = new CCSTParamList(); // empty
  
  CCSTDirectDecParamTypeList *declaration = new CCSTDirectDecParamTypeList(name, param_list);

  std::vector<CCSTInitDeclarator*> func;
  func.push_back(new CCSTDeclarator(0x0, declaration));

  return new CCSTDeclaration(specifier, func);
}

/*
 * 
 * 1. Need to know what to tear-down.
 */
CCSTCompoundStatement* caller_interface_exit_function_body(Module *m)
{
  /* 
     void glue_vfs_exit(void)
{
/* Free vfs data store. 
	vfs_cap_destroy(vfs_cspace);

	/* Tear down cap code 
	vfs_cap_exit();
}
   */
  std::vector<CCSTDeclaration*> body_declarations;
  std::vector<CCSTStatement*> body_statements;

  // tear down all of the cspaces
  std::vector<GlobalVariable*> cspaces = m->cspaces_;
  for(std::vector<GlobalVariable*>::iterator it = cspaces.begin(); it != cspaces.end(); it ++) {
    GlobalVariable *gv = *it;

    std::vector<CCSTAssignExpr*> cap_destroy_args;
    cap_destroy_args.push_back(new CCSTPrimaryExprId(gv->identifier()));
    body_statements.push_back(new CCSTExprStatement( function_call(cap_destroy_name(m->identifier())
								   , cap_destroy_args)));
  }
  // vfs cap exit
  std::vector<CCSTAssignExpr*> cap_exit_args;
  body_statements.push_back(new CCSTExprStatement( function_call(cap_exit_name(m->identifier())
								 , cap_exit_args)));

  return new CCSTCompoundStatement(body_declarations, body_statements);
}

CCSTCompoundStatement* callee_interface_exit_function_body(Module *m)
{
  std::vector<CCSTDeclaration*> body_declarations;
  std::vector<CCSTStatement*> body_statements;

  // tear down cspaces we declared
  std::vector<GlobalVariable*> cspaces = m->cspaces_;
  for(std::vector<GlobalVariable*>::iterator it = cspaces.begin(); it != cspaces.end(); it ++) {
    GlobalVariable *gv = *it;
    
    std::vector<CCSTAssignExpr*> cap_destroy_args;
    cap_destroy_args.push_back(new CCSTPrimaryExprId(gv->identifier()));
    body_statements.push_back(new CCSTExprStatement( function_call(cap_destroy_name(m->identifier())
								   , cap_destroy_args)));
  }
  
  // remove channels from group item thign
  // callee code has a disptach loop, caller does not.
  // remove the one group from the group_item thing
  std::vector<CCSTAssignExpr*> lcd_sync_channel_group_remove_args;
  lcd_sync_channel_group_remove_args.push_back(new CCSTPrimaryExprId(m->channel_group->identifier()));
  lcd_sync_channel_group_remove_args.push_back(new CCSTUnaryExprCastExpr(reference()
									 , new CCSTPrimaryExprId("group_item")));
  body_statements.push_back(new CCSTExprStatement( function_call("lcd_sync_channel_group_remove_args"
								 , lcd_sync_channel_group_remove_args)));


  // tear down cap code
  std::vector<CCSTAssignExpr*> cap_exit_args;
  body_statements.push_back(new CCSTExprStatement( function_call(cap_exit_name(m->identifier())
								 , cap_exit_args)));
  
  return new CCSTCompoundStatement(body_declarations, body_statements);
}
