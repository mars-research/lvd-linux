#include "ccst.h"

CCSTStatement* MarshalVariableVisitor::visit(Variable *param)
{
  if(param->type() == 4) {
    Assert(0 == 1, "Error: variable to marshal cannot be of a complex type.\n");
  }
  
  // need register.
  int reg = param->marshal_info()->get_register();
  const char* func_name = access_register_mapping(reg);
  
  // register_0((long) param)
  std::vector<CCSTAssignExpr*> arguments;
  
  // type that we are castng to.
  std::vector<CCSTSpecifierQual*> spec_quals;
  spec_quals.push_back(new CCSTSimpleTypeSpecifier(long_t));
  spec_quals.push_back(new CCSTSimpleTypeSpecifier(long_t));
  
  arguments.push_back(new CCSTCastExpr(new CCSTTypeName(spec_quals, 0x0)
				       , access_variable(param))); // access here...
  
  //  a CCSTExpression/CCSTExprStatement
  return new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId(func_name), arguments);
}


