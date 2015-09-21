#include "ccst.h"

CCSTStatement* UnmarshalVariableVisitor::visit(Variable *param)
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
  
  return new CCSTAssignExpr(access_variable(param), new CCSTAssignOp(equal_t)
			    , new CCSTCastExpr(type_name, func_name));
}

CCSTPostFixExpr* access_variable(Variable *p)
{
  if(p->accessor() == 0x0) {
    return new CCSTPrimaryExprId(p->identifier());
  }
  
  return new CCSTPostFixExprAccess(access_parameter(p->accessor()), accessor, p->identifier()); 
}






