#include "ccst.h"
#include "code_gen.h"

// for debugging purposes
char* type_number_to_name(int num)
{
  switch(num) {
  case 1:
    {
      return "typedef type";
    }
  case 2: 
    {
      return "integer type";
    }
  case 4:
    {
      return "projection type";
    }
  case 5:
    {
      return "void type";
    }
  case 6:
    {
      return "channel type";
    }
  case 7: 
    {
      return "function pointer type";
    }
  case 8:
    {
      return "unresolved type";
    }

  }
}


CCSTDeclaration* typedef_declaration(Typedef *t)
{
  /*
  std::vector<CCSTDecSpecifier*>specifier;
  specifier.push_back(new CCSTStoClassSpecifier(typedef_t));
  
  std::vector<CCSTInitDeclarator*> decs;

  */
  // possibly unnecessary depending on if proper header files are included

  printf("todo typedef\n");
  return 0x0;
}

CCSTAssignOp* equals() 
{
  return new CCSTAssignOp(equal_t);
}

CCSTUnaryOp* Not()
{
  return new CCSTUnaryOp(unary_bang_t);
}

/* 
 * confirm this works
 * returns a new string with _p on end.
 */ 
const char* parameter_name(const char* name) 
{
  int length = strlen(name);
  char *new_str = (char*) malloc(sizeof(char)*(length+3));
  
  std::ostringstream total;
  total << name << "_p";
  strncpy(new_str, total.str().c_str(), length+3);
  return new_str;
}

/*
 * returns a new string with _container on the end.
 */
/*
const char* container_name(const char* name)
{
  int length = strlen(name);
  int length2 = strlen("_container");
  char *new_str = (char*) malloc(sizeof(char)*(length+length2+1));

  std::ostringstream total;
  total << name << "_container";
  strncpy(new_str, total.str().c_str(), length+length2+1);
  return new_str;
}
*/

/*
 * takes the vector of global variables with also provides the parameters to the function.
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
 * CAST code to declare a static variable
 */
CCSTDeclaration* declare_static_variable(Variable *gv)
{
  
  std::vector<CCSTDecSpecifier*> specifier;
  specifier.push_back(new CCSTStoClassSpecifier(static_t));

  std::vector<CCSTSpecifierQual*> type_for_global = type(gv->type());
  specifier.insert(specifier.end(), type_for_global.begin(), type_for_global.end());

  std::vector<CCSTInitDeclarator*> declarators;
  declarators.push_back(new CCSTDeclarator( pointer(gv->pointer_count()),
					    new CCSTDirectDecId(gv->identifier())));
  
  return new CCSTDeclaration(specifier, declarators);
}

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

const char* string_to_upper(const char* str)
{
  char* ret = (char*) malloc((sizeof(str)+1)*sizeof(char));
  int i;
  for(i = 0; i < sizeof(str); i ++)
    {
      char tmp = str[i];
      ret[i] = toupper(tmp);
    }
  ret[i] = '\0';
  return ret;
}

/* creates a pointer, or pointer to pointer, etc*/
/* may need to be changed if want to do something like
 *    int * const name
 * int * volatile name
 */
