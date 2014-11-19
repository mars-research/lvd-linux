#include "lcd_ast.h"
#include "lcd_idl.h"
#include "gen_visitor.h"
#include "error.h"
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
      ErrorReport* er = ErrorReport::instance();
      if(er->errors())
	{
	  printf("There were errors during parsing\n");
	  // cleanup?
	  exit(0);
	}
      char* out_option = argv[3];
      char* out_file = argv[4];
      FILE* of = fopen(out_file, "w");
      if(!of)
	{
	  printf("Error: unable to open %s for writing\n", out_file);
	  // cleanup
	  exit(0);
	}
      if(!strcmp(argv[1],"-header"))
	{
	  printf("in option\n");
	  HeaderVisitor* hv = new HeaderVisitor(of);
	  printf("have created hv\n");
	  tree->accept(hv);
	  printf("have finished hv\n");
	}
      else if(!strcmp(argv[1],"-source"))
	{
	  // TODO
	}
      else
	{
	  printf("error unrecognized option: %s\n", argv[1]);
	}
      return 0;
    }
  catch (const Parser::ParseException e)
    {
      printf("caught a parser exception\n");
      //  printf("e is: %s\n", e.getReason().c_str());
      
      exit(0);
    }
}
