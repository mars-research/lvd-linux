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
  



Message::Message(char * n, std::vector<Argument*> * a)
{
  name = n;
  args = a;
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

Type::Type()
{
}

