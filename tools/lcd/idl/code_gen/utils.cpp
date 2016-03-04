#include "ccst.h"
#include "code_gen.h"

// for debugging purposes
char* type_number_to_name(int num)
{
  switch(num) {
  case 1:
    {
      return "typedef type";
    }
  case 2: 
    {
      return "integer type";
    }
  case 4:
    {
      return "projection type";
    }
  case 5:
    {
      return "void type";
    }
  case 6:
    {
      return "channel type";
    }
  case 7: 
    {
      return "function pointer type";
    }
  case 8:
    {
      return "unresolved type";
    }

  }
}

// =
CCSTAssignOp* equals() 
{
  return new CCSTAssignOp(equal_t);
}

// !
CCSTUnaryOp* Not()
{
  return new CCSTUnaryOp(unary_bang_t);
}

// &
CCSTUnaryOp* reference()
{
  return new CCSTUnaryOp(unary_bit_and_t);
}

CCSTUnaryOp* indirection()
{
  return new CCSTUnaryOp(unary_mult_t);
}

CCSTUnaryExprCastExpr* dereference(CCSTCastExpr *to_deref)
{
  return new CCSTUnaryExprCastExpr(indirection(), to_deref);
}

bool alloc_caller(Variable *v, const char *side)
{
  if(strcmp(side, "caller") == 0) {
    return v->alloc_caller();
  }
  
  return v->alloc_callee();
}

CCSTPrimaryExprId* function_name(const char *func_name)
{
  return new CCSTPrimaryExprId(func_name);
}

CCSTPostFixExprAssnExpr* function_call(const char *func_name, std::vector<CCSTAssignExpr*> args)
{
  return new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId(func_name), args);
}

/* creates a pointer, or pointer to pointer, etc*/
/* may need to be changed if want to do something like
 *    int * const name
 * int * volatile name
 */
CCSTPointer* pointer(int p_count)
{
  if(p_count == 0)
    return 0x0;
  
  if(p_count == 1)
    return new CCSTPointer();
  
  return new CCSTPointer(pointer(p_count - 1));
}
