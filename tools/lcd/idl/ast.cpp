#include "ast.h"


Interface::Interface(char * n, char * desc, std::vector<Message*> * m)
  {
    name = n;
    description = desc;
    function_decs = m;
  }

Interface::Interface(char * n, std::vector<Message*> * m)
  {
    name = n;
    description = 0;
    function_decs = m;
  }

std::vector<Message*> *Interface::getMessages()
{
  return this->function_decs;
}
  
void Interface::init_types()
{
  // best way to do builtin types ?
  this->types = new std::map<char *,char *>();
  /*
    bool cap char give_away_cap int int8
    int16 int32 int64 intptr iref size
    string uint8 uint16 uint32 uint64 uintptr
   */
  // insert pairs
  this->types->insert( std::pair<char *, char *>("bool", "int"));
  // this for all in above....
						 

}

void Interface::add_type_alias(char * a, char * b)
{
 
}


Message::Message(char * n, std::vector<Argument*> * a)
{
  this->name = n;
  this->args = a;
}

char * Message::getName()
{
  return this->name;
}

std::vector<Argument*> *Message::getArguments()
{
  return this->args;
}

Argument::Argument(Type * t_, char * n, char * s)
{
  array = true;
  t = t_;
  name = n;
  size = s;
  }

Argument::Argument(Type * t_, char * n)
{
  array = false;
  t = t_;
  name = n;
  size =  0;
}

bool Argument::isDynamic()
{
  return this->array;
}

Type * Argument::getType()
{
  return this->t;
}

char * Argument::getName()
{
  return this->name;
}

char * Argument::getSize()
{
  return this->size;
}

Type::Type(char * t)
{
  this->t = t;
}

char * Type::getStr()
{
  return this->t;
}

