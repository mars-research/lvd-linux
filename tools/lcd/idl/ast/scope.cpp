#include "lcd_ast.h"
#include <stdio.h>

GlobalScope* GlobalScope::instance_ = 0;

GlobalScope::GlobalScope()
{
   // init builtin int types
  this->outer_scope_ = 0x0; // global scope 

  // move this code to wherever we create the root scope.
  // instert for each builtin in type, add size to type if not done alreayd
  this->type_definitions_.insert(std::pair<std::string,Type*>("void", new VoidType()));
  this->type_definitions_.insert( std::pair<std::string,Type*>("char"
					       , new IntegerType(pt_char_t, false, sizeof(char))));
  this->type_definitions_.insert( std::pair<std::string,Type*>("unsigned char"
					       , new IntegerType(pt_char_t, true, sizeof(char))));
  this->type_definitions_.insert( std::pair<std::string,Type*>("short"
					      , new IntegerType(pt_short_t, false, sizeof(short))));
  this->type_definitions_.insert( std::pair<std::string,Type*>("unsigned short"
					      , new IntegerType(pt_short_t, true, sizeof(short))));
  this->type_definitions_.insert( std::pair<std::string,Type*>("int"
					      , new IntegerType(pt_int_t, false, sizeof(int))));
  this->type_definitions_.insert( std::pair<std::string,Type*>("unsigned int"
					      , new IntegerType(pt_int_t, true, sizeof(int))));
  this->type_definitions_.insert( std::pair<std::string,Type*>("long"
					      , new IntegerType(pt_long_t, false, sizeof(long))));
  this->type_definitions_.insert( std::pair<std::string,Type*>("unsigned long"
					      , new IntegerType(pt_long_t, true, sizeof(long))));
  this->type_definitions_.insert( std::pair<std::string,Type*>("long long"
					      , new IntegerType(pt_longlong_t, false, sizeof(long long))));
  this->type_definitions_.insert( std::pair<std::string,Type*>("unsigned long long"
					      , new IntegerType(pt_longlong_t, true, sizeof(long long))));
  this->type_definitions_.insert( std::pair<std::string,Type*>("capability"
					       , new IntegerType(pt_capability_t, false, sizeof(int))));
  // dptpr_t
  std::vector<ProjectionField*> fields;
  int err;
  fields.push_back(new ProjectionField(this->lookup("unsigned long", &err), "dptr", 0)); // unsigned long dptr;
  this->type_definitions_.insert( std::pair<std::string, Type*>("dptr_t"
								, new Typedef("dptr_t", new ProjectionType("", "", fields))));
  
  // cptr_t
  std::vector<ProjectionField*> fields2;
  fields2.push_back(new ProjectionField(this->lookup("unsigned long", &err), "cptr", 0)); // unsigned long cptr;
  this->type_definitions_.insert( std::pair<std::string, Type*>("cptr_t"
								, new Typedef("cptr_t", new ProjectionType("", "", fields2))));


  // dstore no fields
  std::vector<ProjectionField*> fields3;
  this->type_definitions_.insert(std::pair<std::string, Type*>("dstore"
							       , new ProjectionType("dstore", "dstore", fields3)));
}

GlobalScope* GlobalScope::instance()
{
  if(!GlobalScope::instance_)
    GlobalScope::instance_ = new GlobalScope();
  return instance_;
}

bool GlobalScope::insert(Rpc *r)
{
  std::string temp(r->name());
  std::pair<std::string, std::vector<Parameter*> > p (temp, r->parameters());
  std::pair<std::map<std::pair<std::string, std::vector<Parameter*> >, Rpc*>::iterator, bool> ret;
  ret = this->rpc_definitions_.insert(std::pair<std::pair<std::string, std::vector<Parameter*> >, Rpc*>(p, r));
  return ret.second;
}

std::vector<Rpc*> GlobalScope::rpc_in_scope()
{
  std::vector<Rpc*> empty;
  return empty;
}

