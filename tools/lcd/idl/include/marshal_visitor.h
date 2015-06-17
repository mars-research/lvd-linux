#ifndef _MARSHAL_
#define _MARSHAL_
#include "lcd_ast.h"

class File;
class ProjectionField;
class Rpc;
class Parameter;
class ProjectionType;
class IntegerType;
class PointerType;
class Typedef;
class VoidType;

class MarshalVisitor
{
 public:
  virtual Marshal_type* visit(File *file, Registers* data);
  virtual Marshal_type* visit(ProjectionField *proj_field, Registers *data);
  virtual Marshal_type* visit(Rpc *rpc, Registers *data);
  virtual Marshal_type* visit(Parameter *parameter, Registers *data);

  // Types
  virtual Marshal_type* visit(ProjectionType *proj_type, Registers *data);
  virtual Marshal_type* visit(IntegerType *t, Registers *data);
  virtual Marshal_type* visit(PointerType *t, Registers *data);
  virtual Marshal_type* visit(Typedef* t, Registers *data);
  virtual Marshal_type* visit(VoidType *vt, Registers *data);
  MarshalVisitor();
};

#endif
