#include "marshal_op.h"

 static const char* register_access_mapping_[]  = {REG0, REG1, REG2, REG3, REG4, REG5, REG6, REG7, REG8, REG9
			    ,REG10, REG11, REG12, REG13, REG14, REG15, REG16, REG17, REG18, REG19
			    ,REG20, REG21, REG22, REG23, REG24, REG25, REG26, REG27, REG28, REG29
			    ,REG30, REG31, REG32, REG33, REG34, REG35, REG36, REG37, REG38, REG39
			    ,REG40, REG41, REG42, REG43, REG44, REG45, REG46, REG47, REG48, REG49
			    ,REG50, REG51, REG52, REG53, REG54, REG55, REG56, REG57, REG58, REG59
			    ,REG60, REG61, REG62, REG63};

static const char* register_store_mapping_[] = {STORE_REG0, STORE_REG1, STORE_REG2, STORE_REG3, STORE_REG4, STORE_REG5, STORE_REG6, STORE_REG7, STORE_REG8, STORE_REG9
			    ,STORE_REG10, STORE_REG11, STORE_REG12, STORE_REG13, STORE_REG14, STORE_REG15, STORE_REG16, STORE_REG17, STORE_REG18, STORE_REG19
			    ,STORE_REG20, STORE_REG21, STORE_REG22, STORE_REG23, STORE_REG24, STORE_REG25, STORE_REG26, STORE_REG27, STORE_REG28, STORE_REG29
			    ,STORE_REG30, STORE_REG31, STORE_REG32, STORE_REG33, STORE_REG34, STORE_REG35, STORE_REG36, STORE_REG37, STORE_REG38, STORE_REG39
			    ,STORE_REG40, STORE_REG41, STORE_REG42, STORE_REG43, STORE_REG44, STORE_REG45, STORE_REG46, STORE_REG47, STORE_REG48, STORE_REG49
			    ,STORE_REG50, STORE_REG51, STORE_REG52, STORE_REG53, STORE_REG54, STORE_REG55, STORE_REG56, STORE_REG57, STORE_REG58, STORE_REG59
			    ,STORE_REG60, STORE_REG61, STORE_REG62, STORE_REG63};

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

void Registers::init(int regs_taken[], int len1, int caps_taken[], int len2)
{
  int i;
  for(i = 0; i < LCD_MAX_REGS && i < len1; i ++) {
    regs_[i] = regs_taken[i];
  }

  for(i = 0; i < LCD_MAX_CAP_REGS && i < len2; i ++) {
    cap_regs_[i] = caps_taken[i];
  }
}

void Registers::init(Registers *r1, Registers *r2)
{
  int i;
  for(i = 0; i < LCD_MAX_REGS; i ++) {
    regs_[i] = r1->regs_[i] + r2->regs_[i]; 
  }

  for(i = 0; i < LCD_MAX_CAP_REGS; i ++) {
    cap_regs_[i] = r1->cap_regs_[i] + r2->cap_regs_[i];
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
Marshal_projection::Marshal_projection()
{
}

void Marshal_projection::set_register(int r)
{
  Assert(1 == 0, "Error: this operation is not allowed\n");
}

int Marshal_projection::get_register()
{
  Assert(1 == 0, "Error: this operation is not allowed\n");
  return 0;
}

Marshal_integer::Marshal_integer(int r)
{
  this->register_ = r;
}

void Marshal_integer::set_register(int r)
{
  this->register_ = r;
}

int Marshal_integer::get_register()
{
  return this->register_;
}

Marshal_void::Marshal_void()
{
}

void Marshal_void::set_register(int r)
{
  Assert(1 == 0, "Error: this operation is not allowed\n");
}

int Marshal_void::get_register()
{
  Assert(1 == 0, "Error: this operation is now allowed\n");
}

Marshal_typedef::Marshal_typedef(Marshal_type *type)
{
  this->true_type_ = type;
}

Marshal_typedef::Marshal_typedef(const Marshal_typedef& other)
{
  this->true_type_ = other.true_type_->clone();
}

void Marshal_typedef::set_register(int r)
{
  true_type_->set_register(r);
}

int Marshal_typedef::get_register()
{
  return this->true_type_->get_register();
}

/* Marshal Prepare visitor code */

MarshalPrepareVisitor::MarshalPrepareVisitor(Registers *r)
{
  this->registers_ = r;
}

Marshal_type* MarshalPrepareVisitor::visit(UnresolvedType *ut)
{
  Assert(1 == 0, "Error: cannot marshal an unresolved type\n");
}

Marshal_type* MarshalPrepareVisitor::visit(Channel *c)
{
  printf("marshal prepare visitor channel todo!\n");
  return 0x0;
}

Marshal_type* MarshalPrepareVisitor::visit(Function *fp)
{
  printf("Error: cannot allocate a register for functino pointer\n");
  return 0x0;
}

Marshal_type* MarshalPrepareVisitor::visit(Typedef *td)
{
  Marshal_type *tmp = td->type()->accept(this);
  return new Marshal_typedef(tmp);
}

Marshal_type* MarshalPrepareVisitor::visit(VoidType *vt)
{
  printf("Error: cannot allocate a register for void type\n");
  return new Marshal_void();
}

Marshal_type* MarshalPrepareVisitor::visit(IntegerType *it)
{
  int r = this->registers_->allocate_next_free_register();
  
  if (r == -1) {
    Assert(1 == 0, "Error: have run out of registers\n");
  }
  
  return new Marshal_integer(r);
}

Marshal_type* MarshalPrepareVisitor::visit(ProjectionType *pt)
{
  // this doesn't work.
  /*
  std::vector<ProjectionField*> fields = pt->fields();
  
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
    ProjectionField *pf = *it;
    if (pf == 0x0) {
      Assert(1 == 0, "Error: null pointer in MarshalPrepareVisit visit ProjectionType\n");
    }
    
    pf->set_marshal_info( pf->type()->accept(this) );
  }
  */
  return new Marshal_projection();
}

Marshal_type* MarshalPrepareVisitor::visit(ProjectionConstructorType *pct)
{
  return new Marshal_projection();
}

Marshal_type* MarshalPrepareVisitor::visit(InitializeType *it)
{
  Assert( 1 == 0, "Error: cannot prepare marshal for initialize type\n");
}
