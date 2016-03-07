#include "code_gen.h"

/*
 * Want this code to be as dumb as possible.
 * The complexity for deciding what to unmarshal occurs previously.
 * Assume structure a variable belongs to has been previously allocated, at the obvious name.
 * This function should never be called on a variable whose type is a projection.
 */
CCSTStatement* unmarshal_variable(Variable *param)
{
  // set value to param, which has already been allocated or whatever.
  // for example: param = (type) get_reg0();
  
  int reg = param->marshal_info()->get_register();
  const char* func_name = store_register_mapping(reg);
  
  TypeNameVisitor *worker = new TypeNameVisitor();
  CCSTTypeName *type_name = param->type()->accept(worker);
  
  // access(param) may not return something that is an object. may need to deref
  return new CCSTAssignExpr(access(param), equals()
			    , new CCSTCastExpr(type_name, new CCSTPrimaryExprId(func_name)));
}




