#include "ccst.h"
#include "code_gen.h"

/*
 * code related to interface initialization and tear-down.
 */


/* TODO
 * 1. Always returns an int.
 * 2. name is interfacename_init
 * 3. Params TODO. What params are always required, what is taken from IDL
 */
CCSTDeclaration* interface_init_function_declaration(Module *m)
{
  /* returns an int */
  std::vector<CCSTDecSpecifier*> specifier;
  specifier.push_back(new CCSTSimpleTypeSpecifier(int_t));
  
  CCSTDirectDecId *name = new CCSTDirectDecId(init_name(m->identifier()));
  //CCSTParamTypeList *param_list = parameter_list(r->parameters()); TODO
  CCSTParamList *param_list = new CCSTParamList();
  CCSTDirectDecParamTypeList *declaration = new CCSTDirectDecParamTypeList(name, param_list);
  
  std::vector<CCSTInitDeclarator*> func;
  func.push_back(new CCSTDeclarator(0x0, declaration));
  
  return new CCSTDeclaration(specifier, func);
}

/* TODO
 * takes the vector of global variables with also provides the parameters to the function.
 * 1. What about the generalization of this?
 */
CCSTCompoundStatement* interface_init_function_body(std::vector<GlobalVariable*> globals)
{
  // set each global variable to a parameter.
  std::vector<CCSTDeclaration*> body_declarations_empty;
  std::vector<CCSTStatement*> body_statements;
  
  for(std::vector<GlobalVariable*>::iterator it = globals.begin(); it != globals.end(); it ++) {
    GlobalVariable *g = *it;
    body_statements.push_back(new CCSTAssignExpr(new CCSTPrimaryExprId(g->identifier())
						 , equals()
						 , new CCSTPrimaryExprId(parameter_name(g->identifier()))));
  }
  body_statements.push_back(new CCSTReturn(new CCSTInteger(0)));
  return new CCSTCompoundStatement(body_declarations_empty, body_statements);
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
