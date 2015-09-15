#include "lcd_ast.h"

CCSTStatement* AllocateTypeVisitor::visit(Typedef *td, Variable *v)
{
  return allocation_helper(td->type(), v, 0);
}

CCSTStatement* AllocateTypeVisitor::visit(VoidType *vt, Variable *v)
{
  return allocation_helper(vt, v, 0);
}

CCSTStatement* AllocateTypeVisitor::visit(IntegerType *it, Variable *v)
{
  return allocation_helper(it, v, 0);
}

CCSTStatement* AllocateTypeVisitor::visit(PointerType *pt, Variable *v)
{
  // count pointers.
  // get inner type
  Type *t = first_non_pointer(pt);
  switch(t->num()) {
  case 1: { // typedef
    Typedef *td = dynamic_cast<Typedef*>(t);
    Assert(td != 0x0, "Error: dynamic cast failed!\n");
    return allocation_helper(td, v, count_pointers(pt));
  }
  case 2: { // integer type
    IntegerType *it = dynamic_cast<IntegerType*>(t);
    Assert(it != 0x0, "Error: dynamic cast failed!\n");
    return allocation_helper(it, v, count_pointers(pt));
  }
  case 3: {
    Assert(1 == 0, "Error: this should not happen!\n");
  }
  case 4: { // projection type
    ProjectionType *proj =  dynamic_cast<ProjectionType*>(t);
    Assert(proj != 0x0, "Error: dynamic cast failed!\n");
    return allocation_helper(pt, v, count_pointers(pt));
  }
  case 5: {
    VoidType *vt = dynamic_cast<VoidType*>(t);
    Assert(vt != 0x0, "Error: dynamic cast failed!\n");
    return allocation_helper(vt, v, count_pointers(pt));
  }
  default: {
    Assert(1 == 0, "Error: unknown type!\n");
  }
    
  }
}

CCSTStatement* AllocateTypeVisitor::visit(ProjectionType *pt, Variable *v)
{
  std::vector<CCSTAssignExpr*> malloc_args;

  TypeNameVisitor *worker = new TypeNameVisitor();
  malloc_args.push_back(new CCSTUnaryExprSizeOf(pt->accept(worker) ));

  new CCSTPostFixExprAssnExpr( new CCSTPrimaryExprId("malloc")
			       , malloc_args);

  std::vector<ProjectionField*> fields = pt->fields();
  
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    ProjectionField *pf = (ProjectionField*) *it;
    Type *t = pf->type();
    if(pointer_to_struct(t)) {
      this->visit(pf); // change to visit variables ? 
    }
  }
}

