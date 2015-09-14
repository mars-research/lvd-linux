#include "lcd_ast.h"

CCSTTypeName* TypeNameVisitor::visit(Typedef *td)
{
  return helper(td, 0x0);
}

CCSTTypeName* TypeNameVisitor::visit(VoidType *vt)
{
  return helper(vt, 0x0); 
}

CCSTTypeName* TypeNameVisitor::visit(IntegerType *it)
{
  return helper(it, 0x0);
}

CCSTTypeName* TypeNameVisitor::visit(PointerType *pt)
{
  return helper(get_non_pointer(pt), pointer_helper(pt));
}

CCSTTypeName* TypeNameVisitor::visit(ProjectionType *pt)
{
  return helper(pt, 0x0);
}

CCSTTypeName* helper(IntegerType *it, CCSTPointer *pt)
{
  std::vector<CCSTSpecifierQual*> sq;
  
  switch(it->int_type()){
  case pt_char_t: {
    sq.push_back(new CCSTSimpleTypeSpecifier(char_t));
  }
  case pt_short_: {
    sq.push_back(new CCSTSimpleTypeSpecifier(short_t));
  }
  case pt_int_t: {
    sq.push_back(new CCSTSimpleTypeSpecifier(int_t));
  }
  case pt_long_t: {
    sq.push_back(new CCSTSimpleTypeSpecifier(long_t));
  }
  case pt_longlong_t: {
    sq.push_back(new CCSTSimpleTypeSpecifier(long_t));
    sq.push_back(new CCSTSimpleTypeSpecifier(long_t));
  }
  case pt_capability_t: {
    sq.push_back(new CCSTSimpleTypeSpecifier(long_t));
  }
  default: {
    Assert(1 == 0, "Error: unknown integer type.\n");
  } 
  }
  return new CCSTTypeName(sq, pt);
}

CCSTTypeName* helper(ProjectionType *proj, CCSTPointer *pt)
{
  std::vector<CCSTSpecifierQual*> sq;
  sq.push_back(new CCSTSTructUnionSpecifier(struct_t, proj->real_type()));
  
  return new CCSTTypeName(sq, pt);
}

CCSTTypeName* helper(VoidType *vt, CCSTPointer *pt)
{
  std::vector<CCSTSpecifierQual*> sq;
  
  sq.push_back(new CCSTSimpleTypeSpecifier(void_t));
  
  return new CCSTypeName(sq, pt);
}

// for now real type.
CCSTTypeName* helper(Typedef *td, CCSTPointer *pt)
{
  return helper(td->type(), pt);
}

CCSTPointer* pointer_helper(PointerType *pt)
{
  if(pt->type() != 3) {
    return new CCSTPointer();
  }
  
  PointerType *pt2 = dynamic_cast<PointerType*>(pt->type());
  Assert(pt2 != null, "Error: dynamic cast failed!\n");
  
  return new CCSTPointer(pointer_helper(pt2));
}

 Type* get_non_pointer(PointerType *pt)
 {
   if(pt->type() != 3) {
     return pt->type();
   }
   
   PointerType *pt2 = dynamic_cast<PointerType*>(pt->type());
   Assert(pt2 != null, "Error: dynamic cast failed!\n");
   
   return get_non_pointer(pt2);
 }
