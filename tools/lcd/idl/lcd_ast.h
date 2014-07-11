#ifndef LCD_AST_H
#define LCD_AST_H

#include <vector>
#include <map>

#define IN 0x1
#define OUT 0x2
#define BIND 0x4
#define ALLOC 0x8

enum definition_type {rpc, module, message, projection};

class Definition
{
 public:
  virtual definition_type get_definition_type() = 0;

};

class Type
{
  char * option;
  char * type_;
  char * module;
  char * pointer;

 public:
  Type(char * o, char * t, char * m, char * p);

};

class Parameter
{
  Type* type;
  char* name;

 public:
  Parameter(Type* t,char * n);
  bool isProjection();
};

class Rpc : public Definition
{
  Type* ret;
  char * name;
  std::vector<Parameter *>* parameters;

 public:
  Rpc(Type* r, char * n, std::vector<Parameter *>* param);
  definition_type get_definition_type(){return rpc;}

};

class Capability // capability? for message
{
  char * name;
  
 public:
  Capability(char * n);
  char * get_name();

};

class ProjField
{
  int specifications; // set bits for in out bind alloc
  Type* t;
  char * name;
  
 public:
  ProjField(int s, Type* t, char * name);
  
};

class Projection : public Definition
{
  char * name;
  Type* underlying;
  std::vector<ProjField *>* fields;

 public:
  Projection(char * name, Type* real, std::vector<ProjField *>* f);
  definition_type get_definition_type(){return projection;}
  char * get_name();

};

class Message : public Definition
{
  char * name;
  std::vector<Capability *>* caps;

 public:
  Message(char * n, std::vector<Capability *>* e);
  std::vector<Capability *>* get_capabilities();
  definition_type get_definition_type() {return message; }
};

class Module
{
  char * name; // just leave "" if top level
  std::vector<char *> * includes; // only top level can have includes
  std::vector<Rpc *> * rpcs;
  std::vector<Message *> * messages;
  std::map<char *, Projection *> *env;
  std::map<char *, Module *> *modules;
  
 public:
  Module(char * n, std::vector<Rpc *>* rs, std::vector<Message *>* ms,
      std::map<char *, Projection *>* e, std::map<char *, Module *>* mods);
  // for top level
  Module(std::vector<char *>* in, std::vector<Rpc *>* rs, std::vector<Message *>* ms,
      std::map<char *, Projection *>* e, std::map<char *, Module *>* mods);
  char * get_name();
  
};



#endif
