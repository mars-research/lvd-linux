#include "ccst.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

/* example
   
   #ifndef LCD_PROTOTYPE_MANUFACTURER_IDL_H
   #define LCD_PROTOTYPE_MANUFACTURER_IDL_H
   
   enum manufacturer_interface_enum {
   MANUFACTURER_MK_ENGINE,
   MANUFACTURER_MK_AUTOMOBILE,
   MANUFACTURER_FREE_ENGINE,
   MANUFACTURER_FREE_AUTOMOBILE,
   MANUFACTURER_DIE,
   };
   
   Locations of manufacturer's boot cptrs 
   #define MANUFACTURER_DEALER_INTERFACE_CAP 31
   
   #endif
*/

/* producing:
   
   declaration --> declaration-specifier --> type-specifier --> enum-specifier
   
   enum-specifier: enum id { enumerator-list };
   
   enumerator-list = enumerator
   | enumerator-list , enumerator
   
   enumerator = id
   | id = constant-expression
*/
CCSTFile* generate_client_header(File* f)
{
}

CCSTFile* generate_client_source(File* f)
{
  
  std::vector<CCSTExDeclaration*> definitions;
  
  
  CCSTFile *c_file = new CCSTFile(definitions);
  return c_file;
}

CCSTFile* generate_server_header(File *file)
{
  // #ifndef
  // #define
  // #endif
  // enum-specifier: enum id 
  std::vector<CCSTExDeclaration*> definitions; // = new std::vector<CCSTExDeclaration*>();
  // check if there are rpcs
  if(!file->rpc_defs().empty())
    {
      printf("rpc not empty\n");
      definitions.push_back(construct_enum(file));
      // function callee function declarations
      std::vector<Rpc*> rpcs = file->rpc_defs();
      for(std::vector<Rpc*>::iterator it = rpcs.begin(); it != rpcs.end(); it ++)
	{
	  definitions.push_back(construct_callee_declaration((Rpc*) *it));
	}
    }
  CCSTFile *c_file = new CCSTFile(definitions);
  return c_file;
}

