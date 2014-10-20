#include "../include/gen_visitor.h"

HeaderVisitor::HeaderVisitor(FILE* out)
{
  this->out_file_ = out;
}

void HeaderVisitor::visit(File* f)
{
  // where to print verbatim?
  // print an ifndef?
  // need to go through all typedef and print out
  // a c typedef in file scope
  
  for(std::vector<Rpc*>::iterator it = f->rpc_defs_->begin(); it != rpc_defs.end(); it++)
    {
      Rpc* r = (Rpc*) *it;
      r->accept(this);
      fprintf(this->out_f_, "\n\n");
    }
}

void HeaderVisitor::visit(Rpc* r)
{
  Type* rt = r->ret_type;
  rt->accept(this); // this seems unnecessary in this case
  char* name = r->name_;
  fprintf(this->out_f_, " %s ", name); 
  
  for(std::vector<Parameter*>::iterator it = r->params->begin(); it != r->params->end(); it++)
    {
      Paramter* p = (Parameter*) *it;
      p->accept(this);
      if((it+1) != r->params->end())
	fprintf(this->out_f_, ", ");
    }
  fprintf(this->out_f_, ");");
}

void HeaderVisitor::visit(Parameter* p)
{
  p->type_->accept(this);
  // print p->name_
  fprintf(this->out_f_, "%s", p->name_);
}

void HeaderVisitor::visit(ProjectionType* p)
{
  fprintf(this->out_f_, "%s", p->real_type_);
}

void HeaderVisitor::visit(ProjectionField* pf)
{
  // nothing to do here
}

void HeaderVisitor::visit(IntegerType* it)
{
  if(it->unsigned_)
    fprintf(this->out_f_, "unsigned ");
    
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
}

void HeaderVisitor::visit(PointerType* pt)
{
  pt->type_->accept(this);
  fprintf(this->out_f_, "* ");
}

void HeaderVisitor::visit(Typedef* td)
{
  // print out their type so they arent confused
  // print td->alias;
  fprintf(this->out_f_, "%s", td->alias);
}
