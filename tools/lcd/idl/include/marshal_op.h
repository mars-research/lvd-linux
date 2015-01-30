#ifndef MARSHAL_OP
#define MARSHAL_OP

class M_info
{
  size_t size_;
};

class M_rpc : public M_info
{
  bool regs[LCD_MAX_REGS];
  bool cap_regs[LCD_MAX_CAP_REGS];
  
 public:
  M_rpc();
  int allocate_register(int reg);
  int next_free_register();
  
};

class M_type : public M_info
{ 
  int register_;
  // can add more necessary info later
 public:
  M_type();
  void set_size(size_t s);
};
