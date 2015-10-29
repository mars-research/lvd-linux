#include "ccst.h"

/*
  creates the C code for a container struct
  for the projection provided
 */
CCSTDeclaration* container_struct(const char* name, ProjectionType *pt)
{

  CCSTStructDeclaration *container_field = new CCSTStructDeclaration();
  CCSTStructUnionSpecifier *container = new CCSTStructUnionSpecifier(struct_t, name, container_fields);


  std::vector<CCSTDecSpecifier*> struct_specifier;
  struct_specifier.push_back(container);
  std::vector<CCSTInitDeclarator*> empty;
  return new CCSTDeclaration(struct_specifier, empty);
}




CCSTPostFixExpr* access_variable(Variable *p)
{
  if(p->accessor() == 0x0) {
    return new CCSTPrimaryExprId(p->identifier());
  }
  
  return new CCSTPostFixExprAccess(access_parameter(p->accessor()), accessor, p->identifier()); 
}

CCSTExDeclaration* construct_enum(File *f)
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

const char* construct_enum_name()
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

char* string_to_upper(char* str)
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
CCSTPointer* create_pointer(int p_count)
{
  if(p_count == 0)
    return 0x0;
  
  if(p_count == 1)
    return new CCSTPointer();
  
  return new CCSTPointer(create_pointer(p_count - 1));
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

CCSTParamTypeList* create_parameter_list()
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
  
  CCSTPointer *p = create_pointer(pointer_count);
  
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
      pointers = new CCSTAbstDeclarator( create_pointer(count_nested_pointer(t)) , 0x0); 
      
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
