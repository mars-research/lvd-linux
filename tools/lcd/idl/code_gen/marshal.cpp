#include "code_gen.h"

/*
 * Want this code to be as dumb as possible.  
 * The complexity for deciding what to marshal occurs previously.
 * when marshal params list is populated.
 * This function should never be called on a variable whose type is a projection
 */
CCSTStatement* marshal_variable(Variable *v)
{
  // function call to set whatever register. 
  // access the variable.

  const char* store_reg_func = store_register_mapping(v->marshal_info()->get_register());
  
  std::vector<CCSTAssignExpr*> store_reg_args;
  
  store_reg_args.push_back(access(v));
  
  return function_call(store_reg_func, store_reg_args);
}
