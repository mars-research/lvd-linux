#ifndef MARSHAL_OP_H
#define MARSHAL_OP_H

#define LCD_MAX_REGS 64
#define LCD_MAX_CAP_REGS 8

#include <stdlib.h>

class M_info
{
 public:
  size_t size_;
};

class M_rpc : public M_info
{
  int* regs;
  int* cap_regs;
  
 public:
  M_rpc();
  int allocate_register(int reg);
  int next_free_register();
  void init();
};

class M_type : public M_info
{ 
  int register_;
  // can add more necessary info later
 public:
  M_type();
  void set_size(size_t s);
  void set_register(int reg);
};

#endif
