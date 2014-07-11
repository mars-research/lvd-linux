#include "ast.h"
#include "flounder_parser.h"
#include "stub-generator.h"
#include "header-generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

int main(int argc, char ** argv)
{
  if(argc != 4)
    {
      printf("Command line must be of form: -option in_file out_file\n");
      exit(0);
    }
  try
    {
      char * option = argv[1];
      char * in_file = argv[2];
      char * out_file = argv[3];
      
     
      if(strstr(option,"-header"))
	{
	  FILE * out;
	  Interface * tree = (Interface *) Parser::parse(std::string(in_file));
	  printf("we have a tree\n");
	  out = fopen(out_file, "w");
	  printf("we have opened a file\n");
	  generate_header_file(tree, out);
	  
	}
      else if(strstr(option,"-stub"))
	{
	  FILE * out;
	  Interface * tree = (Interface *) Parser::parse(std::string(in_file));
	  out = fopen(out_file, "w");
	  generate_stub_file(tree, out);

	}
      else
	printf("not a valid option: %s\n", option);
      
      exit(0);
    }
  catch (const Parser::ParseException &e)
    {
      printf("caught an exception\n");
      std::cout << e.getReason();
    }
  
}
