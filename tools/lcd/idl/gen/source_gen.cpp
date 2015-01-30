#include "gen_visitor.h"

Caller_SourceVisitor::SourceVisitor(FILE* out)
{
  this->out_f_ = out;
}

void Caller_SourceVisitor::visit(File* f)
{
  for(std::vector<Rpc*>::iterator it = f->rpc_defs()->begin(); it != f->rpc_defs()->end(); it++)
    {
      Rpc* r = (Rpc*) *it;
      r->accept(this);
      fprintf(this->out_f_, "\n\n");
    }
  
}

void Caller_SourceVisitor::visit(Rpc* r)
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

void Caller_SourceVisitor::visit(Parameter* p)
{
  p->type()->accept(this);
  // print p->name_
  fprintf(this->out_f_, " %s", p->name());
}

void Caller_SourceVisitor::visit(ProjectionType *p)
{
   fprintf(this->out_f_, "%s", p->real_type());
}

void Caller_SourceVisitor::visit(ProjectionField *pf)
{
}

void Caller_SourceVisitor::visit(IntegerType *it)
{
  if(it->unsigned_huh())
    fprintf(this->out_f_, "unsigned ");
    
  fprintf(this->out_f_, "%s", it->type());
}

void Caller_SourceVisitor::visit(PointerType *pt)
{
  pt->p_type()->accept(this);
  fprintf(this->out_f_, "* ");
}

void Caller_SourceVisitor::visit(Typedef* td)
{
   fprintf(this->out_f_, "%s", td->alias());
}

Callee_SourceVisitor::SourceVisitor(FILE* out)
{
  this->out_f_ = out;
}

void Callee_SourceVisitor::visit(File* f)
{
  for(std::vector<Rpc*>::iterator it = f->rpc_defs()->begin(); it != f->rpc_defs()->end(); it++)
    {
      Rpc* r = (Rpc*) *it;
      r->accept(this);
      fprintf(this->out_f_, "\n\n");
    }
  
}

void Callee_SourceVisitor::visit(Rpc* r)
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

void Callee_SourceVisitor::visit(Parameter* p)
{
  p->type()->accept(this);
  // print p->name_
  fprintf(this->out_f_, " %s", p->name());
}

void Callee_SourceVisitor::visit(ProjectionType *p)
{
   fprintf(this->out_f_, "%s", p->real_type());
}

void Callee_SourceVisitor::visit(ProjectionField *pf)
{
}

void Callee_SourceVisitor::visit(IntegerType *it)
{
  if(it->unsigned_huh())
    fprintf(this->out_f_, "unsigned ");
    
  fprintf(this->out_f_, "%s", it->type());
}

void Callee_SourceVisitor::visit(PointerType *pt)
{
  pt->p_type()->accept(this);
  fprintf(this->out_f_, "* ");
}

void Callee_SourceVisitor::visit(Typedef* td)
{
   fprintf(this->out_f_, "%s", td->alias());
}

