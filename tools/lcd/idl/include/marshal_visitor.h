#ifndef _MARSHAL_
#define _MARHSAL_
#include "lcd_ast.h"


class MarshalVisitor : public ASTVisitor
{
 public:
  virtual void visit(File *file);
  // virtual void visit(Message *message);
  // virtual void visit(MessageField *message_field);
  virtual void visit(ProjectionField *proj_field);
  virtual void visit(Rpc *rpc);
  virtual void visit(Parameter *parameter);

  // Types
  virtual void visit(ProjectionType *proj_type);
  virtual void visit(IntegerType *t);
  virtual void visit(PointerType *t);
  virtual void visit(Typedef* t);
  virtual void visit(VoidType *vt);
  MarshalVisitor();
};

class packVisitor : public ASTVisitor
{
 public:
  virtual void visit(File *file);
  // virtual void visit(Message *message);
  // virtual void visit(MessageField *message_field);
  virtual void visit(ProjectionField *proj_field);
  virtual void visit(Rpc *rpc);
  virtual void visit(Parameter *parameter);

  // Types
  virtual void visit(ProjectionType *proj_type);
  virtual void visit(IntegerType *t);
  virtual void visit(PointerType *t);
  virtual void visit(Typedef* t);
  packVisitor();
};

#endif
