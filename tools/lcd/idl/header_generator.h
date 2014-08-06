#ifndef LCD_HEADER_GENERATOR_H_
#define LCD_HEADER_GENERATOR_H_

#include "lcd_ast.h"
#include "visitor.h"
#include <stdio.h>

class HeaderGeneratorVisitor : public ASTVisitor
{
  FILE *out_file_;

 public:
  HeaderGeneratorVisitor(FILE *out_file) { 
    this->out_file_ = out_file; }
  virtual void visit(Scope *scope);
  virtual void visit(Message *message);
  virtual void visit(MessageField *message_field);
  virtual void visit(Projection *projection);
  virtual void visit(ProjectionField *proj_field);
  virtual void visit(Rpc *rpc);
  virtual void visit(Parameter *parameter);
  virtual void visit(Typedef *type_def);
  virtual void visit(ProjectionType *proj_type);
  virtual void visit(PrimitiveType *prim_type);
  virtual void visit(UnresolvedType *unresolved_type);
  // virtual void visit(Type *type);
  //  virtual void visit(Definition* def);  
};

#endif // LCD_HEADER_GENERATOR_H_
