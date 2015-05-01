#include "ccst.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

/* taken from Scott Bauer 
 * Will be moved to more appropriate place
 */
static void AssertionFailure(char *exp, char *file, int line, const char* format, ... )
{
  printf("Assertion '%s' failed at line %d of file %s\n", exp, line, file);
  printf("Error is %s\n", strerror(errno));
  va_list args;
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
  exit(EXIT_FAILURE);
}

/* taken from Scott Bauer 
 * Will be moved to more appropriate place
 */
#define Assert(exp, format, ...) if (exp) ; else AssertionFailure( #exp, __FILE__,  __LINE__, format, ##__VA_ARGS__ ) 

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
ServerCCSTHeaderVisitor::ServerCCSTHeaderVisitor()
{
}
CCSTFile* ServerCCSTHeaderVisitor::visit(File *file)
{
  // #ifndef
  // #define
  // #endif
  // enum-specifier: enum id 
  std::vector<CCSTExDeclaration*> definitions; // = new std::vector<CCSTExDeclaration*>();
  // check if there are rpcs
  if(!file->rpc_defs()->empty())
    {
      printf("rpc not empty\n");
      definitions.push_back(construct_enum(file));
      // function callee function declarations
      std::vector<Rpc*>* rpcs = file->rpc_defs();
      for(std::vector<Rpc*>::iterator it = rpcs->begin(); it != rpcs->end(); it ++)
	{
	  definitions.push_back(construct_callee_declaration((Rpc*) *it));
	}
    }
  CCSTFile *c_file = new CCSTFile(definitions);
  return c_file;
}

CCSTExDeclaration* construct_callee_declaration(Rpc* r)
{
  std::vector<CCSTDecSpecifier*> *specifier = new std::vector<CCSTDecSpecifier*>();
  specifier->push_back(new CCSTSimpleTypeSpecifier(int_t));
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
  std::vector<CCSTInitDeclarator*> *init_declarator = new std::vector<CCSTInitDeclarator*>();
  init_declarator->push_back(declarator);
  CCSTDeclaration *func_declaration = new CCSTDeclaration(specifier, init_declarator);

  return func_declaration;
}

CCSTExDeclaration* construct_enum(File *f)
{
  const char* enum_name = "todo";
  CCSTEnumeratorList *el = construct_enumlist(f->rpc_defs());
  CCSTEnumSpecifier *e = new CCSTEnumSpecifier(enum_name, el);
  std::vector<CCSTDecSpecifier*> *tmp = new std::vector<CCSTDecSpecifier*>();
  tmp->push_back(e);
  CCSTDeclaration *declaration = new CCSTDeclaration(tmp, new std::vector<CCSTInitDeclarator*>());
  
  return declaration;
}

const char* construct_enum_name()
{
  return "todo";
}

