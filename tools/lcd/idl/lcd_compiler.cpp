#include "include/lcd_ast.h"
#include "parser/lcd_idl.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main(int argc, char ** argv)
{
  if(argc != 2)
    {
      printf("error in number of args\n");
      exit(0);
    }
  try
    {
      char * file = argv[1];
      Scope * tree = (Scope *) Parser::parse(std::string(file));
      printf("we have a tree\n");
    }
  catch (const Parser::ParseException e)
    {
      printf("caught an exception\n");
      //      std::cout << e.getReason();
      exit(0);
    }
}
