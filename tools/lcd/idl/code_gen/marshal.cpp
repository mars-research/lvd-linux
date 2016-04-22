#include "code_gen.h"

/*
 * Want this code to be as dumb as possible.  
 * The complexity for deciding what to marshal occurs previously.
 * when marshal params list is populated.
 * This function should never be called on a variable whose type is a projection
 */
CCSTStatement* marshal_variable(Variable *v, const char* direction)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  
  if (v->container() != 0x0) {
    // marshal container
    printf("trying to marshal container %s\n", v->container()->identifier());
    statements.push_back(marshal_variable(v->container(), direction));
  }
  
  if (v->type()->num() == 4 || v->type()->num() == 9) { // projection
    // loop through fields
    ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
    Assert(pt != 0x0, "Error: dynamic cast to projection failed!\n");
    
    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      if( (strcmp("in", direction) == 0 && pf->in()) || (strcmp("out", direction) == 0 && pf->out())) {
	
	statements.push_back(marshal_variable(pf, direction));
      }
    }
    
  } else {
    printf("marshalling variable %s\n", v->identifier());
    Assert(v->marshal_info() != 0x0, "Error: marshalling info is null\n");
    statements.push_back( marshal(access(v), v->marshal_info()->get_register()));
  }
  
  return new CCSTCompoundStatement(declarations, statements);
}

std::vector<CCSTStatement*> marshal_variable_callee(Variable *v)
{
  std::vector<CCSTStatement*> statements;

  if(v->container() != 0x0) {
    if(v->container()->out()) {
      std::vector<CCSTStatement*> tmp_stmts = marshal_variable_callee(v->container());
      statements.insert(statements.end(), tmp_stmts.begin(), tmp_stmts.end());
    }
  }

  if (v->type()->num() == 4 || v->type()->num() == 9) { // projection
    ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
    Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");

    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      if(pf->out()) {
	std::vector<CCSTStatement*> tmp_stmt = marshal_variable_callee(pf);
	statements.insert(statements.end(), tmp_stmt.begin(), tmp_stmt.end());
      }
    }
    
  } else {
    Assert(v->marshal_info() != 0x0, "Error: marshal info is null\n");
    statements.push_back(marshal(access(v), v->marshal_info()->get_register()));
  }
  
  return statements;
}

std::vector<CCSTStatement*> marshal_variable_no_check(Variable *v)
{
  std::vector<CCSTStatement*> statements;

  if(v->container() != 0x0) {
    std::vector<CCSTStatement*> tmp_stmts = marshal_variable_callee(v->container());
    statements.insert(statements.end(), tmp_stmts.begin(), tmp_stmts.end());
  }

  if (v->type()->num() == 4 || v->type()->num() == 9) { // projection
    ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
    Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");

    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      std::vector<CCSTStatement*> tmp_stmt = marshal_variable_callee(pf);
      statements.insert(statements.end(), tmp_stmt.begin(), tmp_stmt.end());
    }
    
  } else {
    Assert(v->marshal_info() != 0x0, "Error: marshal info is null\n");
    statements.push_back(marshal(access(v), v->marshal_info()->get_register()));
  }
  
  return statements;
}

CCSTStatement* marshal(CCSTPostFixExpr *v, int reg)
{
  const char* store_reg_func = store_register_mapping(reg);
  
  std::vector<CCSTAssignExpr*> store_reg_args;
  store_reg_args.push_back(v);
  
  return new CCSTExprStatement( function_call(store_reg_func, store_reg_args));
}
