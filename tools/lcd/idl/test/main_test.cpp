#include "test.h"

int main(int argc, char ** argv)
{
  if(argc != 2)
    {
      exit(0);
    }
  try
    {
      char * file = argv[1];
      Parser::parse(std::string(file));
      printf("completed call to parse\n");
    }
  catch (const Parser::ParseException e)
    {
      exit(0);
    }
}