CCSTStatement* AllocateTypeVisitor::allocation_helper(ProjectionType *proj, Variable *v, int pointer_count)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  
  if(pointer_count == 0) { // case 1, not a pointer to a struct
    std::vector<CCSTAssignExpr*> malloc_args;
    
    TypeNameVisitor *worker = new TypeNameVisitor();
    
    malloc_args.push_back(new CCSTUnaryExprSizeOf(proj->accept(worker)));
    
    if(v->accessor() != 0x0) { // subcase, variable does not need to be declared.
      statements.push_back(new CCSTAssignExpr(access_variable(v)
					      , new CCSTAssignOP(equal_t)
					      , new CCSTUnaryExprCastExpr(new CCSTUnaryOp(unary_mult_t)
									  ,new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId("malloc")
												       ,malloc_args))));
    } else { // variable needs to be declared
      
      std::vector<CCSTDecSpecifier*>specifier;
      specifier.push_back( new CCSTStructUnionSpecifier(struct_t, proj->real_type()));
      std::vector<CCSTInitDeclarator*> decs;
      decs.push_back(new CCSTInitDeclarator(new CCSTDeclarator(0x0, new CCSTDirectDecId(v->identifier()))
					    ,new CCSTInitializer(new CCSTUnaryExprCastExpr( new CCSTUnaryOp(unary_mult_t)
											    ,new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId("malloc")
															 ,malloc_args)))));
      
      declarations.push_back(new CCSTDeclaration(specifier, decs));
    }
    
  } else if(pointer_count == 1) { // pointer to projection
    std::vector<CCSTAssignExpr*> malloc_args;
    
    TypeNameVisitor *worker = new TypeNameVisitor();
    malloc_args.push_back(new CCSTUnaryExprSizeOf(proj->accept(worker)));
    
    if(v->accessor() != 0x0) { // variable DOES NOT need to be declared
      
      statements.push_back(new CCSTAssignExpr(access_variable(v)
					      , new CCSTAssignOP(equal_t)
					      ,new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId("malloc")
									   ,malloc_args)));
    
    } else { // variable DOES need to be declared
      
      std::vector<CCSTDecSpecifier*>specifier;
      specifier.push_back( new CCSTStructUnionSpecifier(struct_t, proj->real_type()));
      std::vector<CCSTInitDeclarator*> decs;
      decs.push_back(new CCSTInitDeclarator(new CCSTDeclarator(new CCSTPointer()
							       , new CCSTDirectDecId(v->identifier()))
					    ,new CCSTInitializer(new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId("malloc")
											     ,malloc_args))));
      declarations.push_back(new CCSTDeclaration(specifier, decs));
    }
  } else { // pointer count is greater than 1.
    // need a temp variable.
    SymbolTable *st = v->scope()->symbol_table();
    const char* tmp_id = st->unique_tmp();
    
    std::vector<CCSTAssignExpr*> struct_malloc_args;
    TypeNameVisitor *worker = new TypeNameVisitor();
    struct_malloc_args.push_back(new CCSTUnaryExprSizeOf(pt->accept(worker) ));
    
    std::vector<CCSTDecSpecifier*>specifier;
    specifier.push_back( new CCSTStructUnionSpecifier(struct_t, proj->real_type()));
    std::vector<CCSTInitDeclarator*> decs;
    decs.push_back(new CCSTInitDeclarator(new CCSTDeclarator(new CCSTPointer()
							     , new CCSTDirectDecId(tmp_id))
					  ,new CCSTInitializer(new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId("malloc")
											   ,struct_malloc_args))));
    declarations.push_back(new CCSTDeclaration(specifier, decs));

    for(int i = 0; i < pointer_count-2; i ++) {
      Assert(1 == 0, "Error: 3 pointers not supported at the moment.\n");
    }
    
    std::vector<CCSTAssignExpr*> pointer_malloc_args;
    pointer_malloc_args.push_back(new CCSTUnaryExprSizeOf(worker->helper(proj, new CCSTPointer())));

    if(v->accessor() != 0x0) { // variable DOES NOT need to be declared
    } else { // variable NEEDS to be declared 
      std::vector<CCSTDecSpecifier*> var_spec;
      specifier.push_back( new CCSTStructUnionSpecifier(struct_t, proj->real_type()));
      std::vector<CCSTInitDeclarator*> var_decs;
      decs.push_back(new CCSTInitDeclarator(new CCSTDeclarator(new CCSTPointer( new CCSTPointer())
							       , new CCSTDirectDecId(v->identifier()))
					    ,new CCSTInitializer(new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId("malloc")
											     ,pointer_malloc_args))));
      declarations.push_back(new CCSTDeclaration(var_spec, var_decs));
    }

    statements.push_back(new CCSTAssignExpr(new CCSTUnaryExprCastExpr(new CCSTUnaryOp(unary_mult_t)
								      , access_variable(v))
					    , new CCSTAssignOP(equal_t)
					    , new CCSTPrimaryExprId(tmp_id)));
  }

  // have allocated space for struct.  need to allocate space for fields if necessary
  std::vector<ProjectionField*> fields = proj->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    ProjectionField *pf = (ProjectionField*) *it;
    if(pointer_to_struct(pf->type())) {
      statements.push_back(pf->accept(this)):
    }  
  }
  
  return new CCSTCompoundStatement(declarations, statements);
}

CCSTStatement* AllocateTypeVisitor::allocation_helper(Typedef *td, Variable *v, int pointer_count)
{
  Assert(1 == 0, "Not implemented.\n");
}

CCSTStatement* AllocateTypeVisitor::allocation_helper(VoidType *vt, Variable *v, int pointer_count)
{
  Assert(1 == 0, "Not implemented.\n");
}

CCSTStatement* AllocateTypeVisitor::allocation_helper(IntegerType *it, Variable *v, int pointer_count)
{
  Assert(1 == 0, "Not implemented.\n");
}

bool AllocateTypeVisitor::pointer_to_struct(Type *t)
{
  while(t->num() == 3) {
    PointerType *pt = dynamic_cast<PointerType*>(t);
    t = pt->type();
    if(t->num() == 4) {
      return true;
    }
  }
  return false;
}

int AllocateTypeVisitor::count_pointers(PointerType *pt)
{
  if(pt->type()->num != 3)
    return 1;
  
  PointerType *p = dynamic_cast<PointerType*>(pt->type());
  Assert(p != 0x0, "Error: dynamic cast failed!\n");
  
  return count_pointers(p) + 1;
}

Type* AllocateTypeVisitor::first_non_pointer(PointerType *pt)
{
  if(pt->type()->num != 3)
    return pt->type();

  PointerType *p = dynamic_cast<PointerType*>(pt->type());
  Assert(p != 0x0, "Error: dynamic cast failed!\n");
  
  return first_non_pointer(p);
}
