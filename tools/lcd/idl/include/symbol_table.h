#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <sstream>

class SymbolTable 
{
  unsigned int last_tmp_;
  std::vector<const char*> symbols_;
 public:
  SymbolTable();
  SymbolTable(std::vector<const char*> symbols);
  const char* unique_tmp();
  bool contains(const char *symbol);
  int insert(const char *symbol);
  int insert(std::vector<const char*> symbols);
  std::string to_string(int value);
};

#endif
