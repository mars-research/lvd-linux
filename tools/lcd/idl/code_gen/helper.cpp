#include "ccst.h"
#include "code_gen.h"



CCSTExDeclaration* construct_enum(Module *f)
{
  const char* enum_name = "todo";
  std::vector<Rpc*> rpcs = f->rpc_definitions();
  CCSTEnumeratorList *el = construct_enumlist(rpcs);
  CCSTEnumSpecifier *e = new CCSTEnumSpecifier(enum_name, el);
  std::vector<CCSTDecSpecifier*> tmp; // = new std::vector<CCSTDecSpecifier*>();
  tmp.push_back(e);
  std::vector<CCSTInitDeclarator*> empty;
  CCSTDeclaration *declaration = new CCSTDeclaration(tmp, empty);
  
  return declaration;
}

const char* enum_name(const char *name)
{
  return "todo";
}


CCSTEnumeratorList* construct_enumlist(std::vector<Rpc *> rps)
{
  // list of functions to put in enum.
  std::vector<CCSTEnumerator*>* list = new std::vector<CCSTEnumerator*>();
  for(std::vector<Rpc*>::iterator it = rps.begin(); it != rps.end(); it ++)
    {
      Rpc *r = *it;
      const char* upper_name = string_to_upper(r->name());
      char* enum_name = (char*)malloc((sizeof(upper_name)+9+1)*sizeof(char));
      sprintf(enum_name, "%s_CALLEE_T", upper_name);
      list->push_back(new CCSTEnumerator(enum_name));
    }
  CCSTEnumeratorList *enum_list = new CCSTEnumeratorList(list);
  return enum_list;
}

/* Creates a function definition
 * from a function declaration
 * and a body
 */
CCSTFuncDef* function_definition(CCSTDeclaration* function_declaration, CCSTCompoundStatement *body)
{
  Assert(function_declaration->decs_.size() == 1, "Error: More than one initializer/declarator in function declaration");
  
  CCSTDeclarator *func = dynamic_cast<CCSTDeclarator*>(function_declaration->decs_.at(0));
  Assert(func != 0, "Error: dynamic cast from CCSTInitDeclarator to CCSTDeclarator has failed!");
  
  std::vector<CCSTDeclaration*> decs; // not sure what this is for. unnecessary?
  
  return new CCSTFuncDef(function_declaration->specifier_, func, decs, body);
}

CCSTParamTypeList* parameter_list(std::vector<Parameter*> params)
{
  std::vector<CCSTParamDeclaration*> param_decs;
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++) {
    Parameter *p = (Parameter*) *it;
    param_decs.push_back(new CCSTParamDeclaration(type2(p->type()), new CCSTDeclarator(pointer(p->pointer_count()), new CCSTDirectDecId(p->identifier()))));
  }

  return new CCSTParamList(param_decs);
}

/* creates a function declaration from an rpc
 */
CCSTDeclaration* function_declaration(Rpc* r)
{
  std::vector<CCSTDecSpecifier*> specifier = type2(r->return_variable()->type());
  
  std::vector<CCSTInitDeclarator*> func; // = new std::vector<CCSTInitDeclarator*>(); // pointer name, params
  CCSTPointer *p = pointer(r->return_variable()->pointer_count());
  
  CCSTDirectDecId *name = new CCSTDirectDecId(r->name());
  CCSTParamTypeList *param_list = parameter_list(r->parameters());

  CCSTDirectDecParamTypeList *name_params = new CCSTDirectDecParamTypeList(name, param_list);
  
  func.push_back(new CCSTDeclarator(p, name_params));
  
  return new CCSTDeclaration(specifier, func);
}

