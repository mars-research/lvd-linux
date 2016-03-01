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

CCSTUnaryOp* reference()
{
  return new CCSTUnaryOp(unary_bit_and_t);
}

const char* new_name(const char* name, const char* suffix)
{
  int length = strlen(name);
  int length2 = strlen(suffix);
  char *new_str = (char*) malloc(sizeof(char)*(length+length2+1));
  
  std::ostringstream total;
  total << name << suffix;
  strncpy(new_str, total.str().c_str(), length+length2+1);
  return new_str;
}

/* function pointer type*/
const char* container_name(const char* name)
{
  return new_name(name, "_container");
}

const char* hidden_args_name(const char* name)
{
  return new_name(name, "_hidden_args");
}

/* 
 * confirm this works
 * returns a new string with _p on end.
 */ 
const char* parameter_name(const char* name) 
{
  return new_name(name, "_p");
}

/*
 * returns a new string with _container on the end.
 */
const char* trampoline_func_name(const char* name)
{
  return new_name(name, "_trampoline");
}

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

CCSTCompoundStatement* alloc_init_containers_driver(Variable *v, ProjectionType *pt, LexicalScope *ls, const char *side)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;

  std::vector<CCSTInitDeclarator*> err_decs;
  err_decs.push_back(new CCSTDeclarator(0x0, new CCSTDirectDecId("err")));
  CCSTDeclaration *err_variable = new CCSTDeclaration(int_type(), err_decs);
  declarations.push_back(err_variable);

  statements.push_back(alloc_init_containers(v, pt, ls, side));
  return new CCSTCompoundStatement(declarations, statements);
}

// need to pass scope!
CCSTCompoundStatement* alloc_init_containers(Variable *v, ProjectionType *pt, LexicalScope *ls, const char *side) 
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;

  // declare container
  const char* container_name_ = container_name(pt->name());
  declarations.push_back(struct_pointer_declaration(container_name_, container_name_, ls));

  int err;
  Type* container_tmp = ls->lookup(container_name_, &err);
  Assert(container_tmp != 0x0, "Error: could not find type in scope\n");
  ProjectionType *container = dynamic_cast<ProjectionType*>(container_tmp);
  Assert(container != 0x0, "Error: dynamic cast to projection failed\n");
  
  // alloc
  statements.push_back(kzalloc_structure(container_name_, container_name_));

  // error check
  // if null
  // LIBLCD_ERR("kzalloc");
  //	lcd_exit(-1); /* abort */
  statements.push_back(if_cond_fail(new CCSTUnaryExprCastExpr(Not(), new CCSTPrimaryExprId(container_name_))
				    , "kzalloc"));

  // insert into dstore
  // do error checking
  // err = lcd_dstore_insert...
  std::vector<CCSTAssignExpr*> dstore_insert_args;
  dstore_insert_args.push_back(new CCSTPrimaryExprId("dstore")); // lookup this name in the future.
  dstore_insert_args.push_back(new CCSTPrimaryExprId(container_name_)); // what we are inserting
  
  dstore_insert_args.push_back(new CCSTEnumConst("STRUCT_FILE_TAG")); // this part is not clear
  
  ProjectionField *my_ref_field = container->get_field("my_ref");
  Assert(my_ref_field != 0x0, "Error: could not find field in projection\n");
  
  dstore_insert_args.push_back(access(my_ref_field));	
  
  // insert into dstore
  statements.push_back(new CCSTAssignExpr(new CCSTPrimaryExprId("err"), equals(), function_call("lcd_dstore_insert", dstore_insert_args)));
  
  // do error checking
  statements.push_back(if_cond_fail(new CCSTPrimaryExprId("err"), "dstore"));


  Parameter *tmp = new Parameter(container, container_name_, 1);
  std::vector<ProjectionField*> fields = pt->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    ProjectionField *pf = *it;
    pf->set_accessor(v);
    if(pf->type()->num() == 4 && alloc_caller(pf, side)) {
      ProjectionType *pt2 = dynamic_cast<ProjectionType*>(pf->type());
      Assert(pt2 != 0x0, "Error: dynamic cast to Projection type failed!\n");
      
      statements.push_back(alloc_init_containers(pf, pt2, ls, side)); 

      // need to link/init
      
      // need to access this field, but from the container....
      Assert(pf->accessor() != 0x0, "Error: field %s does not have a surrounding variable\n", pf->identifier());
      Variable *sv = pf->accessor()->accessor();
      pf->accessor()->set_accessor(tmp);

      // 1. get container for this field
      const char* container_name2 = container_name(pt2->name());
      Type *container_tmp2 = ls->lookup(container_name2, &err); // fix
      Assert(container_tmp2 != 0x0, "Error: could not find container in environment\n");
      ProjectionType *container2 = dynamic_cast<ProjectionType*>(container_tmp2);
      Assert(container2 != 0x0, "Error: dynamic cast to Projection type failed!\n");
  
      // 2. in container look up field
      ProjectionField *tmp_pf = container2->get_field(pt2->name()); // make sure name returns the name i think it does

      // 3. create a tmp variable
      Parameter *tmp2 = new Parameter(container2, container_name2, 1);
      
      // 4. set accessor in field.
      tmp_pf->set_accessor(tmp2);
      statements.push_back(new CCSTAssignExpr(access(pf), equals(), new CCSTUnaryExprCastExpr(reference() ,access(tmp_pf)))); // doing "&" may be wrong... put in function that checks
    }
    
  }

  return new CCSTCompoundStatement(declarations, statements);
}

