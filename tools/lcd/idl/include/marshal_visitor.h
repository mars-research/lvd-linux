#ifndef _MARSHAL_
#define _MARHSAL_
#include "visitor.h"

class MarshalVisitor : public ASTVisitor
{
  bool regs[LCD_MAX_REGS];
  bool cap_regs[LCD_MAX_CAP_REGS];
  int find_free_reg();
  void set_reg_used(int reg);
 public:
  virtual void visit(File *file);
  // virtual void visit(Message *message);
  // virtual void visit(MessageField *message_field);
  virtual void visit(ProjectionField *proj_field);
  virtual void visit(Rpc *rpc);
  virtual void visit(Parameter *parameter);

  // Types
  virtual void visit(ProjectionType *proj_type);
  virtual void visit(IntegerType *this);
  virtual void visit(PointerType *this);
  virtual void visit(Typedef* this);
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
  virtual void visit(IntegerType *this);
  virtual void visit(PointerType *this);
  virtual void visit(Typedef* this);
  packVisitor();
};

#endif