CCSTDeclaration* function_pointer_function_declaration(Rpc *r) 
{
  std::vector<CCSTDecSpecifier*> specifier = type2(r->return_variable()->type());
  
  std::vector<CCSTInitDeclarator*> func; // = new std::vector<CCSTInitDeclarator*>(); // pointer name, params
  CCSTPointer *p = pointer(r->return_variable()->pointer_count());
  
  CCSTDirectDecId *name = new CCSTDirectDecId(r->name());

  std::vector<Parameter*> all_params;

  std::vector<Parameter*> r_parameters = r->parameters();
  std::vector<Parameter*> r_hidden_args = r->hidden_args_;

  all_params.insert(all_params.end(), r_parameters.begin(), r_parameters.end());
  all_params.insert(all_params.end(), r_hidden_args.begin(), r_hidden_args.end());
  
  CCSTParamTypeList *param_list = parameter_list(all_params);

  CCSTDirectDecParamTypeList *name_params = new CCSTDirectDecParamTypeList(name, param_list);
  
  func.push_back(new CCSTDeclarator(p, name_params));
  
  return new CCSTDeclaration(specifier, func);
  
}

// 
CCSTPostFixExpr* access(Variable *v)
{
  if(v->accessor() == 0x0) {
    int pc = v->pointer_count();
    if (pc > 1) {
      int tmp = pc-1;
      CCSTCastExpr* init = new CCSTPrimaryExprId(v->identifier());
      while(tmp > 0) {
	init = dereference(init);
	tmp -= 1;
      }
      return new CCSTPrimaryExpr(init);
    } else {
      return new CCSTPrimaryExprId(v->identifier());
    }
  } else {
    if (v->accessor()->pointer_count() == 0) {
      int pc = v->pointer_count();
      if (pc > 1) {
	int tmp = pc-1;
        CCSTCastExpr * init = new CCSTPostFixExprAccess(access(v->accessor()), object_access_t, v->identifier());
	while(tmp > 0) {
	  init = dereference(init);
	  tmp -= 1;
	}
	return new CCSTPrimaryExpr(init);
	
      } else {
	return new CCSTPostFixExprAccess(access(v->accessor()), object_access_t, v->identifier());
      }
    } else {
      int pc = v->pointer_count();
      if (pc > 1) {
	int tmp = pc-1;
	CCSTCastExpr *init = new CCSTPostFixExprAccess(access(v->accessor()), pointer_access_t, v->identifier());
	while(tmp > 0) {
	  init = dereference(init);
	  tmp -= 1;
	}
	return new CCSTPrimaryExpr(init);
      } else {
	return new CCSTPostFixExprAccess(access(v->accessor()), pointer_access_t, v->identifier());
      }
    }
  }
}

CCSTIfStatement* if_cond_fail(CCSTExpression *cond, const char *err_msg)
{
  std::vector<CCSTDeclaration*> if_body_declarations;
  std::vector<CCSTStatement*> if_body_statements;
  
  std::vector<CCSTAssignExpr*> liblcd_err_args;
  liblcd_err_args.push_back(new CCSTString(err_msg));
  if_body_statements.push_back(new CCSTExprStatement( function_call("LIBLCD_ERR", liblcd_err_args)));

  std::vector<CCSTAssignExpr*> lcd_exit_args;
  lcd_exit_args.push_back(new CCSTInteger(-1));
  if_body_statements.push_back(new CCSTExprStatement( function_call("lcd_exit", lcd_exit_args)));

  CCSTCompoundStatement *if_body = new CCSTCompoundStatement(if_body_declarations, if_body_statements);
  return new CCSTIfStatement(cond, if_body);
}

CCSTIfStatement* if_cond_fail_goto(CCSTExpression *cond, const char *err_msg, const char* goto_label)
{
  std::vector<CCSTDeclaration*> if_body_declarations;
  std::vector<CCSTStatement*> if_body_statements;
  
  std::vector<CCSTAssignExpr*> liblcd_err_args;
  liblcd_err_args.push_back(new CCSTString(err_msg));
  if_body_statements.push_back(new CCSTExprStatement( function_call("LIBLCD_ERR", liblcd_err_args)));

  if_body_statements.push_back(new CCSTGoto(goto_label));

  CCSTCompoundStatement *if_body = new CCSTCompoundStatement(if_body_declarations, if_body_statements);
  return new CCSTIfStatement(cond, if_body);
}
