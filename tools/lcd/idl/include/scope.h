#ifndef SCOPE_H
#define SCOPE_H
#include "lcd_ast.h"

class Root_scope : public Scope
{
  static Root_scope * instance_;
  Root_scope();
 public:
  static Root_scope * instance();
  something lookup_symbol(char * type);
  something insert_symbol(char* type, value);
}

#endif
