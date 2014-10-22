#include "lcd_ast.h"

Scope::Scope()
{
  // init builtin int types
  builtin = new map<PrimType, Type>();
  // instert for each builtin in type, add size to type if not done alreayd
  builtin->insert( std::pair<int,Type>(1, new IntegerType(kChar, false, sizeof("char"))));
  builtin->insert( std::pair<int,Type>(1, new IntegerType(kChar, true, sizeof("char"))));
  builtin->insert( std::pair<int,Type>(1, new IntegerType(kShort, false, sizeof("short"))));
  builtin->insert( std::pair<int,Type>(1, new IntegerType(kShort, true, sizeof("short"))));
  builtin->insert( std::pair<int,Type>(1, new IntegerType(kInt, false, sizeof("int"))));
  builtin->insert( std::pair<int,Type>(1, new IntegerType(kInt, true, sizeof("int"))));
  builtin->insert( std::pair<int,Type>(1, new IntegerType(kLong, false, sizeof("long"))));
  builtin->insert( std::pair<int,Type>(1, new IntegerType(kLong, true, sizeof("long"))));
  builtin->insert( std::pair<int,Type>(1, new IntegerType(kLongLong, false, sizeof("long long"))));
  builtin->insert( std::pair<int,Type>(1, new IntegerType(kLongLong, true, sizeof("long long"))));
  builtin->insert( std::pair<int,Type>(1, new IntegerType(kCapability, false, sizeof("int"))));
}

void Typedef::accept(ASTVisitor* worker)
{
  worker->visit(this);
}

void IntegerType::accept(ASTVisitor* worker)
{
  worker->visit(this);
}

void PointerType::accept(ASTVisitor* worker)
{
  worker->visit(this);
}

void ProjectionType::accept(ASTVisitor* worker)
{
  worker->visit(this);
}

void Parameter::accept(ASTVisitor* worker)
{
  worker->visit(this);
}

void Rpc::accept(ASTVisitor* worker)
{
  worker->visit(this);
}

void ProjectionField::accept(ASTVisitor* worker)
{
  worker->visit(this);
}

void File::accept(ASTVisitor* worker)
{
  worker->visit(this);
}