CCSTDeclaration* construct_callee_declaration(Rpc* r)
{
  std::vector<CCSTDecSpecifier*> specifier; // = new std::vector<CCSTDecSpecifier*>();
  specifier.push_back(new CCSTSimpleTypeSpecifier(int_t));
  char * callee_name = (char*) malloc((strlen(r->name())+strlen("_callee")+1)*sizeof(char));
  sprintf(callee_name, "%s%s", r->name(), "_callee");
  CCSTDirectDecId* id = new CCSTDirectDecId(callee_name);
  
  std::vector<CCSTDecSpecifier*> *s = new std::vector<CCSTDecSpecifier*>();
  s->push_back(new  CCSTSimpleTypeSpecifier(void_t));
  CCSTParamDeclaration *parameter = new CCSTParamDeclaration(s);
  
  std::vector<CCSTParamDeclaration*> *p_decs = new std::vector<CCSTParamDeclaration*>();
  p_decs->push_back(parameter);
  CCSTParamList *param_list = new CCSTParamList(p_decs);

  CCSTDirectDecParamTypeList *params = new CCSTDirectDecParamTypeList(id, param_list); 
    
  CCSTDeclarator* declarator = new CCSTDeclarator(NULL, params);
  std::vector<CCSTInitDeclarator*> init_declarator; // = new std::vector<CCSTInitDeclarator*>();
  init_declarator.push_back(declarator);
  CCSTDeclaration *func_declaration = new CCSTDeclaration(specifier, init_declarator);
  
  return func_declaration;
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

/* ServerCCSTSourceVisitor */


CCSTFile* generate_server_source(File *file)
{
  printf("In generate_server_source\n");
  // <function-definition> is CCSTFuncDef
  // CCSTDecSpecifier* is <type-specifier> is CCSTTypeSpecifier
  // <declarator> is CCSTDeclarator
  // <declaration> what is this for is CCSTDeclaration
  // <compound-statement> is CCSTCompoundStatement
  // CCSTDeclaration for body
  // CCSTStatement for body

  // see notes in notebook

  /*  
  std::vector<CCSTExDeclaration*> defs;
  CCSTFuncDef* exec_loop = create_exec_loop(file->rpc_defs());
  defs.push_back(exec_loop);
  return new CCSTFile(defs);
  */

  std::vector<CCSTExDeclaration*> definitions;
  
  // dispatch
  /*
  create_function_definition(create_function_declaration()
			     ,create_dispatch_loop_body(file->rpc_defs()));
  */
  std::vector<Rpc*> rps = file->rpc_defs();
  for(std::vector<Rpc*>::iterator it = rps.begin(); it != rps.end(); it ++)
     {
       Rpc* r_tmp = (Rpc*) *it;
       definitions.push_back( create_function_definition(construct_callee_declaration(r_tmp)
							,create_callee_body(r_tmp)));
     }
   
   CCSTFile *c_file = new CCSTFile(definitions);
   printf("in server source gen\n");
   return c_file;
}

/* ------------------------ new ---------------------------- */

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
CCSTFuncDef* create_function_definition(CCSTDeclaration* function_declaration, CCSTCompoundStatement *body)
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
CCSTDeclaration* create_function_declaration(Rpc* r)
{
  std::vector<CCSTDecSpecifier*> specifier = get_type(r->return_type());
  
  std::vector<CCSTInitDeclarator*> func; // = new std::vector<CCSTInitDeclarator*>(); // pointer name, params
  int pointer_count = count_nested_pointer(r->return_type());
  
  CCSTPointer *p = create_pointer(pointer_count);
  
  CCSTDirectDecId *name = new CCSTDirectDecId(r->name());
  CCSTParamTypeList *param_list = create_parameter_list();
  CCSTDirectDecParamTypeList *name_params = new CCSTDirectDecParamTypeList(name, param_list);
  
  func.push_back(new CCSTDeclarator(p, name_params));
  
  return new CCSTDeclaration(specifier, func);
}

/* create a function declaration
 * not from an rpc
 * need return type
 * need name
 * need params
 */


CCSTDeclaration* create_function_declaration()
{
}

/* specific function body creators */

CCSTCompoundStatement* create_dispatch_loop_body(std::vector<Rpc*>* rps)
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
  for(std::vector<Rpc*>::iterator it = rps->begin(); it != rps->end(); it ++)
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
CCSTCompoundStatement* create_callee_body(Rpc *r)
{
  // unmarshal parameters based on marshal data.
  // which says where params are stored.
  
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  
  // loop through params, declare a tmp and pull out marshal value
  std::vector<Parameter*> params = r->parameters();
  std::vector<char*> param_names;

  std::vector<CCSTAssignExpr*> unmarshalled_args;
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++)
    {
      Parameter *p = (Parameter*) *it;
      CCSTCompoundStatement *tmp = unmarshal_parameter(p);
      
      CCSTPrimaryExprId *t = new CCSTPrimaryExprId(p->name());
      unmarshalled_args.push_back(t);
      statements.push_back(tmp);
    }

  // pulled out params make function call

  CCSTInitializer *call = new CCSTInitializer( new CCSTPostFixExprAssnExpr( new CCSTPrimaryExprId(r->name())
									    ,unmarshalled_args ) );
  CCSTPointer *pp = 0x0;
  if(r->return_type()->num() == 3)
    {
      pp = create_pointer(count_nested_pointer(r->return_type()));
    }
  
  const char *ret_name = "ret";
  CCSTDeclarator *ret_value_name = new CCSTDeclarator(pp
						      , new CCSTDirectDecId(ret_name) );

  std::vector<CCSTDecSpecifier*> real_call_specifier;
  std::vector<CCSTInitDeclarator*> real_call_decs;
  
  real_call_specifier = get_type(r->return_type());
  real_call_decs.push_back(new CCSTInitDeclarator(ret_value_name
						  , call));
  
  CCSTDeclaration *ret_value = new CCSTDeclaration(real_call_specifier
						   , real_call_decs);
  
  declarations.push_back(ret_value);

  // marshal return value
  std::vector<CCSTAssignExpr*> ret_arg;
  ret_arg.push_back(new CCSTPrimaryExprId(ret_name));
  CCSTPostFixExprAssnExpr *marshal_ret = new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId("func_name")
								     ,ret_arg);
  statements.push_back(new CCSTExprStatement(marshal_ret));
  return new CCSTCompoundStatement(declarations, statements);
}

