#include "lcd_ast.h"

Module::Module(char * n, std::vector<Rpc *>* rs, std::vector<Message *>* ms,
      std::map<char *, Projection *>* e, std::map<char *, Module *>* mods)
{
  this->name = n;
  this->rpcs = rs;
  this->messages = ms;
  this->env = e;
  this->modules = mods;
}

Module::Module(std::vector<char *>* in, std::vector<Rpc *>* rs, std::vector<Message *>* ms,
      std::map<char *, Projection *>* e, std::map<char *, Module *>* mods)
{
  this->includes = in;
  this->rpcs = rs;
  this->messages = ms;
  this->env = e;
  this->modules = mods;
}

char * Module::get_name()
{
  return this->name;
}

Rpc::Rpc(Type* r, char * n, std::vector<Parameter *>* param)
{
  this->ret = r;
  this->name = n;
  this->parameters = param;
}

Projection::Projection(char * name, Type* real, std::vector<ProjField *>* f)
{
  this->name = name;
  this->underlying = real;
  this->fields = f;
}

char * Projection::get_name()
{
  return this->name;
}

ProjField::ProjField(int s, Type* t, char * name)
{
  this->specifications = s;
  this->t = t;
  this->name = name;
}

/* o is option such as "struct" or "projection
   t is type such as "int"
   m is module, "" if it belongs to top level
   p is * etc */
Type::Type(char * o, char * t, char * m, char * p)
{
  this->option = o;
  this->type_ = t;
  this->module = m;
  this->pointer = p;
}

Parameter::Parameter(Type* t, char * n)
{
  this->type = t;
  this->name = n;
}

bool Parameter::isProjection()
{
  
}

Message::Message(char * n, std::vector<Capability *>* cap)
{
  this->name = n;
  this->caps = cap;
}

/* Capability constructor */
Capability::Capability(char * n)
{
  this->name = n;
}

/* get name of capability */
char * Capability::get_name()
{
  return this->name;
}




