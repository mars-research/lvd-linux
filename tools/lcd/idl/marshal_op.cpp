#include "marshal_op.h"

 static const char* register_access_mapping_[]  = {REG0, REG1, REG2, REG3, REG4, REG5, REG6, REG7, REG8, REG9
			    ,REG10, REG11, REG12, REG13, REG14, REG15, REG16, REG17, REG18, REG19
			    ,REG20, REG21, REG22, REG23, REG24, REG25, REG26, REG27, REG28, REG29
			    ,REG30, REG31, REG32, REG33, REG34, REG35, REG36, REG37, REG38, REG39
			    ,REG40, REG41, REG42, REG43, REG44, REG45, REG46, REG47, REG48, REG49
			    ,REG50, REG51, REG52, REG53, REG54, REG55, REG56, REG57, REG58, REG59
			    ,REG60, REG61, REG62, REG63};

static const char* register_store_mapping_[] = {REG0, REG1, REG2, REG3, REG4, REG5, REG6, REG7, REG8, REG9
			    ,REG10, REG11, REG12, REG13, REG14, REG15, REG16, REG17, REG18, REG19
			    ,REG20, REG21, REG22, REG23, REG24, REG25, REG26, REG27, REG28, REG29
			    ,REG30, REG31, REG32, REG33, REG34, REG35, REG36, REG37, REG38, REG39
			    ,REG40, REG41, REG42, REG43, REG44, REG45, REG46, REG47, REG48, REG49
			    ,REG50, REG51, REG52, REG53, REG54, REG55, REG56, REG57, REG58, REG59
			    ,REG60, REG61, REG62, REG63};

const char* access_register_mapping(int register_index) { return register_access_mapping_[register_index]; }

const char* store_register_mapping(int register_index) { return register_store_mapping_[register_index]; }

/* Registers code*/
/* 
 * Finds next free register, then
 * sets it as used
 */
Registers::Registers()
{
  init();
}

void Registers::init()
{
  int i;
  for(i = 0; i < LCD_MAX_REGS; i ++)
    {
      regs_[i] = 0;
    }
  for(i = 0; i < LCD_MAX_CAP_REGS; i ++)
    {
      cap_regs_[i] = 0;
    }
}

int Registers::allocate_next_free_register()
{
  int i;
  for(i = 0; i < LCD_MAX_REGS; i ++)
    {
      if(!regs_[i])
	{
	  regs_[i] = 1;
	  return i;
	}
    }
  printf("add Assert();");
  return -1;
}

/* marshal type code */

// placed these here instead of header to avoid compilation errors

CCSTCompoundStatement* Marshal_projection::accept(TypeVisitor *worker)
{
  return worker->visit(this);
}

CCSTCompoundStatement* Marshal_integer::accept(TypeVisitor *worker)
{
  return worker->visit(this);
}

CCSTCompoundStatement* Marshal_void::accept(TypeVisitor *worker)
{
  return worker->visit(this);
}

CCSTCompoundStatement* Marshal_typedef::accept(TypeVisitor *worker)
{
  return worker->visit(this);
}

CCSTCompoundStatement* Marshal_pointer::accept(TypeVisitor *worker)
{
  return worker->visit(this);
}

