#include "lcd_ast.h"
#include "lcd_idl.h"
#include "gen_visitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

int main(int argc, char ** argv)
{
  if(argc != 5)
    {
      printf("error in number of args\n");
      exit(0);
    }
  try
    {
      char * file = argv[2];
      File * tree = (File *) Parser::parse(std::string(file));
      
      char* option = argv[1];
      char* out_option = argv[3];
      char * out_file = argv[4];
      FILE* of = fopen(out_file, "w");
      printf("outside if statements\n");
      if(!of)
	{
	  printf("Error: unable to open %s for writing\n", out_file);
	  // cleanup
	  exit(0);
	}
      if(!strcmp(option,"-header"))
	{
	  printf("in option\n");
	  HeaderVisitor* hv = new HeaderVisitor(of);
	  printf("have created hv\n");
	  tree->accept(hv);
	  printf("have finished hv\n");
	}
      else if(!strcmp(option,"-source"))
	{
	  
	}
      else
	{
	  printf("error unrecognized option: %s\n", option);
	}
    }
  catch (const Parser::ParseException e)
    {
      printf("caught an exception\n");
      //      std::cout << e.getReason();
      exit(0);
    }
}
