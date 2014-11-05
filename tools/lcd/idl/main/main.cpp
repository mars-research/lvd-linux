#include "../include/lcd_ast.h"
#include "../include/lcd_idl.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main(int argc, char ** argv)
{
  if(argc != 3)
    {
      printf("error in number of args\n");
      exit(0);
    }
  try
    {
      char * file = argv[2];
      Scope * tree = (Scope *) Parser::parse(std::string(file));
      
      char* option = argv[1];
      printf("option: %s\n", option);
      printf("we have a tree\n");
    }
  catch (const Parser::ParseException e)
    {
      printf("caught an exception\n");
      //      std::cout << e.getReason();
      exit(0);
    }
}
