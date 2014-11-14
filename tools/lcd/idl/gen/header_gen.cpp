#include "gen_visitor.h"
#include <stdio.h>

HeaderVisitor::HeaderVisitor(FILE* out)
{
  this->out_f_ = out;
}

void HeaderVisitor::visit(File* f)
{
  // where to print verbatim?
  // print an ifndef?
  // need to go through all typedef and print out
  // a c typedef in file scope
  
  printf("in file\n");
  for(std::vector<Rpc*>::iterator it = f->rpc_defs()->begin(); it != f->rpc_defs()->end(); it++)
    {
      Rpc* r = (Rpc*) *it;
      r->accept(this);
      fprintf(this->out_f_, "\n\n");
    }
}

void HeaderVisitor::visit(Rpc* r)
{
  printf("here\n");
  Type* rt = r->return_type();
  printf("here in hv hisit rpc\n");
  printf("%d\n", rt->num());
  rt->accept(this); // this seems unnecessary in this case
  const char* name = r->name();
  fprintf(this->out_f_, " %s(", name); 
  
  for(std::vector<Parameter*>::iterator it = r->parameters()->begin(); it != r->parameters()->end(); it++)
    {
      Parameter* p = (Parameter*) *it;
      p->accept(this);
      if((it+1) != r->parameters()->end())
	fprintf(this->out_f_, ", ");
    }
  fprintf(this->out_f_, ");");
}

void HeaderVisitor::visit(Parameter* p)
{
  p->type()->accept(this);
  // print p->name_
  fprintf(this->out_f_, " %s", p->name());
}

void HeaderVisitor::visit(ProjectionType* p)
{
  fprintf(this->out_f_, "%s", p->real_type());
}

void HeaderVisitor::visit(ProjectionField* pf)
{
  // nothing to do here
}

void HeaderVisitor::visit(IntegerType* it)
{
  if(it->unsigned_huh())
    fprintf(this->out_f_, "unsigned ");
    
  fprintf(this->out_f_, "%s", it->type());
  /*
    switch (it->type_) {
    case kChar: 
      fprintf(this->out_f_, "char");
      break;
    case kShort:
      fprintf(this->out_f_, "short");
      break;
    case kInt:
      fprintf(this->out_f_, "int");
      break;
    case kLong:
      fprintf(this->out_f_, "long");
      break;
    case kLongLong:
      fprintf(this->out_f_, "long long");
      break;
    case kCapability:
      fprintf(this->out_f_, "capability_t");
      break;
    default:
      //error
    }
  */
}

void HeaderVisitor::visit(PointerType* pt)
{
  pt->type()->accept(this);
  fprintf(this->out_f_, "* ");
}

void HeaderVisitor::visit(Typedef* td)
{
  // print out their type so they arent confused
  // print td->alias;
  fprintf(this->out_f_, "%s", td->alias());
}
