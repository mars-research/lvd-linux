#include "lcd_ast.h"
#include <stdio.h>

GlobalScope* GlobalScope::instance_ = 0;

GlobalScope::GlobalScope()
{
   // init builtin int types
  this->outer_scope_ = 0x0; // global scope 

  // move this code to wherever we create the root scope.
  // instert for each builtin in type, add size to type if not done alreayd
  this->types_definitions_.insert(std::pair<std::string,Type*>("void", new VoidType()));
  this->types_definitions_.insert( std::pair<std::string,Type*>("char"
					       , new IntegerType(pt_char_t, false, sizeof(char))));
  this->types_definitions_.insert( std::pair<std::string,Type*>("unsigned char"
					       , new IntegerType(pt_char_t, true, sizeof(char))));
  this->types_definitions_.insert( std::pair<std::string,Type*>("short"
					      , new IntegerType(pt_short_t, false, sizeof(short))));
  this->types_definitions_.insert( std::pair<std::string,Type*>("unsigned short"
					      , new IntegerType(pt_short_t, true, sizeof(short))));
  this->types_definitions_.insert( std::pair<std::string,Type*>("int"
					      , new IntegerType(pt_int_t, false, sizeof(int))));
  this->types_definitions_.insert( std::pair<std::string,Type*>("unsigned int"
					      , new IntegerType(pt_int_t, true, sizeof(int))));
  this->types_definitions_.insert( std::pair<std::string,Type*>("long"
					      , new IntegerType(pt_long_t, false, sizeof(long))));
  this->types_definitions_.insert( std::pair<std::string,Type*>("unsigned long"
					      , new IntegerType(pt_long_t, true, sizeof(long))));
  this->types_definitions_.insert( std::pair<std::string,Type*>("long long"
					      , new IntegerType(pt_longlong_t, false, sizeof(long long))));
  this->types_definitions_.insert( std::pair<std::string,Type*>("unsigned long long"
					      , new IntegerType(pt_longlong_t, true, sizeof(long long))));
  this->types_definitions_.insert( std::pair<std::string,Type*>("capability"
					       , new IntegerType(pt_capability_t, false, sizeof(int))));
}

GlobalScope* GlobalScope::instance()
{
  if(!GlobalScope::instance_)
    GlobalScope::instance_ = new GlobalScope();
  return instance_;
}

Type * GlobalScope::lookup(const char * sym, int* err)
{
  std::string temp(sym);
  
  if(this->types_definitions_.find(temp) == this->type_definitions_.end())
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

int GlobalScope::insert(const char* sym, Type * value)
{
  std::string temp(sym);
  printf("insert %s\n",temp.c_str());
  std::pair<std::map<std::string,Type*>::iterator,bool> ret;
  ret = type_definitions_.insert(std::pair<std::string, Type*>(temp, value));
  
  return ret.second;
}

/* -------------------------------------------------------------- */

LexicalScope::LexicalScope()
{
  this->outer_scope_ = 0x0;
}

Type* LexicalScope::lookup(const char *symbol, int *err)
{
  std::string temp(symbol);
  if(this->type_definitions_.find(temp) ==  this->type_definitions_.end()) {
    return this->outer_scope_->lookup_symbol(symbol, err);
  }
  else {
    *err = 1;
    return type_definitions_[temp];
  }
}

int LexicalScope::insert(const char *symbol, Type *type)
{
  std::string temp(sym);
  printf("insert %s\n",temp.c_str());
  std::pair<std::map<std::string,Type*>::iterator,bool> ret;
  ret = type_definitions_.insert(std::pair<std::string, Type*>(temp, value));
  
  return ret.second;
}

