#include "lcd_ast.h"
#include "code_gen.h"

CCSTStatement* AllocateTypeVisitor::visit(Channel *c, Variable *v)
{
  Assert(1 == 0, "Error: allocation for channel not implemented\n");
}

CCSTStatement* AllocateTypeVisitor::visit(UnresolvedType *ut, Variable *v)
{
  Assert(1 == 0, "Error: cannot allocate an unresolved type\n");
}

CCSTStatement* AllocateTypeVisitor::visit(Function *fp, Variable *v)
{
  Assert(1 == 0, "Error: allocation for function pointer not implemented\n");
}

CCSTStatement* AllocateTypeVisitor::visit(Typedef *td, Variable *v)
{
  return td->type()->accept(this, v);
}

CCSTStatement* AllocateTypeVisitor::visit(VoidType *vt, Variable *v)
{
  Assert(1 == 0, "Error: allocation for void not implemented\n");
}

/* Assumes something with the name v->identifier(), has already been declared */
CCSTStatement* AllocateTypeVisitor::visit(IntegerType *it, Variable *v)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;

  std::vector<CCSTSpecifierQual*> void_star_quals;
  void_star_quals.push_back(new CCSTSimpleTypeSpecifier(void_t));

  CCSTTypeName *void_star = new CCSTTypeName(void_star_quals, new CCSTPointer());

  std::vector<CCSTSpecifierQual*> int_type_quals = type(it);
  CCSTTypeName *int_type = new CCSTTypeName(int_type_quals, 0x0);
  
  int p_count_save = v->pointer_count();
  int p_count = v->pointer_count();

  while(p_count > 1) {
    std::vector<CCSTAssignExpr*> kzalloc_args;
    kzalloc_args.push_back(new CCSTUnaryExprSizeOf(void_star));
    kzalloc_args.push_back(new CCSTEnumConst("GFP_KERNEL"));
    
    v->set_pointer_count(p_count_save-p_count);
    statements.push_back(new CCSTAssignExpr(access(v), equals(), function_call("kzalloc", kzalloc_args)));
    p_count -= 1;
  }
  v->set_pointer_count(p_count_save - p_count);
  std::vector<CCSTAssignExpr*> kzalloc_args;

  /* do sizeof(*access(v))  */
  kzalloc_args.push_back(new CCSTUnaryExprSizeOf(int_type));
  kzalloc_args.push_back(new CCSTEnumConst("GFP_KERNEL"));

  statements.push_back(new CCSTAssignExpr(access(v), equals(), function_call("kzalloc", kzalloc_args)));

  v->set_pointer_count(p_count_save); // set back to what it was before
  return new CCSTCompoundStatement(declarations, statements);
}

/* Assume variable at name v->identifier() has already been declared */
CCSTStatement* AllocateTypeVisitor::visit(ProjectionType *pt, Variable *v)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;

  std::vector<CCSTSpecifierQual*> void_star_quals;
  void_star_quals.push_back(new CCSTSimpleTypeSpecifier(void_t));
  CCSTTypeName *void_star = new CCSTTypeName(void_star_quals, new CCSTPointer());

  int p_count_save = v->pointer_count();
  int p_count = v->pointer_count();

  while(p_count > 1) {
    std::vector<CCSTAssignExpr*> kzalloc_args;
    kzalloc_args.push_back(new CCSTUnaryExprSizeOf(void_star));
    kzalloc_args.push_back(new CCSTEnumConst("GFP_KERNEL"));
    
    v->set_pointer_count(p_count_save-p_count);
    statements.push_back(new CCSTAssignExpr(access(v), equals(), function_call("kzalloc", kzalloc_args)));
    p_count -= 1; 
  }
  
  v->set_pointer_count(p_count_save);
  /* allocate the actual structure now */
  std::vector<CCSTAssignExpr*> kzalloc_args;

  /* do sizeof(struct thing) */
  kzalloc_args.push_back(new CCSTUnaryExprSizeOf(access(v)));
  kzalloc_args.push_back(new CCSTEnumConst("GFP_KERNEL"));

  v->set_pointer_count(p_count_save-p_count);
  statements.push_back(new CCSTAssignExpr(access(v), equals(), function_call("kzalloc", kzalloc_args)));
  
  v->set_pointer_count(p_count_save);

  /* Now need to allocate fields */
  std::vector<ProjectionField*> fields = pt->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    ProjectionField *pf = *it;

    if (pf->pointer_count() > 0) {
      statements.push_back(pf->type()->accept(this, pf));
    }
  }

  return new CCSTCompoundStatement(declarations, statements);
}
