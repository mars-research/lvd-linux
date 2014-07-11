#ifndef AST_H
#define AST_H

#include <vector>
#include <stdint.h>

class Type
{
  char * t;
 public:
  Type(char * t);
  char * getStr();
};

class Argument
{
  bool array; // true if arg of form: int name[size]
  Type * t;
  char * name;
  char * size; // NULL if array = false
 public:
  Argument(Type * t, char * n, char * s);
  Argument(Type * t, char *n);
  bool isDynamic();
  Type * getType();
  char * getName();
  char * getSize();
  
  
};

class Message
{
  char * name;
  std::vector<Argument*> * args;
 public:
  Message(char * n, std::vector<Argument* > * a);
  char * getName();
  std::vector<Argument*> * getArguments();

};

class Interface
{
  std::map<char *, char *> types;
  char * name;
  char * description;
  std::vector<Message*> * function_decs;
  void init_types();
 public:
  Interface(char * n, char * desc, std::vector<Message*> * m);
  Interface(char * n, std::vector<Message*> * m);
  std::vector<Message*> * getMessages();

};



enum builtin_type { bool_=1, cap_, char_, give_away_cap,
		    int_, int8, int16, int32, int64,
		    intptr, iref, size, string, uint8,
		    uint16, uint32, uint64, uintptr };

#endif