CCSTCompoundStatement* unmarshal_projection_parameter(const char* param_name, std::vector<int> reg , ProjectionType *pt, bool isPointer)
{
  printf("here in unmarshal proj parameter\n");
  if(isPointer)
    {
      printf("proj is a pointer\n");
      // For now malloc an instance of the struct
      
      std::vector<CCSTInitDeclarator*> d;
      
      CCSTPointer *__p = new CCSTPointer(); // just 1 pointer for now.
      
      CCSTDeclarator *name = new CCSTDeclarator(__p
						, new CCSTDirectDecId( param_name ));
      
      std::vector<CCSTAssignExpr*> args;
  
      printf("Right before type_cast call\n");
      CCSTTypeName *type_name = type_cast(pt);

      printf("just made type_cast call\n");
      if(type_name == 0x0)
	printf("type name is null\n");
      args.push_back(new CCSTUnaryExprSizeOf(type_name));
      
      CCSTCastExpr *m =  new CCSTCastExpr(type_name
					  , new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId("malloc")
								       , args) );
      
      CCSTInitializer *malloc_call = new CCSTInitializer(m); //TODO right here
      
      CCSTInitDeclarator *init_struct = new CCSTInitDeclarator(name
							       , malloc_call);
      d.push_back(init_struct);
      
      CCSTDeclaration *new_struct = new CCSTDeclaration(get_projection_type(pt)
							, d);
      
      // end malloc struct
      

      // not sure if above is totally correct
      std::vector<CCSTDeclaration*> comp_decs;

      comp_decs.push_back(new_struct);

      /* putting each field in */
      std::vector<CCSTStatement*> comp_stmts;
      std::vector<ProjectionField*> fields = pt->fields();
      
      int index = 0;
      for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++)
	{
	  ProjectionField *pf = *it;
	  comp_stmts.push_back( unmarshal_projection_field(pf->field_name(), reg.at(index), pf) );
	  
	  comp_stmts.push_back( set_value_in_struct(param_name, pf->field_name(), pf->field_name(), isPointer) );
	}
      
      return new CCSTCompoundStatement(comp_decs, comp_stmts);
      
    }
  else
    {
      
    }
  printf("todo projection param\n");

  // does C even use structs as params that are not pointesr to structs on heap?
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


// change here too?
CCSTCompoundStatement* unmarshal_parameter(Parameter *p)
{
  // create visitor
  return p->get_marshal_info()->accept(visitor, p->type());
}

/* body for a caller function
 * does marshaling, ipc, etc
 */
CCSTCompoundStatement* create_caller_body(Rpc *r)
{
  // marshal
  // ipc call

  // unmarshal
  // return result
}


/*
 * Takes a Type and returns the CAST equivalent
 * Didn't feel it was necessary to write a visitor
 * When this will do essentially the same thing.
 * Slightly grosser than visitor because must do casting
 */
std::vector<CCSTDecSpecifier*> get_type(Type *t)
{
  std::vector<CCSTDecSpecifier*> ret;
  switch (t->num())
    {
    case 1: // typedef
      {
	// Choice.  use "real type" or use user defined type
	// and put typedef at top of file.
	// go with this, will make code more readable
	
	Typedef *td = dynamic_cast<Typedef*>(t);
	CCSTTypedefName *tmp = new CCSTTypedefName(td->alias());
	ret.push_back(tmp);
	return ret;
      }
    case 2: // Integer type
      {
	IntegerType *it = dynamic_cast<IntegerType*>(t);
	return get_integer_type(it);
      }
    case 3: // Pointer Type
      {
	// don't "care" about pointer at this point,
	// return type that it is a pointer to
	PointerType *pt = dynamic_cast<PointerType*>(t);
	return get_type(pt->type());
      }
    case 4: // Projection Type
      {
	printf("about to call projection type\n");
	ProjectionType *pt = dynamic_cast<ProjectionType*>(t);
	return get_projection_type(pt);
      }
    case 5: // Void Type
      {
	ret.push_back( new CCSTSimpleTypeSpecifier(void_t));
	return ret;
      }
    default:
      {
	printf("error");
      }
    }
}