bool GlobalScope::contains(const char *symbol)
{
  std::string temp(symbol);
  if(this->type_definitions_.find(temp) == this->type_definitions_.end()) {
    return false;
  } else {
    return true;
  }
}

Type * GlobalScope::lookup(const char * sym, int* err)
{
  std::string temp(sym);
  
  if(this->type_definitions_.find(temp) == this->type_definitions_.end())
    {
      *err = 0;
      return 0;
    }
  else
    {
      *err = 1;
      return type_definitions_[temp];
    }
}

bool GlobalScope::insert(const char* sym, Type * value)
{
  std::string temp(sym);
  printf("insert %s\n",temp.c_str());
  std::pair<std::map<std::string,Type*>::iterator,bool> ret;
  ret = type_definitions_.insert(std::pair<std::string, Type*>(temp, value));
  
  return ret.second;
}

void GlobalScope::set_outer_scope(LexicalScope *ls)
{
  printf("error:  attempt to set outer scope of global scope\n");
  return;
}

void GlobalScope::add_inner_scope(LexicalScope *ls)
{
  this->inner_scopes_.push_back(ls);
}

void GlobalScope::add_inner_scopes(std::vector<LexicalScope*> scopes)
{
  printf("add inner scopes global scope todo\n");
}

std::map<std::string, Type*> GlobalScope::type_definitions()
{
  return this->type_definitions_;
}

std::vector<LexicalScope*> GlobalScope::inner_scopes()
{
  return this->inner_scopes_;
}

LexicalScope* GlobalScope::outer_scope()
{
  return this->outer_scope_;
}

void GlobalScope::resolve_types()
{
  for(std::map<std::string, Type*>::iterator it = this->type_definitions_.begin(); it != this->type_definitions_.end(); it ++) {
    it->second->resolve_types(this);
  }

  for(std::vector<LexicalScope*>::iterator it = this->inner_scopes_.begin(); it != this->inner_scopes_.end(); it ++) {
    LexicalScope *ls = (LexicalScope*) *it;
    ls->resolve_types();
  }
}

std::vector<Rpc*> GlobalScope::function_pointer_to_rpc()
{
  std::vector<Rpc*> rpcs;
  for(std::map<std::string, Type*>::iterator it = this->type_definitions_.begin(); it != this->type_definitions_.end(); it ++) {
    Type *t = it->second;
    
    if(t->num() == 4) { // projection type
      ProjectionType *pt = dynamic_cast<ProjectionType*>(t);
      Assert(pt != 0x0, "Error: dynamic cast to projection type failed!\n");
      std::vector<ProjectionField*> fields = pt->fields();
      
      for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
	ProjectionField *pf = (ProjectionField*) *it;
	
	if(pf->type()->num() == 7) { // function pointer field
	  Function *f = dynamic_cast<Function*>(pf->type());
	  rpcs.push_back(f->to_rpc(this, pt));
	}
      }
    }
    // continue
  }
  
  for(std::vector<LexicalScope*>::iterator it2 = this->inner_scopes_.begin(); it2 != this->inner_scopes_.end(); it2 ++) {
    LexicalScope *ls = (LexicalScope*) *it2;
    std::vector<Rpc*> tmp_rpcs = ls->function_pointer_to_rpc();
    rpcs.insert(rpcs.end(), tmp_rpcs.begin(), tmp_rpcs.end());
  }
  
  return rpcs;
}

/* -------------------------------------------------------------- */


LexicalScope::LexicalScope()
{
  this->outer_scope_ = 0x0;
}

LexicalScope::LexicalScope(LexicalScope *outer_scope)
{
  this->outer_scope_ = outer_scope;
}

bool LexicalScope::insert(Rpc *r)
{
  std::string temp(r->name());
  std::pair<std::string, std::vector<Parameter*> > p (temp, r->parameters());
  std::pair<std::map<std::pair<std::string, std::vector<Parameter*> >, Rpc*>::iterator, bool> ret;
  ret = this->rpc_definitions_.insert(std::pair<std::pair<std::string, std::vector<Parameter*> >, Rpc*>(p, r));
  return ret.second;
}

