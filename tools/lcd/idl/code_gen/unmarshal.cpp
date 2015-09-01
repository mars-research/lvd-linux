#include "ccst.h"

CCSTCompoundStatement* unmarshal_parameter(Parameter *p)
{
  // create visitor
  UnmarshalTypeVisitor *visitor = new UnmarshalTypeVisitor();
  return p->get_marshal_info()->accept(visitor);
}

CCSTCompoundStatement* UnmarshalTypeVisitor::visit(Marshal_projection *data, const char *name)
{
  // loop through fields.
  for(std::vector<ProjectionField*>::iterator it = pt->fields().begin(); pt->fields().end(); it ++)
    {
      ProjectionField* pf = *it;
      
    }
}

/*
// This code will produce the CAST that unmarshals the params. instead of where it was before
CCSTCompoundStatement* UnmarshalTypeVisitor::visit(Marshal_projection *data, const char *name)
{
  ProjectionType *pt = dynamic_cast<ProjectionType*>(data->get_type());
  Assert(pt != 0x0, "Error: Expected ProjectionType got something else.\n");

  for(std::vector<ProjectionField*>::iterator it = pt->fields().begin(); pt->fields().end(); it ++)
    {
      
    }

  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;

  std::vector<CCSTInitDeclarator*> decs;

  std::vector<CCSTSpecifierQual*> squal;
  squal.push_back(new CCSTStructUnionSpecifier(struct_t, pt->real_type()));
  
  CCSTTypeName *type_cast = new CCSTTypeName(squal, new CCSTPointer());

  std::vector<CCSTAssignExpr*> args;
  args.push_back( new CCSTUnaryExprSizeOf( new CCSTTypeName(squal, 0x0)));

  CCSTCastExpr *malloc_call =  new CCSTCastExpr(type_cast
					  , new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId("malloc")
								       , args) );
  
  decs.push_back(new CCSTInitDeclarator(new CCSTDeclarator(0x0
							   , new CCSTDirectDecId(data->get_name()))
					, new CCSTInitializer(new CCSTUnaryExprCastExpr(new CCSTUnaryOp(unary_mult_t) , new CCSTCastExpr(type_cast													 , malloc_call )))));
  
  declarations.push_back(new CCSTDeclaration(type(pt), decs));

  // have malloced space for struct

  const char* param_name = data->get_name();
  
  std::vector<Marshal_type*> fields = data->fields(); // all of the out fields
  for(std::vector<Marshal_type*>::iterator it = fields.begin(); it != fields.end(); it ++)
    {
      Marshal_type *mt = *it;
      const char* field_name = mt->get_name();
      CCSTCompoundStatement * cs = mt->accept(this);
      statements.push_back(cs);
      printf("FIELDLDLFDLFL %s\n", field_name);
      CCSTAssignExpr *a = new CCSTAssignExpr(new CCSTPostFixExprAccess( new CCSTPrimaryExprId(param_name)
									, object_access_t
									, field_name)
					     , new CCSTAssignOp(equal_t) 
					     , new CCSTPrimaryExprId(field_name));
      CCSTExprStatement *assign_field = new CCSTExprStatement(a);
      statements.push_back(assign_field);
    }
  return new CCSTCompoundStatement(declarations, statements);
}
*/

CCSTCompoundStatement* UnmarshalTypeVisitor::visit(Marshal_integer *data)
{
  IntegerType* t = dynamic_cast<IntegerType*>(data->get_type());
  Assert(t != 0x0, "Error: expected IntegerType got something else.\n");

  std::vector<CCSTDecSpecifier*>specifier = type(t);
  
  std::vector<CCSTInitDeclarator*> decs;
  std::vector<CCSTAssignExpr*> empty_args;
  CCSTPostFixExprAssnExpr *func_call = new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId(access_register_mapping(data->get_register()))
								   ,empty_args);
  
  CCSTInitializer *init = new CCSTInitializer( new CCSTCastExpr(type_cast(t)
								,func_call));
  decs.push_back(new CCSTInitDeclarator(new CCSTDeclarator(0x0, new CCSTDirectDecId(data->get_name()))
					,init));
  
  std::vector<CCSTDeclaration*> comp_declarations;
  std::vector<CCSTStatement*> comp_statements;
  comp_declarations.push_back(new CCSTDeclaration(specifier, decs));
  return new CCSTCompoundStatement(comp_declarations, comp_statements);
}

CCSTCompoundStatement* UnmarshalTypeVisitor::visit(Marshal_void *data)
{
  VoidType *pt = dynamic_cast<VoidType*>(data->get_type());
  Assert(pt != 0x0, "Dynamic cast has failed!\n");
}

CCSTCompoundStatement* UnmarshalTypeVisitor::visit(Marshal_typedef *data)
{
  Typedef *pt = dynamic_cast<Typedef*>(data->get_type());
  Assert(pt != 0x0, "Dynamic cast has failed!\n");
}

 CCSTCompoundStatement* UnmarshalTypeVisitor::visit(Marshal_pointer *data)
{
  PointerType *pt = dynamic_cast<PointerType*>(data->get_type());
  Assert(pt != 0x0, "Error: expected PointerType got something else.\n");

  CCSTPointer *__p = create_pointer(count_nested_pointer(pt));
  Type *inner_t = get_non_pointer_type(pt);
  
  std::vector<CCSTDecSpecifier*>specifier = type(inner_t);
  std::vector<CCSTInitDeclarator*> decs;

  decs.push_back(new CCSTInitDeclarator( new CCSTDeclarator(__p
							    , new CCSTDirectDecId(data->get_name()))
					 , new CCSTInitializer( new CCSTUnaryExprCastExpr(new CCSTUnaryOp(unary_bit_and_t), new CCSTPrimaryExprId(data->get_m_type()->get_name())))));

  CCSTCompoundStatement *c = data->get_m_type()->accept(this);
  std::vector<CCSTDeclaration*> comp_declarations;
  comp_declarations.push_back(new CCSTDeclaration(specifier, decs));
  std::vector<CCSTStatement*> comp_statements;
  c->add_statement(new CCSTCompoundStatement(comp_declarations, comp_statements));
  return c;
}