CCSTEnumeratorList* construct_enumlist(std::vector<Rpc *>* rps)
{
  // list of functions to put in enum.
  std::vector<CCSTEnumerator*>* list = new std::vector<CCSTEnumerator*>();
  for(std::vector<Rpc*>::iterator it = rps->begin(); it != rps->end(); it ++)
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

char* string_to_upper(const char* str)
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

ServerCCSTSourceVisitor::ServerCCSTSourceVisitor()
{
}

CCSTFile* ServerCCSTSourceVisitor::visit(File *file)
{
  // <function-definition> is CCSTFuncDef
  // CCSTDecSpecifier* is <type-specifier> is CCSTTypeSpecifier
  // <declarator> is CCSTDeclarator
  // <declaration> what is this for is CCSTDeclaration
  // <compound-statement> is CCSTCompoundStatement
  // CCSTDeclaration for body
  // CCSTStatement for body

  // see notes in notebook
  
  std::vector<CCSTExDeclaration*> defs;
  CCSTFuncDef* exec_loop = create_exec_loop(file->rpc_defs());
  defs.push_back(exec_loop);
  return new CCSTFile(defs);
}

/* ------------------------ new ---------------------------- */



int count_nested_pointer(Type *p)
{
  if(p->num != 3)
    return 0;
  else
    {
      PointerType *tmp = dynamic_cast<PointerType*>(p);
      return 1 + count_nested_pointer(tmp->p_type());
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
  Assert(function_declaration->decs_->size() == 1, "Error: More than one initializer/declarator in function declaration");

  CCSTDeclarator *func = dynamic_cast<CCSTDeclarator*>(function_declaration->decs_->at(0));
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
  std::vector<CCSTDecSpecifier*> *specifier = new std::vector<CCSTDecSpecifier*>(); // return type
  specifier->push_back( ); // use a visitor to get return type);


  std::vector<CCSTInitDeclarator*> *func = new std::vector<CCSTInitDeclarator*>(); // pointer name, params
  int pointer_count = count_nested_pointer(r->return_type());

  CCSTPointer *p = create_pointer(pointer_count);
  
  CCSTDirectDecId *name = new CCSTDirectDecId(r->name());
  CCSTParamTypeList *param_list = create_parameter_list();
  CCSTDirectDecParamTypeList *name_params = new CCSTDirectDecParamTypeList(name, param_list);

  func->push_back(new CCSTDeclarator(p, name_params));

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

CCSTCompoundStatement* create_exec_loop_body()
{
  /*
  std::vector<CCSTDecSpecifier*> *s = new std::vector<CCSTDecSpecifier*>();
  std::vector<CCSTInitDeclarator*> *d = new std::vector<CCSTInitDeclarator*>();
  s->push_back(new CCSTSimpleTypeSpecifier(int_t));
  d->push_back(new CCSTDeclarator(null, new CCSTDirectDecId("ret")));
  std::vector<CCSTDeclaration*> decs_in_body;
  decs_in_body.push_back(new CCSTDeclaration(s,d));

  std::vector<CCSTStatement*> body_statements;

  // switch CCSTSelectionStatement
  // case is CCSTLabeledStatement

  std::vector<CCSTDeclaration*> empty; // not needed
  std::vector<CCSTStatement*> cases;
  for(std::vector<Rpc*>::iterator it = rps->begin(); it != rps->end(); it ++)
    {
      Rpc *r = *it;
      // this should be done elsewhere, the enum name, leave here for now, but move it
      char* upper_name = string_to_upper(r->name());
      char* enum_name = (char*)malloc((strlen(upper_name)+strlen("_CALLEE_T")+1)*sizeof(char));
      
      sprintf(enum_name, "%s_CALLEE_T", upper_name);

      char* callee_name = (char*)malloc( (strlen(r->name)+1+strlen("_callee"))*sizeof(char));
      sprintf(callee_name, "%s_callee", r->name);
      
      CCSTConstExpr* case_name = new CCSTConstExpr(new CCSTPrimaryExprId(enum_name));
      // for body of case
      std::vector<CCSTDeclaration*> case_declarations;
      std::vector<CCSTStatement*> case_statements;

      CCSTPrimaryExprId* r = new CCSTPrimaryExprId("ret");
      CCSTAssignOp* eq = new CCSTAssignOp(equal_t);
      // make call to whatever_callee()
      CCSTPostFixExprAssnExpr * func_call = new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId(callee_name), new CCSTAssignExpr(NULL));
      
      cases.push_back( new CCSTCaseStatement(case_name, new CCSTCompoundStatement(case_declarations, case_statements)));
    }

  CCSTSwitchStatement* dispatch = new CCSTSwitchStatement(new CCSTPrimaryExprId("ret"), new CCSTCompoundStatement(empty, ));

  
  CCSTCompoundStatement *body = new CCSTCompoundStatement(decs_in_body, body_statements);
*/
}


/* body for a callee function
 * does unmarshaling, real function call, etc
 */
CCSTCompoundStatement* create_callee_body(Rpc *r)
{
  // unmarshal parameters based on marshal data.
  // which says where params are stored.

  // 
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

CCSTDeclaration* container_struct_declaration()
{
  // needs to contain actual struct
  // needs to contain a capability
  // needs special asm code
 
  // what else
}
