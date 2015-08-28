#include "ccst.h"


/* driver function */
CCSTCompoundStatement* marshal_parameter(Parameter *p)
{
  MarshalTypeVisitor *visitor = new MarshalTypeVisitor();
  return p->get_marshal_info()->accept(visitor);
}

CCSTCompoundStatement* MarshalTypeVisitor::visit(Marshal_projection *data)
{
  ProjectionType *pt = dynamic_cast<ProjectionType*>(data->get_type());
  Assert(pt != 0x0, "Error: Expected ProjectionType got something else.\n");

  std::vector<CCSTDeclaration*> comp_declarations;
  std::vector<CCSTStatement*> comp_statements;
  
  std::vector<Marshal_type*> fields = data->fields();
  for(std::vector<Marshal_type*>::iterator it = fields.begin(); it != fields.end(); it ++)
    {
      Marshal_type *mt = *it;
      std::vector<CCSTDecSpecifier*> type_specifier = type(mt->get_type());
      std::vector<CCSTInitDeclarator*> init_decs;
      if(mt->get_type()->num() == 3)
	{
	  printf("MDAASDFSADFSA1 is %s\n", mt->get_name());
	  init_decs.push_back(new CCSTInitDeclarator( new CCSTDeclarator(new CCSTPointer()
									 , new CCSTDirectDecId(mt->get_name()))
						      , new CCSTInitializer( new CCSTPostFixExprAccess(new CCSTPrimaryExprId(data->get_name()), object_access_t, mt->get_name()))));
	}
      else
	{
	  printf("MTMTMTMTM2 is %s\n", mt->get_name());
	  init_decs.push_back(new CCSTInitDeclarator( new CCSTDeclarator(0x0
									 , new CCSTDirectDecId(mt->get_name()))
						      , new CCSTInitializer( new CCSTPostFixExprAccess( new CCSTPrimaryExprId(data->get_name()), object_access_t, mt->get_name()))));
	}

      comp_declarations.push_back(new CCSTDeclaration(type_specifier, init_decs));
      comp_statements.push_back(mt->accept(this));
    }
  return new CCSTCompoundStatement(comp_declarations, comp_statements);
}

CCSTCompoundStatement* MarshalTypeVisitor::visit(Marshal_integer *data)
{
  IntegerType* t = dynamic_cast<IntegerType*>(data->get_type());
  Assert(t != 0x0, "Error: expected IntegerType got something else.\n");
  
  
  std::vector<CCSTDeclaration*> comp_declarations;
  std::vector<CCSTStatement*> comp_statements;
  
  std::vector<CCSTAssignExpr*> args;
  
  std::vector<CCSTSpecifierQual*> spec_quals;
  spec_quals.push_back(new CCSTSimpleTypeSpecifier(long_t));
  spec_quals.push_back(new CCSTSimpleTypeSpecifier(long_t));
  
  args.push_back(new CCSTCastExpr(new CCSTTypeName(spec_quals, 0x0)
				  , new CCSTPrimaryExprId(data->get_name())));
  
  CCSTPostFixExprAssnExpr *func_call = new CCSTPostFixExprAssnExpr(new CCSTPrimaryExprId(store_register_mapping(data->get_register()))
								   ,args);
  comp_statements.push_back(new CCSTExprStatement(func_call));
  return new CCSTCompoundStatement(comp_declarations, comp_statements);
  
}

CCSTCompoundStatement* MarshalTypeVisitor::visit(Marshal_void *data)
{
  
}

CCSTCompoundStatement* MarshalTypeVisitor::visit(Marshal_typedef *data)
{
  
}

CCSTCompoundStatement* MarshalTypeVisitor::visit(Marshal_pointer *data)
{
  PointerType* t = dynamic_cast<PointerType*>(data->get_type());
  Assert(t != 0x0, "Error: expected PointerType got something else.\n");
  
  Marshal_type *pointer_to = data->get_m_type();
  const char* name = pointer_to->get_name();

  std::vector<CCSTDeclaration*> comp_declarations;
  std::vector<CCSTStatement*> comp_statements;
  
  std::vector<CCSTDecSpecifier*> ds = type(pointer_to->get_type());
  std::vector<CCSTInitDeclarator*> id;
  id.push_back(new CCSTInitDeclarator( new CCSTDeclarator(0x0, new CCSTDirectDecId(name))
				       , new CCSTInitializer( new CCSTUnaryExprCastExpr(new CCSTUnaryOp(unary_mult_t)
											,new CCSTPrimaryExprId(data->get_name()) ) ) ));
  comp_declarations.push_back(new CCSTDeclaration(ds, id));
  
  comp_statements.push_back(pointer_to->accept(this));
  return new CCSTCompoundStatement(comp_declarations, comp_statements);
}