CCSTCompoundStatement* alloc_init_trampoline(Variable *v, ProjectionType *pt, LexicalScope *ls)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;

  int err;
  Type *container_type = ls->lookup(container_name(pt->name()), &err);
  Assert(container_type != 0x0, "Error: could not find projection in scope\n");
  ProjectionType *container = dynamic_cast<ProjectionType*>(container_type);
  Assert(container != 0x0, "Error: dynamic cast to projection failed\n");
  Parameter *tmp_container = new Parameter(container, container_name(pt->name()), 1);

  std::vector<ProjectionField*> fields = pt->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {

    ProjectionField *pf = *it;
    pf->set_accessor(v);
    if (pf->type()->num() == 4) {
      ProjectionType *pt = dynamic_cast<ProjectionType*>(pf->type());
      Assert(pt != 0x0, "Error: dynamic cast to ProjectionType failed.\n");
      statements.push_back(alloc_init_trampoline(pf, pt, ls));
    } else if (pf->type()->num() == 7) {
      // declare trampoline container
      const char* tramp_name = hidden_args_name( pf->type()->name() ); // todo
      declarations.push_back(struct_pointer_declaration(tramp_name, tramp_name, ls)); // ls todo
    
      // alloc
      statements.push_back(kzalloc_structure(tramp_name, tramp_name));
    
      // error checking				
      statements.push_back(if_cond_fail(new CCSTUnaryExprCastExpr(Not(), new CCSTPrimaryExprId(tramp_name))
					, "kzalloc"));

      // dup trampoline
      /* 
	 rm_file_hidden_args->t_handle = 
	 LCD_DUP_TRAMPOLINE(rm_file_trampoline);
	 if (!rm_file_hidden_args->t_handle) {
	 LIBLCD_ERR("dup trampoline");
	 lcd_exit(-1);
	 } */
      int err;
      Type *tmp_trampoline = ls->lookup(tramp_name, &err);
      Assert(tmp_trampoline != 0x0, "Error: could not find trampline in environment\n");
      ProjectionType *trampoline = dynamic_cast<ProjectionType*>(tmp_trampoline);
      Assert(trampoline != 0x0, "Error: dynamic cast to Projection type failed!\n");

      Parameter *tmp = new Parameter(trampoline, tramp_name, 1);
      ProjectionField *t_handle_pf = trampoline->get_field("t_handle");
      Assert(pf != 0x0, "Error: could not find field in projection\n");
      pf->set_accessor(tmp);
  
      std::vector<CCSTAssignExpr*> dup_args;
      dup_args.push_back(new CCSTPrimaryExprId(trampoline_func_name( pf->type()->name() )));
      statements.push_back(new CCSTAssignExpr(access(t_handle_pf), equals(), function_call("LCD_DUP_TRAMPOLINE", dup_args))); // access t_handle
  
      // error check duplication
      statements.push_back(if_cond_fail(access(t_handle_pf), "dup trampoline"));

      // link t_handle hidden args with trampoline
      Assert(t_handle_pf->type()->num() == 4, "Error: field is not a projection\n"); 
      ProjectionType *tmp_t_handle = dynamic_cast<ProjectionType*>(t_handle_pf->type());
      Assert(tmp_t_handle != 0x0, "Error: dynamic cast to projection failed\n");
      
      ProjectionField *ha_tmp = tmp_t_handle->get_field("hidden_args"); // use projeciton type not field.
      Assert(ha_tmp != 0x0, "Error: could not find field in projection\n"); // could easily make get field function handle this
      statements.push_back(new CCSTAssignExpr(access(ha_tmp), equals(), new CCSTPrimaryExprId(tramp_name))); // todo

      // link trampoline container field. with container
      ProjectionField *c_tmp = trampoline->get_field(container_name(pt->name())); // todo
      Assert(c_tmp != 0x0, "Error: could not find field in projection");
      c_tmp->set_accessor(tmp);
      statements.push_back(new CCSTAssignExpr(access(c_tmp), equals(), new CCSTPrimaryExprId(container_name(pt->name())))); // todo

      // link data store
      ProjectionField *d_tmp = trampoline->get_field("dstore"); // todo
      Assert(d_tmp != 0x0, "Error: could not find field in projection");
      d_tmp->set_accessor(tmp);
      statements.push_back(new CCSTAssignExpr(access(d_tmp), equals(), new CCSTPrimaryExprId("dstore_name_todo"))); // todo

      // install trampoline in struct container function pointer.?
      // todo access field
      std::vector<CCSTAssignExpr*> handle_to_tramp_args;
      handle_to_tramp_args.push_back(access(t_handle_pf)); // pf defined on line 770

      // need to access this field. from its struct's container
      Assert(pf->accessor() != 0x0, "Error: accessor field is null\n");
      Variable *tmp_var = pf->accessor()->accessor();
      pf->accessor()->set_accessor(tmp_container);
      statements.push_back(new CCSTAssignExpr(access(pf), equals(), function_call("LCD_HANDLE_TO_TRAMPOLINE", handle_to_tramp_args)));

      pf->accessor()->set_accessor(tmp_var);

    }
  }
  return new CCSTCompoundStatement(declarations, statements);
}