CCSTPointer* pointer(int p_count)
{
  if(p_count == 0)
    return 0x0;
  
  if(p_count == 1)
    return new CCSTPointer();
  
  return new CCSTPointer(pointer(p_count - 1));
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

// constructs a type declaration from a name instead of a type object
std::vector<CCSTDecSpecifier*> struct_type(const char *type_name)
{
  std::vector<CCSTDecSpecifier*> specifier;
  specifier.push_back(new CCSTStructUnionSpecifier(struct_t, type_name));
  return specifier;
}

std::vector<CCSTDecSpecifier*> int_type()
{
  std::vector<CCSTDecSpecifier*>specifier;
  specifier.push_back(new CCSTSimpleTypeSpecifier(int_t));
  
  return specifier;
}

std::vector<CCSTDecSpecifier*> type2(Type *t)
{
  std::vector<CCSTDecSpecifier*>specifier;
  int num = t->num();
  switch(num)  {
  case 1:
    {
      // typdef 
      // todo
    }
  case 2: // int type case
    {
      IntegerType *it = dynamic_cast<IntegerType*>(t);
      Assert(it != 0x0, "Error: dynamic cast failed!\n");
      switch (it->int_type())
	{
	case pt_char_t:
	  {
	    specifier.push_back(new CCSTSimpleTypeSpecifier(char_t));
	    break;
	  }
	case pt_short_t:
	  {
	    specifier.push_back(new CCSTSimpleTypeSpecifier(short_t));
	    break;
	  }
	case pt_int_t:
	  {
	    specifier.push_back(new CCSTSimpleTypeSpecifier(int_t));
	    break;
	  }
	case pt_long_t:
	  {
	    specifier.push_back( new CCSTSimpleTypeSpecifier(long_t));
	    break;
	  }
	case pt_longlong_t:
	  {
	    specifier.push_back(new CCSTSimpleTypeSpecifier(long_t));
	    specifier.push_back(new CCSTSimpleTypeSpecifier(long_t));
	    break;
	  }
	case pt_capability_t:
	  {
	    specifier.push_back(new CCSTTypedefName("capability_t"));
	    break;
	  }
	default:
	  {
	    Assert(1 == 0, "Error: unknown type\n");
	  }
	}
      return specifier;
    }
  case 4: // struct
    {
      ProjectionType *pt = dynamic_cast<ProjectionType*>(t);
      Assert(pt != 0x0, "Error: dynamic cast failed!\n");
      specifier.push_back(new CCSTStructUnionSpecifier(struct_t, pt->real_type()));
      return specifier;
    }
  case 5:
    {
      specifier.push_back(new CCSTSimpleTypeSpecifier(void_t));
      return specifier;
    }
  case 6:
    {
      specifier.push_back(new CCSTTypedefName("cptr_t"));
      return specifier;
    }
  case 7:
    {
      // function pointer type
      // todo
    }
  default:
    {
      printf("Received %s instead of struct or integer\n", type_number_to_name(num));
      Assert(1 == 0, "Error: Not a struct or integer type.\n");
    }
  }
}

std::vector<CCSTSpecifierQual*> type(Type *t)
{
  std::vector<CCSTSpecifierQual*> specifier;

  int num = t->num();
  switch(num)
    {
    case 1:
      {
	specifier.push_back(new CCSTTypedefName(t->name()));
	break;
      }
    case 2: // int type case
      {
	IntegerType *it = dynamic_cast<IntegerType*>(t);
	Assert(it != 0x0, "Error: dynamic cast failed!\n");
	switch (it->int_type())
	  {
	  case pt_char_t:
	    {
	      specifier.push_back(new CCSTSimpleTypeSpecifier(char_t));
	      break;
	    }
	  case pt_short_t:
	    {
	      specifier.push_back(new CCSTSimpleTypeSpecifier(short_t));
	      break;
	    }
	  case pt_int_t:
	    {
	      specifier.push_back(new CCSTSimpleTypeSpecifier(int_t));
	      break;
	    }
	  case pt_long_t:
	    {
	      specifier.push_back( new CCSTSimpleTypeSpecifier(long_t));
	      break;
	    }
	  case pt_longlong_t:
	    {
	      specifier.push_back(new CCSTSimpleTypeSpecifier(long_t));
	      specifier.push_back(new CCSTSimpleTypeSpecifier(long_t));
	      break;
	    }
	  case pt_capability_t:
	    {
	      specifier.push_back(new CCSTTypedefName("capability_t"));
	      break;
	    }
	  default:
	    {
	      Assert(1 == 0, "Error: unknown type\n");
	    }
	  }
	return specifier;
      }
    case 4: // struct
      {
	ProjectionType *pt = dynamic_cast<ProjectionType*>(t);
	Assert(pt != 0x0, "Error: dynamic cast failed!\n");
	specifier.push_back(new CCSTStructUnionSpecifier(struct_t, pt->real_type()));
	return specifier;
      }
    case 5:
      {
	// void type
	// todo
	specifier.push_back(new CCSTSimpleTypeSpecifier(void_t));
	return specifier;
      }
    case 6:
      {
	// channel type
	// cptr_t define this somewhere for easy change
	specifier.push_back(new CCSTTypedefName("cptr_t"));
	return specifier;
      }
    case 7:
      {
	// function pointer type
	// todo
	// where is support in grammar
	break;
      }
    default:
      {
	printf("Received %s with name %s instead of struct or integer\n", type_number_to_name(num), t->name());
	Assert(1 == 0, "Error: Not a struct or integer type. \n");
      }
    }
}


CCSTTypeName* type_cast(Type *t, int pointer_count)
{
  printf("in type+cast\n");
  CCSTAbstDeclarator *pointers = new CCSTAbstDeclarator( pointer(pointer_count), 0x0);
  std::vector<CCSTSpecifierQual*> spec_quals;

  if(t == 0x0) {
    printf("t is null\n");
  }

  switch(t->num())
    {
    case 1: // typedef
      {
	Typedef *td = dynamic_cast<Typedef*>(t);
	const char* name = td->alias();
	spec_quals.push_back( new CCSTTypedefName(name) );
	break;
      }
      case 2: // integer
      {
	IntegerType *it = dynamic_cast<IntegerType*>(t);
	spec_quals = integer_type_cast(it);
	break;
      }
    case 4: // projection
      {
	printf("here in type cast\n");
	ProjectionType *pt = dynamic_cast<ProjectionType*>(t);
	const char* name = pt->real_type();

	printf("projection name: %s\n", name);
	spec_quals.push_back( new CCSTStructUnionSpecifier(struct_t, name) );
	break;
      }
    case 5: // void
      { // does this even happen?
	printf("Warning: casting something as void\n");
	spec_quals.push_back(new CCSTSimpleTypeSpecifier(void_t) );
	break;
      }
    default:
      {
	Assert(1 == 0, "Error: Should never get here\n");
      }
    }
  return new CCSTTypeName(spec_quals, pointers);
}


std::vector<CCSTSpecifierQual*> integer_type_cast(IntegerType *it)
{
  std::vector<CCSTSpecifierQual*> spec_quals;

  if(it->is_unsigned())
    {
      spec_quals.push_back(new CCSTSimpleTypeSpecifier(unsigned_t));
    }
  switch (it->int_type())
    {
    case pt_char_t:
      {
	spec_quals.push_back(new CCSTSimpleTypeSpecifier(char_t));
	break;
      }
    case pt_short_t:
      {
	spec_quals.push_back(new CCSTSimpleTypeSpecifier(short_t));
	break;
      }
    case pt_int_t:
      {
	spec_quals.push_back(new CCSTSimpleTypeSpecifier(int_t));
	break;
      }
    case pt_long_t:
      {
	spec_quals.push_back(new CCSTSimpleTypeSpecifier(long_t));
	break;
      }
    case pt_longlong_t:
      {
	spec_quals.push_back(new CCSTSimpleTypeSpecifier(long_t));
	spec_quals.push_back(new CCSTSimpleTypeSpecifier(long_t));
	break;
      }
    case pt_capability_t:
      {
	spec_quals.push_back(new CCSTTypedefName("capability_t"));
	break;
      }
    default:
      {
	printf("todo\n");
      }
    }
  return spec_quals;
}


CCSTUnaryOp* indirection()
{
  return new CCSTUnaryOp(unary_mult_t);
}

CCSTUnaryExprCastExpr* dereference(CCSTCastExpr *to_deref)
{
  return new CCSTUnaryExprCastExpr(indirection(), to_deref);
}

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

CCSTPrimaryExprId* function_name(const char *func_name)
{
  return new CCSTPrimaryExprId(func_name);
}

CCSTPostFixExprAssnExpr* function_call(const char *func_name, std::vector<CCSTAssignExpr*> args)
{
  return new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId(func_name), args);
}

