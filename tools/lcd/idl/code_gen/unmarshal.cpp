#include "code_gen.h"

/*
 * Want this code to be as dumb as possible.
 * The complexity for deciding what to unmarshal occurs previously.
 * Assume structure a variable belongs to has been previously allocated, at the obvious name.
 * This function should never be called on a variable whose type is a projection.
 */

std::vector<CCSTStatement*> unmarshal_variable_no_check(Variable *v)
{
  std::vector<CCSTStatement*> statements;
  
  if (v->type()->num() == 4 || v->type()->num() == 9) {
    // loop through fields
    ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
    Assert(pt != 0x0, "Error: dynamic cast to projection failed!\n");

    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      std::vector<CCSTStatement*> tmp_stmts = unmarshal_variable_no_check(pf);
      statements.insert(statements.end(), tmp_stmts.begin(), tmp_stmts.end());
    }

  } else {
    
    Assert(v->marshal_info() != 0x0, "Error: no marshal info\n");
    int reg = v->marshal_info()->get_register();
    const char* func_name = access_register_mapping(reg);
    TypeNameVisitor *worker = new TypeNameVisitor();
    CCSTTypeName *type_name = v->type()->accept(worker);
    
    std::vector<CCSTAssignExpr*> access_reg_args_empty;
    
    statements.push_back(new CCSTExprStatement( new CCSTAssignExpr (access(v)
								    , equals()
								    , function_call(func_name, access_reg_args_empty))));
  }

  return statements;
}

CCSTPostFixExprAssnExpr* unmarshal_variable(Variable *v)
{
  Assert(v->marshal_info() != 0x0, "Error: no marshal info\n");
  int reg = v->marshal_info()->get_register();
  const char* func_name = access_register_mapping(reg);
  TypeNameVisitor *worker = new TypeNameVisitor();
  CCSTTypeName *type_name = v->type()->accept(worker);
  std::vector<CCSTAssignExpr*> access_reg_args_empty;
  return function_call(func_name, access_reg_args_empty);
}

// unmarshals variable. 
std::vector<CCSTStatement*> unmarshal_variable_caller(Variable *v)
{
  std::vector<CCSTStatement*> statements;

  if(v->type()->num() == 4 || v->type()->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
    Assert(pt != 0x0, "Error: dynamic cast to projection failed!\n");
    
    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      if(pf->out()) {
	std::vector<CCSTStatement*> tmp_stmt = unmarshal_variable_caller(pf);
	statements.insert(statements.end(), tmp_stmt.begin(), tmp_stmt.end());
      }
    }
    
  } else {
    Assert(v->marshal_info() != 0x0, "Error: variable has no marshalling information\n");
    
    int reg = v->marshal_info()->get_register();
    const char* reg_func = access_register_mapping(reg);
    std::vector<CCSTAssignExpr*> reg_func_args_empty;
    
    statements.push_back(new CCSTExprStatement(new CCSTAssignExpr(access(v)
								  , equals()
								  , function_call(reg_func
										  , reg_func_args_empty))));
  }
  
  return statements;
}


// unmarshals variable. and sets in container if there is a container.
std::vector<CCSTStatement*> unmarshal_variable_callee(Variable *v)
{
  std::vector<CCSTStatement*> statements;

  if(v->type()->num() == 4 || v->type()->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
    
    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      if(pf->in()) {
	std::vector<CCSTStatement*> tmp_stmt = unmarshal_variable_callee(pf);
	statements.insert(statements.end(), tmp_stmt.begin(), tmp_stmt.end());
      }
    }
    
  } else {
    Assert(v->marshal_info() != 0x0, "Error: variable has no marshalling information\n");
    
    int reg = v->marshal_info()->get_register();
    const char* reg_func = access_register_mapping(reg);
    std::vector<CCSTAssignExpr*> reg_func_args_empty;

    if (v->container() != 0x0) {
      ProjectionType *v_container_type = dynamic_cast<ProjectionType*>(v->container()->type());
      Assert(v_container_type != 0x0, "Error: dynamic cast to projection type failed\n");
      ProjectionField *v_container_real_field = v_container_type->get_field(v->type()->name());
      Assert(v_container_real_field != 0x0, "Error: could not find field in structure\n");
      

      statements.push_back(new CCSTExprStatement(new CCSTAssignExpr(access(v_container_real_field)
								    , equals()
								    , function_call(reg_func
										    , reg_func_args_empty))));
    } else {
      // no container
      statements.push_back(new CCSTExprStatement(new CCSTAssignExpr(access(v)
								    , equals()
								    , function_call(reg_func
										    , reg_func_args_empty))));
    }
  }
  return statements;

}

std::vector<CCSTStatement*> unmarshal_container_refs_caller(Variable *v)
{
  std::vector<CCSTStatement*> statements;

  if(v->container() != 0x0) {
    if(v->alloc_callee()) { // except a remote ref
      statements.push_back(set_remote_ref(v));
    }
  }
  
  if( v->type()->num() == 4 || v->type()->num() == 9) { 
    ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
    Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
    
    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      if(pf->out()) {
	std::vector<CCSTStatement*> tmp_stmts = unmarshal_container_refs_caller(pf);
	statements.insert(statements.end(), tmp_stmts.begin(), tmp_stmts.end());
      }
    }
  }
  
  return statements;
}
