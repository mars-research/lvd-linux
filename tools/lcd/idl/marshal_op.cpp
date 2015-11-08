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
Marshal_projection::Marshal_projection(ProjectionType *pt, std::vector<Marshal_type*> in_fields, std::vector<Marshal_type*> out_fields)
{
  this->pt_ = pt;
  this->param_name_ = "";
  this->in_fields_ = in_fields;
  this->out_fields_ = out_fields;
}

void Marshal_projection::set_name(const char *name)
{
  this->param_name_ = name;
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

std::vector<Marshal_type*> Marshal_projection::in_fields()
{
  return this->in_fields_;
}

std::vector<Marshal_type*> Marshal_projection::out_fields()
{
  return this->out_fields_;
}

Type* Marshal_projection::type()
{
  return (Type*) this->pt_;
}

const char* Marshal_projection::name()
{
  return this->param_name_;
}

CCSTCompoundStatement* Marshal_projection::accept(TypeVisitor *worker)
{
  return worker->visit(this);
}

Marshal_integer::Marshal_integer(IntegerType *it, int r)
{
  this->it_ = it;
  this->param_name_ = "";
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

void Marshal_integer::set_name(const char *name)
{
  this->param_name_ = name;
}

const char* Marshal_integer::name()
{
  return this->param_name_;
}

Type* Marshal_integer::type()
{
  return (Type*) this->it_;
}

CCSTCompoundStatement* Marshal_integer::accept(TypeVisitor *worker)
{
  return worker->visit(this);
}

Marshal_void::Marshal_void(VoidType *vt)
{
  this->vt_ = vt;
}

void Marshal_void::set_register(int r)
{
  Assert(1 == 0, "Error: this operation is not allowed\n");
}

int Marshal_void::get_register()
{
  Assert(1 == 0, "Error: this operation is now allowed\n");
}

void Marshal_void::set_name(const char *name)
{
  this->param_name_ = name;
}

const char* Marshal_void::name()
{
  return this->param_name_;
}

Type* Marshal_void::get_type()
{
  return (Type*) this->vt_;
}

CCSTCompoundStatement* Marshal_void::accept(TypeVisitor *worker)
{
  return worker->visit(this);
}

Marshal_typedef::Marshal_typedef(Typedef *t, Marshal_type *type)
{
  this->t_ = t;
  this->true_type_ = type;
}

void Marshal_typedef::set_register(int r)
{
  true_type_->set_register(r);
}

int Marshal_typedef::get_register()
{
  return this->true_type_->get_register();
}

void Marshal_typedef::set_name(const char *name)
{
  this->param_name_ = name;
  this->true_type_->set_name(name);
}

Type* Marshal_typedef::type()
{
  return (Type*) this->t_;
}

const char* Marshal_typedef::name()
{
  return this->param_name_;
}

CCSTCompoundStatement* Marshal_typedef::accept(TypeVisitor *worker)
{
  return worker->visit(this);
}

Marshal_pointer::Marshal_pointer(PointerType *pt, Marshal_type *pointer_type)
{
  this->pt_ = pt;
  this->m_type_ = pointer_type;
}

void Marshal_pointer::set_register(int r)
{
  m_type_->set_register(r);
}

int Marshal_pointer::get_register()
{
  return m_type_->get_register();
}

void Marshal_pointer::set_name(const char *name)
{
  //THINK ABOUT THIS
  char* pointer_param_name = (char*) malloc(sizeof(char)*(strlen(name)+5));
  sprintf(pointer_param_name, "%s_tmp", name);
  this->m_type_->set_name(pointer_param_name); 
  this->param_name_ = name; 
}

const char* Marshal_pointer::name()
{
  return this->param_name_;
}

Marshal_type* Marshal_pointer::m_type()
{
  return this->m_type_;
}

Type* Marshal_pointer::type()
{
  return (Type*) this->pt_;
}

CCSTCompoundStatement* Marshal_pointer::accept(TypeVisitor *worker)
{
  return worker->visit(this);
}

