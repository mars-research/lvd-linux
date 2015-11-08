#include "ccst.h"
#include "code_gen.h"

/*
  creates the C code for a container struct
  for the projection provided
 */
CCSTDeclaration* container_struct_definition(const char* name, ProjectionType *pt, bool channel)
{
  // field for the real struct
  // field for other side capability
  // field for my ref capability

  // optional channel
  std::vector<CCSTStructDeclaration*> container_fields;
  
  std::vector<CCSTStructDeclarator*> real_struct_field;
  real_struct_field.push_back(new CCSTStructDeclarator(new CCSTDeclarator(0x0, new CCSTDirectDecId(pt->real_type()))));
  container_fields.push_back(new CCSTStructDeclaration(type(pt), real_struct_field));

  std::vector<CCSTStructDeclarator*> remote_ref_field;
  remote_ref_field.push_back(new CCSTStructDeclarator(new CCSTDeclarator(0x0, new CCSTDirectDecId("remote_ref"))));
  
  std::vector<CCSTDecSpecifier*> dptr_t;
  dptr_t.push_back(new CCSTTypedefName("dptr_t"));
  container_fields.push_back(new CCSTStructDeclaration(dptr_t, remote_ref_field));
  
  std::vector<CCSTStructDeclarator*> my_ref_field;
  my_ref_field.push_back(new CCSTStructDeclarator(new CCSTDeclarator(0x0, new CCSTDirectDecId("my_ref"))));
  container_fields.push_back(new CCSTStructDeclaration(dptr_t, my_ref_field));


  if(channel) {
    std::vector<CCSTDecSpecifier*> cptr_t;
    cptr_t.push_back(new CCSTTypedefName("cptr_t"));
    
    std::vector<CCSTStructDeclarator*> channel_field;
    channel_field.push_back(new CCSTStructDeclarator(new CCSTDeclarator(0x0, new CCSTDirectDecId("chnl"))));
    container_fields.push_back(new CCSTStructDeclaration(cptr_t, channel_field));
  }
  CCSTStructUnionSpecifier *container = new CCSTStructUnionSpecifier(struct_t, name, container_fields);

  std::vector<CCSTDecSpecifier*> struct_specifier;
  struct_specifier.push_back(container);
  std::vector<CCSTInitDeclarator*> empty;
  return new CCSTDeclaration(struct_specifier, empty);
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
  return 0x0
}

CCSTAssignOp* equals() 
{
  return new CCSTAssignOp(equal_t);
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

  std::vector<CCSTDecSpecifier*> type_for_global = type(gv->type());
  specifier.insert(specifier.end(), type_for_global.begin(), type_for_global.end());

  std::vector<CCSTInitDeclarator*> declarators;
  declarators.push_back(new CCSTDeclarator( pointer( count_nested_pointer(gv->type())),
					    new CCSTDirectDecId(gv->identifier())));
  
  return new CCSTDeclaration(specifier, declarators);
}

//??????????
CCSTPostFixExpr* access_variable(Variable *p)
{
  if(p->accessor() == 0x0) {
    return new CCSTPrimaryExprId(p->identifier());
  }
  
  return new CCSTPostFixExprAccess(access_parameter(p->accessor()), accessor, p->identifier()); 
}

CCSTExDeclaration* construct_enum(Module *f)
{
  const char* enum_name = "todo";
  CCSTEnumeratorList *el = construct_enumlist(f->rpc_defs());
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
      char* upper_name = string_to_upper(r->name());
      char* enum_name = (char*)malloc((sizeof(upper_name)+9+1)*sizeof(char));
      sprintf(enum_name, "%s_CALLEE_T", upper_name);
      list->push_back(new CCSTEnumerator(enum_name));
    }
  CCSTEnumeratorList *enum_list = new CCSTEnumeratorList(list);
  return enum_list;
}

const char* string_to_upper(char* str)
{
  const char* ret = (char*) malloc((sizeof(str)+1)*sizeof(char));
  int i;
  for(i = 0; i < sizeof(str); i ++)
    {
      char tmp = str[i];
      ret[i] = toupper(tmp);
    }
  ret[i] = '\0';
  return ret;
}

Type* get_non_pointer_type(PointerType *p)
{
  Assert(p->type() != 0x0, "Error: pointer to null\n");
  
  if(p->type()->num() != 3)
    return p->type();
  PointerType *pt = dynamic_cast<PointerType*>(p->type());
  return get_non_pointer_type(pt);
}

int count_nested_pointer(Type *p)
{
  if(p->num() != 3)
    return 0;
  else
    {
      PointerType *tmp = dynamic_cast<PointerType*>(p);
      return 1 + count_nested_pointer(tmp->type());
    }
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

CCSTParamTypeList* parameter_list()
{
  
}

/* creates a function declaration
 * from an rpc, not callee
 */
CCSTDeclaration* function_declaration(Rpc* r)
{
  std::vector<CCSTDecSpecifier*> specifier = type(r->explicit_return_type());
  
  std::vector<CCSTInitDeclarator*> func; // = new std::vector<CCSTInitDeclarator*>(); // pointer name, params
  int pointer_count = count_nested_pointer(r->explicit_return_type());
  
  CCSTPointer *p = pointer(pointer_count);
  
  CCSTDirectDecId *name = new CCSTDirectDecId(r->name());
  CCSTParamTypeList *param_list = create_parameter_list();
  CCSTDirectDecParamTypeList *name_params = new CCSTDirectDecParamTypeList(name, param_list);
  
  func.push_back(new CCSTDeclarator(p, name_params));
  
  return new CCSTDeclaration(specifier, func);
}

std::vector<CCSTDecSpecifier*> type(Type *t)
{
  std::vector<CCSTDecSpecifier*>specifier;
  switch(t->num())
    {
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
    case 3:
      {
	PointerType *pt = dynamic_cast<PointerType*>(t);
	Assert(pt != 0x0, "Error: dynamic cast failed!\n");
	return type(pt->type());
      }
    default:
      {
	Assert(1 == 0, "Error: Not a struct or integer type.\n");
      }
    }
}


CCSTTypeName* type_cast(Type *t)
{
  printf("in type+cast\n");
  CCSTAbstDeclarator *pointers = 0x0;
  std::vector<CCSTSpecifierQual*> spec_quals;

  if(t == 0x0)
    printf("t is null\n");
  if(t->num() == 3)
    {
      printf("type cast pointer\n");
      // do pointer stuff....
      pointers = new CCSTAbstDeclarator( pointer(count_nested_pointer(t)) , 0x0); 
      
      PointerType *p = dynamic_cast<PointerType*>(t);
      t = get_non_pointer_type(p); // get first non pointer, write function for this
    }
  
  printf("in type castttt\n");
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