/* 
 * given a projection, returns a struct declaration for the projection name 
 * including the projection fields.
 */
CCSTStructUnionSpecifier* struct_declaration(ProjectionType *pt)
{
  std::vector<CCSTStructDeclaration*> field_decs;

  std::vector<ProjectionField*> fields = pt->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    ProjectionField *pf = (ProjectionField*) *it;
    field_decs.push_back(new CCSTStructDeclaration( type(pf->type())
						    , new CCSTStructDeclarator( new CCSTDeclarator( pointer(pf->pointer_count()), new CCSTDirectDecId(pf->identifier())))));
  }
  
  return new CCSTStructUnionSpecifier(struct_t, pt->name(), field_decs);
}

CCSTIfStatement* if_cond_fail(CCSTExpression *cond, const char *err_msg)
{
  std::vector<CCSTDeclaration*> if_body_declarations;
  std::vector<CCSTStatement*> if_body_statements;
  
  std::vector<CCSTAssignExpr*> liblcd_err_args;
  liblcd_err_args.push_back(new CCSTString(err_msg));
  if_body_statements.push_back(function_call("LIBLCD_ERR", liblcd_err_args));

  std::vector<CCSTAssignExpr*> lcd_exit_args;
  lcd_exit_args.push_back(new CCSTInteger(-1));
  if_body_statements.push_back(function_call("lcd_exit", lcd_exit_args));

  CCSTCompoundStatement *if_body = new CCSTCompoundStatement(if_body_declarations, if_body_statements);
  return new CCSTIfStatement(cond, if_body);
}

