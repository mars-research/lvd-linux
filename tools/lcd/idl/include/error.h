#ifndef ERROR_H
#define ERROR_H

#include <vector>

class ErrorReport
{
  static ErrorReport* instance_;
  ErrorReport();
  bool errors_;
 public:
  static ErrorReport* instance();
  bool errors();
  void set_error();
};

#endif