std::vector<Rpc*> LexicalScope::rpc_in_scope()
{
  printf("rpc in scope lexical scope todo\n");
  std::vector<Rpc*> empty;
  return empty;
}

bool LexicalScope::contains(const char *symbol)
{
  std::string temp(symbol);
  if(this->type_definitions_.find(temp) == this->type_definitions_.end()) {
    if(this->outer_scope_ == 0x0) {
      return false;
    } else {
      return this->outer_scope_->contains(symbol);
    }
  } else {
    return true;
  }

}

// is err needed if we just return null? when would null ever be valid?
Type* LexicalScope::lookup(const char *symbol, int *err)
{
  std::string temp(symbol);
  if(this->type_definitions_.find(temp) ==  this->type_definitions_.end()) {
    if(this->outer_scope_ == 0x0) {
      *err = 0;
      return 0x0;
    } else {
      return this->outer_scope_->lookup(symbol, err);
    }
  }
  else {
    *err = 1;
    return type_definitions_[temp];
  }
}

bool LexicalScope::insert(const char *symbol, Type *type)
{
  std::string temp(symbol);
  std::pair<std::map<std::string,Type*>::iterator,bool> ret;
  ret = type_definitions_.insert(std::pair<std::string, Type*>(temp, type));
  
  return ret.second;
}

void LexicalScope::set_outer_scope(LexicalScope *ls)
{
  this->outer_scope_ = ls;
}

void LexicalScope::add_inner_scope(LexicalScope *ls)
{
  this->inner_scopes_.push_back(ls);
}

void LexicalScope::add_inner_scopes(std::vector<LexicalScope*> scopes)
{
  printf("error add inner scopes lexical scope todo\n");
}

std::map<std::string, Type*> LexicalScope::type_definitions()
{
  return this->type_definitions_;
}

std::vector<LexicalScope*> LexicalScope::inner_scopes()
{
  return this->inner_scopes_;
}

LexicalScope* LexicalScope::outer_scope()
{
  return this->outer_scope_;
}

void LexicalScope::resolve_types()
{
  for(std::map<std::string, Type*>::iterator it = this->type_definitions_.begin(); it != this->type_definitions_.end(); it ++) {
    it->second->resolve_types(this);
  }

  for(std::vector<LexicalScope*>::iterator it = this->inner_scopes_.begin(); it != this->inner_scopes_.end(); it ++) {
    LexicalScope *ls = (LexicalScope*) *it;
    ls->resolve_types();
  }
}

std::vector<Rpc*> LexicalScope::function_pointer_to_rpc()
{
  std::vector<Rpc*> rpcs;
  for(std::map<std::string, Type*>::iterator it = this->type_definitions_.begin(); it != this->type_definitions_.end(); it ++) {
    Type *t = it->second;

    if(t->num() == 4) { // projection type
      ProjectionType *pt = dynamic_cast<ProjectionType*>(t);
      Assert(pt != 0x0, "Error: dynamic cast to projection type failed!\n");
      std::vector<ProjectionField*> fields = pt->fields();

      for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
	ProjectionField *pf = (ProjectionField*) *it;

	if(pf->type()->num() == 7) { // function pointer field
	  Function *f = dynamic_cast<Function*>(pf->type());
	  rpcs.push_back(f->to_rpc(this, pt));
	}
      }
    }
    // continue
  }

  for(std::vector<LexicalScope*>::iterator it2 = this->inner_scopes_.begin(); it2 != this->inner_scopes_.end(); it2 ++) {
    LexicalScope *ls = (LexicalScope*) *it2;
    std::vector<Rpc*> tmp_rpcs = ls->function_pointer_to_rpc();
    rpcs.insert(rpcs.end(), tmp_rpcs.begin(), tmp_rpcs.end());
  }
  
  return rpcs;
}
