#include "visitor.h"

int generate_header_file(Interface * i, FILE * f_out)
{
  // we have an interface, one per file

  // need to write this out to a header file
  
  // what includes?
  
  // for each message in the interface write out to the file
  // a function definition for each message
  
  std::vector<Message*> * functions = i->getMessages();

  for(std::vector<Message*>::iterator it = functions->begin(); it != functions->end(); it ++)
    {
      char * str_out;
      Message * m = *it;
      std::vector<Argument*> * args = m->getArguments();
      char * args_str;
      if(args->size() == 0)
	{
	  args_str = (char *) malloc(sizeof(char));
	  args_str[0] = ' ';
	}
      else
	char * args_str = construct_args_str(args);
      int err = sprintf(str_out,"%s %s(%s);\n", "void", m->getName(), args_str);
      if(err < 0)
	{
	  printf("error using sprintf\n");
	  return -1;
	}
      fputs(str_out, f_out);
    }
  // all messages return void?

  
  // need to match up each Type with an actual C type, builtin type
  // or a typedef etc, support this later
  fclose(f_out);

  return 0;
}

char * construct_args_str(std::vector<Argument*> * args)
{
  if(args->size() == 0)
    printf("args size is zero\n");
  int str_len = 0;
  std::vector<char *> format_args = std::vector<char *>();
  for(std::vector<Argument*>::iterator it = args->begin(); it != args->end(); it ++)
    {
      printf("in for loop\n");
      char * str_arg;
      Argument * a = *it;
      if(a->isDynamic())
	{
	  char * type = a->getType()->getStr();
	  char * name = a->getName();
	  char * size = a->getSize();
	  int len = strlen(type) + strlen(size) + strlen(name);
	  str_arg = (char *) malloc(sizeof(char)*(4+len));
	  int err = sprintf(str_arg, "%s %s[%s]", type, name, size);
	  if(err < 0)
	    printf("error using sprintf\n");
	}
      else
	{
	  char * type = a->getType()->getStr();
	  printf("type is: %s\n", type);
	  char * name = a->getName();
	  int len = strlen(type) + strlen(name);
	  str_arg = (char *) malloc(sizeof(char)*(2+len));
	  int err = sprintf(str_arg, "%s %s", type, name);
	  printf("%s\n", str_arg);
	  if(err < 0)
	    printf("error using sprintf\n");
	}
      format_args.push_back(str_arg);
      str_len = str_len + strlen(str_arg);
    }
  char * total_args = (char *) malloc(sizeof(char)*(str_len+1) + sizeof(char)* (arg_count -1));
  
  char * first = format_args.front();
  printf("got here\n");
  strcpy(total_args, first);
  format_args.erase(format_args.begin());
  char * comma = (char *)malloc(sizeof(char)*2);
  comma[0] = ',';
  comma[1] = '\0';
  for(std::vector<char*>::iterator it2 = format_args.begin(); it2 != format_args.end(); it2 ++)
    {
      // need to add commas
      strcat(total_args, comma);
      strcat(total_args, *it2);
    }
  return total_args;
}

int generate_stub_file(Interface * i, FILE * f_out)
{
  return 0;
}
