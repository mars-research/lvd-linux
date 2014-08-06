#ifndef _VISITOR_
#define _VISITOR_

class ASTVisitor
{
 public:
  virtual void visit(Scope *scope) = 0;
  virtual void visit(Message *message) = 0;
  virtual void visit(MessageField *message_field) = 0;
  virtual void visit(Projection *projection) = 0;
  virtual void visit(ProjectionField *proj_field) = 0;
  virtual void visit(Rpc *rpc) = 0;
  virtual void visit(Parameter *parameter) = 0;
  virtual void visit(Typedef *type_def) = 0;
  virtual void visit(ProjectionType *proj_type) = 0;
  virtual void visit(PrimitiveType *prim_type) = 0;
  virtual void visit(UnresolvedType *unresolved_type) = 0;
  // virtual void visit(Type *type) = 0;
  //  virtual void visit(Definition* def) = 0;  
};

#endif
