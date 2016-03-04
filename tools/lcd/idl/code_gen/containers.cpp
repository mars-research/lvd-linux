#include "ccst.h"
#include "code_gen.h"

/*
 * code related to container structures.
 */


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


/*
 * TODO 
 */
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
