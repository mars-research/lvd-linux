#include "header_generator.h"

void HeaderGeneratorVisitor::visit(File *file)
{
  // maybe i should change name again
  // print out verbatim section
  // call accept on all rpc definitions
  // what about messages again?
  // what to do about symbol table
}

void HeaderGeneratorVisitor::visit(Message *message)
{
  // what is a message used for agian?
}

void HeaderGeneratorVisitor::visit(MessageField *message_field)
{
  // what is a message used for again?

}

void HeaderGeneratorVisitor::visit(Projection *projection)
{
  // don't need to do anything? only relevant
  // in c file generation
}

void HeaderGeneratorVisitor::visit(ProjectionField *proj_field)
{
  // do we need to do anything for header generation?
  // i believe this is only needed when generating
  // c files
}

void HeaderGeneratorVisitor::visit(Rpc *rpc)
{
  // need to call accept on the return type
  // need to print the name and paren
  // need to call accept on each of the parameters
}

void HeaderGeneratorVisitor::visit(Parameter *param)
{
  // need to call accept on the type and print the
  // name
}

void HeaderGeneratorVisitor::visit(Typedef *typedef)
{
  // do we need one for teypdefs? if we are replacing
  // the use of the typedef in the file, with the real type
  // we don't need to print a c typedef at the top, especially if there is one in another file
  // may not need a visit for typedef?  talk to anton
}

void HeaderGeneratorVisitor::visit(ProjectionType *proj_type)
{
  // just need to print the underlying type
}

void HeaderGeneratorVisitor::visit(PrimitiveType *prim_type)
{
  // just need to print the entire type.... 
}

void HeaderGeneratorVisitor::visit(UnresolvedType *unresolved_type)
{
  // need to lookup type in symbol table 
}


