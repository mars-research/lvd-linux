#include "ccst.h"
#include "code_gen.h"

/*
 * code related to container structures.
 */

// declares and initializes an instance of the container struct for the struct
// that this variable is a type of. uses the container structs name as the name of the variable
CCSTCompoundStatement* declare_and_initialize_container_struct(Variable *v, ProjectionType *pt, LexicalScope *ls, const char *side)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;

  // if we are on callee side, and it is alloc callee, we call alloc
  // otherwise we do container_of

  if(alloc_caller(v, side) || alloc_callee(v, side)) {
    statements.push_back(alloc_init_containers(v, pt, ls, side));

  } else { // container of
    // declare container
    const char* container_name_ = container_name(pt->name());
    declarations.push_back(struct_pointer_declaration(container_name_, container_name_, ls));
    
    statements.push_back(new CCSTAssignExpr(new CCSTPrimaryExprId(container_name_), equals()
					    , function_call("container_of", container_of_args( access(v), struct_name(pt), pt->real_type()))));
  }

  int err;
  const char* container_name_ = container_name(pt->name());
  Assert(ls->lookup(container_name_, &err) != 0x0, "Error: could not find type in scope\n");
  ProjectionType *container = dynamic_cast<ProjectionType*>(ls->lookup(container_name_, &err));
  Assert(container != 0x0, "Error: dynamic cast to projection failed\n");
  
  // loop through this projection's fields
  Parameter *tmp = new Parameter(container, container_name_, 1); // to access from.
  std::vector<ProjectionField*> fields = pt->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    ProjectionField *pf = *it;
    if(pf->type()->num() == 4) {
      ProjectionType *pt_tmp = dynamic_cast<ProjectionType*>(pf->type());
      Assert(pt_tmp != 0x0, "Error: dynamic cast to Projection type failed!\n");
      
      pf->set_accessor(v);
      statements.push_back(declare_and_initialize_container_struct(pf, pt_tmp, ls, side));
      
      // link/init these fields
      Variable *sv = pf->accessor()->accessor();
      pf->accessor()->set_accessor(tmp);
      
      const char* container_name_2 = container_name(pt_tmp->name());
      
      Assert(ls->lookup(container_name_2, &err) != 0x0, "Error: could not find container in environment.\n");
      ProjectionType *pt_container = dynamic_cast<ProjectionType*>(ls->lookup(container_name_2, &err));
      Assert(pt_container != 0x0, "Error: dynamic cast to Projection type failed!\n");
      
      ProjectionField *pf_tmp = pt_container->get_field(pt_tmp->name());
      Parameter *tmp_param = new Parameter(pt_container, container_name_2, 1);
      
      pf_tmp->set_accessor(tmp_param);
      statements.push_back(new CCSTAssignExpr(access(pf), equals(), new CCSTUnaryExprCastExpr(reference(), access(pf_tmp))));
      
    }
  }
  
  return new CCSTCompoundStatement(declarations, statements);
}


std::vector<CCSTAssignExpr*> container_of_args(CCSTPostFixExpr *struct_pointer, const char* type_name, const char* field_name)
{
  std::vector<CCSTAssignExpr*> args;
  args.push_back( struct_pointer);
  args.push_back( new CCSTPrimaryExprId(type_name));
  args.push_back( new CCSTPrimaryExprId(field_name));
  return args;
}

CCSTCompoundStatement* alloc_init_containers(Variable *v, ProjectionType *pt, LexicalScope *ls, const char *side) 
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;

  // declare container
  const char* container_name_ = container_name(pt->name());
  declarations.push_back(struct_pointer_declaration(container_name_, container_name_, ls));

  int err;
  Assert(ls->lookup(container_name_, &err) != 0x0, "Error: could not find type in scope\n");
  ProjectionType *container = dynamic_cast<ProjectionType*>(ls->lookup(container_name_, &err));
  Assert(container != 0x0, "Error: dynamic cast to projection failed\n");
  
  // alloc
  statements.push_back(kzalloc_structure(container_name_, container_name_));

  // error check
  statements.push_back(if_cond_fail(new CCSTUnaryExprCastExpr(Not(), new CCSTPrimaryExprId(container_name_))
				    , "kzalloc"));
  // insert into dstore
  // do error checking
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