/*
 * looks up structure and returns a declaration for a variable 
 * with name var_name that is a pointer to struct with name struct_name
 */
CCSTDeclaration* struct_pointer_declaration(const char* struct_name, const char* var_name, LexicalScope *ls)
{
  int err;
  Type *struct_tmp = ls->lookup(struct_name, &err); // fix
  Assert(struct_tmp != 0x0, "Error: could not find container in environment\n");
  ProjectionType *struct_ = dynamic_cast<ProjectionType*>(struct_tmp);
  Assert(struct_ != 0x0, "Error: dynamic cast to Projection type failed!\n");
  
  std::vector<CCSTInitDeclarator*> decs;
  decs.push_back(new CCSTDeclarator(new CCSTPointer(), new CCSTDirectDecId(var_name)));
  return new CCSTDeclaration(type2(struct_), decs);
}

CCSTStatement* kzalloc_structure(const char* struct_name, const char* var_name)
{
  // alloc
  std::vector<CCSTAssignExpr*> kzalloc_args;
  kzalloc_args.push_back(new CCSTUnaryExprSizeOf(new CCSTUnaryExprCastExpr(new CCSTUnaryOp(unary_mult_t)
									   , new CCSTPrimaryExprId(struct_name))));
  kzalloc_args.push_back(new CCSTEnumConst("GFP_KERNEL"));
  return new CCSTAssignExpr(new CCSTPrimaryExprId(var_name), equals(), function_call("kzalloc", kzalloc_args));
}

bool alloc_caller(Variable *v, const char *side)
{
  if(strcmp(side, "caller") == 0) {
    return v->alloc_caller();
  }
  
  return v->alloc_callee();
}

// function pointer trampoline code

