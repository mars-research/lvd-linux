#include "ccst.h"
#include "code_gen.h"

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
