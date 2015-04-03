#include "lcd_ast.h"
#include <stdio.h>

RootScope* RootScope::instance_ = 0;

RootScope::RootScope()
{
   // init builtin int types
  this->types_ = new std::map<std::string, Type*>();
  
  // instert for each builtin in type, add size to type if not done alreayd
  this->types_->insert(std::pair<std::string,Type*>("void", new VoidType()));
  this->types_->insert( std::pair<std::string,Type*>("char"
					       , new IntegerType("char", false, sizeof(char))));
  this->types_->insert( std::pair<std::string,Type*>("unsigned char"
					       , new IntegerType("unsigned char", true, sizeof(char))));
  this->types_->insert( std::pair<std::string,Type*>("short"
					      , new IntegerType("short", false, sizeof(short))));
  this->types_->insert( std::pair<std::string,Type*>("unsigned short"
					      , new IntegerType("unsigned short", true, sizeof(short))));
  this->types_->insert( std::pair<std::string,Type*>("int"
					      , new IntegerType("int", false, sizeof(int))));
  this->types_->insert( std::pair<std::string,Type*>("unsigned int"
					      , new IntegerType("unsigned int", true, sizeof(int))));
  this->types_->insert( std::pair<std::string,Type*>("long"
					      , new IntegerType("long", false, sizeof(long))));
  this->types_->insert( std::pair<std::string,Type*>("unsigned long"
					      , new IntegerType("unsigned long", true, sizeof(long))));
  this->types_->insert( std::pair<std::string,Type*>("long long"
					      , new IntegerType("long long", false, sizeof(long long))));
  this->types_->insert( std::pair<std::string,Type*>("unsigned long long"
					      , new IntegerType("unsigned long long", true, sizeof(long long))));
  this->types_->insert( std::pair<std::string,Type*>("capability"
					       , new IntegerType("capability_t", false, sizeof(int))));
}

RootScope* RootScope::instance()
{
  if(!RootScope::instance_)
    RootScope::instance_ = new RootScope();
  return instance_;
}

Type * RootScope::lookup_symbol(const char * sym, int* err)
{
  std::string temp(sym);
  
  if(this->types_->find(temp) == this->types_->end())
    {
      *err = 0;
      return 0;
    }
  else
    {
      *err = 1;
      return (*(this->types_))[temp];
    }
}

int RootScope::insert_symbol(const char* sym, Type * value)
{
  std::string temp(sym);
  printf("insert %s\n",temp.c_str());
  std::pair<std::map<std::string,Type*>::iterator,bool> ret;
  ret = types_->insert(std::pair<std::string, Type*>(temp, value));
  
  return ret.second;
}

// file scope
Type* FileScope::lookup_symbol(const char* sym, int* err)
{
  std::string temp(sym);
  
  if(this->types_->find(temp) == this->types_->end())
    {
      return this->root_->lookup_symbol(sym, err); 
    }
  else
    {
      *err = 1;
      return (*types_)[temp];
    }
  
}

int FileScope::insert_symbol(const char* sym, Type* value)
{
  std::string temp(sym);
  // redefinition of something at root scope?
  std::pair<std::map<std::string,Type*>::iterator, bool> ret;
  // filescope map not rootscope
  ret = this->types_->insert(std::pair<std::string,Type*>(temp, value));
  return ret.second; 
}

FileScope::FileScope(RootScope* root)
{
  this->root_ = root;
  this->types_ = new std::map<std::string, Type*>();
}