/*
// move to new file
CCSTDeclaration* container_struct_declaration()
{
  // needs to contain actual struct
  // needs to contain a capability
  // needs special asm code
 
  // what else
}
*/


/* code to unmarshal parameters */
// This code will produce the CAST that unmarshals the params. instead of where it was before
CCSTCompoundStatement* MarshalTypeVisitor::visit(Marshal_projection *data, Type *t)
{
  ProjectionType *pt = dynamic_cast<ProjectionType*>(t);
  Assert(pt != 0x0, "Dynamic cast has failed!\n");

  
}

CCSTCompoundStatement* MarshalTypeVisitor::visit(Marshal_integer *data, Type *t)
{
  IntegerType *pt = dynamic_cast<IntegerType*>(t);
  Assert(pt != 0x0, "Dynamic cast has failed!\n");
  
  std::vector<CCSTDecSpecifier*>specifier = type(t);
  
  std::vector<CCSTInitDeclarator*> decs;
  std::vector<CCSTAssignExpr*> empty_args;
  CCSTPostFixExprAssnExpr *func_call = new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId(access_register_mapping(data->get_register()))
								   ,empty_args);
  
  CCSTInitializer *init = new CCSTInitializer( new CCSTCastExpr(type_cast(t)
								,func_call));
  decs.push_back(new CCSTInitDeclarator(new CCSTDeclarator(0x0, new CCSTDirectDecId(data->get_name()))
					,init));
  
  std::vector<CCSTDeclaration*> comp_declarations;
  std::vector<CCSTStatement*> comp_statements;
  comp_declarations.push_back(new CCSTDeclaration(specifier, decs));
  return new CCSTCompoundStatement(comp_declarations, comp_statements);
}

CCSTCompoundStatement* MarshalTypeVisitor::visit(Marshal_void *data, Type *t)
{
  VoidType *pt = dynamic_cast<VoidType*>(t);
  Assert(pt != 0x0, "Dynamic cast has failed!\n");
}

CCSTCompoundStatement* MarshalTypeVisitor::visit(Marshal_typedef *data, Type *t)
{
  Typedef *pt = dynamic_cast<Typedef*>(t);
  Assert(pt != 0x0, "Dynamic cast has failed!\n");
}

 CCSTCompoundStatement* MarshalTypeVisitor::visit(Marshal_pointer *data, Type *t)
{
  PointerType *pt = dynamic_cast<PointerType*>(t);
  Assert(pt != 0x0, "Dynamic cast has failed!\n");

  CCSTPointer *__p = create_pointer(count_nested_pointer(pt));
  Type *inner_t = get_non_pointer_type(pt);
  
  std::vector<CCSTDecSpecifier*>specifier = type(inner_t);
  std::vector<CCSTInitDeclarator*> decs;
  const char* pointer_param_name = (const char*) malloc(sizeof(char)*(strlen(data->get_name())+4));
  sprintf(pointer_param_name, "%s_pt", data->get_name());
  decs.push_back(new CCSTInitDeclarator( new CCSTDeclarator(__p
							    , new CCSTDirectDecId(pointer_param_name))
					 , new CCSTInitializer( new CCSTUnaryExprCastExpr(new CCSTUnaryOp(unary_bit_and_t), new CCSTPrimaryExprId(data->get_name())))));

  CCSTCompoundStatement *c = data->get_m_type()->accept(this, inner_t);
  std::vector<CCSTDeclaration*> comp_declarations;
  comp_declarations.push_back(new CCSTDeclaration(specifier, decs));
  std::vector<CCSTStatement*> comp_statements;
  c->add_statement(new CCSTStatement(comp_declarations, comp_statements));
  return c;
}

CCSTDecSpecifier* type(Type *t)
{
  std::vector<CCSTDecSpecifier*>specifier;
  switch(t->num())
    {
    case 2: // int type case
      {
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
    default:
      {
	Assert(1 == 0, "Error: Not a struct or integer type.\n");
      }
    }
}
 
