#include "code_gen.h"

CCSTStatement* unmarshal_variable(Variable *param)
{
  // get register
  // pull value out of register.
  // ASSUME THE STRUCTURE A VARIABLE BELONGS TO HAS ALREADY BEEN ALLOCATED OR GRABBED.
  
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

// add a visitor for each type because projections will need extra.





