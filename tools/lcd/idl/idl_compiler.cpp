#include "ast.h"
#include "flounder_parser.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
  if(argc < 2)
    {
      printf("Must provide a filename.");
      exit(0);
    }
  
  char * filename = argv[1];
  Interface * tree = (Interface *) Parser::parse(filename);
  printf("testing");
  exit(0);
  
}
