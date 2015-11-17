#include "symbol_table.h"

SymbolTable::SymbolTable()
{
  // init symbols
}

SymbolTable::SymbolTable(std::vector<const char*> symbols)
{
  this->symbols_ = symbols;
}

/*
 * returns a unique const char* that begins with tmp 
 * that is not already in symbol table
 * adds to symbol table
 */
const char* SymbolTable::unique_tmp()
{
  std::string str_tmp = "tmp" + to_string(this->last_tmp_ + 1);
  char *tmp = (char*) malloc(sizeof(char)*(str_tmp.length()+1));
  strcpy(tmp, str_tmp.c_str());
  if(!contains(tmp)) {
    this->last_tmp_ += 1;
    this->insert(tmp);
    return tmp;
  } else {
    this->last_tmp_ += 1;
    return unique_tmp();
  }
}

/*
 * returns true if symbol table contains symbol
 */
bool SymbolTable::contains(const char *symbol)
{
  for(std::vector<const char*>::iterator it = this->symbols_.begin(); it != this->symbols_.end(); it ++) {
    const char *str = (const char*) *it;
    if( strcmp(str, symbol) == 0) {
      return true;
    }
  }
  return false;
}

/*
 * inserts symbol into symbol table
 * if symbol is already in table returns -1
 * returns 0 on success
 */
int SymbolTable::insert(const char *symbol)
{
  if(contains(symbol))
    return -1;
  
  this->symbols_.push_back(symbol);
  return 0;
}

/*
 * inserts a list of symbols into the symbol table
 * if any of these symbols are already in the table it fails
 * and returns -1
 * returns 0 on success
 */
int SymbolTable::insert(std::vector<const char*> symbols)
{
  for(std::vector<const char*>::iterator it = symbols.begin(); it != symbols.end(); it ++) {
    const char *str = (const char*) *it;
    if(contains(str))
      return -1;
  }
  
  this->symbols_.insert(this->symbols_.end(), symbols.begin(), symbols.end());
  return 0;
}

std::string SymbolTable::to_string(int val)
{
  std::ostringstream os;
  os << val;
  return os.str();
}
