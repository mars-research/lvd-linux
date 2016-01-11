#include "code_gen.h"

CCSTStatement* marshal_variable(Variable *param)
{
  // todo
}

CCSTStatement* MarshalTypeVisitor::visit(Channel *c, Variable *v)
{
  printf("Marshal type visitor channel todo\n");
  return 0x0;
}

CCSTStatement* MarshalTypeVisitor::visit(Function *fp, Variable *v)
{
  printf("Marshal function pointer not completed\n");
  return 0x0;
}


CCSTStatement* MarshalTypeVisitor::visit(Typedef *td, Variable *v)
{
  // get register number
  Marshal_type* mt = v->marshal_info();
  
  CCSTPostFixExpr* access_v = access(v);
  
  std::vector<CCSTAssignExpr*> arguments;
  
  if (v->pointer_count() > 0) {
    arguments.push_back(dereference(access_v));
  } else {
    arguments.push_back(access_v);
  }
  return function_call( function_name( store_register_mapping(mt->get_register()))
			, arguments);
}

CCSTStatement* MarshalTypeVisitor::visit(VoidType *vt, Variable *v)
{
  printf("Error: cannot marshal void type\n");
  return 0x0;
}

CCSTStatement* MarshalTypeVisitor::visit(IntegerType *it, Variable *v)
{
  // get register number
  Marshal_type* mt = v->marshal_info();
 
  CCSTPostFixExpr* access_v = access(v);

  std::vector<CCSTAssignExpr*> arguments;

  if (v->pointer_count() > 0) {
    arguments.push_back(dereference(access_v));
  } else {
    arguments.push_back(access_v);
  }
  return function_call( function_name( store_register_mapping(mt->get_register()))
		 , arguments);
}

CCSTStatement* MarshalTypeVisitor::visit(ProjectionType *pt, Variable *v)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;

  // if alloc caller

  // if alloc callee

  // marshal fields
  std::vector<ProjectionField*> fields = pt->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    // marshal each field.
    
    ProjectionField *pf_tmp = *it;
    statements.push_back(pf_tmp->type()->accept(this, pf_tmp));

  }

  // a channel???????
  
}
