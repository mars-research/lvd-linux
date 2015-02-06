#include "gen_visitor.h"

Caller_SourceVisitor::Caller_SourceVisitor(FILE* out)
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
  const char* indent = "  ";
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
      fprintf(this->out_f_, "%s%s\n", indent, p->marshal());
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

/* CALLEE code*/

Callee_SourceVisitor::Callee_SourceVisitor(FILE* out)
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
  exec_loop(f);
  
}

void Callee_SourceVisitor::visit(Rpc* r)
{

  Type* rt = r->return_type();
  /*
  printf("here in hv hisit rpc\n");
  printf("%d\n", rt->num()); */
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

  // TODO unmarshaling

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

void Callee_SourceVisitor::exec_loop(File* f)
{
  const char* indent = "  ";
  fprintf(this->out_f_, "int execution_loop(void) {\n");
  fprintf(this->out_f_, "%sfor (;;) {\n", indent);
  fprintf(this->out_f_, "%s%sret = lcd_recv(%s);\n",
	  indent, indent, "todo_cap_name");
  fprintf(this->out_f_, "%s%sif (ret) {\n", indent, indent);
  fprintf(this->out_f_, "%s%s%sgoto out; }\n", indent, indent, indent);
  
  // switch
  // don't like this hard coding
  fprintf(this->out_f_, "%s%sswitch (lcd_r0()) {\n", indent, indent);
  
  for(std::vector<Rpc*>::iterator it = f->rpc_defs()->begin(); it != f->rpc_defs()->end(); it++)
    {
      Rpc* r = (Rpc*) *it;
      const char* enum_char = r->enum_val();
      fprintf(this->out_f_, "%s%scase %s:\n", indent, indent, enum_char);
      fprintf(this->out_f_, "%s%s%sLCD_MSG(\"todo\");\n", indent, indent, indent);
      fprintf(this->out_f_, "%s%s%sret = %s();\n", indent, indent, indent, r->callee_name());
      fprintf(this->out_f_, "%s%s%sbreak;\n", indent, indent, indent);
    }
  // fprintf(this->out_f_, "TODO DIE\n");
  fprintf(this->out_f_, "%s%scase TODO_DIE:\n", indent, indent);
  fprintf(this->out_f_, "%s%s%sLCD_MSG(\"todo\");\n", indent, indent, indent);
  fprintf(this->out_f_, "%s%s%s die_callee();\n", indent, indent, indent);
  fprintf(this->out_f_, "%s%s%sgoto out;\n", indent, indent, indent);
  fprintf(this->out_f_, "%s%s}\n", indent, indent); // close switch
  fprintf(this->out_f_, "%s}\n", indent); // close for loop
  
  // out
  fprintf(this->out_f_, "out: \n");
  fprintf(this->out_f_, "%sreturn ret;\n", indent);
  fprintf(this->out_f_, "}\n");
  return;
}
