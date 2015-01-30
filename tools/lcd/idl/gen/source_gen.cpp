#include "gen_visitor.h"

SourceVisitor::SourceVisitor(FILE* out)
{
  this->out_f_ = out;
}

void SourceVisitor::visit(File* f)
{
  for(std::vector<Rpc*>::iterator it = f->rpc_defs()->begin(); it != f->rpc_defs()->end(); it++)
    {
      Rpc* r = (Rpc*) *it;
      r->accept(this);
      fprintf(this->out_f_, "\n\n");
    }
  
}

void SourceVisitor::visit(Rpc* r)
{

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
  fprintf(this->out_f_, ") {\n");
  
  for(std::vector<Parameter*>::iterator it = r->parameters()->begin(); it != r->parameters()->end(); it++)
    {
      Parameter* p = (Parameter*) *it;
      fprintf(this->out_f_, "%s\n", p->marshal());
    }
  fprintf(this->out_f_, "}\n");
}

void SourceVisitor::visit(Parameter* p)
{
  p->type()->accept(this);
  // print p->name_
  fprintf(this->out_f_, " %s", p->name());
}

void SourceVisitor::visit(ProjectionType *p)
{
   fprintf(this->out_f_, "%s", p->real_type());
}

void SourceVisitor::visit(ProjectionField *pf)
{
}

void SourceVisitor::visit(IntegerType *it)
{
  if(it->unsigned_huh())
    fprintf(this->out_f_, "unsigned ");
    
  fprintf(this->out_f_, "%s", it->type());
}

void SourceVisitor::visit(PointerType *pt)
{
  pt->p_type()->accept(this);
  fprintf(this->out_f_, "* ");
}

void SourceVisitor::visit(Typedef* td)
{
   fprintf(this->out_f_, "%s", td->alias());
}

