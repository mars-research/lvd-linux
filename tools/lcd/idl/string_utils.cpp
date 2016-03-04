#include "code_gen.h"

const char* new_name(const char* name, const char* suffix)
{
  int length = strlen(name);
  int length2 = strlen(suffix);
  char *new_str = (char*) malloc(sizeof(char)*(length+length2+1));
  
  std::ostringstream total;
  total << name << suffix;
  strncpy(new_str, total.str().c_str(), length+length2+1);
  return new_str;
}

/*
 * returns a new string with _exit on the end.
 */
const char* exit_name(const char* name)
{
  return new_name(name, "_exit");
}

/*
 * returns a new string with _init on the end.
 */
const char* init_name(const char* name)
{
  return new_name(name, "_init");
}

/*
 * returns a new string with _container on the end.
 */
const char* container_name(const char* name)
{
  return new_name(name, "_container");
}

/*
 * returns a new string with _hidden_args on the end.
 */
const char* hidden_args_name(const char* name)
{
  return new_name(name, "_hidden_args");
}

/* 
 * returns a new string with _p on the end.
 */ 
const char* parameter_name(const char* name) 
{
  return new_name(name, "_p");
}

/*
 * returns a new string with _container on the end.
 */
const char* trampoline_func_name(const char* name)
{
  return new_name(name, "_trampoline");
}

/*
 * returns a new string of all uppercase chars.
 */
const char* string_to_upper(const char* str)
{
  char* ret = (char*) malloc((sizeof(str)+1)*sizeof(char));
  int i;
  for(i = 0; i < sizeof(str); i ++)
    {
      char tmp = str[i];
      ret[i] = toupper(tmp);
    }
  ret[i] = '\0';
  return ret;
}
