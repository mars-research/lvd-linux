#include "ccst.h"
#include "code_gen.h"

/*
 * Code related to function pointers and the trampoline code generated for them.
 * 
 */


/*
 * trampoline function declaration.
 */
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

/*
 * generates the body of an rpc which is a function pointer
 * although this function does not verify that.
 * 1. declares a new volatile function pointer
 * 2. declares an instance of the relevant function pointers hidden args structure
 * 3. calls LCD_TRAMPOLINE_LINKAGE macro
 * 4. sets function pointer from step (1) equal to glue code for function pointer
 * 5. returns call to function pointer in step (1)
 */
CCSTCompoundStatement* trampoline_function_body(Rpc* r)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  
  /* start new function pointer declaration */

  std::vector<CCSTDecSpecifier*> new_fp_return_type = type2(r->return_variable()->type());

  /* loop through rpc parameters and add them to the parameters for the new fp*/
  std::vector<CCSTParamDeclaration*> func_pointer_params;

  std::vector<Parameter*> parameters = r->parameters();
  for(std::vector<Parameter*>::iterator it = parameters.begin(); it != parameters.end(); it ++) {
    Parameter *p = *it;
    
    std::vector<CCSTDecSpecifier*> fp_param_tmp = type2(p->type());
    func_pointer_params.push_back(new CCSTParamDeclaration(fp_param_tmp
							   , new CCSTDeclarator(pointer(p->pointer_count()), new CCSTDirectDecId(""))));
  }

  /* declare new function pointer */
  std::vector<type_qualifier> pointer_type_qualifier;
  pointer_type_qualifier.push_back(volatile_t);

  std::vector<CCSTInitDeclarator*> new_fp_declaration;
  new_fp_declaration.push_back(new CCSTDeclarator(0x0
						  , new CCSTDirectDecParamTypeList(new CCSTDirectDecDec(new CCSTDeclarator(new CCSTPointer(pointer_type_qualifier)
															   , new CCSTDirectDecId("new_fp"))) // todo
										   , new CCSTParamList(func_pointer_params))));

  declarations.push_back(new CCSTDeclaration(new_fp_return_type, new_fp_declaration));
  
  /* end new_fp declaration */

  /* declare a hidden args instance */
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

  int err;
  ProjectionType *hidden_args_param = dynamic_cast<ProjectionType*>(r->current_scope()->lookup(hidden_args_name(r->name()), &err));

  // create a tmp variable to do access.
  Parameter *tmp_hidden_args = new Parameter(hidden_args_param, "hidden_args", 1);

  Assert(hidden_args_param != 0x0, "Error: dynamic cast to Projection type failed!\n");
  ProjectionField *container_field = hidden_args_param->get_field("container");
  if(container_field != 0x0) {
    Variable *save_accessor = container_field->accessor();
    container_field->set_accessor(tmp_hidden_args);
    new_fp_args.push_back(access(container_field));
    container_field->set_accessor(save_accessor);
  }

  ProjectionField *dstore_field = hidden_args_param->get_field("dstore");
  if(dstore_field != 0x0) {
    Variable *save_accessor = dstore_field->accessor();
    dstore_field->set_accessor(tmp_hidden_args);
    new_fp_args.push_back(access(dstore_field));
    dstore_field->set_accessor(save_accessor);
  }

  statements.push_back(new CCSTReturn(function_call("new_fp", new_fp_args)));

  return new CCSTCompoundStatement(declarations, statements);
}


/*
 * 
 */
CCSTCompoundStatement* alloc_init_hidden_args_struct(Variable *v, ProjectionType *pt, LexicalScope *ls)
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
      statements.push_back(alloc_init_hidden_args_struct(pf, pt, ls));
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
      ProjectionField *c_tmp = trampoline->get_field("container"); // todo
      Assert(c_tmp != 0x0, "Error: could not find field in projection\n");
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
