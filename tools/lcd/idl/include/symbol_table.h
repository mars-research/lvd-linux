#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include <string>

class SymbolTable 
{
  unsigned int last_tmp_;
  std::vector<const char*> symbols_;
 public:
  SymbolTable();
  const char* unique_tmp();
  bool contains();
  void insert(const char *symbol);
  void insert(std::vector<const char*> symbols);
};

#endif
