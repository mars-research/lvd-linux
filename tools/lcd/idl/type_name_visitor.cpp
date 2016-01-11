#include "lcd_ast.h"

CCSTTypeName* TypeNameVisitor::visit(Typedef *td)
{
  CCSTPointer *pt = 0x0; 
  return td->type()->accept(this);
}

CCSTTypeName* TypeNameVisitor::visit(Channel *c)
{
  printf(" type name visitor channel todo!\n");
  return 0x0;
}

CCSTTypeName* TypeNameVisitor::visit(VoidType *vt)
{
  CCSTPointer *pt = 0x0;
  std::vector<CCSTSpecifierQual*> sq;
  
  sq.push_back(new CCSTSimpleTypeSpecifier(void_t));
  
  return new CCSTTypeName(sq, pt);
}

CCSTTypeName* TypeNameVisitor::visit(IntegerType *it)
{
  CCSTPointer *pt = 0x0;
  std::vector<CCSTSpecifierQual*> sq;
  
  switch(it->int_type()){
  case pt_char_t: {
    sq.push_back(new CCSTSimpleTypeSpecifier(char_t));
  }
  case pt_short_t: {
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

CCSTTypeName* TypeNameVisitor::visit(ProjectionType *proj)
{
  // make sure this is correct
  CCSTPointer *pt = 0x0;
  std::vector<CCSTSpecifierQual*> sq;
  sq.push_back(new CCSTStructUnionSpecifier(struct_t, proj->real_type()));
  
  return new CCSTTypeName(sq, pt);
}

CCSTTypeName* TypeNameVisitor::visit(Function *fp)
{
  printf("type name visitor function pointer todo\n");
  return 0x0;
}

