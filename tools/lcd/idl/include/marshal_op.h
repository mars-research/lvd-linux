#ifndef MARSHAL_OP
#define MARSHAL_OP

class M_info
{
  size_t size_;
};

class M_rpc : public M_info
{
  /*
  bool regs[LCD_MAX_REGS];
  bool cap_regs[LCD_MAX_CAP_REGS]; */
 public:
  M_rpc();
};

class M_proj : public M_info
{
 public:
  M_proj(size_t size) { size_ = size; } 

};

class M_param : public M_info
{
  int reg;
  int size;
 public:
  M_param();
  init(int size, int reg);
};

class M_projType : public M_info
{
 public:
  M_projType(size_t size) { size_ = size; }
};

class M_intType : public M_info
{
 public:
  M_intType(size_t size) {size_ = size; }
  
};

class M_pointerType : public M_info
{
 public:
  M_pointerType(size_t size) { size_ = size; }
};
