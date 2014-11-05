#include "../include/lcd_ast.h"

RootScope* RootScope::instance_ = 0;

RootScope::RootScope()
{
   // init builtin int types
  this->types_ = new std::map<const char*, Type*>();

  // instert for each builtin in type, add size to type if not done alreayd
  this->types_->insert( std::pair<const char*,Type*>("char"
					       , new IntegerType("char", false, sizeof("char"))));
  this->types_->insert( std::pair<const char*,Type*>("unsigned char"
					       , new IntegerType("unsigned char", true, sizeof("char"))));
  this->types_->insert( std::pair<const char*,Type*>("short"
					      , new IntegerType("short", false, sizeof("short"))));
  this->types_->insert( std::pair<const char*,Type*>("unsigned short"
					      , new IntegerType("unsigned short", true, sizeof("short"))));
  this->types_->insert( std::pair<const char*,Type*>("int"
					      , new IntegerType("int", false, sizeof("int"))));
  this->types_->insert( std::pair<const char*,Type*>("unsigned int"
					      , new IntegerType("unsigned int", true, sizeof("int"))));
  this->types_->insert( std::pair<const char*,Type*>("long"
					      , new IntegerType("long", false, sizeof("long"))));
  this->types_->insert( std::pair<const char*,Type*>("unsigned long"
					      , new IntegerType("unsigned long", true, sizeof("long"))));
  this->types_->insert( std::pair<const char*,Type*>("long long"
					      , new IntegerType("long long", false, sizeof("long long"))));
  this->types_->insert( std::pair<const char*,Type*>("unsigned long long"
					      , new IntegerType("unsigned long long", true, sizeof("long long"))));
  this->types_->insert( std::pair<const char*,Type*>("capability"
					       , new IntegerType("capability_t", false, sizeof("int"))));
}

RootScope* RootScope::instance()
{
  if(!RootScope::instance_)
    RootScope::instance_ = new RootScope();
  return instance_;
}

Type * RootScope::lookup_symbol(const char * sym, int* err)
{
  if(types_->find(sym) == types_->end())
    {
      // error
    }
  else
    return (*types_)[sym];
}

int RootScope::insert_symbol(const char* sym, Type * value)
{
  std::pair<std::map<const char*,Type*>::iterator,bool> ret;
  ret = types_->insert(std::pair<const char*, Type*>(sym, value));
  
  return ret.second;
}

// file scope
Type* FileScope::lookup_symbol(const char* sym, int* err)
{
  // lookup here or root first?
  if(types_->find(sym) == types_->end())
    {
      // error
    }
  else
    return (*types_)[sym];
  
}

int FileScope::insert_symbol(const char* sym, Type* value)
{
  // redefinition of something at root scope?
  std::pair<std::map<const char*,Type*>::iterator, bool> ret;
  ret = types_->insert(std::pair<const char*,Type*>(sym, value));
  
  return ret.second;
		      
}

FileScope::FileScope(RootScope* root)
{
  this->root_ = root;
  this->types_ = new std::map<const char*, Type*>();
}

