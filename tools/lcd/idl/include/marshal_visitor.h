#ifndef _MARSHAL_
#define _MARSHAL_
#include "lcd_ast.h"
#include <string>
#include <sstream>

class File;
class ProjectionField;
class Rpc;
class Parameter;
class ProjectionType;
class IntegerType;
class PointerType;
class Typedef;
class VoidType;
class Marshal_type;
class Registers;
class FileScope;
class RootScope;

const char* ret_name_gen(char *name, int num);

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
  virtual Marshal_type* visit(FileScope *fs, Registers *data);
  virtual Marshal_type* visit(RootScope *rs, Registers *data);
  MarshalVisitor();
};

#endif