CCSTDeclaration* trampoline_function_declaration(Rpc* r)
{
  /*
  LCD_TRAMPOLINE_DATA(new_file_trampoline);
  int 
  LCD_TRAMPOLINE_LINKAGE(new_file_trampoline)
  new_file_trampoline(int id, struct file **file_out)
  */
  std::vector<CCSTDecSpecifier*>specifier = type2(r->return_variable()->type());
  
  
  std::vector<CCSTInitDeclarator*> decs;

  std::vector<CCSTParamDeclaration*> tramp_func_params;

  std::vector<Parameter*> parameters = r->parameters();
  for(std::vector<Parameter*>::iterator it = parameters.begin(); it != parameters.end()-2; it ++) {
    Parameter *p = *it;
    
  }
  std::vector<Parameter*> real_parameters(parameters.begin(), parameters.end()-2);
  
  decs.push_back(new CCSTDeclarator(pointer(r->return_variable()->pointer_count())
				    , new CCSTDirectDecParamTypeList(new CCSTDirectDecId(trampoline_func_name(r->name()))
								     , parameter_list(real_parameters))));
  return new CCSTDeclaration(specifier, decs);
}

CCSTCompoundStatement* trampoline_function_body(Rpc* r)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  // todo new function poitner name
  
  // declare a new function pointer
  std::vector<CCSTDecSpecifier*> new_fp_specifier = type2(r->return_variable()->type());

  std::vector<CCSTInitDeclarator*> new_fp_decs;

  /* loop through rpc parameters and add them to the parameters for the new fp*/
  std::vector<CCSTParamDeclaration*> func_pointer_params;

  std::vector<Parameter*> parameters = r->parameters();
  for(std::vector<Parameter*>::iterator it = parameters.begin(); it != parameters.end(); it ++) {
    Parameter *p = *it;

    std::vector<CCSTDecSpecifier*> fp_param_tmp = type2(p->type());
    func_pointer_params.push_back(new CCSTParamDeclaration(fp_param_tmp));
  }

  /* declare new function pointer */
  std::vector<type_qualifier> pointer_type_qualifier;
  pointer_type_qualifier.push_back(volatile_t);

  new_fp_decs.push_back(new CCSTDeclarator(0x0
					   , new CCSTDirectDecParamTypeList(new CCSTDirectDecDec(new CCSTDeclarator(new CCSTPointer(pointer_type_qualifier)
														    , new CCSTDirectDecId("new_fp"))) // todo
									    , new CCSTParamList(func_pointer_params))));

  declarations.push_back(new CCSTDeclaration(new_fp_specifier, new_fp_decs));
  
  // declare a hidden args instance

  // 	struct new_file_hidden_args *hidden_args;
  declarations.push_back(struct_pointer_declaration(hidden_args_name(r->name()), "hidden_args", r->current_scope()));

  // LCD_TRAMPOLINE_PROLOGUE

  std::vector<CCSTAssignExpr*> lcd_tramp_prolog_args;
  lcd_tramp_prolog_args.push_back(new CCSTPrimaryExprId("hidden_args"));
  lcd_tramp_prolog_args.push_back(new CCSTPrimaryExprId(trampoline_func_name(r->name())));
  // LCD_TRAMPOLINE_PROLOGUE(hidden_args, new_file_trampoline);
  statements.push_back(function_call("LCD_TRAMPOLINE_PROLOGUE", lcd_tramp_prolog_args));

  // set new function pointer equal to glue code for function pointer
  // new_filep = new_file;
  statements.push_back(new CCSTAssignExpr(new CCSTPrimaryExprId("new_fp"), equals(), new CCSTPrimaryExprId(r->name())));

  // return call from new function pointer
  std::vector<CCSTAssignExpr*> new_fp_args;
  for(std::vector<Parameter*>::iterator it = parameters.begin(); it != parameters.end()-2; it ++) {
    Parameter *p = *it;
    new_fp_args.push_back(new CCSTPrimaryExprId(p->identifier()));
  }
  
  ProjectionType *hidden_args_param = dynamic_cast<ProjectionType*>(parameters.at(parameters.size()-2)->type());

  Assert(hidden_args_param != 0x0, "Error: dynamic cast to Projection type failed!\n");
  ProjectionField *container_field = hidden_args_param->get_field("container");
  if(container_field != 0x0) {
    new_fp_args.push_back(access(container_field));
  }

  ProjectionField *dstore_field = hidden_args_param->get_field("dstore");
  if(dstore_field != 0x0) {
    new_fp_args.push_back(access(dstore_field));
  }
  statements.push_back(new CCSTReturn(function_call("new_fp", new_fp_args)));

  return new CCSTCompoundStatement(declarations, statements);
}
