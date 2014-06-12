#include "ast.h"
#include "flounder_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main(int argc, char ** argv)
{
  if(argc < 2)
    {
      printf("Must provide a filename.");
      exit(0);
    }
  try
    {
      printf("getting filename");
      char * filename = argv[1];
      printf("%s",filename);
      Interface * tree = (Interface *) Parser::parse(std::string(filename));
      printf("testing");
      exit(0);
    }
  catch (const Parser::ParseException &e)
    {
      printf("caught an exception");
      std::cout << e.getReason();
    }
  
}
