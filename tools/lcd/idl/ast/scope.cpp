#include "include/scope.h"

static Root_scope * Root_scope::instance()
{
  if(!instance_)
    instance_ = new Root_scope();
  return instance_;
}

Type * Root_scope::lookup_symbol(char * sym, int* err)
{
  if(types_.find(sym) == types_.end())
    {
      // error
    }
  else
    return types_[sym];
}

int Root_scope::insert_symbol(char * sym, Type * value)
{
  std::pair<std::map<char*,Type*>::iterator,bool> ret;
  ret = types_.insert(std::pair<char*, Type*>(sym, value));
  
  return ret.second;
}
