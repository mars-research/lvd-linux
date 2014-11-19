#include "error.h"

ErrorReport* ErrorReport::instance_ = 0;

ErrorReport* ErrorReport::instance()
{
  if(!ErrorReport::instance_)
    ErrorReport::instance_ = new ErrorReport();
  return instance_;
}

ErrorReport::ErrorReport()
{
  errors_ = false;
}

void ErrorReport::set_error()
{
  this->errors_ = true;
}

bool ErrorReport::errors()
{
  return errors_;
}

