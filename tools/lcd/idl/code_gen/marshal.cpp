#include "code_gen.h"

CCSTStatement* marshal_variable(Variable *v)
{
  // function call to set whatever register. 
  // access the variable.
  // if the variable is a dptr call
  // dptr_val

  const char* store_reg_func = store_register_mapping(v->marshal_info()->get_register());
  
  std::vector<CCSTAssignExpr*> store_reg_args;
  if(v->type()->name() == "dptr_t") {
    std::vector<CCSTAssignExpr*> dptr_val_args;
    dptr_val_args.push_back(access(v));
    store_reg_args.push_back(function_call("dptr_val", dptr_val_args));
  } else {
    store_reg_args.push_back(access(v));
  }

  return function_call(store_reg_func, store_reg_args);
}

CCSTStatement* marshal_projection_variable(Variable *v, ProjectionType *pt)
{
  std::vector<ProjectionField*> fields = pt->fields();
  // todo
}
